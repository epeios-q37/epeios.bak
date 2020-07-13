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

#ifndef FDR_INC_
# define FDR_INC_

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

namespace fdr {
#ifdef FDR__TS
#	include "mtx.h"
#	define FDR_NO_MUTEX	MTX_INVALID_HANDLER
	typedef mtx::handler___ mutex__;
#else
	typedef void *mutex__;
#	define FDR_NO_MUTEX	NULL
#endif
}

#ifdef FDR_DEFAULT_CACHE_SIZE
#	define FDR__DEFAULT_CACHE_SIZE	FDR_DEFAULT_CACHE_SIZE
#else
#	define FDR__DEFAULT_CACHE_SIZE	1024
#endif

# define FDR_SIZE_MAX	BSO_SIZE_MAX


// predeclaration
namespace flw {
	class iflow__;
	class oflow__;
}

namespace fdr {
	//t Amount of data.
	using bso::size__;

	enum thread_safety__ {
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
		bNonBlocking,	// At least one byte is red, more if this not block.
		bBlocking,		// Unless EOF, the wanted amount is red, even if blocks.
		// Same as above with same name, but the data is kept in the flow and will be available again at next reading.
		bKeepNonBlocking,
		bKeepBlocking,
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
		bso::sBool Lock( bso::sBool Block )
		{
#ifdef FDR__TS
			sTID Caller = tht::GetTID();

			if ( _Mutex != mtx::Undefined ) {
				if ( TryToLock_( _Mutex ) ) {
					if ( Owner_ != tht::Undefined )
						qRFwk();

					Owner_ = Caller;
				} else if ( Owner_ != Caller ) {
					if ( Block )
						Lock_( _Mutex );
					else
						return false;
					Owner_ = Caller;
				}
			}
# else
			Lock_( _Mutex );
# endif
			return true;
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

			if ( _Mutex != mtx::Undefined ) {
				if ( TID == tht::Undefined )
					TID = tht::GetTID();

				if ( TryToLock( _Mutex ) != ( Owner_ == tht::Undefined ) )
					qRFwk();

				Owner_ = TID;
			}

			return Old;
#else
			return tht::Undefined;
#endif
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _Mutex != mtx::Undefined )
					Delete_( _Mutex );
			}

			Owner_ = tht::Undefined;
			_Mutex = NULL;
		}
		E_CVDTOR( _flow_driver_base__ );
		bso::bool__ IsLocked( void )
		{
			return IsLocked_( _Mutex );
		}
		bso::sBool Unlock( qRPN )
		{
#ifdef FDR__TS
			sTID Caller = tht::GetTID();

			if ( _Mutex != mtx::Undefined ) {
				if ( IsLocked_( _Mutex ) ) {
					if ( Owner_ != Caller )
						qRFwk();

					Owner_ = tht::Undefined;

					Unlock_( _Mutex );

					return true;
				} else if ( Owner_ != tht::Undefined )
					qRFwk();
				else if ( ErrHandling == err::hThrowException )
					qRFwk();

				return false;
			}
# else
			Unlock_( _Mutex );
# endif
			return true;
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
					Dismiss( true, err::h_Default );	// Relaying dismissing to underlying level on EOF.

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
		size__ _FillCache( size__ Size )	// If != 0, everything is done to retrieve the wanted quantity. Otherwise, at least one byte is retrieved, unless EOF.
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
		void _CompleteCache( size__ Size )// If the cache does not already contain 'Size' bytes, missing data will be red.
		{
			if ( _Size == 0 )	// No more data available.
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
		size__ ReadFromCache_(
			size__ Size,
			byte__ *Buffer,	// Can be == NULL, if we only want to consume data.
			bso::bool__ Adjust,
			size__ *TotalRed )
		{
			if ( Size > _Available )
				Size = _Available;

			if ( _Available != 0 )  {
				if ( Buffer != NULL  )
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
		size__ ReadThroughCache_(
			size__ Size,
			byte__ *Buffer,	// Can be == NULL if we want only to consume data.
			bso::bool__ Force,
			size__ *TotalRed )	// Si == 'true', on fait le maximum pour lire la quantite demande.
		{
			size__ Red = ReadFromCache_( Size, Buffer, true, TotalRed );

			if ( Red < Size )  {
				if ( Force )
					_FillCache( Size - Red );
				else if ( Red == 0 )
					_FillCache( 0 );
				else
					return Red;

				Red += ReadFromCache_( Size - Red, Buffer == NULL ? Buffer : Buffer + Red, true, TotalRed );
			}

			return Red;
		}

		bso::bool__ EOF_( void )
		{
			Lock(true);

			if ( _Available ) {
				DismissPending_ = true;
				return false;
			} else if ( ( _Size != 0 ) && (_FillCache( 0 ) != 0) ) {
				DismissPending_ = true;
				return false;
			} else {
				if ( !DismissPending_ )
					Unlock( err::h_Default);
				return true;
			}
		}
	protected:
		// Returns the amount of data red. If 0, then no more data are available (EOF).
		virtual size__ FDRRead(
			size__ Maximum,
			byte__ *Buffer ) = 0;
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) = 0;	// If 'Unlock' at true, returns true if unlocking succeed (i.e. was locked, and by the same thread), if 'ErrHandling' allows it.
		virtual sTID FDRRTake( sTID Owner ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				_flow_driver_base__::BaseTake( tht::Undefined );	// Prevent some unwanted error due to bad due to mutex owning.
				Dismiss( true, err::hUserDefined);	// Ignore errors.
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
		sTID RTake( sTID Owner = tht::Undefined )
		{
			return GetTID_( FDRRTake( Owner ), BaseTake( Owner ) );
		}
		bso::sBool Dismiss(
			bso::sBool Unlock,	// When 'Unlock' is set to false, the 'Red_' value is NOT set to 0.
			qRPN )
		{
			bso::sBool Success = true;

			if ( Lock( false ) ) {
			qRH
			qRB
				if ( DismissPending_ ) {
					if ( _Cache != NULL ) {
						Success = FDRDismiss( Unlock, ErrHandling );

						DismissPending_ = false;
					}
				}
			qRR
			qRT
				if ( Unlock ) {
					Red_ = 0;
					if ( Success )
						Success = this->Unlock( ErrHandling );
					else
						this->Unlock( err::hUserDefined );	// Ignore errors.
				}
			qRE
			}

			return Success;
		}
		size__ Read(
			size__ Wanted,
			byte__ *Buffer,	// Can be null if we only want to consume data.
			behavior__ Behavior )
		{
#ifdef FDR_DBG
			if ( Wanted < 1 )
				qRFwk();
#endif
			if ( EOF_() )
				return 0;

			switch ( Behavior ) {
			case bNonBlocking:
				return ReadThroughCache_( Wanted, Buffer, false, &Red_ );
				break;
			case bBlocking:
				if ( ( _Available >= Wanted ) || ( _Size > ( Wanted - _Available ) ) )
					return ReadThroughCache_( Wanted, Buffer, true, &Red_ );
				else {
					size__ Red = ReadFromCache_( Wanted, Buffer, true, &Red_ );

					if ( Red < Wanted )
						Red += _LoopingRead( Wanted - Red, Buffer + Red, &Red_ );

					return Red;
				}

				break;
			case bKeepBlocking:
				_CompleteCache( Wanted );
				// Below comment is taken into account by some compiler, and avoid a 'fall through' warning.
				// fall through
			case bKeepNonBlocking:
				if ( Buffer == NULL )	// Does not make sense in this case.
					qRFwk();

				return ReadFromCache_( Wanted, Buffer, false, NULL );
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
			return EOF_();
		}
		bso::bool__ IFlowIsLocked( void )	// Simplifie l'utilisation de 'ioflow_driver_...'
		{
			return IsLocked();
		}
		size__ AmountRed( void ) const
		{
			return Red_;
		}
		void EmptyCache( void )
		{
			if ( !IsLocked() )
				qRFwk();

			_Available = _Position = 0;
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
		bso::sBool CommitPending_;
		size__ Written_;	// Amount of data written since last commit.
	protected:
		// Returns amount of written data. Returns '0' only when no other data can be written (deconnection...), otherwise must block.
		virtual size__ FDRWrite(
			const byte__ *Buffer,
			size__ Maximum ) = 0;
		// Returns 'false' when underlying write fails, 'true' otherwise.
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) = 0;	// If 'Unlock' at true, returns true if unlocking succeed (i.e. was locked, and by the same thread), if 'ErrHandling' allows it.
		virtual sTID FDRWTake( sTID Owner ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				_flow_driver_base__::BaseTake( tht::Undefined );	// Prevent some unwanted error due to bad due to mutex owning.
				Commit( true, err::hUserDefined);	// Errors are ignored.
			}

			CommitPending_ = false;
			_flow_driver_base__::reset( P );
			Written_ = 0;
		}
		E_CVDTOR( oflow_driver_base___ );
		void Init( thread_safety__ ThreadSafety )
		{
			reset();

			CommitPending_ = false;
			_flow_driver_base__::Init( ThreadSafety );
		}
		bso::sBool Commit(
			bso::sBool Unlock,	// When 'Unlock' is set to false, the 'Written_' value is NOT set to 0.
			qRPN )
		{
			bso::sBool Success = true;

			if ( Lock( false ) ) {
			qRH
			qRB
				if ( CommitPending_ ) {
					Success = FDRCommit( Unlock, ErrHandling );
					CommitPending_ = false;
				}
			qRR
			qRT
				if ( Unlock ) {
					Written_ = false;

					if ( Success )
						Success = this->Unlock( ErrHandling );
					else
						this->Unlock( err::hUserDefined );	// Errors are ignored.
				}
				qRE
			}

			return Success;
		}
		size__ Write(
			const byte__ *Buffer,
			size__ Maximum )
		{
		qRH
		qRB
			Lock(true);
			CommitPending_ = true;
			Maximum = FDRWrite( Buffer, Maximum );

			Written_ += Maximum;
		qRR
			CommitPending_ = false;	// To prevent a new faulty write attempt on commit.
		qRT
		qRE
			return Maximum;
		}
		sTID WTake( sTID Owner = tht::Undefined )
		{
			return GetTID_( FDRWTake( Owner ), BaseTake( Owner ) );
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
		byte__ _InputCache[input_cache_size+1];	// '+1' to handle 'Unget()'.
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

	// Template to copy/paste for a read driver.
	class rRDriver_
	: public fdr::rRDressedDriver
	{
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override;
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override;
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
	public:
		void reset( bso::sBool P = true )
		{
			fdr::rRDressedDriver::reset( P );
		}
		qCVDTOR( rRDriver_ );
		void Init( fdr::eThreadSafety ThreadSafety = fdr::ts_Default )
		{
			fdr::rRDressedDriver::Init( ThreadSafety );
		}
	};

	extern rRDriver_ RDriver_;

	// Template to copy/paste for a write driver.
	class rWDriver_
	: public fdr::rWDressedDriver
	{
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum ) override;
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override;
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
	public:
		void reset( bso::sBool P = true )
		{
			fdr::rWDressedDriver::reset( P );
		}
		qCVDTOR( rWDriver_ );
		void Init( fdr::eThreadSafety ThreadSafety = fdr::ts_Default )
		{
			fdr::rWDressedDriver::Init( ThreadSafety );
		}
	};

	extern rWDriver_  WDriver_;

	// Template to copy/paste for a read/write driver.
	class rRWDriver_
	: public fdr::rRWDressedDriver
	{
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override;
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override;
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum ) override;
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override;
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return fdr::UndefinedTID;
		}
	public:
		void reset( bso::sBool P = true )
		{
			fdr::rRWDressedDriver::reset( P );
		}
		qCVDTOR( rRWDriver_ );
		void Init( fdr::eThreadSafety ThreadSafety = fdr::ts_Default )
		{
			fdr::rRWDressedDriver::Init( ThreadSafety );
		}
	};

	extern rRWDriver_ RWDriver;
}

#endif
