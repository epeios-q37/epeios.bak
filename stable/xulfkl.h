/*
	Header for the 'xulfkl' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: xulfkl.h,v 1.17 2012/10/24 15:03:24 csimon Exp $

#ifndef XULFKL__INC
#define XULFKL__INC

#define XULFKL_NAME		"XULFKL"

#define	XULFKL_VERSION	"$Revision: 1.17 $"

#define XULFKL_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( XULFKL_NODBG )
#define XULFKL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.17 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/10/24 15:03:24 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D XUL Frontend KerneL 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete !"

#include "err.h"
#include "flw.h"

#include "frdkrn.h"

#define XULFKL_M_AMOUNT	2

namespace xulfkl {

	using frdkrn::status__;

	enum message__ 
	{
		mSelectProjectFile,
		mExitConfirmation,
		m_amount,
		m_Undefined
	};

	const char *GetLabel( message__ Message );

	using frdkrn::kernel___;
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
