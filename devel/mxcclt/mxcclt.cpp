/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#define MXCCLT__COMPILATION

#include "mxcclt.h"

using namespace mxcclt;

#ifdef MXCCLT__MT
#	include "mtk.h"
#endif

#define CASE( n )\
	case l##n:\
		return #n;\
		break

const char *mxcclt::GetLogLabel( eLog Log )
{
	switch ( Log ) {
		CASE( Creation );
		CASE( Retrieval );
		CASE( Release );
	default:
		qRFwk();
		return NULL;	// Pour viter un 'warning'.
		break;
	}
}

namespace {
	inline void Ping_(
		void *UP,
		time_t Delay,
		fCallback &Callback )
	{
		fFlow &Flow = Callback.ExtractFlow( UP );

		if ( ( tol::EpochTime( false ) - Callback.EpochTimeStamp( UP ) ) >= Delay )
			if ( !Flow.OFlowIsLocked() ) {
				PutId( MXCBSE_PING, Flow );
				Flow.Commit();

				if ( Flow.Get() != 0 )
					qRFwk();

				Flow.Dismiss();
			}
	}
}

void mxcclt::rCore::Ping( void )
{
	Lock_( MainMutex_ );

	stk::row__ Row = UPs.First();
	
	while ( Row != qNIL )
	{
		::Ping_( UPs( Row ), Ping_.Delay, C_() );

		Row = UPs.Next( Row );
	}

	Unlock_( MainMutex_ );
}

#ifdef MXCCLT__MT
void mxcclt::KeepAlive_( void *UP )
{
	rCore &Core = *(rCore *)UP;
	tol::timer__ Timer;

	Timer.Init( Core.Ping_.Delay );
	Timer.Launch();

	while ( !mtx::IsLocked( Core.Ping_.Mutex ) ) {	// While no terminating request.
		tht::Suspend( MXCCLT_PING_DELAY );

		if ( Timer.IsElapsed() ) {
			Core.Ping();
			Timer.Launch();
		}
	}

	Unlock_( Core.Ping_.Mutex );	// Reports that the terminating request has been handled.
}
#endif

void mxcclt::rCore::KeepAlive_( time_t Delay )
{
#ifdef MXCCLT__MT
	if ( Delay <= MXCCLT_PING_DELAY )
		qRFwk();

	mtk::Launch( ::KeepAlive_, this );
#else
	if ( Delay != 0 )
		qRFwk();
#endif
}

void mxcclt::rCore::ReleaseUPs_( void )
{
	while ( UPs.Amount() != 0 )
	{
		PutId( MXCBSE_CLOSE, C_().ExtractFlow( UPs.Top() ) );
		C_().Release( UPs.Pop() );
	}
}
