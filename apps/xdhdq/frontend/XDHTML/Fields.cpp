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
	class sContent
		: public xdhcmn::cContent {
	private:
		qRMV( core::rSession, S_, Session_ );
		const str::dString &GetContent_( const str::dString &Id )
		{
			return S_().User.GetContent( GetField( Id ) );
		}
	protected:
		virtual void XDHCMNGetContents(
			const str::dStrings &Tags,
			str::dStrings &Contents ) override
		{
			sdr::sRow Row = Tags.First();

			while ( Row != qNIL ) {
				Contents.Append( GetContent_( Tags( Row ) ) );

				Row = Tags.Next( Row );
			}
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

void fields::SetLayout(
	const char *Id,
	core::rSession &Session )
{
	sContent Content;

	Content.Init( Session );

	core::SetElementLayout( Id, XSLAffix_, layout_::Get, Content, Session );
}

void fields::SetCasting(
	const char *Id,
	core::rSession &Session )
{
	core::SetElementCasting( Id, XSLAffix_, casting_::Get, Session );
}

void fields::Display(
	const char *Id,
	core::rSession &Session )
{
	SetLayout( Id, Session );

	SetCasting( Id, Session );
}

#define AC( name ) BASE_AC( fields, name )

AC( Edit )
{
	Session.User.SetField( GetField( str::string( Id ) ) );
	main::DisplayFields( Session );
}

AC( Refresh )
{
	main::DisplayFields( Session );
}



