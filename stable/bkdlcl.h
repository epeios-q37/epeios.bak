/*
	Header for the 'bkdlcl' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2002-2004 Claude SIMON.

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

//	$Id: bkdlcl.h,v 1.2 2011/06/11 18:16:00 csimon Exp $

#ifndef BKDLCL__INC
#define BKDLCL__INC

#define BKDLCL_NAME		"BKDLCL"

#define	BKDLCL_VERSION	"$Revision: 1.2 $"

#define BKDLCL_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &BKDLCLTutor;

#if defined( E_DEBUG ) && !defined( BKDLCL_NODBG )
#define BKDLCL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.2 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2011/06/11 18:16:00 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D BacKenD LoCaLes 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'FBL...' libraries instead !"

#include "err.h"
#include "flw.h"
#include "lcl.h"

namespace bkdlcl {
	using namespace lcl;
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
