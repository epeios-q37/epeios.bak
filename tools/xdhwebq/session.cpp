/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHWebQ.

	XDHWebQ is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHWebQ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHWebQ. If not, see <http://www.gnu.org/licenses/>.
*/

#include "session.h"

#include "mtk.h"

using namespace session;

#define DIGITS "azertyuiopmlkjhgfdsqwxcvbnNBVCXWQSDFGHJKLMPOIUYTREZA9876543210"

inline void session::sJS::XDHUJPExecute(
	const str::string_ &Script,
	TOL_CBUFFER___ *Buffer )
{
qRH
	str::string Result;
qRB
	Result.Init();

	S_().DownstreamExecute( Script, Result );

	if ( Buffer != NULL ) 
		Result.Convert( *Buffer );
//	else if ( Result.Amount() != 0 )
//		qRGnr();
qRR
qRT
qRE
}

static void Launch_(
	shared_data__ &Data,
	rSession &Session,
	mtk::gBlocker &Blocker )
{
qRH;
	TOL_CBUFFER___ Id, Action;
	bso::sBool Locked = false;
qRB;
	xdhcmn::cSession &Callback = Data.Callback();

	Data.Id().Convert( Id );
	Data.Action().Convert( Action );

	Session.UpstreamCallInProgress( true );

	Blocker.Release();

	Locked = true;

	Callback.Launch( Id, Action );
qRR;
	Session.MarkAsDead();
qRT;
	if ( Locked ) {
		Session.Lock();

		Session.UpstreamCallInProgress( false );

		Session.UnlockUpstream();

		Session.Unlock();
	}
qRE;
}

static void Launch_(
	void *UP,
	mtk::gBlocker &Blocker )
{
qRFH;
	shared_data__ &Data = *(shared_data__ *)UP; 
qRFB;
	rSession &Session = Data.Session();

	Launch_( Data, Session, Blocker );
qRFR;
qRFT;
qRFE( sclmisc::ErrFinal() );
}

void session::rSession::UpstreamLaunch(
	const str::string_ &Id,
	const str::string_ &Action,
	str::string_ &Script )
{
qRH
	shared_data__ Data;
qRB
	if ( !IsLocked() )
		qRGnr();

	if ( IsUpstreamCallInProgress() )
		qRReturn;

	Data.Init ( Id, Action, *this, _SC() );

	mtk::Launch( Launch_, &Data );

	_Lock( tUpstream );

	_Unlock( tGlobal );

	_Lock( tUpstream );	// Unlocked by 'DownstreamExecute', when a new script is available (or when no more script is available).

	_Lock( tGlobal );

	_Unlock( tUpstream );

	Script.Append( _Dispatch );

	_Dispatch.Init();
qRR
qRT
	if ( IsLocked() )
		Unlock();
qRE
}

void session::rSession::UpstreamReport(
	const str::string_ &Response,	// Response from previous script.
	str::string_ &Script )	// Script to execute. Empty if nothing to do.
{
qRH
qRB
	if ( !IsLocked() )
		qRGnr();

	_Dispatch.Append( Response );

	_Unlock( tDownstream );

	_Lock( tUpstream );

	_Unlock( tGlobal );

	_Lock( tUpstream );	// Unlocked by 'DownstreamExecute', when a new script is available (or when no more script is available).

	_Lock( tGlobal );

	_Unlock( tUpstream );

	Script.Append( _Dispatch );

	_Dispatch.Init();
qRR
qRT
	if ( IsLocked() )
		Unlock();
qRE
}


void session::rSession::DownstreamExecute(
	const str::string_ &Script,
	str::string_ &Result )
{	
qRH
qRB
	_Lock( tGlobal );

	_Dispatch.Append( Script );

	_Unlock( tUpstream );

	_Lock( tDownstream );

	_Unlock( tGlobal );

	_Lock( tDownstream );	// Unlocked by 'UpstreamReport', when a response is available.

	_Lock( tGlobal );

	Result.Append( _Dispatch );

	_Dispatch.Init();

	_Unlock( tDownstream );
qRR
qRT
	if ( _IsLocked( tGlobal ) )
		_Unlock( tGlobal );
qRE
}


void session::id__::New( void )
{
	int i;

	for( i = 0; i < IdSize; i++ )
		Raw_[i] = DIGITS[rand() % ( sizeof( DIGITS ) - 1 )];

	Raw_[i] = 0;
}


static inline bso::sign__ Test_(
	const char *S1,
	const char *S2 )
{
	return strcmp( S1, S2 );
}

static bso::sign__ Search_(
	const _ids_ &T,
	const char *S,
	idxbtq::E_ISEEKERt__( row__ ) &Seeker )
{
	bso::sign__ Test;
	row__ Row = Seeker.GetCurrent();

	while ( ( Row != qNIL )
			&& ( ( Test = Test_( T( Row ).Value(), S ) ) != 0 ) ) {
		switch( Test ) {
		case 1:
			Row = Seeker.SearchGreater();
			break;
		case -1:
			Row = Seeker.SearchLesser();
			break;
		default:
			qRGnr();
			break;
		}
	}

	return Test;
}

row__ session::dSessions::New(
	id__ &Id,
	const str::string_ &Language,
	const str::dString &Token )	// If _not_ empty, DEMO.
{
	row__ Row = qNIL;
qRH
	xdhcmn::cSession *SessionCallback = NULL;
	rSession *Session = NULL;
	xdhujp::sProxyCallback *ProxyCallback = NULL;
	timer__ Timer;
	TOL_CBUFFER___ Buffer;
qRB
	Row = Sessions.New();

	Session = new session::rSession;

	if ( Session == NULL )
		qRAlc();

	ProxyCallback = new xdhujp::sProxyCallback;

	if ( ProxyCallback == NULL )
		qRAlc();

	SessionCallback = _A().RetrieveCallback( Language.Convert( Buffer ), Token, ProxyCallback );

	if ( SessionCallback != NULL ) {
		Session->Init( *SessionCallback );

		Sessions.Store( Session, Row );

		ProxyCallback->Init( Session->JSCallback() );

		_AdjustSizes();

		do {
			Id.New();
		} while ( Search( Id ) != qNIL );

		Ids.Store( Id, Row );

		if ( S_.Root == qNIL ) {
			S_.Root = Row;
			Order.Create( Row );
		} else {
			idxbtq::E_ISEEKERt__( row__ ) Seeker;

			Seeker.Init( Index, S_.Root );

			switch ( Search_( Ids, Id.Value(), Seeker ) ) {
			case 1:
				S_.Root = Index.BecomeGreater( Row, Seeker.GetCurrent(), S_.Root );
				break;
			case -1:
				S_.Root = Index.BecomeLesser( Row, Seeker.GetCurrent(), S_.Root );
				break;
			default:
				qRGnr();
				break;
			}

			Order.BecomeNext( Row, Order.Tail() );
		}


		if ( time( &Timer.Relative ) == -1 )
			qRLbr();

		if ( time( &Timer.Absolute ) == -1 )
			qRLbr();

		Timer.Persistent = false;

		Timers.Store( Timer, Row );
	} else {
		Sessions.Delete( Row );
		Row = qNIL;
	}
qRR
	Row = qNIL;
qRT
	if ( Row == qNIL ) {
		if ( ProxyCallback != NULL )
			delete ProxyCallback;

		ProxyCallback = NULL;
	}
qRE
	return Row;
}

row__ session::dSessions::Search( const char *Id ) const
{
	if ( S_.Root != qNIL )	{
		idxbtq::E_ISEEKERt__( row__ ) Seeker;

		Seeker.Init( Index, S_.Root );

		if ( Search_( Ids, Id, Seeker ) == 0 )
			return Seeker.GetCurrent();
		else
			return qNIL;
	}

	return qNIL;
}

row__ session::dSessions::Search( const str::string_ &Id ) const
{
	row__ Row = qNIL;
qRH
	TOL_CBUFFER___ Buffer;
qRB
	Row = Search(Id.Convert( Buffer ) );
qRR
qRT
qRE
	return Row;
}


