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

	namespace layout_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{
			Session.User.Panel().DumpColumnBuffer( Writer );
		}
	}

	namespace casting_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{}
	}
}

void column::SetLayout(
	const char *Id,
	core::rSession &Session )
{
	core::SetElementLayout( Id, XSLAffix_, layout_::Get, Session );
}

void column::SetCasting(
	const char *Id,
	core::rSession &Session )
{
	core::SetElementCasting( Id, XSLAffix_, casting_::Get, Session );
}

void column::Display(
	const char *Id,
	core::rSession &Session )
{
	SetLayout( Id, Session );

	SetCasting( Id, Session );
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
	Session.GetValue( "FieldLabel", Label );

	Comment.Init();
	Session.GetValue( "FieldComment", Comment );

	if ( !Session.GetNumericalValue( "Type", **Type ) )
		sclmisc::ReportAndAbort( "FieldTypeIsRequired" );

	if ( !Session.GetNumericalValue( "Number", **Number ) )
		sclmisc::ReportAndAbort( "FieldNumberIsRequired" );

	Label.StripCharacter( ' ' );
	
	if ( Label.Amount() == 0 )
		sclmisc::ReportAndAbort( "FieldLabelCanNotBeEmpty" );

	Session.User.Panel().NewField( Type, Number, Label, Comment );

	main::SetRecordLayout( Session );
	record::SetFieldsLayout( Session );
	fields::SetFieldLayout( Session );
qRR
qRT
qRE
}

