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

#define N4NJS_COMPILATION_

#include "n4njs.h"

using namespace n4njs;

namespace {
	namespace {
		template <typename t> inline void Delete_( t *&P )
		{
			if ( P != NULL )
				delete P;
		}
	}

	template <typename t> inline void Delete_( void *&P )
	{
		Delete_<t>( (t *&)P );
	}
}

void n4njs::Delete( n4njs::dArguments &Arguments )
{
	sArgument Argument;

	sdr::sRow Row = Arguments.First();

	while ( Row != qNIL ) {
		Arguments.Recall( Row, Argument );

		Argument.reset();

		Row = Arguments.Next( Row );
	}
}

void n4njs::Delete( dCallbacks &Callbacks )
{
	sdr::sRow Row = Callbacks.First();

	while ( Row != qNIL ) {
		cUCallback *Callback = Callbacks( Row );

		if ( Callback != NULL ) {
			delete Callback;
			Callbacks.Store( NULL, Row );
		}

			Row = Callbacks.Next( Row );
	}
}



