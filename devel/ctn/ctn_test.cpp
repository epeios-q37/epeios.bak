/*
	'ctn_test.cpp' by Claude SIMON (http://zeusw.org/).

	'ctn_test.cpp' is part of the Epeios framework.

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

#include "ctn.h"

#include "err.h"
#include "cio.h"

#include "str.h"

using cio::CIn;
using cio::COut;
using cio::CErr;

typedef str::string_ datum_;
E_AUTO( datum );

typedef ctn::E_MCONTAINER_( datum_ ) data_;
E_AUTO( data );

typedef ctn::E_CONTAINER_( data_ ) data_cluster_;
E_AUTO( data_cluster );

void Generic( int argc, char *argv[] )
{
ERRProlog
	datum Datum;
	data Data;
	data_cluster DataCluster;
ERRBegin
	Datum.Init( "coucou" );

	Data.Init();

	Data.Dynamics.AStorage.DisplayStructure( cio::COut );
	cio::COut << txf::nl;

	Data.Append( Datum );

	Data.Dynamics.AStorage.DisplayStructure( cio::COut );
	cio::COut << txf::nl;

	DataCluster.Init();

	DataCluster.Dynamics.AStorage.DisplayStructure( cio::COut );
	cio::COut << txf::nl;

	DataCluster.Append( Data );

	DataCluster.Dynamics.AStorage.DisplayStructure( cio::COut );
	cio::COut << txf::nl;
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
	int ExitValue = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	COut << "Test of library " << CTN_NAME << ' ' << __DATE__" "__TIME__"\n";

	Generic( argc, argv );
ERRFErr
ERRFEnd
ERRFEpilog
	return ExitValue;
}
