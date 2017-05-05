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

#include "records.h"

#include "core.h"
#include "registry.h"
#include "sclfrntnd.h"

namespace {
	E_CDEF( char *, XSLAffix_, "Records" );

	namespace layout_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{
			Session.User.Panel().DumpRecords( Writer );
		}
	}

	namespace casting_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{}
	}
}

void records::SetLayout(
	const char *Id,
	core::rSession &Session )
{
	core::SetElementLayout( Id, XSLAffix_, layout_::Get, Session );
}

void records::SetCasting(
	const char *Id,
	core::rSession &Session )
{
	core::SetElementCasting( Id, XSLAffix_, layout_::Get, Session );
}

void records::Display(
	const char *Id,
	core::rSession &Session )
{
	SetLayout( Id, Session );

	SetCasting( Id, Session );
}

#define AC( name ) BASE_AC( records, name )

AC( DefineNewRecord )
{
	Session.User.Panel().NewRecord();

	main::Display( Session );
	main::SetRecordLayout( Session );
	record::SetFieldsLayout( Session );
}

AC( DefineRecord )
{
	frdinstc::sRecord Record = frdinstc::UndefinedRecord;

	Session.GetNumericalValue( Id, **Record );

	Session.User.Panel().DefineRecord( Record );

	main::Display( Session );
	main::SetRecordLayout( Session );
	record::SetFieldsLayout( Session );
}

