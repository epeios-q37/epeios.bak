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

#define BKDCST__COMPILATION

#include "bkdcst.h"

using namespace bkdcst;

// MUST reflect 'bkdcst::cast'
const char *bkdcst::CastsNames[c_amount] = {
	"_end_",
	"Object",
	"Boolean",
	"Booleans",
	"SLong",
	"SLongs",
	"ULong",
	"ULongs",
	"Id8",
	"Ids8",
	"XIds8",
	"Id16",
	"Ids16",
	"XIds16",
	"Id32", 
	"Ids32", 
	"XIds32", 
	"Char",
	"String",
	"Strings",
	"XStrings",
	"Byte",
	"Binary",
	"Binaries",
	"Items8",
	"Items16",
	"Items32",
	"XItems8",
	"XItems16",
	"XItems32",
	"CommandsDetails",
	"ObjectsReferences"
};

cast bkdcst::GetID( const char *CastName )
{
	int i = 0;

	while( ( i < c_amount ) && strcmp( CastName, CastsNames[i] ) )
		i++;

	if ( i >= c_amount )
		i = c_Unknow;

	return (cast)i;
}
