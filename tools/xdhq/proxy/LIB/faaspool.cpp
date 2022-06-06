/*
	Copyright (C) 2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

	'XDHq' is free software: you can redistribute it and/or modify it
	under the terms of the GNU Affero General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	'XDHq' is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

# include "faaspool.h"

using namespace faaspool;

#include "registry.h"

#include "plugins.h"
#include "prtcl.h"

#include "csdbns.h"
#include "flx.h"
#include "idxbtq.h"
#include "lstbch.h"
#include "lstcrt.h"
#include "mtk.h"
#include "sclm.h"
#include "str.h"
#include "xdhdws.h"

#include <time.h>

namespace {
  namespace protocol_ {
    qCDEF( char *, Id, "4c837d30-2eb5-41af-9b3d-6c8bf01d8dbf" );
    qCDEF( csdcmn::sVersion, LastVersion, 1 );
    // Since V1: user part of the HTML head section is retrieved from backend on each new connection, and no more cached.
  }

	namespace registry_ {
		namespace parameter {
			// Inactivity delay, in minutes, after which a connection is closed.
			// Default value: see 'connection_timeout_::Default_';
			sclr::rEntry ConnectionTimeout_("ConnectionTimeout", registry::parameter::FaaS);
		}

		namespace definition {
			namespace {
					sclr::rEntry FaaS_("FaaS", sclr::Definitions);
					sclr::rEntry Notifications_("Notifications", FaaS_);
					sclr::rEntry LooseNotification_("Notification", Notifications_);
			}

			sclr::rEntry DefaultNotification( "DefaultNotification", Notifications_ );
			sclr::rEntry TaggedNotification( RGSTRY_TAGGING_ATTRIBUTE("flavour"), LooseNotification_);
			sclr::rEntry URL( "URL", FaaS_ );
		}
	}

	qROW( FRow_ );	// Frontend 'Row'.

	typedef lstbch::qLBUNCHd( rGate *, sFRow_ ) dGates_;
	qW( Gates_ );

	qROW( BRow_ );	// Back-end Row.

	// Handling of the connection timeout.
	namespace connection_timeout_ {
		qROW( Row );	// Connection Timeout row

		namespace {
			// Default inactivity delay, in minutes, after which a connection is closed.
			qCDEF(bso::sU8, Default_, 60);
			mtx::rMutex Mutex_ = mtx::Undefined;
			lstbch::qLBUNCHw(sck::rRWDriver *, sRow) Drivers_;
		}

		void CTor(void) {
			time_t Stamp = tol::EpochTime(false);

			if ( difftime(Stamp + 60, Stamp) != 60 )
				qRChk();	// 'time_t' is not expressed in second!

			Drivers_.Init();
			Mutex_ = mtx::Create();
		}

		void DTor(void) {
			Drivers_.reset();

			if ( Mutex_ != mtx::Undefined )
				mtx::Delete(Mutex_);

			Mutex_ = mtx::Undefined;
		}

		sRow Store(sck::rRWDriver &Driver)
		{
			sRow Row = qNIL;
		qRH;
			mtx::rHandle Mutex;
		qRB;
			Mutex.InitAndLock(Mutex_);

			Row = Drivers_.Add(&Driver);
		qRR;
		qRT;
		qRE;
			return Row;
		}

		void Delete(sRow Row)
		{
		qRH;
			mtx::rHandle Mutex;
		qRB;
			Mutex.InitAndLock(Mutex_);

			Drivers_.Delete(Row);
		qRR;
		qRT;
		qRE;
		}

		void Purge(void)
		{
		qRH;
			mtx::rHandle Mutex;
			time_t Stamp = 0;	// 'time_t' must be expressed in seconds (tested in CTor(…)).
			bso::sU32 Timeout = 0;
			sRow Row = qNIL, Next = qNIL;
		qRB;
			Timeout = sclm::OGetU32(registry_::parameter::ConnectionTimeout_, Default_);

			if ( Timeout )
				Stamp = tol::EpochTime( false ) - Timeout * 60;

			Mutex.InitAndLock(Mutex_);

			Row = Drivers_.First();

			while ( Row != qNIL ) {
				sck::rRWDriver *Driver = Drivers_(Row);

				if ( ( Driver != NULL ) && ( Driver->GetEpochTimeStamp() < Stamp ) ) {
					// 'Shutdown()' will cause an error in the thread reading the socket,
					// which, in turn, will close the socket gracefully.
					// The corresponding Driver will also be removed from 'Drivers_'.
					sck::Shutdown(Driver->Socket(), qRPU);

					Drivers_.Store(NULL, Row);

					Next = Drivers_.Next(Row);
					Row = Next;
				} else
					Row = Drivers_.Next(Row);
			}
		qRR;
		qRT;
		qRE;
		}

		bso::sBool WasLate(sRow Row)
		{
			bso::sBool WasLate = false;
		qRH;
			mtx::rHandle Mutex;
		qRB;
			if ( Row != qNIL ) {
				Mutex.InitAndLock(Mutex_);

				WasLate = Drivers_(Row) == NULL;
			}
		qRR;
		qRT;
		qRE;
			return WasLate;
		}
	}
}

namespace faaspool {
  namespace {
    // Head content handling since FaaS protocol v1.
    class rHeadRelay_
    {
    private:
      str::dString *Relay_;
      tht::rBlocker
        Processed_,  // Blocks until the relay data is processed.
        Guard_; // Used on error, to ensure
    public:
      void reset(bso::sBool P = true)
      {
        Relay_ = NULL;

        tol::reset(P, Processed_, Guard_);
      }
      qCDTOR(rHeadRelay_);
      void Init(void)
      {
        Relay_ = NULL;
        tol::Init(Processed_, Guard_);
      }
     void SetRelay(str::dString &Head)
      {
        if ( Relay_ != NULL )
          qRGnr();

        Relay_ = &Head;
      }
      // If returns false, an error occurs and the head could not be retrieved.
      bso::sBool WaitForData(void)
      {
        Processed_.Wait();

        if ( Relay_ != NULL ) {
          Relay_ = NULL;
          return true;
        } else
          return false;
      }
      str::dString &GetRelay(void) const
      {
        // No locking or blocking, as this is called only after a 'GetHead' request.

        if ( Relay_ == NULL )
          qRGnr();

        if ( Relay_->Amount() )
          qRGnr();

        return *Relay_;
      }
      void ReportProcessed(void)
      {
        Processed_.Unblock(); // The blocker is also rearmed.
      }
      bso::sBool IsBusy(void) const
      {
        return Relay_ != NULL;
      }
      bso::sBool Dismiss(void)  // Called on error.
      {
        if ( Relay_ != NULL ) {
          Relay_ = NULL;  // To report the error status.
          Processed_.Unblock();
          Guard_.Wait();
          return true;
        } else
          return false;
      }
      void ReleaseGuard(void)
     {
        Guard_.Unblock();
     }
    };
  }

	class rBackend_
	{
	private:
		// Protects access to 'Gates' and 'PendingDismiss_';
		mtx::rMutex Mutex_;
		bso::sBool PendingDismiss_;
		// Prevents destruction of 'Driver_' until no more client use it.
		tht::rBlocker NoMoreClientBlocker_;
		void InvalidAll_( void )
		{
			sFRow_ Row = Gates.First();

			while ( Row != qNIL ) {
				Gates(Row)->UnsetId();

				Row = Gates.Next( Row );
			}
		}
	public:
		sBRow_ Row;	// Backend row.
		sRow TRow; // Token row.	// Can be 'qNIL' in self-hosted mode (no/empty token).
		fdr::rRWDriver *Driver;	// Is also set to NULL when the backend is no more present.
		wGates_ Gates;
		mtx::rMutex Access;
		tht::rBlocker Switch;
		csdcmn::sVersion ProtocolVersion;
		str::wString
			IP,
			Token,
			HeadCache; // Head content cache on old behavior (FaaS protocol v0).
    rHeadRelay_ HeadRelay;
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Mutex_ != mtx::Undefined)
					mtx::Delete(Mutex_);

				if ( Access != mtx::Undefined )
					mtx::Delete( Access, true );

				if ( TRow != qNIL )
          common::GetCallback().Remove(TRow);

				InvalidAll_();
			}

			Mutex_ = mtx::Undefined;
			PendingDismiss_ = false;
			tol::reset( P, NoMoreClientBlocker_);
			TRow = qNIL;
			Row = qNIL;
			Driver = NULL;
			Access = mtx::Undefined;
			ProtocolVersion = csdcmn::UnknownVersion;
			tol::reset(P, Gates, Switch, IP, Token, HeadCache, HeadRelay);
		}
		qCDTOR( rBackend_ );
		void Init(
			sBRow_ Row,
			sRow TRow,
			fdr::rRWDriver &Driver,
			csdcmn::sVersion ProtocolVersion,
			const str::dString &IP,
			const str::dString &Token,
			const str::dString &Head)
		{
			reset();

			if ( ( ProtocolVersion > 1 ) && Head.Amount() ) // With protocol > 1, 'Head' is no more cached,
                                                      // but retrieved on each new browser connection.
        qRGnr();

			Mutex_ = mtx::Create();
			PendingDismiss_ = false;
			tol::Init(NoMoreClientBlocker_);
			this->Row = Row;
			this->TRow = TRow;
			this->Driver = &Driver;
			Gates.Init();
			Access = mtx::Create();
			Switch.Init();
			this->ProtocolVersion = ProtocolVersion;
			this->IP.Init( IP );
			this->Token.Init(Token);
			this->HeadCache.Init(Head);
			HeadRelay.Init();
		}
		bso::sBool Set(rGate &Gate)
		{
			sFRow_ Row = qNIL;
		qRH
		qRB
			Row = Gates.New();

			if ( *Row > MaxId )
				qRGnr();

			Gates.Store( &Gate, Row );

			Gate.Set(*Row, Driver, &Switch);
		qRR
			if ( Row != qNIL )
				Gates.Remove(Row);

			Row = qNIL;
		qRT
		qRE
			return Row != qNIL;
		}
		void Release(sId Id)
		{
		qRH
			mtx::rHandle Mutex;
		qRB
			Mutex.InitAndLock(Mutex_);

			Gates.Remove((sFRow_)Id);

			if ( PendingDismiss_ )
				if ( Gates.Amount() == 0 )
					NoMoreClientBlocker_.Unblock();
		qRR
		qRT
		qRE
		}
		void WaitUntilNoMoreClient(void)
		{
		qRH
			mtx::rHandle Mutex;
		qRB
			common::GetCallback().QuitAll(TRow);
			Mutex.InitAndLock(Mutex_);

			if ( Gates.Amount() ) {
				PendingDismiss_ = true;
				Mutex.Unlock();
				NoMoreClientBlocker_.Wait();
				Mutex.Lock();	// Otherwise 'Mutex_' could be destroyed before above 'Release' unlocks it.
			}
		qRR
		qRT
		qRE
		}
	};

	mtx::rMutex Mutex_ = mtx::Undefined;
	idxbtq::qINDEXw( sBRow_ ) Index_;
	sBRow_ IRoot_ = qNIL;
	lstbch::qLBUNCHw( rBackend_ *, sBRow_ ) Backends_;
	namespace listener_ {
    csdbns::rListener Channel;
    namespace blocker {
      tht::rBlocker Blocker;  // To avoid above channel destruction while still in use. Currently not in function.
      bso::sBool IsInitialised = false;
    }
	}

	namespace {
		sBRow_ SearchInIndex_(
			const str::dString &Token,
			bso::sSign &Sign)
		{
			sBRow_ Row = IRoot_, Candidate = qNIL;
			bso::sBool Continue = true;

			if ( !mtx::IsLocked( Mutex_ ) )
				qRGnr();

			if ( Row == qNIL)
				qRGnr();

			while ( Continue ) {
				switch ( Sign = str::Compare(Token,Backends_(Row)->Token) ) {
				case -1:
					Candidate = Index_.GetLesser(Row);

					if ( Candidate != qNIL )
						Row = Candidate;
					else
						Continue = false;

					break;
				case 0:
					Continue = false;
					break;
				case 1:
					Candidate = Index_.GetGreater(Row);

					if ( Candidate != qNIL )
						Row = Candidate;
					else
						Continue = false;

					break;
				default:
					qRUnx();
					break;
				}
			}

			return Row;
		}

		void PutInIndex_(
			const str::dString &Token,
			sBRow_ Row )
		{
			bso::sSign Sign = 0;
			sBRow_ IRow = SearchInIndex_(Token, Sign);

			switch ( Sign ) {
			case -1:
				Index_.BecomeLesser(Row,IRow,IRoot_);
				break;
			case 0:
				qRGnr();
				break;
			case 1:
				Index_.BecomeGreater(Row,IRow,IRoot_);
				break;
			default:
				qRUnx();
				break;
			}
		}
	}

	// NOTA : TU : Thread Unsafe ; TS : Thread Safe.

	sBRow_ TUGetBackendRow_( const str::dString &Token )
	{
		bso::sSign Sign = 0;
		sBRow_ Row = IRoot_ == qNIL ? qNIL : SearchInIndex_(Token, Sign);

		if ( Sign != 0 )
			Row = qNIL;

		return Row;
	}

	rBackend_ *TUGetBackend_( const str::dString &Token )
	{
		sBRow_ Row = TUGetBackendRow_( Token );

		if ( Row != qNIL )
			if ( Backends_ (Row)->Driver != NULL )
				return Backends_(Row);
			else
				return NULL;
		else
			return NULL;
	}

	rBackend_ *TSGetBackend_( const str::dString &Token )
	{
		rBackend_ *Backend = NULL;
	qRH;
		mtx::rHandle Mutex;
	qRB;
		Mutex.InitAndLock( Mutex_ );

		Backend = TUGetBackend_( Token );
	qRR;
	qRT;
	qRE;
		return Backend;
	}

	bso::sBool TUGetCachedHead_(  // Old behavior.
		const str::dString &Token,
		str::dString &Head )
	{
		sBRow_ Row = TUGetBackendRow_( Token );

		if ( Row != qNIL ) {
			Head = Backends_(Row)->HeadCache;
			return true;
     } else
			return false;
	}

	// Don't forget to lock the returned backend using 'Access' member!
	bso::sBool TSGetCachedHead_(
		const str::dString &Token,
		str::dString &Head )
	{
		bso::sBool Found = false;
	qRH;
		mtx::rHandle Mutex;
	qRB;
		Mutex.InitAndLock( Mutex_ );

		Found = TUGetCachedHead_( Token, Head );
	qRR;
	qRT;
	qRE;
		return Found;
	}

	void Remove_(sBRow_ Row)
	{
	qRH;
		mtx::rHandle Mutex;
	qRB;
		Mutex.InitAndLock( Mutex_ );

		if ( !Backends_.Exists( Row ) )
			qRGnr();

		Backends_.Remove( Row );

		IRoot_ = Index_.Delete(Row, IRoot_);

		if ( IRoot_ != qNIL )
			IRoot_ = Index_.Balance(IRoot_);
	qRR;
	qRT;
	qRE;
	}

	rBackend_ *Create_(
		fdr::rRWDriver &Driver,
		csdcmn::sVersion ProtocolVersion,
		const str::dString &IP,
		const str::dString &Token,
		const str::dString &Head )
	{
		rBackend_ *Backend = NULL;
	qRH;
		mtx::rHandle Mutex;
		sBRow_ Row = qNIL;
	qRB;
		Mutex.InitAndLock( Mutex_) ;

		Row = TUGetBackendRow_( Token );

		if ( Row == qNIL ) {
			Row = Backends_.New();

			Index_.Allocate(Backends_.Extent());

			if ( IRoot_ == qNIL )
				IRoot_ = Row;
			else
				PutInIndex_(Token, Row);

			if ( (Backend = new rBackend_) == NULL )
				qRAlc();

			Backend->Init(Row, common::GetCallback().Create(Token), Driver, ProtocolVersion, IP, Token, Head);

			Backends_.Store( Backend, Row );
		}
	qRR;
		if ( Backend != NULL )
			delete Backend;
	qRT;
	qRE;
		return Backend;
	}
}

namespace {
	void Get_(
		flw::rRFlow &Flow,
		str::dString &String )
	{
		prtcl::Get( Flow, String );
	}

	void Put_(
		const str::dString &String,
		flw::rWFlow &Flow )
	{
		prtcl::Put( String, Flow );
	}

	void Put_(
		const char *String,
		flw::rWFlow &Flow )
	{
		prtcl::Put( String, Flow );
	}

	namespace token_ {
		plgn::rRetriever<plugins::cToken> PluginRetriever_;

		namespace {
		  namespace {
		    const str::dString &GenerateRandomToken_(str::dString &Token)
		    {
		    qRH;
          flx::rStringWFlow StringFlow;
          cdgb64::rEncodingWFlow EncoderFlow;
          int i = 9;
		    qRB;
          StringFlow.Init(Token);
          EncoderFlow.Init(StringFlow, cdgb64::fURL);

          tol::InitializeRandomGenerator();

          while ( i-- )
            EncoderFlow.Put(rand());
		    qRR;
		    qRT;
		    qRE;
          return Token;
		    }
		  }

			class sToken_
			: public plugins::cToken
			{
			protected:
				plugins::eStatus PLUGINSHandle(
					const str::dString &Raw,
					str::dString &Normalized ) override
				{
					Normalized = Raw;

					if ( Raw.Amount() == 0 ) {
#if 0 // OLD, token is an UUID.
            tol::bUUID UUID;
						Normalized.Append( tol::UUIDGen( UUID ) );
#else
            GenerateRandomToken_(Normalized);
#endif
					} else if ( (Raw.Amount() > 1) && (Raw( 0 ) == '&') )
						Normalized.Remove( Normalized.First() );
					else
						return plugins::sBad;

					return plugins::sOK;
				}
			public:
				void reset(bso::sBool = true ) {}
				qCVDTOR( sToken_ );
				void Init( void ) {}
			} DefaultHandler_;
		}

		plugins::cToken &GetPlugin( void )
		{
			PluginRetriever_.Init();

			if ( sclm::Plug( plugins::TokenPluginTarget, NULL, PluginRetriever_, qRPU ) )
				return PluginRetriever_.Plugin();
			else {
				DefaultHandler_.Init();
				return DefaultHandler_;
			}
		}
	}

	void Notify_(
		flw::rWFlow &Flow,
		const str::dString &Flavour)
	{
	qRH;
		str::wString Notification;
	qRB;
		Notification.Init();

		if (!sclm::OGetValue( rgstry::rTEntry(registry_::definition::TaggedNotification, Flavour), Notification) )
      sclm::OGetValue(registry_::definition::DefaultNotification, Notification);

		prtcl::Put(Notification, Flow);
	qRR;
	qRT;
	qRE;
	}

	csdcmn::sVersion Handshake_(
    fdr::rRWDriver &Driver,
    str::dString &Flavour )
	{
	  csdcmn::sVersion ProtocolVersion = csdcmn::UnknownVersion;
	qRH;
		flw::rDressedRWFlow<> Flow;
	qRB;
		Flow.Init( Driver );

		switch ( ProtocolVersion = csdcmn::GetProtocolVersion(protocol_::Id, protocol_::LastVersion, Flow) ) {
		case csdcmn::UnknownVersion:
			Put_( "\nUnknown FaaS protocol version!\n", Flow );
			Flow.Commit();
			qRGnr();
			break;
		case csdcmn::BadProtocol:
			Put_( "\nUnknown FaaS protocol!\n", Flow );
			Flow.Commit();
			qRGnr();
			break;
		default:
		  if ( ProtocolVersion > protocol_::LastVersion )
        qRUnx();
      Get_(Flow, Flavour);
			Put_( "", Flow );
			Notify_(Flow, Flavour);
			Flow.Commit();
			common::Handshake(Flow);
			break;
		}
	qRR;
	qRT;
	qRE;
    return ProtocolVersion;
	}

	const str::dString &BuildURL_(
		const str::dString &Address,
		const str::dString &RawService,
		const str::dString &Token,
		str::dString &URL )
	{
	qRH
		str::wString TaggedURL, Service;
		tagsbs::tvalues Tags;
	qRB
		TaggedURL.Init();
		sclm::MGetValue(registry_::definition::URL, TaggedURL);

		if ( RawService.Amount()) {
			Service.Init(":");
			Service.Append(RawService);
		} else
			Service.Init();

		Tags.Init();

		Tags.Append("Address", Address, "Service", Service, "Token", Token);

		if ( !tagsbs::SubstituteLongTags(TaggedURL, Tags, URL) )
			qRFwk();
	qRR
	qRE
	qRT
		return URL;
	}

	template <typename string> void Log_(
		const str::dString &IP,
		const str::dString &Token,
		const string &Message )
	{
	qRH;
		logq::rLogRack<> Log;
	qRB;
		Log.Init( common::LogDriver );

		Log << Token << ' ' << IP << ": " <<  Message;
	qRR;
	qRT;
	qRE;
	}

	rBackend_ *CreateBackend_(
		fdr::rRWDriver &Driver,
		csdcmn::sVersion ProtocolVersion,
		const str::dString &IP,
		const str::dString &Flavour ) // 'PYH', 'JRE'…
	{
		rBackend_ *Backend = NULL;
	qRH;
		flw::rDressedRWFlow<> Flow;
		str::wString Token, Head, Address, Misc, ErrorMessageLabel, ErrorMessage, URL, Log;
		plugins::eStatus Status = plugins::s_Undefined;
	qRB;
		Flow.Init( Driver );

		Token.Init();
		Get_( Flow, Token );

		switch ( Status = token_::GetPlugin().Handle(Token) ) {
		case plugins::sOK:
			tol::Init(Head, Address, Misc);
			if ( ProtocolVersion < 1 )
        Get_(Flow, Head);
			Get_(Flow, Address);    // Address to which the toolkit has connected.
			Get_(Flow, Misc);

			if ( ( Backend = Create_( Driver, ProtocolVersion, IP, Token, Head ) ) == NULL ) {
				ErrorMessage.Init();
				sclm::GetBaseTranslation( "TokenAlreadyInUse", ErrorMessage, Token );
				Token.Init();	// To report backend that there is an error.
			}
			break;
		default:
			Token.Init();	// To report an error (see below).
			ErrorMessageLabel.Init( "PLUGINS_" );
			ErrorMessageLabel.Append( plugins::GetLabel( Status ) );
			ErrorMessage.Init();
			sclm::GetBaseTranslation( ErrorMessageLabel, ErrorMessage );
			break;
		}

		Put_( Token, Flow );	// An empty token reports an error, which description is send below.

		if ( Backend == NULL )
			Put_( ErrorMessage, Flow );
		else {
			URL.Init();
			Put_(BuildURL_(Address, str::Empty, Token, URL), Flow);

			bso::pInteger Buffer;
			Log.Init(Flavour);
			Log.Append(" / v");
			Log.Append(bso::Convert(ProtocolVersion, Buffer));
			Log_(IP, Token, Log );
		}
	qRR;
		if ( Backend != NULL )
			delete Backend;

		Backend = NULL;
	qRT;
	qRE;
		return Backend;
	}

  void BroadcastScript_(
    flw::rRFlow &Flow,
    sRow TRow)
  {
  qRH
    str::wString Script;
  qRB
    qRFbd();
    Script.Init();

    prtcl::Get(Flow, Script);

    Flow.Dismiss();

//    common::GetCallback().Broadcast(Script, TRow);
  qRR
  qRT
  qRE
  }

  void BroadcastAction_(
    flw::rRFlow &Flow,
    sRow TRow)
  {
  qRH
    str::wString Action, Id;
  qRB
    tol::Init(Action, Id);

    prtcl::Get(Flow, Action);
    prtcl::Get(Flow, Id);

    Flow.Dismiss();

    common::GetCallback().Broadcast(Action, Id, TRow);
  qRR
  qRT
  qRE
  }

  void Unblock_(const dGates_ &Gates)
  {
    sFRow_ Row = Gates.First();

    while ( Row != qNIL ) {
      Gates(Row)->UnblockAndQuit();

      Row = Gates.Next(Row);
    }
  }

  void Release_(
    flw::rWFlow &Flow,
    sId Id )
  {
    PutId(upstream::ClosingId, Flow);
    PutId(Id, Flow);

    Flow.Commit();
  }

	void HandleSwitching_(
    const str::dString &IP,
		fdr::rRWDriver &Driver,
		rBackend_ &Backend)
	{
	qRH;
		flw::rDressedRWFlow<> Flow;
		sId Id = UndefinedId;
		bso::sBool IsError = false;
		str::wString Input;
	qRB;
		Flow.Init( Driver );

		while ( true ) {
			Id = UndefinedId;

			if ( Flow.EndOfFlow() ) {
				break;
			}

			Id = GetId( Flow, &IsError );

			if ( IsError )
				qRGnr();

			switch( Id ) {
			case UndefinedId:
			  Input.Init();
			  prtcl::Get(Flow, Input);

				if ( Input != xdhcmn::ScriptNameForInform )
          qRGnr();

        Input.Init();
        prtcl::Get(Flow, Input);

        Log_(IP, Backend.Token, Input);
				break;
			case downstream::BroadcastScriptId:
				BroadcastScript_(Flow, Backend.TRow);
				break;
			case downstream::BroadcastActionId:
				BroadcastAction_(Flow, Backend.TRow);
				break;
      case downstream::HeadSendingId:
        prtcl::Get(Flow, Backend.HeadRelay.GetRelay());
        Backend.HeadRelay.ReportProcessed();
        break;
			default:
				if ( !Backend.Gates.Exists( Id ) ) {
          Release_(Flow, Id);
				} else {
          Backend.Gates(Id)->UnblockReading();

          Backend.Switch.Wait();	// Waits until all data in flow red.
				}
				break;
			}
		}
	qRR;
	qRT;
    Backend.HeadRelay.Dismiss();
		Unblock_(Backend.Gates);
	qRE;
	}

	struct gConnectionData_
	{
		sck::sSocket Socket = sck::Undefined;
		csdbns::tIP IP = "";
	};

	void NewConnexion_(
		gConnectionData_ &Data,
		mtk::gBlocker &Blocker )
	{
	qRH;
		sck::sSocket Socket = sck::Undefined;
		csdcmn::sVersion ProtocolVersion = csdcmn::UnknownVersion;
		str::wString IP, Flavour;
		sck::rRWDriver Driver;
		rBackend_ *Backend = NULL;
		connection_timeout_::sRow CTRow = qNIL;
		str::wString Message;
		bso::pInteger IBuffer;
	qRB;
		Socket = Data.Socket;
		IP.Init( Data.IP );

		Blocker.Release();

		CTRow = connection_timeout_::Store(Driver);

		Driver.Init( Socket, false, fdr::ts_Default );

		Flavour.Init();
		ProtocolVersion = Handshake_(Driver, Flavour);

		if ( ( Backend = CreateBackend_(Driver, ProtocolVersion, IP, Flavour) ) != NULL ) {
			HandleSwitching_(IP, Driver, *Backend);	// Does not return until disconnection or error.
		}
	qRR;
		sclm::ErrorDefaultHandling();	// Also resets the error, otherwise the `WaitUntilNoMoreClient()` will lead to a deadlock on next error.
	qRT;
		if ( Backend != NULL ) {
			Message.Init("Quit (");
			Message.Append(bso::Convert(*CTRow, IBuffer));
			Message.Append(')');
			if ( connection_timeout_::WasLate(CTRow) ) {
				Message.Append(" T");
			}

			Log_(IP, Backend->Token, Message);
			Backend->Driver = NULL;	// This signals that the backend is no more present.
			Backend->WaitUntilNoMoreClient();
			Remove_( Backend->Row );
			delete Backend;

			Backend = NULL;
		} else if ( CTRow != qNIL ) {
			Message.Init("Dump (");
			Message.Append(bso::Convert(*CTRow, IBuffer));
			Message.Append(')');
			if ( connection_timeout_::WasLate(CTRow) ) {
				Message.Append(" T");
			}

			Log_(IP, str::wString("(N/A)"), Message);
		} else {
			// Should not happen!
			Message.Init("???");

			Log_(IP, str::wString("(N/A)"), Message);
		}

		if ( CTRow != qNIL )
			connection_timeout_::Delete(CTRow);

		Driver.reset();	// Otherwise it will be done after the destruction of the socket, hence the commit will fail.

		if ( Socket != sck::Undefined ) {
			sck::Close( Socket, err::hUserDefined );	// An error occurring during closing is ignored.
			Socket = sck::Undefined;
		}
	qRE;
	}

	void NewConnexionRoutine_(
		gConnectionData_ &Data,
		mtk::gBlocker &Blocker )
	{
	qRFH;
	qRFB;
		NewConnexion_( Data, Blocker );

		connection_timeout_::Purge();
	qRFR;
	qRFT;
	qRFE(sclm::ErrorDefaultHandling());
	}

	void ListeningRoutine_( void * )
	{
	qRFH;
		gConnectionData_ Data;
	qRFB;
		while ( true ) {
			Data.Socket = listener_::Channel.GetConnection(Data.IP);

			mtk::Launch(NewConnexionRoutine_, Data);
		}
	qRFR;
	qRFT;
    listener_::blocker::Blocker.Unblock();
	qRFE(sclm::ErrorDefaultHandling());
	}
}

void faaspool::_Initialize( void )
{
qRH;
	csdbns::sService Service = csdbns::Undefined;
qRB;
	if ( (Service = sclm::OGetU16( registry::parameter::faas::Service, csdbns::Undefined ) ) != csdbns::Undefined ) {
		listener_::Channel.Init( Service );
    listener_::blocker::Blocker.Init();
    listener_::blocker::IsInitialised = true;

		mtk::RawLaunch(ListeningRoutine_, NULL, true);
	}
qRR;
qRT;
qRE;
}

sRow faaspool::NewSession_(
	const str::dString &Token,
	str::dString &IP,
	rGate &Gate,
	rBackend_ *&Backend)
{
	sRow Row = qNIL;
qRH;
	mtx::rHandle Mutex;
	flw::rDressedWFlow<> Flow;
qRB;
	if ( Backend != NULL)
		qRGnr();

	Backend = TSGetBackend_( Token );

	if ( Backend != NULL ) {
		Mutex.InitAndLock( Backend->Access );

		if ( Backend->Set(Gate) ) {
			Row = Backend->TRow;
			Flow.Init(*Backend->Driver);
			IP.Append(Backend->IP);
			PutId(upstream::CreationId, Flow);	// To signal to the back-end a new connection.
			PutId(Gate.Id(), Flow);	// The id of the new front-end.
			Flow.Commit();
		} else
			Backend = NULL;
	}
qRR;
qRT;
qRE;
	return Row;	// 'qNIL' report an error, as in 'FaaS' mode, the token can not be empty.
}

namespace {
  bso::sBool RetrieveHeadRemotely_(
    const str::dString &Token,
    str::dString &Head,
    csdcmn::sVersion &ProtocolVersion)
  {
    bso::sBool Success = false;
  qRH;
    mtx::rHandle Mutex;
    flw::rDressedWFlow<> Flow;
    rBackend_ *Backend = NULL;
  qRB;
    Backend = TSGetBackend_( Token );

    if ( Backend != NULL ) {
      Mutex.InitAndLock(Backend->Access);

      if ( Backend->HeadRelay.IsBusy() )
        qRGnr();

      ProtocolVersion = Backend->ProtocolVersion;

      if ( ProtocolVersion >= 1 ) {
        Flow.Init(*Backend->Driver);
        Backend->HeadRelay.SetRelay(Head);
        PutId(upstream::HeadRetrievingId, Flow);
        Flow.Commit();
        Success = Backend->HeadRelay.WaitForData();
      }
    }
  qRR;
  qRT;
    if ( !Success && ( Backend != NULL ) )
      Backend->HeadRelay.ReleaseGuard();
  qRE;
    return Success;
  }
}

void faaspool::rRWDriver::Release_(void)
{
qRH
	flw::rDressedWFlow<> Flow;
qRB
	Flow.Init(D_());

	::Release_(Flow, Gate_.Id());

	Flow.reset();	// Commits and frees the underlying driver, or the below 'Release' will block.
qRR
	qRRst();
qRT
	B_().Release(Gate_.Id());
	Gate_.UnsetId();
	Backend_ = NULL;
qRE
}

bso::sBool faaspool::GetHead(
    const str::dString &Token,
    str::dString &Head )
{
  csdcmn::sVersion ProtocolVersion = csdcmn::UnknownVersion;

  if ( !RetrieveHeadRemotely_(Token, Head, ProtocolVersion) )
    if ( ProtocolVersion < 1 )  // Falling back to old behavior
      return TSGetCachedHead_(Token,Head);
    else
      return false;
  else
    return true;
}

qGCTOR(faaspool)
{
qRFH;
qRFB;
	Mutex_ = mtx::Create();
	Backends_.Init();
	Index_.Init();
	connection_timeout_::CTor();
qRFR;
qRFT;
qRFE(sclm::ErrorDefaultHandling());
}

qGDTOR(faaspool)
{
	if ( Mutex_ != mtx::Undefined )
		mtx::Delete( Mutex_, true );

	sBRow_ Row = Backends_.First();

	while ( Row != qNIL ) {
		delete Backends_( Row );

		Row = Backends_.Next( Row );
	}

	connection_timeout_::DTor();
  Backends_.reset();

  if ( listener_::blocker::IsInitialised && false ) // To activate when listener will be aware of interruption (SIGTERM, for example).
    listener_::blocker::Blocker.Wait();
}

