/*
	'fblfaq.cpp' by Claude SIMON (http://zeusw.org/).

	'fblfaq' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#define FBLFAQ__COMPILATION

#include "fblfaq.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace fblfaq;

using namespace fblfrd;

namespace {

	void HandleCommands_(
		const command_items_ &Items,
		type__ Type,
		frontend___ &Frontend,
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
			Frontend.GetParameters( Type, Item( P ).ID(), Command.Parameters );
			Commands.Append( Command );
			P = Items.Next( P );
		}
	ERRErr
	ERREnd
	ERREpilog
	}

	void GetCommands_(
		type__ Type,
		fblfrd::frontend___ &Frontend,
		commands_ &Commands )
	{
	ERRProlog
		command_items Items;
	ERRBegin
		Items.Init();

		Frontend.GetCommandsIDAndName( Type, Items );

		HandleCommands_( Items, Type, Frontend, Commands );
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
		fblfrd::frontend___ &Frontend,
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

			GetCommands_( Item( P ).ID(), Frontend, Type.Commands );

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
	fblfrd::frontend___ &Frontend,
	types_ &Types )
{
ERRProlog
	types_items Items;
ERRBegin
	Items.Init();
	
	PutMasterType_( Items );
	
	Frontend.GetTypesIDAndPrefixAndName( Items );
	
	HandleTypes_( Items, Frontend, Types );
ERRErr
ERREnd
ERREpilog
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class fblfaqpersonnalization
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


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static fblfaqpersonnalization Tutor;
