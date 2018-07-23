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

#ifndef MTX__INC
# define MTX__INC

# define MTX_NAME		"MTX"

# if defined( E_DEBUG ) && !defined( MTX_NODBG )
#  define MTX_DBG
# endif

// MuTeX

# include "bso.h"
# include "cpe.h"
# include "tol.h"

/***************/
/***** OLD *****/
/***************/

//# define MTX_NATIVE		// Use native library.
//# define MTX_PTHREAD	// Use pthread library.
//# define MTX_BASIC		// Use basic operations (atomicity NOY guaranted !).
// By default, the <atomic> library is used.

# ifdef E_MUTEXES_DEFAULT_DELAY
#  define MTX__DEFAULT_DELAY	E_MUTEXES_DEFAULT_DELAY
# else
#  define MTX__DEFAULT_DELAY	1
# endif

# ifdef MTX_NATIVE
#  if defined( CPE_S_WIN )
#   define MTX__WINDOWS
#  elif defined ( CPE_S_LINUX )
#    define MTX__LINUX
#  elif defined (CPE_S_DARWIN )
#    define MTX__DARWIN
#  else
#   error
#  endif
# elif defined( MTX_PTHREAD )
#  ifdef CPE_S_POSIX
#   define MTX__PTHREAD
#  else
#   error
# endif
# elif defined( MTX_BASIC )
#  define MTX__BASIC
# else
#  define MTX__ATOMIC
# endif

# if defined( MTX__WINDOWS ) || defined( MTX__LINUX ) || defined( MTX__DARWIN )
#  define MTX__MESSAGE	"CAUTION : Mutexes based on native atomic operations !!!"
# elif defined( MTX__PTHREAD )
#   define MTX__MESSAGE	"CAUTION : Mutexes based on pthread !!!"
# elif defined( MTX__BASIC )
#   define MTX__MESSAGE	"CAUTION : Mutexes based on basic operations !!!"
# elif !defined( MTX__ATOMIC ) 
#  error
# endif

# ifdef MTX__MESSAGE
#  ifndef MTX_SUPPRESS_WARNING
#   ifdef CPE_C_GCC
#    pragma message __LOC__ MTX__MESSAGE
#   else
#    pragma message( __LOC__ MTX__MESSAGE )
#   endif
#  endif
# endif

# ifdef MTX__ATOMIC
# elif defined( MTX__DARWIN )
# elif defined( MTX__LINUX )
# elif defined( MTX__WINDOWS )
# elif defined( MTX__PTHREAD )
# elif defined( MTX__BASIC )
# else
#  error
# endif

# ifdef MTX__ATOMIC
#  define MTX__system	system
#  undef system
#  include <atomic>
#  define system MTX__system
# elif defined( MTX__DARWIN )
#  include <libkern/OSAtomic.h>
# elif defined( MTX__LINUX )
#  include <asm/atomic>
# elif defined( MTX__WINDOWS )
# elif defined( MTX__PTHREAD )
#  include <pthread.h>
# elif defined( MTX__BASIC )
# else
#  error
# endif

/*
	A cause du module 'err', dont l'objet peut tre utilis simumtanment dans un contexte 'single-threaded' et 'mono-threaded'
	(voir 'NOTA' dans le fichier d'entte de ce module), on est amen  utiliser ce module mme dans un contexte 'single-threaded'.
*/
# if 0
#  ifndef CPE_F_MT
#   error "This library only useful in multitasking context, in which you are not."
#  endif
# endif

# ifndef MTX_NO_CONTROL
#  ifdef MTX_DBG
#   define MTX__CONTROL
#  endif
# endif

//d A invalid mutex handler.
# define MTX__INVALID_HANDLER	NULL	// Utilisation interne.
# define MTX_INVALID_HANDLER		MTX__INVALID_HANDLER	// Pour les utilisateurs.


# define MTX__COUNTER_OVERFLOW_VALUE	BSO_S16_MIN

namespace mtx {
# ifdef MTX__ATOMIC
	typedef int counter_t__;
	typedef std::atomic<counter_t__> counter__;
# elif defined( MTX__DARWIN )
	typedef int32_t	counter_t__;
	typedef counter_t__ counter__;
# elif defined( MTX__LINUX )
	typedef atomic_t	counter_t__;
	typedef counter_t__ counter__;
# elif defined( MTX__WINDOWS )
	typedef LONG counter_t__;
	typedef counter_t__ counter__;
# elif defined( MTX__PTHREAD )
	typedef volatile bso::s16__ counter_t__;
	struct counter__ {
		counter_t__ Value;
		pthread_mutexattr_t MutexAttr;
		pthread_mutex_t Mutex;
	};
# elif defined( MTX__BASIC )
	typedef volatile int counter_t__;
	typedef counter_t__ counter__;
# else
#  error
# endif

# ifdef MTX__CONTROL
#  define MTX__RELEASED_MUTEX_COUNTER_VALUE	3
# endif

# define MTX__DISABLED_MUTEX_COUNTER_VALUE	2
# define MTX__UNLOCKED_MUTEX_COUNTER_VALUE	1
# define MTX__UNLOCKED_MUTEX_COUNTER_SIGN	MTX__UNLOCKED_MUTEX_COUNTER_VALUE

	inline void _Set(
		counter__ &Counter,
		int Value,
		bso::bool__ Destroy )
	{
# ifdef MTX__ATOMIC
//#  ifdef CPE_C_CLANG
# if FALSE	// Disabled as some old version of 'clang++' issues a linker error (see https://travis-ci.org/epeios-q37/xppq-cli/jobs/270384361).
		std::atomic_init( &Counter, Value );	// Warning : there seems to be a bug in g++ header about this function.
#  else
#   ifdef CPE_C_CLANG
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wbraced-scalar-init"	// To disable warning visible at https://travis-ci.org/epeios-q37/xppq-cli/jobs/233135308.
#   endif
		Counter = ATOMIC_VAR_INIT( Value );		// Is the correct way, but 'clang++' issues a warning, hence the 'pragma's above and below.
#   ifdef CPE_C_CLANG
#    pragma clang diagnostic pop
#   endif
#  endif
# elif defined( MTX__DARWIN )
		Counter = Value;
# elif defined( MTX__LINUX )
		atomic_set( &Counter, Value );
# elif defined( MTX__WINDOWS )
		Counter = Value;
# elif defined( MTX__PTHREAD )
		Counter.Value = Value;
		if ( Destroy ) {
			if ( pthread_mutex_destroy( &Counter.Mutex ) )
				qRFwk();
			if ( pthread_mutexattr_destroy( &Counter.MutexAttr ) )
				qRFwk();
		} else {
			if ( pthread_mutexattr_init( &Counter.MutexAttr ) )
				qRFwk();
			if ( pthread_mutexattr_setpshared( &Counter.MutexAttr, PTHREAD_PROCESS_PRIVATE ) )
				qRFwk();
			if ( pthread_mutex_init( &Counter.Mutex, &Counter.MutexAttr ) )
				qRFwk();
		}
# elif defined( MTX__BASIC )
		Counter = Value;
# else
#  error
# endif
	}

	inline counter_t__ _GetValue( counter__ &Counter )	// Retourne la valeur de 'Counter'.
	{

# ifdef MTX__ATOMIC
		return std::atomic_load( &Counter );
# elif defined( MTX__DARWIN )
		return Counter;
# elif defined( MTX__LINUX )
		return atomic_read( &Counter );
# elif defined( MTX__WINDOWS )
		return Counter;
# elif defined( MTX__PTHREAD )
		counter_t__ Buffer;
		if ( pthread_mutex_lock( &Counter.Mutex ) )
			qRFwk();
		Buffer = Counter.Value;
		if ( pthread_mutex_unlock( &Counter.Mutex ) )
			qRFwk();
		return Buffer;
# elif defined( MTX__BASIC )
		return Counter;
# else
#  error
# endif
	}

	inline bso::sign__ _GetSign( counter__ &Counter )	// Retourne le signe de 'Counter'.
	{
		return _GetValue( Counter );
	}

	inline void _Inc( counter__ &Counter )	// Incrmente 'Counter'.
	{
# ifdef MTX__ATOMIC
		++Counter;
# elif defined( MTX__DARWIN )
		OSAtomicIncrement32( &Counter );
# elif defined( MTX__LINUX )
		atomic_inc( &Counter );
# elif defined( MTX__WINDOWS )
		InterlockedIncrement( &Counter );
# elif defined( MTX__PTHREAD )
		if ( pthread_mutex_lock( &Counter.Mutex ) )
			qRFwk();
		++Counter.Value;
		if ( pthread_mutex_unlock( &Counter.Mutex ) )
			qRFwk();
# elif defined( MTX__BASIC )
		++Counter;
# else
#  error
# endif
	}

	inline bso::bool__ _DecAndTest( counter__ &Counter )	// Dcrmente 'Counter'.et retourne 'true' si  zro.
	{
# ifdef MTX__ATOMIC
		return --Counter == 0;
# elif defined( MTX__DARWIN )
//		return OSAtomicDecrement32( &Counter ) == 1;	// Il existe un autre 'OSAtomic.h', dans lequel 'OSAtomicDecrement(|8|16|64)(...)'. retourne la valeur AVANT dcrmentation.
														// Cependant, celui-ci n'a pas de fonction 'OSAtomicDecrement32'(...)".
		return OSAtomicDecrement32( &Counter ) == 0;
# elif defined( MTX__LINUX )
		return atomic_dec_and_test( &Counter );
# elif defined( MTX__WINDOWS )
		return InterlockedDecrement( &Counter ) == 0;
# elif defined( MTX__PTHREAD )
		bso::bool__ Buffer;
		if ( pthread_mutex_lock( &Counter.Mutex ) )
			qRFwk();
		Buffer = --Counter.Value == 0;
		if ( pthread_mutex_unlock( &Counter.Mutex ) )
			qRFwk();
		return Buffer;
# elif defined( MTX__BASIC )
		return --Counter == 0;
# else
#  error
# endif
	}

	//t A mutex handler.
	typedef struct _mutex__ {
		counter__ Counter;
#ifdef MTX__CONTROL
		void Release( bso::bool__ Destroy )	// 'Destruction'  vrai si appel par le destructeur.
		{
			_Set( Counter, MTX__RELEASED_MUTEX_COUNTER_VALUE, Destroy );
		}
		bso::bool__ IsReleased( void )
		{
			return _GetValue( Counter ) == MTX__RELEASED_MUTEX_COUNTER_VALUE;
		}
#endif
		bso::bool__ IsDisabled( void )
		{
			return _GetValue( Counter ) == MTX__DISABLED_MUTEX_COUNTER_VALUE;
		}
		bso::bool__ IsLocked( void )
		{
#ifdef MTX__CONTROL
			if ( IsReleased() )
				qRFwk();
#endif
			if ( IsDisabled() )
				return false;

			return _GetSign( Counter ) != MTX__UNLOCKED_MUTEX_COUNTER_SIGN;
		}
		bso::bool__ TryToLock( void )
		{
#ifdef MTX__CONTROL
			if ( IsReleased() )
				qRFwk();
#endif
			if ( IsDisabled() )
				return true;

			if ( IsLocked() )
				return false;

			if ( _GetValue( Counter ) == MTX__COUNTER_OVERFLOW_VALUE )
				qRLmt();

			if ( _DecAndTest( Counter ) )
				return true;
			else {
				_Inc( Counter );
				return false;
			}
		}
		void Unlock( void )
		{
			if ( IsDisabled() )
				return;

			if ( !IsLocked() )
				qRFwk();

			_Inc( Counter );
		}
		_mutex__( bso::bool__ Disabled )
		{
			_Set( Counter, ( Disabled ? MTX__DISABLED_MUTEX_COUNTER_VALUE: MTX__UNLOCKED_MUTEX_COUNTER_VALUE ), false );
		}
		~_mutex__( void )
		{
#ifdef MTX__CONTROL
			Release( true );
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


	inline bso::bool__ TryToLock( handler___ Handler)
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			qRFwk();
#endif
		return Handler->TryToLock();
	}

	void Defer_( void );

	// Wait until mutex unlocked.
	inline bso::sBool WaitUntilUnlocked_(
		handler___ Handler,
		tol::sDelay Timeout ) // If == '0' or lock succeed, returns always 'true', or returns false after 'Timeout' ms.
	{
		tol::sTimer Timer;
		bso::sBool NoTimeout = true;

		Timer.Init( Timeout );

		Timer.Launch();

		while( !TryToLock( Handler ) && ( NoTimeout = !Timer.IsElapsed() ) )
			Defer_();

		return NoTimeout;
	}

	// Lock 'Handler'. Blocks until lock succeed or after 'Timeout' ms.
	inline bso::sBool Lock(
		handler___ Handler,
		tol::sDelay Timeout = 0 ) // If == '0' or lock succeed, returns always 'true', or returns false after 'Timeout' ms.
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			qRFwk();
#endif
		if ( !TryToLock( Handler ) )
			return WaitUntilUnlocked_( Handler, Timeout );
		else
			return true;
	}

	//f Unlock 'Handler'.
	inline void Unlock( handler___ Handler )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			qRFwk();
#endif
		Handler->Unlock();
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

	enum state__ {
		sUnlocked,
		sLocked,
		s_amount,
		s_Undefined
	};

	class mutex___
	{
	private:
		state__ _State;
		handler___ _Handler;
		void _Test( void )
		{
# ifdef MTX_DBG
			if ( _Handler == MTX__INVALID_HANDLER )
				qRFwk();
# endif
		}
		void _UnlockIfInitializedAndLocked( void )
		{
			if ( _Handler != MTX__INVALID_HANDLER )
				if ( _State == sLocked ) {
					mtx::Unlock( _Handler );

					_State = sUnlocked;
				}
		}
	public:
		void reset( bool P = true )
		{
			if ( P )
				_UnlockIfInitializedAndLocked();

			_Handler = MTX__INVALID_HANDLER;
			_State = s_Undefined;

		}
		E_CDTOR( mutex___ );
		void Init( handler___ Handler )
		{
			_UnlockIfInitializedAndLocked();
			
			_Handler = Handler;

			_State = sUnlocked;
		}
		bso::bool__ TryToLock( void )
		{
			_Test();

			if ( _State == sLocked )
				return true;
			else if ( mtx::TryToLock( _Handler ) ) {
				_State = sLocked;
				return true;
			} else
				return false;
		}
		bso::sBool Lock( tol::sDelay Timeout = 0 ) // If == '0' or lock succeed, returns always 'true', or returns false after 'Timeout' ms.
		{
			_Test();

			if ( _State != sLocked ) {
				if ( mtx::Lock( _Handler, Timeout ) ) {
					_State = sLocked;
					return true;
				} else
					return false;
			} else
				return true;
		}
		void Unlock( void )
		{
			_Test();

			if ( _State == sUnlocked )
				qRFwk();

			_State = sUnlocked;

			mtx::Unlock( _Handler );
		}
	};
}

/***************/
/***** NEW *****/
/***************/

namespace mtx {
	typedef mutex___ rMutex;
	typedef handler___ rHandler;
}

#endif
