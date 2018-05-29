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

// Client-Server Devices MuXing Base

#ifndef CSDMXB__INC
# define CSDMXB__INC

# define CSDMXB_NAME		"CSDMXB"

# if defined( E_DEBUG ) && !defined( CSDMXB_NODBG )
#  define CSDMXB_DBG
# endif

# include "bso.h"
# include "flw.h"
# include "dtfptb.h"

# include "err.h"

# define CSDMXB_PING		BSO_U32_MAX
# define CSDMXB_CLOSE		( CSDMXB_PING - 1 )
# define CSDMXB_UNDEFINED	( CSDMXB_CLOSE - 1 )
# define CSDMXB_RESERVED	CSDMXB_UNDEFINED

namespace csdmxb {
	typedef bso::u32__ sId;

	inline void PutId(
		sId Id,
		flw::oflow__ &Flow )
	{
		dtfptb::VPut( Id, Flow );
	}

	inline sId GetId( flw::iflow__ &Flow )
	{
		sId Id = 0;

		return dtfptb::VGet( Flow,Id );
	}
}

#endif
