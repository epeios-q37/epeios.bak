/*
	'xhtfbs.h' by Claude SIMON (http://zeusw.org/).

	'xhtfbs' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef XHTFBS__INC
# define XHTFBS__INC

# define XHTFBS_NAME		"XHTFBS"

# if defined( E_DEBUG ) && !defined( XHTFBS_NODBG )
#  define XHTFBS_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// XHT(ML) Frontend Base

# include "xhtagent.h"

# include "rgstry.h"

# include "lcl.h"
# include "err.h"
# include "flw.h"
# include "xml.h"

namespace xhtfbs {

	class xml_rack___
	{
	private:
		flx::E_STRING_OFLOW___ _Flow;
		txf::text_oflow__ _TFlow;
		xml::writer _Writer;
	public:
		void reset( bso::bool__ P = true )
		{
			_Writer.reset( P );
			_TFlow.reset( P );
			_Flow.reset( P );
		}
		E_CDTOR( xml_rack___ );
		void Init( str::string_ &Target )
		{
			_Flow.Init( Target );
			_TFlow.Init( _Flow );
			_Writer.Init( _TFlow, xml::oIndent, xml::e_Default );
		}
		xml::writer_ &operator()( void )
		{
			return _Writer;
		}
	};

	class event_handler__
	{
	public:
		void reset( bso::bool__ = true )
		{
			//Standardisation.
		}
		E_CVDTOR( event_handler__ )
		void Init( void )
		{
			// Standadisation;
		}
	};

	void Start(
		const rgstry::multi_level_registry_ &Registry,
		const lcl::locale_ &Locale,
		const char *Language,
		xml::writer_ &Writer );

	enum project_type__ {
		ptNew,
		ptPredefined,
		ptUser,
		pt_amount,
		pt_Undefined
	};

	const char *GetLabel( project_type__ ProjectType );

	project_type__ GetProjectType( const str::string_ &Pattern );

	enum backend_type__ {
		btDaemon,
		btEmbedded,
		btPredefined,
		bt_amount,
		bt_Undefined
	};

	const char *GetLabel( backend_type__ BackendType );

	backend_type__ GetBackendType( const str::string_ &Pattern );

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
