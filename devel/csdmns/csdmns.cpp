/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#define CSDMNS__COMPILATION

#include "csdmns.h"

using namespace csdmns;

#define CASE( n )\
	case l##n:\
		return #n;\
		break

const char *csdmns::GetLogLabel( log__ Log )
{
	switch ( Log ) {
		CASE( New );
		CASE( Store );
		CASE( TestAndGet );
		CASE( Delete );
	default:
		qRFwk();
		return NULL;	// Pour viter un 'warning'.
		break;
	}
}

void csdmns::_callback___::_Clean( void )
{
	sdr::row__ Row = _Core.UPs.First();

	while ( Row != qNIL ) {
		_Callback->PostProcess( _Core.UPs( Row ) );

		Row = _Core.UPs.Next( Row );
	}

	_Core.reset();
}

