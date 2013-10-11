/*
	'sclcgi' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'sclcgi' header file ('sclcgi.h').
	Copyright (C) 20132004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
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



//	$Id: sclcgi.cpp,v 1.3 2013/05/15 14:40:34 csimon Exp $

#define SCLCGI__COMPILATION

#include "sclcgi.h"

class sclcgitutor
: public ttr_tutor
{
public:
	sclcgitutor( void )
	: ttr_tutor( SCLCGI_NAME )
	{
#ifdef SCLCGI_DBG
		Version = SCLCGI_VERSION "\b\bD $";
#else
		Version = SCLCGI_VERSION;
#endif
		Owner = SCLCGI_OWNER;
		Date = "$Date: 2013/05/15 14:40:34 $";
	}
	virtual ~sclcgitutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "cgigat.h"
#include "ssnmng.h"
#include "scldaemon.h"

using namespace sclcgi;

const char *GetLanguage( void )
{
	return "en";	// A changer.
}

typedef cgigat::cgi_gate__ _gate__;
typedef ssnmng::user_functions__ _user_functions__;

class gate__
: public _gate__,
  public _user_functions__
{
private:
	steering_callback__ *_SteeringCallback;
	ssnmng::sessions_manager _Sessions;
	steering_callback__ &_Callback( void ) const
	{
		if ( _SteeringCallback == NULL )
			ERRFwk();

		return *_SteeringCallback;
	}
protected:
	virtual void SSNMNGDelete( void *UP )
	{
		_Callback().DeleteSession( UP );
	}
	virtual void CGIDATProcess(
		const cgiarg::arguments_ &Arguments,
		xml::writer_ &Writer,
		str::string_ &XSLFileName )
	{
	ERRProlog
		void *UP = NULL;
		ssnmng::row__ SessionRow = E_NIL;
		cgiarg::row__ ArgRow = E_NIL;
		str::string RawSession;
		ssnmng::session_id__ Session;
	ERRBegin
		ArgRow = Arguments.Locate( "_session" );
		RawSession.Init();

		if ( ArgRow == E_NIL )
			SessionRow = _Sessions.New( _Callback().NewSession( Arguments ) );
		else if ( ( SessionRow = _Sessions.Search( Arguments.GetValue( ArgRow, RawSession ) ) ) == E_NIL )
			_Callback().ReportExpiredSession( Writer, XSLFileName );
		else if ( _Sessions.IsExpired( SessionRow ) ) {
			_Sessions.Close( SessionRow );	// Appelle 'sclcgi::SCLCGI::DeleteSession.
			_Callback().ReportExpiredSession( Writer, XSLFileName );
			SessionRow = E_NIL;
		}

		if ( SessionRow != E_NIL ) {
			Session.Init();
			_Sessions.SessionID( SessionRow );

			switch ( _Callback().HandleQuery( UP, Arguments, Writer, XSLFileName ) ) {
			case sclcgi::shKeepAlive:
				break;
			case sclcgi::shClose:
				_Sessions.Close( SessionRow );	// Appelle 'sclcgi::SCLCGI::DeleteSession.
				break;
			default:
				ERRFwk();
				break;
			}
		}
	ERRErr
	ERREnd
	ERREpilog
	}
public:
	void reset( bso::bool__ P = true )
	{
		_gate__::reset( P );
		_Sessions.reset( P );
		_user_functions__::reset( P );
		_SteeringCallback = NULL;
	}
	E_CVDTOR( gate__ )
	void Init( steering_callback__ &SteeringCallback )
	{
		_gate__::Init();
		_user_functions__::Init();
		_Sessions.Init( 60, 60, *this );
		_SteeringCallback = &SteeringCallback;
	}
};

csdleo::user_functions__ *scldaemon::RetrieveSteering(
	csdleo::mode__ Mode,
	const lcl::locale_ &Locale )
{
	gate__ *Gate = NULL;
ERRProlog
	steering_callback__ *Callback = NULL;
ERRBegin
	Callback = SCLCGICreateSteering();

	if ( Callback == NULL )
		ERRReturn;

	if ( ( Gate = new gate__ ) == NULL )
		ERRAlc();

	Gate->Init( *Callback );
ERRErr
	if ( Gate != NULL )
		delete Gate;

	Gate = NULL;
ERREnd
ERREpilog
	return Gate;
}

void scldaemon::ReleaseSteering( csdleo::user_functions__ *Steering )
{
	delete Steering;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class sclcgipersonnalization
: public sclcgitutor
{
public:
	sclcgipersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sclcgipersonnalization( void )
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

static sclcgipersonnalization Tutor;

ttr_tutor &SCLCGITutor = Tutor;
