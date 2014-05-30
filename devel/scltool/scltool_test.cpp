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

const char *sclmisc::SCLMISCTargetName = "TEST";

void scltool::SCLTOOLMain( const str::string_ &Command )
{
ERRProlog
	rgstry::row__ Row = E_NIL;
	rgstry::level__ Level = E_NIL;
ERRBegin
	cio::COut << txf::tab << "----- Configuration registry -----" << txf::nl;
	sclrgstry::GetRegistry().Dump( sclrgstry::GetConfigurationRegistryLevel(), E_NIL, true, xml::oIndent, xml::e_Default, cio::COut );
	cio::COut << txf::tab << "----- Project registry -----" << txf::nl;
	sclrgstry::GetRegistry().Dump( sclrgstry::GetProjectRegistryLevel(), E_NIL, true, xml::oIndent, xml::e_Default, cio::COut );
	cio::COut << txf::tab << "----- Setup registry -----" << txf::nl;
	sclrgstry::GetRegistry().Dump( scltool::GetSetupRegistryLevel(), E_NIL, true, xml::oIndent, xml::e_Default, cio::COut );
	cio::COut << txf::tab << "----- Arguments registry -----" << txf::nl;
	sclrgstry::GetRegistry().Dump( scltool::GetArgumentsRegistryLevel(), E_NIL, true, xml::oIndent, xml::e_Default, cio::COut );

	cio::COut << txf::nl;
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
