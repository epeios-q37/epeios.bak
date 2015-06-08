/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#define CSDSNC__COMPILATION

#include "csdsnc.h"

#ifdef CSDSNC__MT
#	include "mtk.h"
#endif

using namespace csdsnc;

#define CASE( n )\
	case l##n:\
		return #n;\
		break

const char *csdsnc::GetLogLabel( log__ Log )
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
			csdsnb::PutId( CSDSNB_PING, Flow );
			Flow.Commit();

			if ( Flow.Get() != 0 )
				qRFwk();

			Flow.Dismiss();
		}
}

void csdsnc::core_::Ping( void )
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

#ifdef CSDSNC__MT
static void KeepAlive_( void *UP )
{
	csdsnc::core_ &Core = *(csdsnc::core_ *)UP;
	tol::timer__ Timer;

	Timer.Init( Core.S_.Ping.Delay );
	Timer.Launch();

	while ( !mtx::IsLocked( Core.S_.Ping.Mutex ) ) {	// Tant que pas de demande de terminaison.
		tht::Suspend( CSDSNC_PING_RESOLUTION );

		if ( Timer.IsElapsed() ) {
			Core.Ping();
			Timer.Launch();
		}
	}

	_Unlock( Core.S_.Ping.Mutex );	// Signale que la demande de terminaison a t prise en compte.
}
#endif

void csdsnc::core_::_KeepAlive( time_t Delay )
{
#ifdef CSDSNC__MT
	if ( Delay <= CSDSNC_PING_RESOLUTION )
		qRFwk();

	mtk::Launch( ::KeepAlive_, this );
#else
	if ( Delay != 0 )
		qRFwk();
#endif
}

void csdsnc::core_::_DeleteFlows( void )
{
	while ( Flows.Amount() != 0 )
	{
		PutId( CSDSNB_CLOSE, *Flows.Top() );
		delete Flows.Pop();
	}
}
