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

//	$Id: csdsnb.h,v 1.4 2013/03/29 11:02:50 csimon Exp $

#ifndef CSDSNB__INC
#define CSDSNB__INC

#define CSDSNB_NAME		"CSDSNB"

#define	CSDSNB_VERSION	"$Revision: 1.4 $"

#define CSDSNB_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( CSDSNB_NODBG )
#define CSDSNB_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/03/29 11:02:50 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D Client-Server Standard Network Base 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#	include "err.h"
#	include "flw.h"
#	include "dtfptb.h"

#	define CSDSNB_PING	BSO_U8_MAX
#	define CSDSNB_CLOSE	( CSDSNB_PING - 1 )
#	define CSDSNB_UNDEFINED	( CSDSNB_CLOSE - 1 )
#	define CSDSNB_RESERVED	CSDSNB_UNDEFINED

namespace csdsnb {
	typedef bso::u8__ id__;

	inline void PutId(
		id__ Id,
		flw::oflow__ &Flow )
	{
		dtfptb::PutU8( Id, Flow );
	}

	inline id__ GetId( flw::iflow__ &Flow )
	{
		return dtfptb::GetU8( Flow );
	}
}

/*$END$*/
#endif
