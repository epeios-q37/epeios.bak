/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

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
		Q37_MRMDF( flw::iflow__, IF_, IFlow_ );
		Q37_MRMDF( flw::oflow__, OF_, OFlow_ );
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum) override
		{
			return OF_().WriteUpTo( Buffer, Maximum );
		}
		virtual void FDRCommit( void ) override
		{
			OF_().Commit();
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			return IF_().ReadUpTo( Maximum, Buffer );
		}
		virtual void FDRDismiss( void ) override
		{
			IF_().Dismiss();
		}
public:
		void reset( bso::bool__ P = true )
		{
			_flow__::reset( P );
			_flow_driver___::reset( P );
			IFlow_ = NULL;
			OFlow_ = NULL;
		}
		E_CVDTOR( flow___ );
		void Init( void )
		{
			reset();

			_flow_driver___::Init( fdr::ts_Default );
			_flow__::Init( *this );

			// Other initializaiton will be made by other methods.
		}
		void SetIn( flw::iflow__ &Flow )
		{
			if ( IFlow_ != NULL )
				qRGnr();

			IFlow_ = &Flow;
		}
		void SetOut( flw::oflow__ &Flow )
		{
			if ( OFlow_ != NULL )
				qRGnr();

			OFlow_ = &Flow;
		}
		bso::sBool IsSet( void ) const
		{
			if ( IFlow_ != NULL ) {
				if ( OFlow_ != NULL )
					return true;
				else
					qRGnr();
			} else if ( OFlow_ == NULL )
				return false;
			else
				qRGnr();
		}
		void Clear( void )
		{
			if ( OFlow_ != NULL )
				_flow__::Commit();

			if ( IFlow_ != NULL )
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
		Q37_MRMDF( flw::sIOFlow, C_, Client_ );
		Q37_MRMDF( flw::sIOFlow, S_, Server_ );
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
			flw::ioflow__ *Flow,
			prxybase::eType Type )
		{
			switch ( Type ) {
			case prxybase::tClient:
				C2S_.SetIn( *Flow );
				S2C_.SetOut( *Flow );
				if ( Client_ != NULL )
					qRGnr();
				Client_ = Flow;
				break;
			case prxybase::tServer:
				S2C_.SetIn( *Flow );
				C2S_.SetOut( *Flow );
				if ( Server_ != NULL )
					qRGnr();
				Server_ = Flow;
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
		flw::sOFlow &GetCorrespondingOFlow( prxybase::eType Type )
		{
			flw::sOFlow *Flow = NULL;

			switch ( Type ) {
			case prxybase::tClient:
				Flow = &C_();
				break;
			case prxybase::tServer:
				Flow = &S_();
				break;
			default:
				qRGnr();
				break;
			}

			return *Flow;
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
		qRFB
			while ( ( Size = Flow.ReadUpTo( sizeof( Buffer ), Buffer ) ) != 0) {
				Written = 0;
				while ( Size > Written )
					Written += Flow.WriteUpTo( Buffer + Written, Size - Written );
				Flow.Commit();
			}
		qRFR
			if ( ERRType == err::tLibrary ) {
				// cio::COut << ">>>" << txf::pad << err::Message( ERRBuffer ) << txf::nl << txf::commit;	// sck.cpp:78 'WSAECONNABORTED'.
				ERRRst();
			}
		qRFT
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
		void WaitForOther_( void )
		{
			if ( mtx::TryToLock(Mutex_) ) {
				mtx::Lock( Mutex_ );
				mtx::Unlock( Mutex_ );
				FinalDismiss_ = true;
			} else {
				mtx::Unlock( Mutex_ );
				while ( !FinalDismiss_ )
					tht::Defer( 10 );
			}
		}
		void AnswerOKAndCommit_( flw::sOFlow &Flow )
		{
			prxybase::PutAnswer( prxybase::aOK, Flow );

			Flow.Commit();
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
			flw::ioflow__ *Flow,
			prxybase::eType Type )
		{
			reset();

			Rack_.Init();

			Rack_.Plug( Flow, Type );

			Mutex_ = mtx::Create();
		}
		// BLOCKING !
		void PostInit( prxybase::eType Type )
		{
			Process_( Type );

			return WaitForOther_();
		}
		void ASyncPostInit( prxybase::eType Type )
		{
			sData_ Data;

			Data.Init( *this, Type );

			AnswerOKAndCommit_( Rack_.GetCorrespondingOFlow( Type ) );

			mtk::Launch( PostInit_, &Data );

			Data.Wait();
		}
		void Plug(
			flw::ioflow__ *Flow,
			prxybase::eType Type )
		{
			Rack_.Plug( Flow, Type );

			ASyncPostInit( prxybase::GetOther( Type ) );

			AnswerOKAndCommit_( *Flow );

			Process_( Type );

			return WaitForOther_();
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
		void plug( str::cHook &Hook )
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
		namespace {
			void Create_(
				const str::string_ &Id,
				flw::ioflow__ *Flow,
				prxybase::eType Type )
			{
			qRH
				rProxy *Proxy = NULL;
				wPending Pending;
			qRB
				Proxy = new rProxy;

				if ( Proxy == NULL )
					qRAlc();

				Pending.Init( Proxy, Id );

				GetPendings_( Type ).Append( Pending );

				Locker_.Unlock();	// Locked by caller.

				Proxy->Init( Flow, Type );
			qRR
			qRT
			qRE
			}

			void Plug_(
				rProxy *Proxy,
				flw::ioflow__ *Flow,
				prxybase::eType Type )
			{
				Locker_.Unlock();	// Locked by caller.

				Proxy->Plug( Flow, Type );	// Blocking until deconnection.

				delete Proxy;
			}
		}

		void A( flw::ioflow__ *Flow )
		{
		qRH
			prxybase::eType Type = prxybase::t_Undefined;
			str::string Id;
			rProxy *Proxy = NULL;
			rLogRack_ Log;
		qRB
			Type = prxybase::GetType( *Flow );

			Id.Init();
			prxybase::GetId( *Flow, Id );

			Locker_.Lock();

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
				Create_( Id, Flow, Type );
			} else {
				Log << "plug";
				Log << " (" << Id << ')';
				Log.reset();
				Plug_( Proxy, Flow, Type );	// BLOQUANT !!! (d'où pas de 'log' aprés).
			}

		qRR
		qRT
			Locker_.UnlockIfLocked();
		qRE
		}
	}

	void Plug_(
		csdcmn::sVersion Version,
		flw::sIOFlow *Flow )
	{
		switch ( Version ) {
		default:
			return plug_::A( Flow );
			break;
#pragma warning( suppress: 4065 )	//  switch statement contains 'default' but no 'case' labels.
		}
	}

	namespace dismiss_ {
		void A( flw::sIOFlow &Flow )
		{
		qRH
			prxybase::eType Type = prxybase::t_Undefined;
			str::wString ID;
			rLogRack_ Log;
		qRB
			Type = prxybase::GetType( Flow );

			if ( Type != prxybase::tServer )
				qRGnr();

			ID.Init();
			prxybase::GetId( Flow, ID );

			Locker_.Lock();

			Log.Init( Log_ );

			Log << "Dimissing (" << ID << ')' << txf::commit;

			DismissServers_( ID );

			prxybase::PutAnswer( prxybase::aOK, Flow );

			Flow.Commit();
		qRR
		qRT
			Locker_.UnlockIfLocked();
		qRE
		}
	}

	void Dismiss_(
		csdcmn::sVersion Version,
		flw::sIOFlow &Flow )
	{
		switch ( Version ) {
		default:
			return dismiss_::A( Flow );
			break;
#pragma warning( suppress: 4065 )	//  switch statement contains 'default' but no 'case' labels.
		}
	}

	namespace ping_ {
		void A( flw::ioflow__ &Flow )
		{
			prxybase::PutAnswer( prxybase::aOK, Flow );

			Flow.Commit();
		}
	}

	void Ping_(
		csdcmn::sVersion Version,
		flw::sIOFlow &Flow )
	{
		switch ( Version ) {
		default:
			return ping_::A( Flow );
			break;
#pragma warning( suppress: 4065 )	//  switch statement contains 'default' but no 'case' labels.
		}
	}

	namespace freeze_{
		void A(
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
	}

	void Freeze_(
		csdcmn::sVersion Version,
		flw::sIOFlow &Flow,
		bso::sBool FromLocalhost )
	{
		switch ( Version ) {
		default:
			return freeze_::A( Flow, FromLocalhost );
			break;
#pragma warning( suppress: 4065 )	//  switch statement contains 'default' but no 'case' labels.
		}
	}

	namespace crash_ {
		void A(
			flw::ioflow__ &Flow,
			bso::sBool FromLocalhost )
		{
			if ( FromLocalhost ) {
				tol::Crash();
			} else
				prxybase::PutAnswer( prxybase::aForbidden, Flow );

			Flow.Commit();
		}
	}

	void Crash_(
		csdcmn::sVersion Version,
		flw::sIOFlow &Flow,
		bso::sBool FromLocalhost )
	{
		switch ( Version ) {
		default:
			return crash_::A( Flow, FromLocalhost );
			break;
#pragma warning( suppress: 4065 )	//  switch statement contains 'default' but no 'case' labels.
		}
	}

	class callback__
	: public cProcessing
	{
	private:
		bso::sBool FromLocalhost_;
	protected:
		void *CSDSCBPreProcess(
			flw::sIOFlow *Flow,
			const ntvstr::char__ *Origin,
			bso::sBool *OwnerShipTaken ) override
		{
		qRH
			qCBUFFERr Buffer;
		qRB
			FromLocalhost_ = strncmp( ntvstr::string___( Origin ).UTF8( Buffer ), "127.", 4 ) == 0;

			*OwnerShipTaken = true;
		qRR
		qRT
		qRE
			return NULL;
		}
		csdscb::action__ CSDSCBProcess(
			flw::sIOFlow *Flow,
			void *UP ) override
		{
		qRH
			prxybase::eType Type = prxybase::t_Undefined;
			str::string Id;
			csdcmn::sVersion Version = csdcmn::UndefinedVersion;
			bso::sBool DeleteFlow = true;
		qRB
			if ( ( Version = csdcmn::GetProtocolVersion( prxybase::ProtocolId, *Flow ) ) != prxybase::ProtocolVersion )
				qRGnr();

			switch ( prxybase::GetRequest( *Flow ) ) {
			case prxybase::rPlug:
				Plug_( Version, Flow );
				DeleteFlow = false;
				break;
			case prxybase::rDismiss:
				Dismiss_( Version, *Flow );
				break;
			case prxybase::rPing:
				Ping_( Version, *Flow );
				break;
			case prxybase::rFreeze:
				Freeze_( Version, *Flow, FromLocalhost_ );
				break;
			case prxybase::rCrash:
				Crash_( Version, *Flow, FromLocalhost_ );
				break;
			default:
				qRGnr();
				break;
			}
		qRR
			Locker_.UnlockIfLocked();
		qRT
			if ( DeleteFlow )
				delete Flow;
		qRE
			return csdscb::aStop;
		}
		virtual void CSDSCBPostProcess( void *UP ) override
		{}
	public:
		void reset( bso::bool__ P = true )
		{
			FromLocalhost_ = false;
		}
		E_CVDTOR( callback__ );
		void Init( void )
		{
			FromLocalhost_ = false;
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

