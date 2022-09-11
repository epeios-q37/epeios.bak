/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHDq' software.

    'XDHDq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHDq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHDq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Fields.h"

#include "main.h"

#include "core.h"
#include "registry.h"
#include "sclfrntnd.h"

using namespace fields;

using namespace instc;

namespace {
	qCDEF( char *, XSLAffix_, "Fields" );

	namespace layout_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{
			eField Field = Session.User.GetField();

			Writer.PushTag( "Fields " );

			if ( Field != f_Undefined )
				Writer.PutAttribute( "Editable", GetLabel( Field ) );

			int i = f_amount;

			while ( i-- ) {
				Writer.PushTag( "Field " );

				Writer.PutAttribute( "id", GetLabel( (eField)i ) );

				Writer.PopTag();
			}

			Writer.PopTag();
		}
	}

	namespace casting_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{
			Writer.PushTag( "Test" );

			Writer.PutAttribute( "Enabled", ( Session.User.TestButtonIsVisible() ? "true" : "false" ) );
		}
	}
}

namespace {
	void GetContent_(
		instc::eField Field,
		core::rSession &Session,
		str::dString &Tag,
		str::dString &Content )
	{
		Tag.Append( instc::GetLabel( Field ) );
		Content.Append( Session.User.GetContent( Field ) );
	}

	void GetContents_(
		core::rSession &Session,
		str::dStrings &Tags,
		str::dStrings &Contents )
	{
	qRH;
		int Field = instc::f_amount;
		str::wString Tag, Content;
	qRB;
		while ( Field-- ) {
			tol::Init( Tag, Content );

			GetContent_( (instc::eField)Field, Session, Tag, Content );

			Tags.Append( Tag );
			Contents.Append( Content );
		}
	qRR;
	qRT;
	qRE;
	}
}

void fields::SetLayout(
	const char *Id,
	core::rSession &Session )
{
qRH;
	str::wStrings Ids, Contents;
qRB;
	Session.SetElementLayout( Id, XSLAffix_, layout_::Get );

	tol::Init( Ids, Contents );
	GetContents_( Session, Ids, Contents );
	Session.SetContents( Ids, Contents );
	Session.DressWidgets( Id );
qRR;
qRT;
qRE;
}

void fields::Display(
	const char *Id,
	core::rSession &Session )
{
	SetLayout( Id, Session );
}

#define AC( name ) BASE_AC( fields, name )

AC( Edit )
{
qRH;
	str::wString FieldId;
qRB;
	FieldId.Init( Id+6 );
	Session.User.SetField( GetField( FieldId ) );
	main::DisplayFields( Session );
qRR;
qRT;
qRE;
}

AC( Refresh )
{
	main::DisplayFields( Session );
}



