/*
	Header for the 'scldaemon' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20132004 Claude SIMON.

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

//	$Id: scldaemon.h,v 1.2 2013/03/08 11:13:03 csimon Exp $

#ifndef SCLDAEMON__INC
#define SCLDAEMON__INC

#define SCLDAEMON_NAME		"SCLDAEMON"

#define	SCLDAEMON_VERSION	"$Revision: 1.2 $"

#define SCLDAEMON_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &SCLDAEMONTutor;

#if defined( E_DEBUG ) && !defined( SCLDAEMON_NODBG )
#define SCLDAEMON_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.2 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/03/08 11:13:03 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D SoCLe DAEMON 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"

#include "csdleo.h"

namespace scldaemon {
	const char *GetLanguage( void );

	// A définir par l'utilisateur.
	extern const char *TargetName;

	// A définir par l'utilisateur.
	csdleo::user_functions__ *RetrieveSteering(
		csdleo::mode__ Mode,
		const lcl::locale_ &Locale );	// To overload !

	// A définir par l'utilisateur.
	void ReleaseSteering( csdleo::user_functions__ *Steering );	// To overload.

	void DisplayModuleClosingMessage( void );

	void DisplayModuleClosedMessage( void );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
