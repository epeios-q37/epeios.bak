/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.h"

#include "core.h"
#include "registry.h"
#include "sclfrntnd.h"

namespace{
	qCDEF( char *, XSLAffix_, "Main" );

	namespace layout_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{}
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

void main::SetLayout( core::rSession &Session )
{
	core::SetDocumentLayout( XSLAffix_, layout_::Get, Session );
}

void main::SetCasting( core::rSession &Session )
{
	core::SetDocumentCasting( XSLAffix_, casting_::Get, Session );
}

void main::Display( core::rSession &Session )
{
	SetLayout( Session );

	SetCasting( Session );

	Session.SwitchTo( core::pMain );
}

#define AC( name ) BASE_AC( main, name )

namespace {
	class sContent
	: public xdhcmn::cContent {
	private:
		qRMV( core::rSession, S_, Session_ );
	protected:
		virtual void XDHCMNGetContents(
			const str::dStrings &Tags,
			str::dStrings &Contents ) override
		{
		qRH;
			str::string Test;
			TOL_CBUFFER___ Buffer;
		qRB;
			Test.Init( S_().GetValue( "Pattern", Buffer ) );

			S_().User.ToUpper( Test );

			Contents.Append( Test );
		qRR;
		qRT;
		qRE;
		}	public:
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

	void SubmissionNew_(
		const char *Id,
		core::rSession &Session )
	{
	qRH;
		str::wString Content;
	qRB;
	qRR;
	qRT;
	qRE;
	}
	}
	{
		sContent Content;

		Content.Init( Session );

		Session.SetContents( Id, Content );
	}

	void SubmissionOld_(
		const char *Id,
		core::rSession &Session )
	{
	qRH;
		str::string Test;
		TOL_CBUFFER___ Buffer;
	qRB;
		Test.Init( Session.GetValue( "Pattern", Buffer ) );

		Session.User.ToUpper( Test );

		Session.SetValue( "Pattern", Test );
	qRR;
	qRT;
	qRE;
	}
}

AC( Submission )
{
#if 1
	SubmissionNew_( Id, Session );
#else
	SubmissionOld_( Id, Session );
#endif
}

AC( ShowTestButton )
{
	Session.User.TestButtonIsVisible() = true;
	SetCasting( Session );
}

AC( HideTestButton )
{
	Session.User.TestButtonIsVisible() = false;
	SetCasting(Session );
}

AC( Testing )
{
	Session.User.TestMessage();
}

