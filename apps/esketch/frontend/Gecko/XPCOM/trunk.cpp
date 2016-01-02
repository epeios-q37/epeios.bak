/*
	'trunk.cpp' by Claude SIMON (http://q37.info/contact/).

	This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id: trunk.cpp,v 1.5 2013/06/18 09:36:02 csimon Exp $

#include "trunk.h"

using namespace trunk;

void trunk::user_callback__::XULFTKAboutText(
	const char *LauncherIdentification,
	const char *BuildInformations,
	str::string_ &Text )
{
	xulftk::BuildAboutText(
		LauncherIdentification, BuildInformations, 
		PROGRAM_NAME, VERSION,
		AUTHOR_NAME, AUTHOR_CONTACT,
		COPYRIGHT,
		SOFTWARE_DETAILS, SOFTWARE_URL,
		_T(), Text );
}

void trunk::user_callback__::XULFTKDefineSession( const fnm::name___ &ProjectFileName )
{
qRH
	str::string ProjectId;
	bso::integer_buffer__ Buffer;
qRH
	ProjectId.Init();

	if ( ProjectFileName.Size() != 0 )
		sclmisc::LoadProject( ProjectFileName, ProjectId );
	else {
		sclmisc::EraseProjectRegistry();
		ProjectId.Append( bso::Convert( tol::EpochTime( true ), Buffer ) );
	}

	_DefineSession( ProjectId );
	_T().FinalizeDefineSession();
qRR
qRT
qRE
}
