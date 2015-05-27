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

#define BKDDCB__COMPILATION

#include "bkddcb.h"


using namespace bkddcb;

using namespace bkdacc;

namespace {

	void HandleCommands_(
		const commands_items_ &Items,
		type__ Type,
		backend_access___ &Backend,
		commands_ &Commands )
	{
	ERRProlog
		ctn::E_CMITEM( command_item_ ) Item;
		epeios::row__ P =NONE;
		command Command;
	ERRBegin
		Item.Init( Items );

		P = Items.First();

		while( P != NONE ) {
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
		bkdacc::backend_access___ &Backend,
		commands_ &Commands )
	{
	ERRProlog
		commands_items Items;
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
		epeios::row__ P = NONE;
		ctn::E_CMITEM( str::string_ ) Value;

		Value.Init( Item.Values );

		Type.ID( *Item.ID() );

		P = Item.Values.First();

		if ( P == NONE) 
			ERRb();

		Type.Prefix = Value( P );

		P = Item.Values.Next( P );

		if ( P == NONE) 
			ERRb();

		Type.Name = Value( P );

		P = Item.Values.Next( P );

		if ( P != NONE) 
			ERRb();
	}

	void HandleTypes_(
		const types_items_ &Items,
		bkdacc::backend_access___ &Backend,
		types_ &Types )
	{
	ERRProlog
		ctn::E_CITEM( xitem16_ ) Item;
		epeios::row__ P = NONE;
		type Type;
	ERRBegin
		Item.Init( Items );

		P = Items.First();

		while( P != NONE ) {

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
		Item.ID( BKDTPM_UNDEFINED_ID16 );

		Items.Append( Item );
	ERRErr
	ERREnd
	ERREpilog
	}
}

void bkddcb::GetDescription(
	bkdacc::backend_access___ &Backend,
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
