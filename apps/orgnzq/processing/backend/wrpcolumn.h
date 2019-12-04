/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#ifndef WRPCOLUMN_INC_
# define WRPCOLUMN_INC_

# include "ogzclm.h"

# include "fblbkd.h"

# include "common.h"

# define WRPCOLUMN_COLUMN_PREFIX	OGZINF_LC_SHORT
# define WRPCOLUMN_COLUMN_NAME		"Column"

namespace wrpcolumn {
	typedef tol::dObject<ogzclm::rColumnBuffer> dColumn_;

	class dColumn
	: public dColumn_
	{
	public:
		struct s
		: public dColumn_::s
		{};
		dColumn( s &S )
		: dColumn_( S )
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
		dColumn_ *OBJECT( void )
		{
			return this;
		}
	};

	qW( Column );
}


#endif
