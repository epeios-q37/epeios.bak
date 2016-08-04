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

#include "field.h"

#include "core.h"
#include "registry.h"
#include "sclfrntnd.h"

namespace {

	E_CDEF( char *, XSLAffix_, "Field" );

	void GetContext_(
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rContextRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );

		if ( Session.User.Panel().IsEntryDraggingInProgress() )
			Rack().PutValue( "InProgress", "Dragging" );
	qRR
	qRT
	qRE
	}

	static void GetContent_(
		const sclrgstry::dRegistry &Registry,
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rContentRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );

		Session.User.Panel().DumpFieldBuffer( Rack );
	qRR
	qRT
	qRE
	}
}

void field::SetCasting(
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

void field::SetLayout(
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

	SetCasting( Id, Session );

//	Session.SwitchTo( core::fframe );
qRR
qRT
qRE
}

#define AC( name ) BASE_AC( field, name )

AC( DefineNewEntry )
{
	Session.User.Panel().DefineEntry( frdinstc::UndefinedEntry );
	fields::SetFieldLayout( Session );
}

AC( DefineEntry )
{
	frdinstc::sEntry Entry = frdinstc::UndefinedEntry;

	Session.GetNumericalContent( Id, **Entry );

	Session.User.Panel().DefineEntry( Entry );

	fields::SetFieldLayout( Session );
}

namespace {
	inline frdinstc::sEntry GetContent_(
		core::rSession &Session,
		const char *Id )
	{
		frdinstc::sEntry Entry = frdinstc::UndefinedEntry;

		Session.GetNumericalContent( Id, **Entry );

		return Entry;
	}
}

AC( DragEntry )
{
	Session.User.Panel().DragEntry( GetContent_( Session, Id ) );
	fields::SetFieldCasting( Session );
}

AC( DropEntry )
{
	Session.User.Panel().DropEntry( GetContent_( Session, Id ) );
	fields::SetFieldLayout( Session );
}

AC( EndEntryDragging )
{
	Session.User.Panel().EndEntryDragging();
	fields::SetFieldCasting( Session );
}
