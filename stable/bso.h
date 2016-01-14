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

/*************************/
/****** New version ******/
/*************************/

// 'object__' -> 'qS( object )' (Static).
# define qS( name ) S_##name

namespace bso {
	// Unit of digital information, without any intrinsec signification.
	typedef unsigned char qS( byte );

	// Boolean.
	typedef bool qS( bool );

	typedef unsigned char qS( u8 );

	// Type de base d'un enum (Utilisé par E_ENUM(...).
	typedef bso::qS( u8 ) qS( enum );
}

/*************************/
/****** Old version ******/
/*************************/

namespace bso {
	typedef qS( byte ) byte__;

	typedef qS( bool ) bool__;

# define BSO__DEF( type, name, value )\
	static const type##__ &name = value

# define BSO__MIN_MAX_DEF( type, name, affix )\
	BSO__DEF( type, name##Min, affix##_MIN );\
	BSO__DEF( type, name##Max, affix##_MAX );

# define BSO__INT_MIN_MAX_BDEF( type, name, s_affix, u_affix)\
	BSO__MIN_MAX_DEF( s##type, S##name, s_affix );\
	BSO__DEF( u##type, U##name##Max, u_affix##_MAX );\
	BSO__DEF( u##type, U##name##Min, 0 )

# define BSO__INT_MIN_MAX_DEF( type, affix )\
	BSO__INT_MIN_MAX_BDEF( type, type, affix, U##affix)


	#define BSO_S8_MAX		INT8_MAX
	#define BSO_S8_MIN		INT8_MIN
	typedef int8_t	s8__;

	#define BSO_U8_MAX		UINT8_MAX
	#define BSO_U8_MIN		0
	typedef qS( u8 ) u8__;

	BSO__INT_MIN_MAX_DEF( 8, INT8 );


	#define BSO_S16_MAX		INT16_MAX
	#define BSO_S16_MIN		INT16_MIN
	typedef int16_t s16__;

	#define BSO_U16_MAX		UINT16_MAX
	#define BSO_U16_MIN		0
	typedef uint16_t u16__;

	BSO__INT_MIN_MAX_DEF( 16, INT16 );


	#define BSO_S32_MAX		INT32_MAX
	#define BSO_S32_MIN		INT32_MIN
	typedef int32_t s32__;

	#define BSO_U32_MAX		UINT32_MAX
	#define BSO_U32_MIN   0
	typedef uint32_t u32__;

	BSO__INT_MIN_MAX_DEF( 32, INT32 );


	#define BSO_S64_MAX		INT64_MAX
	#define BSO_S64_MIN		INT64_MIN
	typedef int64_t s64__;

	#define BSO_U64_MAX		UINT64_MAX
	#define BSO_U64_MIN		0
	typedef uint64_t u64__;

	BSO__INT_MIN_MAX_DEF( 64, INT64 );


# define BSO_SIZE_MAX	((size_t)-1)
// # define BSO_SIZE_MAX	SIZE_MAX

	//t Size of a memory .
	typedef size_t size__;

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

# define BSO_CONVERSION_SIZE_MAX	100

	struct buffer__ {
		char Datum[BSO_CONVERSION_SIZE_MAX+1];	// + 1 for the NUL chracter.
	};

	typedef buffer__ pointer_buffer__;

	inline const char *Convert(
		pointer__ Value,
		pointer_buffer__ &Buffer )
	{
		sprintf( Buffer.Datum, "%p", Value );

		return Buffer.Datum;
	}


	// Taille maximale nécessaire pour stocker la valeur d'un entirer en décimal dans une chaine de caractère (+ le signe).
	#define BSO_ASCII_CONVERTED_INTEGER_MAX_SIZE	( 1 + ( sizeof( bso::pointer__ ) * 8 ) )

	typedef buffer__ integer_buffer__;

// 'natural unsigned integer'
# ifdef BSO__64
	typedef u64__ uint__;
	static const uint__ &UIntMin = U64Min;
	static const uint__ &UIntMax = U64Max;
#  define BSO_UINT_MAX		BSO_U64_MAX
#  define BSO_UINT_MIN		BSO_U64_MIN
//#  define BSO_UINT_SIZE		BSO_U64_SIZE
# elif defined( BSO__32 )
	typedef u32__ uint__;
	static const uint__ &UIntMin = U32Min;
	static const uint__ &UIntMax = U32Max;
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
		sprintf( Buffer.Datum, "%llu", Value );

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
		sprintf( Buffer.Datum, "%u", Value );

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



// Non utilisation de 'E_CDEF', car l'incusion de 'tol.h' pose problème.
// 'natural signed integer'
# ifdef BSO__64
	typedef s64__ sint__;
	static const sint__ SIntMin = S64Min;
	static const sint__ SIntMax = S64Max;
#  define BSO_SINT_MAX		BSO_S64_MAX
#  define BSO_SINT_MIN		BSO_S64_MIN
# elif defined( BSO__32 )
	typedef s32__ sint__;
	static const sint__ SIntMin = S32Min;
	static const sint__ SIntMax = S32Max;
#  define BSO_SINT_MAX		BSO_S32_MAX
#  define BSO_SINT_MIN		BSO_S32_MIN
# else
#  error
#endif

	// Entier générique, comme facilité.
	typedef uint__ int__;
	static const int__ IntMin = UIntMin;
	static const int__ IntMax = UIntMax;
# define BSO_INT_MAX		BSO_UINT_MAX
# define BSO_INT_MIN		BSO_UINT_MIN
//# define BSO_INT_SIZE		BSO_UINT_SIZE

	typedef qS( enum ) enum__;

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
		sprintf( Buffer.Datum, "%lli", Value );

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
		sprintf( Buffer.Datum, "%i", Value );

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



# define BSO_DINT_SIZE_MAX ( ( ( 8 * sizeof( bso::int__ ) ) / 7 ) + 1 )

	typedef byte__ dint__[BSO_DINT_SIZE_MAX];

	typedef bso::u8__ length__;
# define SDRM__LENGTH_MAX BSO_UBYTE_MAX

	const struct xint__ &_ConvertToDInt(
		int__ Int,
		struct xint__ &XInt );	//Avec '_', pour éviter des problèmes d'ambiguïté ('int__' <=> 'uint__').

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
			// Pour éviter l'inclusion de "strng.h", qui pose problème.
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
			int__ Int,
			struct xint__ &XInt );
	};

	inline const xint__ &ConvertToDInt(
		uint__ UInt,
		xint__ &XInt )
	{
		return _ConvertToDInt( UInt, XInt );
	}

	inline const xint__ &ConvertToDInt(
		sint__ SInt,
		xint__ &XInt )
	{
		return _ConvertToDInt( ( SInt < 0 ? 1 : 0 ) | ( SInt << ( sizeof( SInt ) * 8 - 1 ) ), XInt );
	}

	int__ ConvertToInt(
		const byte__ *DInt,
		size__ *Length = NULL );

	inline int__ ConvertToInt(
		const byte__ *DInt,
		size__ &Length )
	{
		return ConvertToInt( DInt, &Length );
	}

	inline uint__ ConvertToUInt(
		const byte__ *DInt,
		size__ *Length = NULL )
	{
		return ConvertToInt( DInt, Length );
	}

	inline uint__ ConvertToUInt(
		const byte__ *DInt,
		size__ &Length  )
	{
		return ConvertToUInt( DInt, &Length );
	}

	inline sint__ ConvertToSInt(
		const byte__ *DInt,
		size__ *Length = NULL )
	{
		int__ Int = ConvertToInt( DInt, Length );
		sign__ Sign = ( Int & 1 ? -1 : 1 );

		Int >>= sizeof( Int ) * 8 - 1;

		switch ( Sign ) {
		case 1:
			return Int;
			break;
		case 0:
			qRFwk();
			break;
		case -1:
			return -(sint__)Int;
			break;
		default:
			qRFwk();
			break;
		}

		return 0;	// Pour éviter un 'warning'.
	}

	inline sint__ ConvertToSInt(
		const byte__ *DInt,
		size__ &Length )
	{
		return ConvertToSInt( DInt, &Length );
	}

}

#endif
