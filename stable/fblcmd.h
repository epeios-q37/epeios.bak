/*
	Header for the 'fblcmd' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: fblcmd.h,v 1.4 2013/04/09 17:57:23 csimon Exp $

#ifndef FBLCMD__INC
#define FBLCMD__INC

#define FBLCMD_NAME		"FBLCMD"

#define	FBLCMD_VERSION	"$Revision: 1.4 $"

#define FBLCMD_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( FBLCMD_NODBG )
#define FBLCMD_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/09 17:57:23 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Frontend/Backend Layout CoMmanDs 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "fblcst.h"

namespace fblcmd {
	using namespace fblcst;

	enum command {
		cThrowERRFwk,
		cThrowERRFree,
		cTestNotification,
		cGetNewObject,
		cGetType,
//		cGetRawMessages,
		cGetCommand,
		cGetCommands,
		cGetTypeAndCommands,
		cRemoveObject,
		cAbout,
		cDisconnect,
		cGetTypesIDAndPrefixAndName,
		cGetCommandsIDAndName,
		cGetParameters,
		cGetLanguage,
		cSetLanguage,
		c_amount
	};

	//o Command names.
	extern const char *CommandsNames[c_amount];

	//o Commands parameters.
	extern const cast__ *CommandsParameters[c_amount];


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
