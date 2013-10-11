/*
	Header for the 'cgigat' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2001-2004 Claude SIMON.

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

//	$Id: cgigat.h,v 1.2 2013/01/24 08:25:25 csimon Exp $

#ifndef CGIGAT__INC
#define CGIGAT__INC

#define CGIGAT_NAME		"CGIGAT"

#define	CGIGAT_VERSION	"$Revision: 1.2 $"

#define CGIGAT_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &CGIGATTutor;

#if defined( E_DEBUG ) && !defined( CGIGAT_NODBG )
#define CGIGAT_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.2 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/01/24 08:25:25 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D CGI (Common Gateway Interface) GATe 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "cgiarg.h"

# include "xml.h"

# include "csdbns.h"


namespace cgigat {

	typedef csdsuf::user_functions__ _server_user_functions__;
		
	class cgi_gate__
	: public _server_user_functions__
	{
	private:
		void _Process(
			const cgiarg::arguments_ &Arguments,
			xml::writer_ &Writer,
			str::string_ &XMLFileName )
		{
			CGIDATProcess( Arguments, Writer, XMLFileName );
		}
		void _Process(
			const cgiarg::arguments_ &Arguments,
			str::string_ &XML,
			str::string_ &XMLFileName );
		void *CSDSUFPreProcess( const char *Origin )
		{
			return NULL;
		}
		csdsuf::action__ CSDSUFProcess(
			flw::ioflow__ &Flow,
			void *UP );
		void CSDSUFPostProcess( void *UP )
		{}
		void CSDSUFExit( void )
		{}
	protected:
		virtual void CGIDATProcess(
			const cgiarg::arguments_ &Arguments,
			xml::writer_ &Writer,
			str::string_ &XMLFileName ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			_server_user_functions__::reset( P );
		}
		E_CVDTOR( cgi_gate__ )
		void Init( void )
		{
			_server_user_functions__::Init();
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
