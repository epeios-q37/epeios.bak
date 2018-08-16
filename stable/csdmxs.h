/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

// Client-Server Devices MuXing Server

#ifndef CSDMXS__INC
# define CSDMXS__INC

# define CSDMXS_NAME		"CSDMXS"

# if defined( E_DEBUG ) && !defined( CSDMXS_NODBG )
#  define CSDMXS_DBG
# endif

# include "csdmxb.h"
# include "csdscb.h"

# include "flf.h"

# include "sdr.h"
# include "lstbch.h"

# include "err.h"

namespace csdmxs {
	using namespace csdmxb;

	using csdscb::cProcessing;

	qENUM( Log ) {
		lNew,
		lStore,
		lTestAndGet,
		lDelete,
		l_amount,
		l_Undefined
	};

	const char *GetLabel( eLog Log );

	typedef void *sUserPointer;

	class cLog {
	protected:
		virtual void CSDMXSLog(
			eLog Log,
			sId Id,
			void *UP,
			sdr::size__ Amount ) = 0;
	public:
		void Log(
			eLog Log,
			sId Id,
			sUserPointer UP,
			sdr::size__ Amount )
		{
			CSDMXSLog( Log, Id, UP, Amount );
		}
	};

	class rLogCallback
	: public cLog
	{
	private:
		flf::file_oflow___ Flow_;
		txf::text_oflow__ TFlow_;
	protected:
		virtual void CSDMXSLog(
			eLog Log,
			sId Id,
			void *UP,
			sdr::size__ Amount ) override
		{
			TFlow_ << GetLabel(Log) << " ("  << Id << ") : " << UP << " - " << Amount << txf::nl << txf::commit;
		}
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, TFlow_, Flow_ );
		}
		qCVDTOR( rLogCallback );
		void Init( const fnm::rName &Name )
		{
			Flow_.Init( Name );
			TFlow_.Init( Flow_ );
		}
	};


	typedef lstbch::qLBUNCHdl( sUserPointer ) dUserPointers;

	class dCore
	{
	private:
		bso::bool__ Exists_( sId Id ) const
		{
			return UPs.Exists( Id );
		}
		void Log_(
			eLog Log,
			sId Id,
			void *UP ) const
		{
			if ( S_.Log.Callback != NULL ) {
qRH
qRB
				mtx::Lock( S_.Log.Mutex );
				S_.Log.Callback->Log( Log, Id, UP, UPs.Amount() );
qRR
qRT
				mtx::Unlock( S_.Log.Mutex );
qRE
			}
		}
	public:
		struct s
		{
			dUserPointers::s UPs;
			mtx::handler___ Mutex;
			struct log__ {
				cLog *Callback;
				mtx::handler___ Mutex;
			} Log;
		} &S_;
		dUserPointers UPs;
		dCore ( s &S )
		: S_( S ),
		  UPs( S.UPs )
		{}
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( S_.Mutex != mtx::Undefined )
					mtx::Delete( S_.Mutex );
			}

			UPs.reset( P );
			S_.Mutex = mtx::Undefined;
			S_.Log.Mutex = mtx::Undefined;
			S_.Log.Callback = NULL;

		}
		void plug( qASd *AS )
		{
			UPs.plug( AS );
		}
		dCore &operator =( const dCore &C )
		{
			qRFwk();

			return *this;
		}
		void Init( cLog *LogCallback )
		{
			reset();

			UPs.Init();
			S_.Mutex = mtx::Create();
			S_.Log.Mutex = mtx::Create();
			S_.Log.Callback = LogCallback;
		}
		sId New( void )
		{
			mtx::Lock( S_.Mutex );

			sdr::row__ Row = UPs.New();

			if ( *Row >= BSO_U16_MAX )
				qRLmt();

			mtx::Unlock( S_.Mutex );

			Log_( lNew, (sId)*Row, NULL );

			return (sId)*Row;
		}
		void Store(
			sUserPointer UP,
			sId Id )
		{
#ifdef CSDMXS_DBG
			if ( Id == CSDMXB_UNDEFINED )
				qRFwk();
#endif
			mtx::Lock( S_.Mutex );
			UPs.Store( UP, Id );
			mtx::Unlock( S_.Mutex );

			Log_( lStore, Id, UP );
		}
		bso::bool__ Exists( sId Id ) const
		{
			bso::bool__ Result;

			mtx::Lock( S_.Mutex );

			Result =  Exists_( Id );

			mtx::Unlock( S_.Mutex );

			return Result;
		}
		bso::bool__ TestAndGet(
			sId Id,
			void *&UP ) const
		{
			bso::bool__ Result = false;

			mtx::Lock( S_.Mutex );

			Result = Exists_( Id );

			if ( Result )
				UP = UPs( Id );

			mtx::Unlock( S_.Mutex );

			Log_( lTestAndGet, Id, UP );

			return Result;
		}
		bso::bool__ Delete( sId Id )
		{
			bso::bool__ Result = false;

			mtx::Lock( S_.Mutex );

			Result = Exists_( Id );

			if ( Result )
				UPs.Delete( Id );

			mtx::Unlock( S_.Mutex );

			Log_( lDelete, Id, NULL );

			return Result;
		}

	};

	qW( Core );

	class rData_
	{
	public:
		void reset( bso::sBool P = true )
		{
		}
		qCDTOR( rData_ );
		void Init( void )
		{
		}
	};


	class rProcessing
	: public cProcessing
	{
	private:
		wCore Core_;
		qRMV( cProcessing,  C_, Callback_ );
		ntvstr::string___ _Origin;
		void _Clean( void );	// Appelle le 'PostProcess' pour tous les objets utilisateurs.
	protected:
		virtual void *CSDSCBPreProcess(
			fdr::rRWDriver *RWDriver,
			const ntvstr::char__ *Origin ) override
		{
			_Origin.Init( Origin );

			rData_ *Data = new rData_;

			if ( Data == NULL )
				qRAlc();

			Data->Init();

			return Data;
		}
		virtual csdscb::action__ CSDSCBProcess(
			fdr::rRWDriver *IODriver,
			void *UP ) override
		{
			csdscb::action__ MainAction = csdscb::aContinue;
		qRH
			sId Id = CSDMXB_UNDEFINED;
			csdscb::action__ Action = csdscb::aContinue;
			bso::sBool OwnerShipTaken = false;
			flw::sDressedRWFlow<> Flow;
		qRB
#ifdef CSDMXS_DBG
			if ( UP == NULL )
				qRFwk();
#endif

			void *SUP = NULL;	// Server UP.

			Flow.Init( *IODriver );

			Id = GetId( Flow );

			if ( Id == CSDMXB_UNDEFINED ) {
				Id = Core_.New();
				PutId( Id, Flow );
				Flow.Commit( false );
				SUP = Callback_->PreProcess( IODriver, _Origin );
				if ( OwnerShipTaken )
					qRFwk();
				Core_.Store( SUP, Id );
#if 0
				Action = Callback_->Process( IODriver, SUP );
#else
				Action = csdscb::aContinue;
#endif
			} else if ( Id == CSDMXB_PING ) {
				Flow.Put( (flw::byte__)0 );
				Flow.Commit();
			} else if ( Id == CSDMXB_CLOSE ) {
				MainAction = csdscb::aStop;
			} else if ( !Core_.TestAndGet( Id, SUP ) ) {
				Flow.Put( (flw::byte__)-1 );
				Flow.Commit();
				Action = csdscb::aStop;
			} else {
				Flow.Put( 0 );
				Flow.Commit( false );
				Action = Callback_->Process( IODriver, SUP );
			}

			switch ( Action ) {
			case csdscb::aContinue:
				break;
			case csdscb::aStop:
				Callback_->PostProcess( SUP );
				if ( Id < CSDMXB_RESERVED )
					Core_.Delete( Id );
				break;
			default:
				qRFwk();
				break;
			}
		qRR
			MainAction = csdscb::aStop;
		qRT
		qRE
			return MainAction;	// Always 'aContinue', because we want to keep the physical connection, even when the logical connection was closed.
		}
		virtual bso::sBool CSDSCBPostProcess( void *UP ) override
		{
			if ( UP == NULL )
				qRFwk();

			rData_ *Data = (rData_ *)UP;

			delete Data;

			return false;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				_Clean();

			Core_.reset( P );
			Callback_ = NULL;
			_Origin.reset( P );
		}
		qCVDTOR( rProcessing );
		void Init(
			cProcessing &Callback,
			cLog *LogCallback = NULL )
		{
			reset();

			Core_.Init( LogCallback );
			Callback_ = &Callback;
			_Origin.Init();
		}
	};

# if 0
	// Pour l'utilisation en tant que service Windows, voir csdbns::server__'. 
	class server___
	{
	private:
		csdbns::server___ _Server;
		callback___ _Callback;
	public:
		void reset( bso::bool__ P = true )
		{
			_Callback.reset( P );
			_Server.reset( P );
		}
		E_CDTOR( server___ );
		void Init(
			port__ Port,
			_callback__ &Callback,
			log_callback__ *LogCallback = NULL )
		{
			_Callback.Init( Callback, LogCallback );

			_Server.Init( Port, _Callback );
		}
		void Init(
			port__ Port,
			_callback__ &Callback,
			log_callback__ &LogCallback )
		{
			Init( Port, Callback, &LogCallback );
		}
		bso::bool__ LaunchService( const char *ServiceName )
		{
			return _Server.LaunchService( ServiceName );
		}
		void Process( sck::duration__ Timeout = SCK_INFINITE )
		{
			_Server.Process( Timeout );
		}
	};
# endif

}

#endif
