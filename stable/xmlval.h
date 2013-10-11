/*
	Header for the 'xmlval' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2001-2004 Claude SIMON.

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

//	$Id: xmlval.h,v 1.5 2010/07/14 14:54:31 csimon Exp $

#ifndef XMLVAL__INC
#define XMLVAL__INC

#define XMLVAL_NAME		"XMLVAL"

#define	XMLVAL_VERSION	"$Revision: 1.5 $"

#define XMLVAL_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XMLVALTutor;

#if defined( E_DEBUG ) && !defined( XMLVAL_NODBG )
#define XMLVAL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.5 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2010/07/14 14:54:31 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D XML VALue 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D Hierarchically Organized Structures VALue.
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "xmlbsc.h"
#include "xmlelm.h"

namespace xmlval {

	using xmlelm::erow__;

	typedef xmlelm::item_ 	value_;
	typedef xmlelm::item	value;

	E_ROW( vrow__ );	// Value row.

	typedef xmlbsc::basic_<erow__, vrow__> value_tree_;
	E_AUTO( value_tree )



}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
