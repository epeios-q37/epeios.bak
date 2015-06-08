/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#define FBLFRB__COMPILATION

#include "fblfrb.h"


using namespace fblfrb;

#define CIN( name, type )\
	case fblcst::c##name:\
	Flow.Put( fblcst::c##name );\
	fbltyp::Put##name( *( const fbltyp::type *)Pointer, Flow );\
	break;


void fblfrb::backend_remote_access_base___::In(
	fblcst::cast Cast,
	const void *Pointer,
	flw::ioflow__ &Flow )
{
	switch ( Cast ) {
	CIN( Object, object__)
	CIN( Boolean, boolean__ )
	CIN( Booleans, booleans_ )
	CIN( SInt, sint__ )
	CIN( SInts, sints_ )
	CIN( UInt, uint__ )
	CIN( UInts, uints_ )
	CIN( Id8, id8__ )
	CIN( Id8s, id8s_ )
	CIN( XId8s, xid8s_ )
	CIN( Id16, id16__ )
	CIN( Id16s, id16s_ )
	CIN( XId16s, xid16s_ )
	CIN( Id32, id32__ )
	CIN( Id32s, id32s_ )
	CIN( XId32s, xid32s_ )
	CIN( Id, id__ )
	CIN( Ids, ids_ )
	CIN( XIds, xids_ )
	CIN( Char, char__ )
	CIN( String, string_ )
	CIN( Strings, strings_ )
	CIN( XStrings, xstrings_ )
	CIN( Byte, byte__ )
	CIN( Binary, binary_ )
	CIN( Binaries, binaries_ )
	CIN( Item8s, item8s_ )
	CIN( Item16s, item16s_ )
	CIN( Item32s, item32s_ )
	CIN( Items, items_ )
	CIN( XItem8s, xitem8s_ )
	CIN( XItem16s, xitem16s_ )
	CIN( XItem32s, xitem32s_ )
	CIN( XItems, xitems_ )
	CIN( CommandsDetails, commands_details_ )
	CIN( ObjectsReferences, objects_references_ )
	default:
		qRFwk();
		break;
	}
}

void fblfrb::backend_remote_access_base___::Out(
		flw::ioflow__ &Flow,
		fblcst::cast Cast,
		void *Pointer )
{
	Data.Append( datum__( Cast, Pointer ) );
}

#define COUT( name, type )\
	case fblcst::c##name:\
	if ( Flow.Get() != Datum.Cast )\
		qRFwk();\
	fbltyp::Get##name( Flow, *( fbltyp::type *)Datum.Pointer );\
	break;

void Pop_( 
	const datum__ &Datum,
	flw::iflow__ &Flow )
{
	switch ( Datum.Cast ) {
	COUT( Object, object__)
	COUT( Boolean, boolean__ )
	COUT( Booleans, booleans_ )
	COUT( SInt, sint__ )
	COUT( SInts, sints_ )
	COUT( UInt, uint__ )
	COUT( UInts, uints_ )
	COUT( Id8, id8__ )
	COUT( Id8s, id8s_ )
	COUT( Id16, id16__ )
	COUT( XId8s, xid8s_ )
	COUT( Id16s, id16s_ )
	COUT( XId16s, xid16s_ )
	COUT( Id32, id32__ )
	COUT( Id32s, id32s_ )
	COUT( XId32s, xid32s_ )
	COUT( Id, id__ )
	COUT( Ids, ids_ )
	COUT( XIds, xids_ )
	COUT( Char, char__ )
	COUT( String, string_ )
	COUT( Strings, strings_ )
	COUT( XStrings, xstrings_ )
	COUT( Byte, byte__ )
	COUT( Binary, binary_ )
	COUT( Binaries, binaries_ )
	COUT( Item8s, item8s_ )
	COUT( Item16s, item16s_ )
	COUT( Item32s, item32s_ )
	COUT( Items, items_ )
	COUT( XItem8s, xitem8s_ )
	COUT( XItem16s, xitem16s_ )
	COUT( XItem32s, xitem32s_ )
	COUT( XItems, xitems_ )
	COUT( CommandsDetails, commands_details_ )
	COUT( ObjectsReferences, objects_references_ )
	default:
		qRFwk();
		break;
	}
}

void fblfrb::backend_remote_access_base___::PostProcess( flw::ioflow__ &Flow )
{
	sdr::row__ Row = Data.First();
	datum__ Datum;

	while ( Row != qNIL ) {
		Pop_( Data( Row ), Flow );

		Row = Data.Next( Row );
	}
}

