/*
	Copyright (C) 2018 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'TaskQ' software.

    'TaskQ' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'TaskQ' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'TaskQ'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "login.h"

#include "core.h"
#include "main.h"
#include "prolog.h"
#include "registry.h"

#include "xdhdws.h"

namespace {
	qCDEF( char *, XSLAffix_, "Login" );

	namespace layout_ {
		void Get(
			core::rSession &Session,
			xml::rWriter &Writer )
		{
			sclx::login::WriteLayout(Session, Writer);
		}
	}
}

void login::SetLayout( core::rSession &Session )
{
	Session.Inner( "", XSLAffix_, layout_::Get );
}

void login::Display( core::rSession &Session )
{
	SetLayout( Session );

	sclx::login::HandleBackendTypeSwitching( Session );

	Session.SwitchTo( base::pLogin );
}

#define A( name ) BASE_ADef( login, name )

A( SwitchBackendType )
{
	sclx::login::HandleBackendTypeSwitching( Session );
}

A( DisplayEmbeddedBackendFilename )
{
	sclx::login::DisplaySelectedEmbeddedBackendFilename( Session, Id );
}

#if 1
A( Connect )
{
	core::Connect(Mode, true, Session);

	main::Display( Session );
}
#else
A( Connect )
{
qRH
	fblfrd::incompatibility_informations IncompatibilityInformations;
	sclf::rBackendFeatures Features;
qRB
	if ( Mode == xdhcdc::mMonoUser ) {
		Features.Init();
		sclx::login::GetBackendFeatures( Session, Features );
		core::Kernel().Init( Features, plgn::EmptyAbstracts );
	}

	IncompatibilityInformations.Init();
	if ( !Session.Connect( fblfrd::compatibility_informations__( TSKINF_LC, TASKQ_API_VERSION ), IncompatibilityInformations ) )
		qRGnr();

	main::Display( Session );
qRR
	Session.Disconnect();
qRE
qRT
}
#endif

A( Dismiss )
{
	prolog::Display( Session );
}

