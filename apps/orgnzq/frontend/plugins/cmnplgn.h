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

#ifndef CMNPLGN_INC_
# define CMNPLGN_INC_

# include "frdplgn.h"

namespace cmnplgn {
	typedef frdplgn::sXML sPlugin_;

	class sPlugin
	: public sPlugin_
	{
	protected:
		virtual void FRDCLLBCKGetBackendPluginLabel( str::dString &Id ) override;
	};
}

#endif
