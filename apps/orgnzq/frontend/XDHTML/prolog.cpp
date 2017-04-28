/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "prolog.h"

#include "core.h"
#include "registry.h"

#include "xdhdws.h"

#include "sclfrntnd.h"

namespace {
	E_CDEF(char *, XSLAffix_, "Prolog" );

	void GetCasting_(
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rCastingRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );

		sclxdhtml::prolog::GetContext( Session, Rack );
	qRR
	qRT
	qRE
	}

	void SetCasting_( core::rSession &Session )
	{
	qRH
		str::string XML, XSL;
	qRB
		XML.Init();
		GetCasting_( Session,  XML );

		XSL.Init();
		sclxdhtml::LoadXSLAndTranslateTags(rgstry::tentry___( registry::definition::XSLCastingFile, XSLAffix_ ), sclxdhtml::GetRegistry(), XSL );	// Outside session, so we use the global registry...

		Session.FillDocumentCastings( XML, XSL );
	qRR
	qRT
	qRE
	}

	void GetLayout_(
		const sclrgstry::dRegistry &Registry,
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rLayoutRack Rack;
		TOL_CBUFFER___ Buffer;
	qRB
		Rack.Init( XSLAffix_, XML, Session );

		sclxdhtml::prolog::GetContent( Session, Rack );
	qRR
	qRT
	qRE
	}
}

void prolog::Display( core::rSession &Session )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init();
	GetLayout_( sclxdhtml::GetRegistry(), Session, XML );	// Outside session, so we use the global registry...

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( registry::definition::XSLLayoutFile, XSLAffix_ ), sclxdhtml::GetRegistry(), XSL );	// Outside session, so we use the global registry...

	Session.FillDocument( XML, XSL );

	SetCasting_( Session );
	Session.SwitchTo( core::pProlog );
qRR
qRT
qRE
}

#define AC( name ) BASE_AC( prolog, name )

AC( SwitchProjectType )
{
	SetCasting_( Session );
}

AC( DisplayProjectFilename )
{
	sclxdhtml::prolog::DisplaySelectedProjectFilename( Session, Id );
}

AC( LoadProject )
{
	sclxdhtml::prolog::LoadProject( Session );

	login::Display( Session );
}
