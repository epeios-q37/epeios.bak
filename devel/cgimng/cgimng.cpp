/*
	'cgimng.cpp' by Claude SIMON (http://zeusw.org/).

	'cgimng' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#define CGIMNG__COMPILATION

#include "cgimng.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

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
		ERRPrm();
		return NULL;	// Pour éviter un 'warning'.
		break;
	}

}

csdleo::action__ cgimng::core_manager::CSDSCBProcess(
	flw::ioflow__ &Client,
	void *UP )
{
ERRProlog
	cgiarg::arguments CGIArguments;
ERRBegin
#ifdef CGIMNG_DBG
	if ( UP != NULL )
		ERRFwk();
#endif

	CGIArguments.Init();
	CGIArguments.Parse( Client );

	UserFunctions_->Handle( CGIArguments, *this, Client );
ERRErr
ERREnd
ERREpilog
	return csdleo::aStop;
}


struct data__ {
	void *UP;
	mtx::mutex_handler__ Mutex;
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
#		pragma message ( __LOC__ "bogue toujours non résolu ; contournement temporaire !" )
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
ERRProlog
	ssnmng::row__ P = E_NIL;
	session_id__ SID;
	lck::read_write_access___<sessions_> Sessions;
	data__ *Data = NULL;
ERRBegin
	Sessions.Init( Sessions_ );

	Data = new data__;

	if ( Data == NULL )
		ERRAlc();

	Data->UP = UP;
	Data->Mutex = mtx::Create( mtx::mProtecting );
	Data->InUse = true;

	SID.Init();

	tol::InitializeRandomGenerator();

	P = Sessions().New( Data );
//	Sessions().Store( Data, P );
	SID = Sessions().SessionID( P );

	SessionID = SID.Value();

	mtx::Lock( Data->Mutex );

	_Log( lCreate, SessionID );
ERRErr
ERREnd
ERREpilog
}

answer__ cgimng::tool___::GetAndLock(
	const str::string_ &SessionID,
	void *&UP )
{
	answer__ Answer = aUndefined;
ERRProlog
	ssnmng::row__ P = E_NIL;
	lck::read_write_access___<sessions_> Sessions;
ERRBegin
	Sessions.Init( Sessions_ );

	P = Sessions().Search( SessionID );

	if ( P != E_NIL ) {

		data__ &Data =  *(data__ *)Sessions().Pointers( P );

		Data.InUse = true;

		if ( !mtx::TryToLock( Data.Mutex ) ) {
			Sessions.reset();	// Pour déverouller 'Sessions'.
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
ERRErr
ERREnd
ERREpilog
	return Answer;
}

void cgimng::tool___::Release( const str::string_ &SessionID )
{
ERRProlog
	ssnmng::row__ P = E_NIL;
	lck::read_write_access___<sessions_> Sessions;
ERRBegin
	Sessions.Init( Sessions_ );

	if ( ( P = Sessions().Search( SessionID ) ) != E_NIL )	{

		data__ &Data =  *(data__ *)Sessions().Pointers( P );

#ifdef CGIMNG_DBG
		if ( !mtx::IsLocked( Data.Mutex ) )
			ERRFwk();
#endif

		Sessions().Touch( P );

		Data.InUse = false;

		mtx::Unlock( Data.Mutex );

		_Log( lRelease, SessionID );
	} else
		ERRFwk();
ERRErr
ERREnd
ERREpilog
}


answer__ cgimng::tool___::Close( const str::string_ &SessionID )
{
	answer__ Answer = aUndefined;
ERRProlog
	ssnmng::row__ P = E_NIL;
	lck::read_write_access___<sessions_> Sessions;
ERRBegin
	Sessions.Init( Sessions_ );

	P = Sessions().Search( SessionID );

	if ( P != E_NIL ) {

		data__ &Data =  *(data__ *)Sessions().Pointers( P );

		if ( !mtx::TryToLock( Data.Mutex ) ) {
			Answer = aLocked;
			_Log( lCloseButLocked, SessionID );
			ERRReturn;	// To avoid the below 'mtx::Unlock'.
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
ERRErr
ERREnd
ERREpilog
	return Answer;
}

void cgimng::tool___::CloseExpired( bso::bool__ Balance )
{
ERRProlog
	lck::read_write_access___<sessions_> Sessions;
ERRBegin
	Sessions.Init( Sessions_ );

	Sessions().CloseExpired();

	if ( Balance )
		Sessions().Balance();

	_Log( lCleaning, str::string() );
ERRErr
ERREnd
ERREpilog
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class cgimngpersonnalization
{
public:
	cgimngpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~cgimngpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static cgimngpersonnalization Tutor;
