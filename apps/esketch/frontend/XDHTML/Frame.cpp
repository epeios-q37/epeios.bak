/*
	Copyright (C) 2018 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#include "Frame.h"

#include "core.h"
#include "registry.h"

namespace {
	qCDEF( char *, XSLAffix_, "Frame" );

	namespace layout_ {
		void Get(
			core::rSession &Session,
			xml::rWriter &Writer )
		{}
	}
}

void frame::SetLayout(
	const str::dString &Id,
	core::rSession &Session )
{
	Session.SetElementLayout( Id, XSLAffix_, layout_::Get );
}

void frame::SetCasting(
	const str::dString &Id,
	core::rSession &Session )
{
}

void frame::Display(
	const str::dString &Id,
	core::rSession &Session )
{
	SetLayout( Id, Session );

	SetCasting( Id, Session );
}

#define A( name ) BASE_ADef( frame, name )

A( Template )
{
	Session.AlertT( "Template", "TemplateTitle" );
}

