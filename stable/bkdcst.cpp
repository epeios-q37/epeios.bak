/*
	'bkdcst' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'bkdcst' header file ('bkdcst.h').
	Copyright (C) 2001, 2003-2004 Claude SIMON.

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
