/*
	Header for the 'mscvkx' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2002, 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: mscvkx.h,v 1.1 2010/07/16 08:25:54 csimon Exp $

#ifndef MSCVKX__INC
#define MSCVKX__INC

#define MSCVKX_NAME		"MSCVKX"

#define	MSCVKX_VERSION	"$Revision: 1.1 $"

#define MSCVKX_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"


#if defined( E_DEBUG ) && !defined( MSCVKX_NODBG )
#define MSCVKX_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.1 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2010/07/16 08:25:54 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D MuSiC VK (from Roland VK combo organ) XML 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "mscvkp.h"
#include "xtf.h"
#include "xpp.h"

namespace mscvkx {
	//f Write 'DataSets' into 'Database'.
	void Write(
		const mscvkp::data_sets_ &DataSets,
		txf::text_oflow__ &Flow );

	xpp::status__ Parse( 
		flw::iflow__ &IFlow,
		const str::string_ &BaseDirectory,
		mscvkp::data_sets_ &DataSets,
		xtf::coord__ &ErrorCoords,
		str::string_ &GuiltyFileName );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
