/*
	'csdsnc' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'csdsnc' header file ('csdsnc.h').
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id: csdsnc.cpp,v 1.47 2013/07/26 10:45:22 csimon Exp $

#define CSDSNC__COMPILATION

#include "csdsnc.h"

class csdsnctutor
: public ttr_tutor
{
public:
	csdsnctutor( void )
	: ttr_tutor( CSDSNC_NAME )
	{
#ifdef CSDSNC_DBG
		Version = CSDSNC_VERSION "\b\bD $";
#else
		Version = CSDSNC_VERSION;
#endif
		Owner = CSDSNC_OWNER;
		Date = "$Date: 2013/07/26 10:45:22 $";
	}
	virtual ~csdsnctutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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
		ERRPrm();
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
				ERRDta();

			Flow.Dismiss();
		}
}

void csdsnc::core_::Ping( void )
{
	_Lock( S_.MainMutex );

	stk::row__ Row = Flows.First();
	
	while ( Row != E_NIL )
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
		ERRFwk();

	mtk::Launch( ::KeepAlive_, this );
#else
	if ( Delay != 0 )
		ERRPrm();
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

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class csdsncpersonnalization
: public csdsnctutor
{
public:
	csdsncpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~csdsncpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static csdsncpersonnalization Tutor;

ttr_tutor &CSDSNCTutor = Tutor;
