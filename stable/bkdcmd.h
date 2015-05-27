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

//	$Id: bkdcmd.h,v 1.8 2011/06/11 18:16:00 csimon Exp $

#ifndef BKDCMD__INC
#define BKDCMD__INC

#define BKDCMD_NAME		"BKDCMD"

#define	BKDCMD_VERSION	"$Revision: 1.8 $"

#define BKDCMD_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( BKDCMD_NODBG )
#define BKDCMD_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.8 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2011/06/11 18:16:00 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D BacKenD CoMmanDs 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'FBL...' libraries instead !"

#include "err.h"
#include "flw.h"
#include "bkdcst.h"

// Si modification, modifier 'BKDRPL' en consquence.

//d Version of the commands.
// #define BKDCMD_COMMANDS_VERSION	"4"

namespace bkdcmd {
	using namespace bkdcst;

	enum command {
		cThrowUError,
		cThrowIError,
		cThrowUserDefinedError,
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
	extern const char *CommandsNames[bkdcmd::c_amount];

	//o Commands parameters.
	extern const cast *CommandsParameters[bkdcmd::c_amount];

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
