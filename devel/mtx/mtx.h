/*
	'mtx.h' by Claude SIMON (http://zeusw.org/).

	'mtx' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MTX__INC
# define MTX__INC

# define MTX_NAME		"MTX"

# if defined( E_DEBUG ) && !defined( MTX_NODBG )
#  define MTX_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// MuTeX

# include "bso.h"
# include "cpe.h"
# include "tht.h"
# include "tol.h"

# ifdef E_MUTEXES_DEFAULT_DELAY
#  define MTX__DEFAULT_DELAY	E_MUTEXES_DEFAULT_DELAY
# else
#  define MTX__DEFAULT_DELAY	1
# endif

# if !defined( E_MUTEXES_NO_ATOMIC_OPERATIONS )
#  define MTX__USE_ATOMIC_OPERATIONS
# endif

# ifdef MTX__USE_ATOMIC_OPERATIONS
#  if defined( CPE_WIN )
#   define MTX__USE_WIN_ATOMIC_OPERATIONS
#  elif defined( CPE_POSIX )
#   if defined ( CPE_LINUX )
#    if ( MTX__USE_ATOMIC_LIB )
#     define MTX__USE_LINUX_ATOMIC_OPERATIONS
#    else
#     define MTX__USE_PTHREAD_MUTEX
#    endif
#   elif defined (CPE_XCODE )
#    define MTX__USE_MAC_ATOMIC_OPERATIONS
#   else
#    define MTX__USE_PTHREAD_MUTEX
#   endif
#  else
#   error
#  endif
# else
#  define MTX__NO_ATOMIC_OPERATIONS
# endif

# ifndef MTX__USE_ATOMIC_OPERATIONS
#  ifndef MTX_SUPPRESS_NO_ATOMIC_OPERATIONS_WARNING
#   ifdef CPE_VC
#    pragma message( "BE CAREFUL : Mutexes do not use atomic operations !" )
#   elif defined( CPE_GCC )
#    pragma message( "BE CAREFUL : Mutexes do not use atomic operations !"
#   endif
#  endif
# endif

# ifdef MTX__USE_LINUX_ATOMIC_OPERATIONS
#  include "asm/atomic.h"
# endif

# ifdef MTX__USE_MAC_ATOMIC_OPERATIONS
#  include <libkern/OSAtomic.h>
# endif

# ifdef MTX__USE_PTHREAD_MUTEX
#  include <pthread.h>
# endif

# ifndef CPE_MT
#  error "This library only useful in multitasking context, in which you are not."
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

#ifdef	MTX__USE_WIN_ATOMIC_OPERATIONS
	typedef LONG counter__;
#elif defined( MTX__USE_LINUX_ATOMIC_OPERATIONS )
	typedef atomic_t	counter__;
#elif defined( MTX__USE_MAC_ATOMIC_OPERATIONS )
	typedef int32_t	counter__;
#elif defined ( MTX__USE_PTHREAD_MUTEX )
	struct counter__ {
		volatile bso::s16__ Value;
		pthread_mutexattr_t MutexAttr;
		pthread_mutex_t Mutex;
	};
#elif defined( MTX__NO_ATOMIC_OPERATIONS )
	typedef volatile bso::sshort__ counter__;
#else
#	error "No mutex handling scheme !"
#endif

#ifdef MTX__CONTROL
#	define MTX__RELEASED_MUTEX_COUNTER_VALUE	2
#endif

#define MTX__UNLOCKED_MUTEX_COUNTER_VALUE	1
#define MTX__UNLOCKED_MUTEX_COUNTER_SIGN	MTX__UNLOCKED_MUTEX_COUNTER_VALUE

	inline void _Set(
		counter__ &Counter,
		int Value,
		bso::bool__ Destroy )
	{
#ifdef	MTX__USE_WIN_ATOMIC_OPERATIONS
		Counter = Value;
#elif defined( MTX__USE_LINUX_ATOMIC_OPERATIONS )
		atomic_set( &Counter, Value );
#elif defined( MTX__USE_MAC_ATOMIC_OPERATIONS )
		Counter = Value;
#elif defined( MTX__USE_PTHREAD_MUTEX )
		Counter.Value = Value;
		if ( Destroy ) {
			if ( pthread_mutex_destroy( &Counter.Mutex ) )
				ERRFwk();
			if ( pthread_mutexattr_destroy( &Counter.MutexAttr ) )
				ERRFwk();
		} else {
			if ( pthread_mutexattr_init( &Counter.MutexAttr ) )
				ERRFwk();
			if ( pthread_mutexattr_setpshared( &Counter.MutexAttr, PTHREAD_PROCESS_PRIVATE ) )
				ERRFwk();
			if ( pthread_mutex_init( &Counter.Mutex, &Counter.MutexAttr ) )
				ERRFwk();
		}
#elif defined( MTX__NO_ATOMIC_OPERATIONS )
		Counter = Value;
#else
#	error "No mutex handling scheme !"
#endif
	}

	inline bso::sign__ _GetSign( counter__ &Counter )	// Retourne le signe de 'Counter'.
	{
#ifdef	MTX__USE_WIN_ATOMIC_OPERATIONS
		return Counter;
#elif defined( MTX__USE_LINUX_ATOMIC_OPERATIONS )
		return atomic_read( &Counter );
#elif defined( MTX__USE_MAC_ATOMIC_OPERATIONS )
		return Counter;
#elif defined( MTX__USE_PTHREAD_MUTEX )
		return Counter.Value;
#elif defined( MTX__NO_ATOMIC_OPERATIONS )
		return Counter;
#else
#	error "No mutex handling scheme !"
#endif
	}

	inline int _GetValue( counter__ &Counter )	// Retourne la valeur de 'Counter'.
	{
#ifdef	MTX__USE_WIN_ATOMIC_OPERATIONS
		return Counter;
#elif defined( MTX__USE_LINUX_ATOMIC_OPERATIONS )
		return atomic_read( &Counter );
#elif defined( MTX__USE_MAC_ATOMIC_OPERATIONS )
		return Counter;
#elif defined( MTX__USE_PTHREAD_MUTEX )
		return Counter.Value;
#elif defined( MTX__NO_ATOMIC_OPERATIONS )
		return Counter;
#else
#	error "No mutex handling scheme !"
#endif
	}

	inline void _Inc( counter__ &Counter )	// Incrémente 'Counter'.
	{
#ifdef	MTX__USE_WIN_ATOMIC_OPERATIONS
		InterlockedIncrement( &Counter );
#elif defined( MTX__USE_LINUX_ATOMIC_OPERATIONS )
		atomic_inc( &Counter );
#elif defined( MTX__USE_MAC_ATOMIC_OPERATIONS )
		OSAtomicIncrement32( &Counter );
#elif defined( MTX__USE_PTHREAD_MUTEX )
		if ( pthread_mutex_lock( &Counter.Mutex ) )
			ERRFwk();
		++Counter.Value;
		if ( pthread_mutex_unlock( &Counter.Mutex ) )
			ERRFwk();
#elif defined( MTX__NO_ATOMIC_OPERATIONS )
		++Counter;
#else
#	error "No mutex handling scheme !"
#endif
	}

	inline bso::bool__ _DecAndTest( counter__ &Counter )	// Décrémente 'Counter'.et retourne 'true' si à zéro.
	{
#ifdef	MTX__USE_WIN_ATOMIC_OPERATIONS
		return InterlockedDecrement( &Counter ) == 0;
#elif defined( MTX__USE_LINUX_ATOMIC_OPERATIONS )
		return atomic_dec_and_test( &Counter );
#elif defined( MTX__USE_MAC_ATOMIC_OPERATIONS )
//		return OSAtomicDecrement32( &Counter ) == 1;	// Il existe un autre 'OSAtomic.h', dans lequel 'OSAtomicDecrement(|8|16|64)(...)'. retourne la valeur AVANT décrémentation.
														// Cependant, celui-ci n'a pas de fonction 'OSAtomicDecrement32'(...)".
		return OSAtomicDecrement32( &Counter ) == 0;
#elif defined( MTX__USE_PTHREAD_MUTEX )
		bso::bool__ Buffer;
		if ( pthread_mutex_lock( &Counter.Mutex ) )
			ERRFwk();
		Buffer = --Counter.Value == 0;
		if ( pthread_mutex_unlock( &Counter.Mutex ) )
			ERRFwk();
		return Buffer;
#elif defined( MTX__NO_ATOMIC_OPERATIONS )
		return --Counter == 0;
#else
#	error "No mutex handling scheme !"
#endif
	}

	//t A mutex handler.
	typedef struct mutex__ {
		counter__ Counter;
#ifdef MTX__CONTROL
		void Release( bso::bool__ Destroy )	// 'Destruction' à vrai si appelé par le destructeur.
		{
			_Set( Counter, MTX__RELEASED_MUTEX_COUNTER_VALUE, Destroy );
		}
		bso::bool__ IsReleased( void )
		{
			return _GetValue( Counter ) == MTX__RELEASED_MUTEX_COUNTER_VALUE;
		}
#endif
		bso::bool__ IsLocked( void )
		{
#ifdef MTX__CONTROL
			if ( IsReleased() )
				ERRFwk();
#endif
			return _GetSign( Counter ) != MTX__UNLOCKED_MUTEX_COUNTER_SIGN;
		}
		bso::bool__ TryToLock( bso::bool__ *AlreadyLocked )	// 'AlreadyLocked' significatif seulement si valeur retournée == 'true'.
		{
#ifdef MTX__CONTROL
			if ( IsReleased() )
				ERRFwk();
#endif
			if ( IsLocked() )
				return false;

			if ( _GetValue( Counter ) == MTX__COUNTER_OVERFLOW_VALUE )
				ERRLmt();

			if ( _DecAndTest( Counter ) ) {
				if ( AlreadyLocked != NULL )
					*AlreadyLocked = false;
				return true;
			} else {
				_Inc( Counter );
				return false;
			}
		}
		void Unlock( void )
		{
			if ( !IsLocked() )
				ERRFwk();

			_Inc( Counter );
		}
		mutex__( void )
		{
			_Set( Counter, MTX__UNLOCKED_MUTEX_COUNTER_VALUE, false );
		}
		~mutex__( void )
		{
#ifdef MTX__CONTROL
			Release( true );
#endif
		}
	} *mutex_handler__;


	//f Return a new mutex handler.
	inline mutex_handler__ Create( void )
	{
		mutex_handler__ Handler;
		
		if ( ( Handler = new mutex__ ) == NULL )
			ERRAlc();

		return Handler;
	}

	inline bso::bool__ IsLocked( mutex_handler__ Handler )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			ERRPrm();
#endif
		return Handler->IsLocked();
	}


	//f Try to lock 'Handler' without blocking. Return 'true' if locks succeed, false otherwise.
	inline bso::bool__ TryToLock(
		mutex_handler__ Handler,
		bso::bool__ &AlreadyLocked = *(bso::bool__ *)NULL )	// 'AlreadyLocked' significatif seulement si valeur retournée == 'true'.
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			ERRPrm();
#endif
		return Handler->TryToLock( &AlreadyLocked );
	}

	// Wait until mutex unlocked.
	inline void WaitUntilUnlocked_( mutex_handler__ Handler )
	{
		while( !TryToLock( Handler ) )
		{
			tol::InitializeRandomGenerator();
			tht::Defer( ( (bso::uint__)tht::GetTID() + rand() ) % 5 + 1 ); // 'rand() donne-t'il la même suite dans des coeurs différents ?
		}
	}

	//f Lock 'Handler'. Blocks until lock succeed.
	inline bso::bool__ Lock( mutex_handler__ Handler )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			ERRPrm();
#endif
		bso::bool__ AlreadyLocked = false;

		if ( !TryToLock( Handler, AlreadyLocked ) )
			WaitUntilUnlocked_( Handler );

		return !AlreadyLocked;
	}

	//f Unlock 'Handler'.
	inline void Unlock( mutex_handler__ Handler )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			ERRPrm();
#endif
		Handler->Unlock();
	}

	//f Delete the mutex of handler 'Handler'.
	inline void Delete(
		mutex_handler__ Handler,
		bso::bool__ EvenIfLocked = false )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			ERRPrm();

		if ( Handler->IsLocked() && !EvenIfLocked )
			ERRPrm();
#endif
		delete Handler;
	}

	//c A mutex.
	class mutex___
	{
	private:
		// Mutex handler.
		mutex_handler__ Handler_;
	public:
		void reset( bool P = true )
		{
			Handler_ = MTX__INVALID_HANDLER;
		}
		mutex___( void )
		{
			reset( false );
		}
		~mutex___( void )
		{
			reset( true );
		}
		//f Initialization.with mutex handler 'Handler'.
		void Init( mutex_handler__ Handler )
		{
			reset();
			
			Handler_ = Handler;
		}
		//f Lock the semaphore.
		bso::bool__ Lock( void )
		{
#ifdef MTX_DBG
			if ( Handler_ == MTX__INVALID_HANDLER )
				ERRPrm();
#endif
			mtx::Lock( Handler_ );
		}
		//f Unlock the semaphore.
		void Unlock( void )
		{
#ifdef MTX_DBG
			if ( Handler_ == MTX__INVALID_HANDLER )
				ERRPrm();
#endif
			return mtx::Unlock( Handler_ );
		}
		//f Try to lock without blocking. Return true if success, false otherwise.
		bso::bool__ TryToLock( void )
		{
#ifdef MTX_DBG
			if ( Handler_ == MTX__INVALID_HANDLER )
				ERRPrm();
#endif
			return mtx::TryToLock( Handler_ );
		}
		bso::bool__ IsLocked( void )
		{
			return mtx::IsLocked( Handler_ );
		}
	};
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
