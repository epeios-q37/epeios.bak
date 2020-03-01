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

// Client-Server Devices MuXing Client

#ifndef CSDMXC_INC_
# define CSDMXC_INC_

# define CSDMXC_NAME		"CSDMXC"

# if defined( E_DEBUG ) && !defined( CSDMXC_NODBG )
#  define CSDMXC_DBG
# endif

# include "csdmxb.h"

# include "stkbch.h"
# include "flf.h"
# include "err.h"

#ifdef CPE_F_MT
#	define CSDMXC__MT
#endif

#ifdef CSDMXC__MT
#	include "mtx.h"
#endif


#define CSDMXC_PING_DELAY	( 2 * 1000 )	// Delay between 2 pings to maintain the connection.

#define CSDMXC_DEFAULT_CACHE_SIZE	100

namespace csdmxc {
	using namespace csdmxb;
#ifdef CSDMXC__MT
	typedef mtx::handler___	mutex__;
#	define CSDMXC_NO_MUTEX			MTX_INVALID_HANDLER
#else
	typedef void *mutex__;
#	define CSDMXC_NO_MUTEX			NULL
#endif

	inline void Lock_( mutex__ Mutex )
	{
#ifdef CSDMXC__MT
		mtx::Lock( Mutex );
#endif
	}

	inline void Unlock_( mutex__ Mutex )
	{
#ifdef CSDMXC__MT
		mtx::Unlock( Mutex );
#endif
	}

	inline void Delete_(
		mutex__ Mutex,
		bso::bool__ EvenIfLocked = false )
	{
#ifdef CSDMXC__MT
		mtx::Delete( Mutex, EvenIfLocked );
#endif
	}

	inline mutex__ _Create( void )
	{
#ifdef CSDMXC__MT
		return mtx::Create();
#else
		return CSDMXC_NO_MUTEX;
#endif
	}

	typedef fdr::rRWDriver rDriver_;

	typedef stkbch::qBSTACKdl( void * )	dUPs;
	qW( UPs );

	qENUM( Log ) {
		lCreation,
		lAcquire,
		lRelease,
		l_amount,
		l_Undefined
	};

	const char *GetLabel( eLog Log );

	class cLog
	{
	protected:
		virtual void CSDMXCLog(
			eLog Log,
			const void *UP,
			sdr::size__ Amount ) = 0;
	public:
		void Log(
			eLog Log,
			const void *UP,
			sdr::size__ Amount )
		{
			CSDMXCLog( Log, UP, Amount );
		}
	};

	class rLogCallback
	: public cLog
	{
	private:
		flf::file_oflow___ Flow_;
		txf::text_oflow__ TFlow_;
	protected:
		virtual void CSDMXCLog(
			eLog Log,
			const void *UP,
			sdr::size__ Amount ) override
		{
			TFlow_ << GetLabel(Log) << " : " << UP << " - " << Amount << txf::nl << txf::commit;
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

	class cCallback
	{
	protected:
		virtual void *CSDMXCNew( void ) = 0;
		virtual rDriver_ &CSDMXCExtractDriver( void *UP ) = 0;
		virtual void CSDMXCRelease( void *UP ) = 0;
		virtual time_t CSDMXCEpochTimeStamp( void *UP )	// By default, the connection is always in use.
		{
			return tol::EpochTime( false );
		}
	public:
		void *New( void )
		{
			return CSDMXCNew();
		}
		virtual rDriver_ &ExtractDriver( void *UP )
		{
			return CSDMXCExtractDriver( UP );
		}
		void Release( void *Flow )
		{
			return CSDMXCRelease( Flow );
		}
		time_t EpochTimeStamp( void *UP )
		{
			return CSDMXCEpochTimeStamp( UP );
		}
	};

# ifdef CSDMXC__MT
	// Predeclaration
	void KeepAlive_( void * );
# endif

	class rCore {
	private:
		qRMV( cCallback, C_, Callback_ );
		wUPs UPs;
		mutex__ MainMutex_;
		struct fLog_ {
			cLog *Callback;
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
				if ( Ping_.Mutex != CSDMXC_NO_MUTEX )
					Lock_( Ping_.Mutex );	// Signale au 'thread' du 'ping' qu'il doit se terminer.

				ReleaseUPs_();

				if ( Ping_.Mutex != CSDMXC_NO_MUTEX ) {
					Lock_( Ping_.Mutex );	// Attend que le 'thread' ud 'ping' prenne acte de la demnade de terminaison.
					Delete_( Ping_.Mutex, true );
				}

				if ( MainMutex_ != CSDMXC_NO_MUTEX )
					Delete_( MainMutex_ );

				if ( Log_.Mutex != CSDMXC_NO_MUTEX )
					Delete_( Log_.Mutex );
			}

			UPs.reset( P );

			Callback_ = NULL;
			MainMutex_ = CSDMXC_NO_MUTEX;
			Log_.Mutex = CSDMXC_NO_MUTEX;
			Log_.Callback = NULL;
			Ping_.Delay = 0;
			Ping_.Mutex = CSDMXC_NO_MUTEX;
		}
		qCVDTOR( rCore );
		bso::bool__ Init(
			cCallback &Callback,
			bso::uint__ PingDelay = 0,
			cLog *LogCallback = NULL )
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
			cCallback &Callback,
			bso::uint__ PingDelay,
			cLog &LogCallback )
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
				Log = lAcquire;
			} else {
				UP = C_().New();

				if ( UP != NULL )
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
		rDriver_ &ExtractDriver( void *UP )
		{
			return C_().ExtractDriver( UP );
		}
		void Release( void *UP )
		{
		qRH
			bso::sBool Locked = false;
		qRB
			Lock_( MainMutex_ );
			Locked = true;

			UPs.Push( UP );

			Unlock_( MainMutex_ );
			Locked = false;

			Record_( lRelease, UP );
		qRR
		qRT
			if ( Locked )
				Unlock_( MainMutex_ );
		qRE
		}
		void Ping( void );	// Emet un 'ping' sur les connections reste inactives trop longtemps.
# ifdef CSDMXC__MT
		friend void csdmxc::KeepAlive_( void *UP );
# endif
		void GiveUp( void )
		{
			UPs.Init();
		}
	};

	class rRWDriver
	: public fdr::ioflow_driver___<>
	{
	private:
		void *UP_;
		qRMV( rCore, C_, Core_ );
		flw::rDressedRWFlow<> Flow_;
		rDriver_ &Driver_( void )
		{
			if ( UP_ == NULL )
				qRFwk();

			return C_().ExtractDriver( UP_ );
		}
		sId Id_;
		bso::bool__ Prepare_( void )	// Return true if has already a flow, false otherwise.
		{
			bso::bool__ Created = UP_ == NULL;

			if ( Created ) {
				UP_ = C_().Get();

				if ( UP_ == NULL )	// Not able to connect to server.
					qRFwk();
			}

			Flow_.Init( Driver_() );

			if ( Created ) {
				PutId( Id_, Flow_ );
			}

			return !Created;
		}
		bso::sBool Commit_( bso::sBool Unlock )
		{
			bso::sBool Return = false;
		qRH;
			bso::sBool Unlocked = false;
		qRB;
			Unlocked = !Flow_.RDriver().IsLocked();	/* Below operation will lock the read driver,
													so we have to unlock it when it was now already locked,
													so another thread will be able to take the ownership. */
			Flow_.Commit( Unlock );


			if ( Id_ == CSDMXB_UNDEFINED ) {
				Id_ = GetId( Flow_ );
				Return = true;
			} else if ( !Flow_.EndOfFlow() ) {
				if ( Flow_.Get() == 0 ) {
					Return = true;
				} else
					qRFwk();
			}
		qRR;
		qRT;
			if ( Unlocked )
				Flow_.RDriver().Unlock( err::h_Default );
		qRE;
			return Return;
		}
		void GiveUp_( void )
		{
			C_().GiveUp();
			Flow_.reset();
			UP_ = NULL;
			Id_ = CSDMXB_UNDEFINED;
			Core_ = NULL;
		}
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum ) override
		{
			fdr::size__ Amount = 0;
		qRH
		qRB
			if ( Core_ != NULL ) {
				Prepare_();

				Amount = Flow_.WriteUpTo( Buffer, Maximum );

				if ( Amount == 0 )
					GiveUp_();
			}
		qRR
			GiveUp_();
		qRT
		qRE
			return Amount;
		}
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override
		{
		qRH
		qRB
			if ( UP_ != NULL )
				if ( !Commit_( Unlock ) )	// This 'Commit_' is specific to this object.
					GiveUp_();
		qRR
			GiveUp_();
		qRT
		qRE
			return true;
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			 return Driver_().WTake( Owner );
		}
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			fdr::size__ Amount = 0;
		qRH
		qRB
			if ( Core_ != NULL ) {
				if ( !Prepare_() )
					Commit_( false );

				Amount = Flow_.ReadUpTo( Maximum, Buffer );

				if ( Amount == 0 )
					GiveUp_();
			}
		qRR
			GiveUp_();
		qRT
		qRE
			return Amount;
		}
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override
		{
			bso::sBool Success = true;
		qRH
		qRB
			if ( UP_ != NULL ) {
				Success = Driver_().Dismiss( Unlock, ErrHandling );
				C_().Release( UP_ );
			}

			UP_ = NULL;
		qRR
			GiveUp_();
		qRT
		qRE
			return Success;
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			 return Driver_().RTake( Owner );
		}
		public:
			void reset( bso::bool__ P = true )
			{
				Flow_.reset( P );
				fdr::ioflow_driver___<>::reset( P );

				if ( P ) {
					if ( UP_ != NULL )
						C_().Release( UP_ );
				}

				UP_ = NULL;
				Id_ = CSDMXB_UNDEFINED;
				Core_ = NULL;
			}
			qCVDTOR( rRWDriver );
			void Init(
				rCore &Core,
				fdr::thread_safety__ ThreadSafety )
			{
				reset();

				fdr::ioflow_driver___<>::Init( ThreadSafety );
				Core_ = &Core;
				// 'Flow_' will be initalized later.
			}
	};

	class rRWFlow
	: public flw::ioflow__
	{
	private:
		rRWDriver _Driver;
		flw::byte__ _Cache[CSDMXC_DEFAULT_CACHE_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			flw::ioflow__::reset( P );
			_Driver.reset( P );
		}
		qCDTOR( rRWFlow );
		void Init( rCore &Core )
		{
			reset();

			_Driver.Init( Core, fdr::tsDisabled );
			ioflow__::Init( _Driver, _Cache, sizeof( _Cache ) );
		}
	};
}

#endif
