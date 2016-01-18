/*
	'wrpexample.h' by Claude SIMON (http://q37.info/contact/).

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

#ifndef WRPEXAMPLE__INC
# define WRPEXAMPLE__INC

# include "ogzxmp.h"

# include "fblbkd.h"

# include "common.h"

# define WRPEXAMPLE_MYOBJECT_PREFIX	OGZINF_LC_SHORT
# define WRPEXAMPLE_MYOBJECT_NAME	"myobject"

namespace wrpexample {
	typedef ogzxmp::myobject_	_myobject_;

	class myobject_
	: public _myobject_
	{
	public:
		struct s
		: public _myobject_::s
		{};
		myobject_( s &S )
		: _myobject_( S )
		{}
		void HANDLE(
			fblbkd::backend___ &Backend,
			fblbkd::untyped_module &Module,
			fblbkd::index__ Index,
			fblbkd::command__ Command,
			fblbkd::request__ &Request,
			void *UP );
		static void NOTIFY(
			fblbkd::untyped_module &Module,
			common::stuff___ &Stuff );
		static const char *PREFIX;
		static const char *NAME;
//		static void RAW_MESSAGES( lcl::strings_ &Messages );
		_myobject_ *OBJECT( void )
		{
			return this;
		}
	};

	E_AUTO(	myobject );
}


#endif
