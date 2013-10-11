/*
	Header for the 'sclfrntnd' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: xxx.h,v 1.9 2012/11/14 16:06:23 csimon Exp $

#ifndef SCLFRNTND__INC
#define SCLFRNTND__INC

#define SCLFRNTND_NAME		"SCLFRNTND"

#define	SCLFRNTND_VERSION	"$Revision: 1.9 $"

#define SCLFRNTND_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &SCLFRNTNDTutor;

#if defined( E_DEBUG ) && !defined( SCLFRNTND_NODBG )
#define SCLFRNTND_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.9 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/11/14 16:06:23 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D SoCLe FRoNTeND 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "frdkrn.h"

namespace sclfrntnd {

	void Report(
		frdkrn::kernel___ &Kernel,
		const char *Message );

	void Notify(
		frdkrn::kernel___ &Kernel,
		const char *Message );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
