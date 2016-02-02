/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

// MuXed Connections BaSE

#ifndef MXCBSE__INC
# define MXCBSE__INC

# define MXCBSE_NAME		"MXCBSE"

# if defined( E_DEBUG ) && !defined( MXCBSE_NODBG )
#  define MXCBSE_DBG
# endif

# include "dtfptb.h"
# include "flw.h"
# include "bso.h"

# include "err.h"

# define MXCBSE_PING	BSO_UINT_MAX
# define MXCBSE_CLOSE	( MXCBSE_PING - 1 )
# define MXCBSE_UNDEFINED	( MXCBSE_CLOSE - 1 )
# define MXCBSE_RESERVED	MXCBSE_UNDEFINED

namespace mxcbse {
	typedef bso::uint__ fId;

	inline void PutId(
		fId Id,
		flw::oflow__ &Flow )
	{
		dtfptb::VPut( Id, Flow );
	}

	inline fId GetId( flw::iflow__ &Flow )
	{
		fId Id = 0;

		return dtfptb::VGet( Flow,Id );
	}
}

#endif
