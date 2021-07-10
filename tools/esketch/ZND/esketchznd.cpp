/*
	Copyright (C) 2021 Claude SIMON (http://q37.info/contact/).

	This file is part of eSketch.

	eSketch is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	eSketch is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with eSketch. If not, see <http://www.gnu.org/licenses/>.
*/

#include "esketchznd.h"

#include "registry.h"

#include "iof.h"
#include "xpp.h"
#include "lcl.h"
#include "sclznd.h"

SCLI_DEF( esketchznd, NAME_LC, "eSketch" );

void sclznd::SCLZNDInfo( txf::sWFlow &Flow )
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

	SCLZND_F( TestStrings_ )
	{
	qRH;
		str::wStrings Strings;
		sdr::sRow Row = qNIL;
	qRB;
		Strings.Init();

		Caller.Get( Strings );

		Row = Strings.First();

		while ( Row != qNIL ) {
			str::ToUpper( Strings( Row ) );

			Row = Strings.Next( Row );
		}

		Caller.SetReturnValue( Strings );
	qRR;
	qRT;
	qRE;
	}
}

const scli::sInfo &sclznd::SCLZNDRegister( sclznd::sRegistrar &Registrar )
{
	Registrar.Register( ReturnArgument_, TestStrings_ );

	return esketchznd::Info;
}
