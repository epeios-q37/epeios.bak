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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sclplugin.h"

#include "err.h"
#include "cio.h"

using cio::CIn;
using cio::COut;
using cio::CErr;

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
	int ExitValue = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	COut << "Test of library " << SCLPLUGIN_NAME << ' ' << __DATE__" "__TIME__"\n";

	Generic( argc, argv );
ERRFErr
	ExitValue = EXIT_FAILURE;
ERRFEnd
ERRFEpilog
	return ExitValue;
}
