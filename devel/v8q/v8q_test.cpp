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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v8q.h"

#include "err.h"
#include "cio.h"

using cio::CIn;
using cio::COut;
using cio::CErr;

void Generic( int argc, char *argv[] )
{
qRH
qRB
qRR
qRT
qRE
}

namespace {
	void ErrFinal_( void )
	{
		if ( ERRType != err::t_Abort ) {
			err::buffer__ Buffer;

			const char *Message = err::Message( Buffer );

			ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.

			cio::COut << txf::commit;
			cio::CErr << txf::nl << txf::tab << "{ " << Message << " }" << txf::nl << txf::commit;
		} else
			ERRRst();
	}
}

int main( int argc, char *argv[] )
{
	int ExitValue = EXIT_SUCCESS;
qRFH
qRFB
	cio::Initialize( cio::t_Default );

	COut << "Test of library " << V8Q_NAME << ' ' << __DATE__ " " __TIME__ "\n";

	Generic( argc, argv );
qRFR
	ExitValue = EXIT_FAILURE;
qRFT
qRFE( ErrFinal_() )
	return ExitValue;
}