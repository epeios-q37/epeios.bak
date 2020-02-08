/*
	Copyright (C) 2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

    'XDHq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef XDHQXDH_INC_
# define XDHQXDH_INC_

# include "xdhdws.h"
# include "scli.h"

# define PROGRAM_NAME		"xdhq"	XDHDWS_DEFAULT_SUFFIX
# define SOFTWARE_NAME		"XDHq"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			AUTHOR_NAME
# define OWNER_CONTACT		AUTHOR_CONTACT
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"
# define SOFTWARE_VERSION	"0.9"
# define SOFTWARE_DETAILS	SKTINF_SOFTWARE_NAME " V" SKTINF_SOFTWARE_VERSION
# define SOFTWARE_URL		"http://q37.info/"

SCLI_DEC( xdhqxdh );

#endif
