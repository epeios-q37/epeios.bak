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

//	$Id: xmlval.h,v 1.5 2010/07/14 14:54:31 csimon Exp $

#ifndef XMLVAL_INC_
#define XMLVAL_INC_

#define XMLVAL_NAME		"XMLVAL"

#define	XMLVAL_VERSION	"$Revision: 1.5 $"

#define XMLVAL_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( XMLVAL_NODBG )
#define XMLVAL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.5 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2010/07/14 14:54:31 $

/* End of automatic documentation generation part. */

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
#endif
