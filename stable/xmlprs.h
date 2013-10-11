/*
  Header for the 'xmlprs' library by Claude L. Simon (csimon@webmails.com)
  Copyright (C) 2000,2001 Claude L. SIMON (csimon@webmails.com) 

  This file is part of the Epeios (http://epeios.org/) project.
  

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

//	$Id: xmlprs.h,v 1.6 2012/11/14 16:06:40 csimon Exp $

#ifndef XMLPRS__INC
#define XMLPRS__INC

#define XMLPRS_NAME		"XMLPRS"

#define	XMLPRS_VERSION	"$Revision: 1.6 $"	

#define XMLPRS_OWNER		"the Epeios project (http://epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &XMLPRSTutor;

#if defined( E_DEBUG ) && !defined( XMLPRS_NODBG )
#define XMLPRS_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.6 $
//C Claude L. SIMON (csimon@webmails.com)
//R $Date: 2012/11/14 16:06:40 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D eXtended Markup Langage PaRSer
/* End addendum to automatic documentation generation part. */

#error "Obsolete. Use library 'HOSPRS' instead."

#include "err.h"
#include "flw.h"
#include "xmldcm.h"
#include "xmldbs.h"

namespace xmlprs {
	/*f Fill 'XMLD' with contents of 'Flow', with buffer
	of 'BufferLength' (expat issue). */
	void ParseXML(
		txf::text_iflow___ &Flow,
		xmldcm::xml_document_ &XMLD,
		int BufferLength );

	/*f Fill 'XMLD' with contents of 'Flow', with buffer
	of 'BufferLength' (expat issue). */
	void ParseXML(
		txf::text_iflow___ &Flow,
		xmldbs::xml_database_ &XMLD,
		int BufferLength );

	void License( txf::text_oflow___ &OFlow );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
