/*
	'esketchbkd.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id: esketchcgi.cpp,v 1.4 2013/07/25 15:59:07 csimon Exp $

#include "scldaemon.h"
#include "sclrgstry.h"
#include "scllocale.h"

#include "sktinf.h"

#include "frdsessn.h"

#include "cgigat.h"
#include "ssnmng.h"

#include "sclcgi.h"

#define NAME	SKTINF_LC_AFFIX	"cgi"

const char *scldaemon::TargetName = NAME;

using frdsessn::session___;

typedef sclcgi::steering_callback__ _steering_callback__;

typedef frdkrn::reporting_functions__ _reporting_functions__;

class reporting_functions__
: public _reporting_functions__
{
protected:
	virtual void FRDKRNReport( const str::string_ &Message ) {}
	virtual void FRDKRNNotify( const str::string_ &Message ) {}
};

class steering_callback__
: public _steering_callback__
{
private:
	frdkernl::kernel___ _Kernel;
	reporting_functions__ _ReportingFunctions;
protected:
	virtual void *SCLCGINewSession( const cgiarg::arguments_ &Arguments )
	{
		session___ *Session = NULL;
	ERRProlog
	ERRBegin
		Session = new session___;

		if ( Session == NULL )
			ERRAlc();

		Session->Init( _Kernel );
	ERRErr
		if ( Session != NULL )
			delete Session;

		Session = NULL;
	ERREnd
	ERREpilog
		return Session;
	}
	virtual sclcgi::session_handling__ SCLCGIHandleQuery(
			void *UP,
			const cgiarg::arguments_ &Arguments,
			xml::writer_ &Writer,
			str::string_ &XSLFileName )
	{
		Writer.PushTag( "Handle" );
		XSLFileName = "Test";

		return sclcgi::shKeepAlive;
	}
	virtual void SCLCGIReportExpiredSession(
		xml::writer_ &Writer,
		str::string_ &XSLFileName )
	{
		Writer.PushTag( "End" );
		XSLFileName = "Test";
	}
	virtual void SCLCGIDeleteSession( void *UP )
	{
		if( UP == NULL )
			ERRPrm();

		delete (session___ *)UP;
	}
public:
	void reset( bso::bool__ P = true )
	{
		_Kernel.reset( P );
		_steering_callback__::reset( P );
		_ReportingFunctions.reset( P );
	}
	E_CVDTOR( steering_callback__ )
	void Init( void )
	{
		_ReportingFunctions.Init( _Kernel );
		_Kernel.Init( sclrgstry::GetRegistry(), sclrgstry::GetRoot(), scllocale::GetLocale(), sclcgi::GetLanguage(), _ReportingFunctions );
		_steering_callback__::Init();
	}
};


sclcgi::steering_callback__ *sclcgi::SCLCGICreateSteering( void )
{
	steering_callback__ *SteeringCallback = NULL;
ERRProlog
ERRBegin
	if ( ( SteeringCallback = new ::steering_callback__ ) == NULL )
		ERRAlc();

	SteeringCallback->Init();
ERRErr
	if ( SteeringCallback != NULL )
		delete SteeringCallback;

	SteeringCallback = NULL;
ERREnd
ERREpilog
	return SteeringCallback;
}


void sclcgi::SCLCGIDeleteSteering( steering_callback__ *SteeringCallback )
{
	delete SteeringCallback;
}
