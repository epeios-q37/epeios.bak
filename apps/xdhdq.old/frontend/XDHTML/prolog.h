/*
	'prolog.h' by Claude SIMON (http://q37.info/contact/).

	 This file is part of 'xdhdq' software.

    'xdhdq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhdq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhdq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PROLOG_INC_
# define PROLOG_INC_

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