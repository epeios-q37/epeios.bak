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

#include "main.h"

#include "core.h"
#include "registry.h"
#include "sclfrntnd.h"

namespace {
	E_CDEF( char *, XSLAffix_, "Main" );

	E_CDEF( char *, RecordsFrameId_, "Records" );
	E_CDEF( char *, RecordFrameId_, "Record" );

	namespace layout_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{
			Session.User.DumpPanels( Writer );
		}
	}

	namespace casting_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{
			if ( Session.User.IsPanelDraggingInProgress() )
				Writer.PutValue( "InProgress", "Dragging" );
		}
	}
}

void main::SetLayout( core::rSession &Session )
{
	core::SetDocumentLayout( XSLAffix_, layout_::Get, Session );
}

void main::SetCasting( core::rSession &Session )
{
	core::SetDocumentCasting( XSLAffix_, casting_::Get, Session );
}

void main::Display(
	core::rSession &Session,
	bso::sBool Refresh )
{
	SetLayout( Session );

	SetCasting( Session );

	if ( Refresh ) {
		switch ( Session.User.Panel().Focus() ) {
		case frdinstc::tColumn:
			main::SetRecordLayout( Session );
			record::SetFieldsLayout( Session );
			record::SetColumnLayout( Session );
			break;
		case frdinstc::tField:
			main::SetRecordLayout( Session );
			record::SetFieldsLayout( Session );
			fields::SetFieldLayout( Session );
			break;
		case frdinstc::tRecord:
			main::SetRecordLayout( Session );
			record::SetFieldsLayout( Session );
			break;
		case frdinstc::tRecords:
			main::SetRecordsLayout( Session );
			break;
		default:
			qRGnr();
			break;
		}
	}

	Session.SwitchTo( core::pMain );
}

void main::SetRecordsLayout( core::rSession &Session )
{
	records::Display( RecordsFrameId_, Session );
}

void main::SetRecordLayout( core::rSession &Session )
{
	record::Display( RecordFrameId_, Session );
}

#define AC( name ) BASE_AC( main, name )

AC( NewPanel )
{
	Session.User.NewPanel();

	Display( Session, true );
}

AC( SelectPanel )
{
	frdinstc::sPPos Pos = qNIL;

	Session.GetNumericalValue( Id, *Pos );

	Session.User.SelectPanel( Pos );

	Display( Session, true );
}

AC( DragPanel )
{
	frdinstc::sPPos Pos = qNIL;

	Session.GetNumericalValue( Id, *Pos );

	Session.User.DragPanel( Pos );

	SetCasting( Session );
}

AC( DropPanel )
{
	frdinstc::sPPos Pos = qNIL;

	Session.GetNumericalValue( Id, *Pos );

	Session.User.DropPanel( Pos );

	SetCasting( Session );
}

AC( EndPanelDragging )
{
	Session.User.EndPanelDragging();

	Display( Session, true );
}

AC( Template )
{
}
