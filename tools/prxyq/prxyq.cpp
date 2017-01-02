/*
	Copyright (C) 2016-2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'prxyq'.

    'prxyq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'prxyq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'prxyq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "registry.h"
#include "watchdog.h"

#include "prxybase.h"

#include "scltool.h"
#include "sclerror.h"

#include "csdcmn.h"
#include "csdbns.h"
#include "cio.h"
#include "crt.h"
#include "epsmsc.h"
#include "err.h"
#include "flf.h"
#include "fnm.h"
#include "log.h"
#include "mtk.h"
#include "lstbch.h"
#include "xpp.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

# define NAME_MC			"prxyq"
# define NAME_LC			"prxyq"
# define NAME_UC			"PRXYQ"
# define WEBSITE_URL		"http://q37.info/"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	

namespace {
	log::rFDriver<> Log_;

	typedef log::rLogRack<> rLogRack_;
}

namespace {
	bso::sBool FreezeFlag_ = false;	// For watchdog testing putpose.

	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}

	typedef fdr::ioflow_driver___<> _flow_driver___;

	typedef flw::standalone_ioflow__<> _flow__;

	class flow___
	: public _flow_driver___,
	  public _flow__
	{
	private:
		flw::sDressedIFlow<> IFlow_;
		flw::sDressedOFlow<> OFlow_;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum) override
		{
			return OFlow_.WriteUpTo( Buffer, Maximum );
		}
		virtual void FDRCommit( bso::sBool Unlock = true ) override
		{
			OFlow_.Commit( Unlock );
		}
		virtual fdr::sTID FDROTake( fdr::sTID Owner ) override
		{
			return OFlow_.ODriver().OTake( Owner );
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			return IFlow_.ReadUpTo( Maximum, Buffer );
		}
		virtual void FDRDismiss( bso::sBool Unlock = true ) override
		{
			IFlow_.Dismiss( Unlock );
		}
		virtual fdr::sTID FDRITake( fdr::sTID Owner ) override
		{
			return IFlow_.IDriver().ITake( Owner );
		}
public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				IFlow_.Dismiss( false );
				OFlow_.Commit( false );
			}

			_flow__::reset( P );
			_flow_driver___::reset( P );

			tol::reset( P, IFlow_, OFlow_ );
		}
		E_CVDTOR( flow___ );
		void Init( void )
		{
			reset();

			_flow_driver___::Init( fdr::ts_Default );
			_flow__::Init( *this );

			// Other initializaiton will be made by other methods.
		}
		void SetIn( fdr::rIDriver &IDriver )
		{
			if ( IFlow_.IsInitialized() )
				qRGnr();

			IFlow_.Init( IDriver );
		}
		void SetOut( fdr::rODriver &ODriver )
		{
			if ( OFlow_.IsInitialized() )
				qRGnr();

			OFlow_.Init( ODriver );
		}
		bso::sBool IsSet( void ) const
		{
			if ( IFlow_.IsInitialized() ) {
				if ( OFlow_.IsInitialized() )
					return true;
				else
					qRGnr();
			} else if ( !OFlow_.IsInitialized() )
				return false;
			else
				qRGnr();
		}
		void Clear( void )
		{
			if ( OFlow_.IsInitialized() )
				_flow__::Commit();

			if ( IFlow_.IsInitialized() )
				_flow__::Dismiss();
		}
	};

	// Pre-declaration.
	class rProxy;

	namespace {
		struct sData_ {
		private:
			tht::rBlocker Blocker_;
		public:
			rProxy *Proxy;
			prxybase::eType Type;
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Blocker_, Proxy );
				Type = prxybase::t_Undefined;
			}
			qCDTOR( sData_ );
			void Init(
				rProxy &Proxy,
				prxybase::eType Type )
			{
				Blocker_.Init();
				this->Proxy = &Proxy;
				this->Type = Type;
			}
			void Wait( void )
			{
				Blocker_.Wait();
			}
			void Unblock( void )
			{
				Blocker_.Unblock();
			}
		};

		void PostInit_( sData_ &Data );

		void PostInit_( void *UP )
		{
		qRFH
		qRFB
			PostInit_( *(sData_ *)UP );
		qRFR
		qRFT
		qRFE( sclmisc::ErrFinal() )
		}
	}

	class rRack_ {
	private:
		flow___ C2S_, S2C_;
		Q37_MRMDF( fdr::rIODriver, C_, Client_ );
		Q37_MRMDF( fdr::rIODriver, S_, Server_ );
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				C2S_.Clear();
				S2C_.Clear();

				if ( Client_ != NULL )
					delete( Client_ );

				if ( Server_ != NULL )
					delete( Server_ );

				P = false;

			}

			tol::reset( P, Client_, Server_, C2S_, S2C_ );
		}
		qCDTOR( rRack_ );
		void Init( void )
		{
			reset();

			C2S_.Init();
			S2C_.Init();
		}
		void Plug(
			fdr::rIODriver *IODriver,
			prxybase::eType Type )
		{
			switch ( Type ) {
			case prxybase::tClient:
				C2S_.SetIn( *IODriver );
				S2C_.SetOut( *IODriver );
				if ( Client_ != NULL )
					qRGnr();
				Client_ = IODriver;
				break;
			case prxybase::tServer:
				S2C_.SetIn( *IODriver );
				C2S_.SetOut( *IODriver );
				if ( Server_ != NULL )
					qRGnr();
				Server_ = IODriver;
				break;
			default:
				qRGnr();
				break;
			}
		}
		flw::sIOFlow &C2S( void )
		{
			return C2S_;
		}
		flw::sIOFlow &S2C( void )
		{
			return S2C_;
		}
		fdr::rODriver &GetCorrespondingODriver( prxybase::eType Type )
		{
			fdr::rODriver *ODriver = NULL;

			switch ( Type ) {
			case prxybase::tClient:
				ODriver= &C_();
				break;
			case prxybase::tServer:
				ODriver = &S_();
				break;
			default:
				qRGnr();
				break;
			}

			return *ODriver;
		}
	};

	class rProxy
	{
	private:
		rRack_ Rack_;
		mtx::rHandler Mutex_;
		bso::sBool FinalDismiss_;	// To ensure that this instance will no more be used before destruction.
		void Process_( flw::ioflow__ &Flow )
		{
		qRFH
			fdr::byte__ Buffer[1024];
			fdr::size__ Size = 0,  Written = 0;
//			err::buffer__ ERRBuffer;
			fdr::sTID
				ITID = fdr::UndefinedTID,
				OTID = fdr::UndefinedTID;
		qRFB
			ITID = Flow.IDriver().ITake( fdr::UndefinedTID );
			OTID = Flow.ODriver().OTake( fdr::UndefinedTID );
			while ( ( Size = Flow.ReadUpTo( sizeof( Buffer ), Buffer ) ) != 0) {
				Written = 0;
				while ( Size > Written )
					Written += Flow.WriteUpTo( Buffer + Written, Size - Written );
				Flow.Commit();
			}

			Flow.Dismiss();
		qRFR
			if ( ERRType == err::tLibrary ) {
				// cio::COut << ">>>" << txf::pad << err::Message( ERRBuffer ) << txf::nl << txf::commit;	// sck.cpp:78 'WSAECONNABORTED'.
				ERRRst();
			}
		qRFT
			if ( ITID != fdr::UndefinedTID )
				Flow.IDriver().ITake( ITID );

			if ( OTID != fdr::UndefinedTID )
				Flow.ODriver().OTake( OTID );
		qRFE( sclmisc::ErrFinal() )
		}
		void Process_( prxybase::eType Type )
		{
			switch ( Type ) {
			case prxybase::tClient:
				Process_( Rack_.C2S() );
				break;
			case prxybase::tServer:
				Process_( Rack_.S2C() );
				break;
			default:
				qRGnr();
				break;
			}
		}
		bso::sBool WaitForOther_( tol::sDelay Delay )
		{
			if ( mtx::TryToLock(Mutex_) ) {
				if ( !mtx::Lock( Mutex_, Delay ) )
					return false;
				mtx::Unlock( Mutex_ );
				FinalDismiss_ = true;
			} else {
				mtx::Unlock( Mutex_ );
				while ( !FinalDismiss_ )
					tht::Defer( 10 );
			}

			return true;
		}
		void AnswerOKAndCommit_( fdr::rODriver &ODriver )
		{
		qRH
			flw::sDressedOFlow<> Flow;
		qRB
			Flow.Init( ODriver );
			prxybase::PutAnswer( prxybase::aOK, Flow );

			Flow.Commit();
		qRR
		qRT
		qRE
		}
	public:
		void reset( bso::bool__ P = true )
		{
			Rack_.reset( P );

			if ( P ) {
				if ( Mutex_ != mtx::UndefinedHandler )
					mtx::Delete( Mutex_ );
			}

			Mutex_ = mtx::UndefinedHandler;

			FinalDismiss_ = false;
		}
		E_CDTOR( rProxy );
		// NOT blocking ; 'PostInit(...)' is the blocking one.
		void Init(
			fdr::rIODriver *IODriver,
			prxybase::eType Type )
		{
			reset();

			Rack_.Init();

			Rack_.Plug( IODriver, Type );

			Mutex_ = mtx::Create();
		}
		// BLOCKING !
		void PostInit( prxybase::eType Type )
		{
			Process_( Type );

			WaitForOther_( 0 );
		}
		void ASyncPostInit( prxybase::eType Type )
		{
			sData_ Data;

			Data.Init( *this, Type );

			AnswerOKAndCommit_( Rack_.GetCorrespondingODriver( Type ) );

			mtk::Launch( PostInit_, &Data );

			Data.Wait();
		}
		void Plug(
			fdr::rIODriver *IODriver,
			prxybase::eType Type,
			tol::sDelay Timeout )
		{
			Rack_.Plug( IODriver, Type );

			ASyncPostInit( prxybase::GetOther( Type ) );

			AnswerOKAndCommit_( *IODriver );

			Process_( Type );

			if ( !WaitForOther_( Timeout ) )
				Rack_.reset();
		}
	};

	namespace {
		void PostInit_( sData_ &Data )
		{
			rProxy &Proxy = *Data.Proxy;
			prxybase::eType Type = Data.Type;

			Data.Unblock();

			Proxy.PostInit( Type );
		}
	}

	using csdbns::cProcessing;

	class dPending
	{
	public:
		struct s {
			rProxy *Proxy;
			str::dString::s Id;
		} &S_;
		str::dString Id;
		dPending( s &S )
		: S_( S ),
		  Id( S.Id )
		{}
		void reset( bso::sBool P = true )
		{
			S_.Proxy = NULL;
			Id.reset( P );
		}
		void plug( str::sHook &Hook )
		{
			Id.plug(Hook );
		}
		void plug( qASd *AS )
		{
			Id.plug( AS );
		}
		const dPending &operator =( const dPending &P )
		{
			S_.Proxy = P.S_.Proxy;

			Id = P.Id;

			return *this;
		}
		void Init(
			rProxy *Proxy,
			const str::dString &Id )
		{
			S_.Proxy = Proxy;

			this->Id.Init( Id );
		}
		rProxy *Proxy( void ) const
		{
			if ( S_.Proxy == NULL )
				qRGnr();

			return S_.Proxy;
		}
	};

	qW( Pending );

	typedef ctn::qMCONTAINERdl( dPending ) dPendings;
	qW( Pendings );

	wPendings PendingClients_, PendingServers_;

	tht::rLocker Locker_;

	void DeletePendings_( dPendings &Pendings)
	{
		sdr::sRow Row = Pendings.First();

		while ( Row != qNIL ) {
			delete Pendings( Row ).Proxy();

			Pendings.Flush();

			Row = Pendings.Next( Row );
		}

		Pendings.reset();
	}

	rProxy *SearchPendingAndRemoveIfExists_(
		const str::string_ &Id,
		dPendings &Pendings )
	{
		rProxy *Proxy = NULL;
	qRH
		sdr::row__ Row = qNIL;
	qRB
		Row = Pendings.First();

		while ( ( Row != qNIL ) && ( Pendings( Row ).Id != Id ) )
			Row = Pendings.Next( Row );

		if ( Row != qNIL ) {
			Proxy = Pendings(Row).Proxy();
			Pendings.Flush();
			Pendings.Remove( Row );
		}

	qRR
	qRT
	qRE
		return Proxy;
	}

	dPendings &GetPendings_( prxybase::eType Type )
	{
		switch ( Type ) {
		case prxybase::tClient:
			return PendingClients_;
			break;
		case prxybase::tServer:
			return PendingServers_;
			break;
		default:
			qRGnr();
			break;
		}

		return *(dPendings *)NULL;	//To avoid a warning.
	}

	namespace {
		void Dismiss_(
			dPendings &Pendings,
			const str::dString &ID )
		{
			sdr::sRow Row = Pendings.Last();
			sdr::sRow Previous = qNIL;

			while ( Row != qNIL ) {
				if ( Pendings( Row ).Id == ID ) {
					Previous = Pendings.Previous( Row );

					delete Pendings( Row ).Proxy();

					Pendings.Remove( Row );

					Row = Previous;
				} else
					Row = Pendings.Previous( Row );
			}
		}
	}

	// Close cponnections frome all server of ID 'ID'.
	void DismissServers_( const str::dString &ID )
	{
		Dismiss_( PendingServers_, ID );
	}

	namespace plug_ {
		void Create_(
			const str::string_ &Id,
			fdr::rIODriver *IODriver,
			prxybase::eType Type )
		{
		qRH
			rProxy *Proxy = NULL;
			wPending Pending;
			tht::rLockerHandler Locker;
		qRB
			Proxy = new rProxy;

			if ( Proxy == NULL )
				qRAlc();

			Pending.Init( Proxy, Id );

			Locker.Init( Locker_ );

			GetPendings_( Type ).Append( Pending );

			Locker.Unlock();

			Proxy->Init( IODriver, Type );
		qRR
		qRT
		qRE
		}

		void Plug_(
			rProxy *Proxy,
			fdr::rIODriver *IODriver,
			prxybase::eType Type,
			tol::sDelay Timeout )
		{
			Proxy->Plug( IODriver, Type, Timeout );	// Blocking until deconnection or 'Timeout' ms.

			delete Proxy;
		}
	}

	void Plug_1_(
		fdr::rIODriver *IODriver,
		tol::sDelay Timeout )
	{
		bso::sBool NoTimeout = true;
	qRH
		prxybase::eType Type = prxybase::t_Undefined;
		str::string Id;
		rProxy *Proxy = NULL;
		rLogRack_ Log;
		flw::sDressedIFlow<> Flow;
	qRB
		Flow.Init( *IODriver );

		Type = prxybase::GetType( Flow );

		Id.Init();
		prxybase::GetId( Flow, Id );

		switch ( Type ) {
		case prxybase::tClient:
			Proxy = SearchPendingAndRemoveIfExists_( Id, GetPendings_( prxybase::tServer ) );
			break;
		case prxybase::tServer:
			Proxy = SearchPendingAndRemoveIfExists_( Id, GetPendings_( prxybase::tClient ) );
			break;
		default:
			qRGnr();
			break;
		}

		Log.Init( Log_ );

		Log << prxybase::GetLabel( Type ) << ' ';

		if ( Proxy == NULL ) {
			Log << "new";
			Log << " (" << Id << ')';
			Log.reset();
			plug_::Create_( Id, IODriver, Type );
		} else {
			Log << "plug";
			Log << " (" << Id << ')';
			Log.reset();
			plug_::Plug_( Proxy, IODriver, Type, Timeout );	// BLOQUANT !!! (d'où pas de 'log' aprés).
		}

	qRR
	qRT
	qRE
	}

	void Dismiss_1_( flw::sIOFlow &Flow )
	{
	qRH
		prxybase::eType Type = prxybase::t_Undefined;
		str::wString ID;
		rLogRack_ Log;
		tht::rLockerHandler Locker;
	qRB
		Type = prxybase::GetType( Flow );

		if ( Type != prxybase::tServer )
			qRGnr();

		ID.Init();
		prxybase::GetId( Flow, ID );

		Locker.Init( Locker_ );

		Log.Init( Log_ );

		Log << "Dimissing (" << ID << ')' << txf::commit;

		DismissServers_( ID );

		prxybase::PutAnswer( prxybase::aOK, Flow );

		Flow.Commit();
	qRR
	qRT
	qRE
	}

	void Ping_1_( flw::ioflow__ &Flow )
	{
		prxybase::PutAnswer( prxybase::aOK, Flow );

		Flow.Commit();
	}

	void Freeze_1_(
		flw::ioflow__ &Flow,
		bso::sBool FromLocalhost )
	{
		if ( FromLocalhost ) {
			FreezeFlag_ = true;
			prxybase::PutAnswer( prxybase::aOK, Flow );
		} else
			prxybase::PutAnswer( prxybase::aForbidden, Flow );


		Flow.Commit();
	}

	void Crash_1_(
		flw::ioflow__ &Flow,
		bso::sBool FromLocalhost )
	{
		if ( FromLocalhost ) {
			tol::Crash();
		} else
			prxybase::PutAnswer( prxybase::aForbidden, Flow );

		Flow.Commit();
	}

	class callback__
	: public cProcessing
	{
	private:
		bso::sBool FromLocalhost_;
		bso::sBool CloseSocketUpstream_;
	protected:
		void *CSDSCBPreProcess(
			fdr::rIODriver *IODriver,
			const ntvstr::char__ *Origin ) override
		{
		qRH
			qCBUFFERr Buffer;
		qRB
			FromLocalhost_ = strncmp( ntvstr::string___( Origin ).UTF8( Buffer ), "127.", 4 ) == 0;
		qRR
		qRT
		qRE
			return NULL;
		}
		csdscb::action__ CSDSCBProcess(
			fdr::rIODriver *IODriver,
			void *UP ) override
		{
		qRH
			prxybase::eType Type = prxybase::t_Undefined;
			str::string Id;
			csdcmn::sVersion Version = csdcmn::UndefinedVersion;
			flw::sDressedIOFlow<> Flow;
			tht::rLockerHandler Locker;
		qRB
			Flow.Init( *IODriver );

			if ( ( Version = csdcmn::GetProtocolVersion( prxybase::ProtocolId, Flow ) ) != prxybase::ProtocolVersion )
				qRGnr();

			CloseSocketUpstream_ = true;

			switch ( prxybase::GetRequest( Flow ) ) {
			case prxybase::rPlug_1:
				 Plug_1_( IODriver, 5000 );
				 CloseSocketUpstream_ = false;
				break;
			case prxybase::rDismiss_1:
				Dismiss_1_( Flow );
				break;
			case prxybase::rPing_1:
				Ping_1_( Flow );
				break;
			case prxybase::rFreeze_1:
				Freeze_1_( Flow, FromLocalhost_ );
				break;
			case prxybase::rCrash_1:
				Crash_1_( Flow, FromLocalhost_ );
				break;
			default:
				qRGnr();
				break;
			}

			Flow.Commit( false );
		qRR
		qRT
		qRE
			return csdscb::aStop;
		}
		virtual bso::sBool CSDSCBPostProcess( void *UP ) override
		{
			return CloseSocketUpstream_;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			FromLocalhost_ = false;
			CloseSocketUpstream_ = false;
		}
		E_CVDTOR( callback__ );
		void Init( void )
		{
			FromLocalhost_ = false;
			CloseSocketUpstream_ = true;
		}
	};
}

namespace {

	void Process_( void )
	{
	qRH
		csdbns::server___ Server;
		callback__ Callback;
	qRB
		Callback.Init();
		Server.Init(sclmisc::MGetU16( registry::parameter::Service ), Callback );
		Server.Process( &FreezeFlag_ );
	qRR
	qRT
	qRE
	}

	void Ping_( void )
	{
		watchdog::Ping();
	}

	void Freeze_( void )
	{
		watchdog::Freeze();
	}

	void Crash_( void )
	{
		watchdog::Crash();
	}
}

#define C( name )\
	else if ( Command == #name )\
		name##_()
		

int scltool::SCLTOOLMain(
	const str::string_ &Command,
	const scltool::oddities__ &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH
qRB
	Log_.Init( cio::COut );

	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( Process );
	C( Ping );
	C( Freeze );
	C( Crash );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;

Q37_GCTOR( prxyq )
{
	PendingClients_.Init();
	PendingServers_.Init();

	Locker_.Init();
}

Q37_GDTOR( prxyq )
{
	DeletePendings_( PendingClients_ );
	DeletePendings_( PendingServers_ );
}

