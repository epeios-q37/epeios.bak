/*
	'bomhdl' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'bomhdl' header file ('bomhdl.h').
	Copyright (C) 20132004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/


#define BOMHDL__COMPILATION

#include "bomhdl.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
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

static bso::bool__ Match_(
	const bom__ &Bom,
	const fdr::datum__ *Buffer,
	fdr::size__ &Size )	// Si retourne 'true', "Size' est modifi pour contenir la taille du 'BOM'.
{
	if ( ( Size >= Bom.Size ) && ( memcmp( Bom.Data, Buffer, Bom.Size ) == 0 ) ) {
		Size = Bom.Size;
		return true;
	} else
		return false;
}


/* byte_order_marker__ bom::DetectBOM(
	const fdr::datum__ *Buffer,
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
	const fdr::datum__ *Buffer;
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
};


byte_order_marker__ bomhdl::DetectBOM(
	const fdr::datum__ *Buffer,
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
		ERRPrm();

#pragma warning(suppress: 6385)
	return BOMS_[BOM];
}

Q37_GCTOR( bomhdl )
{
	if ( bom_amount != 5 )
		ERRChk();

	FillAutomat_();
}
