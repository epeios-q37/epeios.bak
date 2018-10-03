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

#define SCLI_COMPILATION_

#include "scli.h"

#include "tol.h"

using namespace scli;

namespace {
	// Use an new uuid if the 'sInfo' object is modified !
	qCDEF( char *, Id_, "ac41a47a-1661-4b60-b940-82c9cae00c7e" );
}

const char *scli::sInfo::Id_ = ::Id_;

void scli::sInfo::Control_( void )
{
	if ( strcmp( scli::sInfo::Id_, ::Id_ ) )
		qRFwk();
}
