/*
	Header for the 'sclcgi' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20132004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: sclcgi.h,v 1.1 2013/03/08 11:13:03 csimon Exp $

#ifndef SCLCGI__INC
#define SCLCGI__INC

#define SCLCGI_NAME		"SCLCGI"

#define	SCLCGI_VERSION	"$Revision: 1.1 $"

#define SCLCGI_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &SCLCGITutor;

#if defined( E_DEBUG ) && !defined( SCLCGI_NODBG )
#define SCLCGI_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.1 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/03/08 11:13:03 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D SoCLe CGI (Common Gateway Interface) 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

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

	// A définir par l'utilisateur.
	steering_callback__ *SCLCGICreateSteering( void );	// To overload !

	// A définir par l'utilisateur.
	void SCLCGIDeleteSteering( steering_callback__ *SteeringCallback );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
