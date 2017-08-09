/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'NJSq.

    'NJSq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'NJSq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'NJSq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef REGISTRAR__INC
# define REGISTRAR__INC

# include "common.h"

namespace registrar {
	typedef n4a::cRegistrar cRegistrar_;

	class sRegistrar
	: public cRegistrar_ {
	protected:
		virtual void N4ARegister( void *Function ) override;
	public:
		void Init( void )
		{}
	};
}

#endif
