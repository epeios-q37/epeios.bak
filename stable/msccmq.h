/*
	Header for the 'msccmq' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

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

//	$Id: msccmq.h,v 1.1 2010/07/15 10:58:18 csimon Exp $

#ifndef MSCCMQ__INC
#define MSCCMQ__INC

#define MSCCMQ_NAME		"MSCCMQ"

#define	MSCCMQ_VERSION	"$Revision: 1.1 $"

#define MSCCMQ_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &MSCCMQTutor;

#if defined( E_DEBUG ) && !defined( MSCCMQ_NODBG )
#define MSCCMQ_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.1 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2010/07/15 10:58:18 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D MuSiC Customized Midi Quantizer 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "mscmld.h"
#include "mscmdf.h"
#include "mscmdm.h"

namespace msccmq {
	void Parse(
		flw::iflow__ &IFlow,
		mscmld::melody_ &Melody );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
