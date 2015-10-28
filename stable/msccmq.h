/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

//	$Id: msccmq.h,v 1.1 2010/07/15 10:58:18 csimon Exp $

#ifndef MSCCMQ__INC
#define MSCCMQ__INC

#define MSCCMQ_NAME		"MSCCMQ"

#define	MSCCMQ_VERSION	"$Revision: 1.1 $"

#define MSCCMQ_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( MSCCMQ_NODBG )
#define MSCCMQ_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.1 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2010/07/15 10:58:18 $

/* End of automatic documentation generation part. */

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
#endif
