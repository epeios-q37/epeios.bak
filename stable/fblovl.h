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

//	$Id: fblovl.h,v 1.5 2013/04/13 16:21:13 csimon Exp $

#ifndef FBLOVL__INC
#define FBLOVL__INC

#define FBLOVL_NAME		"FBLOVL"

#define	FBLOVL_VERSION	"$Revision: 1.5 $"

#define FBLOVL_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( FBLOVL_NODBG )
#define FBLOVL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.5 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/13 16:21:13 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Frontend/Backend Layout OVerLapping 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#define FBLOVL_PROTOCOL_VERSION	"9"

namespace fblovl {

	enum mode__ {
		mEmbedded,
		mRemote,
		m_Amount,
		m_Undefined
	};

	enum reply__ {
		rOK,
		rSoftwareError,	// Lorsque survient un 'ERR[x|X](...)'.
		rRequestError,	// Lorsque survient un problme lors de l'excution de la requte.
		r_amount,
		r_Undefined
	};

	const char *GetLabel( reply__ Reply );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
