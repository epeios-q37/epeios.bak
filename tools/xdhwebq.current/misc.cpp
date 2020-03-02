/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHWebQ.

	XDHWebQ is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHWebQ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHWebQ. If not, see <http://www.gnu.org/licenses/>.
*/

#include "misc.h"

#include "xdhujs.h"

using namespace misc;

void misc::Report(
	const nstring___ &Message,
	str::string_ &Script )
{
qRH
	str::string Buffer;
qRB
	Script.Append( "alert(\"" );

	Buffer.Init();
	xdhcmn::Escape( Message.UTF8( Buffer ), Script, '"' );
	Script.Append( "\");");
qRR
qRT
qRE
}
