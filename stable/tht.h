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

// THread Tools

#ifndef THT_INC_
# define THT_INC_

# define THT_NAME		"THT"

# if defined( E_DEBUG ) && !defined( THT_NODBG )
#  define THT_DBG
# endif

# include "cpe.h"
# include "mtx.h"
# include "thtsub.h"

# if defined( CPE_S_POSIX )
#  define THT__POSIX
# elif defined ( CPE_S_WIN )
#  define THT__WIN
# else
#  error
# endif

# ifdef THT__WIN
#  include <process.h>
#  include <windows.h>
# elif defined( THT__POSIX )
#  include <pthread.h>
#  include <unistd.h>
#  include <stdlib.h>
#  include <signal.h>
# else
#  error
# endif

# define THT_UNDEFINED_THREAD_ID	0	// Totaly arbitrary ; should correspond to the system thread, so should never be returned by 'GetTID()'.

namespace tht {
	using namespace thtsub;

	typedef sThreadID thread_id__;

	//f Return an unique ID for the current thread.
	inline thread_id__ GetTID( void )
	{
# ifdef THT__WIN
		return GetCurrentThreadId();
# elif defined( THT__POSIX )
		return pthread_self();
# else
#  error
# endif
	}


	//f Suspend the current thred for 'Delay' milliseconds.
	void Suspend( unsigned long Delay );

	//f Wait 'Seconds' seconds.
	inline void Wait( unsigned long Seconds )
	{
		Suspend( Seconds * 1000 );
	}

	//f Tell the remainder to give hand to the next thread.
	void Defer( void );

	//f Tell the remainder to give hand to the next thread and wait 'Delay' millisecond.
	inline void Defer( unsigned long Delay )
	{
		Defer();

		Suspend( Delay );
	}
}

/*************/
/**** NEW ****/
/*************/

namespace tht {
	typedef thread_id__ sThreadID;
	typedef sThreadID sTID;

	qCDEF( sTID, Undefined, THT_UNDEFINED_THREAD_ID );

	class rCore_
	{
	private:
		mtx::rMutex Mutex_;
		void Release_( void )
		{
			if ( Mutex_ != mtx::Undefined )
				Delete( Mutex_ );

			Mutex_ = mtx::Undefined;
		}
		void Test_( void ) const
		{
			if ( Mutex_ == mtx::Undefined )
				qRFwk();
		}
	public:
		sTID ThreadID;
		void reset( bso::sBool P = true )
		{
			if ( P )
				Release_();

			Mutex_ = mtx::Undefined;
			ThreadID = Undefined;
		}
		qCDTOR( rCore_ );
		void Init( void )
		{
			Release_();

			Mutex_ = mtx::Create();
			ThreadID = Undefined;
		}
		bso::sBool IsLocked( void ) const
		{
			Test_();

			return mtx::IsLocked( Mutex_ );
		}
		bso::sBool TryToLock( void )
		{
			Test_();

			return mtx::TryToLock( Mutex_ );
		}
		bso::sBool Lock( void )
		{
			Test_();

			return mtx::Lock( Mutex_ );
		}
		void Unlock( void )
		{
			Test_();

			mtx::Unlock( Mutex_ );
		}
		// Returns 'true' if registry was locked.
		bso::sBool UnlockIfLocked( void )
		{
			if ( mtx::IsLocked(Mutex_) ) {
				mtx::Unlock( Mutex_ );
				return true;
			} else
				return false;
		}
	};

	typedef bso::sUInt sCounter_;
	qCDEF( sCounter_, CounterMax_, bso::UIntMax );

	// Ensure that a resource is only accessed by one thread at a time.
	// All consecutive locking from same thread does not lock again.
	// Unlocking is only effective after be called as much as being locked.
	class rLocker {
	private:
		rCore_ Core_;
		sCounter_ Counter_;
	public:
		void reset( bso::sBool P = true )
		{
			Core_.reset( P );
			Counter_ = 0;
		}
		qCDTOR( rLocker );
		void Init( void )
		{
			Core_.Init();
			Counter_ = 0;
		}
		bso::sBool IsLocked( void ) const
		{
			return Core_.IsLocked();
		}
		bso::sBool Lock( void )
		{
		  bso::sBool WasNotLocked = true;
			tht::sTID TID = GetTID();

			if ( Core_.ThreadID != TID ) {
				WasNotLocked = Core_.Lock();

				if ( Core_.ThreadID == Undefined )
					Core_.ThreadID = TID;
				else
					qRFwk();
			} // Else was not locked by another thread.

			if ( Counter_ == CounterMax_ )
				qRLmt();

			Counter_++;

			return WasNotLocked;
		}
		void Unlock( void )
		{
			if ( Core_.ThreadID == GetTID() )
				Counter_--;
			else if ( Core_.ThreadID == Undefined )
				qRFwk();
			else
				qRFwk();

			if ( Counter_ == 0 ) {
				Core_.ThreadID = Undefined;
				Core_.Unlock();
			}
		}
	};

	class rLockerHandler
	{
	private:
		qRMV( rLocker, L_, Locker_ );
		bso::sBool Locked_;
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Locked_ )
					L_().Unlock();
			}

			tol::reset( P, Locker_, Locked_ );
		}
		qCDTOR( rLockerHandler );
		void Init( rLocker &Locker )
		{
			reset();

			Locker_ = &Locker;

			Lock();
		}
		void Lock( void )
		{
			if ( !Locked_ )
				L_().Lock();

			Locked_ = true;
		}
		void Unlock( void )
		{
			if ( !Locked_ )
				qRGnr();

			L_().Unlock();

			Locked_ = false;
		}
	};

		// Block a thread until another unblocks it.
	class rBlocker {
	private:
		mtx::rMutex
			Local_,	// To protect access to below mutex.
			Main_;	// Main mutex.
		void ReleaseMutex_( mtx::rMutex &Mutex )
		{
			if ( Mutex != mtx::Undefined )
				mtx::Delete( Mutex, true );

			Mutex = mtx::Undefined;
		}
		void ReleaseMutexes_( void )
		{
			ReleaseMutex_( Local_ );
			ReleaseMutex_( Main_ );
		}
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				ReleaseMutexes_();
			}

			Local_ = Main_ = mtx::Undefined;
		}
		qCDTOR( rBlocker );
		void Init( bso::sBool SkipPrefetching = false )
		{
		qRH;
		qRB;
			ReleaseMutexes_();

			Local_ = mtx::Create();
			Main_ = mtx::Create();

			if ( !SkipPrefetching ) {
				mtx::Lock( Main_ );
			}
		qRR;
			ReleaseMutexes_();
		qRT;
		qRE;
		}
		void Wait( void )
		{
		qRH
			mtx::rHandle Mutex;
		qRB
			Mutex.InitAndLock( Local_ );

			if ( mtx::TryToLock( Main_ ) )
				mtx::Unlock( Main_ );
			else
				Mutex.Unlock();

			mtx::Lock( Main_ );
		qRR
		qRT
		qRE
		}
		bso::sBool IsBlocked(void)
		{
			bso::sBool Blocked = false;
		qRH
			mtx::rHandle Mutex;
		qRB
			Mutex.InitAndLock( Local_ );

			Blocked = mtx::IsLocked(Main_);
		qRR
		qRT
		qRE
			return Blocked;
		}
		void Unblock( void )
		{
		qRH
//			mtx::rMutex Mutex;	// Can not be used, because the destructor could be called after destruction of underlying mutes.
			bso::sBool Locked = false;
		qRB
			mtx::Lock( Local_ );
			Locked = true;

			if ( mtx::IsLocked( Main_ ) ) {
				mtx::Unlock( Local_ );
				Locked = false;
				mtx::Unlock( Main_ );
			}
		qRR
		qRT
			if ( Locked )
				mtx::Unlock( Local_ );
		qRE
		}
	};

	// One can read only when one has write, and one can write only if previous writing was red.
	class rReadWrite
	{
	private:
		mtx::rMutex Write_, Read_;
		void Delete_( mtx::rMutex Mutex )
		{
			if ( Mutex != mtx::Undefined )
				mtx::Delete( Mutex, true );
		}
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				Delete_( Write_ );
				Delete_( Read_ );
			}

			Write_ = Read_ = mtx::Undefined;
		}
		qCDTOR( rReadWrite );
		void Init( void )
		{
			Delete_( Write_ );
			Delete_( Read_ );

			Write_ = mtx::Create();
			Read_ = mtx::Create();

			mtx::Lock( Read_ );
		}
		bso::sBool WriteBegin( tol::sDelay Timeout = 0 )	// If != 0, returns 'false' after 'Timeout' ms, otherwise returns 'true' when locks succeeds.
		{
			return mtx::Lock( Write_, Timeout );
		}
		void WriteEnd( void )
		{
			mtx::Unlock( Read_ );
		}
		bso::sBool WriteDismiss( tol::sDelay Timeout = 0 )	// If != 0, returns 'false' after 'Timeout' ms, otherwise returns 'true' when locks succeeds.
		{
			if ( !WriteBegin( Timeout ) )
				return false;

			WriteEnd();

			return true;
		}
		bso::sBool ReadBegin( tol::sDelay Timeout = 0 )	// If != 0, returns 'false' after 'Timeout' ms, otherwise returns 'true' when locks succeeds.
		{
			return mtx::Lock( Read_, Timeout );
		}
		void ReadEnd( void )
		{
			mtx::Unlock( Write_ );
		}
		bso::sBool ReadDismiss( tol::sDelay Timeout = 0 )	// If != 0, returns 'false' after 'Timeout' ms, otherwise returns 'true' when locks succeeds.
		{
			if ( !ReadBegin( Timeout ) )
				return false;

			ReadEnd();

			return true;
		}
	};

}

#endif
