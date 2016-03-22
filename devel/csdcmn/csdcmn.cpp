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

#define CSDCMN__COMPILATION

#include "csdcmn.h"

#include "str.h"

using namespace csdcmn;

namespace {
	void Write_(
		const char *Text,
		flw::oflow__ &Flow )
	{
		Flow.Write( Text, strlen( Text ) + 1 );	// '+1' to put the final 0.
	}
}

void csdcmn::SendProtocol(
	const char *Label,
	sVersion Version,
	flw::oflow__ &Flow )
{
	bso::buffer__ Buffer;

	if ( Version == UndefinedVersion )
		qRFwk();

	Write_( Label, Flow );

	Write_(bso::Convert( Version, Buffer ), Flow );
}

namespace {
	inline const str::dString &GetString_(
		flw::iflow__ &Flow,
		str::dString &String )
	{
		bso::char__ C = 0;

		while ( ( C = Flow.Get() ) != 0 )
			String.Append( C );

		return String;
	}
}


sVersion csdcmn::GetProtocolVersion(
	const char *Label,
	flw::iflow__ &Flow )
{
	sVersion Version = UndefinedVersion;
qRH
	str::wString Incoming;
	sdr::sRow Error = qNIL;
qRB
	Incoming.Init();
	GetString_( Flow, Incoming );

	if ( Incoming != Label )
		qRReturn;

	Incoming.Init();
	GetString_( Flow, Incoming );

	Incoming.ToNumber( Version, &Error );

	if ( Error != qNIL )
		Version = UndefinedVersion;
qRR
qRT
qRE
	return Version;
}


