/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
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
