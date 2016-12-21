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

// INSTanCes

#ifndef INSTC__INC
# define INSTC__INC

# include "frdfrntnd.h"

namespace instc {

	using namespace frdfrntnd;

	using fbltyp::dString;
	using fbltyp::dStrings;

	class rUser
	{
	private:
		rMyObject MyObject_;
		bso::bool__ _TestButtonIsVisible;
	public:
		void reset( bso::bool__ P = true )
		{	
			MyObject_.reset( P );
			_TestButtonIsVisible = false;
		}
		E_CVDTOR( rUser );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			if ( Frontend.IsConnected() )
				MyObject_.Init( Frontend );

			_TestButtonIsVisible = false;
		}
		dString &ToUpper( dString &String )
		{
			return MyObject_.ToUpper( String );
		}
		void TestMessage( void )
		{
			MyObject_.TestMessage();
		}
		E_RWDISCLOSE__( bso::bool__, TestButtonIsVisible );
	};
}

#endif
