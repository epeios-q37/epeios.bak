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

//	$Id: xulfrg.h,v 1.12 2013/01/02 17:32:14 csimon Exp $

#ifndef XULFRG__INC
#define XULFRG__INC

#define XULFRG_NAME		"XULFRG"

#define	XULFRG_VERSION	"$Revision: 1.12 $"

#define XULFRG_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( XULFRG_NODBG )
#define XULFRG_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.12 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/01/02 17:32:14 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D XUL Fro,te,d ReGistryWiDGets 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "frdrgy.h"

namespace xulfrg {

	using frdrgy::registry_;
	using frdrgy::registry;

	extern rgstry::entry___ AnnexTarget;
	extern rgstry::entry___ AnnexTargetType;

	extern rgstry::entry___ BackendSelectionMode;
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
