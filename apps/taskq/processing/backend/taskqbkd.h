/*
	Copyright (C) 2021 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#ifndef TASKQBKD_INC_
# define TASKQBKD_INC_

# include "tskinf.h"

# include "scli.h"

# define BACKEND_NAME	TSKINF_LC	"bkd"
# define COPYRIGHT		COPYRIGHT_YEARS " " TSKINF_OWNER_NAME " (" TSKINF_OWNER_CONTACT ")"
# define API_VERSION	"1"

SCLI_DEC( taskqbkd );

#endif
