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

#define STR__COMPILATION

#include "str.h"

#include "flx.h"

using namespace str;

static const char Table_[] = "AAAAAACEEEE-+DNUUUUOOUUUUYaaaaaaceeeeiiiionooooouuuuyy";

static inline int GetAccentFree__( int C )
{
	if ( C >= 192 )
		C = Table_[C - 192];

	return ( C < 0 ? 256 - C : C );
}


static inline int GetAccentFree_( int C )
{
	if ( C < 0 ) {
		C = GetAccentFree__( 256 + C );

		if ( C >= 128 )
			C = C - 256;

		return C;
	} else
		return GetAccentFree__( C );
}


static inline int toxlower_( int C )
{
	return tolower( GetAccentFree_( C ) );
}

static inline int toxupper_( int C )
{
	return toupper( GetAccentFree_( C ) );
}

static bso::u8__ Convert_( bso::char__ C )
{
	if ( isdigit( C ) )
		return C - '0';
	else
		return tolower( C )- 'a' + 10;
}

template <class ostream> static void Put_(
	const string_ &String,
	ostream &OStream )		// Can be optimized by using a buffer.
{
	sdr::size__ Amount = String.Amount();

	if ( Amount != 0 )
	{
#ifdef STR_DBG
		flw::byte__ C;
#endif
		sdr::row_t__ P = 0;


		while ( P < Amount ) {
#ifdef STR_DBG
			OStream.Put( C = (flw::byte__)String.Get( P++ ) );

			if ( C == 0 )
				qRFwk();
#else
			OStream.Put( (flw::byte__)String.Get( P++ ) );
#endif
		}
	}
}


flw::oflow__ &str::operator <<(
	flw::oflow__ &OStream,
	const string_ &String )
{
	dtfptb::VPut(String.Amount(), OStream );

	Put_( String, OStream );

	return OStream;
}


flw::iflow__ &str::operator >>(
	flw::iflow__ &IStream,
	string_ &S )
{
	sdr::size__ Size = dtfptb::VGet( IStream, Size );

	while( Size-- )
		S.Append( (char)IStream.Get() );

	return IStream;
}
	

txf::text_oflow__ &str::operator <<(
	txf::text_oflow__ &OStream,
	const string_ &String )
{
	Put_( String, OStream );

	return OStream;
}

// Convertit le contenu de 'String' en majuscule.
string_ &str::ToUpper(
	string_ &String,
	bso::bool__ DontHandleAccent )
{
	sdr::row_t__ P = String.Amount();

	if ( DontHandleAccent )
		while ( P-- )
			String.Store( (char)toupper( String.Get( P ) ) , P );
	else
		while ( P-- )
			String.Store( (char)toxupper_( String.Get( P ) ) , P );

	return String;
}

// Convertit le contenu de 'String' en minuscule.
string_ &str::ToLower(
	string_ &String,
	bso::bool__ DontHandleAccent )
{
	sdr::row_t__ P = String.Amount();

	if ( DontHandleAccent )
		while ( P-- )
			String.Store( (char)tolower( String.Get( P )) , P );
	else
		while ( P-- )
			String.Store( (char)toxlower_( String.Get( P )) , P );

	return String;
}

void str::string_::FillWith( flw::iflow__ &Flow )
{
	char Buffer[1000];

	while ( !Flow.EndOfFlow() )
		Append(Buffer, Flow.ReadUpTo( sizeof( Buffer ), Buffer ) );
}

// A optimiser en utilsant un 'buffer'.
void str::string_::Dump( flw::oflow__ &Flow ) const
{
	sdr::row__ Row = First();

	while ( Row != qNIL ) {
		Flow.Put( Get( Row ) );

		Row = Next( Row );
	}
}


// Convertit la chaine 'char *' et rajoute un 0. Le pointeur retourn doit tre libr par un 'free'.
const char *string_::Convert(
	sdr::row__ Position,
	sdr::size__ Quantity,
	TOL_CBUFFER___ &Buffer ) const
{
qRH
qRB
#ifdef STR_DBG
	if ( *Position > Amount() && ( Position != 0 ) )
		qRFwk();
#endif

	if ( Quantity > ( Amount() - *Position ) )
		Quantity = Amount() - *Position;

	Buffer.Malloc( Quantity + 1 );

	if ( Quantity != 0 )
		Recall( Position, Quantity, Buffer );

	Buffer[Quantity] = 0;
qRR
qRT
qRE
	return Buffer;
}

// Could be easily optimized, would be when I have some time.
void string_::FilterOut( char Model )
{
	sdr::row_t__ Source, Dest = 0;
	char Char;

	for( Source = 0; Source < Amount(); Source++ ) {
		if ( ( Char = Get( Source ) ) != Model )
			Store( Char, Dest++ );
	}

	Allocate( Dest );
}

// Could be easily optimized.
sdr::sSize string_::AmountOfLeadChars( char Model ) const
{
	sdr::row__ Row = First();
	sdr::size__ Amount = 0;

	while ( ( Row != qNIL ) && ( Get( Row ) == Model ) ) {
		Amount++;
		Row = Next( Row );
	}

	return Amount;
}

// Could be easily optimized, would be when I have some time.
sdr::sSize string_::AmountOfTailChars(char Model) const
{
	sdr::row__ Row = Last();
	sdr::size__ Amount = 0;

	while ( ( Row != qNIL ) && ( Get( Row ) == Model ) ) {
		Amount++;
		Row = Previous( Row );
	}

	return Amount;
}

bso::sBool string_::IsBlank( void ) const
{
	flx::sStringIFlow Flow;

	Flow.Init( *this );

	while ( !Flow.EndOfFlow() && ( isspace( Flow.View() ) ) )
		Flow.Skip();

	return Flow.EndOfFlow();
}

// Could be easily optimized, would be when I have some time.
void string_::Replace(
	char Old,
	char New )
{
	sdr::row_t__ Source;
	char Char;

	for( Source = 0; Source < Amount(); Source++ ) {
		if ( ( Char = Get( Source ) ) == Old )
			Store( New, Source );
		else
			Store( Char, Source );
	}
}

bso::bool__ string_::Replace(
	char Tag,
	bso::uint__ Position,
	const str::string_ &Value )
{
	sdr::row__ Row = First();

#ifdef STR_DBG
	if ( Position == 0 )
		qRFwk();
#endif

	while ( ( Position-- ) && ( Row != qNIL ) ) {
		Row = Search( Tag, Row );
	}

	if ( Row != qNIL ) {
		Remove( Row, 1 );

		InsertAt( Value, Row );

		return true;
	} else
		return false;
}

sdr::row__ string_::Search(
	const string_ &S,
	sdr::row__ Start ) const
{
	if ( S.Amount() <= Amount() ) {
		sdr::size__ Amount = S.Amount();
		sdr::row_t__ Limit = this->Amount() - Amount;

		while( ( *Start <= Limit )
			&& ( Compare( S, *this, 0, Start, Amount ) != 0 ) )
			(*Start)++;

		if ( *Start > Limit )
			return qNIL;
		else
			return Start;
	}
	else {
		return qNIL;
	}
}

sdr::row__ string_::Search(
	char C,
	sdr::row__ Start ) const
{
	sdr::row_t__ Limit = Amount();

	while( ( *Start < Limit )
		&& ( Get( Start ) != C ) )
		(*Start)++;

	if ( *Start >= Limit )
		return qNIL;
	else
		return Start;
}

template <typename uint> static uint GenericUnsignedConversion_(
	const str::string_ &String,
	sdr::row__ Begin,
	sdr::row__ *ErrP,
	base__ BaseFlag,
	uint Limit )
{
	uint Result = 0;
	sdr::row__ &P = Begin;
	bso::u8__ C;
	bso::u8__ Base;
	uint OtherLimit = 0;

	if ( BaseFlag == bAuto ) {
		if ( ( ( String.Amount() != 0 ) ) && ( String.Get( P ) == HexadecimalMarker ) ) {
			BaseFlag = b16;
			P = String.Next( P );
		} else
			BaseFlag = b10;
	}

	Base = (int)BaseFlag;

	OtherLimit = Limit / Base;

	if ( *P < String.Amount() )
		while( P != qNIL ) {
			C = Convert_( String.Get( P ) );

			if ( C >= Base )
				break;

			if ( Result > OtherLimit )
				break;

			Result *= Base;

			if ( ( Limit - Result ) < C )
			   break;

			Result += C;

			P = String.Next( P );
		}

	if ( P != qNIL ) {
		if ( ErrP )
			*ErrP = P;
		else
			qRFwk();
	}

	return Result;
}

template <typename sint, typename uint> sint GenericSignedConversion_(
	const class string_ &String,
	sdr::row__ Begin,
	sdr::row__ *ErrP,
	base__ Base,
	sint PositiveLimit,
	sint NegativeLimit )
{
	if ( PositiveLimit < 0 )
		qRFwk();

	if ( NegativeLimit > 0 )
		qRFwk();

	if ( String.Get( Begin ) == '-' )
		if ( String.Next( Begin ) == qNIL ) {
			*ErrP = *Begin + 1;
			return 0;
		} else 
			return -(sint)GenericUnsignedConversion_<uint>( String, String.Next( Begin ), ErrP, Base, -NegativeLimit );
	else if ( String.Get( Begin ) == '+' )
		if ( String.Next( Begin ) == qNIL ) {
			if ( ErrP != NULL )
				*ErrP = *Begin + 1;
			else
				qRFwk();
			return 0;
		} else 
			return (sint)GenericUnsignedConversion_( String, String.Next( Begin ), ErrP, Base, PositiveLimit );
	else 
		return (sint)GenericUnsignedConversion_( String, Begin, ErrP, Base, PositiveLimit );
}

unsigned long long int str::_UConversion(
	const str::string_ &String,
	sdr::row__ Begin,
	sdr::row__ *ErrP,
	base__ BaseFlag,
	unsigned long long int Limit )
{
	return GenericUnsignedConversion_( String, Begin, ErrP, BaseFlag, Limit );
}

signed long long int str::_SConversion(
	const class string_ &String,
	sdr::row__ Begin,
	sdr::row__ *ErrP,
	base__ Base,
	signed long long int PositiveLimit,
	signed long long int NegativeLimit )
{
	return GenericSignedConversion_<bso::s64__, bso::u64__>( String, Begin, ErrP, Base, PositiveLimit, NegativeLimit );
}
/*
uint__ str::_UIntConversion(
	const str::string_ &String,
	sdr::row__ Begin,
	sdr::row__ *ErrP,
	base__ BaseFlag,
	uint__ Limit )
{
	return GenericUnsignedConversion_( String, Begin, ErrP, BaseFlag, Limit );
}

sint__ str::_SIntConversion(
	const class string_ &String,
	sdr::row__ Begin,
	sdr::row__ *ErrP,
	base__ Base,
	sint__ PositiveLimit,
	sint__ NegativeLimit )
{
	return GenericSignedConversion_<sint__, uint__>( String, Begin, ErrP, Base, PositiveLimit, NegativeLimit );
}
*/
bso::lfloat__ string_::ToLF(
	sdr::row__ *ErrP,
	sdr::row__ Begin ) const
{
	bso::bool__ Negate = false;
	bso::lfloat__ Result = 0;
	sdr::row__ &P = Begin;
	unsigned char C;

	if ( *P < Amount() ) {
		if ( P != qNIL ) {
			if ( Get( P ) == '-' ) {
				Negate = true;
				P = Next( P );
			} else if ( Get( P ) == '+' ) {
				P = Next( P );
			}
		}

		while( ( P != qNIL ) && isdigit( C = Get( P ) ) && ( Result < ( BSO_LFLOAT_MAX / 10 ) ) ) {
			Result = Result * 10 + C - '0';
			P = Next( P );
		}

		if ( ( P != qNIL )
			  && ( ( ( C = Get( P ) ) == '.' ) || ( C == ',' ) ) ) {
			bso::lfloat__ Factor = .1;
			P = Next( P );
			while( ( P != qNIL ) && isdigit( C = Get( P ) ) ) {
				Result += ( C - '0' ) * Factor;
				Factor /= 10;
				P = Next( P );
			}
		}
	}

	if ( P != qNIL ) {
		if ( ErrP )
			*ErrP = P;
		else
			qRFwk();
	}

	if ( Negate )
		return -Result;
	else
		return Result;
}
