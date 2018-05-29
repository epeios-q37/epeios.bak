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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fls.h"

#include "err.h"
#include "cio.h"

#include "str.h"

using cio::CIn;
using cio::COut;
using cio::CErr;

void Generic( int argc, char *argv[] )
{
qRH
	uys::state__ State = uys::s_Undefined;
	fls::id__ Id = FLS_UNDEFINED_ID;
	bch::bunch_file_manager___ FM1, FM2;
	str::string S1, S2;
qRB
	Id = fls::GetId();

	FM1.Init( "FM1", fil::mReadWrite, true, Id );
	FM2.Init( "FM2", fil::mReadWrite, true, Id );

	State = bch::Plug( S1, FM1 );

	if ( uys::IsError( State ) )
		qRFwk();

	if ( bch::Plug( S2, FM2 ) != State )
		qRFwk();

	if ( State == uys::sAbsent ) {
		cio::COut << "CREATION !!!" << txf::nl << txf::commit;
		S1.Append( 'A' );
		S2.Append( 'B' );
		S1.Append( 'C' );
	}

	cio::COut << S1 << txf::pad;
	cio::COut << S1 << txf::pad;
	cio::COut << S1 << txf::pad;
	cio::COut << S1 << txf::pad;
	cio::COut << S1 << txf::pad;
	cio::COut << S1 << txf::pad;
	cio::COut << S1 << txf::pad;
	cio::COut << S1 << txf::pad;
	cio::COut << S1 << txf::pad;
	cio::COut << S2 << txf::pad;
	cio::COut << S2 << txf::pad;
	cio::COut << S2 << txf::pad;
	cio::COut << S2 << txf::pad;
	cio::COut << S2 << txf::pad;
	cio::COut << S2 << txf::pad;
	cio::COut << S1 << txf::pad;
	cio::COut << S1 << txf::pad;
	cio::COut << S1 << txf::pad;
	cio::COut << S1 << txf::pad;
	cio::COut << S1 << txf::pad;
	cio::COut << S1 << txf::pad;
	cio::COut << S1 << txf::pad;
	cio::COut << S1 << txf::pad;
	cio::COut << txf::nl << txf::commit;

qRR
qRT
	if ( Id != FLS_UNDEFINED_ID  )
		fls::ReleaseId( Id );
qRE
}

int main( int argc, char *argv[] )
{
qRFH
qRFB
Generic( argc, argv );

	COut << "Test of library " << FLS_NAME << ' ' << __DATE__" "__TIME__"\n";

qRFR
qRFT
qRFE
	return 0;
}
