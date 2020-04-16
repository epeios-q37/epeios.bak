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

// STRing

#ifndef STR_INC_
# define STR_INC_

# define STR_NAME		"STR"

# if defined( E_DEBUG ) && !defined( STR_NODBG )
#  define STR_DBG
# endif

/* Begin of automatic documentation generation part. */

# include "err.h"
# include "txf.h"
# include "bch.h"
# include "cpe.h"
# include "ctn.h"
# include "crt.h"

/*************************/
/****** Old version ******/
/*************************/


namespace str {

	E_CDEF( char, HexadecimalMarker, '#' );

	namespace {
		using bch::bunch_;
		using bso::uint__;
		using bso::sint__;
	}

	enum base__
	: bso::u8__
	{
		bAuto = 0,	// Dtermination automatique de la base (suivant le premier caractre).
		b10 = 10,	// Base dcimale.
		b16 = 16,	// Base hexadcimale.
	};

	class string_;	// Prdclaration.

	long long unsigned _UConversion(
		const class string_ &String,
		sdr::row__ Begin,
		sdr::row__ *ErrP,
		base__ Base,
		long long unsigned Limit );

	long long signed _SConversion(
		const class string_ &String,
		sdr::row__ Begin,
		sdr::row__ *ErrP,
		base__ Base,
		long long signed PositiveLimit,
		long long signed NegativeLimit );

	class _string_size_handler {
	public:
		static sdr::size__ SizeOf( const char *S )
		{
			if ( NULL == S )
				S = "";

			return strlen( S );
		}
	};

	using uys::sHook;

	//c A string.
	class string_
	: public E_BUNCHx_( bso::char__, _string_size_handler )
	{
	private:
	public:
		struct s
		: public E_BUNCHx_( bso::char__, _string_size_handler )::s {};
		string_( s &S )
		: E_BUNCHx_( bso::char__, _string_size_handler )( S )
		{}
		void reset( bool P = true )
		{
			E_BUNCHx_( bso::char__, _string_size_handler )::reset( P );
		}
		/*
		void plug( qSD__ &Driver )
		{
			E_BUNCHx_( bso::char__, _string_size_handler )::plug( Driver );
		}
		void plug( qAS_ &AS )
		{
			E_BUNCHx_( bso::char__, _string_size_handler )::plug( AS );
		}
		*/
		string_ &operator =( const string_ &O )
		{
			E_BUNCHx_( bso::char__, _string_size_handler )::operator =( O );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			E_BUNCHx_( bso::char__, _string_size_handler )::Init();
		}
		//f Initialization with 'Seed'.
		void Init( const bso::char__ *Seed )
		{
			if ( NULL == Seed )
				Seed = "";

			E_BUNCHx_( bso::char__, _string_size_handler )::Init( Seed, strlen( Seed ) );
		}
		//f Initialization with 'Seed'.
		void Init( const str::string_ &Seed )
		{
			if ( this != &Seed ) {
				Init();

				this->operator =( Seed );
			}
		}
		string_ &operator =( const char *Chaine )
		{
			if ( NULL == Chaine )
				Chaine = "";

			Init();
			E_BUNCHx_( bso::char__, _string_size_handler )::StoreAndAdjust( Chaine, strlen( Chaine ) );

			return *this;
		}
		void FillWith( flw::iflow__ &Flow );
		void Dump( flw::oflow__ &Flow ) const;
		//f Convert 'Amount' characters at 'Position' from string to a 'char *'.
		const char *Convert(
			sdr::row__ Position,
			sdr::size__ Amount,
			TOL_CBUFFER___ &Buffer ) const;
		const char *Convert(
			sdr::row__ Position,
			TOL_CBUFFER___ &Buffer ) const
		{
			return Convert( Position, TYS_MAX_SIZE, Buffer );
		}
		const char *Convert( TOL_CBUFFER___ &Buffer ) const
		{
			return Convert( 0, TYS_MAX_SIZE, Buffer );
		}
		//f Filter out the 'Model' charactere.
		void FilterOut( char Model );
		sdr::sSize AmountOfLeadChars(char model = ' ') const;
		sdr::sSize AmountOfTailChars(char model = ' ') const;
		//f Remove characters 'Model' beginnig the string.
		void StripLeadingChars(char Model = ' ') {
			Remove(0, AmountOfLeadChars(Model));
		}
		//f Remove characters 'Model' at ending the string.
		void StripTailingChars(char Model = ' ') {
			Truncate(AmountOfTailChars(Model));
		}
		//f Remove characters 'Model' both beginning and ending the string.
		void StripChars(char Model = ' ')
		{
			StripLeadingChars( Model );
			StripTailingChars( Model );
		}
		bso::sBool IsBlank( void ) const;	// Returns 'true' if contains only blan characters.
		string_ &Truncate( sdr::row__ Row )
		{
			E_BUNCHx_( bso::char__, _string_size_handler )::Truncate( Row );

			return *this;
		}
		string_ &Truncate( bso::size__ Amount = 1 )
		{
			E_BUNCHx_( bso::char__, _string_size_handler )::Truncate( Amount );

			return *this;
		}
		//f Replace the 'Old' char with the 'New' one.
		void Replace(
			char Old,
			char New );
		// Remplace la 'Position'ime occurence de 'Tag' par 'Value'. Retourne 'true' si succs, 'false' sinon.
		bso::bool__ Replace(
			char Tag,
			bso::uint__ Position,
			const string_ &Value );
		//f Return the position of the first occurence of 'S', beginning at 'Start'. Return 'qNIL' if no occurence.
		sdr::row__ Search(
			const string_ &S,
			sdr::row__ Start = 0 ) const;
		//f Return the position of the first occurence of 'C', beginning at 'Start'. Return 'qNIL' if no occurence.
		sdr::row__ Search(
			char C,
			sdr::row__ Start = 0 ) const;
		// NOTA : Les mthodes 'ToNumber'(...)' facilitent la mise en oeuvre de 'template's.
# define STR_UN( name, type, limit )\
	type To##name(\
			sdr::row__ Begin,\
			sdr::row__ *ErrP = NULL,\
			base__ Base = bAuto,\
			type Limit = limit ) const\
		{\
			return (type)_U##Conversion( *this, Begin, ErrP, Base, Limit );\
		}\
		type To##name(\
			sdr::row__ *ErrP = NULL,\
			base__ Base = bAuto,\
			type Limit = limit ) const\
		{\
			return To##name( 0, ErrP, Base, Limit );\
		}
# define STR_TUN( type, limit )\
		void ToNumber( \
			   type &Number, \
			   sdr::row__ *Error = NULL ) const\
		{\
			Number = (type)_UConversion( *this, 0, Error, bAuto, limit );\
		}\
		void ToNumber(\
			type &Number,\
			type Limit,\
			sdr::row__ *Error = NULL ) const\
		{\
			Number = (type)_UConversion( *this, 0, Error, bAuto, Limit );\
		}\
		void ToNumber(\
			type &Number,\
			sdr::row__ Begin,\
			sdr::row__ *Error = NULL ) const\
		{\
			Number = (type)_UConversion( *this, Begin, Error, bAuto, limit );\
		}\
		void ToNumber(\
			type &Number,\
			type Limit,\
			sdr::row__ Begin,\
			sdr::row__ *Error = NULL ) const\
		{\
			Number = (type)_UConversion( *this, Begin, Error, bAuto, Limit );\
		}
# define STR_SN( name, type, positive_limit, negative_limit )\
	type To##name(\
			sdr::row__ Begin,\
			sdr::row__ *ErrP,\
			base__ Base,\
			type PositiveLimit = positive_limit,\
			type NegativeLimit = negative_limit ) const\
		{\
			return (type)_SConversion( *this, Begin, ErrP, Base, PositiveLimit, NegativeLimit );\
		}\
		type To##name(\
			sdr::row__ *ErrP = NULL,\
			base__ Base = bAuto,\
			type PositiveLimit = positive_limit,\
			type NegativeLimit = negative_limit ) const\
		{\
			return To##name( 0, ErrP, Base, PositiveLimit, NegativeLimit );\
		}
# define STR_TSN( type, negative_limit, positive_limit )\
		void ToNumber(\
			type &Number,\
			sdr::row__ *Error = NULL ) const\
		{\
			Number = (type)_SConversion( *this, 0, Error, bAuto, positive_limit, negative_limit );\
		}\
		void ToNumber(\
			type &Number,\
			type PositiveLimit,\
			type NegativeLimit,\
			sdr::row__ *Error = NULL ) const\
		{\
			Number = (type)_SConversion( *this, 0, Error, bAuto, PositiveLimit, NegativeLimit );\
		}
		STR_UN( Row, sdr::row_t__, SDR_ROW_T_MAX )
		STR_UN( UInt, bso::uint__, BSO_UINT_MAX )
		STR_SN( SInt, bso::sint__, BSO_SINT_MAX, BSO_SINT_MIN )
# ifdef CPE_F_64BITS
		STR_UN( U64, bso::u64__, BSO_U64_MAX )
		STR_SN( S64, bso::s64__, BSO_S64_MAX, BSO_S64_MIN )
# elif defined( CPE_F_32BITS )
		STR_UN( U64, bso::u64__, BSO_U64_MAX )
		STR_SN( S64, bso::s64__, BSO_S64_MAX, BSO_S64_MIN )
# else
#  error
# endif
		STR_UN( U32, bso::u32__, BSO_U32_MAX )
		STR_SN( S32, bso::s32__, BSO_S32_MAX, BSO_S32_MIN )
		STR_UN( U16, bso::u16__, BSO_U16_MAX )
		STR_SN( S16, bso::s16__, BSO_S16_MAX, BSO_S16_MIN )
		STR_UN( U8, bso::u8__, BSO_U8_MAX )
		STR_SN( S8, bso::s8__, BSO_S8_MAX, BSO_S8_MIN )
		bso::lfloat__ ToLF(
			sdr::row__ *ErrP,
			sdr::row__ Begin ) const;
		bso::lfloat__ ToLF( sdr::row__ *ErrP = NULL ) const
		{
			return ToLF( ErrP, 0 );
		}
		void *ToPointer( sdr::row__ *ErrP = NULL )
		{
# ifdef CPE_F_64BITS
			return (void *)ToU64( ErrP, str::b16 );
# elif defined ( CPE_F_32BITS )
			return (void *)ToU32( ErrP, str::b16 );
# else
#  error "Unknown integer natural size !"
# endif
		}
		void ToNumber(
			bso::lfloat__ &Number,
			sdr::row__ *Error = NULL ) const
		{
			Number = ToLF( Error );
		}
		STR_TUN( long long unsigned int, ULLONG_MAX )
		STR_TUN( long unsigned int, ULONG_MAX )
		STR_TUN( unsigned int, UINT_MAX )
		STR_TUN( unsigned short, USHRT_MAX )
		STR_TUN( unsigned char, UCHAR_MAX )
		STR_TSN( long long signed int, LLONG_MIN, LLONG_MAX )
		STR_TSN( long signed int, LONG_MIN, LONG_MAX )
		STR_TSN( signed int , INT_MIN, INT_MAX )
		STR_TSN( signed short, SHRT_MIN, SHRT_MAX )
		STR_TSN( signed char, SCHAR_MIN, SCHAR_MAX )
	};

	template <typename type> inline void ToEnum(
		const str::string_ &String,
		type &Number )
	{
		String.ToNumber( *(bso::enum__ *)&Number );
	}

	template <typename type> inline type ToEnum( const str::string_ &String )
	{
		type Number;

		String.ToNumber( *(bso::enum__ *)&Number );

		return Number;
	}

	//f Put all the caracters of 'String' on 'Ostream', and put a 'NULL' character.
	flw::oflow__ &operator <<(
		flw::oflow__ &OStream,
		const string_ &String );

	//f Append all the characters from 'IStream' to 'String' until encounter a 'NULL' character.
	flw::iflow__ &operator >>(
		flw::iflow__ &IStream,
		string_ &String );


	txf::text_oflow__ &operator <<(
		txf::text_oflow__ &OStream,
		const string_ &String );

	//f Return 'S1' - 'S2' beginning at 'BeginS1' and 'BeginS2' and a length of 'Amount'.
	inline bso::sign__ Compare(
		const string_ &S1,
		const string_ &S2,
		sdr::row__ BeginS1,
		sdr::row__ BeginS2,
		sdr::size__ Amount )
	{
		return bch::Compare( S1, S2, BeginS1 ,BeginS2 , Amount );
	}

	//f Return 'S1' - 'S2' beginning at 'BeginS1' and 'BeginS2'.
	inline bso::sign__ Compare(
		const string_ &S1,
		const string_ &S2,
		sdr::row__ BeginS1 = 0,
		sdr::row__ BeginS2 = 0 )
	{
		bso::sign__ Resultat;
		sdr::size__ T1 = S1.Amount() - *BeginS1, T2 = S2.Amount() - *BeginS2;

		if ( ( Resultat = Compare( S1, S2, BeginS1, BeginS2, T1 < T2 ? T1 : T2 ) ) != 0 )
			return Resultat;
		else
			return bso::Compare ( T1, T2 );
	}

	inline bso::bool__ operator ==(
		const string_ &S1,
		const string_ &S2 )
	{
		return Compare( S1, S2 ) == 0;
	}

	inline bso::bool__ operator !=(
		const string_ &S1,
		const string_ &S2 )
	{
		return Compare( S1, S2 ) != 0;
	}

	inline bso::bool__ operator <(
		const string_ &S1,
		const string_ &S2 )
	{
		return Compare( S1, S2 ) < 0;
	}

	inline bso::bool__ operator <=(
		const string_ &S1,
		const string_ &S2 )
	{
		return Compare( S1, S2 ) <= 0;
	}

	inline bso::bool__ operator >(
		const string_ &S1,
		const string_ &S2 )
	{
		return Compare( S1, S2 ) > 0;
	}

	inline bso::bool__ operator >=(
		const string_ &S1,
		const string_ &S2 )
	{
		return Compare( S1, S2 ) >= 0;
	}

	class string
	: public string_
	{
	public:
		string_::s static_;
		string( void )
		: string_( static_ )
		{
			reset( false );
		}
		string( const char *S )
		: string_( static_ )
		{
			reset( false );

			string_::Init();

			if ( S != NULL )
				string_::Append( S );
		}
		string(
			const char *S,
			sdr::size__ Length )
		: string_( static_ )
		{
			reset( false );

			string_::Init();
			string_::Append( S, Length );
		}
		string( char C )
		: string_( static_ )
		{
			reset( false );

			string_::Init();
			string_::Append( C );
		}
		string( const string_ &String )
		: string_( static_ )
		{
			reset( false );

			string_::Init();
			string_::operator =( String );
		}
		string( const string &String )
		: string_( static_ )
		{
			reset( false );

			string_::Init();
			string_::operator =( String );
		}
		~string( void )
		{
			reset( true );
		}
		string &operator =( const string &S )
		{
			string_::operator =( S );

			return *this;
		}
		string &operator =( const string_ &S )
		{
			string_::operator =( S );

			return *this;
		}
		string &operator =( const char *O )
		{
			string_::operator =( O );

			return *this;
		}
		string_ &operator *( void )
		{
			return *this;
		}
		const string_ &operator *( void ) const
		{
			return *this;
		}
	};

	template <typename row> E_TTCLONE_( ctn::E_MCONTAINERt_( str::string_, row ), tstrings_ );

	typedef tstrings_<sdr::sRow> strings_;
	E_AUTO(strings)

	inline bso::bool__ operator ==(
		const str::string_ &Op1,
		const char *Op2 )
	{
		return Op1 == str::string( Op2 );
	}

	inline bso::bool__ operator !=(
		const str::string_ &Op1,
		const char *Op2 )
	{
		return Op1 != str::string( Op2 );
	}

	inline bso::bool__ operator <(
		const str::string_ &Op1,
		const char *Op2 )
	{
		return Op1 < str::string( Op2 );
	}

	inline bso::bool__ operator <=(
		const str::string_ &Op1,
		const char *Op2 )
	{
		return Op1 <= str::string( Op2 );
	}

	inline bso::bool__ operator >=(
		const str::string_ &Op1,
		const char *Op2 )
	{
		return Op1 >= str::string( Op2 );
	}

	inline bso::bool__ operator >(
		const str::string_ &Op1,
		const char *Op2 )
	{
		return Op1 > str::string( Op2 );
	}

	//f Convert 'String' to upper case. Returns cnverted 'String'.
	string_ &ToUpper(
		string_ &String,
		bso::bool__ DontHandleAccent = false );

	//f Convert 'String' to lower case. Returns converted 'String'.
	string_ &ToLower(
		string_ &String,
		bso::bool__ dontHandleAccent = false );

	template <typename t, typename row> inline void Append(
		t Value,
		str::tstrings_<row> &Values )
	{
		bso::buffer__ Buffer;

		Values.Append( str::string( bso::Convert( Value, Buffer ) ) );
	}
 #if 0
	template <typename t> inline void AppendInt(
		t Value,
		str::strings_ &Values )
	{
		Append<t, bso::integer_buffer__>( Value, Values );
	}

	template <typename t> inline void AppendFloat(
		t Value,
		str::strings_ &Values )
	{
		Append<t, bso::float_buffer__>( Value, Values );
	}
# endif
# if 0
# ifdef CPE_F_32BITS
	inline void Append(
		bso::u64__ Value,
		str::strings_ &Values )
	{
		AppendInt( Value, Values );
	}

	inline void Append(
		bso::s64__ Value,
		str::strings_ &Values )
	{
		AppendInt( Value, Values );
	}
# endif

	inline void Append(
		bso::uint__ Value,
		str::strings_ &Values )
	{
		AppendInt( Value, Values );
	}

	inline void Append(
		bso::sint__ Value,
		str::strings_ &Values )
	{
		AppendInt( Value, Values );
	}
# endif
	template <typename row> inline row Search(
		const str::string &String,
		const ctn::mono_container_<str::string_,row> &Strings,
		sdr::sRow First = qNIL )
	{
		return ctn::Search<row, string_>( String, Strings, First );
	}

	template <typename row> inline sdr::sRow NewAndInit( str::tstrings_<row> &Strings )
	{
		return crt::NewAndInit( Strings );
	}
}

/*************************/
/****** New version ******/
/*************************/

namespace str {
	typedef string_ dString;
	typedef string wString;

	template <typename row> class dTStrings_
	: public tstrings_<row>
	{
	public:
        using tstrings_<row>::tstrings_;
        using tstrings_<row>::Append;
        sdr::sRow Append(const char *String)
        {
            return tstrings_<row>::Append(wString(String));
        }
	};

//	template <typename row> qTCLONE(dTStrings_<row>, dTStrings);

    typedef dTStrings_<sdr::sRow> dStrings;

	qW1(TStrings_);

	template <typename row> class wTStrings
	: public wTStrings_<row>
	{
	public:
        using wTStrings_<row>::wTStrings_;
        using wTStrings_<row>::Init;
        void Init( const str::dString &String)
        {
            Init();
            wTStrings_<row>::Append(String);
        }
        void Init( const bso::sChar *String)
        {
            Init();
            wTStrings_<row>::Append(String);
        }
		wTStrings(void) // Needed by 'Windows'.
		: wTStrings_<row>()
		{}
		wTStrings(const dString &String)
        : wTStrings_<row>()
		{
			Init(String);
		}
		wTStrings(const char *String)
        : wTStrings_<row>()
        {
            Init(String);
        }
    };

    typedef wTStrings<sdr::sRow> wStrings;


    extern const string_ &Empty;   // An empty string.
}

#define qSTRING( name )\
	typedef str::dString d##name;\
	typedef str::wString w##name

#define qSTRINGS( name, row )\
	typedef crt::qMCRATEd( str::dString, row ) d##name;\
	typedef crt::qMCRATEw( str::dString, row ) w##name

#define qSTRINGSl( name ) qSTRINGS( name, sdr::sRow )

#endif
