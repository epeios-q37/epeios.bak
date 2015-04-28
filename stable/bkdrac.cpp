/*
	'bkdrac' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'bkdrac' header file ('bkdrac.h').
	Copyright (C) 2004 Claude SIMON.

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

#define BKDRAC__COMPILATION

#include "bkdrac.h"

using namespace bkdrac;

#define CIN( name, type )\
	case bkdcst::c##name:\
	Flow.Put( bkdcst::c##name );\
	bkdtpm::Put##name( *( const bkdtpm::type *)Pointer, Flow );\
	break;


void bkdrac::backend_remote_access_base___::In(
	bkdcst::cast Cast,
	const void *Pointer,
	flw::ioflow__ &Flow )
{
	switch ( Cast ) {
	CIN( Object, object__)
	CIN( Boolean, boolean__ )
	CIN( Booleans, booleans_ )
	CIN( SLong, slong__ )
	CIN( SLongs, slongs_ )
	CIN( ULong, ulong__ )
	CIN( ULongs, ulongs_ )
	CIN( Id8, id8__ )
	CIN( Ids8, ids8_ )
	CIN( XIds8, xids8_ )
	CIN( Id16, id16__ )
	CIN( Ids16, ids16_ )
	CIN( XIds16, xids16_ )
	CIN( Id32, id32__ )
	CIN( Ids32, ids32_ )
	CIN( XIds32, xids32_ )
	CIN( Char, char__ )
	CIN( String, string_ )
	CIN( Strings, strings_ )
	CIN( XStrings, xstrings_ )
	CIN( Byte, byte__ )
	CIN( Binary, binary_ )
	CIN( Binaries, binaries_ )
	CIN( Items8, items8_ )
	CIN( Items16, items16_ )
	CIN( Items32, items32_ )
	CIN( XItems8, xitems8_ )
	CIN( XItems16, xitems16_ )
	CIN( XItems32, xitems32_ )
	CIN( CommandsDetails, commands_details_ )
	CIN( ObjectsReferences, objects_references_ )
	default:
		ERRu();
		break;
	}
}

void bkdrac::backend_remote_access_base___::Out(
		flw::ioflow__ &Flow,
		bkdcst::cast Cast,
		void *Pointer )
{
	Data.Append( datum__( Cast, Pointer ) );
}

#define COUT( name, type )\
	case bkdcst::c##name:\
	if ( Flow.Get() != Datum.Cast )\
		ERRb();\
	bkdtpm::Get##name( Flow, *( bkdtpm::type *)Datum.Pointer );\
	break;

void Pop_( 
	const datum__ &Datum,
	flw::iflow__ &Flow )
{
	switch ( Datum.Cast ) {
	COUT( Object, object__)
	COUT( Boolean, boolean__ )
	COUT( Booleans, booleans_ )
	COUT( SLong, slong__ )
	COUT( SLongs, slongs_ )
	COUT( ULong, ulong__ )
	COUT( ULongs, ulongs_ )
	COUT( Id8, id8__ )
	COUT( Ids8, ids8_ )
	COUT( Id16, id16__ )
	COUT( XIds8, xids8_ )
	COUT( Ids16, ids16_ )
	COUT( XIds16, xids16_ )
	COUT( Id32, id32__ )
	COUT( Ids32, ids32_ )
	COUT( XIds32, xids32_ )
	COUT( Char, char__ )
	COUT( String, string_ )
	COUT( Strings, strings_ )
	COUT( XStrings, xstrings_ )
	COUT( Byte, byte__ )
	COUT( Binary, binary_ )
	COUT( Binaries, binaries_ )
	COUT( Items8, items8_ )
	COUT( Items16, items16_ )
	COUT( Items32, items32_ )
	COUT( XItems8, xitems8_ )
	COUT( XItems16, xitems16_ )
	COUT( XItems32, xitems32_ )
	COUT( CommandsDetails, commands_details_ )
	COUT( ObjectsReferences, objects_references_ )
	default:
		ERRu();
		break;
	}
}

void bkdrac::backend_remote_access_base___::PostProcess( flw::ioflow__ &Flow )
{
	epeios::row__ Row = Data.First();
	datum__ Datum;

	while ( Row != NONE ) {
		Pop_( Data( Row ), Flow );

		Row = Data.Next( Row );
	}

	Data.Init();
}
