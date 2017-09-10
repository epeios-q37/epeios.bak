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

namespace {
	namespace {
		typedef sclnjs::cAsync cAsync_;
		using stream_s::rRack;
	}

	class rStreamRackASyncCallback_	// By naming it simply 'rRackASyncCallback_', VC++ debugger confuses it with the one in 'parser.cpp', although both are defined in an anonymous namespace !
	: public rRack,
	  public cAsync_
	{
	protected:
		virtual void SCLNJSWork( void ) override
		{
			rRack::Retrieve();
		}
		virtual uvq::eBehavior SCLNJSAfter( void ) override
		{
			if ( rRack::Send() ) {
				return uvq::bExitAndDelete;
			} else
				return uvq::bRelaunch;
		}
	public:
		void reset( bso::sBool P = true )
		{
			rRack::reset( P );
		}
		qCVDTOR( rStreamRackASyncCallback_ );
		void Init( sclnjs::rRStream &Stream )
		{
			rRack::Init( Stream );
		}
	};
}

void stream::OldSet( sclnjs::sCaller &Caller )
{
qRH
	sclnjs::rRStream Source, *This = NULL;
	rStreamRackASyncCallback_ *Rack = NULL;
qRB
	Rack = new rStreamRackASyncCallback_;

	if ( Rack == NULL )
		qRAlc();

	This = new sclnjs::rRStream;

	if ( This == NULL )
		qRAlc();

	tol::Init( Source, *This );

	Caller.GetArgument( Source, *This );

	Rack->Init( *This );

	Source.Set( "_rack0", Rack );
	This->Set( "_rack0", Rack );

#if 1
//	Source.OnReadable( OnReadable_ );
# else // Doesn't always work. Sometimes, 'onend' event is not launched...
	Source.OnData( OnData_ );
	Source.OnEnd( OnEnd_ );
#endif

//	This.OnRead( OnRead_ );

	sclnjs::Launch( *Rack );
qRR
	if ( Rack != NULL )
		delete Rack;

	if ( This != NULL )
		delete This;
qRT
qRE
}

using stream_s::rNewRack;

void stream::Set( sclnjs::sCaller &Caller )
{
qRH
	sclnjs::rRStream Source, *This = NULL;
	rNewRack *Rack = NULL;
qRB
	Rack = new rNewRack;

	if ( Rack == NULL )
		qRAlc();

	This = new sclnjs::rRStream;

	if ( This == NULL )
		qRAlc();

	tol::Init( Source, *This );

	Caller.GetArgument( Source, *This );

	Rack->Init( *This );

	Source.Set( "_rack0", Rack );
	This->Set( "_rack0", Rack );

#if 1
//	Source.OnReadable( OnReadable_ );
# else // Doesn't always work. Sometimes, 'onend' event is not launched...
	Source.OnData( OnData_ );
	Source.OnEnd( OnEnd_ );
#endif

//	This.OnRead( OnRead_ );

	sclnjs::Launch( *Rack );
qRR
	if ( Rack != NULL )
		delete Rack;

	if ( This != NULL )
		delete This;
qRT
qRE
}
