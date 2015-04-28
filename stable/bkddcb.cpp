/*
	'bkddcb' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'bkddcb' header file ('bkddcb.h').
	Copyright (C) 2001, 2003-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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
