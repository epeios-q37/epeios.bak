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

#ifndef PROLOG__INC
# define PROLOG__INC

# include "base.h"

namespace prolog {
	BASE_ACD( SwitchProjectType );
	BASE_ACD( DisplayProjectFilename );
	BASE_ACD( LoadProject );

	class fActionCallbacks
	{
	public:
		BASE_ACU( SwitchProjectType );
		BASE_ACU( DisplayProjectFilename );
		BASE_ACU( LoadProject );
		void reset( bso::bool__ P = true )
		{
			BASE_ACR( SwitchProjectType );
			BASE_ACR( DisplayProjectFilename );
			BASE_ACR( LoadProject );
		}
		qCVDTOR( fActionCallbacks );
		void Init( void )
		{
			BASE_ACI( SwitchProjectType );
			BASE_ACI( DisplayProjectFilename );
			BASE_ACI( LoadProject );
		}
	};

	void SetLayout( core::fSession &Session);
}

#endif