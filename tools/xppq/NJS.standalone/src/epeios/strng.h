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

//	$Id: strng.h,v 1.7 2013/05/19 09:28:14 csimon Exp $

#ifndef STRNG__INC
#define STRNG__INC

#define STRNG_NAME		"STRNG"

#define	STRNG_VERSION	"$Revision: 1.7 $"

#define STRNG_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( STRNG_NODBG )
#define STRNG_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.7 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/05/19 09:28:14 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D STRiNG 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "cpe.h"

/* Cette bibliothque vise  remplacer <string.h>, car l'inclusion directe de cette dernire
pose problme avec la bibliothque 'FLS' sous MacOS. */

# if 0	// OLD. Obsolete ?
# ifdef CPE_XCODE
# include <unistd.h>
namespace strng {
# endif
# include <string.h>
# ifdef CPE_XCODE
using ::ssize_t;
}

using strng::memchr;
using strng::memcmp;
using strng::memcpy;
using strng::memmove;
using strng::memset;

using strng::ssize_t;

using strng::strcat;
using strng::strchr;
using strng::strcmp;
using strng::strncmp;
using strng::strcpy;
using strng::strerror;
using strng::strlen;
using strng::strpbrk;
using strng::strrchr;
# endif
# else
#  include <string.h>
# endif


/*$END$*/
#endif
