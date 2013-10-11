/*
	'frdkernl.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id: frdkernl.h,v 1.3 2013/06/18 09:36:02 csimon Exp $

#ifndef FRDKERNL__INC
# define FRDKERNL__INC

# include "sktinf.h"
# include "sktapi.h"

# include "frdrgstry.h"

# include "sclfrntnd.h"

# include "frdkrn.h"

# include "csdsnc.h"
# include "csducl.h"
# include "xml.h"


namespace frdkernl {
	typedef frdkrn::kernel___ _kernel___;

	using frdkrn::status__;

	class kernel___
	: public _kernel___
	{
	protected:
		virtual void FRDKRNConnection( fblfrd::frontend___ &Frontend )
		{
			Statics.Init( Frontend );
			MyObject.Init( Frontend );
		}
		virtual void FRDKRNDisconnection( void )
		{
			Statics.reset();
			MyObject.reset();
		}
	public:
		esketch::statics___ Statics;
		esketch::skt_myobject_common__ MyObject;
		void reset( bso::bool__ P = true )
		{
			_kernel___::reset( P );
			Statics.reset( P );
			MyObject.reset( P );
		}
		kernel___( void )
		{
			reset( false );
		}
		~kernel___( void )
		{
			reset();
		}
		status__ Init(
			const rgstry::registry_ &ConfigurationRegistry,
			rgstry::row__ ConfigurationRegistryRoot,
			const lcl::locale_ &Locale,
			const char *Language,
			frdkrn::reporting_functions__ &ReportingFunctions )
		{
			// Les autres objets sont initialisés lors de la connection.

			return _kernel___::Init( ConfigurationRegistry, ConfigurationRegistryRoot, Locale, Language, ReportingFunctions );
		}
		void Report( const char *Message )
		{
			sclfrntnd::Report( *this, Message );
		}
		void Notify( const char *Message )
		{
			sclfrntnd::Notify( *this, Message );
		}
	};

}

#endif
