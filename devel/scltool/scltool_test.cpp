/*
	'scltool_test.cpp' by Claude SIMON (http://zeusw.org/).

	'scltool_test.cpp' is part of the Epeios framework.

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

#include "scltool.h"
#include "sclrgstry.h"

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

const char *scltool::TargetName = "TEST";

void scltool::Main(
	int argc,
	const char *argv[] )
{
ERRProlog
	str::string Argument;
ERRBegin
	scltool::GetRegistry().Dump( scltool::GetRegistryConfigurationLevel(), true, xml::oIndent, xml::e_Default, cio::COut );
	scltool::GetRegistry().Dump( scltool::GetRegistryProjectLevel(), true, xml::oIndent, xml::e_Default, cio::COut );
	scltool::GetRegistry().Dump( scltool::GetRegistrySetupLevel(), true, xml::oIndent, xml::e_Default, cio::COut );

	cio::COut << txf::nl;

	scltool::PrintUsage();
ERRErr
ERREnd
ERREpilog
}

#if 0	// Puyisque l'objet de cette bibliothèque est jutement de prendre en charge le 'main'.
int main( int argc, char *argv[] )
{
	int ExitValue = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	COut << "Test of library " << SCLTOOL_NAME << ' ' << __DATE__" "__TIME__"\n";
ERRFErr
	ExitValue = EXIT_FAILURE;
ERRFEnd
ERRFEpilog
	return ExitValue;
}
#endif
