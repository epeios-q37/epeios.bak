/*
	Header for the 'csdles' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

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

//	$Id: csdles.h,v 1.17 2013/04/15 10:50:49 csimon Exp $

#ifndef CSDLES__INC
#define CSDLES__INC

#define CSDLES_NAME		"CSDLES"

#define	CSDLES_VERSION	"$Revision: 1.17 $"

#define CSDLES_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &CSDLESTutor;

#if defined( E_DEBUG ) && !defined( CSDLES_NODBG )
#define CSDLES_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.17 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/15 10:50:49 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Client-Server Devices Library Embedded Server 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "csdleo.h"

namespace csdles {
	csdleo::user_functions__ *CSDLESRetrieveSteering( csdleo::data__ *Data );	// A surcharger.
	void CSDLESReleaseSteering( csdleo::user_functions__ * );	// A surcharger.
}		


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
