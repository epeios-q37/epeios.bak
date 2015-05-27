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

//	$Id: bkdrpl.h,v 1.4 2011/06/11 18:16:00 csimon Exp $

#ifndef BKDRPL__INC
#define BKDRPL__INC

#define BKDRPL_NAME		"BKDRPL"

#define	BKDRPL_VERSION	"$Revision: 1.4 $"

#define BKDRPL_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( BKDRPL_NODBG )
#define BKDRPL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2011/06/11 18:16:00 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D BacKenD RePLy 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'FBL...' libraries instead !"

#include "err.h"
#include "flw.h"
#include "bkdlcl.h"

#define BKDRPL_PROTOCOL_VERSION	"3"

namespace bkdrpl {

	enum reply__ {
		rOK,
		rBackendError,	// Lorsque survient un 'ERR[x|X](...)'.
		rUserError,		// Erreur dfinie par l'utilisateur.
		r_amount,
		r_Undefined
	};

	const char *Label( reply__ Reply );

	inline const char *GetTranslation(
		reply__ Reply,
		const bkdlcl::rack__ &Rack,
		STR_BUFFER___ &Buffer )
	{
		return Rack.GetTranslation( Label( Reply ), "BKDRPL_", Buffer );
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
