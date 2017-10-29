/*
	Copyright (C) 2017 Claude SIMON (http://q37.info/contact/).

	This file is part of XPPq.

	XPPq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XPPq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XPPq. If not, see <http://www.gnu.org/licenses/>.
*/

#include "xppqznd.h"

#include "registry.h"

#include "iof.h"
#include "xpp.h"
#include "lcl.h"
#include "sclznd.h"

void sclznd::SCLZNDInfo( txf::sOFlow &Flow )
{
	Flow << NAME_MC << " v" << VERSION << txf::nl
		 << txf::pad << "Build : " __DATE__ " " __TIME__ " (" << cpe::GetDescription() << ')';
}

namespace {
	SCLZND_F( ReturnArgument_ )
	{
	qRH
		str::wString Input, Text;
	qRB
		Input.Init();
		Caller.Get( Input );

		Text.Init();
		sclmisc::GetBaseTranslation( "Argument", Text, Input );

		Caller.SetReturnValue( Text );
	qRR
	qRT
	qRE
	}
	SCLZND_F( ProcessingNew_ )
	{
	qRH;
		sclznd::rStreamDriver *Stream = NULL;
	qRB;
		if ( ( Stream = new sclznd::rStreamDriver ) == NULL )
			qRAlc();

		Stream->Init( Caller );

		Caller.SetReturnValue( (bso::sU64)Stream );
	qRR;
		if ( Stream != NULL )
			delete Stream;
	qRT;
	qRE;
	}

	SCLZND_F( ProcessingRead_ )
	{
	qRH;
		bso::sS64 Long = 0, Count = 0;
		flw::sDressedIFlow<> Stream;
		str::wString Buffer;
	qRB;
		Caller.Get( Long );

		if ( Long == 0 )
			qRGnr();

		Stream.Init( *( sclznd::rStreamDriver * )Long );

		Caller.Get( Count );

		Buffer.Init();

		while ( Count-- && !Stream.EndOfFlow() )
			Buffer.Append( Stream.Get() );

		str::ToUpper( Buffer );

		Caller.SetReturnValue( Buffer );
	qRR;
	qRT;
	qRE;
	}

	SCLZND_F( ProcessingEOF_ )
	{
	qRH;
		bso::sS64 Long = 0;
		flw::sDressedIFlow<> Stream;
	qRB;
		Caller.Get( Long );

		if ( Long == 0 )
			qRGnr();

		Stream.Init( *( sclznd::rStreamDriver * )Long );

		Caller.SetReturnValue( Stream.EndOfFlow() );
	qRR;
	qRT;
	qRE;
	}
}

void sclznd::SCLZNDRegister( sclznd::sRegistrar &Registrar )
{
	Registrar.Register( ReturnArgument_ );
	Registrar.Register( ProcessingNew_, ProcessingRead_, ProcessingEOF_ );
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;
