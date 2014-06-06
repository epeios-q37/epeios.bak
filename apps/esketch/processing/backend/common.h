/*
	'common.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COMMON__INC
# define COMMON__INC

# include "sktbsc.h"
# include "sktinf.h"

# include "fblbkd.h"

#include "sclbacknd.h"

namespace common {
	typedef sclbacknd::backend___ _backend___;

	class stuff___	// 'xbackend' ('eXtended' 'backend') : 'backend' qui hérite du 'backend' de base.
	{
	private:
		_backend___ *XBackend_;
	public:
		void reset( bso::bool__ = true )
		{
			XBackend_ = NULL;
		}
		E_CVDTOR( stuff___ );
		void Init( _backend___ &XBackend )
		{
			XBackend_ = &XBackend;
		}
		_backend___ &XBackend( void ) const
		{
			return *XBackend_;
		}
	};
}

#endif
