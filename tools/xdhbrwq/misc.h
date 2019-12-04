/*
	Header for the 'misc' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of the 'xdhbrwq' tool.
	Copyright (C) 2015 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'xdhbrwq'.

    'xdhbrwq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhbrwq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhbrwq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MISC_INC_
# define MISC_INC_

# include "ntvstr.h"

# include "xdhups.h"

# define MISC_NAME_MC			"xdhbrwq"	// X(SL)/DH(TML) in (web) BRoWser
# define MISC_NAME_LC			"xdhbrwq"
# define MISC_NAME_UC			"WDHBRWQ"
# define MISC_WEBSITE_URL		"http://q37.info/"
# define MISC_AUTHOR_NAME		"Claude SIMON"
# define MISC_AUTHOR_CONTACT	"http://q37.info/contact.html"
# define MISC_OWNER_NAME		"Claude SIMON"
# define MISC_OWNER_CONTACT		"http://q37.info/contact.html"
# define MISC_COPYRIGHT			COPYRIGHT_YEARS " " MISC_OWNER_NAME " (" MISC_OWNER_CONTACT ")"	
# define MISC_VERSION			VERSION

namespace misc {
	typedef ntvstr::char__ nchar__;
	typedef ntvstr::string___ nstring___;

	typedef xdhups::sSession sSession;

	void Report(
		const nstring___ &Message,
		str::string_ &Script );
}

#endif
