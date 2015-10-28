/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#ifndef FDR__INC
#define FDR__INC

#define FDR_NAME		"FDR"

#define	FDR_VERSION	"$Revision: 1.18 $"

#define FDR_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( FDR_NODBG )
#define FDR_DBG
#endif

//D Flow DRiver 

# include "err.h"
# include "bso.h"
# include "strng.h"
# include "tol.h"

#ifdef FDR_THREAD_SAFE
#	define FDR__TS
#elif !defined( FDR_THREAD_UNSAFE )
#	ifdef CPE_F_MT
#		define FDR__TS
#	endif
#endif

#ifdef FDR__TS
#	include "mtx.h"
#	define FDR_NO_MUTEX	MTX_INVALID_HANDLER
	typedef mtx::handler___ mutex__;
#else
	typedef void *mutex__;
#	define FDR_NO_MUTEX	NULL
#endif

#ifdef FDR_DEFAULT_CACHE_SIZE
#	define FDR__DEFAULT_CACHE_SIZE	FDR_DEFAULT_CACHE_SIZE
#else
#	define FDR__DEFAULT_CACHE_SIZE	1024
#endif

# define FDR_SIZE_MAX	BSO_SIZE_MAX


// predeclaration
namespace flw
{
	class iflow__;
	class oflow__;
}

namespace fdr {
		//t Amount of data.
	using bso::size__;

	enum thread_safety__
	{
		tsEnabled,
		tsDisabled,
		ts_amount,
		ts_Undefined,
		ts_Default =
#ifdef FDR__TS
		tsEnabled
#else
		tsDisabled
#endif
	};

	enum behavior__ {
		bNonBlocking,	// Au moins un octet est lu, davantage si cela n'entrne pas de blocage.
		bBlocking,		// Sauf si 'EOF', le nombre d'octets demand sera lu, mme si blocage.
		bKeep,			// Sauf si 'EOF', le nombre d'octets demands sera lu, mme si blocage, mais ils restent dans le flux.
		b_amount,
		b_Undefined
	};

	//d The max value for a amount.
#	define FDR_SIZE_MAX			BSO_SIZE_MAX

	// Unit of digital information, without any intrinsec signification.
	using bso::byte__;

#ifdef FDR__TS
	inline void Test_( mutex__ Mutex )
	{
		if ( Mutex == FDR_NO_MUTEX )
			qRFwk();
	}
#endif

	inline mutex__ Create_( thread_safety__ ThreadSafety )
	{
		switch ( ThreadSafety ) {
		case tsEnabled:
#ifdef FDR__TS
			return mtx::Create();
#else
			qRFwk();
#endif
			break;
		case tsDisabled:
			return FDR_NO_MUTEX;
			break;
		default:
			qRFwk();
			break;
		}

		return FDR_NO_MUTEX;	// Pour viter un 'warning'.
	}


	inline void Delete_( mutex__ Mutex )
	{
#ifdef FDR__TS
		if ( Mutex != FDR_NO_MUTEX )
			mtx::Delete( Mutex );
#endif
	}

	inline void Lock_( mutex__ Mutex )
	{
#ifdef FDR__TS
		if ( Mutex != FDR_NO_MUTEX )
			mtx::Lock( Mutex );
#endif
	}

	inline bso::bool__ TryToLock_( mutex__ Mutex )
	{
#ifdef FDR__TS
		if ( Mutex != FDR_NO_MUTEX )
			return mtx::TryToLock(  Mutex );
#endif
		return true;
	}

	inline void Unlock_( mutex__ Mutex )
	{
#ifdef FDR__TS
		if ( Mutex != FDR_NO_MUTEX )
			mtx::Unlock( Mutex );
	#endif
	}

	inline bso::bool__ IsLocked_( mutex__ Mutex )
	{
#ifdef FDR__TS
		if ( Mutex != FDR_NO_MUTEX )
			return mtx::IsLocked( Mutex );
#endif
		return false;
	}

	template <typename flow> class _flow_driver_base__
	{
	private:
		mutex__ _Mutex;	// Mutex pour protger la ressource.
		const flow *_User;
	protected:
		void Lock( const flow *User )
		{
#ifdef FDR__TS
			if ( _Mutex != mtx::UndefinedHandler ) {
				if ( TryToLock_(_Mutex) ) {
					if ( _User != NULL )
						qRFwk();

					_User = User;
				} else if ( _User != User ) {
					Lock_( _Mutex );
					_User = User;
				}
			}
# else
			Lock_( _Mutex );
# endif		
		}
		void Unlock( const flow *User )
		{
#ifdef FDR__TS
			if ( _Mutex != mtx::UndefinedHandler ) {
				if ( IsLocked_( _Mutex ) ) {
					if ( ( _User != User) && ( User != NULL ) )
						qRFwk();

					Unlock_( _Mutex );

					_User = NULL;
				}
			}
# else
			Unlock_( _Mutex );
# endif
		}
		void Init( thread_safety__ ThreadSafety )
		{
			reset();

			_Mutex = Create_( ThreadSafety );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				Delete_( _Mutex );

			_User = NULL;
			_Mutex = NULL;
		}
		E_CVDTOR( _flow_driver_base__ );
		bso::bool__ IsLocked( void )
		{
			return IsLocked_( _Mutex );
		}
	};

	class iflow_driver_base___
	: public _flow_driver_base__<flw::iflow__>
	{
	private:
		byte__ *_Cache;
		size__ _Size;	// Si == '0', signale 'EOF' atteint.
		size__ _Available;
		size__ _Position;
		size__ _Read(
			size__ Wanted,
			byte__ *Buffer )	// Si valeur retourne == 0, alors , alors 'EOF' atteint.
		{
# ifdef FDR_DBG
			if ( Wanted == 0 )
				qRFwk();
# endif
			if ( _Size != 0 )
				return FDRRead( Wanted, Buffer );
			else
				return 0;
		}
		size__ _LoopingRead(
			size__ Wanted,
			byte__ *Buffer )	// Si valeur retourne diffrent de 'Wanted', alors 'EOF' atteint.
		{
			size__ Red = 0, PonctualRed = 0;

			while ( ( Red < Wanted ) && ( ( PonctualRed = _Read( Wanted - Red, Buffer + Red ) ) != 0 ) )
				Red += PonctualRed;

			return Red;
		}
		size__ _FillCache( size__ Size )	// Si != 0, alors on fait le maximum pour lire la quantit demande. Sinon, on en lit au moins 1, sauf si 'EOF'.
		{
#ifdef FDR_DBG
			if ( _Cache == NULL )
				qRFwk();

			if ( _Available != 0 )
				qRFwk();
#endif
			_Position = 0;

			if ( _Size < Size )
				qRFwk();

			if ( Size != 0 ) {
				_Available = _LoopingRead( Size, _Cache );

				if ( _Available < Size )
					_Size = 0;	// Pour signaler 'EOF' atteint.
			} else {
				_Available = _Read( _Size, _Cache );

				if ( _Available == 0 )
					_Size = 0;	// Pour signaler 'EOF' atteint.
			}

			return _Available;
		}
		void _CompleteCache( size__ Size )	// Fait le maximum pour que le cache, avec les donnes dj disponibles, contienne la quantit demande.
		{
			if ( _Size == 0 )	// Plus de donne disponibles.
				return;

			if ( _Available < Size ) {
				if ( ( _Size - _Position ) < Size ) {
					if ( _Size < Size )
						qRFwk();

					if ( _Available != 0 )
						memcpy( _Cache, _Cache + _Position, _Available );

					_Position = 0;
				}

				_Available += _LoopingRead( Size - _Available, _Cache + _Position + _Available );

				if ( _Available < Size )
					_Size = 0;	// Pour signaler 'EOF' atteint.
			}
		}
		size__ _ReadFromCache(
			size__ Size,
			byte__ *Buffer,
			bso::bool__ Adjust )
		{
			if ( Size > _Available )
				Size = _Available;

			if ( _Available != 0 )  {
				memcpy( Buffer, _Cache + _Position, Size );

				if ( Adjust ) {
					_Available -= Size;
					_Position += Size;
				}
			}

			return Size;
		}
		size__ _ReadThroughCache(
			size__ Size,
			byte__ *Buffer,
			bso::bool__ Force )	// Si == 'true', on fait le maximum pour lire la quantite demande.
		{
			size__ Red = _ReadFromCache( Size, Buffer, true );

			if ( Red < Size )  {
				if ( Force )
					_FillCache( Size - Red );
				else if ( Red == 0 )
					_FillCache( 0 );
				else
					return Red;

				Red += _ReadFromCache( Size - Red, Buffer + Red, true );
			}

			return Red;
		}

		bso::bool__ _EOF( void )
		{
			if ( _Available ) 
				return false;
			else if ( _Size == 0 )
				return true;
			else
				return _FillCache( 0 ) == 0;
		}
	protected:
		// Retourne le nombre d'octets effectivement lus. Ne retourne '0' que si plus aucune donne n'est disponibe.
		virtual size__ FDRRead(
			size__ Maximum,
			byte__ *Buffer ) = 0;
		virtual void FDRDismiss( void ) = 0;
	public:
		void reset( bso::bool__ P = true ) 
		{
			if ( P ) {
				Dismiss( NULL );
			}

			_Cache = NULL;
			_Size = _Available = _Position = 0;
			_flow_driver_base__::reset( P );
		}
		E_CVDTOR( iflow_driver_base___ );
		void Init(
			byte__ *Cache,
			size__ Size,
			thread_safety__ ThreadSafety )
		{
			reset();
#ifdef FDR_DBG
			if ( Cache == NULL )
				qRFwk();
#endif

			_Cache = Cache;
			_Size = Size;

			_Available = _Position = 0;
			_flow_driver_base__::Init( ThreadSafety );
		}
		void Dismiss( flw::iflow__ *User )
		{
			if ( _Cache != NULL ) {
				FDRDismiss();
				Unlock( User );
			}
		}
		size__ Read(
			const flw::iflow__ *User,
			size__ Wanted,
			byte__ *Buffer,
			behavior__ Behavior )
		{
#ifdef FDR_DBG
			if ( Wanted < 1 )
				qRFwk();
#endif
			Lock( User );

			if ( _EOF() )
				return 0;

			switch ( Behavior ) {
			case bNonBlocking:
				return _ReadThroughCache( Wanted, Buffer, false );
				break;
			case bBlocking:

				if ( ( _Available >= Wanted ) || ( _Size > ( Wanted - _Available ) ) )
					return _ReadThroughCache( Wanted, Buffer, true );
				else {
					size__ Red = _ReadFromCache( Wanted, Buffer, true );

					if ( Red < Wanted )
						Red += _LoopingRead( Wanted - Red, Buffer + Red );

					return Red;
				}

				break;
			case bKeep:
				_CompleteCache( Wanted );

				return _ReadFromCache( Wanted, Buffer, false );
				break;
			default:
				qRFwk();
				break;
			}


			return 0;	// Pour viter un 'warning'.
		}
		bso::bool__ IsCacheEmpty( bso::size__ *Available = NULL ) const
		{
			if ( Available != NULL )
				*Available = _Available;

			return _Available == 0;
		}
		bso::bool__ EndOfFlow( const flw::iflow__ *User )
		{
			Lock( User );

			return _EOF();
		}
		bso::bool__ IFlowIsLocked( void )	// Simplifie l'utilisation de 'ioflow_driver_...'
		{
			return IsLocked();
		}
	};

	template <int cache_size = FDR__DEFAULT_CACHE_SIZE> class iflow_driver___
	: public iflow_driver_base___
	{
	private:
		byte__ _Cache[cache_size+1];	// '+1' pour grer le 'Unget()'.
	public:
		void reset( bso::bool__ P = true )
		{
			iflow_driver_base___::reset( P );
		}
		E_CVDTOR( iflow_driver___ );
		void Init( thread_safety__ ThreadSafety )
		{
			iflow_driver_base___::Init( _Cache, sizeof( _Cache ), ThreadSafety );
		}
	};

	class oflow_driver_base___
	: public _flow_driver_base__<flw::oflow__>
	{
	private:
		bso::bool__ _Initialized;	// Pour viter des 'pure virtual function call'.
	protected:
		// Retourne le nombre d'octets effectivement crits. Ne retourne '0' que si plus aucune donne ne peut tre crite.
		virtual size__ FDRWrite(
			const byte__ *Buffer,
			size__ MAximum ) = 0;
		virtual void FDRCommit( void ) = 0;
	public:
		void reset( bso::bool__ P = true ) 
		{
			if ( P ) {
				Commit( NULL );
			}

			_Initialized = false;
			_flow_driver_base__::reset( P );
		}
		E_CVDTOR( oflow_driver_base___ );
		void Init( thread_safety__ ThreadSafety )
		{
			reset();

			_Initialized = true;
			_flow_driver_base__::Init( ThreadSafety );
		}
		void Commit( const flw::oflow__ *User  )
		{
			if ( _Initialized ) {
				FDRCommit();
				Unlock( User );
			}
		}
		size__ Write(
			const flw::oflow__ *User,
			const byte__ *Buffer,
			size__ Maximum )
		{
			Lock( User );
			return FDRWrite( Buffer, Maximum );
		}
		bso::bool__ OFlowIsLocked( void )	// Simplifie l'utilisation de 'ioflow_driver_...'
		{
			return IsLocked();
		}
	};

	// Uniquement pour avoir une symtrie par rapport  'iflow_driver___'.
	template <int Dummy = 0> class oflow_driver___
	: public oflow_driver_base___
	{
	public:
		void reset( bso::bool__ P = true )
		{
			if ( Dummy != 0 )	
				qRFwk();	// 'Dummy' n'tant pas utilis, rien ne sert de modifier sa valeur.

			oflow_driver_base___::reset( P );
		}
		E_CVDTOR( oflow_driver___ );
	};

	class ioflow_driver_base___
	: public iflow_driver_base___,
	  public oflow_driver_base___
	{
	public:
		void reset( bso::bool__ P = true )
		{
			iflow_driver_base___::reset( P );
			oflow_driver_base___::reset( P );
		}
		void Init(
			byte__ *InputCache,
			size__ InputCacheSize,
			thread_safety__ ThreadSafety )
		{
			iflow_driver_base___::Init( InputCache, InputCacheSize, ThreadSafety );
			oflow_driver_base___::Init( ThreadSafety );
		}
	};

	template <int input_cache_size = FDR__DEFAULT_CACHE_SIZE> class ioflow_driver___
	: public ioflow_driver_base___
	{
	private:
		byte__ _InputCache[input_cache_size];
	public:
		void Init( thread_safety__ ThreadSafety )
		{
			ioflow_driver_base___::Init( _InputCache, sizeof( _InputCache ), ThreadSafety );
		}
	};
}

#endif
