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

// OrGaniZer MeTa Callback

#ifndef OGZMTC_INC_
# define OGZMTC_INC_

# ifdef XXX_DBG
#	define OGZMTC__DBG
# endif

# include "ogzbsc.h"
# include "ogzfld.h"
# include "ogzrcd.h"
# include "ogzdta.h"
# include "ogzusr.h"

namespace ogzmtc {
	class callback__
	{
	protected:
		virtual ogzdta::callback__ &OGZMTCGetDTA( void ) = 0;
		virtual ogzfld::callback__ &OGZMTCGetFLD( void ) = 0;
		virtual ogzrcd::callback__ &OGZMTCGetRCD( void ) = 0;
		virtual ogzusr::callback__ &OGZMTCGetUSR( void ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardization.
		}
		E_CVDTOR( callback__ );
		void Init( void )
		{
			// Standardization.
		}
		ogzdta::callback__ &GetDTA( void )
		{
			return OGZMTCGetDTA();
		}
		ogzfld::callback__ &GetFLD( void )
		{
			return OGZMTCGetFLD();
		}
		ogzrcd::callback__ &GetRCD( void )
		{
			return OGZMTCGetRCD();
		}
		ogzusr::callback__ &GetUSR( void )
		{
			return OGZMTCGetUSR();
		}
	};
}


#endif
