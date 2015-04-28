/*
	Header for the 'geckob' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20112004 Claude SIMON.

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

//	$Id: geckob.h,v 1.11 2013/04/09 17:57:31 csimon Exp $

#ifndef GECKOB__INC
#define GECKOB__INC

#define GECKOB_NAME		"GECKOB"

#define	GECKOB_VERSION	"$Revision: 1.11 $"

#define GECKOB_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( GECKOB_NODBG )
#define GECKOB_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.11 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/09 17:57:31 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D GECKO Backend 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

#include "geckoo.h"

namespace geckob {
	geckoo::steering_callback__ *GECKOBCreateSteering( geckoo::shared_data__ *Data );	// A surcharger.
	void GECKOBDeleteSteering( geckoo::steering_callback__ * );	// A surcharger.

	void SetSteering( geckoo::steering_callback__ &Steering );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
