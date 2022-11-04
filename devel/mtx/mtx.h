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

# ifdef rand
#  define MTX_RAND_BUFFER_ rand
#  undef rand
# endif

# include <condition_variable>
# include <mutex>

# ifdef MTX_SYSTEM_BUFFER_
#  define system MTX_SYSTEM_BUFFER_
#  undef MTX_SYSTEM_BUFFER_
# endif

# ifdef MTX_RAND_BUFFER_
#  define rand MTX_RAND_BUFFER_
#  undef MTX_RAND_BUFFER_
# endif


/***************/
/***** OLD *****/
/***************/

# define MTX__INVALID_HANDLER	NULL	// Internal use.
# define MTX_INVALID_HANDLER	MTX__INVALID_HANDLER	// For users.

namespace mtx {

  qENUM( Behavior ) {
    bKeep, // The lock is keeped once obtained.
    bRelease, // The lock is released once obtained,
    b_amount,
    b_Undefined,
    b_Default = bKeep
  };

	qENUM( State_ )	{
//		sFree,  //: Replaced by already existing but obviously not used 'sRelease'.
		sLocked,
		sReleased, // '_' to test if not otherwise used.
		sDisabled,
		s_amount,
		s_Undefined
	};

	typedef struct _mutex__ {
	private:
		std::mutex Guard_;
		std::condition_variable Core_;
		eState_ State_;
    bso::sBool *IsLockedFlag_; // '*IsLockedFlag_' (the boolean) reflects the status (locked or not) if != NULL.
                                // 'IsLockedFlag_' (the pointer) is potentially set by user.
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
		void SetState_(
      eState_ State,
      eBehavior Behavior)
		{
		  if ( ( State == sLocked ) && ( Behavior == bRelease ) )
        State = sReleased;

		  if ( IsLockedFlag_ != NULL ) {
        switch ( State ) {
        case sReleased:
        case sDisabled:
          *IsLockedFlag_ = false;
          break;
        case sLocked:
          *IsLockedFlag_ = true;
          break;
        default:
          qRFwk();
          break;
        }
		  }

		  State_ = State;
		}
	public:
#ifdef MTX__CONTROL
		void Release( void )
		{
			ststd::unique_lock<decltype(Guard_)> Guard(Guard_);

			SetState_(s_Released);;
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
			std::unique_lock<decltype(Guard_)> Guard(Guard_);
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
		void SetIsLockedFlag(bso::sBool *IsLockedFlag)
		{
			std::unique_lock<decltype(Guard_)> Guard(Guard_);

		  IsLockedFlag_ = IsLockedFlag;

		  SetState_(State_, b_Undefined);  // Sets '*IsLockedFlag_'.
		}
		bso::sBool Lock(eBehavior Behavior) // Returns true when the lock was obtained immediately.
		{
		  bso::sBool WasNotLocked = false;
			std::unique_lock<decltype(Guard_)> Guard(Guard_);
#ifdef MTX__CONTROL
			if ( IsReleased_() ) {
				Lock.unlock();	// In case of the use of the 'setjmp' version or the error library.
				qRFwk();
			}
#endif
			if ( IsDisabled_() )
				return true;

			if ( IsLocked_() )
				Core_.wait(Guard, [this]() {return State_ == sReleased;});	// third parameter to handle spurious awake.
      else
        WasNotLocked = true;

			SetState_(sLocked, Behavior);

			return WasNotLocked;
		}
		bso::bool__ TryToLock(
      eBehavior Behavior,
      tol::sDelay TimeOut )
		{
			std::unique_lock<decltype(Guard_)> Guard(Guard_);
#ifdef MTX__CONTROL
			if ( IsReleased_() ) {
				Lock.unlock();	// In case of the use of the 'setjmp' version or the error library.
				qRFwk();
			}
#endif
			if ( IsDisabled_() )
				return true;

			if ( IsLocked_() ) {
				if ( ( TimeOut == 0 ) || !Core_.wait_for(Guard, std::chrono::milliseconds(TimeOut), [this]() {return State_ == sReleased;}) )	// third parameter to handle spurious awake.
					return false;
			}

			SetState_(sLocked, Behavior);

			return true;
		}
		bso::sBool Unlock( qRPN )
		{
			std::unique_lock<decltype(Guard_)> Guard(Guard_);

			if ( IsDisabled_() )
				return true;

			if ( !IsLocked_() ) {
				if ( ErrHandling == err::hThrowException ) {
					Guard.unlock();	// In case of the use of the 'setjmp' version or the error library.
					qRFwk();
				} else
					return false;
			}

			SetState_(sReleased, b_Undefined);

			Guard.unlock();

			Core_.notify_one();

			return true;
		}
		_mutex__(
      bso::bool__ Disabled,
      bso::sBool *IsLockedFlag )
		{
		  IsLockedFlag_ = IsLockedFlag;
			SetState_(Disabled ? sDisabled : sReleased, b_Undefined);
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
	inline handler___ Create(
    bso::bool__ Disabled = false,
    bso::sBool *IsLockedFlag = NULL)	// Si True, utilisation dans un contexte mono-thread.
	{
		handler___ Handler;

		if ( ( Handler = new _mutex__(Disabled, IsLockedFlag) ) == NULL )
			qRAlc();

		return Handler;
	}

	inline handler___ Create(bso::sBool *IsLockedFlag)
	{
	  return Create(false, IsLockedFlag);
	}

	inline handler___ Create(bso::sBool Disabled)
	{
	  return Create(Disabled, NULL);
	}

	inline bso::bool__ IsLocked( handler___ Handler )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			qRFwk();
#endif
		return Handler->IsLocked();
	}

	inline void SetIsLockedFlag(
    handler___ Handler,
    bso::sBool *IsLockedFlag)
  {
    return Handler->SetIsLockedFlag(IsLockedFlag);
  }

	inline bso::bool__ TryToLock(
		handler___ Handler,
		eBehavior Behavior = b_Default,
		tol::sDelay TimeOut = 0)	// Returns 'true' if lock successful, or return false after timeout.
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			qRFwk();
#endif
		return Handler->TryToLock(Behavior, TimeOut);
	}

	inline bso::bool__ TryToLock(
		handler___ Handler,
		tol::sDelay TimeOut,
		eBehavior Behavior = b_Default)	// Returns 'true' if lock successful, or return false after timeout.
  {
    return TryToLock(Handler, Behavior, TimeOut);
  }

	inline bso::sBool Lock(
    handler___ Handler,
    eBehavior Behavior = b_Default)
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			qRFwk();
#endif
		return Handler->Lock(Behavior);
	}

	inline bso::sBool Lock(
		handler___ Handler,
		tol::sDelay TimeOut,
		eBehavior Behavior = b_Default)	// Returns 'true' as soon as the lock succeeds, or false if timeout expired.
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			qRFwk();
#endif
		if ( TimeOut == 0 )
			qRFwk();

		return Handler->TryToLock(Behavior, TimeOut);
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
		void InitAndLock(handler___ Handler)
		{
			Init( Handler, true );
		}
		bso::sBool IsInitialized(void) const
		{
		  return Handler_ != NULL;
		}
		bso::bool__ TryToLock(
      eBehavior Behavior = b_Default,
      tol::sDelay TimeOut = 0)	// Returns 'true' if lock successful, or return false after timeout.
		{
			return mtx::TryToLock( H_(), Behavior, TimeOut );
		}
		bso::bool__ TryToLock(
      tol::sDelay TimeOut,
      eBehavior Behavior = b_Default)	// Returns 'true' if lock successful, or return false after timeout.
    {
      return TryToLock(Behavior, TimeOut);
    }
		bso::sBool Lock(eBehavior Behavior = b_Default)
		{
			return mtx::Lock(H_(), Behavior);
		}
		bso::sBool Lock(
      tol::sDelay TimeOut,
      eBehavior Behavior = b_Default)	// Returns 'true' as soon as the lock succeeds, or false if timeout expired.
		{
			return mtx::Lock(H_(), TimeOut, Behavior);
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
