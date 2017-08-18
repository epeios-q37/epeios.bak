/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

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

# include "xppqnjs.h"

#include "registry.h"

#include "sclmisc.h"
#include "scln4a.h"

void scln4a::SCLN4AInfo( txf::sOFlow &Flow )
{
	Flow << NAME_MC << " v" << VERSION << txf::nl
		 << txf::pad << "Build : " __DATE__ " " __TIME__ " (" << cpe::GetDescription() << ')';
}

namespace {
	void ReturnArgument_( scln4a::sCaller &Caller )
	{
	qRH
		str::wString Input, Text;
	qRB
		Input.Init();
		Caller.GetArgument( Input );

		Text.Init();
		sclmisc::GetBaseTranslation( "Argument", Text, Input );

		Caller.SetReturnValue( Text );
	qRR
	qRT
	qRE
	}
}

void scln4a::SCLN4ARegister( scln4a::sRegistrar &Registrar )
{
	Registrar.Register( ReturnArgument_ );
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;
// const char *scln4a::SCLN4AProductVersion = VERSION;
