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
#include "rnd.h"
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
	namespace listener_ {
    csdbns::rListener Channel;
    namespace blocker {
      tht::rBlocker Blocker;  // To avoid above channel destruction while still in use. Currently not in function.
      bso::sBool IsInitialised = false;
    }
	}

	bso::sBool GetCachedHead_(  // FaaS Protocol <1.
		const str::dString &Token,
		str::dString &Head)
  {
    faasbckd::rBackend *Backend = NULL;
  qRH;
    faasbckd::hGuard Guard;
  qRB;
    Backend = faasbkds::GetBackend(Token, Guard);

    if ( Backend != NULL )
      Head = Backend->HeadCache;
  qRR;
  qRT;
  qRE;
    return Backend != NULL;
  }

	faasbckd::rBackend *Create_(
		fdr::rRWDriver &Driver,
		csdcmn::sVersion ProtocolVersion,
		const str::dString &IP,
		const str::dString &Token,
		const str::dString &Head )
	{
		faasbckd::rBackend *Backend = NULL;
	qRH;
		faasbckd::sRow Row = qNIL;
	qRB;
		Backend = faasbkds::Create(Token, Row);

		if ( Backend != NULL ) {
      if ( Row == qNIL )
        qRGnr();
			Backend->Init(Row, common::GetCallback().Create(Token), Driver, ProtocolVersion, IP, Token, Head);
		}
	qRR;
		if ( Backend != NULL ) {
      faasbkds::Remove(Row);
			delete Backend;
		}
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
          rnd::hGuard Guard;
		    qRB;
          StringFlow.Init(Token);
          EncoderFlow.Init(StringFlow, cdgb64::fURL);

          rnd::LockRand(Guard);

          while ( i-- )
            EncoderFlow.Put(rnd::TURand());
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

	faasbckd::rBackend *CreateBackend_(
		fdr::rRWDriver &Driver,
		csdcmn::sVersion ProtocolVersion,
		const str::dString &IP,
		const str::dString &Flavour ) // 'PYH', 'JRE'…
	{
		faasbckd::rBackend *Backend = NULL;
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
    faas::sRow Row)
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
    faas::sRow Row)
  {
  qRH
    str::wString Action, Id;
  qRB
    tol::Init(Action, Id);

    prtcl::Get(Flow, Action);
    prtcl::Get(Flow, Id);

    Flow.Dismiss();

    common::GetCallback().Broadcast(Action, Id, Row);
  qRR
  qRT
  qRE
  }

  namespace {
    void Unblock_(const faasgate::dGates &Gates)
    {
      faasgate::sRow Row = Gates.First();

      while ( Row != qNIL ) {
        Gates(Row)->UnblockAndQuit();

        Row = Gates.Next(Row);
      }
    }
  }


  void UnblockGates_(faasbckd::rBackend &Backend)
  {
  qRH;
    faasgate::hGuard Guard;
  qRB;
    Unblock_(Backend.LockAndGetGates(Guard));
  qRR;
  qRT;
  qRE;
  }

  void Release_(
    flw::rWFlow &Flow,
    faas::sId Id )
  {
    PutId(faas::upstream::ClosingId, Flow);
    PutId(Id, Flow);

    Flow.Commit();
  }

	void HandleSwitching_(
    const str::dString &IP,
		fdr::rRWDriver &Driver,
		faasbckd::rBackend &Backend)
	{
	qRH;
		flw::rDressedRWFlow<> Flow;
    faasgate::hGuard GatesGuard;
		faas::sId Id = faas::UndefinedId;
		bso::sBool IsError = false;
		str::wString Input;
	qRB;
		Flow.Init( Driver );

		while ( true ) {
			Id = faas::UndefinedId;

			if ( Flow.EndOfFlow() ) {
				break;
			}

			Id = GetId( Flow, &IsError );

			if ( IsError )
				qRGnr();

			switch( Id ) {
      case faas::UndefinedId:
        Input.Init();
        prtcl::Get(Flow, Input);

        if ( Input != xdhcmn::ScriptNameForInform )
          qRGnr();

        Input.Init();
        prtcl::Get(Flow, Input);

        Log_(IP, Backend.Token, Input);
        break;
      case faas::downstream::BroadcastScriptId:
        BroadcastScript_(Flow, Backend.TRow);
        break;
      case faas::downstream::BroadcastActionId:
        BroadcastAction_(Flow, Backend.TRow);
        break;
      case faas::downstream::HeadSendingId:
        prtcl::Get(Flow, Backend.Depot.GetReceptacle());
        Backend.Depot.ReportContentAvailable();
        break;
      default:
        faasgate::dGates &Gates = Backend.LockAndGetGates(GatesGuard);

        if ( !Gates.Exists( Id ) ) {
          Backend.UnlockGates(GatesGuard);
          Release_(Flow, Id);
        } else {
          Gates(Id)->UnblockReading();
          Backend.UnlockGates(GatesGuard);
          Backend.Switch.Wait();	// Waits until all data in flow red.
        }
        break;
      }
		}
	qRR;
	qRT;
    Backend.Depot.Dismiss();
		UnblockGates_(Backend);
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
		faasbckd::rBackend *Backend = NULL;
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
			faasbkds::Remove(Backend->Row);

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

void faaspool::Initialize( void )
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

faas::sRow faaspool::NewSession_(
	const str::dString &Token,
	str::dString &IP,
	faasgate::rGate &Gate,
	faasbckd::rBackend *&Backend)
{
	faas::sRow Row = qNIL;
qRH;
	faasbckd::hGuard Guard;
	flw::rDressedWFlow<> Flow;
qRB;
	if ( Backend != NULL)
		qRGnr();

	Backend = faasbkds::GetBackend(Token, Guard);

	if ( Backend != NULL ) {
		if ( Backend->Set(Gate) ) {
			Row = Backend->TRow;
			Flow.Init(*Backend->Driver);
			IP.Append(Backend->IP);
			PutId(faas::upstream::CreationId, Flow);	// To signal to the back-end a new connection.
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
    faasbckd::hGuard Guard;
    flw::rDressedWFlow<> Flow;
    faasbckd::rBackend *Backend = NULL;
  qRB;
    Backend = faasbkds::GetBackend(Token, Guard);

    if ( Backend != NULL ) {
      if ( Backend->Depot.IsBusy() )
        qRGnr();

      ProtocolVersion = Backend->ProtocolVersion;

      if ( ProtocolVersion >= 1 ) {
        Flow.Init(*Backend->Driver);
        Backend->Depot.SetReceptacle(Head);
        PutId(faas::upstream::HeadRetrievingId, Flow);
        Flow.Commit();
        Success = Backend->Depot.WaitForContent();
      }
    }
  qRR;
  qRT;
    if ( !Success && ( Backend != NULL ) ) {
      Flow.reset();
      Backend->Depot.Release();  // Backend waits for this before being destroyed.
    }
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
      return GetCachedHead_(Token,Head);
    else
      return false;
  else
    return true;
}

qGCTOR(faaspool)
{
qRFH;
qRFB;
	connection_timeout_::CTor();
qRFR;
qRFT;
qRFE(sclm::ErrorDefaultHandling());
}

qGDTOR(faaspool)
{
	connection_timeout_::DTor();

  if ( listener_::blocker::IsInitialised && false ) // To activate when listener will be aware of interruption (SIGTERM, for example).
    listener_::blocker::Blocker.Wait();
}

