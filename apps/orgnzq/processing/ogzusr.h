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

// OrGaniZer USeR

#ifndef OGZUSR__INC
# define OGZUSR__INC

# ifdef XXX_DBG
#	define OGZUSR__DBG
# endif

# include "ogzbsc.h"
# include "ogzcbs.h"
# include "ogzrcd.h"

# include "bch.h"

namespace ogzusr {
	typedef ogzbsc::sURow sRow;
	typedef ogzbsc::sURRow sRRow;

	typedef bch::qBUNCHd( ogzrcd::sRow, sRRow ) dUser;
	qW( User );

// Template parameters.
# define OGZUSR_TP	ogzusr::dUser, ogzusr::wUser, ogzusr::sRow, ogzrcd::sRow, ogzusr::sRRow

	typedef ogzcbs::cDynamic<OGZUSR_TP> cUser;

	typedef ogzcbs::sDynamicItems<OGZUSR_TP> sUsers;

	typedef ogzcbs::rRegularDynamicCallback<OGZUSR_TP> rRegularUserCallback;

	class cAuthentication
	{
	protected:
		virtual ogzusr::sRow OGZUSRAuthenticate(
			const str::vString &Username,
			const str::vString &Password ) = 0;
	public:
		ogzusr::sRow Authenticate(
			const str::vString &Username,
			const str::vString &Password )
		{
			return OGZUSRAuthenticate( Username, Password );
		}
	};

}


#endif
