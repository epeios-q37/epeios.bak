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

#ifndef WRPRECORD__INC
# define WRPRECORD__INC

# include "ogzrcd.h"

# include "fblbkd.h"

# include "common.h"

# define WRPRECORD_RECORD_PREFIX	OGZINF_LC_SHORT
# define WRPRECORD_RECORD_NAME		"Record"

namespace wrprecord {
	typedef tol::dObject<ogzrcd::rRecordBuffer> dRecord_;

	class dRecord
	: public dRecord_
	{
	public:
		struct s
		: public dRecord_::s
		{};
		dRecord( s &S )
		: dRecord_( S )
		{}
		void HANDLE(
			fblbkd::backend___ &Backend,
			fblbkd::untyped_module &Module,
			fblbkd::index__ Index,
			fblbkd::command__ Command,
			fblbkd::rRequest &Request,
			void *UP );
		static void NOTIFY(
			fblbkd::untyped_module &Module,
			common::rStuff &Stuff );
		static const char *PREFIX;
		static const char *NAME;
//		static void RAW_MESSAGES( lcl::strings_ &Messages );
		dRecord_ *OBJECT( void )
		{
			return this;
		}
	};

	qW( Record );
}


#endif
