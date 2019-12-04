/*
	'frdkernl.h' by Claude SIMON (http://q37.info/contact/).

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

#ifndef FRDFRNTND_INC_
# define FRDFRNTND_INC_

# include "xddinf.h"
# include "frdapi.h"

# include "frdrgstry.h"

# include "sclfrntnd.h"

# include "csducl.h"
# include "xml.h"


namespace frdfrntnd {
	typedef sclfrntnd::frontend___ _frontend___;

	class frontend___
	: public _frontend___
	{
	protected:
		virtual void FBLFRDOnConnect( void ) override
		{
			Statics.Init( *this );
			MyObject.Init( *this );
		}
		virtual void FBLFRDOnDisconnect( void ) override
		{
			Statics.reset();
			MyObject.reset();
		}
	public:
		xdhdq::statics___ Statics;
		xdhdq::xdd_myobject_common__ MyObject;
		void reset( bso::bool__ P = true )
		{
			_frontend___::reset( P );
			Statics.reset( P );
			MyObject.reset( P );
		}
		E_CVDTOR( frontend___ );
		void Init(
			sclfrntnd::kernel___ &Kernel,
			const char *Language,
			fblfrd::reporting_callback__ &ReportingCallback,
			const rgstry::multi_level_registry_ &Registry )
		{
			_frontend___::Init( Kernel, Language, ReportingCallback, Registry );
		}
		void TestMessage( void )
		{
			Statics.XDDTest();
		}
	};

}

#endif
