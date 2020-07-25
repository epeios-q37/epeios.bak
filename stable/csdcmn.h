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

// Client-Server Devices CoMmoN

#ifndef CSDCMN_INC_
# define CSDCMN_INC_

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
	typedef bso::sU8 sVersion;

	qCDEF(sVersion, UnknownVersion, bso::U8Max );
	qCDEF(sVersion, BadProtocol, UnknownVersion - 1);
	// If above modified, adjust below!
	qCDEF(sVersion, VersionMax, BadProtocol - 1);

	void SendProtocol(
		const char *Id,
		sVersion Version,
		flw::oflow__ &Flow );

	sVersion GetProtocolVersion(
		const char *Id,
		sVersion LastVersion,	// To avoid, when updating protocol id, to accept version from previous protocol.
		flw::iflow__ &Flow );	// If 'UndefinedVersion' is returned, then the protocol is not the good one, or the content of the flow is not valid.

	// Facilities.

	inline const str::dString &Get_(
		flw::rRFlow &Flow,
		str::dString &String )
	{
		bso::sSize Size = 0;

		dtfptb::VGet( Flow, Size );

		while ( Size-- )
			String.Append( Flow.Get() );

		return String;
	}

	inline const str::dString &Get(
		flw::rRFlow &Flow,
		str::dString &String )
	{
		return Get_( Flow, String );
	}

	inline const str::dString &Get(
		flw::rRFlow &Flow,
		str::wString &String )
	{
		return Get_( Flow, String );
	}

	inline void Put_(
		const str::dString &String,
		flw::rWFlow &Flow )
	{
		dtfptb::VPut( String.Amount(), Flow );

		sdr::sRow Row = String.First();

		while ( Row != qNIL ) {
			Flow.Put( String( Row ) );

			Row = String.Next( Row );
		}
	}

	inline void Put(
		const str::dString &String,
		flw::rWFlow &Flow )
	{
		return Put_( String, Flow );
	}

	inline void Put(
		const str::wString &String,
		flw::rWFlow &Flow )
	{
		return Put_( String, Flow );
	}

	inline void Put(
		const char *String,
		flw::rWFlow &Flow )
	{
		return Put_( str::wString( String ), Flow );
	}

	inline void Put(
		char *String,
		flw::rWFlow &Flow )
	{
		return Put( (const char *)String, Flow );
	}

	void Put_(
		const str::dStrings &Strings,
		flw::rWFlow &Flow );

	inline void Put(
		const str::dStrings &Strings,
		flw::rWFlow &Flow )
	{
		return Put_( Strings, Flow );
	}

	inline void Put(
		const str::wStrings &Strings,
		flw::rWFlow &Flow )
	{
		return Put_( Strings, Flow );
	}

	void Get_(
		flw::rRFlow &Flow,
		str::dStrings &Strings );

	inline void Get(
		flw::rRFlow &Flow,
		str::dStrings &Strings )
	{
		Get_( Flow, Strings );
	}

	inline void Get(
		flw::rRFlow &Flow,
		str::wStrings &Strings )
	{
		Get_( Flow, Strings );
	}

	template <typename integer> inline void Put(
		integer Integer,
		flw::rWFlow &Flow )
	{
		return dtfptb::VPut( Integer, Flow );
	}

	template <typename integer> inline void Put(
		integer Integer,
		fdr::rWDriver &Driver )
	{
		return dtfptb::VPut( Integer, Driver );
	}

	template <typename integer> inline integer Get(
		flw::rRFlow &Flow,
		integer &Integer )
	{
		return dtfptb::VGet( Flow, Integer );
	}

	template <typename integer> inline integer Get(
		fdr::rRDriver &Driver,
		integer &Integer )
	{
		return dtfptb::VGet( Driver, Integer );
	}
}

#endif
