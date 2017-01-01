/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

#define FBLCST__COMPILATION

#include "fblcst.h"


using namespace fblcst;

// MUST reflect 'bkdcst::cast'
const char *fblcst::CastsNames[c_amount] = {
	"_end_",
	"Object",
	"Boolean",
	"Booleans",
	"SInt",
	"SInts",
	"UInt",
	"UInts",
	"SBig",
	"SBigs",
	"UBig",
	"UBigs",
	"Id8",
	"Id8s",
	"XId8s",
	"Id16",
	"Id16s",
	"XId16s",
	"Id32", 
	"Id32s", 
	"XId32s", 
	"Id",
	"Ids",
	"XIds", 
	"Char",
	"String",
	"Strings",
	"XStrings",
	"Byte",
	"Binary",
	"Binaries",
	"Item8s",
	"Item16s",
	"Item32s",
	"IStrings",
	"XItem8s",
	"XItem16s",
	"XItem32s",
	"XItems",
	"CommandsDetails",
	"ObjectsReferences",
	"Flow",
};

cast__ fblcst::GetID( const char *CastName )
{
	int i = 0;

	while( ( i < c_amount ) && strcmp( CastName, CastsNames[i] ) )
		i++;

	if ( i >= c_amount )
		i = c_Undefined;

	return (cast__)i;
}

Q37_GCTOR( fblcst )
{
	if ( c_amount > bso::S8Max )
		qRChk();
}
