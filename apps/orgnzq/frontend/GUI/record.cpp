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

#include "record.h"

#include "core.h"
#include "registry.h"
#include "sclfrntnd.h"

E_CDEF(char *, record::FieldsFrameId_, "Fields");

namespace {
	E_CDEF( char *, XSLAffix_, "Record" );

	E_CDEF( char *, FieldsFrameId_, "Fields" );
	E_CDEF( char *, ColumnFrameId_, "Column" );

	namespace layout_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{
			Session.User.Panel().DumpFieldBuffer( Writer );
		}
	}

	namespace casting_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{}
	}
}

void record::SetLayout(
	const char *Id,
	core::rSession &Session )
{
	core::SetElementLayout( Id, XSLAffix_, layout_::Get, Session );
}

void record::SetCasting(
	const char *Id,
	core::rSession &Session )
{
	core::SetElementCasting( Id, XSLAffix_, casting_::Get, Session );
}

void record::Display(
	const char *Id,
	core::rSession &Session )
{
	SetLayout( Id, Session );

	SetCasting( Id, Session );
}

void record::SetFieldsCasting( core::rSession &Session )
{
	fields::SetCasting( FieldsFrameId_, Session );
}

void record::SetFieldsLayout( core::rSession &Session )
{
	fields::Display( FieldsFrameId_, Session );
}

void record::SetColumnLayout( core::rSession &Session )
{
	column::Display( ColumnFrameId_, Session );
}

#define AC( name ) BASE_AC( record, name )

AC( DefineNewField )
{
	Session.User.Panel().NewColumn();

	main::SetRecordLayout( Session );
	record::SetFieldsLayout( Session );
	record::SetColumnLayout( Session );
}

AC( BackToList )
{
	Session.User.Panel().BackToList();
	main::Display( Session, true );
}

