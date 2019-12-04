/*
	'frdrinstc.h' by Claude SIMON (http://q37.info/contact/).

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

// FRontenD INSTanCes

#ifndef FRDINSTC_INC_
# define FRDINSTC_INC_

# include "frdfrntnd.h"

namespace frdinstc {

	using namespace frdfrntnd;

	class _user___
	{
	private:
		Q37_MRMDF( frdfrntnd::frontend___, F_,  Frontend_ );
		xdhdq::xdd_myobject___ _Object;
	public:
		void reset( bso::bool__ P = true )
		{	
			Frontend_ = NULL;

			_Object.reset( P );
		}
		E_CVDTOR( _user___ );
		void Init( frdfrntnd::frontend___ &Frontend )
		{
			Frontend_ = &Frontend;

			_Object.Init( Frontend.MyObject );
		}
	};

	E_ENUM( field ) {
		fCKEditor,
		fJQTE,
		fDatepicker,
		fTimepicker,
		f_amount,
		f_Undefined,
	};

	const char *GetLabel( field__ );

	field__ GetField( const str::string_ &Pattern );

	class user___
	{
	private:
		_user___ Core_;
		bso::bool__ _FacetiousButtonIsVisible;
		field__ _EditableField;
		str::string _FieldContent[f_amount];
	public:
		void reset( bso::bool__ P = true )
		{	
			int i = f_amount;

			while ( i-- )
				_FieldContent[i].reset( P );

			Core_.reset( P );
			_FacetiousButtonIsVisible = false;

			_EditableField = f_Undefined;
		}
		E_CVDTOR( user___ );
		void Init( frdfrntnd::frontend___ &Frontend )
		{
			int i = f_amount;

			while ( i-- )
				_FieldContent[i].Init();

			if ( Frontend.IsConnected() )
				Core_.Init( Frontend );

			_FacetiousButtonIsVisible = true;
			_EditableField = f_Undefined;
		}
		str::string_ &FieldContent( field__ Field )
		{
			return _FieldContent[Field];
		}
		E_RWDISCLOSE__( bso::bool__, FacetiousButtonIsVisible );
		E_RWDISCLOSE__( field__, EditableField );
	};
}

#endif
