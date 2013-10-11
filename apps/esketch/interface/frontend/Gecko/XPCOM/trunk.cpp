/*
	'trunk.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
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
		AFFILIATED_SOFTWARE_NAME, AFFILIATED_SOFTWARE_URL,
		_T(), Text );
}
