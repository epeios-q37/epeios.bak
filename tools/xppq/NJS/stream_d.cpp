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

namespace {
	typedef sclnjs::cAsync cAsync_;

	class sAsync_
	: public cAsync_ {
	private:
		bso::sBool IsFirst_;
		qRMV( rNewRack, R_, Rack_ );
		void *Buffer_[100];
		bso::sSize Amount_ = 0;
		qRMV( sclnjs::rRStream, S_, Stream_ );
	protected:
		virtual void UVQWork( void ) override
		{
			if ( IsFirst_ )
				R_().DelayedInit();

			if ( !R_().EndOfFlow() )
				Amount_ = R_().ReadUpTo( sizeof( Buffer_ ), Buffer_ );
		}
		// Note to 'v8' user : you CAN access any of the 'v8' data from this method.
		virtual sclnjs::eBehavior UVQAfter( void ) override
		{
			if ( Amount_ == 0 )
				S_().End();
			else
				S_().Push( Buffer_, Amount_ );

			return sclnjs::bExitAndDelete;
		}
	public:
		void reset( bso::sBool P = true )
		{
			IsFirst_ = false;
			Rack_ = NULL;
			Stream_ = NULL;
			Amount_ = 0;
		}
		qCVDTOR( sAsync_ );
		void Init(
			bso::sBool IsFirst,
			rNewRack &Rack,
			sclnjs::rRStream &Stream )
		{
			IsFirst_ = IsFirst;
			Rack_ = &Rack;
			Stream_ = &Stream;
			Amount_ = 0;
		}
	};
}

void stream_d::NewOnRead( sclnjs::sCaller &Caller )
{
	static bso::sBool IsFirst = true;
qRH
	sclnjs::rRStream This;
	sAsync_ *Async = NULL;
qRB
	This.Init();
	Caller.GetArgument( This );

	rNewRack &Rack = *(rNewRack *)( This.Get( "_rack0" ) );

	Async = new sAsync_;

	if ( Async == NULL )
		qRAlc();

	Async->Init( IsFirst, Rack, This );

	sclnjs::Launch( *Async );
qRR
	if ( Async != NULL )
		delete( Async );
qRE
qRT
}

