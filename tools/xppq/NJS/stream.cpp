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

#include "stream.h"

#include "stream_s.h"

using stream_s::rRack;

void stream::Set( sclnjs::sCaller &Caller )
{
qRH
	sclnjs::rRStream Source, *This = NULL;
	rRack *Rack = NULL;
qRB
	Rack = new rRack;

	if ( Rack == NULL )
		qRAlc();

	This = new sclnjs::rRStream;

	if ( This == NULL )
		qRAlc();

	tol::Init( Source, *This );

	Caller.GetArgument( Source, *This );

	Rack->Init( *This );

	Source.Set( stream_s::Id, Rack );
	This->Set( stream_s::Id, Rack );

	sclnjs::Launch( *Rack );
qRR
	if ( Rack != NULL )
		delete Rack;

	if ( This != NULL )
		delete This;
qRT
qRE
}
