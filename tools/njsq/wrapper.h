/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#ifndef WRAPPER_INC_
# define WRAPPER_INC_

# include "n4allw.h"
# include "n4njs.h"

// Note to developer : include 'h:\hg\NJSIncludeDirectories.props' in the '.vcxproj'.
# include "nodeq.h"

namespace wrapper {
	typedef n4allw::rLauncher rLauncher_;

	class rLauncher
	: public rLauncher_
	{
	public:
		bso::sBool Register(
			const fnm::rName &ComponentFilename,
			sclmisc::sRack &Rack,
			n4njs::gShared &Shared,
			qRPD )
		{
			return rLauncher_::Init( ComponentFilename, dlbrry::nExtOnly, Rack, &Shared, qRP );
		}
		void Call( const v8::FunctionCallbackInfo<v8::Value>& Info );
	};
}

#endif
