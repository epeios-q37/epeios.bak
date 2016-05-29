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

// Client-Server Devices CoMmoN

#ifndef CSDCMN__INC
# define CSDCMN__INC

# define CSDCMN_NAME		"CSDCMN"

# if defined( E_DEBUG ) && !defined( CSDCMN_NODBG )
#  define CSDCMN_DBG
# endif

# include "err.h"

# include "bso.h"
# include "tol.h"
# include "flw.h"
# include "str.h"
# include "dtfptb.h"

namespace csdcmn {
	typedef bso::uint__ sVersion;

	qCDEF( sVersion, UndefinedVersion, BSO_UINT_MAX );

	void SendProtocol(
		const char *Id,
		sVersion Version,
		flw::oflow__ &Flow );

	sVersion GetProtocolVersion(
		const char *Id,
		flw::iflow__ &Flow );	// If 'UndefinedVersion' is returned, then the protocol is not the good one, or the content of the flow is not valid.

	// Facilities.

	inline const str::dString &Get(
		flw::sIFlow &Flow,
		str::dString &Value )
	{
		bso::sSize Size = 0;
		
		dtfptb::VGet( Flow, Size );

		while ( Size-- )
			Value.Append( Flow.Get() );

		return Value;
	}

	inline void Put(
		const str::dString &Value,
		flw::sOFlow &Flow )
	{
		dtfptb::VPut( Value.Amount(), Flow );

		sdr::sRow Row = Value.First();

		while ( Row != qNIL ) {
			Flow.Put(Value( Row ) );

			Row = Value.Next( Row );
		}
	}

	inline void Put(
		const char *Value,
		flw::sOFlow &Flow )
	{
		return csdcmn::Put( str::wString( Value ), Flow );	// 'csdcmn::' should not be necessary, but VC++ is confused.
	}
}

#endif
