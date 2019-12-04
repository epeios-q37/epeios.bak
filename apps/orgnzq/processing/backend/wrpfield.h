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

#ifndef WRPFIELD_INC_
# define WRPFIELD_INC_

# include "common.h"

# include "ogzfld.h"

# include "fblbkd.h"

# define WRPFIELD_FIELD_PREFIX	OGZINF_LC_SHORT
# define WRPFIELD_FIELD_NAME		"Field"

namespace wrpfield {
	typedef ogzbsc::dData dData_;

	class dField_
	: public dData_
	{
	public:
		struct s
		: public dData_::s
		{
			ogztyp::sRow Type;
			ogzclm::eNumber Number;
		} &S_;
		dField_( s &S )
		: S_( S ),
		  dData_( S )
		{}
		void reset( bso::sBool P = true )
		{
			dData_::reset( P );
			S_.Type = qNIL;
			S_.Number = ogzclm::n_Undefined;
		}
		void plug( ogzetr::sHooks &Hook )
		{
			dData_::plug( Hook );
		}
		void plug( qASd *AS )
		{
			dData_::plug( AS );
		}
		dField_ &operator =(const dField_ &F)
		{
			dData_::operator =( F );

			S_.Type = F.S_.Type;
			S_.Number = F.S_.Number;

			return *this;
		}
		void Init( void )
		{
			dData_::Init();

			S_.Type = qNIL;
			S_.Number = ogzclm::n_Undefined;
		}
		void Clear( void )
		{
			dData_::Init();
		}
		qRWDISCLOSEd( ogztyp::sRow, Type );
		qRWDISCLOSEd( ogzclm::eNumber, Number );
	};

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
