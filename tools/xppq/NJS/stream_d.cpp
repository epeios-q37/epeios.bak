/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of XPPq.

	XPPq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XPPq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XPPq. If not, see <http://www.gnu.org/licenses/>.
*/

#include "stream_d.h"

#include "stream_s.h"

using namespace stream_d;

using stream_s::rRack;

void stream_d::OnRead( sclnjs::sCaller &Caller )
{
qRH
	sclnjs::rRStream This;
qRB
	This.Init();
	Caller.GetArgument( This );

	rRack &Rack = *(rRack *)( This.Get( "_rack0" ) );

	if ( Rack.Error.Amount() != 0 ) {
		This.EmitError( Rack.Error );
		Rack.Error.Init();
	}

	Rack.Blocker.Unblock();
qRR
qRE
qRT
}

using stream_s::rNewRack;


void stream_d::NewOnRead( sclnjs::sCaller &Caller )
{
qRH
	sclnjs::rRStream This;
qRB
	This.Init();

	Caller.GetArgument( This );

	rNewRack &Rack = *(rNewRack *)( This.Get( "_rack0" ) );

	Rack.Unblock();
qRR
qRE
qRT
}

