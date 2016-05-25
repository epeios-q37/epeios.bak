/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef WRPFIELD__INC
# define WRPFIELD__INC

# include "common.h"

# include "ogzfld.h"

# include "fblbkd.h"

# define WRPFIELD_FIELD_PREFIX	OGZINF_LC_SHORT
# define WRPFIELD_FIELD_NAME		"Field"

namespace wrpfield {
	typedef ogzfld::dFieldBuffer dField_;

	class dField
	: public dField_
	{
	public:
		struct s
		: public dField_::s
		{};
		dField( s &S )
		: dField_( S )
		{}
		void HANDLE(
			fblbkd::backend___ &Backend,
			fblbkd::rModule &Module,
			fblbkd::command__ Command,
			fblbkd::rRequest &Request );
		static void NOTIFY( fblbkd::rModule &Module );
		static const char *PREFIX;
		static const char *NAME;
//		static void RAW_MESSAGES( lcl::strings_ &Messages );
		dField_ *OBJECT( void )
		{
			return this;
		}
	};

	qW( Field );
}


#endif
