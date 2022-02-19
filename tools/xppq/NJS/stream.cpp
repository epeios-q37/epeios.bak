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

using namespace stream;

namespace {
  qCDEF( char *, Id_, "_q37RackPreprocessor" );
}

void stream::upstream::OnData( sclnjs::sCaller &Caller )
{
qRH
	sclnjs::rObject This;
	sclnjs::rBuffer Chunk;
qRB
	tol::Init( This, Chunk );
	Caller.GetArgument( This, Chunk );

	This.Get<shared::rRack>(Id_) << Chunk;
	cio::COut << Chunk << txf::commit;
qRR
qRT
qRE
}

void stream::upstream::OnEnd( sclnjs::sCaller &Caller )
{
qRH
	sclnjs::rObject This;
qRB
	tol::Init( This );
	Caller.GetArgument( This );

	This.Get<shared::rRack>(Id_).Commit();
qRR
qRT
qRE
}

void stream::downstream::Read( sclnjs::sCaller &Caller )
{
qRH
	sclnjs::rObject This;
qRB
	This.Init();

	Caller.GetArgument( This );

	This.Get<shared::rRack>(Id_).Unblock();	// Reports than 'push()' operations can be made.
qRR
qRE
qRT
}

void stream::_Set( sclnjs::sCaller &Caller )
{
qRH
	sclnjs::rObject Source, *This = NULL;
	shared::rRack *Rack = NULL;
qRB
	Rack = new shared::rRack;

	if ( Rack == NULL )
		qRAlc();

	This = new sclnjs::rObject;

	if ( This == NULL )
		qRAlc();

	tol::Init( Source, *This );

	Caller.GetArgument( Source, *This );

	Rack->Init( *This );

	Source.Set( Id_, Rack );
	This->Set( Id_, Rack );

	sclnjs::Launch( *Rack );
qRR
	if ( Rack != NULL )
		delete Rack;

	if ( This != NULL )
		delete This;
qRT
qRE
}
