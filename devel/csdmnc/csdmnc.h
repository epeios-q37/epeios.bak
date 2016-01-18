/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#ifndef CSDMNC__INC
# define CSDMNC__INC

# define CSDMNC_NAME		"CSDMNC"

# if defined( E_DEBUG ) && !defined( CSDMNC_NODBG )
#  define CSDMNC_DBG
# endif

// Client-Server Devices Muxed Network Client;

# include "csdmnb.h"
# include "csdbnc.h"

# include "err.h"
# include "sck.h"
# include "stk.h"

#ifdef CPE_F_MT
#	define CSDMNC__MT
#endif

#ifdef CSDMNC__MT
#	include "mtx.h"
#endif

#define CSDMNC_PING_DELAY	( 2 * 1000 )	// Delay between 2 pings to maintain the connection.

#define CSDMNC_DEFAULT_CACHE_SIZE	100

namespace csdmnc {
		using namespace csdmnb;
#ifdef CSDMNC__MT
	typedef mtx::handler___	mutex__;
#	define CSDMNC_NO_MUTEX			MTX_INVALID_HANDLER
#else
	typedef void *mutex__;
#	define CSDMNC_NO_MUTEX			NULL
#endif

	inline void _Lock( mutex__ Mutex )
	{
#ifdef CSDMNC__MT
		mtx::Lock( Mutex );
#endif
	}

	inline void _Unlock( mutex__ Mutex )
	{
#ifdef CSDMNC__MT
		mtx::Unlock( Mutex );
#endif
	}

	inline void _Delete(
		mutex__ Mutex,
		bso::bool__ EvenIfLocked = false )
	{
#ifdef CSDMNC__MT
		mtx::Delete( Mutex, EvenIfLocked );
#endif
	}

	inline mutex__ _Create( void )
	{
#ifdef CSDMNC__MT
		return mtx::Create();
#else
		return CSDMNC_NO_MUTEX;
#endif
	}

	typedef sck::socket_ioflow___ _flow___;

	typedef stk::E_BSTACK_( _flow___ * )	flows_;

	enum log__ {
		lCreation,
		lRetrieval,
		lRelease,
		l_amount,
		l_Undefined
	};

	const char *GetLogLabel( log__ Log );

	class log_callback__
	{
	protected:
		virtual void CSDMNCLog(
			log__ Log,
			const void *Flow,
			sdr::size__ Amount ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			// Standardisation.
		}
		E_CVDTOR( log_callback__ );
		void Init( void )
		{
			// Standardization.
		}
		void Log(
			log__ Log,
			const void *Flow,
			sdr::size__ Amount )
		{
			CSDMNCLog( Log ,Flow, Amount );
		}
	};

	class core_ {
	private:
		void _DeleteFlows( void );
		void _KeepAlive( time_t Delay );	// Lance le thread permettant de 'pinger' les connections inactives durant plus de 'Delay' ms.
		void _Log(
			log__ Log,
			const _flow___ *Flow )
		{
			if ( S_.Log.Callback != NULL ) {
qRH
qRB
				_Lock( S_.Log.Mutex );
				S_.Log.Callback->Log( Log, Flow, Flows.Amount() );
qRR
qRT
				_Unlock( S_.Log.Mutex );
qRE
			}
		}
	public:
		struct s {
			char *HostService;
			mutex__ MainMutex;
			struct log__ {
				log_callback__ *Callback;
				mutex__ Mutex;
			} Log;
			struct ping__ {
				bso::uint__ Delay;	// Dlai maximum d'inactivit.
				mutex__ Mutex;	// Pour signaler et attendre la terminaison du 'thread' ddi au 'ping'.
			} Ping;
			flows_::s Flows;
		} &S_;
		flows_ Flows;
		core_( s &S )
		: S_( S ),
		  Flows( S.Flows )
		{}
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( S_.Ping.Mutex != CSDMNC_NO_MUTEX )
					_Lock( S_.Ping.Mutex );	// Signale au 'thread' du 'ping' qu'il doit se terminer.

				_DeleteFlows();

				if ( S_.HostService != NULL )
					free( S_.HostService );

				if ( S_.Ping.Mutex != CSDMNC_NO_MUTEX ) {
					_Lock( S_.Ping.Mutex );	// Attend que le 'thread' ud 'ping' prenne acte de la demnade de terminaison.
					_Delete( S_.Ping.Mutex, true );
				}

				if ( S_.MainMutex != CSDMNC_NO_MUTEX )
					_Delete( S_.MainMutex );

				if ( S_.Log.Mutex != CSDMNC_NO_MUTEX )
					_Delete( S_.Log.Mutex );
			}

			Flows.reset( P );

			S_.HostService = NULL;
			S_.MainMutex = CSDMNC_NO_MUTEX;
			S_.Log.Mutex = CSDMNC_NO_MUTEX;
			S_.Log.Callback = NULL;
			S_.Ping.Delay = 0;
			S_.Ping.Mutex = CSDMNC_NO_MUTEX;
		}
		void plug( qSD__ &SD )
		{
			Flows.plug( SD );
		}
		void plug( qAS_ &AS )
		{
			Flows.plug( AS );
		}
		core_ &operator =( const core_ &C )
		{
			qRFbd();

			return *this;
		}
		bso::bool__ Init( 
			const char *HostService,
			bso::uint__ PingDelay = 0,
			log_callback__ *LogCallback = NULL )
		{
			reset();

			if ( ( S_.HostService = (char *)malloc( strlen( HostService ) + 1 ) ) == NULL )
				qRAlc();

			strcpy( S_.HostService, HostService );

			S_.MainMutex = _Create();
			S_.Log.Mutex = _Create();
			S_.Log.Callback = LogCallback;

			Flows.Init();

			_flow___ *Flow = Get( err::hUserDefined );

			if ( Flow == NULL )
				return false;

			Release( Flow );

			S_.Ping.Delay = PingDelay;

			if ( PingDelay != 0 ) {
				S_.Ping.Mutex = _Create();
				_KeepAlive( PingDelay );
			}

			return true;
		}
		bso::bool__ Init(
			const char *HostService,
			bso::uint__ PingDelay,
			log_callback__ &LogFunctions )
		{
			return Init( HostService, PingDelay, &LogFunctions );
		}
		_flow___ *Get( err::handling__ ErrorHandling = err::h_Default )
		{
			_flow___ *Flow = NULL;
		qRH
			log__ Log = l_Undefined;
			bso::bool__ Locked = false;
			sck::socket__ Socket = SCK_INVALID_SOCKET;
		qRB
			_Lock( S_.MainMutex );
			Locked = true;

			if ( Flows.Amount() ) {
				Flow = Flows.Pop();
				Log = lRetrieval;
			} else {
				Flow = new _flow___;

				if ( Flow == NULL )
					qRAlc();

				if ( ( Socket = csdbnc::Connect( S_.HostService, ErrorHandling ) ) == SCK_INVALID_SOCKET ) {
					delete Flow;
					Flow = NULL;
					qRReturn;
				}

				Flow->Init( Socket );

				Log = lCreation;
			}

			_Unlock( S_.MainMutex );
			Locked = false;

			_Log( Log, Flow );
		qRR
			if ( Flow != NULL ) {
				delete Flow;
				Flow = NULL;
			}
		qRT
			if ( Locked )
				_Unlock( S_.MainMutex );
		qRE

			return Flow;
		}
		void Release( _flow___ *Flow )
		{
			_Lock( S_.MainMutex );

			Flows.Push( Flow );

			_Unlock( S_.MainMutex );

			_Log( lRelease, Flow );
		}
		void Ping( void );	// Emet un 'ping' sur les connections reste inactives trop longtemps.
	};

	E_AUTO( core );

	class _driver___
	: public fdr::ioflow_driver___<>
	{
	private:
		_flow___ *_Flow;
		core_ *_Core;
		id__ _Id;
		bso::bool__ _Prepare( void )	// Return true if has already a flow, false otherwise.
		{
			bso::bool__ Created = _Flow == NULL;

			if ( Created ) {
				_Flow = _Core->Get();

				PutId( _Id, *_Flow );
			}

			return !Created;
		}
		void _Commit( void )
		{
#ifdef CSDMNC_DBG
			if ( _Flow == NULL )
				qRFwk();
#endif

			_Flow->Commit();

			if ( _Id == CSDMNB_UNDEFINED )
				_Id = GetId( *_Flow );
			else if ( _Flow->Get() != 0 )
				qRFwk();
		}
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum )
		{
			fdr::size__ Amount = 0;

			_Prepare();

			return _Flow->WriteUpTo( Buffer, Maximum );
		}
		virtual void FDRCommit( void )
		{
			if ( _Flow != NULL )
				_Commit();
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer )
		{
			return _Flow->ReadUpTo( Maximum, Buffer );
		}
		virtual void FDRDismiss( void )
		{
			if ( _Flow != NULL ) {
				_Core->Release( _Flow );
				_Flow->Dismiss();
			}

			_Flow = NULL;
		}
		public:
			void reset( bso::bool__ P = true )
			{
				fdr::ioflow_driver___<>::reset( P );

				if ( P ) {
					if ( _Flow != NULL )
						delete _Flow;
				}

				_Flow = NULL;
				_Id = CSDMNB_UNDEFINED;
				_Core = NULL;
			}
			_driver___( void )
			{
				reset( false );
			}
			~_driver___( void )
			{
				reset();
			}
			void Init(
				core_ &Core,
				fdr::thread_safety__ ThreadSafety )
			{
				reset();

				fdr::ioflow_driver___<>::Init( ThreadSafety );
				_Core = &Core;
			}
	};

	class client_ioflow___
	: public flw::ioflow__
	{
	private:
		_driver___ _Driver;
		flw::byte__ _Cache[CSDMNC_DEFAULT_CACHE_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			flw::ioflow__::reset( P );
			_Driver.reset( P );
		}
		client_ioflow___( void )
		{
			reset( false );
		}
		void Init( core_ &Core )
		{
			reset();

			_Driver.Init( Core, fdr::tsDisabled );
			ioflow__::Init( _Driver, _Cache, sizeof( _Cache ), FLW_AMOUNT_MAX );
		}
	};
}

#endif