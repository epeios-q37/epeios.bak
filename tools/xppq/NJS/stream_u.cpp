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

#include "stream_u.h"

#include "stream_s.h"

using namespace stream_u;

using stream_s::rRack;

void stream_u::OnData( sclnjs::sCaller &Caller )
{
qRH
	sclnjs::rRStream This;
	sclnjs::rBuffer Chunk;
qRB
	tol::Init( This, Chunk );
	Caller.GetArgument( This, Chunk );

	rRack &Rack = *(rRack *)This.Get( stream_s::Id );

	Rack << Chunk;
qRR
qRT
qRE
}

void stream_u::OnEnd( sclnjs::sCaller &Caller )
{
qRH
	sclnjs::rRStream This;
qRB
	tol::Init( This );
	Caller.GetArgument( This );

	rRack &Rack = *(rRack *)This.Get( stream_s::Id );

	Rack.Commit();
qRR
qRT
qRE
}

