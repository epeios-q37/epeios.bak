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

#ifndef MAIN__INC
# define MAIN__INC

# include "base.h"

namespace main {
	BASE_ACD( Submission );
	BASE_ACD( ShowTestButton );
	BASE_ACD( HideTestButton );
	BASE_ACD( Testing );

	class fActionCallbacks
	{
	public:
		BASE_ACU( Submission );
		BASE_ACU( ShowTestButton );
		BASE_ACU( HideTestButton );
		BASE_ACU( Testing );
	public:
		void reset( bso::bool__ P = true )
		{
			BASE_ACR( Submission );
			BASE_ACR( ShowTestButton );
			BASE_ACR( HideTestButton );
			BASE_ACR( Testing );
		}
		qCVDTOR( fActionCallbacks );
		void Init( void )
		{
			BASE_ACI( Submission );
			BASE_ACI( ShowTestButton );
			BASE_ACI( HideTestButton );
			BASE_ACI( Testing );
		}
	};

	void SetLayout( core::session___ &Session );
}

#endif