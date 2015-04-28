/*
	'lgg' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'lgg' header file ('lgg.h').
	Copyright (C) 2000, 2002-2004 Claude SIMON.

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

#define LGG__COMPILATION

#include "lgg.h"

using namespace lgg;

const char *lgg::LanguageNames[lgg::l_amount] = {
	"raw",
	"English",
	"Franais",
	"Deutsch",
	"Espaol",
	"Nederlands",
	"Portugus"
 };

const char *lgg::LanguageLabels[lgg::l_amount] = {
	"xx",
	"en",
	"fr",
	"de",
	"es",
	"nl",
	"pt"
};

language__ lgg::GetLanguageIDNamed( const char *Name )
{
	int i = 0;

	while( ( i < l_amount ) && ( strcmp( Name, LanguageNames[i] ) ) )
		i++;

	if ( i >= l_amount )
		return l_undefined;
	else
		return (language__)i;
}

language__ lgg::GetLanguageIDWithLabel( const char *Name )
{
	int i = 0;

	while( ( i < l_amount ) && ( strcmp( Name, LanguageLabels[i] ) ) )
		i++;

	if ( i >= l_amount )
		return l_undefined;
	else
		return (language__)i;
}

