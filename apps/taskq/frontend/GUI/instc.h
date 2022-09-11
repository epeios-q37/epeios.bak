/*
	Copyright (C) 2021 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'TaskQ' software.

    'TaskQ' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'TaskQ' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'TaskQ'.  If not, see <http://www.gnu.org/licenses/>.
*/

// INSTanCes

#ifndef INSTC_INC_
# define INSTC_INC_

# include "frdfrntnd.h"

namespace instc {
	using namespace frdfrntnd;

	using fbltyp::dString;
	using fbltyp::dStrings;

	class rUser
	{
	private:
		rMyObject MyObject_;
	public:
		void reset( bso::bool__ P = true )
		{
			MyObject_.reset( P );
		}
		E_CVDTOR( rUser );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			if ( Frontend.IsConnected() )
				MyObject_.Init( Frontend );
		}
		dString &ToUpper( dString &String )
		{
			return MyObject_.ToUpper( String );
		}
		void TestMessage( void )
		{
			MyObject_.TestMessage();
		}
	};

	// There can also be a instance for the administration, for example.


	class rInstances
	{
	public:
		instc::rUser User;
		void reset( bso::bool__ P = true )
		{
			User.reset( P );
		}
		E_CVDTOR( rInstances );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
		    User.Init(Frontend);
		}
	};
}

#endif
