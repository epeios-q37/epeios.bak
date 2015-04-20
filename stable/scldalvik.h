/*
	Header for the 'scldalvik' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20122004 Claude SIMON.

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

//	$Id: scldalvik.h,v 1.3 2012/09/23 19:54:55 csimon Exp $

#ifndef SCLDALVIK__INC
#define SCLDALVIK__INC

#define SCLDALVIK_NAME		"SCLDALVIK"

#define	SCLDALVIK_VERSION	"$Revision: 1.3 $"

#define SCLDALVIK_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &SCLDALVIKTutor;

#if defined( E_DEBUG ) && !defined( SCLDALVIK_NODBG )
#define SCLDALVIK_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.3 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/09/23 19:54:55 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D SoCLe DALVIK 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "jni.h"
# include "dalvik.h"

namespace scldalvik {

	// A dfinir par l'utilisateur.
	dalvik::steering_callback___ *CreateSteering( void );

	// A dfinir par l'utilisateur.
	void DeleteSteering( dalvik::steering_callback___ *Steering );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
