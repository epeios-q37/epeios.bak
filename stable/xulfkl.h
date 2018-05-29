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

//	$Id: xulfkl.h,v 1.17 2012/10/24 15:03:24 csimon Exp $

#ifndef XULFKL__INC
#define XULFKL__INC

#define XULFKL_NAME		"XULFKL"

#define	XULFKL_VERSION	"$Revision: 1.17 $"

#define XULFKL_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( XULFKL_NODBG )
#define XULFKL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.17 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/10/24 15:03:24 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D XUL Frontend KerneL 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete !"

#include "err.h"
#include "flw.h"

#include "frdkrn.h"

#define XULFKL_M_AMOUNT	2

namespace xulfkl {

	using frdkrn::status__;

	enum message__ 
	{
		mSelectProjectFile,
		mExitConfirmation,
		m_amount,
		m_Undefined
	};

	const char *GetLabel( message__ Message );

	using frdkrn::kernel___;
}

/*$END$*/
#endif
