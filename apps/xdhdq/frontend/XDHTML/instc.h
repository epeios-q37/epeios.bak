/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHDq' software.

    'XDHDq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHDq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHDq'.  If not, see <http://www.gnu.org/licenses/>.
*/

// INSTanCes

#ifndef INSTC_INC_
# define INSTC_INC_

# include "frdfrntnd.h"

namespace instc {
	using namespace frdfrntnd;

	using fbltyp::dString;
	using fbltyp::dStrings;

	qENUM( Field )
	{
		fCKEditor,
		fJQTE,
		fDatepicker,
		fTimepicker,
		f_amount,
		f_Undefined,
	};

	const char *GetLabel( eField );

	eField GetField( const str::string_ &Pattern );

	class rUser
	{
	private:
		rMyObject MyObject_;
		bso::bool__ _TestButtonIsVisible;
		eField Field_;
		str::wString Content_[f_amount];
		void Test_( eField Field ) const
		{
			if ( Field >= f_amount )
				qRGnr();
		}
	public:
		void reset( bso::bool__ P = true )
		{	
			MyObject_.reset( P );
			Field_ = f_Undefined;

			int i = f_amount;

			while ( i-- )
				Content_[i].reset( P );

			_TestButtonIsVisible = false;
		}
		E_CVDTOR( rUser );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			if ( Frontend.IsConnected() )
				MyObject_.Init( Frontend );

			int i = f_amount;

			while ( i-- )
				Content_[i].Init();

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
		eField GetField( void ) const
		{
			return Field_;
		}
		void SetField( eField Field )
		{
			Test_( Field );

			Field_ = Field;
		}
		void ResetField( void )
		{
			Field_ = f_Undefined;
		}
		const str::dString &GetContent( eField Field ) const
		{
			Test_( Field );

			return Content_[Field];
		}
		void SetContent(
			eField Field,
			const str::dString &Content )
		{
			Test_( Field );

			Content_[Field] = Content;
		}
		E_RWDISCLOSE__( bso::bool__, TestButtonIsVisible );
	};
}

#endif
