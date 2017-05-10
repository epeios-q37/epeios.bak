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

	namespace layout_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{
			Session.User.Panel().DumpCurrentRecordColumns( Writer );
			Session.User.Panel().DumpCurrentRecordFields( Writer );
			Session.User.Panel().DumpColumnBuffer( Writer );
		}
	}

	namespace casting_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{
			if ( Session.User.Panel().IsFieldDraggingInProgress() )
				Writer.PutValue( "InProgress", "Dragging" );
		}
	}
}

void fields::SetLayout(
	const char *Id,
	core::rSession &Session )
{
	core::SetElementLayout( Id, XSLAffix_, layout_::Get, Session );
}

void fields::SetCasting(
	const char *Id,
	core::rSession &Session )
{
	core::SetElementCasting( Id, XSLAffix_, casting_::Get, Session );
}

namespace {
	class sContent
	: public xdhcmn::cContent
	{
	private:
		qRMV( core::rSession, S_, Session_ );
		void Convert_(
			const str::dString &Tag,
			fbltyp::sId &Field,
			fbltyp::sId &Entry )
		{
			sdr::sRow Error = qNIL;

			Tag.ToNumber( *Field, &Error );

			if ( Error == qNIL )
				qRGnr();

			Error = Tag.Next( Error );

			if ( Error == qNIL )
				qRGnr();

			Tag.ToNumber( *Entry, Error );
		}
		void Convert_(
			const str::dStrings &Tags,
			fbltyp::dIds &Fields,
			fbltyp::dIds &Entries )
		{
			sdr::sRow Row = Tags.First();

			fbltyp::sId Field = fbltyp::UndefinedId, Entry = fbltyp::UndefinedId;

			while ( Row != qNIL ) {
				Convert_( Tags( Row ), Field, Entry );

				Fields.Append( Field );
				Entries.Append( Entry );

				Row = Tags.Next( Row );
			}
		}
	protected:
		virtual void XDHCMNGetContents(
			const str::dStrings &Tags,
			str::dStrings &Contents ) override
		{
		qRH
			fbltyp::wIds Fields, Entries;
		qRB
			tol::Init( Fields, Entries );

			Convert_( Tags, Fields, Entries );

			S_().GetEntries( Fields, Entries, Contents );
		qRR
		qRT
		qRE
		}
	public:
		void reset( bso::sBool = true )
		{
			Session_ = NULL;
		}
		qCVDTOR( sContent );
		void Init( core::rSession &Session )
		{
			Session_ = &Session;
		}
	};
}

void fields::Display(
	const char *Id,
	core::rSession &Session )
{
	sContent Content;

	SetLayout( Id, Session );

	SetCasting( Id, Session );

	Content.Init( Session );
	Session.SetContents( "Root", Content );
}

void fields::SetFieldLayout( core::rSession &Session )
{
	field::Display( FieldFrameId_, Session );
}

void fields::SetFieldCasting( core::rSession &Session )
{
	field::SetCasting( FieldFrameId_, Session );
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
	
	Session.GetNumericalValue( Id, **Field );

	Session.User.Panel().DefineField( Field );

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

		Session.GetNumericalValue( Id, **Field );

		return Field;
	}
}

AC( DragField )
{
	Session.User.Panel().DragField( GetContent_( Session, Id ) );
	record::SetFieldsCasting( Session );
}

AC( DropField )
{
	Session.User.Panel().DropField( GetContent_( Session, Id ) );
	record::SetFieldsLayout( Session );
}

AC( EndFieldDragging )
{
	Session.User.Panel().EndFieldDragging();
}

