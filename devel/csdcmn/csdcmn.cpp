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

#define CSDCMN_COMPILATION_

#include "csdcmn.h"

#include "str.h"

using namespace csdcmn;

void csdcmn::SendProtocol(
	const char *Label,
	sVersion Version,
	flw::oflow__ &Flow )
{
	bso::pInteger Buffer;

	if ( Version > VersionMax )
		qRFwk();

	Put( Label, Flow );

	Put ( bso::Convert( Version, Buffer ), Flow );
}

sVersion csdcmn::GetProtocolVersion(
	const char *Label,
	sVersion LastVersion,
	flw::iflow__ &Flow )
{
	sVersion Version = BadProtocol;
qRH
	str::wString Incoming;
	sdr::sRow Error = qNIL;
qRB
	Incoming.Init();
	csdcmn::Get( Flow, Incoming );	// 'csdcmn::' should not be necessary, but VC++ is confused.

	if ( LastVersion > VersionMax )
		qRFwk();

	if ( Incoming == Label ) {
		Incoming.Init();
		csdcmn::Get( Flow, Incoming );

		Incoming.ToNumber( Version, &Error );

		if ( Error != qNIL )
			Version = BadProtocol;
		else if ( Version > LastVersion )
			Version = UnknownVersion;
	}
qRR
qRT
qRE
	return Version;
}

void csdcmn::Put_(
	const str::dStrings &Strings,
	flw::rWFlow &Flow )
{
	sdr::sRow Row = Strings.First();

	dtfptb::VPut( Strings.Amount(), Flow );

	while ( Row != qNIL ) {
		Put(Strings( Row ), Flow );

		Row = Strings.Next( Row );
	}
}

void csdcmn::Get_(
	flw::rRFlow &Flow,
	str::dStrings &Strings )
{
qRH
	str::wString String;
qRB
	bso::sSize Size = 0;

	dtfptb::VGet( Flow, Size );

	while ( Size-- ) {
		String.Init();

		Get( Flow, String );

		Strings.Append( String );
	}

qRR
qRT
qRE
}




