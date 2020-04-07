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

#define EPSMSC_COMPILATION_

#include "epsmsc.h"

using namespace epsmsc;
using namespace txf;

void epsmsc::PrintLicense(
	const char *Name,
	txf::text_oflow__ &Flow )
{
	Flow
	<< pad << Name << " is free software: you can redistribute it and/or modify" << nl
	<< pad << "it under the terms of the GNU Affero General Public License as" << nl
	<< pad << "published by the Free Software Foundation, either version 3 of" << nl
	<< pad << "the License, or (at your option) any later version." << nl
	<< nl
	<< pad << Name << " is distributed in the hope that it will be useful, but" << nl
	<< pad << "WITHOUT ANY WARRANTY; without even the implied warranty of" << nl
	<< pad << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See" << nl
	<< pad << "the GNU Affero General Public License for more details." << nl
	<< pad << nl
	<< pad << "You should have received a copy of the GNU Affero" << nl
	<< pad << "General Public License along with " << Name << ". If not," << nl
	<< pad << "see <http://www.gnu.org/licenses/>" << nl;
}
