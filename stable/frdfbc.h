/*
	Header for the 'frdfbc' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: frdfbc.h,v 1.5 2012/11/14 16:06:29 csimon Exp $

#ifndef FRDFBC__INC
#define FRDFBC__INC

#define FRDFBC_NAME		"FRDFBC"

#define	FRDFBC_VERSION	"$Revision: 1.5 $"

#define FRDFBC_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( FRDFBC_NODBG )
#define FRDFBC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.5 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/11/14 16:06:29 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D FRontenD Frontend Backend Common 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#eerror "No more in use !"

#include "err.h"
#include "flw.h"
#include "rgstry.h"
#include "lcl.h"

#define FRDFBC_DATA_STRUCTURE_VERSION	"1"

namespace frdfbc {

	// If modified, update 'FRDFBC_DATA_STRUCTURE_VERSION'.
	struct data___ {
		const char *Id;
		const rgstry::registry_ *Registry;
		rgstry::row__ Root;
		lcl::locale_ *Locale;
		str::string Message;
		data___( void )
		{
			Id = NULL;
			Registry = NULL;
			Root = NONE;
			Locale = NULL;
		}
		void Init( void )
		{
			Message.Init();
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
