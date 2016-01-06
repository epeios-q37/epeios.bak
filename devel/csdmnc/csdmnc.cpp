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

#define CSDMNC__COMPILATION

#include "csdmnc.h"

using namespace csdmnc;

#ifdef CSDMNC__MT
#	include "mtk.h"
#endif

#define CASE( n )\
	case l##n:\
		return #n;\
		break

const char *csdmnc::GetLogLabel( log__ Log )
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

static void Ping_(
	_flow___ &Flow,
	time_t Delay )
{
	if ( ( tol::EpochTime( false ) - Flow.EpochTimeStamp() ) >= Delay )
		if ( !Flow.OFlowIsLocked() ) {
			csdmnb::PutId( CSDMNB_PING, Flow );
			Flow.Commit();

			if ( Flow.Get() != 0 )
				qRFwk();

			Flow.Dismiss();
		}
}

void csdmnc::core_::Ping( void )
{
	_Lock( S_.MainMutex );

	stk::row__ Row = Flows.First();
	
	while ( Row != qNIL )
	{
		Ping_( *Flows( Row ), S_.Ping.Delay );

		Row = Flows.Next( Row );
	}

	_Unlock( S_.MainMutex );
}

#ifdef CSDMNC__MT
static void KeepAlive_( void *UP )
{
	csdmnc::core_ &Core = *(csdmnc::core_ *)UP;
	tol::timer__ Timer;

	Timer.Init( Core.S_.Ping.Delay );
	Timer.Launch();

	while ( !mtx::IsLocked( Core.S_.Ping.Mutex ) ) {	// While no terminating request.
		tht::Suspend( CSDMNC_PING_DELAY );

		if ( Timer.IsElapsed() ) {
			Core.Ping();
			Timer.Launch();
		}
	}

	_Unlock( Core.S_.Ping.Mutex );	// Reports that the terminating request has been handled.
}
#endif

void csdmnc::core_::_KeepAlive( time_t Delay )
{
#ifdef CSDMNC__MT
	if ( Delay <= CSDMNC_PING_DELAY )
		qRFwk();

	mtk::Launch( ::KeepAlive_, this );
#else
	if ( Delay != 0 )
		qRFwk();
#endif
}

void csdmnc::core_::_DeleteFlows( void )
{
	while ( Flows.Amount() != 0 )
	{
		PutId( CSDMNB_CLOSE, *Flows.Top() );
		delete Flows.Pop();
	}
}
