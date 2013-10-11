/*
	Header for the 'mllio' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: mllio.h,v 1.7 2012/12/04 15:28:51 csimon Exp $

#ifndef MLLIO__INC
#define MLLIO__INC

#define MLLIO_NAME		"MLLIO"

#define	MLLIO_VERSION	"$Revision: 1.7 $"

#define MLLIO_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &MLLIOTutor;

#if defined( E_DEBUG ) && !defined( MLLIO_NODBG )
#define MLLIO_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.7 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/12/04 15:28:51 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D MinGW Low-Level Input/Output 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "wllio.h"

# define MLLIO_UNDEFINED_DESCRIPTOR WLLIO_UNDEFINED_DESCRIPTOR

namespace mllio {
	using namespace wllio;
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
