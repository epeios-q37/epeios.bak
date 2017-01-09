/*
	Copyright (C) 2016-2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq'.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "pop3.h"

#include "misc.h"
#include "registry.h"

#include "muapo3.h"

#include "sclmisc.h"

#include "bso.h"
#include "csdbnc.h"
#include "err.h"

using namespace pop3;

using namespace muapo3;
namespace {
	using namespace base;

	void ReportError_( hBody &Body )
	{
	qRH
		str::wString Message;
	qRB
		Message.Init();

		misc::Dump( Body.GetDriver(), Message );

		if ( Message.Amount() )
			sclmisc::ReportAndAbort("Error", Message );
		else
			sclmisc::ReportAndAbort( "ErrorWithNoMessage" );
	qRR
	qRT
	qRE
	}

	void Handle_(
		eIndicator Indicator,
		hBody &Body )
	{
		switch ( Indicator.Value() ) {
		case iOK:
			break;
		case iError:
			ReportError_( Body );
			break;
		case iErroneous:
			sclmisc::ReportAndAbort( "ErroneousAnswer" );
			break;
		default:
			qRGnr();
			break;
		}
	}

	void GetUsernameAndPassword_(
		str::dString &Username,
		str::dString &Password )
	{
		sclmisc::MGetValue( registry::parameter::pop3::Username, Username );
		sclmisc::MGetValue( registry::parameter::pop3::Password, Password );
	}

	void Init_( csdbnc::rIODriver &Server )
	{
	qRH
		str::wString HostPort;
		qCBUFFERr Buffer;
	qRB
		HostPort.Init();
		sclmisc::MGetValue( registry::parameter::pop3::HostPort, HostPort );

		if ( !Server.Init( HostPort.Convert( Buffer ), SCK_INFINITE, qRPU ) )
			sclmisc::ReportAndAbort("UnableToConnect", HostPort );
	qRR
	qRT
	qRE
	}

	eIndicator InitAndAuthenticate_(
		csdbnc::rIODriver &Server,
		hBody &Body )
	{
		base::eIndicator Indicator = i_Undefined;
	qRH
		str::wString Username, Password;
		qCBUFFERr Buffer;
	qRB
		tol::Init( Username, Password );

		GetUsernameAndPassword_( Username, Password );

		Init_( Server );

		Indicator = base::Authenticate( Username, Password, Server, Body );
	qRR
	qRT
	qRE
		return Indicator;
	}

	bso::sBool InitAndAuthenticate( csdbnc::rIODriver &Server )
	{
		bso::sBool Success = false;
	qRH
		hBody Body;
	qRB
		Success = InitAndAuthenticate_(Server, Body ).Boolean();
	qRR
	qRT
	qRE
		return Success;
	}

}

void pop3::List( void )
{
qRH
	csdbnc::rIODriver Server;
	muapo3::hBody Body;
	bso::sUInt Number = 0;
qRB
	Handle_( InitAndAuthenticate_( Server, Body ), Body );

	Number = sclmisc::OGetUInt( registry::parameter::Message, 0 );

	Handle_( muapo3::base::List( Number, Server, false, Body ), Body );

	misc::Dump( Body.GetDriver() );

	Handle_( muapo3::base::Quit( Server, Body ), Body );
qRR
qRT
qRE
}

void pop3::Retrieve( void )
{
qRH
	csdbnc::rIODriver Server;	
	muapo3::hBody Body;
	bso::sUInt Number = 0;
qRB
	Handle_( InitAndAuthenticate_( Server, Body ), Body );

	Number = sclmisc::MGetUInt( registry::parameter::Message );

	Handle_( muapo3::base::Retrieve( Number, Server, !sclmisc::BGetBoolean( registry::parameter::KeepAnswer, false ), Body ), Body );

	misc::Dump( Body.GetDriver() );

	Handle_( muapo3::base::Quit( Server, Body ), Body );
qRR
qRT
qRE
}

void pop3::Top( void )
{
qRH
	csdbnc::rIODriver Server;	
	muapo3::hBody Body;
	bso::sUInt Number = 0, Lines = 0;
qRB
	Handle_( InitAndAuthenticate_( Server, Body ), Body );

	Number = sclmisc::MGetUInt( registry::parameter::Message );
	Lines = sclmisc::MGetUInt( registry::parameter::Lines );

	Handle_( muapo3::base::Top( Number, Lines, Server, !sclmisc::BGetBoolean( registry::parameter::KeepAnswer, false ), Body ), Body );

	misc::Dump( Body.GetDriver() );

	Handle_( muapo3::base::Quit( Server, Body ), Body );
qRR
qRT
qRE
}

void pop3::UIDL( void )
{
qRH
	csdbnc::rIODriver Server;	
	muapo3::hBody Body;
	bso::sUInt Number = 0;
qRB
	Handle_( InitAndAuthenticate_( Server, Body ), Body );

	Number = sclmisc::OGetUInt( registry::parameter::Message, 0 );

	Handle_( muapo3::base::UIDL( Number, Server, Body ), Body );

	misc::Dump( Body.GetDriver() );

	Handle_( muapo3::base::Quit( Server, Body ), Body );
qRR
qRT
qRE
}
