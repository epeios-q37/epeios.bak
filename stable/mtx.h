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

/*
	A cause du module 'err', dont l'objet peut être utilisé simumtanément dans un contexte 'single-threaded' et 'mono-threaded'
	(voir 'NOTA' dans le fichier d'entête de ce module), on est amené à utiliser ce module même dans un contexte 'single-threaded'.
*/
# if 0
#  ifndef CPE_MT
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
#ifdef	MTX__USE_WIN_ATOMIC_OPERATIONS
	typedef LONG counter_t__;
	typedef counter_t__ counter__;
#elif defined( MTX__USE_LINUX_ATOMIC_OPERATIONS )
	typedef atomic_t	counter_t__;
	typedef counter_t__ counter__;
#elif defined( MTX__USE_MAC_ATOMIC_OPERATIONS )
	typedef int32_t	counter_t__;
	typedef counter_t__ counter__;
#elif defined ( MTX__USE_PTHREAD_MUTEX )
	typedef volatile bso::s16__ counter_t__;
	struct counter__ {
		counter_t__ Value;
		pthread_mutexattr_t MutexAttr;
		pthread_mutex_t Mutex;
	};
#elif defined( MTX__NO_ATOMIC_OPERATIONS )
	typedef volatile bso::s16__ counter__;
#else
#	error "No mutex handling scheme !"
#endif

#ifdef MTX__CONTROL
#	define MTX__RELEASED_MUTEX_COUNTER_VALUE	3
#endif

#define MTX__DISABLED_MUTEX_COUNTER_VALUE	2
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

	inline counter_t__ _GetValue( counter__ &Counter )	// Retourne la valeur de 'Counter'.
	{
#ifdef	MTX__USE_WIN_ATOMIC_OPERATIONS
		return Counter;
#elif defined( MTX__USE_LINUX_ATOMIC_OPERATIONS )
		return atomic_read( &Counter );
#elif defined( MTX__USE_MAC_ATOMIC_OPERATIONS )
		return Counter;
#elif defined( MTX__USE_PTHREAD_MUTEX )
		counter_t__ Buffer;
		if ( pthread_mutex_lock( &Counter.Mutex ) )
			ERRFwk();
		Buffer = Counter.Value;
		if ( pthread_mutex_unlock( &Counter.Mutex ) )
			ERRFwk();
		return Buffer;
#elif defined( MTX__NO_ATOMIC_OPERATIONS )
		return Counter;
#else
#	error "No mutex handling scheme !"
#endif
	}

	inline bso::sign__ _GetSign( counter__ &Counter )	// Retourne le signe de 'Counter'.
	{
		return _GetValue( Counter );
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
	typedef struct _mutex__ {
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
		bso::bool__ IsDisabled( void )
		{
			return _GetValue( Counter ) == MTX__DISABLED_MUTEX_COUNTER_VALUE;
		}
		bso::bool__ IsLocked( void )
		{
#ifdef MTX__CONTROL
			if ( IsReleased() )
				ERRFwk();
#endif
			if ( IsDisabled() )
				return false;

			return _GetSign( Counter ) != MTX__UNLOCKED_MUTEX_COUNTER_SIGN;
		}
		bso::bool__ TryToLock( void )
		{
#ifdef MTX__CONTROL
			if ( IsReleased() )
				ERRFwk();
#endif
			if ( IsDisabled() )
				return true;

			if ( IsLocked() )
				return false;

			if ( _GetValue( Counter ) == MTX__COUNTER_OVERFLOW_VALUE )
				ERRLmt();

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
				ERRFwk();

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

	E_CDEF( handler___, UndefinedHandler, NULL );

	//f Return a new mutex handler.
	inline handler___ Create( bso::bool__ Disabled = false )	// Si True, utilisation dans un contexte mono-thread.
	{
		handler___ Handler;
		
		if ( ( Handler = new _mutex__( Disabled ) ) == NULL )
			ERRAlc();

		return Handler;
	}

	inline bso::bool__ IsLocked( handler___ Handler )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			ERRPrm();
#endif
		return Handler->IsLocked();
	}


	inline bso::bool__ TryToLock( handler___ Handler)
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			ERRPrm();
#endif
		return Handler->TryToLock();
	}

	// Wait until mutex unlocked.
	inline void WaitUntilUnlocked_( handler___ Handler )
	{
		while( !TryToLock( Handler ) )
		{
			tol::InitializeRandomGenerator();
			tht::Defer( ( (bso::uint__)tht::GetTID() + rand() ) % 5 + 1 ); // 'rand() donne-t'il la même suite dans des coeurs différents ?
		}
	}

	// Lock 'Handler'. Blocks until lock succeed.
	inline void Lock( handler___ Handler )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			ERRPrm();
#endif
		if ( !TryToLock( Handler ) )
			WaitUntilUnlocked_( Handler );
	}

	//f Unlock 'Handler'.
	inline void Unlock( handler___ Handler )
	{
#ifdef MTX_DBG
		if ( Handler == NULL )
			ERRPrm();
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
			ERRPrm();

		if ( Handler->IsLocked() && !EvenIfLocked )
			ERRPrm();
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
				ERRFwk();
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
		void Lock( void )
		{
			_Test();

			if ( _State != sLocked ) {
				mtx::Lock( _Handler );

				_State = sLocked;
			}
		}
		void Unlock( void )
		{
			_Test();

			if ( _State == sUnlocked )
				ERRFwk();

			_State = sUnlocked;

			mtx::Unlock( _Handler );
		}
	};
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
