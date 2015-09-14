/*
	'esketchbkd.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "scldaemon.h"
#include "sclrgstry.h"
#include "scllocale.h"

#include "sktinf.h"

#include "frdsessn.h"

#include "cgigat.h"
#include "ssnmng.h"

#include "sclcgi.h"
#include "sclmisc.h"

#define NAME	SKTINF_LC_AFFIX	"cgi"

const char *sclmisc::SCLMISCTargetName = NAME;

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
	qRH
	qRH
		Session = new session___;

		if ( Session == NULL )
			qRAlc();

		Session->Init( _Kernel );
	qRR
		if ( Session != NULL )
			delete Session;

		Session = NULL;
	qRT
	qRE
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
		_Kernel.Init( sclrgstry::GetRegistry(), scllocale::GetLocale(), sclcgi::GetLanguage(), _ReportingFunctions );
		_steering_callback__::Init();
	}
};


sclcgi::steering_callback__ *sclcgi::SCLCGICreateSteering( void )
{
	steering_callback__ *SteeringCallback = NULL;
qRH
qRH
	if ( ( SteeringCallback = new ::steering_callback__ ) == NULL )
		qRAlc();

	SteeringCallback->Init();
qRR
	if ( SteeringCallback != NULL )
		delete SteeringCallback;

	SteeringCallback = NULL;
qRT
qRE
	return SteeringCallback;
}


void sclcgi::SCLCGIDeleteSteering( steering_callback__ *SteeringCallback )
{
	delete SteeringCallback;
}
