/*
	'tol_test.cpp' by Claude SIMON (http://zeusw.org/).

	'tol_test.cpp' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tol.h"

#include "err.h"
#include "cio.h"

using cio::CIn;
using cio::COut;
using cio::CErr;

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
	tol::System("/s \"c:/Program Files/7-Zip/7z.exe\" \"a\"");
	tol::System("\"c:\\Program Files\\7-Zip\\7z.exe\" \"a\"");
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
	int ExitValue = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	COut << "Test of library " << TOL_NAME << ' ' << __DATE__" "__TIME__"\n";

	Generic( argc, argv );
ERRFErr
	ExitValue = EXIT_FAILURE;
ERRFEnd
ERRFEpilog
	return ExitValue;
}
