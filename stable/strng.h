/*
	Header for the 'strng' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: strng.h,v 1.7 2013/05/19 09:28:14 csimon Exp $

#ifndef STRNG__INC
#define STRNG__INC

#define STRNG_NAME		"STRNG"

#define	STRNG_VERSION	"$Revision: 1.7 $"

#define STRNG_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &STRNGTutor;

#if defined( E_DEBUG ) && !defined( STRNG_NODBG )
#define STRNG_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.7 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/05/19 09:28:14 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D STRiNG 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "cpe.h"

/* Cette bibliothèque vise à remplacer <string.h>, car l'inclusion directe de cette dernière
pose problème avec la bibliothèque 'FLS' sous MacOS. */

# ifdef CPE_XCODE
namespace strng {
# endif
# include <string.h>
# ifdef CPE_XCODE
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
using strng::strcpy;
using strng::strerror;
using strng::strlen;
using strng::strpbrk;
using strng::strrchr;
# endif


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
