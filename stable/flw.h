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

// FLoW.

#ifndef FLW_INC_
# define FLW_INC_

# define FLW_NAME		"FLW"

# if defined( E_DEBUG ) && !defined( FLW_NODBG )
#  define FLW_DBG
# endif

# include "bso.h"
# include "cpe.h"
# include "fdr.h"

// Not uses in this library, but in calling libraries.
# ifndef FLW_INPUT_CACHE_SIZE
#  define FLW__INPUT_CACHE_SIZE	FDR__DEFAULT_CACHE_SIZE
# else
#  define FLW__INPUT_CACHE_SIZE	FLW_INPUT_CACHE_SIZE
# endif

# ifndef FLW_OUTPUT_CACHE_SIZE
#  define FLW__OUTPUT_CACHE_SIZE	FDR__DEFAULT_CACHE_SIZE
# else
#  define FLW__OUTPUT_CACHE_SIZE	FLW_OUTPUT_CACHE_SIZE
# endif

# ifdef CPE_S_UNIX
#  ifndef FLW_LET_SIGPIPE
#   define FLW__IGNORE_SIGPIPE
#  endif
# endif

# define FLW_AMOUNT_MAX BSO_SIZE_MAX

/**************/
/**** OLD *****/
/**************/

namespace flw {
	using fdr::byte__;
	using fdr::size__;

	//c Base input flow.
	class iflow__	/* Bien que cette classe ai un destructeur, elle est suffixe par '__', d'une part pour simplifier
					son utilisation (comme dclaration de paramtre d'une fonction) et, d'autre part,
					parce qu'elle ne sera jamais instancie telle quelle, mais toujours hrite (bien que ce ne
					soit pas obligatoire d'un point de vue C++, car ce n'est pas une fonction abstraite).*/
	{
	private:
		fdr::iflow_driver_base___ *_Driver;
		// Amount of data red since the last reset.
		fdr::iflow_driver_base___ &_D( void ) const
		{
			if ( _Driver == NULL )
				qRFwk();

			return *_Driver;
		}
# if 0
		size__ _ReadUpTo(
			size__ Amount,
			byte__ *Buffer,
			size__ Minimum,
			bso::bool__ Adjust,
			bso::bool__ &CacheIsEmpty )
		{
			return _RawRead( Minimum, Buffer, Amount, Adjust, CacheIsEmpty );
		}
		// Place 'Amount' bytes in 'Buffer'.
		void _Read(
			size__ Amount,
			byte__ *Buffer,
			bso::bool__ Adjust,
			bso::bool__ &CacheIsEmpty )
		{
			if ( _ReadUpTo( Amount, Buffer, Amount, Adjust, CacheIsEmpty ) != Amount )
				qRFwk();
		}
		// Generic read.
		size__ _LoopingRawRead(
			size__ Minimum,
			byte__ *Buffer,
			size__ Wanted,
			bso::bool__ Adjust,
			bso::bool__ &CacheIsEmpty )
		{
			size__ PonctualAmount = _D().Read( Wanted, Buffer, Adjust, CacheIsEmpty );
			size__ CumulativeAmount = PonctualAmount;

			while ( ( PonctualAmount != 0 ) && ( Minimum > CumulativeAmount ) ) {
				PonctualAmount = _D().Read( Wanted - CumulativeAmount, Buffer + CumulativeAmount, Adjust, CacheIsEmpty );
				CumulativeAmount += PonctualAmount;
			}

			return CumulativeAmount;
		}
		size__ _RawRead(
			size__ Minimum,
			byte__ *Buffer,
			size__ Wanted,
			bso::bool__ Adjust,
			bso::bool__ &CacheIsEmpty );
# endif
		byte__ Get_( bso::sBool *IsError )
		{
			byte__ C = 0;

			if ( _D().Read( 1, &C, fdr::bBlocking ) != 1 ) {
				if ( IsError != NULL )
					*IsError = true;
				else
					qRFwk();
            }

			return C;
		}
		bso::sBool _Dismiss(
			bso::sBool Unlock,
			qRPN )
		{
			return _D().Dismiss( Unlock, ErrHandling );
		}
		void Init( fdr::iflow_driver_base___ &Driver )	// To force the use of the 'Dressed' version.
		{
			_Driver = &Driver;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _Driver != NULL )
					_Dismiss( true, err::hUserDefined );	// Errors are ignored.
			}

			_Driver = NULL;
		}
		qCVDTOR( iflow__ );
		void SetAutoDismissOnEOF( bso::sBool Value = true )
		{
			if ( _Driver == NULL )
				qRFwk();

			_Driver->SetAutoDismissOnEOF( Value );
		}
		fdr::iflow_driver_base___ &RDriver( void ) const
		{
			return _D();
		}
		tht::sTID Take( tht::sTID Owner = tht::Undefined )
		{
			if ( Owner == tht::Undefined )
				Owner = tht::GetTID();

			return _D().RTake( Owner );
		}
		tht::sTID Owner( void ) const
		{
			return _D().Owner();
		}
		bso::bool__ IsCacheEmpty( bso::size__ *Available = NULL ) const
		{
			return _D().IsCacheEmpty( Available );
		}
		// Si valeur retourne == '0', alors toutes les donnes ont t lues.
		size__ ReadUpTo(
			size__ Amount,
			void *Buffer )
		{
			return _D().Read( Amount, (byte__ *)Buffer, fdr::bNonBlocking );
		}
		//f Place 'Amount' bytes in 'Buffer'.
		void Read(
			size__ Amount,
			void *Buffer )
		{
			if ( _D().Read( Amount, (byte__ *)Buffer, fdr::bBlocking ) != Amount )
				qRFwk();
		}
		bso::bool__ EndOfFlow( void )
		{
			return _D().EndOfFlow();
		}
		size__ View(
			size__ Size,
			byte__ *Datum )
		{
      			return _D().Read( Size, Datum, fdr::bKeepBlocking );
		}
		byte__ View( void )
		{
			byte__ C = 0;

			if ( View( 1, &C ) != 1 )
				qRFwk();

			return C;
		}
		// Returns immediately if 'IsError' != NULL and '*IsError' == true.
		byte__ Get( bso::sBool *IsError = NULL )
		{
			if ( (IsError == NULL) || !*IsError )
				return Get_( IsError );
			else
				return 0;
		}
		//f Skip 'Amount' bytes.
		void Skip(
			size__ Amount,
			bso::sBool *IsError )
		{
			while ( Amount-- )
				Get( IsError );
		}
		//f Skip 'Amount' bytes.
		void Skip( size__ Amount = 1 )
		{
			return Skip( Amount, NULL );
		}
		//f Return the amount of data red since last 'Reset()'.
		size__ AmountRed( void ) const
		{
			return _D().AmountRed();
		}
		bso::sBool Dismiss(
			bso::sBool Unlock = true,
			qRPD )
		{
			if ( _Driver != NULL )
				return _Dismiss( Unlock, ErrHandling );
			else
				return true;
		}
		bso::bool__ IsInitialized( void ) const
		{
			return _Driver != NULL;
		}
		bso::bool__ IsLocked( void )
		{
#ifdef FLW_DBG
			if ( !IsInitialized() )
				qRFwk();
#endif

			return _D().IsLocked();
		}
		bso::bool__ IFlowIsLocked( void )	// Facilite l'utilisation de 'ioflow__'
		{
			return IsLocked();
		}
		friend class _standalone_iflow__;
		friend class ioflow__;
	};

	// To allow the declaration as 'friend', which does not allow templates.
	class _standalone_iflow__
	: public iflow__
	{
	public:
		void reset( bso::sBool P = true )
		{
			iflow__::reset( P );
		}
		qCVDTOR( _standalone_iflow__ );
		void Init( fdr::iflow_driver_base___ &Driver )
		{
			iflow__::Init( Driver );
		}
	};

	// For symetry with  'standalone_oflow__'.
	template <int Dummy = 0> class standalone_iflow__
	: public _standalone_iflow__
	{
	public:
		void reset( bso::sBool P = true )
		{
			if ( Dummy != 0 )
				qRFwk();	// 'Dummy' n'tant pas utilis, rien ne sert de modifier sa valeur.

			_standalone_iflow__::reset( P );
		}
		qCVDTOR( standalone_iflow__ );
		void Init( fdr::iflow_driver_base___ &Driver )
		{
			_standalone_iflow__::Init( Driver );
		}
	};


	//f Get 'StaticObject' from 'InputFlow'.
	template <class t> inline void Get(
		iflow__ &InputFlow,
		t &StaticObject )
	{
		InputFlow.Read( sizeof( t ), &StaticObject );
	}

	/*f Place, from 'IFlow', up to 'Maximum' character in 'Buffer' or until
	reading the 'NULL' character. False is returned, if not enough place to put
	the string with its final NULL character. Buffer contains then 'Maximum'
	characters of the string. Otherwise, it returns true. */
	bool GetString(
		iflow__ &IFlow,
		char *Buffer,
		size__ Maximum );

	//c Basic output flow.
	class oflow__	/* Bien que cette classe ai un destructeur, elle est suffixe par '__', d'une part pour simplifier
					son utilisation (comme dclaration de paramtre d'une fonction) et, d'autre part,
					parce qu'elle ne sera jamais instancie telle quelle, mais toujours hrite (bien que ce ne
					soit pas obligatoire d'un point de vue C++, car ce n'est pas une classe abstraite).*/
	{
	private:
		fdr::oflow_driver_base___ *_Driver;
		// The cache.
		byte__ *_Cache;
		// The size of the cache.
		size__ _Size;
		// The amount of bytes yet free.
		size__ _Free;
		fdr::oflow_driver_base___ &_D( void ) const
		{
			if ( _Driver == NULL )
				qRFwk();

			return *_Driver;
		}
		size__ _LoopingWrite(
			const byte__ *Buffer,
			size__ Wanted,
			size__ Minimum )
		{
			size__ PonctualAmount = _D().Write( Buffer, Wanted );
			size__ CumulativeAmount = PonctualAmount;

			if ( Minimum == 0 )
				qRFwk();

			while ( ( PonctualAmount != 0 ) && ( Minimum > CumulativeAmount ) ) {
				PonctualAmount = _D().Write( Buffer + CumulativeAmount, Wanted - CumulativeAmount );
				CumulativeAmount += PonctualAmount;
			}

			if ( CumulativeAmount < Minimum )
					CumulativeAmount = 0;	// Even if some data were written, there was a problem, which is reported upstream by returning '0'.

			return CumulativeAmount;
		}

		// Put up to 'Wanted' and a minimum of 'Minimum' bytes from buffer directly into the device.
		size__ _DirectWrite(
			const byte__ *Buffer,
			size__ Wanted,
			size__ Minimum );
		// Returns true if the cache was successful dumped ; in this case,
		// 'Empty' is set to true if the cache was already empty, otherwise
		// its value is not changed. Returns false if the content of the cache
		// could not be written.
		bso::sBool DumpCache_(
			bso::sBool *WasEmpty,   // May be obsolete!
			qRPN )
		{
			size__ Stayed = _Size - _Free;

			if ( Stayed != 0 ) {
				if ( _DirectWrite( _Cache, Stayed, Stayed ) == Stayed ) {
					_Free = _Size;
					return true;
				} else if ( ErrHandling == err::hThrowException ) {
					_Free = _Size;	// So the next attempt (probably on destruction) will not throw an error.
					qRFwk();
				}
				return false;
			} else {
				if ( WasEmpty != NULL )
					*WasEmpty = true;
				return true;
			}
		}
		size__ _WriteIntoCache(
			const byte__ *Buffer,
			size__ Amount )
		{
			if ( _Free < Amount )
				Amount = _Free;

			memcpy( _Cache + _Size - _Free, Buffer, (size_t)Amount );

			_Free -= Amount;

			return Amount;
		}
		/* Put up to 'Amount' bytes from 'Buffer' directly or through the cache.
		Return amount of bytes written. Cache MUST be EMPTY. */
		size__ _DirectWriteOrIntoCache(
			const byte__ *Buffer,
			size__ Amount )
		{
#ifdef FLW_DBG
			if ( _Size != _Free )
				qRFwk();
#endif
			if ( Amount > _Size )
				return _DirectWrite( Buffer, Amount, Amount );
			else
				return _WriteIntoCache( Buffer, Amount );
		}
		// Synchronization.
		bso::sBool _Commit(
			bso::sBool Unlock,
			qRPN )
		{
			bso::sBool WasEmpty = false;

			if ( DumpCache_( &WasEmpty, ErrHandling ) ) {
//				if ( !WasEmpty )
					return _D().Commit( Unlock, ErrHandling );
/*				else
					return true;
*/			}

			return false;
		}
		bso::sBool _Unlock( qRPN )
		{
			return _D().Unlock( ErrHandling );
		}
		// Put up to 'Amount' bytes from 'Buffer'. Return number of bytes written.
		size__ _WriteUpTo(
			const byte__ *Buffer,
			size__ Amount )
		{
			size__ AmountWritten = _WriteIntoCache( Buffer, Amount );

			if ( ( AmountWritten == 0 )  && ( Amount != 0 ) ) {
				DumpCache_( NULL, err::hThrowException );
				AmountWritten = _DirectWriteOrIntoCache( Buffer, Amount );
			}

			return AmountWritten;
		}
		// Put 'Amount' data from 'Buffer'. Returns true on success, false otherwise, or throws an excpeiotn, depending on RP.
		bso::sBool _Write(
			const byte__ *Buffer,
			size__ Amount,
			qRPN );
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _Size != _Free )
					Commit(true, err::hUserDefined);	// Errors are ignored.
			}

			_Driver = NULL;
			_Cache = NULL;
			_Size = _Free = 0;
		}
		qCVDTOR( oflow__ );
		void Init(
			fdr::oflow_driver_base___ &Driver,
			byte__ *Cache,
			size__ Size )
		{
			if ( _Size != _Free )
				Commit( true, err::hUserDefined );	// Errors are ignored.

			_Driver = &Driver;
			_Cache = Cache;
			_Size = _Free = Size;
		}
		tht::sTID Take( tht::sTID Owner = tht::Undefined )
		{
			if ( Owner == tht::Undefined )
				Owner = tht::GetTID();

			return _D().WTake( tht::GetTID() );
		}
		tht::sTID Owner( void ) const
		{
			return _D().Owner();
		}
		fdr::oflow_driver_base___ &WDriver( void ) const
		{
			return _D();
		}
		//f Put up to 'Amount' bytes from 'Buffer'. Return number of bytes written.
		size__ WriteUpTo(
			const void *Buffer,
			size__ Amount )
		{
			if ( Amount == 0 )
				return 0;
			else
				return _WriteUpTo( (byte__ *)Buffer, Amount );
		}
		//f Put 'Amount' data from 'Buffer'.
		bso::sBool Write(
			const void *Buffer,
			size__ Amount,
			qRPD )
		{
			if ( Amount == 0 )
				return true;
			else
				return _Write((const byte__ *)Buffer, Amount, qRP);
		}
		//f Put 'C'.
		bso::sBool Put(
			byte__ C,
			qRPD )
		{
			return _Write( &C, 1, qRP );
		}
		//f Synchronization.
		bso::sBool Commit(
			bso::sBool Unlock = true,
			qRPD )
		{
			if ( _Driver != NULL )
				return _Commit( Unlock, ErrHandling );
			else
				return true;
		}
		void DumpCache(void)
		{
		    DumpCache_(NULL, err::h_Default);
		}
		//f Return the amount of data written since last 'Synchronize()'.
		size__ AmountWritten( void ) const
		{
			return _D().AmountWritten() + ( _Size - _Free );
		}
# if 0
		size__ WriteRelay(
			const byte__ *Buffer,
			size__ Maximum )
		{
			return WriteUpTo( Buffer, Maximum );
		}
		byte__ *GetCurrentCacheDatum( bso::bool__ MarkAsUsed )	/* Si 'AsUsed'  vrai, considre le 'datum' retourn comme utilis. */
		{
			if ( _Free == 0 )
				_DumpCache();

			if ( _Free == 0 )
				ERRf();

			if ( MarkAsUsed )
				return _Cache + _Size - _Free--;
			else
				return _Cache + _Size - _Free;
		}
#endif
		size__ GetCacheSize( void) const
		{
			return _Size;
		}
		bso::bool__ IsInitialized( void ) const
		{
			return _Driver != NULL;
		}
		bso::bool__ IsLocked( void )
		{
#ifdef FLW_DBG
			if ( !IsInitialized() )
				qRFwk();
#endif

			return _Driver->IsLocked();
		}
		bso::bool__ OFlowIsLocked( void )	// Facilite l'utilisation de 'ioflow__'
		{
			return IsLocked();
		}
	};

	template <int CacheSize = FLW__OUTPUT_CACHE_SIZE> class standalone_oflow__
	: public oflow__
	{
	private:
		flw::byte__ _Cache[CacheSize];
	public:
		void reset( bso::sBool P = true )
		{
			oflow__::reset( P );
		}
		qCVDTOR( standalone_oflow__ );
		void Init( fdr::oflow_driver_base___ &Driver )
		{
			oflow__::Init( Driver, _Cache, sizeof( _Cache ) );
		}
	};


	//f Write to 'OutputFlow' 'StaticObject'.
	template <class t> inline bso::sBool Put(
		const t &StaticObjet,
		oflow__ &OutputFlow,
		qRPD )
	{
		return OutputFlow.Write( &StaticObjet, sizeof( t ), qRP );
	}

	//f Write to 'OutputFlow' the 'NULL' terminated String 'String' WITH the 'NULL' character.
	inline bso::sBool PutString(
		const char *String,
		oflow__ &OutputFlow,
		qRPD )
	{
		return OutputFlow.Write( String, (size__)( strlen( String ) + 1 ), qRP );
	}

	//c Basic input/output flow.
	class ioflow__
	: public iflow__,
	  public oflow__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			iflow__::reset( P );
			oflow__::reset( P );
		}
		qCVDTOR( ioflow__ );
		void Init(
			fdr::ioflow_driver_base___ &Driver,
			byte__ *Cache,	// The cache is only used for the output flow.
			size__ Size )
		{
			iflow__::Init( Driver );
			oflow__::Init( Driver, Cache, Size );
		}
		tht::sTID Take( tht::sTID Owner = tht::Undefined )
		{
			if ( Owner == tht::Undefined )
				Owner = tht::GetTID();

			return tol::Same( iflow__::Take( Owner ), oflow__::Take( Owner ) );
		}
		tht::sTID Owner( void ) const
		{
			return tol::Same( iflow__::Owner(), oflow__::Owner() );
		}
	};

	template <int OutCacheSize = FLW__OUTPUT_CACHE_SIZE> class standalone_ioflow__
	: public ioflow__
	{
	private:
		flw::byte__ _OutputCache[OutCacheSize];
	public:
		void reset( bso::sBool P = true )
		{
			ioflow__::reset( P );
		}
		qCVDTOR( standalone_ioflow__ );
		void Init( fdr::ioflow_driver_base___ &Driver )
		{
			ioflow__::Init( Driver, _OutputCache, sizeof( _OutputCache ) );
		}
	};


# if 0
	// Copie 'Amount' octets de 'IFlow' dans 'OFlow'.
	void Copy(
		iflow__ &IFlow,
		size__ Amount,
		oflow__ &OFlow );
# endif

	template <int BufferSize = 1024> inline fdr::sSize Copy(
		iflow__ &IFlow,
		oflow__ &OFlow,
		tol::sDelay Delay = 0 )
	{
		fdr::byte__ Buffer[BufferSize];
		fdr::sSize TotalSize = 0, PartialSize = 0;
		tol::sTimer Timer;

		Timer.Init( Delay );

		Timer.Launch();

		while ( !Timer.IsElapsed() && !IFlow.EndOfFlow() ) {
			OFlow.Write( Buffer, PartialSize = IFlow.ReadUpTo( BufferSize, Buffer ) );
			TotalSize += PartialSize;
		}

		return TotalSize;

	}

	template <int BufferSize = 1024> inline void Purge( iflow__ &IFlow )
	{
		fdr::byte__ Buffer[BufferSize];

		while ( !IFlow.EndOfFlow() )
			IFlow.ReadUpTo( BufferSize, Buffer );
	}
}

inline flw::oflow__ &operator <<(
	flw::oflow__ &OFlow,
	const char *String )
{
	OFlow.Write( String, strlen( String ) );

	return OFlow;
}

inline flw::oflow__ &operator <<(
	flw::oflow__ &OFlow,
	char Character )
{
	OFlow.Put( (flw::byte__)Character );

	return OFlow;
}

/**************/
/**** NEW *****/
/**************/

namespace flw {
	typedef flw::byte__ sByte;

	// Facilitates the use of templates (for internal and also external use).
	template <typename flow, typename driver> class rDressedFlow
	: public flow
	{
	protected:
		driver Driver_;
		void subInit( void )
		{
			flow::Init( Driver_ );
		}
	public:
		void reset( bso::sBool P = true )
		{
			flow::reset( P );
			Driver_.reset( P );
		}
		qCDTOR( rDressedFlow );
	};

	typedef flw::iflow__ rRFlow;	// '__' -> 'r...' instead of 's... : see comment of 'iflow__'.
	template <int Dummy = 0> qTCLONEs( standalone_iflow__<Dummy>, rDressedRFlow );
	template <typename driver, int Dummy = 0> qTCLONE( flw::rDressedFlow<qCOVER2( flw::rDressedRFlow<Dummy>, driver )>, rXDressedRFlow );


	typedef flw::oflow__ rWFlow;	// '__' -> 'r...' instead of 's... : see comment of 'oflow__'.
	template <int CacheSize = FLW__OUTPUT_CACHE_SIZE> qTCLONEs( standalone_oflow__<CacheSize>, rDressedWFlow );
	template <typename driver, int CacheSize = FLW__OUTPUT_CACHE_SIZE> qTCLONE( flw::rDressedFlow<qCOVER2( flw::rDressedWFlow<CacheSize>, driver )>, rXDressedWFlow );

	typedef flw::ioflow__ rRWFlow;	// '__' -> 'r...' instead of 's... : see comment of 'ioflow__'.
	template <int OutCacheSize = FLW__OUTPUT_CACHE_SIZE> qTCLONEs( standalone_ioflow__<OutCacheSize>, rDressedRWFlow );
	template <typename driver, int CacheSize = FLW__OUTPUT_CACHE_SIZE> qTCLONE( flw::rDressedFlow<qCOVER2( flw::rDressedRWFlow<CacheSize>, driver )>, rXDressedRWFlow );
}

#endif
