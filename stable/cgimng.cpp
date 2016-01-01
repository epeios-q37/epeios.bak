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

#define CGIMNG__COMPILATION

#include "cgimng.h"

using namespace cgimng;

#define CASE( n )\
case l##n:\
		return #n;\
		break

const char *cgimng::GetLogLabel( log__ Log )
{
	switch( Log ) {
	CASE( Create );
	CASE( Get );
	CASE( GetButExpired );
	CASE( GetButUnknown );
	CASE( Release );
	CASE( Close );
	CASE( CloseButExpired );
	CASE( CloseButUnknown );
	CASE( CloseButLocked );
	CASE( CloseButInUse );
	CASE( Cleaning );
	default:
		qRFwk();
		return NULL;	// Pour viter un 'warning'.
		break;
	}

}

csdleo::action__ cgimng::core_manager::CSDSCBProcess(
	flw::ioflow__ &Client,
	void *UP )
{
qRH
	cgiarg::arguments CGIArguments;
qRB
#ifdef CGIMNG_DBG
	if ( UP != NULL )
		qRFwk();
#endif

	CGIArguments.Init();
	CGIArguments.Parse( Client );

	UserFunctions_->Handle( CGIArguments, *this, Client );
qRR
qRT
qRE
	return csdleo::aStop;
}


struct data__ {
	void *UP;
	mtx::handler__ Mutex;
	bso::bool__ InUse;
};

void cgimng::user_functions__::SSNMNGDelete( void *UP )
{	
	data__ &Data = *(data__ *)UP;

#ifdef CGIMNG_DBG
#	if 0

	if ( Data.InUse )
		ERRc();

	if ( mtx::IsLocked( Data.Mutex ) )
		ERRc();

#	else
#		pragma message ( __LOC__ "bogue toujours non rsolu ; contournement temporaire !" )
#	endif

#endif

	Delete( Data.UP );

	if ( Data.Mutex != MTX_INVALID_HANDLER )
		mtx::Delete( Data.Mutex );

	delete &Data;
}


void cgimng::tool___::Create(
	void *UP,
	str::string_ &SessionID )
{
qRH
	ssnmng::row__ P = qNIL;
	session_id__ SID;
	lck::read_write_access___<sessions_> Sessions;
	data__ *Data = NULL;
qRB
	Sessions.Init( Sessions_ );

	Data = new data__;

	if ( Data == NULL )
		qRAlc();

	Data->UP = UP;
	Data->Mutex = mtx::Create();
	Data->InUse = true;

	SID.Init();

	tol::InitializeRandomGenerator();

	P = Sessions().New( Data );
//	Sessions().Store( Data, P );
	SID = Sessions().SessionID( P );

	SessionID = SID.Value();

	mtx::Lock( Data->Mutex );

	_Log( lCreate, SessionID );
qRR
qRT
qRE
}

answer__ cgimng::tool___::GetAndLock(
	const str::string_ &SessionID,
	void *&UP )
{
	answer__ Answer = aUndefined;
qRH
	ssnmng::row__ P = qNIL;
	lck::read_write_access___<sessions_> Sessions;
qRB
	Sessions.Init( Sessions_ );

	P = Sessions().Search( SessionID );

	if ( P != qNIL ) {

		data__ &Data =  *(data__ *)Sessions().Pointers( P );

		Data.InUse = true;

		if ( !mtx::TryToLock( Data.Mutex ) ) {
			Sessions.reset();	// Pour dverouller 'Sessions'.
			mtx::Lock( Data.Mutex );
			Sessions.Init( Sessions_ );	// Pour reverroullier 'Sessions'.
		}

		UP = Data.UP;

		if ( Sessions().IsAlive( P ) ) {
			Sessions().MarkAsImmortal( P );
			Answer = aOK;
			_Log( lGet, SessionID );
		} else {
			Answer = aExpired;
			Data.InUse = false;
			mtx::Unlock( Data.Mutex );
			_Log( lGetButExpired, SessionID );
		}
	}
	else {
		Answer = aUnknow;
		_Log( lGetButUnknown, SessionID );
	}
qRR
qRT
qRE
	return Answer;
}

void cgimng::tool___::Release( const str::string_ &SessionID )
{
qRH
	ssnmng::row__ P = qNIL;
	lck::read_write_access___<sessions_> Sessions;
qRB
	Sessions.Init( Sessions_ );

	if ( ( P = Sessions().Search( SessionID ) ) != qNIL )	{

		data__ &Data =  *(data__ *)Sessions().Pointers( P );

#ifdef CGIMNG_DBG
		if ( !mtx::IsLocked( Data.Mutex ) )
			qRFwk();
#endif

		Sessions().Touch( P );

		Data.InUse = false;

		mtx::Unlock( Data.Mutex );

		_Log( lRelease, SessionID );
	} else
		qRFwk();
qRR
qRT
qRE
}


answer__ cgimng::tool___::Close( const str::string_ &SessionID )
{
	answer__ Answer = aUndefined;
qRH
	ssnmng::row__ P = qNIL;
	lck::read_write_access___<sessions_> Sessions;
qRB
	Sessions.Init( Sessions_ );

	P = Sessions().Search( SessionID );

	if ( P != qNIL ) {

		data__ &Data =  *(data__ *)Sessions().Pointers( P );

		if ( !mtx::TryToLock( Data.Mutex ) ) {
			Answer = aLocked;
			_Log( lCloseButLocked, SessionID );
			qRReturn;	// To avoid the below 'mtx::Unlock'.
		} else if ( Data.InUse ) {
			Answer = aInUse;
			_Log( lCloseButInUse, SessionID );
		} else if ( Sessions().IsAlive( P ) ) {
			Answer = aOK;
			_Log( lClose, SessionID );
		} else {
			Answer = aExpired;
			_Log( lCloseButExpired, SessionID );
		}

		mtx::Unlock( Data.Mutex );
		Sessions().Close( P );
	} else {
		Answer = aUnknow;
		_Log( lCloseButUnknown, SessionID );
	}
qRR
qRT
qRE
	return Answer;
}

void cgimng::tool___::CloseExpired( bso::bool__ Balance )
{
qRH
	lck::read_write_access___<sessions_> Sessions;
qRB
	Sessions.Init( Sessions_ );

	Sessions().CloseExpired();

	if ( Balance )
		Sessions().Balance();

	_Log( lCleaning, str::string() );
qRR
qRT
qRE
}
