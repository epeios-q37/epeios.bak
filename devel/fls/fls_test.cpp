/*
	'fls_test.cpp' by Claude SIMON (http://zeusw.org/).

	'fls_test.cpp' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
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
ERRProlog
	uys::state__ State = uys::s_Undefined;
	fls::id__ Id = FLS_UNDEFINED_ID;
	bch::bunch_file_manager___ FM1, FM2;
	str::string S1, S2;
ERRBegin
	Id = fls::GetId();

	FM1.Init( "FM1", fil::mReadWrite, true, Id );
	FM2.Init( "FM2", fil::mReadWrite, true, Id );

	State = bch::Plug( S1, FM1 );

	if ( uys::IsError( State ) )
		ERRFwk();

	if ( bch::Plug( S2, FM2 ) != State )
		ERRFwk();

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

ERRErr
ERREnd
	if ( Id != FLS_UNDEFINED_ID  )
		fls::ReleaseId( Id );
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
Generic( argc, argv );

	COut << "Test of library " << FLS_NAME << ' ' << __DATE__" "__TIME__"\n";

ERRFErr
ERRFEnd
ERRFEpilog
	return 0;
}
