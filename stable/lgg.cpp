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

