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

// Flow DRiver

#ifndef FDR__INC
# define FDR__INC

# define FDR_NAME		"FDR"

# if defined( E_DEBUG ) && !defined( FDR_NODBG )
#  define FDR_DBG
# endif

# include "err.h"
# include "bso.h"
# include "strng.h"
# include "tht.h"
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
			mtx::Delete( Mutex, true );
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

	using tht::sTID;
	qCDEF( sTID, UndefinedTID, tht::Undefined );

	class _flow_driver_base__
	{
	private:
		mutex__ _Mutex;	// Mutex pour protger la ressource.
		sTID Owner_;
	protected:
		void Lock( void )
		{
#ifdef FDR__TS
			sTID Caller = tht::GetTID();

			if ( _Mutex != mtx::Undefined ) {
				if ( TryToLock_( _Mutex ) ) {
					if ( Owner_ != tht::Undefined )
						qRFwk();

					Owner_ = Caller;
				} else if ( Owner_ != Caller ) {
					Lock_( _Mutex );
					Owner_ = Caller;
				}
			}
# else
			Lock_( _Mutex );
# endif		
		}
		void Init( thread_safety__ ThreadSafety )
		{
			reset();

			_Mutex = Create_( ThreadSafety );
		}
		sTID BaseTake( sTID TID )
		{
#ifdef FDR__TS
			sTID Old = Owner_;

//			if ( Old != tht::Undefined ) {

				if ( TID == tht::Undefined )
					TID = tht::GetTID();

				Owner_ = TID;
//			}

			return Old;
#else
			return tht::Undefined;
#endif
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				Delete_( _Mutex );

			Owner_ = tht::Undefined;
			_Mutex = NULL;
		}
		E_CVDTOR( _flow_driver_base__ );
		bso::bool__ IsLocked( void )
		{
			return IsLocked_( _Mutex );
		}
		void Unlock( void )
		{
#ifdef FDR__TS
			sTID Caller = tht::GetTID();

			if ( _Mutex != mtx::Undefined ) {
				if ( IsLocked_( _Mutex ) ) {
					if ( Owner_ != Caller )
						qRFwk();

					Unlock_( _Mutex );

					Owner_ = tht::Undefined;
				}
			}
# else
			Unlock_( _Mutex );
# endif
		}
		sTID Owner( void ) const
		{
			return Owner_;
		}
	};

	inline sTID GetTID_(
		sTID Donwstream,
		sTID Base )
	{
		if ( Base != Donwstream )
			if ( Base == tht::Undefined )
				if ( Donwstream != tht::Undefined )
					return Donwstream;
				else
					qRFwk();
			else if ( Donwstream == tht::Undefined )
				return Base;
			else
				qRFwk();
		else
			return Base;

		return tht::Undefined;	// To avoid a warning.

	}

	class iflow_driver_base___
	: public _flow_driver_base__
	{
	private:
		byte__ *_Cache;
		size__ _Size;	// If == '0', report EOF.
		size__ _Available;
		size__ _Position;
		size__ Red_;	// Amount of red data since last dismiss (NOT physically red, but what was returned to user).
		bso::sBool DismissPending_;
		bso::sBool AutoDismissOnEOF_;	// If at 'true', 'Dismiss' is automatically called on EOF. Can be useful when the object is reused, i.e. when several 'Init(...)' are called.
		size__ _Read(
			size__ Wanted,
			byte__ *Buffer )	// id returns 0, the EOF reached.
		{
			size__ Amount = 0;
# ifdef FDR_DBG
			if ( Wanted == 0 )
				qRFwk();
# endif
			if ( _Size != 0 ) {
				Amount = FDRRead( Wanted, Buffer );

				if ( ( Amount == 0 ) && AutoDismissOnEOF_ && DismissPending_ )
					Dismiss( true );	// Relaying dismissing to underlying level on EOF.
				else if ( Amount != 0 )
					DismissPending_ = true;

				return Amount;
			} else
				return 0;
		}
		size__ _LoopingRead(
			size__ Wanted,
			byte__ *Buffer,
			size__ *TotalRed )	// Si valeur retourne diffrent de 'Wanted', alors 'EOF' atteint.
		{
			size__ Red = 0, PonctualRed = 0;

			while ( ( Red < Wanted ) && ( ( PonctualRed = _Read( Wanted - Red, Buffer + Red ) ) != 0 ) )
				Red += PonctualRed;

			if ( TotalRed != NULL )
				*TotalRed += Red;

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
				_Available = _LoopingRead( Size, _Cache, NULL );

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

				_Available += _LoopingRead( Size - _Available, _Cache + _Position + _Available, NULL );

				if ( _Available < Size )
					_Size = 0;	// Pour signaler 'EOF' atteint.
			}
		}
		size__ _ReadFromCache(
			size__ Size,
			byte__ *Buffer,
			bso::bool__ Adjust,
			size__ *TotalRed )
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

			if ( TotalRed != NULL )
				*TotalRed += Size;

			return Size;
		}
		size__ _ReadThroughCache(
			size__ Size,
			byte__ *Buffer,
			bso::bool__ Force,
			size__ *TotalRed )	// Si == 'true', on fait le maximum pour lire la quantite demande.
		{
			size__ Red = _ReadFromCache( Size, Buffer, true, TotalRed );

			if ( Red < Size )  {
				if ( Force )
					_FillCache( Size - Red );
				else if ( Red == 0 )
					_FillCache( 0 );
				else
					return Red;

				Red += _ReadFromCache( Size - Red, Buffer + Red, true, TotalRed );
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
		virtual void FDRDismiss( bso::sBool Unlock ) = 0;
		virtual sTID FDRITake( sTID Owner ) = 0;
	public:
		void reset( bso::bool__ P = true ) 
		{
			if ( P ) {
				Dismiss( true );
			}

			_Cache = NULL;
			_Size = _Available = _Position = 0;
			_flow_driver_base__::reset( P );
			Red_ = 0;
			DismissPending_ = false;
			AutoDismissOnEOF_ = false;
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
			Red_ = 0;
			DismissPending_ = false;

			_Available = _Position = 0;
			_flow_driver_base__::Init( ThreadSafety );
			AutoDismissOnEOF_ = false;
		}
		void SetAutoDismissOnEOF( bso::sBool Value = true )
		{
			AutoDismissOnEOF_ = Value;
		}
		sTID ITake( sTID Owner )
		{
			return GetTID_( FDRITake( Owner ), BaseTake( Owner ) );
		}
		void Dismiss( bso::sBool Unlock )	// When 'Unlock' is set to false, the 'Red_' value is NOT set to 0.
		{
			if ( DismissPending_ ) {
				if ( _Cache != NULL ) {
				qRH
				qRB
					FDRDismiss( Unlock );
				qRR
				qRT
					if ( Unlock )
						this->Unlock();
				qRE
				}

				if ( Unlock )
					Red_ = 0;

				DismissPending_ = false;
			}
		}
		size__ Read(
			size__ Wanted,
			byte__ *Buffer,
			behavior__ Behavior )
		{
#ifdef FDR_DBG
			if ( Wanted < 1 )
				qRFwk();
#endif
			Lock();

			if ( _EOF() )
				return 0;

			switch ( Behavior ) {
			case bNonBlocking:
				return _ReadThroughCache( Wanted, Buffer, false, &Red_ );
				break;
			case bBlocking:

				if ( ( _Available >= Wanted ) || ( _Size > ( Wanted - _Available ) ) )
					return _ReadThroughCache( Wanted, Buffer, true, &Red_ );
				else {
					size__ Red = _ReadFromCache( Wanted, Buffer, true, &Red );

					if ( Red < Wanted )
						Red += _LoopingRead( Wanted - Red, Buffer + Red, &Red_ );

					return Red;
				}

				break;
			case bKeep:
				_CompleteCache( Wanted );

				return _ReadFromCache( Wanted, Buffer, false, &Red_ );
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
		bso::bool__ EndOfFlow( void )
		{
			Lock();

			return _EOF();
		}
		bso::bool__ IFlowIsLocked( void )	// Simplifie l'utilisation de 'ioflow_driver_...'
		{
			return IsLocked();
		}
		size__ AmountRed( void ) const
		{
			return Red_;
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
	: public _flow_driver_base__
	{
	private:
		bso::bool__ _Initialized;	// Pour viter des 'pure virtual function call'.
		bso::sBool CommitPending_;
		size__ Written_;	// Amount of data written since last commit.
	protected:
		// Returns amount of written data. Returns '0' only when no other data can be written (deconnection...), otherwise must block.
		virtual size__ FDRWrite(
			const byte__ *Buffer,
			size__ Maximum ) = 0;
		// Returns 'false' when underlying write fails, 'true' otherwise.
		virtual void FDRCommit( bso::sBool Unlock ) = 0;
		virtual sTID FDROTake( sTID Owner ) = 0;
	public:
		void reset( bso::bool__ P = true ) 
		{
			if ( P ) {
				Commit( true );
			}

			_Initialized = false;
			CommitPending_ = false;
			_flow_driver_base__::reset( P );
			Written_ = 0;
		}
		E_CVDTOR( oflow_driver_base___ );
		void Init( thread_safety__ ThreadSafety )
		{
			reset();

			_Initialized = true;
			CommitPending_ = false;
			_flow_driver_base__::Init( ThreadSafety );
		}
		void Commit( bso::sBool Unlock )	// When 'Unlock' is set to false, the 'Written_' value is NOT set to 0.
		{
			if ( CommitPending_ ) {
				if ( _Initialized ) {
				qRH
				qRB
					FDRCommit( Unlock );
				qRR
				qRT
					if ( Unlock )
						this->Unlock();
				qRE
				}

				CommitPending_ = false;

				if ( Unlock )
					Written_ = 0;
			}
		}
		size__ Write(
			const byte__ *Buffer,
			size__ Maximum )
		{
			Lock();
			CommitPending_ = true;
			Maximum = FDRWrite( Buffer, Maximum );

			Written_ += Maximum;

			return Maximum;
		}
		sTID OTake( sTID Owner )
		{
			return GetTID_( FDROTake( Owner ), BaseTake( Owner ) );
		}
		bso::bool__ OFlowIsLocked( void )	// Simplifie l'utilisation de 'ioflow_driver_...'
		{
			return IsLocked();
		}
		// Returns amount of data written since last commit.
		size__ AmountWritten( void ) const
		{
			return Written_;
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
		qCVDTOR( ioflow_driver_base___ );
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

/**************/
/**** NEW *****/
/**************/

namespace fdr {
# if 1	// Deprecated.
	typedef fdr::iflow_driver___<> rRFlow;
	typedef fdr::oflow_driver___<> rWFlow; 
	typedef fdr::ioflow_driver___<> rRWFlow;
# endif

	typedef fdr::iflow_driver_base___ rRDriver;
	typedef fdr::oflow_driver_base___ rWDriver; 
	typedef fdr::ioflow_driver_base___ rRWDriver;

	typedef fdr::iflow_driver___<> rRDressedDriver;
	typedef fdr::oflow_driver___<> rWDressedDriver;
	typedef fdr::ioflow_driver___<> rRWDressedDriver;

	typedef fdr::size__ sSize;
	typedef fdr::byte__ sByte;

	typedef fdr::thread_safety__ eThreadSafety;

	void Copy_(
		rRDriver &RDriver,
		sByte *Buffer,
		sSize BufferSize,
		rWDriver &WDriver );


	template <int BufferSize = 1024> inline void Copy(
		fdr::rRDriver &RDriver,
		fdr::rWDriver &WDriver )
	{
		fdr::byte__ Buffer[BufferSize];

		Copy_( RDriver, Buffer, BufferSize, WDriver );
	}

	void Purge_(
		rRDriver &RDriver,
		sByte *Buffer,
		sSize BufferSize );

	template <int BufferSize = 1024> inline void Purge( fdr::rRDriver &Driver )
	{
		fdr::byte__ Buffer[BufferSize];

		Purge_( Driver, Buffer, BufferSize );
	}
}

#endif
