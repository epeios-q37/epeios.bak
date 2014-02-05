/*
	'esketch' by Claude SIMON (simon.claude@zeusw.org)

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'esketch' tool.

    'esketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'esketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'esketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "registry.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

# define BEGIN
// Ne pas enlever ce qu'il y a entre le 'BEGIN' ci-dessus et le 'END' ci-dessous !
/* PLACEHOLDER */

# define NAME_MC			"eSketch"
# define NAME_UC			"ESKETCH"
# define NAME_LC			"esketch"
# define WEBSITE_URL		"http://zeusw.org/intl/"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT	"http://zeusw.org/intl/contact.html"
# define OWNER_NAME		"Claude SIMON"
# define OWNER_CONTACT	"http://zeusw.org/intl/contact.html"
# define COPYRIGHT		COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	

# define END

/* Beginning of the part which handles command line arguments. */

static void PrintHeader_( void )
{
	COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
	COut << "Copyright " COPYRIGHT << txf::nl;
	COut << txf::pad << "Build : "__DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
}

/* End of the part which handles command line arguments. */

void scltool::Main( const str::string_ &Command )
{
ERRProlog
ERRBegin
	if ( Command == "Test" )
		cio::COut << "Test" << txf::nl;
	else if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense();
	else
		ERRFwk();
ERRErr
ERREnd
ERREpilog
}

const char *scltool::TargetName = NAME_LC;

