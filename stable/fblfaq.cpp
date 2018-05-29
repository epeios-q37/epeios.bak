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

#define FBLFAQ__COMPILATION

#include "fblfaq.h"

using namespace fblfaq;

using namespace fblfrd;

namespace {

	void HandleCommands_(
		const command_items_ &Items,
		type__ Type,
		frontend___ &Frontend,
		commands_ &Commands )
	{
	qRH
		sdr::row__ P =qNIL;
		command Command;
	qRB
		P = Items.First();

		while( P != qNIL ) {
			Command.Init();
			Command.Identification.Value = Items( P ).Value;
			Command.Identification.ID( Items( P ).ID() );
			Frontend.GetParameters( Type, Items( P ).ID(), Command.Parameters );
			Commands.Append( Command );
			P = Items.Next( P );
		}
	qRR
	qRT
	qRE
	}

	void GetCommands_(
		type__ Type,
		fblfrd::frontend___ &Frontend,
		commands_ &Commands )
	{
	qRH
		command_items Items;
	qRB
		Items.Init();

		Frontend.GetCommandsIDAndName( Type, Items );

		HandleCommands_( Items, Type, Frontend, Commands );
	qRR
	qRT
	qRE
	}

	void HandleType_(
		const type_item_ &Item,
		type_ &Type )
	{
		sdr::row__ P = qNIL;

		Type.SetID( Item.ID() );

		P = Item.Values.First();

		if ( P == qNIL) 
			qRFwk();

		Type.Prefix = Item.Values( P );

		P = Item.Values.Next( P );

		if ( P == qNIL) 
			qRFwk();

		Type.Name = Item.Values( P );

		P = Item.Values.Next( P );

		if ( P != qNIL) 
			qRFwk();
	}

	void HandleTypes_(
		const types_items_ &Items,
		fblfrd::frontend___ &Frontend,
		types_ &Types )
	{
	qRH
		sdr::row__ P = qNIL;
		type Type;
	qRB
		P = Items.First();

		while( P != qNIL ) {

			Type.Init();

			HandleType_( Items( P ), Type );

			GetCommands_( Items( P ).ID(), Frontend, Type.Commands );

			Types.Append( Type );
			P = Items.Next( P );
		}
	qRR
	qRT
	qRE
	}

	void PutMasterType_( types_items_ &Items )
	{
	qRH
		type_item Item;
	qRB
		Item.Init();

		Item.Values.Append( str::string( "" ) );
		Item.Values.Append( str::string( "" ) );
		Item.ID( FBLTYP_UNDEFINED_ID16 );

		Items.Append( Item );
	qRR
	qRT
	qRE
	}
}

void fblfaq::GetDescription(
	fblfrd::frontend___ &Frontend,
	types_ &Types )
{
qRH
	types_items Items;
qRB
	Items.Init();
	
	PutMasterType_( Items );
	
	Frontend.GetTypesIDAndPrefixAndName( Items );
	
	HandleTypes_( Items, Frontend, Types );
qRR
qRT
qRE
}
