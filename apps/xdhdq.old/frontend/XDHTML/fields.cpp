/*
	'fields.cpp' by Claude SIMON (http://q37.info/contact/).

	 This file is part of 'xdhdq' software.

    'xdhdq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhdq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhdq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "fields.h"

#include "core.h"
#include "registry.h"
#include "sclfrntnd.h"

using namespace frdinstc;

namespace {
	E_CDEF( char *, XSLAffix_, "Fields" );

	void GetContext_(
		core::session___ &Session,
		str::string_ &XML )
	{
	qRH
		base::context_rack___ Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );
	qRR
	qRT
	qRE
	}

	void SetCasting_(
		const char *Id,
		core::session___ &Session )
	{
	qRH
		str::string XML, XSL;
	qRB
		XML.Init();
		GetContext_( Session,  XML );

		XSL.Init();
		sclxdhtml::LoadXSLAndTranslateTags(rgstry::tentry___( registry::XSLCastingFile, XSLAffix_ ), Session.Registry() , XSL );

		Session.FillElementCastings( Id, XML, XSL );
	qRR
	qRT
	qRE
	}

	void GetContent_(
		const sclrgstry::registry_ &Registry,
		core::session___ &Session,
		str::string_ &XML )
	{
	qRH
		base::content_rack___ Rack;
		int i = 0;
	qRB
		Rack.Init( XSLAffix_, XML, Session );

		Rack().PushTag("Fields" );

		if ( Session.User.EditableField() != f_Undefined )
			Rack().PutAttribute("Editable", GetLabel( Session.User.EditableField() ) );

		i = f_amount;

		while ( i-- ) {
			Rack().PushTag("Field" );

			Rack().PutAttribute("id", GetLabel( (field__)i ) );

			Rack().PutValue(Session.User.FieldContent((field__ )i) );

			Rack().PopTag();
		}

		Rack().PopTag();
	qRR
	qRT
	qRE
	}
}

void fields::SetLayout(
	const char *Id,
	core::session___ &Session )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init(); 
	GetContent_( Session.Registry(), Session, XML );

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( registry::XSLLayoutFile, XSLAffix_ ), Session.Registry(), XSL );

	Session.FillElement( Id, XML, XSL );

	SetCasting_( Id, Session );

	Session.SwitchTo( core::pMain );

	if ( Session.User.EditableField() != f_Undefined )
		Session.Focus(GetLabel( Session.User.EditableField() ) );
qRR
qRT
qRE
}

BASE_AC( fields::edit__ )
{
	Session.User.EditableField() = GetField( str::string( Id ) );

	main::RefreshFields( Session );
}

BASE_AC( fields::refresh__ )
{
	main::RefreshFields( Session );
}

