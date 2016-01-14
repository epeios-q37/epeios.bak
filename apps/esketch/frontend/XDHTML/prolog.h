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

	BASE_ACD( switch_project_type );
	BASE_ACD( display_project_filename );
	BASE_ACD( load_project );

	class action_callbacks__
	{
	public:
		BASE_ACU( switch_project_type, SwitchProjectType );
		BASE_ACU( display_project_filename, DisplayProjectFilename );
		BASE_ACU( load_project, LoadProject );
		void reset( bso::bool__ P = true )
		{
			BASE_ACR( SwitchProjectType );
			BASE_ACR( DisplayProjectFilename );
			BASE_ACR( LoadProject );
		}
		E_CVDTOR( action_callbacks__ );
		void Init( void )
		{
			BASE_ACI( SwitchProjectType );
			BASE_ACI( DisplayProjectFilename );
			BASE_ACI( LoadProject );
		}
	};

	void SetLayout( core::session___ &Session);
}

#endif