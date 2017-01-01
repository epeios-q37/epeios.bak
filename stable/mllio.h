/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

//	$Id: mllio.h,v 1.7 2012/12/04 15:28:51 csimon Exp $

#ifndef MLLIO__INC
#define MLLIO__INC

#define MLLIO_NAME		"MLLIO"

#define	MLLIO_VERSION	"$Revision: 1.7 $"

#define MLLIO_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( MLLIO_NODBG )
#define MLLIO_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.7 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/12/04 15:28:51 $

/* End of automatic documentation generation part. */

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
#endif
