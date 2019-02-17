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

#include "flw.h"

#include "err.h"
#include "cio.h"
#include "mtk.h"

using cio::CIn;
using cio::COut;
using cio::CErr;

typedef bso::sU16 sId;

void Log_( sId Id )
{
qRH;
	txf::rWFlow Flow;
qRB;
	Flow.Init( cio::GetOutDriver() );

	Flow << (int)Id << txf::tab << txf::commit;
qRR;
qRT;
qRE;
}

struct sData
{
	sId Id;
};

void Routine_(
	sData &Data,
	mtk::gBlocker &Blocker )
{
	sId Id = Data.Id;

	Blocker.Release();

	Log_( Id );

	while ( true ) {
		tht::Defer();
//		Log_( Id );
	}
}

void Launch_( void )
{
	static sId Id =  0;

	sData Data;

	Data.Id = Id++;

	mtk::Launch<sData>( Routine_, Data );
}

void Generic( int argc, char *argv[] )
{
qRH
	int i = 5000;
qRB
	while ( i-- )
		Launch_();

	tht::Defer( 2000 );

	while ( true ) {
		tht::Defer( 1000 );
	}
qRR
qRT
qRE
}

namespace {
	void ErrFinal_( void )
	{
		if ( ERRType != err::t_Abort ) {
			err::buffer__ Buffer;

			const char *Message = err::Message( Buffer );

			ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.

			cio::COut << txf::commit;
			cio::CErr << txf::nl << txf::tab << "{ " << Message << " }" << txf::nl << txf::commit;
		} else
			ERRRst();
	}
}

int main( int argc, char *argv[] )
{
	int ExitValue = EXIT_SUCCESS;
qRFH
qRFB
	cio::Initialize( cio::t_Default );

	COut << "Test of library " << FLW_NAME << ' ' << __DATE__ " " __TIME__ "\n";

	Generic( argc, argv );
qRFR
	ExitValue = EXIT_FAILURE;
qRFT
qRFE( ErrFinal_() )
	return ExitValue;
}