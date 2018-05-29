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


#define BOMHDL__COMPILATION

#include "bomhdl.h"
/*$BEGIN$*/


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

using namespace bomhdl;

// Pas de 'E_CDEF(...), car sinon on ne peut facielemtn dterminer la taille,  cause de '\0' contenu dans les 'BOM's.

#define UTF_32_BE "\x00\x00\xFE\xFF"
#define UTF_32_LE	"\xFF\xFE\x00\x00"
#define UTF_16_BE "\xFE\xFF"
#define UTF_16_LE	"\xFF\xFE"
#define UTF_8		"\xEF\xBB\xBF"


static stsfsm::automat Automat_;

#define M( m ) { m, sizeof( m ) - 1 }
// #define M( m ) bom__( m, strlen( m ) - 1 )

// doit relter l'ordre des dclarations dans 'byte_order_marker__'.
static bom__ BOMS_[bom_amount] =
{
    M( UTF_32_BE ),
    M( UTF_32_LE ),
    M( UTF_8 ),
    M( UTF_16_BE ),
    M( UTF_16_LE ),
};

#define ADD( name )	stsfsm::Add( name, BOMS_[bom##name].Size, bom##name, Automat_ )

static void FillAutomat_( void )
{
	Automat_.Init();

    ADD( UTF_32_BE );
    ADD( UTF_32_LE );
    ADD( UTF_16_BE );
    ADD( UTF_16_LE );
    ADD( UTF_8 );
}

void bomhdl::InitializeParser( stsfsm::parser__ &Parser )
{
	Parser.Init( Automat_ );
}

/*
static bso::bool__ Match_(
	const bom__ &Bom,
	const fdr::byte__ *Buffer,
	fdr::size__ &Size )	// Si retourne 'true', "Size' est modifi pour contenir la taille du 'BOM'.
{
	if ( ( Size >= Bom.Size ) && ( memcmp( Bom.Data, Buffer, Bom.Size ) == 0 ) ) {
		Size = Bom.Size;
		return true;
	} else
		return false;
}
*/

/* byte_order_marker__ bom::DetectBOM(
	const fdr::byte__ *Buffer,
	fdr::size__ &Size )
{
	int Position = bom_First;

	while ( ( Position <= bom_Last ) && ( !Match_( BOMS_[Position], Buffer, Size ) ) )
		Position++;

	if ( Position > bom_Last)
		Position = bom_UnknownOrNone;

	return (byte_order_marker__)Position;
}*/

struct feeder__ {
	const fdr::byte__ *Buffer;
	sdr::size__ Size;
	sdr::size__ Position;
	bso::bool__ IsEmpty( void ) const
	{
		return Position >= Size;
	}
	bso::char__ Get( void )
	{
		return Buffer[Position++];
	}
	bso::char__ View( void )
	{
		return Buffer[Position];
	}
};


byte_order_marker__ bomhdl::DetectBOM(
	const fdr::byte__ *Buffer,
	fdr::size__ &Size )
{
	if ( Size == 0 )
        return bomhdl::bom_UnknownOrNone;

	feeder__ Feeder;

	Feeder.Buffer = Buffer;
	Feeder.Size = Size;
	Feeder.Position = 0;

	byte_order_marker__ BOM = DetectBOM( Feeder, Size );

	return BOM;
}

const bom__ &bomhdl::GetBOM( byte_order_marker__ BOM )
{
	if ( BOM > bom_amount )
		qRFwk();

#ifdef CPE_S_WIN
# pragma warning(suppress: 6385)
#endif
	return BOMS_[BOM];
}

Q37_GCTOR( bomhdl )
{
	if ( bom_amount != 5 )
		qRChk();

	FillAutomat_();
}
