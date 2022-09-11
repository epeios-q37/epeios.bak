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

#ifndef BASE_INC_
# define BASE_INC_

# include "taskqxdh.h"

# include "sclx.h"

# include "tskfbc.h"

# include "frdfrntnd.h"

# include "xdhdws.h"

/***** Macros dealing with actions ****/
// Declaration.
# define BASE_AD( name ) SCLX_ADec( core::rSession, name )

// Registering.
# define BASE_AR( name )\
	base::Register( s##name::Name, name );

// Definition.
# define BASE_ADef( owner, name ) SCLX_ADef( core::rSession, owner, name )

/**********/

namespace base {
	qENUM( Page )
	{
		pProlog,
		pLogin,
		pMain,
		p_amount,
		p_Undefined
	};
}

namespace instc {
    class rInstances;
}

namespace core {
	using instc::rInstances;
	class sDump;
	typedef sclx::rSession<rInstances, frdfrntnd::rFrontend, base::ePage, base::p_Undefined, sDump> rSession;
}

namespace base {
	extern const char *Name;

	typedef sclx::cAction<core::rSession> cAction;

	void Register(
		const char *Name,
		cAction &Callback );
}

#endif
