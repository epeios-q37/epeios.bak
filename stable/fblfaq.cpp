/*
	'fblfaq' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fblfaq' header file ('fblfaq.h').
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



//	$Id: fblfaq.cpp,v 1.8 2013/07/25 15:59:14 csimon Exp $

#define FBLFAQ__COMPILATION

#include "fblfaq.h"

class fblfaqtutor
: public ttr_tutor
{
public:
	fblfaqtutor( void )
	: ttr_tutor( FBLFAQ_NAME )
	{
#ifdef FBLFAQ_DBG
		Version = FBLFAQ_VERSION "\b\bD $";
#else
		Version = FBLFAQ_VERSION;
#endif
		Owner = FBLFAQ_OWNER;
		Date = "$Date: 2013/07/25 15:59:14 $";
	}
	virtual ~fblfaqtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace fblfaq;

using namespace fblfrd;

namespace {

	void HandleCommands_(
		const command_items_ &Items,
		type__ Type,
		backend_access___ &Backend,
		commands_ &Commands )
	{
	ERRProlog
		ctn::E_CMITEM( command_item_ ) Item;
		sdr::row__ P =E_NIL;
		command Command;
	ERRBegin
		Item.Init( Items );

		P = Items.First();

		while( P != E_NIL ) {
			Command.Init();
			Command.Identification.Value = Item( P ).Value;
			Command.Identification.ID( Item( P ).ID() );
			Backend.GetParameters( Type, Item( P ).ID(), Command.Parameters );
			Commands.Append( Command );
			P = Items.Next( P );
		}
	ERRErr
	ERREnd
	ERREpilog
	}

	void GetCommands_(
		type__ Type,
		fblfrd::backend_access___ &Backend,
		commands_ &Commands )
	{
	ERRProlog
		command_items Items;
	ERRBegin
		Items.Init();

		Backend.GetCommandsIDAndName( Type, Items );

		HandleCommands_( Items, Type, Backend, Commands );
	ERRErr
	ERREnd
	ERREpilog
	}

	void HandleType_(
		const type_item_ &Item,
		type_ &Type )
	{
		sdr::row__ P = E_NIL;
		ctn::E_CMITEM( str::string_ ) Value;

		Value.Init( Item.Values );

		Type.ID( Item.ID() );

		P = Item.Values.First();

		if ( P == E_NIL) 
			ERRFwk();

		Type.Prefix = Value( P );

		P = Item.Values.Next( P );

		if ( P == E_NIL) 
			ERRFwk();

		Type.Name = Value( P );

		P = Item.Values.Next( P );

		if ( P != E_NIL) 
			ERRFwk();
	}

	void HandleTypes_(
		const types_items_ &Items,
		fblfrd::backend_access___ &Backend,
		types_ &Types )
	{
	ERRProlog
		ctn::E_CITEM( xitem16_ ) Item;
		sdr::row__ P = E_NIL;
		type Type;
	ERRBegin
		Item.Init( Items );

		P = Items.First();

		while( P != E_NIL ) {

			Type.Init();

			HandleType_( Item( P ), Type );

			GetCommands_( Item( P ).ID(), Backend, Type.Commands );

			Types.Append( Type );
			P = Items.Next( P );
		}
	ERRErr
	ERREnd
	ERREpilog
	}

	void PutMasterType_( types_items_ &Items )
	{
	ERRProlog
		type_item Item;
	ERRBegin
		Item.Init();

		Item.Values.Append( str::string( "" ) );
		Item.Values.Append( str::string( "" ) );
		Item.ID( FBLTYP_UNDEFINED_ID16 );

		Items.Append( Item );
	ERRErr
	ERREnd
	ERREpilog
	}
}

void fblfaq::GetDescription(
	fblfrd::backend_access___ &Backend,
	types_ &Types )
{
ERRProlog
	types_items Items;
ERRBegin
	Items.Init();
	
	PutMasterType_( Items );
	
	Backend.GetTypesIDAndPrefixAndName( Items );
	
	HandleTypes_( Items, Backend, Types );
ERRErr
ERREnd
ERREpilog
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class fblfaqpersonnalization
: public fblfaqtutor
{
public:
	fblfaqpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fblfaqpersonnalization( void )
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

static fblfaqpersonnalization Tutor;

ttr_tutor &FBLFAQTutor = Tutor;
