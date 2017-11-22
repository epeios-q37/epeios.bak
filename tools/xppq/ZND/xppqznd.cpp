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

#include "parser.h"
#include "processor.h"
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
	namespace base_ {
		SCLZND_F( ReturnArgument )
		{
		qRH;
			str::wString Input, Text;
		qRB;
			Input.Init();
			Caller.Get( Input );

			Text.Init();
			sclmisc::GetBaseTranslation( "Argument", Text, Input );

			Caller.SetReturnValue( Text );
		qRR;
		qRT;
		qRE;
		}
	}
}

void sclznd::SCLZNDRegister( sclznd::sRegistrar &Registrar )
{
	Registrar.Register( base_::ReturnArgument );
	Registrar.Register( processor::New, processor::EndOfFlow, processor::Read, processor::Delete );
	Registrar.Register( parser::New, parser::Parse, parser::TagName, parser::AttributeName, parser::Value, parser::Delete );
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;
