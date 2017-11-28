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

#include "prtcl.h"

#include "registry.h"

#include "csdbns.h"
#include "iof.h"
#include "xpp.h"
#include "lcl.h"
#include "scljre.h"

void scljre::SCLJREInfo( txf::sOFlow &Flow )
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

	namespace {
		csdbns::rListener Listener_;
		sck::socket_ioflow___ Flow_;
	}

	SCLJRE_F( Initialize_ )
	{
	qRH;
		const char *IP = NULL;
		str::wString XML, XSL;
		csdcmn::sVersion Version = csdcmn::UndefinedVersion;
	qRB;
		Listener_.Init( 12345 );
		Flow_.Init( Listener_.GetConnection( IP ), false, sck::NoTimeout );

		if ( ( Version = csdcmn::GetProtocolVersion( prtcl::ProtocolId, Flow_ ) ) != prtcl::ProtocolVersion )
			qRGnr();

		if ( prtcl::GetRequest( Flow_ ) != prtcl::rNext_1 )
			qRGnr();
		Flow_.Dismiss();

		tol::Init( XML, XSL );

		Caller.Get( XML );
		Caller.Get( XSL );

		prtcl::PutAnswer( prtcl::aSetLayout_1, Flow_ );
		prtcl::Put( "Root", Flow_ );
		prtcl::Put( XML, Flow_ );
		prtcl::Put( XSL, Flow_ );

		Flow_.Commit();
	qRR;
	qRT;
	qRE;
		return NULL;
	}
}

void scljre::SCLJRERegister( scljre::sRegistrar &Registrar )
{
	Registrar.Register( ReturnArgument_ );
	Registrar.Register( Initialize_ );
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;
