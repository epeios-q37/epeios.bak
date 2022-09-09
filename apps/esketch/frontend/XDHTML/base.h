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

#ifndef BASE_INC_
# define BASE_INC_

# include "esketchxdh.h"

# include "scls.h"

# include "sktfbc.h"

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
	typedef scls::rSession<rInstances, frdfrntnd::rFrontend, base::ePage, base::p_Undefined, sDump> rSession;
}

namespace base {
	extern const char *Name;

	typedef sclx::cAction<core::rSession> cAction;

	void Register(
		const char *Name,
		cAction &Callback );
}

#endif
