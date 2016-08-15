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

// THread Tools 

#ifndef THT__INC
# define THT__INC

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
		mtx::rHandler Mutex_;
		void Release_( void )
		{
			if ( Mutex_ != mtx::UndefinedHandler )
				Delete( Mutex_ );

			Mutex_ = mtx::UndefinedHandler;
		}
		void Test_( void )
		{
			if ( Mutex_ == mtx::UndefinedHandler )
				qRFwk();
		}
	public:
		sTID ThreadID;
		void reset( bso::sBool P = true )
		{
			if ( P )
				Release_();

			Mutex_ = mtx::UndefinedHandler;
			ThreadID = Undefined;
		}
		qCDTOR( rCore_ );
		void Init( void )
		{
			Release_();

			Mutex_ = mtx::Create();
			ThreadID = Undefined;
		}
		bso::sBool TryToLock( void )
		{
			Test_();

			return mtx::TryToLock( Mutex_ );
		}
		void Lock( void )
		{
			Test_();

			mtx::Lock( Mutex_ );
		}
		void Unlock( void )
		{
			Test_();

			mtx::Unlock( Mutex_ );
		}
		void UnlockIfLocked( void )
		{
			if ( mtx::IsLocked( Mutex_ ) )
				mtx::Unlock( Mutex_ );
		}
	};

	// Block a thread until another unblocks it.
	class rBlocker {
	private:
		rCore_ Core_;
	public:
		void reset( bso::sBool P = true )
		{
			Core_.reset( P );
		}
		qCDTOR( rBlocker );
		void Init( void )
		{
			Core_.Init();

			Core_.Lock();
			Core_.ThreadID = GetTID();
		}
		void Wait( void )
		{
			if ( Core_.ThreadID == Undefined ) {
				Core_.Lock();
				Core_.ThreadID = GetTID();
			} else 	if ( Core_.ThreadID != GetTID() )
				qRFwk();

			Core_.Lock();

			Core_.ThreadID = Undefined;

			Core_.Unlock();
		}
		void Unblock( void )
		{
			if ( Core_.ThreadID == Undefined )
				qRFwk();
			else if ( Core_.ThreadID == GetTID() )
				qRFwk();

			Core_.Unlock();
		}
	};

	// Ensure that a ressource is only acces by one thread at a time.
	class rLocker {
	private:
		rCore_ Core_;
	public:
		void reset( bso::sBool P = true )
		{
			Core_.reset( P );
		}
		qCDTOR( rLocker );
		void Init( void )
		{
			Core_.Init();
		}
		void Lock( void )
		{
			Core_.Lock();

			if ( Core_.ThreadID != Undefined )
				qRFwk();

			Core_.ThreadID = GetTID();
		}
		void Unlock( void )
		{
			if ( Core_.ThreadID == Undefined )
				qRFwk();
			else if ( Core_.ThreadID == GetTID() )
				qRFwk();

			Core_.ThreadID = Undefined;

			Core_.Unlock();
		}
		void UnlockIfLocked( void )
		{
			Core_.UnlockIfLocked();

			Core_.ThreadID = Undefined;
		}
	};
}

#endif
