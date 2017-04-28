/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Frame.h"

#include "core.h"
#include "registry.h"
#include "sclfrntnd.h"


namespace {
	qCDEF( char *, XSLAffix_, "Frame" );
}

namespace layout_ {
	static void GetLayout(
		const sclrgstry::registry_ &Registry,
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rLayoutRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );
	qRR
	qRT
	qRE
	}
}

void frame::Display(
	const char *Id,
	core::rSession &Session )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init(); 
	layout_::GetLayout( Session.Registry(), Session, XML );

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( registry::definition::XSLLayoutFile, XSLAffix_ ), Session.Registry(), XSL );

	Session.FillElement( Id, XML, XSL );

	SetCasting( Id, Session );
qRR
qRT
qRE
}

namespace casting_ {
	void Get(
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rCastingRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );
	qRR
	qRT
	qRE
	}
}

void frame::SetCasting(
	const char *Id,
	core::rSession &Session )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init();
	casting_::Get( Session,  XML );

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags(rgstry::tentry___( registry::definition::XSLCastingFile, XSLAffix_ ), Session.Registry() , XSL );

	Session.FillElementCastings( Id, XML, XSL );
qRR
qRT
qRE
}


#define AC( name ) BASE_AC( frame, name )

AC( Template )
{
	Session.AlertT( "Template" );
}

