/*
	'sclcgi.h' by Claude SIMON (http://zeusw.org/).

	'sclcgi' is part of the Epeios framework.

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

#ifndef SCLCGI__INC
# define SCLCGI__INC

# define SCLCGI_NAME		"SCLCGI"

# if defined( E_DEBUG ) && !defined( SCLCGI_NODBG )
#  define SCLCGI_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// SoCLe CGI (Common Gateway Interface)

# include "err.h"
# include "flw.h"

# include "cgiarg.h"
# include "xml.h"
# include "sclxslui.h"

namespace sclcgi {
	using namespace sclxslui;

	const char *GetLanguage( void );	// Langage d'administrartion.

	enum session_handling__ 
	{
		shKeepAlive,
		shClose,
		sh_amount,
		sh_Undefined
	};

	class steering_callback__ {
	protected:
		virtual void *SCLCGINewSession( const cgiarg::arguments_ &Arguments ) = 0;
		virtual session_handling__ SCLCGIHandleQuery(
			void *UP,
			const cgiarg::arguments_ &Arguments,
			xml::writer_ &Writer,
			str::string_ &XSLFileName ) = 0;
		virtual void SCLCGIReportExpiredSession(
			xml::writer_ &Writer,
			str::string_ &XMLFileName ) = 0;
		virtual void SCLCGIDeleteSession( void *UP ) = 0;
	public:
		void reset( bso::bool__ = true )
		{}
		E_CVDTOR( steering_callback__ );
		void Init( void )
		{}
		void *NewSession( const cgiarg::arguments_ &Arguments )
		{
			return SCLCGINewSession( Arguments );
		}
		session_handling__ HandleQuery(
			void *UP,
			const cgiarg::arguments_ &Arguments,
			xml::writer_ &Writer,
			str::string_ &XSLFileName ) 
		{
			return SCLCGIHandleQuery( UP, Arguments, Writer, XSLFileName );
		}
		void ReportExpiredSession(
			xml::writer_ &Writer,
			str::string_ &XSLFileName )
		{
			SCLCGIReportExpiredSession( Writer, XSLFileName );
		}
		 void DeleteSession( void *UP )
		{
			SCLCGIDeleteSession( UP );
		}
	};

	// A dfinir par l'utilisateur.
	steering_callback__ *SCLCGICreateSteering( void );	// To overload !

	// A dfinir par l'utilisateur.
	void SCLCGIDeleteSteering( steering_callback__ *SteeringCallback );

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
