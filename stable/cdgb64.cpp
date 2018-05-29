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

#define CDGB64__COMPILATION

#include "cdgb64.h"

#include "flx.h"

using namespace cdgb64;

bso::byte__ *cdgb64::CommonEncoding_ = (bso::byte__ *)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
flw::sByte *cdgb64::OriginalEncoding_ = (flw::sByte *)"+/";
flw::sByte *cdgb64::URLEncoding_ = (flw::sByte *)"-_";

bso::byte__ *cdgb64::Decoding_ = (bso::byte__ *)
						"@@@@@@@@@@@@@@@@"
						"@@@@@@@@@@@@@@@c"
						"@@@@@@@@@@@\x3e@\x3e@\x3f"
						"\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d@@@@@@"
						"@\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e"
						"\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19@@@@\x3f"
						"@\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28"
						"\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33@@@@@"
						"@@@@@@@@@@@@@@@@"
						"@@@@@@@@@@@@@@@@"
						"@@@@@@@@@@@@@@@@"
						"@@@@@@@@@@@@@@@@"
						"@@@@@@@@@@@@@@@@"
						"@@@@@@@@@@@@@@@@"
						"@@@@@@@@@@@@@@@@"
						"@@@@@@@@@@@@@@@@";

const str::string_ &cdgb64::Encode(
	const str::string_ &Plain,
	eFlavor Flavor,
	str::string_ &Encoded )
{
qRH
	flx::E_STRING_OFLOW___ SFlow;
	encoding_oflow___ EFlow;
	sdr::row__ Row = qNIL;
qRB
	SFlow.Init( Encoded );
	EFlow.Init( SFlow, Flavor );

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

const str::string_ &cdgb64::Decode(
	const str::string_ &Encoded,
	str::string_ &Plain )
{
qRH
	flx::E_STRING_IFLOW__ SFlow;
	decoding_iflow___ DFlow;
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


