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

// FLoW.

#ifndef FLW__INC
# define FLW__INC

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
/**** NEW *****/
/**************/

namespace flw {
	class iflow__;
	typedef iflow__ fIFlow;

	class oflow__;
	typedef oflow__ fOFlow;
}

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
		size__ _Red;
		// Max amount of data alllowed between 2 reset.
		size__ _AmountMax;
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
		void _Dismiss( void )
		{
			_D().Dismiss( this );
			_Red = 0;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _Red )
					Dismiss();
			}

			_Red = 0;
			_Driver = NULL;
			_AmountMax = 0;
		}
		iflow__( void )
		{
			reset( false );
		}
		~iflow__( void )
		{
			reset();
		}
		void Init(
			fdr::iflow_driver_base___ &Driver,
			size__ AmountMax )
		{
			if ( _Red )
				Dismiss();

			_Driver = &Driver;
			_AmountMax = AmountMax;

			_Red = 0;
		}
		fdr::iflow_driver_base___ &Driver( void ) const
		{
			return _D();
		}
		bso::bool__ IsCacheEmpty( bso::size__ &Available = *(bso::size__ *)NULL ) const
		{
			return _D().IsCacheEmpty( &Available );
		}
		// Si valeur retourne == '0', alors toutes les donnes ont t lues.
		size__ ReadUpTo(
			size__ Amount,
			void *Buffer )
		{
			return _D().Read( this, Amount, (byte__ *)Buffer, fdr::bNonBlocking );
		}
		//f Place 'Amount' bytes in 'Buffer'.
		void Read(
			size__ Amount,
			void *Buffer )
		{
			if ( _D().Read( this, Amount, (byte__ *)Buffer, fdr::bBlocking ) != Amount )
				qRFwk();
		}
		bso::bool__ EndOfFlow( void )
		{
			return _D().EndOfFlow( this );
		}
		size__ View(
			size__ Size,
			byte__ *Datum )
		{
      			return _D().Read( this, Size, Datum, fdr::bKeep );
		}
		byte__ View( void )
		{
			byte__ C;

			if ( View( 1, &C ) != 1 )
				qRFwk();

			return C;
		}
		byte__ Get( void )
		{
			byte__ C;

			if ( _D().Read( this, 1, &C, fdr::bBlocking ) != 1 )
				qRFwk();

			return C;
		}
		//f Skip 'Amount' bytes.
		void Skip( size__ Amount = 1 )
		{
			while ( Amount-- )
				Get();
		}
		//f Return the amount of data red since last 'Reset()'.
		size__ AmountRed( void ) const
		{
			return _Red;
		}
		void SetAmountMax( size__ AmountMax )
		{
			_AmountMax = AmountMax;
		}
		void Dismiss( void )
		{
			if ( _Driver != NULL )
				_Dismiss();
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
	};


	// Uniquement pour avoir une symtrie par rapport  'standalone_oflow__'.
	template <int Dummy = 0> class standalone_iflow__
	: public iflow__
	{
	public:
		standalone_iflow__( void )
		{
			if ( Dummy != 0 )	
				qRFwk();	// 'Dummy' n'tant pas utilis, rien ne sert de modifier sa valeur.
		}
		void Init(
			fdr::iflow_driver_base___ &Driver,
			size__ AmountMax = FDR_SIZE_MAX )
		{
			iflow__::Init( Driver, AmountMax );
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
		// Amount of data written since the last synchronizing.
		size__ _Written;
		// Max amount of data between 2 synchronizing.
		size__ _AmountMax;
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
			size__ PonctualAmount = _D().Write( this, Buffer, Wanted );
			size__ CumulativeAmount = PonctualAmount;

			while ( ( PonctualAmount != 0 ) && ( Minimum > CumulativeAmount ) ) {
				PonctualAmount = _D().Write( this, Buffer + CumulativeAmount, Wanted - CumulativeAmount );
				CumulativeAmount += PonctualAmount;
			}

			return CumulativeAmount;
		}

		// Put up to 'Wanted' and a minimum of 'Minimum' bytes from buffer directly into the device.
		size__ _DirectWrite(
			const byte__ *Buffer,
			size__ Wanted,
			size__ Minimum );
		void _DumpCache( void )
		{
			size__ Stayed = _Size - _Free;
			
			if ( Stayed != 0 ) {
				_DirectWrite( _Cache, Stayed, Stayed );

				_Free = _Size;
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
		void _Commit( void )
		{
			_DumpCache();
			_D().Commit( this );

			_Written = 0;
		}
		// Put up to 'Amount' bytes from 'Buffer'. Return number of bytes written.
		size__ _WriteUpTo(
			const byte__ *Buffer,
			size__ Amount )
		{
			size__ AmountWritten = _WriteIntoCache( Buffer, Amount );

			if ( ( AmountWritten == 0 )  && ( Amount != 0 ) ) {
				_DumpCache();
				AmountWritten = _DirectWriteOrIntoCache( Buffer, Amount );
			}

			return AmountWritten;
		}
		// Put 'Amount' data from 'Buffer'.
		void _Write(
			const byte__ *Buffer,
			size__ Amount );
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _Size != _Free )
					Commit();
			}

			_Driver = NULL;
			_Cache = NULL;
			_Size = _Free = 0;
			_AmountMax = 0;
			_Written = 0;
		}
		oflow__( void )
		{
			reset( false );

		}
		virtual ~oflow__( void )
		{
			reset();
		}
		void Init(
			fdr::oflow_driver_base___ &Driver,
			byte__ *Cache,
			size__ Size,
			size__ AmountMax )
		{
			if ( _Size != _Free )
				Commit();

			_Driver = &Driver;
			_Cache = Cache;
			_Size = _Free = Size;
			_AmountMax = AmountMax;

			_Written = 0;
		}
		fdr::oflow_driver_base___ &Driver( void ) const
		{
			return _D();
		}
		//f Put up to 'Amount' bytes from 'Buffer'. Return number of bytes written.
		size__ WriteUpTo(
			const void *Buffer,
			size__ Amount )
		{
			return _WriteUpTo( (byte__ *)Buffer, Amount );
		}
		//f Put 'Amount' data from 'Buffer'.
		void Write(
			const void *Buffer,
			size__ Amount )
		{
			_Write( (const byte__ *)Buffer, Amount );
		}
		//f Put 'C'.
		void Put( byte__ C )
		{
			_Write( &C, 1 );
		}
		//f Synchronization.
		void Commit( void )
		{
			if ( _Driver != NULL )
				_Commit();
		}
		//f Return the amount of data written since last 'Synchronize()'.
		size__ AmountWritten( void ) const
		{
			return _Written + ( _Size - _Free );
		}
		void SetAmountMax( size__ AmountMax )
		{
			_AmountMax = AmountMax;
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
		void Init(
			fdr::oflow_driver_base___ &Driver,
			size__ AmountMax = FDR_SIZE_MAX )
		{
			oflow__::Init( Driver, _Cache, sizeof( _Cache ), AmountMax );
		}
	};


	//f Write to 'OutputFlow' 'StaticObject'.
	template <class t> inline void Put(
		const t &StaticObjet,
		oflow__ &OutputFlow )
	{
		OutputFlow.Write( &StaticObjet, sizeof( t ) );
	}

	//f Write to 'OutputFlow' the 'NULL' terminated String 'String' WITH the 'NULL' character.
	inline void PutString(
		const char *String,
		oflow__ &OutputFlow )
	{
		OutputFlow.Write( String, (size__)( strlen( String ) + 1 ) );
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
		ioflow__( void )
		{
			reset( false );
		}
		void SetAmountMax(
			size__ ReadAmountMax,
			size__ WriteAmountMax )
		{
			iflow__::SetAmountMax( ReadAmountMax );
			oflow__::SetAmountMax( WriteAmountMax );
		}
		void SetAmountMax( size__ AmountMax )
		{
			SetAmountMax( AmountMax, AmountMax );
		}
		void Init(
			fdr::ioflow_driver_base___ &Driver,
			size__ ReadAmountMax,
			byte__ *OCache,
			size__ OSize,
			size__ WriteAmountMax )
		{
			iflow__::Init( Driver, ReadAmountMax );
			oflow__::Init( Driver, OCache, OSize, WriteAmountMax );

		}
		void Init(
			fdr::ioflow_driver_base___ &Driver,
			byte__ *Cache,
			size__ Size,
			size__ AmountMax )
		{
			iflow__::Init( Driver, AmountMax );
			oflow__::Init( Driver, Cache + Size / 2, Size / 2, AmountMax );
		}
	};

	template <int OutCacheSize = FLW__OUTPUT_CACHE_SIZE> class standalone_ioflow__
	: public ioflow__
	{
	private:
		flw::byte__ _OutputCache[OutCacheSize];
	public:
		void Init(
			fdr::ioflow_driver_base___ &Driver,
			size__ ReadAmountMax,
			size__ WriteAmountMax )
		{
			ioflow__::Init( Driver, ReadAmountMax, _OutputCache, sizeof( _OutputCache ), WriteAmountMax );
		}
		void Init(
			fdr::ioflow_driver_base___ &Driver,
			size__ AmountMax = FDR_SIZE_MAX )
		{
			ioflow__::Init( Driver, AmountMax, _OutputCache, sizeof( _OutputCache ), AmountMax );
		}
	};


# if 0
	// Copie 'Amount' octets de 'IFlow' dans 'OFlow'.
	void Copy(
		iflow__ &IFlow,
		size__ Amount,
		oflow__ &OFlow );
# endif

	template <int BufferSize = 1024> inline void Copy(
		iflow__ &IFlow,
		oflow__ &OFlow )
	{
		fdr::byte__ Buffer[BufferSize];

		while ( !IFlow.EndOfFlow() )
			OFlow.Write( Buffer, IFlow.ReadUpTo( BufferSize, Buffer ) );
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


/*$END$*/
#endif
