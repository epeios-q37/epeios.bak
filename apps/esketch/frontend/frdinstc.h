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

// FRontenD INSTanCes

#ifndef FRDINSTC__INC
# define FRDINSTC__INC

# include "frdfrntnd.h"

namespace frdinstc {

	using namespace frdfrntnd;

	using fbltyp::dString;
	using fbltyp::dStrings;

	class rUser_
	{
	private:
		qRMV( frdfrntnd::rFrontend, F_,  Frontend_ );
		esketch::fStatics S_( void )
		{
			return F_().Statics;
		}
		esketch::rSKTMyObject Object_;
	public:
		void reset( bso::bool__ P = true )
		{	
			Frontend_ = NULL;

			Object_.reset( P );
		}
		E_CVDTOR( rUser_ );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			Frontend_ = &Frontend;

			Object_.Init( Frontend.MyObject );
		}
		void Ping( void )
		{
			F_().Ping();
		}
		void Crash( void )
		{
			F_().Crash();
		}
		void LoadSetupOfId( const dString &Id )
		{
			S_().LoadSetupOfId_1( Id );
		}
		void LoadSetupContent( const dString &Content )
		{
			S_().LoadSetupContent_1( Content );
		}
		dString &ToUpper( dString &String );
		void TestMessage( void )
		{
			Object_.Test();
		}
	};

	class rUser
	{
	private:
		rUser_ Core_;
		bso::bool__ _TestButtonIsVisible;
	public:
		void reset( bso::bool__ P = true )
		{	
			Core_.reset( P );
			_TestButtonIsVisible = false;
		}
		E_CVDTOR( rUser );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			if ( Frontend.IsConnected() )
				Core_.Init( Frontend );

			_TestButtonIsVisible = false;
		}
		dString &ToUpper( dString &String )
		{
			return Core_.ToUpper( String );
		}
		void TestMessage( void )
		{
			Core_.TestMessage();
		}
		E_RWDISCLOSE__( bso::bool__, TestButtonIsVisible );
	};
}

#endif
