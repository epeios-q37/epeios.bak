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
		{
			if ( Session.User.Panel().IsEntryDraggingInProgress() )
				Writer.PutValue( "InProgress", "Dragging" );
		}
	}
}

void field::SetLayout(
	const char *Id,
	core::rSession &Session )
{
	core::SetElementLayout( Id, XSLAffix_, layout_::Get, Session );
}

void field::SetCasting(
	const char *Id,
	core::rSession &Session )
{
	core::SetElementCasting( Id, XSLAffix_, casting_::Get, Session );
}

void field::Display(
	const char *Id,
	core::rSession &Session )
{
	SetLayout( Id, Session );

	SetCasting( Id, Session );
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

	Session.GetNumericalValue( Id, **Entry );

	Session.User.Panel().DefineEntry( Entry );

	fields::SetFieldLayout( Session );
}

namespace {
	inline frdinstc::sEntry GetContent_(
		core::rSession &Session,
		const char *Id )
	{
		frdinstc::sEntry Entry = frdinstc::UndefinedEntry;

		Session.GetNumericalValue( Id, **Entry );

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
