/*
	Header for the 'dtflcf' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000, 2003 Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: dtflcf.h,v 1.6 2012/11/14 16:06:26 csimon Exp $

#ifndef DTFLCF__INC
#define DTFLCF__INC

#define DTFLCF_NAME		"DTFLCF"

#define	DTFLCF_VERSION	"$Revision: 1.6 $"

#define DTFLCF_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &DTFLCFTutor;

#if defined( E_DEBUG ) && !defined( DTFLCF_NODBG )
#define DTFLCF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.6 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:26 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Data TransFert LoCal Features 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#error "Obsolete. Use 'dtfntv' library instead."

#include "err.h"
#include "flw.h"
#include "dtfbsc.h"

namespace dtflcf {
	using namespace dtfbsc;

	//f Put 'Object', which is a pointer of type 'object', in 'Flow'.	
	template <typename object> inline void PutPointer(
		const object *Object,
		flw::oflow___ &Flow )
	{
		flw::Put( Object, Flow );
	}
		
	//f Get from 'Flow' a pointer to an object of type 'object'.
	template <typename object> inline object *GetPointer( flw::iflow___ &Flow )
	{
		object *P;
		
		flw::Get( Flow, P );
		
		return P;
	}
	
	//f Put 'Object' to 'Flow'.
	template <typename object__> inline void Put(
		object__ &Object,
		flw::oflow___ &Flow )
	{
		flw::Put( Object, Flow );
	}
	
	//f Return an object of type 'object__' from 'Flow'.
	template <typename object__> inline object__ Get( flw::iflow___ &Flow )
	{
		object__ O;
		
		flw::Get( Flow, O );
		
		return O;
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
