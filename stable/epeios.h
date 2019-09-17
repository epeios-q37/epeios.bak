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

//	$Id: epeios.h,v 1.15 2012/05/03 09:52:49 csimon Exp $

#ifndef EPEIOS_INC_
#define EPEIOS_INC_

#define EPEIOS_NAME		"EPEIOS"

#define	EPEIOS_VERSION	"$Revision: 1.15 $"

#define EPEIOS_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( EPEIOS_NODBG )
#define EPEIOS_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.15 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/05/03 09:52:49 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D EPEIOS often used features. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D EPEIOS current features.
/* End addendum to automatic documentation generation part. */

// # error "Obsolete !"

#include "err.h"
#include "flw.h"
#include "tol.h"
#include "mdr.h"

namespace epeios_deprecated {
	typedef mdr::row_t__	row_t__;
	E_TMIMIC__( row_t__, row__ );

	typedef mdr::p_row_t__	p_row_t__;
	E_TMIMIC__( p_row_t__, p_row__ );

	#define EPEIOS_DEPRECATED_ROW_MAX		MDR_ROW_T_MAX

	using mdr::size__;

	#define EPEIOS_DEPRECATED_SIZE_MAX		MDR_SIZE_MAX

#ifndef NONE
#	define NONE ((mdr::row_t__)-1)
#endif

	using mdr::datum__;
}

/*$END$*/
#endif
