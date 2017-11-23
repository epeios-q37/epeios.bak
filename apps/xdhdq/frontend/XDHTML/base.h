/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHDq' software.

    'XDHDq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHDq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHDq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BASE__INC
# define BASE__INC

# include "xdhdqxdh.h"

# include "sclxdhtml.h"

# include "xddfbc.h"

# include "frdfrntnd.h"

# include "xdhdws.h"

/***** Macros dealing with actions ****/
// Declaration.
# define BASE_ACD( name )\
	extern class s##name\
	: public base::cAction\
	{\
	protected:\
		virtual void SCLXLaunch(\
			core::rSession &Session,\
			const char *Id ) override;\
	public:\
		static const char *Name;\
	} name

// Registering.
# define BASE_ACR( name )\
	base::Register( s##name::Name, name );

// Definition.
# define BASE_AC( owner, name )\
	owner::s##name owner::name;\
	const char *owner::s##name::Name = #name;\
	void owner::s##name::SCLXLaunch(\
		core::rSession &Session,\
		const char *Id )
/**********/

// Predeclaration.

namespace core {
	class rSession;
}

namespace base {
	extern const char *Name;

	typedef sclxdhtml::cAction<core::rSession> cAction;

	void Register(
		const char *Name,
		cAction &Callback );

	class sActionHelper
	: public sclxdhtml::cActionHelper<core::rSession>
	{
	protected:
		virtual bso::bool__ SCLXOnBeforeAction(
			core::rSession &Session,
			const char *Id,
			const char *Action ) override;
		virtual void SCLXOnRefresh( core::rSession &Session ) override;
		virtual bso::bool__ SCLXOnClose( core::rSession &Session ) override;
	};
}

#endif