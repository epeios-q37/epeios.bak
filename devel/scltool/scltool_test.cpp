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

#include "scltool.h"
#include "sclrgstry.h"

#include "err.h"
#include "cio.h"

using cio::CIn;
using cio::COut;
using cio::CErr;

const char *sclmisc::SCLMISCTargetName = "TEST";
const char *sclmisc::SCLMISCProductName = "TEST";
const char *sclmisc::SCLMISCOrganizationName = "TEST";
 

int scltool::SCLTOOLMain(
	const str::string_ &Command,
	const scltool::oddities__ &Oddities )
{
qRH
	rgstry::row__ Row = qNIL;
	rgstry::level__ Level = qNIL;
qRB
	sclmisc::SetLastingValue( str::wString( "test" ), str::wString( "TEST" ) );
	sclmisc::SetLastingValue( str::wString( "essai" ), str::wString( "ESSAI" ) );

	cio::COut << txf::tab << "----- Configuration registry -----" << txf::nl;
	sclmisc::GetRegistry().Dump( sclmisc::GetRegistryRawLevel( sclrgstry::lMain ), qNIL, true, xml::oIndent, xml::e_Default, cio::COut );
	cio::COut << txf::tab << "----- Lasting registry -----" << txf::nl;
	sclmisc::GetRegistry().Dump( sclmisc::GetRegistryRawLevel( sclrgstry::lLasting ), qNIL, true, xml::oIndent, xml::e_Default, cio::COut );
	cio::COut << txf::tab << "----- Project registry -----" << txf::nl;
	sclmisc::GetRegistry().Dump( sclmisc::GetRegistryRawLevel( sclrgstry::lProject ), qNIL, true, xml::oIndent, xml::e_Default, cio::COut );
	cio::COut << txf::tab << "----- Setup registry -----" << txf::nl;
	sclmisc::GetRegistry().Dump( sclmisc::GetRegistryRawLevel( sclrgstry::lSetup ), qNIL, true, xml::oIndent, xml::e_Default, cio::COut );
	cio::COut << txf::tab << "----- Arguments registry -----" << txf::nl;
	sclmisc::GetRegistry().Dump( sclmisc::GetRegistryRawLevel( sclrgstry::lArguments ), qNIL, true, xml::oIndent, xml::e_Default, cio::COut );

	cio::COut << txf::nl << txf::commit;
qRR
qRT
qRE
	return EXIT_SUCCESS;
}

#if 0	// Puyisque l'objet de cette bibliothque est jutement de prendre en charge le 'main'.
int main( int argc, char *argv[] )
{
	int ExitValue = EXIT_SUCCESS;
qRFH
qRFB
	COut << "Test of library " << SCLTOOL_NAME << ' ' << __DATE__" "__TIME__"\n";
qRFR
	ExitValue = EXIT_FAILURE;
qRFT
qRFE
	return ExitValue;
}
#endif

class starter {
public:
	starter( void )
	{
		scltool::IgnoreUnknownArguments = true;

	}
} Starter;