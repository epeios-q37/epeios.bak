/*
	'fblbrr' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fblbrr' header file ('fblbrr.h').
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



//	$Id: fblbrr.cpp,v 1.10 2013/04/15 10:50:50 csimon Exp $

#define FBLBRR__COMPILATION

#include "fblbrr.h"

class fblbrrtutor
: public ttr_tutor
{
public:
	fblbrrtutor( void )
	: ttr_tutor( FBLBRR_NAME )
	{
#ifdef FBLBRR_DBG
		Version = FBLBRR_VERSION "\b\bD $";
#else
		Version = FBLBRR_VERSION;
#endif
		Owner = FBLBRR_OWNER;
		Date = "$Date: 2013/04/15 10:50:50 $";
	}
	virtual ~fblbrrtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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
		ERRPrm();
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
	case cFlow:
		Parameter.Init( &Flow, Cast );
		break;
	default:
		ERRPrm();
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
		ERRPrm();

	if ( Parameter.Content == NULL )
		ERRFwk();

	switch ( Parameter.Cast ) {
	CD( Object, object__)
	CD( Boolean, boolean__ )
	CD( Booleans, booleans )
	CD( SInt, sint__ )
	CD( SInts, sints )
	CD( UInt, uint__ )
	CD( UInts, uints )
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
	default:
		ERRPrm();
		break;
	}
}


void fblbrr::remote_callbacks___::FBLBRQPopIn(
	sdr::row__ CRow,
	flw::iflow__ &Flow,
	cast__ Cast )
{
	if ( _Parameters.Append( _CreateAndGet( Flow, Cast ) ) != CRow )
		ERRFwk();
}

void fblbrr::remote_callbacks___::FBLBRQPopInEnd(
	sdr::row__ CRow,
	flw::iflow__ &Flow )
{
	if ( _Parameters.Append( parameter__( cEnd ) ) != CRow )
		ERRFwk();
}

void fblbrr::remote_callbacks___::FBLBRQPopOut(
	sdr::row__ CRow,
	flw::iflow__ &Flow,
	cast__ Cast )
{
	if ( _Parameters.Append( Create_( Cast ) ) != CRow )
		ERRFwk();
}

#define CP( name, type )\
	case c##name:\
		Flow.Put( Parameter.Cast );\
		fbltyp::Put##name( *(fbltyp::type *)Parameter.Content, Flow );\
		Delete_( Parameter, Cast );\
		break;


static void PushAndDelete_(
	flw::oflow__ &Flow,
	const parameter__ &Parameter,
	cast__ Cast )
{
	if ( Parameter.Cast != Cast )
		ERRPrm();

	switch ( Parameter.Cast ) {
	CP( Object, object__)
	CP( Boolean, boolean__ )
	CP( Booleans, booleans )
	CP( SInt, sint__ )
	CP( SInts, sints )
	CP( UInt, uint__ )
	CP( UInts, uints )
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
	default:
		ERRPrm();
		break;
	}
}

void fblbrr::remote_callbacks___::FBLBRQPush(
	const casts_ &Casts,
	flw::oflow__ &Flow )
{
	cast__ Cast = c_Undefined;
	sdr::row__ Row = Casts.First();

	while ( ( Row != E_NIL )
		    && ( ( Cast = (cast__)Casts( Row ) ) != cEnd ) ) {
		Delete_(_Parameters( Row ), Cast );
		Row = Casts.Next( Row );
	}

	if ( Row == E_NIL )
		ERRFwk();

	Row = Casts.Next( Row );

	while ( Row != E_NIL ) {
		PushAndDelete_( Flow, _Parameters( Row ), (cast__)Casts( Row ) );

		Row = Casts.Next( Row );
	}
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class fblbrrpersonnalization
: public fblbrrtutor
{
public:
	fblbrrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fblbrrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static fblbrrpersonnalization Tutor;

ttr_tutor &FBLBRRTutor = Tutor;
