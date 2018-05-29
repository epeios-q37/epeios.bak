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

#define FBLBRR__COMPILATION

#include "fblbrr.h"

using namespace fblbrr;

#define CC( name )\
	case c##name:\
		Parameter.Init( fbltyp::New##name(), c##name );\
		break;

static parameter__ Create_( cast__ Cast )
{
	parameter__ Parameter;

	switch ( Cast ) {
	CC( Object )
	CC( Boolean )
	CC( Booleans )
	CC( SInt )
	CC( SInts )
	CC( UInt )
	CC( UInts )
	CC( SBig )
	CC( SBigs )
	CC( UBig )
	CC( UBigs )
	CC( Id8 )
	CC( Id8s )
	CC( XId8s )
	CC( Id16 )
	CC( Id16s )
	CC( XId16s )
	CC( Id32 )
	CC( Id32s )
	CC( XId32s )
	CC( Id )
	CC( Ids )
	CC( XIds )
	CC( Char )
	CC( String )
	CC( Strings )
	CC( XStrings )
	CC( Byte )
	CC( Binary )
	CC( Binaries )
	CC( Item8s )
	CC( Item16s )
	CC( Item32s )
	CC( Items )
	CC( XItem8s )
	CC( XItem16s )
	CC( XItem32s )
	CC( XItems )
	CC( CommandsDetails )
	CC( ObjectsReferences )
	default:
		qRFwk();
		break;
	}

	return Parameter;
}

#define CCAG( name, type )\
	case c##name:\
		Parameter = Create_( c##name );\
		fbltyp::Get##name( Flow, *(fbltyp::type *)Parameter.Content );\
		break;


static parameter__ _CreateAndGet(
	flw::iflow__ &Flow,
	cast__ Cast )
{
	parameter__ Parameter;

	switch ( Cast ) {
	CCAG( Object, object__)
	CCAG( Boolean, boolean__ )
	CCAG( Booleans, booleans )
	CCAG( SInt, sint__ )
	CCAG( SInts, sints )
	CCAG( UInt, uint__ )
	CCAG( UInts, uints )
	CCAG( SBig, sbig__ )
	CCAG( SBigs, sbigs )
	CCAG( UBig, ubig__ )
	CCAG( UBigs, ubigs )
	CCAG( Id8, id8__ )
	CCAG( Id8s, id8s )
	CCAG( XId8s, xid8s )
	CCAG( Id16, id16__ )
	CCAG( Id16s, id16s )
	CCAG( XId16s, xid16s )
	CCAG( Id32, id32__ )
	CCAG( Id32s, id32s )
	CCAG( XId32s, xid32s )
	CCAG( Id, id__ )
	CCAG( Ids, ids )
	CCAG( XIds, xids )
	CCAG( Char, char__ )
	CCAG( String, string )
	CCAG( Strings, strings )
	CCAG( XStrings, xstrings )
	CCAG( Byte, byte__ )
	CCAG( Binary, binary )
	CCAG( Binaries, binaries )
	CCAG( Item8s, item8s )
	CCAG( Item16s, item16s )
	CCAG( Item32s, item32s )
	CCAG( Items, items )
	CCAG( XItem8s, xitem8s )
	CCAG( XItem16s, xitem16s )
	CCAG( XItem32s, xitem32s )
	CCAG( XItems, xitems )
	CCAG( CommandsDetails, commands_details )
	CCAG( ObjectsReferences, objects_references )
	default:
		qRFwk();
		break;
	}

	return Parameter;
}

#define CD( name, type )\
	case c##name:\
		fbltyp::Delete##name( (fbltyp::type *)Parameter.Content );\
		break;

static void Delete_(
	const parameter__ &Parameter,
	cast__ Cast )
{
	if ( Parameter.Cast != Cast )
		qRFwk();

	if ( Parameter.Content == NULL )
		qRFwk();

	switch ( Parameter.Cast ) {
	CD( Object, object__)
	CD( Boolean, boolean__ )
	CD( Booleans, booleans )
	CD( SInt, sint__ )
	CD( SInts, sints )
	CD( UInt, uint__ )
	CD( UInts, uints )
	CD( SBig, sbig__ )
	CD( SBigs, sbigs )
	CD( UBig, ubig__ )
	CD( UBigs, ubigs )
	CD( Id8, id8__ )
	CD( Id8s, id8s )
	CD( XId8s, xid8s )
	CD( Id16, id16__ )
	CD( Id16s, id16s )
	CD( XId16s, xid16s )
	CD( Id32, id32__ )
	CD( Id32s, id32s )
	CD( XId32s, xid32s )
	CD( Id, id__ )
	CD( Ids, ids )
	CD( XIds, xids )
	CD( Char, char__ )
	CD( String, string )
	CD( Strings, strings )
	CD( XStrings, xstrings )
	CD( Byte, byte__ )
	CD( Binary, binary )
	CD( Binaries, binaries )
	CD( Item8s, item8s )
	CD( Item16s, item16s )
	CD( Item32s, item32s )
	CD( Items, items )
	CD( XItem8s, xitem8s )
	CD( XItem16s, xitem16s )
	CD( XItem32s, xitem32s )
	CD( XItems, xitems )
	CD( CommandsDetails, commands_details )
	CD( ObjectsReferences, objects_references )
	case cFlow:
		break;	// 'Parameter.Content' contient '&IFlow', et pas un objet cre par un 'new', donc il n'ya arien a effacer.
	default:
		qRFwk();
		break;
	}
}


void fblbrr::remote_callbacks___::FBLBRQPopIn(
	sdr::row__ CRow,
	flw::iflow__ &Channel,
	cast__ Cast )
{
	parameter__ Parameter;

	if ( Cast == cFlow ) {
		_IFlow.Init( Channel, flx::dhPropagate );
		Parameter.Init( &_IFlow, Cast );
	} else
		Parameter = _CreateAndGet( Channel, Cast );

	if ( _Parameters.Append( Parameter ) != CRow )
		qRFwk();
}

void fblbrr::remote_callbacks___::FBLBRQPopInEnd(
	sdr::row__ CRow,
	flw::iflow__ &Flow )
{
	if ( _Parameters.Append( parameter__( cEnd ) ) != CRow )
		qRFwk();
}

void fblbrr::remote_callbacks___::FBLBRQPopOut(
	sdr::row__ CRow,
	flw::iflow__ &Flow,
	cast__ Cast )
{
	if ( Cast != cFlow )	// Pour 'Cast == cFlow', '_Parameters' sera mis  jour ultrieurement.
		if ( _Parameters.Append( Create_( Cast ) ) != CRow )
			qRFwk();
}

#define CP( name, type )\
	case c##name:\
		if ( FirstCall ) {\
			Channel.Put( Parameter.Cast );\
			fbltyp::Put##name( *(fbltyp::type *)Parameter.Content, Channel );\
			Delete_( Parameter, Cast );\
		}\
		break;


static void PushAndDelete_(
	bso::bool__ FirstCall,
	flw::oflow__ &Channel,
	const parameter__ &Parameter,
	cast__ Cast )
{
	if ( Parameter.Cast != Cast )
		qRFwk();

	switch ( Parameter.Cast ) {
	CP( Object, object__)
	CP( Boolean, boolean__ )
	CP( Booleans, booleans )
	CP( SInt, sint__ )
	CP( SInts, sints )
	CP( UInt, uint__ )
	CP( UInts, uints )
	CP( SBig, sbig__ )
	CP( SBigs, sbigs )
	CP( UBig, ubig__ )
	CP( UBigs, ubigs )
	CP( Id8, id8__ )
	CP( Id8s, id8s )
	CP( XId8s, xid8s )
	CP( Id16, id16__ )
	CP( Id16s, id16s )
	CP( XId16s, xid16s )
	CP( Id32, id32__ )
	CP( Id32s, id32s )
	CP( XId32s, xid32s )
	CP( Id, id__ )
	CP( Ids, ids )
	CP( XIds, xids )
	CP( Char, char__ )
	CP( String, string )
	CP( Strings, strings )
	CP( XStrings, xstrings )
	CP( Byte, byte__ )
	CP( Binary, binary )
	CP( Binaries, binaries )
	CP( Item8s, item8s )
	CP( Item16s, item16s )
	CP( Item32s, item32s )
	CP( Items, items )
	CP( XItem8s, xitem8s )
	CP( XItem16s, xitem16s )
	CP( XItem32s, xitem32s )
	CP( XItems, xitems )
	CP( CommandsDetails, commands_details )
	CP( ObjectsReferences, objects_references )
	case cFlow:
	{
	qRH
		flx::size_embedded_oflow___ SEFlow;
		flw::iflow__ &PFlow = *(flw::iflow__ *)Parameter.Content;
	qRB
		if ( FirstCall ) 
			Channel.Put( Cast );
		else {
			SEFlow.Init( Channel, flx::chHold );

			flw::Copy( PFlow, SEFlow );

			SEFlow.Commit();
		}
	qRR
	qRT
	qRE
	}
		break;
	default:
		qRFwk();
		break;
	}
}

void fblbrr::remote_callbacks___::FBLBRQPush(
	bso::bool__ FirstCall,
	const casts_ &Casts,
	flw::oflow__ &Flow )
{
	cast__ Cast = c_Undefined;
	sdr::row__ Row = Casts.First();

	while ( ( Row != qNIL )
		    && ( ( Cast = (cast__)*Casts( Row ) ) != cEnd ) ) {
		if ( FirstCall )
			Delete_(_Parameters( Row ), Cast );
		Row = Casts.Next( Row );
	}

	if ( Row == qNIL )
		qRFwk();

	Row = Casts.Next( Row );

	while ( Row != qNIL ) {
		PushAndDelete_( FirstCall, Flow, _Parameters( Row ), (cast__)*Casts( Row ) );

		Row = Casts.Next( Row );
	}
}

