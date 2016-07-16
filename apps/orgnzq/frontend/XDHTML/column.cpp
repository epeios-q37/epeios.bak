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

#include "column.h"

#include "core.h"
#include "registry.h"
#include "sclfrntnd.h"

namespace {

	E_CDEF( char *, XSLAffix_, "Column" );

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

	static void GetContent_(
		const sclrgstry::dRegistry &Registry,
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rContentRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );

		Session.User.DumpColumnBuffer( Rack() );
	qRR
	qRT
	qRE
	}
}

void column::SetLayout(
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

	SetCasting_( Id, Session );

//	Session.SwitchTo( core::fColumn );
qRR
qRT
qRE
}

#define AC( name ) BASE_AC( column, name )

AC( CreateField )
{
qRH
	str::wString Label, Comment;
	frdinstc::sType Type = frdinstc::UndefinedType;
	frdinstc::sNumber Number = frdinstc::UndefinedNumber;
qRB
	Label.Init();
	Session.GetContent( "FieldLabel", Label );

	Comment.Init();
	Session.GetContent( "FieldComment", Comment );

	if ( !Session.GetNumericalContent( "Type", **Type ) )
		sclmisc::ReportAndAbort( "FieldTypeIsRequired" );

	if ( !Session.GetNumericalContent( "Number", **Number ) )
		sclmisc::ReportAndAbort( "FieldNumberIsRequired" );

	Label.StripCharacter( ' ' );
	
	if ( Label.Amount() == 0 )
		sclmisc::ReportAndAbort( "FieldLabelCanNotBeEmpty" );

	Session.User.NewField( Type, Number, Label, Comment );

	main::SetRecordLayout( Session );
	record::SetFieldsLayout( Session );
	fields::SetFieldLayout( Session );
qRR
qRT
qRE
}

