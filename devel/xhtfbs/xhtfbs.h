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

# include "frdbse.h"

# include "rgstry.h"

# include "lcl.h"
# include "err.h"
# include "flw.h"
# include "xml.h"

namespace xhtfbs {

	E_CDEF( char *, ContentTagName, "Content" );
	E_CDEF( char *, ContextTagName, "Context" );

	class generic_rack___
	{
	private:
		flx::E_STRING_TOFLOW___ _Flow;
		xml::writer _Writer;
	public:
		void reset( bso::bool__ P = true )
		{
			_Writer.reset( P );
			_Flow.reset( P );
		}
		E_CDTOR( generic_rack___ );
		void Init(
			const char *RootTagName,
			const char *SubRootTagName,
			str::string_ &Target )
		{
			_Flow.Init( Target );
			_Writer.Init( _Flow, xml::oIndent, xml::e_Default );
			_Writer.PushTag( RootTagName );
			_Writer.PutAttribute("TimeStamp", tol::DateAndTime() );
			_Writer.PutAttribute("Enviroment", CPE_ENVIROMENT_LABEL );
			_Writer.PushTag( SubRootTagName );
		}
		operator xml::writer_ &()
		{
			return _Writer;
		}
		xml::writer_ &operator()( void )
		{
			return _Writer;
		}
	};

	class content_rack___
	: public generic_rack___
	{
	public:
		void Init(
			const char *RootTagName,
			str::string_ &Target )
		{
			generic_rack___::Init( RootTagName, ContentTagName, Target );
		}
	};

	class context_rack___
	: public generic_rack___
	{
	public:
		void Init(
			const char *RootTagName,
			str::string_ &Target )
		{
			generic_rack___::Init( RootTagName, ContextTagName, Target );
		}
	};

# define XHTFBS_RACK( RootTagName, Type )\
	class Type##_rack___\
	: public xhtfbs::Type##_rack___\
	{\
	public:\
		void Init( str::string_ &Target )\
		{\
			xhtfbs::Type##_rack___::Init( RootTagName, Target );\
		}\
	}

# define XHTFBS_RACKS( RootTagName)\
	XHTFBS_RACK( RootTagName, content );\
	XHTFBS_RACK( RootTagName, context )

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
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
