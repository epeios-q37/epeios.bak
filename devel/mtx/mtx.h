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

#ifndef MTX_INC_
# define MTX_INC_

# define MTX_NAME		"MTX"

# if defined( E_DEBUG ) && !defined( MTX_NODBG )
#  define MTX_DBG
# endif

// MuTeX


# include "bso.h"
# include "cpe.h"
# include "tol.h"

# ifdef system
#  define MTX_SYSTEM_BUFFER_ system
#  undef system
# endif

# include <condition_variable>
# include <mutex>	// Before 'tol.h', otherwise they may be some conflict.

# ifdef MTX_SYSTEM_BUFFER_
#  define system MTX_SYSTEM_BUFFER_
#  undef MTX_SYSTEM_BUFFER_
# endif


/***************/
/***** OLD *****/
/***************/

# define MTX__INVALID_HANDLER	NULL	// Internal use.
# define MTX_INVALID_HANDLER	MTX__INVALID_HANDLER	// For users.

namespace mtx {

	qENUM( State_ )
	{
		sFree,
		sLocked,
		sReleased,
		sDisabled,
		s_amount,
		s_Undefined
	};

	typedef struct _mutex__ {
	private:
		std::mutex Mutex_;
		std::condition_variable Core_;
		eState_ State_;
	private:
		bso::bool__ IsReleased_( void ) const
		{
			return State_ == sReleased;
		}
		bso::bool__ IsDisabled_( void ) const
		{
			return State_ == sDisabled;
		}
		bso::bool__ IsLocked_( void ) const
		{
			return State_ == sLocked;
		}
	public:
#ifdef MTX__CONTROL
		void Release( void )
		{
			ststd::unique_lock<decltype(Mutex_)> Lock( Mutex_ );

			State_ = sReleased;
		}
		bso::bool__ IsReleased( void ) const
		{
			return IsReleased_();
		}
#endif
		bso::bool__ IsDisabled( void ) const
		{
			return IsDisabled_();
		}
		bso::bool__ IsLocked( void )
		{
			std::unique_lock<decltype(Mutex_)> Lock( Mutex_ );
#ifdef MTX__CONTROL
			if ( IsReleased_() ) {
				Lock.unlock();	// In case of the use of the 'setjmp' version or the error library.
				qRFwk();
			}
#endif
			if ( IsDisabled_() )
				return false;

			return IsLocked_();
		}
		void Lock( void )
		{
			std::unique_lock<decltype(Mutex_)> Lock( Mutex_ );
#ifdef MTX__CONTROL
			if ( IsReleased_() ) {
				Lock.unlock();	// In case of the use of the 'setjmp' version or the error library.
				qRFwk();
			}
#endif
			if ( IsDisabled_() )
				return;

			if ( IsLocked_() )
				Core_.wait( Lock, [this]() {return State_ == sFree;} );	// third parameter to handle spurious awake.

			State_ = sLocked;
		}
		bso::bool__ TryToLock( tol::sDelay TimeOut )
		{
			std::unique_lock<decltype(Mutex_)> Lock( Mutex_ );
#ifdef MTX__CONTROL
			if ( IsReleased_() ) {
				Lock.unlock();	// In case of the use of the 'setjmp' version or the error library.
				qRFwk();
			}
#endif
			if ( IsDisabled_() )
				return true;

			if ( IsLocked_() ) {
				if ( ( TimeOut == 0 ) || !Core_.wait_for( Lock, std::chrono::milliseconds( TimeOut ), [this]() {return State_ == sFree;} ) )	// third parameter to handle spurious awake.
					return false;
			}

			State_ = sLocked;

			return true;
		}
		bso::sBool Unlock( qRPN )
		{
			std::unique_lock<decltype(Mutex_)> Lock( Mutex_ );

			if ( IsDisabled_() )
				return true;

			if ( !IsLocked_() ) {
				if ( ErrHandling == err::hThrowException ) {
					Lock.unlock();	// In case of the use of the 'setjmp' version or the error library.
					qRFwk();
				} else
					return false;
			}

			State_ = sFree;

			Lock.unlock();

			Core_.notify_one();

			return true;
		}
		_mutex__( bso::bool__ Disabled )
		{
			State_ = Disabled ? sDisabled : sFree;
		}
		~_mutex__( void )
		{
#ifdef MTX__CONTROL
			Release();
#endif
		}
	} *handler___;

	E_CDEF( handler___, Undefined, NULL );

	//f Return a new mutex handler.
	inline handler___ Create( bso::bool__ Disabled = false )	// Si True, utilisation dans un contexte mono-thread.
	{
		handler___ Handler;

		if ( ( Handler = new _mutex__( Disabled ) ) == NULL )
			qRAlc();

		return Handler;
	}

	inline bso::bool__ IsLocked( handler___ Handler )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			qRFwk();
#endif
		return Handler->IsLocked();
	}


	inline bso::bool__ TryToLock(
		handler___ Handler,
		tol::sDelay TimeOut = 0 )	// Returns 'true' if lock successful, or return false after timeout.
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			qRFwk();
#endif
		return Handler->TryToLock( TimeOut );
	}

	inline void Lock( handler___ Handler )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			qRFwk();
#endif
		return Handler->Lock();
	}

	inline bso::sBool Lock(
		handler___ Handler,
		tol::sDelay TimeOut )	// Returns 'true' as soon as the lock succeeds, or false if timeout expired.
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			qRFwk();
#endif
		if ( TimeOut == 0 )
			qRFwk();

		return Handler->TryToLock( TimeOut );
	}

	//f Unlock 'Handler'.
	inline bso::sBool Unlock(
		handler___ Handler,
		qRPD )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			qRFwk();
#endif
		return Handler->Unlock( ErrHandling );
	}

	//f Delete the mutex of handler 'Handler'.
	inline void Delete(
		handler___ Handler,
		bso::bool__ EvenIfLocked = false )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			qRFwk();

		if ( Handler->IsLocked() && !EvenIfLocked )
			qRFwk();
#endif
		delete Handler;
	}

/*
	enum state__ {
		sUnlocked,
		sLocked,
		s_amount,
		s_Undefined
	};
*/
	class mutex___
	{
	private:
		qPMV( _mutex__, H_, Handler_ );
		void _UnlockIfInitializedAndLocked( void )
		{
			if ( Handler_ != NULL )
				mtx::Unlock( Handler_, err::hUserDefined );	// Ignore error if already unlocked;
		}
	public:
		void reset( bool P = true )
		{
			if ( P )
				_UnlockIfInitializedAndLocked();

			Handler_ = NULL;

		}
		E_CDTOR( mutex___ );
		void Init(
			handler___ Handler,
			bso::sBool Lock )
		{
			_UnlockIfInitializedAndLocked();

			Handler_ = Handler;

			if ( Lock )
				this->Lock();
		}
		void InitAndLock( handler___ Handler )
		{
			Init( Handler, true );
		}
		bso::bool__ TryToLock( tol::sDelay TimeOut = 0 )	// Returns 'true' if lock successful, or return false after timeout.
		{
			return mtx::TryToLock( H_(), TimeOut );
		}
		void Lock( void )
		{
			return mtx::Lock( H_() );
		}
		bso::sBool Lock(
			handler___ Handler,
			tol::sDelay TimeOut )	// Returns 'true' as soon as the lock succeeds, or false if timeout expired.
		{
			return mtx::Lock( H_(), TimeOut );
		}
		bso::sBool Unlock( qRPD )
		{
			return mtx::Unlock( H_(), ErrHandling );
		}
	};
}

/***************/
/***** NEW *****/
/***************/

namespace mtx {
	typedef handler___ rMutex;	// The mutex itself; was 'rHandle'.
	typedef mutex___ rHandle;	// To handle the mutex; wax 'rMutex'.
}

#endif
