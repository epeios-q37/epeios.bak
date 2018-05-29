/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#ifndef SCLTOOL__INC
# define SCLTOOL__INC

# define SCLTOOL_NAME		"SCLTOOL"

# if defined( E_DEBUG ) && !defined( SCLTOOL_NODBG )
#  define SCLTOOL_DBG
# endif

// SoCLe TOOL

# include "err.h"
# include "flw.h"
# include "lcl.h"

# include "sclmisc.h"
# include "scllocale.h"
# include "sclerror.h"
# include "sclrgstry.h"

/*************************/
/****** New version ******/
/*************************/

namespace scltool {
	struct fOddities {
# ifdef CPE_S_WIN
		HINSTANCE hInstance;
		HINSTANCE hPrevInstance;
		PWSTR pCmdLine;
		int nCmdShow;
		LPWSTR *argv;
# else
		ntvstr::char__ **argv;
# endif
		int argc;
	};
}


/*************************/
/****** Olf version ******/
/*************************/

namespace scltool {

# if 0	// Obsolete ?
	enum command {
		cHelp,
		cVersion,
		cLicense,
		c_amount
	};
# endif

	typedef fOddities oddities__;

	// To define by user.
	const scli::sInfo &SCLTOOLInfo( void );

	// To define by user.
	int SCLTOOLMain(
		const str::string_ &Command,
		const oddities__ &Oddities );

	extern bso::bool__ IgnoreUnknownArguments;	// 'false' by default, meaning that all command-line arguments are handled, and unknown ones will be reported, but can be modified by user.
}

#endif
