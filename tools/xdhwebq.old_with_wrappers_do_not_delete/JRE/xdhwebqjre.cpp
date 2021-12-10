/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHWebQ.

	XDHWebQ is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHWebQ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHWebQ. If not, see <http://www.gnu.org/licenses/>.
*/

#include "xdhwebqjre.h"

#include "registry.h"

#include "iof.h"
#include "xpp.h"
#include "lcl.h"
#include "scljre.h"

void scljre::SCLJREInfo( txf::sWFlow &Flow )
{
	Flow << NAME_MC << " v" << VERSION << txf::nl
		 << txf::pad << "Build : " __DATE__ " " __TIME__ " (" << cpe::GetDescription() << ')';
}

namespace {
	SCLJRE_F( ReturnArgument_ )
	{
		scljre::sJObject Return;
	qRH
		str::wString Input, Text;
	qRB
		Input.Init();
		Caller.Get( Input );

		Text.Init();
		sclmisc::GetBaseTranslation( "Argument", Text, Input );

		Return = scljre::String( Text );
	qRR
	qRT
	qRE
		return Return;
	}
}

void scljre::SCLJRERegister( scljre::sRegistrar &Registrar )
{
	Registrar.Register( ReturnArgument_ );
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;
