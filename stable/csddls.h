/*
	Header for the 'csddls' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

//	$Id: csddls.h,v 1.10 2012/11/14 16:06:26 csimon Exp $

#ifndef CSDDLS__INC
#define CSDDLS__INC

#define CSDDLS_NAME		"CSDDLS"

#define	CSDDLS_VERSION	"$Revision: 1.10 $"

#define CSDDLS_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( CSDDLS_NODBG )
#define CSDDLS_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.10 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:26 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Client-Server Devices Dynamic Library Server 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#error "Obsolete : Use 'CSDLE'."

#include "err.h"
#include "flw.h"
#include "csdscm.h"
#include "cpe.h"

#ifdef CPE__T_MS
#	define CSDDSL__FUNCTION_SPEC	__declspec(dllexport)
#else
#	define CSDDSL__FUNCTION_SPEC
#endif

extern "C" CSDDSL__FUNCTION_SPEC csdscm::user_functions__ *CSDDLEntry( void *UP );

namespace csddls {
	csdscm::user_functions__ *CSDDLSCallback( void *UP );	// A surcharger.
}		

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
