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

#include "Frame.h"

#include "core.h"
#include "registry.h"
#include "sclfrntnd.h"

namespace {
	qCDEF( char *, XSLAffix_, "Frame" );

	namespace layout_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{}
	}
}

void frame::SetLayout(
	const char *Id,
	core::rSession &Session )
{
	Session.SetElementLayout( Id, XSLAffix_, layout_::Get );
}

void frame::Display(
	const char *Id,
	core::rSession &Session )
{
	SetLayout( Id, Session );
}

#define AC( name ) BASE_AC( frame, name )

AC( Template )
{
	Session.AlertT( "Template" );
}

