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

// MuXed Connections CLienT

#ifndef MXCCLT__INC
# define MXCCLT__INC

# define MXCCLT_NAME		"MXCCLT"

# if defined( E_DEBUG ) && !defined( MXCCLT_NODBG )
#  define MXCCLT_DBG
# endif

# include "mxcbse.h"
# include "sck.h"
# include "stk.h"
# pragma message( __LOC__ "Include à enlever !")
# include "csdbnc.h"

# include "err.h"
# include "mtx.h"

#ifdef CPE_F_MT
#	define MXCCLT__MT
#endif

#ifdef MXCCLT__MT
#	include "mtx.h"
#endif

#define MXCCLT_PING_DELAY	( 2 * 1000 )	// Delay between 2 pings to maintain the connection.

#define MXCCLT_DEFAULT_CACHE_SIZE	100


namespace mxcclt {
	using namespace mxcbse;
#ifdef MXCCLT__MT
	typedef mtx::handler___	mutex__;
#	define MXCCLT_NO_MUTEX			MTX_INVALID_HANDLER
#else
	typedef void *mutex__;
#	define MXCCLT_NO_MUTEX			NULL
#endif

	inline void Lock_( mutex__ Mutex )
	{
#ifdef MXCCLT__MT
		mtx::Lock( Mutex );
#endif
	}

	inline void Unlock_( mutex__ Mutex )
	{
#ifdef MXCCLT__MT
		mtx::Unlock( Mutex );
#endif
	}

	inline void Delete_(
		mutex__ Mutex,
		bso::bool__ EvenIfLocked = false )
	{
#ifdef MXCCLT__MT
		mtx::Delete( Mutex, EvenIfLocked );
#endif
	}

	inline mutex__ _Create( void )
	{
#ifdef MXCCLT__MT
		return mtx::Create();
#else
		return MXCCLT_NO_MUTEX;
#endif
	}

	typedef sck::socket_ioflow___ rFlow;

	typedef stk::E_BSTACK_( rFlow * )	vFlows;

	qENUM( Log ) {
		lCreation,
		lRetrieval,
		lRelease,
		l_amount,
		l_Undefined
	};

	const char *GetLogLabel( eLog Log );

	class fLogCallback
	{
	protected:
		virtual void MXCCLTLog(
			eLog Log,
			const void *Flow,
			sdr::size__ Amount ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			// Standardisation.
		}
		qCVDTOR( fLogCallback );
		void Init( void )
		{
			// Standardization.
		}
		void Log(
			eLog Log,
			const void *Flow,
			sdr::size__ Amount )
		{
			MXCCLTLog( Log ,Flow, Amount );
		}
	};

	class vCore {
	private:
		void DeleteFlows_( void );
		void KeepAlive_( time_t Delay );	// Lance le thread permettant de 'pinger' les connections inactives durant plus de 'Delay' ms.
		void Log_(
			eLog Log,
			const rFlow *Flow )
		{
			if ( S_.Log.Callback != NULL ) {
qRH
qRB
				Lock_( S_.Log.Mutex );
				S_.Log.Callback->Log( Log, Flow, Flows.Amount() );
qRR
qRT
				Unlock_( S_.Log.Mutex );
qRE
			}
		}
	public:
		struct s {
			char *HostService;
			mutex__ MainMutex;
			struct log__ {
				fLogCallback *Callback;
				mutex__ Mutex;
			} Log;
			struct ping__ {
				bso::uint__ Delay;	// Dlai maximum d'inactivit.
				mutex__ Mutex;	// Pour signaler et attendre la terminaison du 'thread' ddi au 'ping'.
			} Ping;
			vFlows::s Flows;
		} &S_;
		vFlows Flows;
		vCore( s &S )
		: S_( S ),
		  Flows( S.Flows )
		{}
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( S_.Ping.Mutex != MXCCLT_NO_MUTEX )
					Lock_( S_.Ping.Mutex );	// Signale au 'thread' du 'ping' qu'il doit se terminer.

				DeleteFlows_();

				if ( S_.HostService != NULL )
					free( S_.HostService );

				if ( S_.Ping.Mutex != MXCCLT_NO_MUTEX ) {
					Lock_( S_.Ping.Mutex );	// Attend que le 'thread' ud 'ping' prenne acte de la demnade de terminaison.
					Delete_( S_.Ping.Mutex, true );
				}

				if ( S_.MainMutex != MXCCLT_NO_MUTEX )
					Delete_( S_.MainMutex );

				if ( S_.Log.Mutex != MXCCLT_NO_MUTEX )
					Delete_( S_.Log.Mutex );
			}

			Flows.reset( P );

			S_.HostService = NULL;
			S_.MainMutex = MXCCLT_NO_MUTEX;
			S_.Log.Mutex = MXCCLT_NO_MUTEX;
			S_.Log.Callback = NULL;
			S_.Ping.Delay = 0;
			S_.Ping.Mutex = MXCCLT_NO_MUTEX;
		}
		void plug( qSD__ &SD )
		{
			Flows.plug( SD );
		}
		void plug( qAS_ &AS )
		{
			Flows.plug( AS );
		}
		vCore &operator =( const vCore &C )
		{
			qRFbd();

			return *this;
		}
		bso::bool__ Init( 
			const char *HostService,
			bso::uint__ PingDelay = 0,
			fLogCallback *LogCallback = NULL )
		{
			reset();

			if ( ( S_.HostService = (char *)malloc( strlen( HostService ) + 1 ) ) == NULL )
				qRAlc();

			strcpy( S_.HostService, HostService );

			S_.MainMutex = _Create();
			S_.Log.Mutex = _Create();
			S_.Log.Callback = LogCallback;

			Flows.Init();

			rFlow *Flow = Get( err::hUserDefined );

			if ( Flow == NULL )
				return false;

			Release( Flow );

			S_.Ping.Delay = PingDelay;

			if ( PingDelay != 0 ) {
				S_.Ping.Mutex = _Create();
				KeepAlive_( PingDelay );
			}

			return true;
		}
		bso::bool__ Init(
			const char *HostService,
			bso::uint__ PingDelay,
			fLogCallback &LogFunctions )
		{
			return Init( HostService, PingDelay, &LogFunctions );
		}
		rFlow *Get( err::handling__ ErrorHandling = err::h_Default )
		{
			rFlow *Flow = NULL;
		qRH
			eLog Log = l_Undefined;
			bso::bool__ Locked = false;
			sck::socket__ Socket = SCK_INVALID_SOCKET;
		qRB
			Lock_( S_.MainMutex );
			Locked = true;

			if ( Flows.Amount() ) {
				Flow = Flows.Pop();
				Log = lRetrieval;
			} else {
				Flow = new rFlow;

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

			Unlock_( S_.MainMutex );
			Locked = false;

			Log_( Log, Flow );
		qRR
			if ( Flow != NULL ) {
				delete Flow;
				Flow = NULL;
			}
		qRT
			if ( Locked )
				Unlock_( S_.MainMutex );
		qRE

			return Flow;
		}
		void Release( rFlow *Flow )
		{
			Lock_( S_.MainMutex );

			Flows.Push( Flow );

			Unlock_( S_.MainMutex );

			Log_( lRelease, Flow );
		}
		void Ping( void );	// Emet un 'ping' sur les connections reste inactives trop longtemps.
	};

	qW( Core );

	class _driver___
	: public fdr::ioflow_driver___<>
	{
	private:
		rFlow *Flow_;
		vCore *Core_;
		fId Id_;
		bso::bool__ Prepare_( void )	// Return true if has already a flow, false otherwise.
		{
			bso::bool__ Created = Flow_ == NULL;

			if ( Created ) {
				Flow_ = Core_->Get();

				PutId( Id_, *Flow_ );
			}

			return !Created;
		}
		void Commit_( void )
		{
#ifdef MXCCLT_DBG
			if ( Flow_ == NULL )
				qRFwk();
#endif

			Flow_->Commit();

			if ( Id_ == MXCBSE_UNDEFINED )
				Id_ = GetId( *Flow_ );
			else if ( Flow_->Get() != 0 )
				qRFwk();
		}
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum )
		{
			fdr::size__ Amount = 0;

			Prepare_();

			return Flow_->WriteUpTo( Buffer, Maximum );
		}
		virtual void FDRCommit( void )
		{
			if ( Flow_ != NULL )
				Commit_();
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer )
		{
			return Flow_->ReadUpTo( Maximum, Buffer );
		}
		virtual void FDRDismiss( void )
		{
			if ( Flow_ != NULL ) {
				Core_->Release( Flow_ );
				Flow_->Dismiss();
			}

			Flow_ = NULL;
		}
		public:
			void reset( bso::bool__ P = true )
			{
				fdr::ioflow_driver___<>::reset( P );

				if ( P ) {
					if ( Flow_ != NULL )
						delete Flow_;
				}

				Flow_ = NULL;
				Id_ = MXCBSE_UNDEFINED;
				Core_ = NULL;
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
				vCore &Core,
				fdr::thread_safety__ ThreadSafety )
			{
				reset();

				fdr::ioflow_driver___<>::Init( ThreadSafety );
				Core_ = &Core;
			}
	};

	class client_ioflow___
	: public flw::ioflow__
	{
	private:
		_driver___ _Driver;
		flw::byte__ _Cache[MXCCLT_DEFAULT_CACHE_SIZE];
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
		void Init( vCore &Core )
		{
			reset();

			_Driver.Init( Core, fdr::tsDisabled );
			ioflow__::Init( _Driver, _Cache, sizeof( _Cache ), FLW_AMOUNT_MAX );
		}
	};

}

#endif
