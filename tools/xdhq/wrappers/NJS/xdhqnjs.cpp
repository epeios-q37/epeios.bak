/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHq.

	XDHq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHq. If not, see <http://www.gnu.org/licenses/>.
*/

# include "xdhqnjs.h"

#include "registry.h"
#include "xdhp.h"
#include "treep.h"

#include "sclmisc.h"
#include "sclnjs.h"

void sclnjs::SCLNJSInfo( txf::sWFlow &Flow )
{
	Flow << NAME_MC << " v" << VERSION << txf::nl
		 << txf::pad << "Build : " __DATE__ " " __TIME__ " (" << cpe::GetDescription() << ')';
}

namespace {
	SCLNJS_F( ReturnArgument_ )
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

void sclnjs::SCLNJSRegister( sclnjs::sRegistrar &Registrar )
{
	Registrar.Register( ReturnArgument_ );
	Registrar.Register( treep::New, treep::Delete, treep::PushTag, treep::PopTag, treep::PutValue, treep::PutAttribute );	// 1 - 6
	Registrar.Register( xdhp::Register, xdhp::Listen );	// 7 - 8
	Registrar.Register( xdhp::Alert, xdhp::Confirm, xdhp::SetLayout, xdhp::GetContents, xdhp::SetContents, xdhp::DressWidgets ); // 9 - 14
	Registrar.Register( xdhp::AddClasses, xdhp::RemoveClasses, xdhp::ToggleClasses, xdhp::EnableElements, xdhp::DisableElements );	// 15 - 19.
	Registrar.Register( xdhp::GetAttribute, xdhp::SetAttribute, xdhp::GetProperty, xdhp::SetProperty );	// 20 - 23.
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;
// const char *scln4a::SCLN4AProductVersion = VERSION;
