/*
	Copyright (C) 2017 Claude SIMON (http://q37.info/contact/).

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
	along with xdhq. If not, see <http://www.gnu.org/licenses/>.
*/

#include "xdhqjre.h"

#include "registry.h"
#include "treep.h"
#include "xdhp.h"

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
	Registrar.Register( ReturnArgument_ );	// 0
	Registrar.Register( treep::New, treep::Delete, treep::PushTag, treep::PopTag, treep::PutValue, treep::PutAttribute );	// 1 - 6
	Registrar.Register( xdhp::Listen, xdhp::New, xdhp::Set, xdhp::GetAction, xdhp::Execute );	// 7 - 11
	Registrar.Register( xdhp::Alert, xdhp::Confirm, xdhp::SetLayout, xdhp::GetContents, xdhp::SetContents, xdhp::DressWidgets ); //12 - 17
	Registrar.Register( xdhp::AddClasses, xdhp::RemoveClasses, xdhp::ToggleClasses, xdhp::EnableElements, xdhp::DisableElements );	// 18 - 22.
	Registrar.Register( xdhp::SetAttribute, xdhp::GetAttribute, xdhp::RemoveAttribute, xdhp::SetProperty, xdhp::GetProperty );	// 23 - 27.
	Registrar.Register( xdhp::Focus );	// 28

}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;
