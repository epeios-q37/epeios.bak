/*
	Copyright (C) 2018 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'TaskQ' software.

    'TaskQ' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'TaskQ' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'TaskQ'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TASKQXDH_INC_
# define TASKQXDH_INC_

# include "tskinf.h"

# include "scli.h"

# define NAME				TSKINF_LC	SCLX_DEFAULT_SUFFIX
# define AUTHOR_NAME		TSKINF_AUTHOR_NAME
# define AUTHOR_CONTACT		TSKINF_AUTHOR_CONTACT
# define OWNER_NAME			TSKINF_OWNER_NAME
# define OWNER_CONTACT		TSKINF_OWNER_CONTACT
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"
# define SOFTWARE_NAME		TSKINF_SOFTWARE_NAME
# define SOFTWARE_VERSION	TSKINF_SOFTWARE_VERSION
# define SOFTWARE_DETAILS	TSKINF_SOFTWARE_NAME " V" TSKINF_SOFTWARE_VERSION
# define SOFTWARE_URL		TSKINF_SOFTWARE_URL

SCLI_DEC( taskqxdh );

#endif
