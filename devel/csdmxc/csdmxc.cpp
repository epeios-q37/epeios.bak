/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define CSDMXC_COMPILATION_

#include "csdmxc.h"

using namespace csdmxc;

#ifdef CSDMXC__MT
#	include "mtk.h"
#endif

#define CASE( n )\
	case l##n:\
		return #n;\
		break

const char *csdmxc::GetLabel( eLog Log )
{
	switch ( Log ) {
		CASE( Creation );
		CASE( Acquire );
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
		cCallback &Callback )
	{
	qRH
		flw::rDressedRWFlow<> Flow;
	qRB
		Flow.Init( Callback.ExtractDriver( UP ) );

		if ( ( tol::EpochTime( false ) - Callback.EpochTimeStamp( UP ) ) >= Delay ) {
			if ( !Flow.OFlowIsLocked() ) {
				PutId( CSDMXB_PING, Flow );
				Flow.Commit();

				if ( Flow.Get() != 0 )
					qRFwk();

				Flow.Dismiss();
			} else {
				Flow.Commit( false );
				Flow.Dismiss( false );
			}
		}
	qRR
	qRT
	qRE
	}
}

void csdmxc::rCore::Ping( void )
{
	Lock_( MainMutex_ );

	stkbch::sRow Row = UPs.First();

	while ( Row != qNIL )
	{
		::Ping_( UPs( Row ), Ping_.Delay, C_() );

		Row = UPs.Next( Row );
	}

	Unlock_( MainMutex_ );
}

#ifdef CSDMXC__MT
void csdmxc::KeepAlive_( void *UP )
{
	rCore &Core = *(rCore *)UP;
	tol::timer__ Timer;

	Timer.Init( Core.Ping_.Delay );
	Timer.Launch();

	while ( !mtx::IsLocked( Core.Ping_.Mutex ) ) {	// While no terminating request.
		tht::Suspend( CSDMXC_PING_DELAY );

		if ( Timer.IsElapsed() ) {
			Core.Ping();
			Timer.Launch();
		}
	}

	Unlock_( Core.Ping_.Mutex );	// Reports that the terminating request has been handled.
}
#endif

void csdmxc::rCore::KeepAlive_( time_t Delay )
{
#ifdef CSDMXC__MT
	if ( Delay <= CSDMXC_PING_DELAY )
		qRFwk();

	mtk::RawLaunch(::KeepAlive_, this, true);
#else
	if ( Delay != 0 )
		qRFwk();
#endif
}

void csdmxc::rCore::ReleaseUPs_( void )
{
qRH
	flw::rDressedWFlow<> Flow;
qRB
	while ( UPs.Amount() != 0 )
	{
		Flow.Init( C_().ExtractDriver( UPs.Top() ) );
		PutId( CSDMXB_CLOSE, Flow );
		Flow.Commit();
		C_().Release( UPs.Pop() );
	}
qRR
qRT
qRE
}
