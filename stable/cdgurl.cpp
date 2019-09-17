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

#define CDGURL_COMPILATION_

#include "cdgurl.h"

#include "flx.h"

using namespace cdgurl;

const str::string_ &cdgurl::Encode(
	const str::string_ &Plain,
	str::string_ &Encoded )
{
qRH;
	flx::rStringRDriver In;
	flx::rStringWDriver Out;
	rURLEncoderRDriver Encoder;
qRB;
	In.Init( Plain );
	Out.Init( Encoded );

	Encoder.Init( In );

	fdr::Copy( Encoder, Out );
qRR;
qRT;
qRE;
	return Encoded;
}

const str::string_ &cdgurl::Decode(
	const str::string_ &Encoded,
	str::string_ &Plain )
{
qRH
	flx::rStringRDriver In;
	flx::rStringWDriver Out;
	rURLDecoderRDriver Decoder;
qRB
	In.Init( Encoded );
	Out.Init( Plain );

	Decoder.Init( In );

	fdr::Copy( Decoder, Out );
qRR
qRT
qRE
	return Plain;
}

