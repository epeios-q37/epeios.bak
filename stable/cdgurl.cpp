/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

#define CDGURL__COMPILATION

#include "cdgurl.h"

#include "flx.h"

using namespace cdgurl;

const str::string_ &cdgurl::Encode(
	const str::string_ &Plain,
	str::string_ &Encoded )
{
qRH
	flx::E_STRING_OFLOW___ SFlow;
	rEncodingOFlow EFlow;
	sdr::row__ Row = qNIL;
qRB
	SFlow.Init( Encoded );
	EFlow.Init( SFlow );

	Row = Plain.First();

	while( Row != qNIL ) {
		EFlow.Put( Plain( Row ) );

		Row = Plain.Next( Row );
	}
qRR
qRT
qRE
	return Encoded;
}

const str::string_ &cdgurl::Decode(
	const str::string_ &Encoded,
	str::string_ &Plain )
{
qRH
	flx::E_STRING_IFLOW__ SFlow;
	rDecodingIflow DFlow;
qRB
	SFlow.Init( Encoded );
	DFlow.Init( SFlow );

	while ( !DFlow.EndOfFlow() )
		Plain.Append( DFlow.Get() );
qRR
qRT
qRE
	return Plain;
}

