/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

//	$Id: prshct.h,v 1.4 2012/11/14 16:06:32 csimon Exp $

#ifndef PRSHCT__INC
#define PRSHCT__INC

#define PRSHCT_NAME		"PRSHCT"

#define	PRSHCT_VERSION	"$Revision: 1.4 $"	

#define PRSHCT_OWNER		"the Epeios project (http://zeusw.org/epeios/)"

#if defined( E_DEBUG ) && !defined( PRSHCT_NODBG )
#define PRSHCT_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:32 $

/* End of automatic documentation generation part. */
/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "txmhch.h"
#include "txmpgh.h"
#include "xtf.h"

struct prshct {
	//e Error codes.
	enum error {
		//i No error.
		rOK,
		//i Level error.
		rLevel,
		//i Syntax error.
		rSyntax
	};
};

/*f Parse 'IFlow' with marker 'Marker' and fill 'Text'. If 'ErrH' != 'err::hUsual',
you MUST give a 'Error'. */
POSITION__ PRSHCTParse(
	xtf::extended_text_iflow___ &IFlow,
	txmhch_hierarchical_text_ &Text,
	bso__char Marker,
	err::handle ErrH = err::hUsual,
	prshct::error &Error = *(prshct::error *)NULL );

/*$END$*/
#endif
