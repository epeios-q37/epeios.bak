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

#ifndef BSO__INC
# define BSO__INC

# define BSO_NAME		"BSO"

# if defined( E_DEBUG ) && !defined( BSO_NODBG )
#  define BSO_DBG
# endif

// BaSic Objects
# include <limits.h>
# include <stdint.h>
# include <float.h>

# ifndef __STDC_FORMAT_MACROS
#  define __STDC_FORMAT_MACROS	// Needed by below header with the 'mingw' compiler.
# endif
# include <inttypes.h>


# include "err.h"
# include "cpe.h"

# ifdef CPE_F_32BITS
#  define BSO__32
# elif defined( CPE_F_64BITS )
#  define BSO__64
# else
#  error
# endif

# if !defined( BSO__32 ) && !defined( BSO__64 )
#  error "Undefined size !"
# endif

# define BSO_CONVERSION_SIZE_MAX	100

/*************************/
/****** New version ******/
/*************************/

namespace bso {
	// Unit of digital information, without any intrinsec signification.
	typedef unsigned char sByte;

	//t Size of a memory .
	typedef size_t sSize;

	// Boolean.
	typedef bool sBool;

	typedef unsigned char sU8;

	typedef sU8 tEnum;

	struct bGlobal_ {
		char Datum[BSO_CONVERSION_SIZE_MAX+1];	// + 1 for the NUL chracter.
	};

	typedef bGlobal_ bInteger;
	typedef bInteger bInt;

	typedef bGlobal_ bFloat;
}

/*************************/
/****** Old version ******/
/*************************/

namespace bso {
	typedef sByte byte__;

	typedef sBool bool__;

# define BSO__DEF( type, name, value )\
	const type##__ name = value

# define BSO__MIN_MAX_DEF( type, name, affix )\
	BSO__DEF( type, name##Min, affix##_MIN );\
	BSO__DEF( type, name##Max, affix##_MAX );

# define BSO__INT_MIN_MAX_BDEF( type, name, s_affix, u_affix)\
	BSO__MIN_MAX_DEF( s##type, S##name, s_affix );\
	BSO__DEF( u##type, U##name##Max, u_affix##_MAX );\
	BSO__DEF( u##type, U##name##Min, 0 )

# define BSO__INT_MIN_MAX_DEF( type, affix )\
	BSO__INT_MIN_MAX_BDEF( type, type, affix, U##affix)


# define BSO_S8_MAX		INT8_MAX
# define BSO_S8_MIN		INT8_MIN
	typedef int8_t	s8__;

# define BSO_U8_MAX		UINT8_MAX
# define BSO_U8_MIN		0
	typedef sU8 u8__;

	BSO__INT_MIN_MAX_DEF( 8, INT8 );


# define BSO_S16_MAX		INT16_MAX
# define BSO_S16_MIN		INT16_MIN
	typedef int16_t s16__;

# define BSO_U16_MAX		UINT16_MAX
# define BSO_U16_MIN		0
	typedef uint16_t u16__;

	BSO__INT_MIN_MAX_DEF( 16, INT16 );


	typedef int32_t s32__;
# define BSO_S32_MAX		INT32_MAX
# define BSO_S32_MIN		INT32_MIN
# define BSO_S32_P_			PRIi32

	typedef uint32_t u32__;
# define BSO_U32_MAX		UINT32_MAX
# define BSO_U32_MIN   0
# define BSO_U32_P_			PRIu32

	BSO__INT_MIN_MAX_DEF( 32, INT32 );


	typedef int64_t s64__;
# define BSO_S64_MAX		INT64_MAX
# define BSO_S64_MIN		INT64_MIN
# define BSO_S64_P_			PRIi64

	typedef uint64_t u64__;
# define BSO_U64_MAX		UINT64_MAX
# define BSO_U64_MIN		0
# define BSO_U64_P_			PRIu64

	BSO__INT_MIN_MAX_DEF( 64, INT64 );


# define BSO_SIZE_MAX	((size_t)-1)
// # define BSO_SIZE_MAX	SIZE_MAX

	//t Size of a memory .
	typedef sSize size__;

	//t Short-sized float.
	typedef float sfloat__;
# define BSO_SFLOAT_MIN	FLT_MIN
# define BSO_SFLOAT_MAX	FLT_MAX
	BSO__MIN_MAX_DEF( sfloat, SFLoat, FLT );

	//t Middle-sized float.
	typedef double float__;
# define BSO_FLOAT_MIN	DBL_MIN
# define BSO_FLOAT_MAX	DBL_MAX
	BSO__MIN_MAX_DEF( float, Float, DBL );

	typedef long double lfloat__;
# define BSO_LFLOAT_MIN	LDBL_MIN
# define BSO_LFLOAT_MAX	LDBL_MAX
	BSO__MIN_MAX_DEF( lfloat, LFloat, LDBL );

	//t Character
	typedef char char__;

	typedef void *pointer__;

	typedef bGlobal_ buffer__;

	/*
	typedef buffer__ pointer_buffer__;

	inline const char *Convert(
		pointer__ Value,
		pointer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%p", Value );

		return Buffer.Datum;
	}
	*/

	// Taille maximale n�cessaire pour stocker la valeur d'un entirer en d�cimal dans une chaine de caract�re (+ le signe).
# define BSO_ASCII_CONVERTED_INTEGER_MAX_SIZE	( 1 + ( sizeof( bso::pointer__ ) * 8 ) )

	typedef buffer__ integer_buffer__;

// 'natural unsigned integer'
# ifdef BSO__64
	typedef u64__ uint__;
	const uint__ UIntMin = U64Min;
	const uint__ UIntMax = U64Max;
#  define BSO_UINT_MAX		BSO_U64_MAX
#  define BSO_UINT_MIN		BSO_U64_MIN
//#  define BSO_UINT_SIZE		BSO_U64_SIZE
# elif defined( BSO__32 )
	typedef u32__ uint__;
	const uint__ UIntMin = U32Min;
	const uint__ UIntMax = U32Max;
#  define BSO_UINT_MAX		BSO_U32_MAX
#  define BSO_UINT_MIN		BSO_U32_MIN
//#  define BSO_UINT_SIZE		BSO_U32_SIZE
# else
#  error
#endif

	inline const char *Convert(
		u64__ Value,
		integer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%" BSO_U64_P_, Value );

		return Buffer.Datum;
	}
#  ifndef CPE_F_MT
	inline const char *Convert( u64__ Value )
	{
		static integer_buffer__ Buffer;

		return Convert( Value, Buffer );
	}
#  endif

	inline const char *Convert(
		bso::u32__ Value,
		integer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%" BSO_U32_P_, Value );

		return Buffer.Datum;
	}

#ifndef CPE_F_MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::u32__ Value )
	{
		static integer_buffer__ Buffer;

		return Convert( Value, Buffer );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		bso::u16__ Value,
		integer_buffer__ &Buffer )
	{
		return Convert( (uint__)Value, Buffer );
	}

#ifndef CPE_F_MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::u16__ Value )
	{
		return Convert( (uint__)Value );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		bso::u8__ Value,
		integer_buffer__ &Buffer )
	{
		return Convert( (uint__)Value, Buffer );
	}

#ifndef CPE_F_MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::u8__ Value )
	{
		return Convert( (uint__)Value );
	}
#endif


# if ULONG_MAX == UINT_MAX
	inline const char *Convert(
		unsigned long Value,
		bso::integer_buffer__ &Buffer )
	{
		return Convert( (unsigned int)Value, Buffer );
	}
# elif ULONG_MAX == ULLONG_MAX
#  if !defined( CPE_S_POSIX ) || defined( CPE_S_DARWIN )
	inline const char *Convert(
		unsigned long Value,
		bso::integer_buffer__ &Buffer )
	{
		return Convert( (unsigned long long)Value, Buffer );
	}
# endif
# else
#  error
# endif



// Non utilisation de 'E_CDEF', car l'incusion de 'tol.h' pose probl�me.
// 'natural signed integer'
# ifdef BSO__64
	typedef s64__ sint__;
	const sint__ SIntMin = S64Min;
	const sint__ SIntMax = S64Max;
#  define BSO_SINT_MAX		BSO_S64_MAX
#  define BSO_SINT_MIN		BSO_S64_MIN
# elif defined( BSO__32 )
	typedef s32__ sint__;
	const sint__ SIntMin = S32Min;
	const sint__ SIntMax = S32Max;
#  define BSO_SINT_MAX		BSO_S32_MAX
#  define BSO_SINT_MIN		BSO_S32_MIN
# else
#  error
#endif

	// Entier g�n�rique, comme facilit�.
	typedef uint__ int__;
	const int__ IntMin = UIntMin;
	const int__ IntMax = UIntMax;
# define BSO_INT_MAX		BSO_UINT_MAX
# define BSO_INT_MIN		BSO_UINT_MIN
//# define BSO_INT_SIZE		BSO_UINT_SIZE

	typedef tEnum enum__;

	//c A sign ( -(1) +(1) 0 ).
	class sign__
	{
	private:
		s8__ Sign_;
	public:
		sign__( sint__ Sign = 0 )
		{
			Sign_ = (s8__)( Sign ? ( Sign > 0 ? 1 : -1 ) : 0 );
		}
		operator s8__( void )
		{
			return Sign_;
		}
	};

	// Retourne le signe ce 'V1' - 'V2'.
	template <typename t> inline bso::sign__ Compare(
		const t &V1,
		const t &V2 )
	{
		if ( V1 < V2 )
			return -1;
		else if ( V1 == V2 )
			return 0;
		else
			return 1;
	}

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		s64__ Value,
		integer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%" BSO_S64_P_, Value );

		return Buffer.Datum;
	}

# ifndef CPE_F_MT
	inline const char *Convert( s64__ Value )
	{
		static integer_buffer__ Buffer;

		return Convert( Value, Buffer );
	}
# endif


	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		s32__ Value,
		integer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%" BSO_S32_P_, Value );

		return Buffer.Datum;
	}

#ifndef CPE_F_MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::s32__ Value )
	{
		static integer_buffer__ Buffer;

		return Convert( Value, Buffer );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		bso::s16__ Value,
		integer_buffer__ &Buffer )
	{
		return Convert( (sint__)Value, Buffer );
	}

#ifndef CPE_F_MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::s16__ Value )
	{
		return Convert( (sint__)Value );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		bso::s8__ Value,
		integer_buffer__ &Buffer )
	{
		return Convert( (sint__)Value, Buffer );
	}

#ifndef CPE_F_MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( bso::s8__ Value )
	{
		return Convert( (sint__)Value );
	}
#endif

	typedef buffer__ float_buffer__;

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		lfloat__ Value,
		float_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%Lg", Value );

		return Buffer.Datum;
	}

#ifndef CPE_F_MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( lfloat__ Value )
	{
		static float_buffer__ Buffer;

		return Convert( Value, Buffer );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		float__ Value,
		float_buffer__ &Buffer )
	{
		return Convert( (lfloat__)Value, Buffer );
	}

#ifndef CPE_F_MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( float__ Value )
	{
		return Convert( (lfloat__)Value );
	}
#endif

	//f Return 'Value' as string in 'String'.
	inline const char *Convert(
		sfloat__ Value,
		float_buffer__ &Buffer )
	{
		return Convert( (lfloat__)Value, Buffer );
	}

#ifndef CPE_F_MT
	//f Return 'Value' as string. Valid only until next call of a 'Convert(..)' function.
	inline const char *Convert( sfloat__ Value )
	{
		return Convert( (lfloat__)Value );
	}
#endif

# if 0
# ifdef CPE_S_DARWIN
	inline const char *Convert(
		size__ Value,
		integer_buffer__ &Buffer )
	{
		return Convert( (uint__)Value, Buffer );
	}

#  ifndef CPE_F_MT
	inline const char *Convert( size__ Value )
	{
		return Convert( (uint__)Value );
	}
#  endif
# endif
#endif

# if LONG_MAX == INT_MAX
	inline const char *Convert(
		long Value,
		bso::integer_buffer__ &Buffer )
	{
		return Convert( (int)Value, Buffer );
	}
# elif LONG_MAX == LLONG_MAX
#  if !defined( CPE_S_POSIX ) || defined( CPE_S_DARWIN )

	inline const char *Convert(
		long Value,
		bso::integer_buffer__ &Buffer )
	{
		return Convert( (long long)Value, Buffer );
	}
#  endif
# else
#  error
# endif

	typedef u64__ sBig;
	typedef u64__ sUBig;
	typedef s64__ sSBig;

# define BSO_BIG_MAX BSO_U64_MAX

# define BSO_DINT_SIZE_MAX ( ( ( 8 * sizeof( bso::sBig ) ) / 7 ) + 1 )

	typedef byte__ dint__[BSO_DINT_SIZE_MAX];

	typedef bso::u8__ length__;
# define SDRM__LENGTH_MAX BSO_UBYTE_MAX

	const struct xint__ &_ConvertToDInt(
		sBig Big,
		struct xint__ &XInt );	//Avec '_', pour �viter des probl�mes d'ambigu�t� ('int__' <=> 'uint__').

	struct xint__ {
	private:
		dint__ _Int;
		length__ _Length;
	public:
		const byte__ *DSizeBuffer( void ) const
		{
# ifdef BSO_DBG
			if ( _Length == 0 )
				qRFwk();
# endif

			return _Int + BSO_DINT_SIZE_MAX - _Length;
		}
		length__ BufferSize( void ) const
		{
			return _Length;
		}
		void reset( bso::bool__ = true )
		{
			// Pour �viter l'inclusion de "strng.h", qui pose probl�me.
			// memset( _Int, 0, sizeof( _Int ) );
			_Length = 0;
		}
		xint__( void )
		{
			reset( false );
		}
		~xint__( void )
		{
			reset();
		}
		friend const struct xint__ &_ConvertToDInt(
			sBig Big,
			struct xint__ &XInt );
	};

	inline const xint__ &ConvertToDInt(
		sUBig UBig,
		xint__ &XInt )
	{
		return _ConvertToDInt( UBig, XInt );
	}

	inline const xint__ &ConvertToDInt(
		sSBig SBig,
		xint__ &XInt )
	{
		return _ConvertToDInt( ( SBig < 0 ? 1 : 0 ) | ( SBig << ( sizeof( SBig ) * 8 - 1 ) ), XInt );
	}

# ifndef BSO__64
	inline const xint__ &ConvertToDInt(
		int__ Int,
		xint__ &XInt )
	{
		return ConvertToDInt( (sUBig)Int, XInt );
	}
# endif

	sBig ConvertToBig(
		const byte__ *DInt,
		size__ *Length = NULL );

	inline int__ ConvertToInt(
		const byte__ *DInt,
		size__ *Length = NULL )
	{
		sBig Big = ConvertToBig( DInt, Length );

		if ( Big > BSO_INT_MAX )
			qRFwk();

		return (int__)Big;
	}

	inline sBig ConvertToBig(
		const byte__ *DInt,
		size__ &Length )
	{
		return ConvertToBig( DInt, &Length );
	}

	inline int__ ConvertToInt(
		const byte__ *DInt,
		size__ &Length )
	{
		sBig Big = ConvertToBig( DInt, Length );

		if ( Big > BSO_INT_MAX )
			qRFwk();

		return (int__)Big;
	}

	inline sUBig ConvertToUBig(
		const byte__ *DInt,
		size__ *Length = NULL )
	{
		return ConvertToBig( DInt, Length );
	}

	inline sUBig ConvertToUBig(
		const byte__ *DInt,
		size__ &Length  )
	{
		return ConvertToUBig( DInt, &Length );
	}

	inline sSBig ConvertToSBig(
		const byte__ *DInt,
		size__ *Length = NULL )
	{
		sBig Big = ConvertToBig( DInt, Length );
		sign__ Sign = ( Big & 1 ? -1 : 1 );

		Big >>= sizeof( Big ) * 8 - 1;

		switch ( Sign ) {
		case 1:
			return Big;
			break;
		case 0:
			qRFwk();
			break;
		case -1:
			return -(sSBig)Big;
			break;
		default:
			qRFwk();
			break;
		}

		return 0;	// Pour �viter un 'warning'.
	}

	inline sSBig ConvertToSBig(
		const byte__ *DInt,
		size__ &Length )
	{
		return ConvertToSBig( DInt, &Length );
	}

}

/*************/
/**** NEW ****/
/*************/

namespace bso {
	typedef uint__ sUInt;
	typedef sint__ sSInt;

	typedef u8__ sU8;
	typedef s8__ sS8;

	typedef u16__ sU16;
	typedef s16__ sS16;

	typedef u32__ sU32;
	typedef s32__ sS32;

	typedef u64__ sU64;
	typedef s64__ sS64;

	typedef sUInt sFlag;

	typedef char__ sChar;

	const bso::sSize SizeMax = BSO_SIZE_MAX;

	typedef sfloat__ sSFloat;
	typedef float__ sFloat;
	typedef lfloat__ sLFloat;
}
#endif
