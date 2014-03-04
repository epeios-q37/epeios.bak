/*
	Header for the 'sclerror' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20112004 Claude SIMON.

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

//	$Id: sclerror.h,v 1.6 2013/07/18 19:46:25 csimon Exp $

#ifndef SCLERROR__INC
#define SCLERROR__INC

#define SCLERROR_NAME		"SCLERROR"

#define	SCLERROR_VERSION	"$Revision: 1.6 $"

#define SCLERROR_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &SCLERRORTutor;

#if defined( E_DEBUG ) && !defined( SCLERROR_NODBG )
#define SCLERROR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.6 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/07/18 19:46:25 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D SoCLe ERROR 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "lcl.h"
# include "cio.h"

namespace sclerror {
	const lcl::meaning_ &GetMeaning( void );

	bso::bool__ IsErrorPending( void );

	void ResetPendingError( void );

	void SetMeaning( const lcl::meaning_ &Meaning );

	bso::bool__ GetPendingError(
		const char *Language,
		str::string_ &Translation,
		err::handling__ ErrHandling = err::h_Default );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
