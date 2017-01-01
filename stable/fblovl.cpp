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

#define FBLOVL__COMPILATION

#include "fblovl.h"

#include "err.h"

using namespace fblovl;

#define CASE( m )\
	case r##m:\
	return #m;\
	break

const char *fblovl::GetLabel( reply__ Reply )
{
	switch ( Reply ) {
	CASE( OK );
	CASE( SoftwareError );
	CASE( RequestError );
	CASE( Disconnected );
	default:
		qRFwk();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}

