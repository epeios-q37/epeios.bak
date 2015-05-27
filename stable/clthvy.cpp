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



//	$Id: clthvy.cpp,v 1.6 2012/11/14 16:06:25 csimon Exp $

#define CLTHVY__COMPILATION

#include "clthvy.h"

using namespace clthvy;

#define CASE( n )\
	case l##n:\
		return #n;\
		break


const char *clthvy::GetLogLabel( log__ Log )
{
	switch ( Log ) {
		CASE( Creation );
		CASE( Retrieval );
		CASE( Release );
	default:
		ERRu();
		return NULL;	// Pour viter un 'warning'.
		break;
	}
}



void clthvy::core_::_DeleteFlows( void )
{
	while ( Flows.Amount() != 0 )
		delete Flows.Pop();
}
