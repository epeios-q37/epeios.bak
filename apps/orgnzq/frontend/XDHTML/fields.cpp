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

#include "fields.h"

#include "core.h"
#include "registry.h"
#include "sclfrntnd.h"
#include "field.h"

E_CDEF( char *, FieldFrameId_, "Field" );

namespace {
	E_CDEF( char *, XSLAffix_, "Fields" );

	void GetContext_(
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rContextRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );
	qRR
	qRT
	qRE
	}

	void SetCasting_(
		const char *Id,
		core::rSession &Session )
	{
	qRH
		str::string XML, XSL;
	qRB
		XML.Init();
		GetContext_( Session,  XML );

		XSL.Init();
		sclxdhtml::LoadXSLAndTranslateTags(rgstry::tentry___( registry::definition::XSLCastingFile, XSLAffix_ ), Session.Registry() , XSL );

		Session.FillElementCastings( Id, XML, XSL );
	qRR
	qRT
	qRE
	}

	void GetContent_(
		const sclrgstry::registry_ &Registry,
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rContentRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );

		Session.User.DumpCurrentRecordColumns( Rack );
		Session.User.DumpCurrentRecordFields( Rack );
	qRR
	qRT
	qRE
	}
}


void fields::SetLayout(
	const char *Id,
	core::rSession &Session )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init(); 
	GetContent_( Session.Registry(), Session, XML );

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( registry::definition::XSLLayoutFile, XSLAffix_ ), Session.Registry(), XSL );

	Session.FillElement( Id, XML, XSL );
qRR
qRT
qRE
}

void fields::SetFieldLayout( core::rSession &Session )
{
	field::SetLayout( FieldFrameId_, Session );
}

#define AC( name ) BASE_AC( fields, name )

AC( CreateField )
{
qRH
qRB
	qRVct();
qRR
qRT
qRE
}

AC( DefineField )
{
	frdinstc::sField Field = frdinstc::UndefinedField;
	
	Session.GetNumericalContent( Id, **Field );

	Session.User.DefineField( Field );

	main::SetRecordLayout( Session );
	record::SetFieldsLayout( Session );
	fields::SetFieldLayout( Session );
}

namespace {
	inline frdinstc::sField GetContent_(
		core::rSession &Session,
		const char *Id )
	{
		frdinstc::sField Field = frdinstc::UndefinedField;

		Session.GetNumericalContent( Id, **Field );

		return Field;
	}
}

AC( DragField )
{
	Session.User.DragField( GetContent_( Session, Id ) );
}

AC( DropField )
{
	Session.User.DropField( GetContent_( Session, Id ) );
	fields::SetFieldLayout( Session );
}

AC( EndFieldDragging )
{
	Session.User.EndFieldDragging();
}

