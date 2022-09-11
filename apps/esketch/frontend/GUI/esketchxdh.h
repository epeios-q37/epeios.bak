/*
	Copyright (C) 2018 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#ifndef ESKETCHXDH_INC_
# define ESKETCHXDH_INC_

# include "sktinf.h"

# include "scli.h"

# define NAME				SKTINF_LC	SCLX_DEFAULT_SUFFIX
# define AUTHOR_NAME		SKTINF_AUTHOR_NAME
# define AUTHOR_CONTACT		SKTINF_AUTHOR_CONTACT
# define OWNER_NAME			SKTINF_OWNER_NAME
# define OWNER_CONTACT		SKTINF_OWNER_CONTACT
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"
# define SOFTWARE_NAME		SKTINF_SOFTWARE_NAME
# define SOFTWARE_VERSION	SKTINF_SOFTWARE_VERSION
# define SOFTWARE_DETAILS	SKTINF_SOFTWARE_NAME " V" SKTINF_SOFTWARE_VERSION
# define SOFTWARE_URL		SKTINF_SOFTWARE_URL

SCLI_DEC( esketchxdh );

#endif
