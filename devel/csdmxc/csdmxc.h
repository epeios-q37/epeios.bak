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

// Client-Server Devices MuXing Client

#ifndef CSDMXC__INC
# define CSDMXC__INC

# define CSDMXC_NAME		"CSDMXC"

# if defined( E_DEBUG ) && !defined( CSDMXC_NODBG )
#  define CSDMXC_DBG
# endif

# include "csdmxb.h"

# include "stk.h"

# include "err.h"

#define CXDMXC_PING_DELAY	( 2 * 1000 )	// Delay between 2 pings to maintain the connection.

#define CXDMXC_DEFAULT_CACHE_SIZE	100

namespace csdmxc {
	using namespace csdmxb;
#ifdef CXDMXC__MT
	typedef mtx::handler___	mutex__;
#	define CXDMXC_NO_MUTEX			MTX_INVALID_HANDLER
#else
	typedef void *mutex__;
#	define CXDMXC_NO_MUTEX			NULL
#endif

	inline void Lock_( mutex__ Mutex )
	{
#ifdef CXDMXC__MT
		mtx::Lock( Mutex );
#endif
	}

	inline void Unlock_( mutex__ Mutex )
	{
#ifdef CXDMXC__MT
		mtx::Unlock( Mutex );
#endif
	}

	inline void Delete_(
		mutex__ Mutex,
		bso::bool__ EvenIfLocked = false )
	{
#ifdef CXDMXC__MT
		mtx::Delete( Mutex, EvenIfLocked );
#endif
	}

	inline mutex__ _Create( void )
	{
#ifdef CXDMXC__MT
		return mtx::Create();
#else
		return CXDMXC_NO_MUTEX;
#endif
	}

	typedef flw::ioflow__ fFlow;

	typedef stk::E_BSTACK_( void * )	vUPs;
	qW( UPs );

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
		virtual void CXDMXCLog(
			eLog Log,
			const void *UP,
			sdr::size__ Amount ) = 0;
	public:
		qCALLBACK_DEF( LogCallback );
		void Log(
			eLog Log,
			const void *UP,
			sdr::size__ Amount )
		{
			CXDMXCLog( Log, UP, Amount );
		}
	};

	class fCallback
	{
	protected:
		virtual void *CXDMXCNew( void ) = 0;
		virtual fFlow &CXDMXCExtractFlow( void *UP ) = 0;
		virtual void CXDMXCRelease( void *UP ) = 0;
		virtual time_t CXDMXCEpochTimeStamp( void *UP )	// By default, the connection is always in use.
		{
			return tol::EpochTime( false );
		}
	public:
		qCALLBACK_DEF( Callback );
		void *New( void )
		{
			return CXDMXCNew();
		}
		virtual fFlow &ExtractFlow( void *UP )
		{
			return CXDMXCExtractFlow( UP );
		}
		void Release( void *Flow )
		{
			return CXDMXCRelease( Flow );
		}
		time_t EpochTimeStamp( void *UP )
		{
			return CXDMXCEpochTimeStamp( UP );
		}
	};

# ifdef CXDMXC__MT
	// Predeclaration
	void KeepAlive_( void * );
# endif

	class rCore {
	private:
		qRVM( fCallback, C_, Callback_ );
		iUPs UPs;
		mutex__ MainMutex_;
		struct fLog_ {
			fLogCallback *Callback;
			mutex__ Mutex;
		} Log_;
		struct fPing__ {
			bso::uint__ Delay;	// Dlai maximum d'inactivit.
			mutex__ Mutex;	// Pour signaler et attendre la terminaison du 'thread' ddi au 'ping'.
		} Ping_;
		void ReleaseUPs_( void );
		void KeepAlive_( time_t Delay );	// Lance le thread permettant de 'pinger' les connections inactives durant plus de 'Delay' ms.
		void Record_(
			eLog Log,
			const void *UP )
		{
			if ( Log_.Callback != NULL ) {
qRH
qRB
				Lock_( Log_.Mutex );
				Log_.Callback->Log( Log, UP, UPs.Amount() );
qRR
qRT
				Unlock_( Log_.Mutex );
qRE
			}
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( Ping_.Mutex != CXDMXC_NO_MUTEX )
					Lock_( Ping_.Mutex );	// Signale au 'thread' du 'ping' qu'il doit se terminer.

				ReleaseUPs_();

				if ( Ping_.Mutex != CXDMXC_NO_MUTEX ) {
					Lock_( Ping_.Mutex );	// Attend que le 'thread' ud 'ping' prenne acte de la demnade de terminaison.
					Delete_( Ping_.Mutex, true );
				}

				if ( MainMutex_ != CXDMXC_NO_MUTEX )
					Delete_( MainMutex_ );

				if ( Log_.Mutex != CXDMXC_NO_MUTEX )
					Delete_( Log_.Mutex );
			}

			UPs.reset( P );

			Callback_ = NULL;
			MainMutex_ = CXDMXC_NO_MUTEX;
			Log_.Mutex = CXDMXC_NO_MUTEX;
			Log_.Callback = NULL;
			Ping_.Delay = 0;
			Ping_.Mutex = CXDMXC_NO_MUTEX;
		}
		qVDTOR( rCore );
		bso::bool__ Init( 
			fCallback &Callback,
			bso::uint__ PingDelay = 0,
			fLogCallback *LogCallback = NULL )
		{
			reset();

			Callback_ = &Callback;


			MainMutex_ = _Create();
			Log_.Mutex = _Create();
			Log_.Callback = LogCallback;                

			UPs.Init();

			void *UP = Get( err::hUserDefined );

			if ( UP == NULL )
				return false;

			Release( UP );

			Ping_.Delay = PingDelay;

			if ( PingDelay != 0 ) {
				Ping_.Mutex = _Create();
				KeepAlive_( PingDelay );
			}

			return true;
		}
		bso::bool__ Init(
			fCallback &Callback,
			bso::uint__ PingDelay,
			fLogCallback &LogCallback )
		{
			return Init( Callback, PingDelay, &LogCallback );
		}
		void *Get( err::handling__ ErrorHandling = err::h_Default )
		{
			void *UP = NULL;
		qRH
			eLog Log = l_Undefined;
			bso::bool__ Locked = false;
		qRB
			Lock_( MainMutex_ );
			Locked = true;

			if ( UPs.Amount() ) {
				UP = UPs.Pop();
				Log = lRetrieval;
			} else {
				UP = C_().New();

				if ( UP == NULL )
					qRAlc();

				Log = lCreation;
			}

			Unlock_( MainMutex_ );
			Locked = false;

			Record_( Log, UP );
		qRR
			if ( UP != NULL ) {
				C_().Release( UP );
				UP = NULL;
			}
		qRT
			if ( Locked )
				Unlock_( MainMutex_ );
		qRE

			return UP;
		}
		fFlow &ExtractFlow( void *UP )
		{
			return C_().ExtractFlow( UP );
		}
		void Release( void *UP )
		{
			Lock_( MainMutex_ );

			UPs.Push( UP );

			Unlock_( MainMutex_ );

			Record_( lRelease, UP );
		}
		void Ping( void );	// Emet un 'ping' sur les connections reste inactives trop longtemps.
# ifdef CXDMXC__MT
		friend void csdmxc::KeepAlive_( void *UP );
# endif
	};

	class _driver___
	: public fdr::ioflow_driver___<>
	{
	private:
		void *UP_;
		qRVM( rCore, C_, Core_ );
		fFlow &F_( void )
		{
			if ( UP_ == NULL )
				qRFwk();

			return C_().ExtractFlow( UP_ );
		}
		fId Id_;
		bso::bool__ Prepare_( void )	// Return true if has already a flow, false otherwise.
		{
			bso::bool__ Created = UP_ == NULL;

			if ( Created ) {
				UP_ = C_().Get();

				PutId( Id_, F_() );
			}

			return !Created;
		}
		void Commit_( void )
		{
			fFlow &Flow = F_();

			Flow.Commit();

			if ( Id_ == CSDMXB_UNDEFINED )
				Id_ = GetId( Flow );
			else if ( Flow.Get() != 0 )
				qRFwk();
		}
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum )
		{
			fdr::size__ Amount = 0;

			Prepare_();

			return F_().WriteUpTo( Buffer, Maximum );
		}
		virtual void FDRCommit( void )
		{
			if ( UP_ != NULL )
				Commit_();
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer )
		{
			return F_().ReadUpTo( Maximum, Buffer );
		}
		virtual void FDRDismiss( void )
		{
			if ( UP_ != NULL ) {
				F_().Dismiss();
				C_().Release( UP_ );
			}

			UP_ = NULL;
		}
		public:
			void reset( bso::bool__ P = true )
			{
				fdr::ioflow_driver___<>::reset( P );

				if ( P ) {
					if ( UP_ != NULL )
						C_().Release( UP_ );
				}

				UP_ = NULL;
				Id_ = CSDMXB_UNDEFINED;
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
				rCore &Core,
				fdr::thread_safety__ ThreadSafety )
			{
				reset();

				fdr::ioflow_driver___<>::Init( ThreadSafety );
				Core_ = &Core;
			}
	};

	class rClientIOFlow
	: public flw::ioflow__
	{
	private:
		_driver___ _Driver;
		flw::byte__ _Cache[CXDMXC_DEFAULT_CACHE_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			flw::ioflow__::reset( P );
			_Driver.reset( P );
		}
		qCDTOR( rClientIOFlow );
		void Init( rCore &Core )
		{
			reset();

			_Driver.Init( Core, fdr::tsDisabled );
			ioflow__::Init( _Driver, _Cache, sizeof( _Cache ), FLW_AMOUNT_MAX );
		}
	};
}

#endif
