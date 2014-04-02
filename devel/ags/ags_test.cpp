/*
	'ags_test.cpp' by Claude SIMON (http://zeusw.org/).

	'ags_test.cpp' is part of the Epeios framework.

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

#include "ags.h"

#include "err.h"
#include "cio.h"
#include "ctn.h"
#include "str.h"


using cio::CIn;
using cio::COut;
using cio::CErr;

using cio::CIn;
using cio::COut;
using cio::CErr;

#define DS	Storage.DisplayStructure( COut );COut << txf::commit

void Test1( void )
{
ERRProlog
	ags::E_ASTORAGE Storage;
	ags::descriptor__ D1, D2, D3, D4, D5;
ERRBegin
	Storage.Init();
	DS;

	D1 = Storage.Allocate( 2 );
	DS;
	D2 = Storage.Allocate( 2000 );
	DS;
	D3 = Storage.Allocate( 3000 );
	DS;
	D4 = Storage.Allocate( 4000 );
	DS;
	D5 = Storage.Allocate( 5000 );
	DS;
	D3 = Storage.Reallocate( D3, 4000 );
	DS;
	D2 = Storage.Reallocate( D2, 2500 );
	DS;
	Storage.Free( D2 );
	DS;
	Storage.Free( D4 );
	DS;
ERRErr
ERREnd
ERREpilog
}

typedef ctn::E_MCONTAINER_( str::string_ ) strings_;
E_AUTO( strings );

sdr::row__ Locate(
	const str::string_ &Name,
	const strings_ &Strings )
{
	ctn::E_CMITEM( str::string_ ) String;
	sdr::row__ Row = Strings.First();

	String.Init( Strings );

	while ( ( Row!= E_NIL ) && ( String( Row ) != Name ) )
		Row = Strings.Next( Row );

	if ( Row == E_NIL )
		ERRFwk();

	return Row;
}


void Test2( void )
{
ERRProlog
	ags::E_ASTORAGE Storage;
	strings Strings1, Strings2;
ERRBegin
	Storage.Init();
	Strings1.plug( Storage );
	Strings2.plug( Storage );

	Strings1.Init();
	Strings2.Init();

	Strings1.Append( str::string( "toto" ) );
	DS;
	Strings2.Append( str::string( "toto" ) );
	DS;

	Strings1.Append( str::string( "tata" ) );
	DS;
	Strings2.Append( str::string( "tata" ) );
	DS;

	Strings1.Append( str::string( "titi" ) );
	DS;
	Strings2.Append( str::string( "titi" ) );
	DS;

	Strings1.Remove( Locate( str::string( "toto" ), Strings1 ) );
	DS;
ERRErr
ERREnd
ERREpilog
}



int main( int argc, char *argv[] )
{
	int ExitValue = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	COut << "Test of library " << AGS_NAME << ' ' << __DATE__" "__TIME__"\n";

	Test2();
ERRFErr
	ExitValue = EXIT_FAILURE;
ERRFEnd
ERRFEpilog
	return ExitValue;
}
