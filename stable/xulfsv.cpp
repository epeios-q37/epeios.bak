/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#define XULFSV_COMPILATION_

#include "xulfsv.h"

#include "xulftk.h"
#include "xulfrg.h"

using namespace xulfsv;

using xulftk::trunk___;

void xulfsv::_session_view_core__::_Refresh( xml::writer_ & )
{
}

void xulfsv::_session_view_core__::Attach( nsIDOMWindow *Window )
{
	Widgets.Attach( Window );

	Trunk().UI().EventHandlers.SV.Attach( Window );
}

