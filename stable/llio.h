/*
	Header for the 'llio' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

//	$Id: llio.h,v 1.13 2013/04/06 15:33:18 csimon Exp $

#ifndef LLIO__INC
#define LLIO__INC

#define LLIO_NAME		"LLIO"

#define	LLIO_VERSION	"$Revision: 1.13 $"

#define LLIO_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"


#if defined( E_DEBUG ) && !defined( LLIO_NODBG )
#define LLIO_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.13 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/06 15:33:18 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Low-Level Input/Output 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "cpe.h"

# undef LLIO__USE_MLLIO
# undef LLIO__USE_PLLIO
# undef LLIO__USE_WLLIO

#ifdef CPE_VC
# define LLLIO__USE_WLLIO
#elif defined( CPE_LINUX )
# define LLLIO__USE_PLLIO
#elif defined( CPE_CYGWIN )
# define LLLIO__USE_PLLIO
#elif defined( CPE_XCODE )
# define LLLIO__USE_PLLIO
#elif defined( CPE_MINGW )
# define LLLIO__USE_MLLIO
#else
# error "Undefined platform !"
#endif

# ifdef LLLIO__USE_MLLIO
#  include "mllio.h"
#  define LLIO_UNDEFINED_DESCRIPTOR MLLIO_UNDEFINED_DESCRIPTOR
# elif defined( LLLIO__USE_PLLIO )
#  include "pllio.h"
#  define LLIO_UNDEFINED_DESCRIPTOR PLLIO_UNDEFINED_DESCRIPTOR
# elif defined( LLLIO__USE_WLLIO )
#  include "wllio.h"
#  define LLIO_UNDEFINED_DESCRIPTOR WLLIO_UNDEFINED_DESCRIPTOR
# else
#  error "Should not occur !!!"
# endif

namespace llio {
# ifdef LLLIO__USE_MLLIO
	using namespace mllio;
# elif defined( LLLIO__USE_PLLIO )
	using namespace pllio;
# elif defined( LLLIO__USE_WLLIO )
	using namespace wllio;
# else
#  error "Should not occur !!!"
# endif
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
