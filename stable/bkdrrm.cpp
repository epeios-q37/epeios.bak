/*
	'bkdrrm' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'bkdrrm' header file ('bkdrrm.h').
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

#define BKDRRM__COMPILATION

#include "bkdrrm.h"

using namespace bkdrrm;

#define CC( name, type )\
	case c##name:\
		P = bkdtpm::New##name();\
		break;

void *bkdrrm::remote_request_manager::_Create(
	flw::iflow__ &Flow,
	cast Cast )
{
	void *P = NULL;

	switch ( Cast ) {
	CC( Object, object__)
	CC( Boolean, boolean__ )
	CC( Booleans, booleans )
	CC( SLong, slong__ )
	CC( SLongs, slongs )
	CC( ULong, ulong__ )
	CC( ULongs, ulongs )
	CC( Id8, id8__ )
	CC( Ids8, ids8 )
	CC( XIds8, xids8 )
	CC( Id16, id16__ )
	CC( Ids16, ids16 )
	CC( XIds16, xids16 )
	CC( Id32, id32__ )
	CC( Ids32, ids32 )
	CC( XIds32, xids32 )
	CC( Char, char__ )
	CC( String, string )
	CC( Strings, strings )
	CC( XStrings, xstrings )
	CC( Byte, byte__ )
	CC( Binary, binary )
	CC( Binaries, binaries )
	CC( Items8, items8 )
	CC( Items16, items16 )
	CC( Items32, items32 )
	CC( XItems8, xitems8 )
	CC( XItems16, xitems16 )
	CC( XItems32, xitems32 )
	CC( CommandsDetails, commands_details )
	CC( ObjectsReferences, objects_references )
	default:
		ERRc();
		break;
	}

	return P;
}

#define CCAG( name, type )\
	case c##name:\
		P = _Create( Flow, c##name );\
		bkdtpm::Get##name( Flow, *(bkdtpm::type *)P );\
		break;


void *bkdrrm::remote_request_manager::_CreateAndGet(
	flw::iflow__ &Flow,
	cast Cast )
{
	void *P = NULL;

	switch ( Cast ) {
	CCAG( Object, object__)
	CCAG( Boolean, boolean__ )
	CCAG( Booleans, booleans )
	CCAG( SLong, slong__ )
	CCAG( SLongs, slongs )
	CCAG( ULong, ulong__ )
	CCAG( ULongs, ulongs )
	CCAG( Id8, id8__ )
	CCAG( Ids8, ids8 )
	CCAG( XIds8, xids8 )
	CCAG( Id16, id16__ )
	CCAG( Ids16, ids16 )
	CCAG( XIds16, xids16 )
	CCAG( Id32, id32__ )
	CCAG( Ids32, ids32 )
	CCAG( XIds32, xids32 )
	CCAG( Char, char__ )
	CCAG( String, string )
	CCAG( Strings, strings )
	CCAG( XStrings, xstrings )
	CCAG( Byte, byte__ )
	CCAG( Binary, binary )
	CCAG( Binaries, binaries )
	CCAG( Items8, items8 )
	CCAG( Items16, items16 )
	CCAG( Items32, items32 )
	CCAG( XItems8, xitems8 )
	CCAG( XItems16, xitems16 )
	CCAG( XItems32, xitems32 )
	CCAG( CommandsDetails, commands_details )
	CCAG( ObjectsReferences, objects_references )
	default:
		ERRc();
		break;
	}

	return P;
}

#define CD( name, type )\
	case c##name:\
	bkdtpm::Delete##name( (bkdtpm::type *)P );\
		break;

void bkdrrm::remote_request_manager::_Delete(
	void *P,
	cast Cast )
{
	switch ( Cast ) {
	CD( Object, object__)
	CD( Boolean, boolean__ )
	CD( Booleans, booleans )
	CD( SLong, slong__ )
	CD( SLongs, slongs )
	CD( ULong, ulong__ )
	CD( ULongs, ulongs )
	CD( Id8, id8__ )
	CD( Ids8, ids8 )
	CD( XIds8, xids8 )
	CD( Id16, id16__ )
	CD( Ids16, ids16 )
	CD( XIds16, xids16 )
	CD( Id32, id32__ )
	CD( Ids32, ids32 )
	CD( XIds32, xids32 )
	CD( Char, char__ )
	CD( String, string )
	CD( Strings, strings )
	CD( XStrings, xstrings )
	CD( Byte, byte__ )
	CD( Binary, binary )
	CD( Binaries, binaries )
	CD( Items8, items8 )
	CD( Items16, items16 )
	CD( Items32, items32 )
	CD( XItems8, xitems8 )
	CD( XItems16, xitems16 )
	CD( XItems32, xitems32 )
	CD( CommandsDetails, commands_details )
	CD( ObjectsReferences, objects_references )
	default:
		ERRu();
		break;
	}
}


void bkdrrm::remote_request_manager::_CreateAll(
	flw::iflow__ &Flow,
	const casts_ &Casts )
{
	epeios::row__ Row = NONE;

	Row = Casts.First();

	while ( ( Row != NONE )
		    && ( Casts( Row ) != cEnd ) ) {

		if ( Flow.Get() != *Casts( Row ) )
			ERRb();

		if ( Repository_.Append( _CreateAndGet( Flow, (cast)*Casts( Row ) ) ) != Row )
			ERRc();

		Row = Casts.Next( Row );
	}

	if ( Row == NONE )
		ERRc();

	if ( Repository_.Append( (void *)NULL ) != Row )
		ERRc();

	Row = Casts.Next( Row );

	while ( Row != NONE ) {
		if ( Repository_.Append( _Create( Flow, (cast)*Casts( Row ) ) ) != Row )
			ERRc();

		Row = Casts.Next( Row );
	}
}

void bkdrrm::remote_request_manager::_DeleteAll( const casts_ &Casts )
{
#ifdef BKDRRM_DBG
	if ( Casts.Amount() != Repository_.Amount() )
		ERRu();
#endif
	if ( Casts.Amount() == 0 )
		return;

	epeios::row__ Row = Casts.First();

	while ( ( Row != NONE )
		    && ( Casts( Row ) != cEnd ) ) {
		_Delete( Repository_( Row ), (cast)*Casts( Row ) );

		Row = Casts.Next( Row );
	}

	if ( Row == NONE )
		ERRc();

	Row = Casts.Next( Row );

	while ( Row != NONE ) {
		_Delete( Repository_( Row ), (cast)*Casts( Row ) );

		Row = Casts.Next( Row );
	}
}

#define CP( name, type )\
	case c##name:\
		Flow.Put( Cast );\
		bkdtpm::Put##name( *(bkdtpm::type *)P, Flow );\
		break;


void bkdrrm::remote_request_manager::_Push(
	flw::oflow__ &Flow,
	void *P,
	cast Cast )
{
	switch ( Cast ) {
	CP( Object, object__)
	CP( Boolean, boolean__ )
	CP( Booleans, booleans )
	CP( SLong, slong__ )
	CP( SLongs, slongs )
	CP( ULong, ulong__ )
	CP( ULongs, ulongs )
	CP( Id8, id8__ )
	CP( Ids8, ids8 )
	CP( XIds8, xids8 )
	CP( Id16, id16__ )
	CP( Ids16, ids16 )
	CP( XIds16, xids16 )
	CP( Id32, id32__ )
	CP( Ids32, ids32 )
	CP( XIds32, xids32 )
	CP( Char, char__ )
	CP( String, string )
	CP( Strings, strings )
	CP( XStrings, xstrings )
	CP( Byte, byte__ )
	CP( Binary, binary )
	CP( Binaries, binaries )
	CP( Items8, items8 )
	CP( Items16, items16 )
	CP( Items32, items32 )
	CP( XItems8, xitems8 )
	CP( XItems16, xitems16 )
	CP( XItems32, xitems32 )
	CP( CommandsDetails, commands_details )
	CP( ObjectsReferences, objects_references )
	default:
		ERRc();
		break;
	}
}


void bkdrrm::remote_request_manager::BKDRQMPush(
	flw::oflow__ &Flow,
	const casts_ &Casts )
{
#ifdef BKDRRM_DBG
	if ( Casts.Amount() != Repository_.Amount() )
		ERRu();
#endif

	epeios::row__ Row = Casts.First();

	while ( ( Row != NONE )
		    && ( Casts( Row ) != cEnd ) )
		Row = Casts.Next( Row );

	if ( Row == NONE )
		ERRc();

	Row = Casts.Next( Row );

	while ( Row != NONE ) {
		_Push( Flow, Repository_( Row ), (cast)*Casts( Row ) );

		Row = Casts.Next( Row );
	}
}

