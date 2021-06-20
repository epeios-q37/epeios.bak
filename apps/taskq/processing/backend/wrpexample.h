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

#ifndef WRPEXAMPLE_INC_
# define WRPEXAMPLE_INC_

# include "tsktasks.h"

# include "fblbkd.h"

# include "common.h"

# define WRPEXAMPLE_MYOBJECT_PREFIX	TSKINF_LC_SHORT
# define WRPEXAMPLE_MYOBJECT_NAME	"MyObject"

namespace wrpexample {
	/*
	typedef tskxmp::dMyObject	dMyObject_;

	class dMyObject
	: public dMyObject_
	{
	public:
		struct s
		: public dMyObject_::s
		{};
		dMyObject( s &S )
		: dMyObject_( S )
		{}
		void HANDLE(
			fblbkd::backend___ &Backend,
			fblbkd::rModule &Module,
			fblbkd::command__ Command,
			fblbkd::rRequest &Request );
		static void NOTIFY(	fblbkd::rModule &Module );
		static const char *PREFIX;
		static const char *NAME;
//		static void RAW_MESSAGES( lcl::strings_ &Messages );
		dMyObject_ *OBJECT( void )
		{
			return this;
		}
	};

	qW( MyObject );
	*/
}


#endif
