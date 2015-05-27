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

//	$Id: fblrpl.h,v 1.4 2012/11/14 16:06:27 csimon Exp $

#ifndef FBLRPL__INC
#define FBLRPL__INC

#define FBLRPL_NAME		"FBLRPL"

#define	FBLRPL_VERSION	"$Revision: 1.4 $"

#define FBLRPL_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( FBLRPL_NODBG )
#define FBLRPL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/11/14 16:06:27 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Frontend/Backend Layout RePLy 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "lcl.h"

#error "Obsolete ! Use 'FBMOVL' instead !".

#define FBLRPL_PROTOCOL_VERSION	"4"

namespace fblrpl {

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
		const lcl::rack__ &Rack,
		STR_BUFFER___ &Buffer )
	{
		return Rack.GetTranslation( Label( Reply ),  FBLRPL_NAME "_", Buffer );
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
