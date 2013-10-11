/*
	Header for the 'bkdrpl' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: bkdrpl.h,v 1.4 2011/06/11 18:16:00 csimon Exp $

#ifndef BKDRPL__INC
#define BKDRPL__INC

#define BKDRPL_NAME		"BKDRPL"

#define	BKDRPL_VERSION	"$Revision: 1.4 $"

#define BKDRPL_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &BKDRPLTutor;

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
		rUserError,		// Erreur définie par l'utilisateur.
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
