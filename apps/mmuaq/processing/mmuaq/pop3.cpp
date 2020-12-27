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

#include "sclm.h"

#include "bso.h"
#include "csdbnc.h"
#include "err.h"

using namespace pop3;

using namespace muapo3;

#define M( name ) qCDEF( char *, name, #name );
namespace message_ {
	M( Error );
	M( ErrorWithNoMessage );
	M( ErroneousAnswer );
}

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
			sclm::ReportAndAbort( message_::Error, Message );
		else
			sclm::ReportAndAbort( message_::ErrorWithNoMessage );
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
			sclm::ReportAndAbort( message_::ErroneousAnswer );
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
		sclm::MGetValue( registry::parameter::pop3::Username, Username );
		sclm::MGetValue( registry::parameter::pop3::Password, Password );
	}

	class rVerboseIODriver_
	: public misc::rVerboseIODriver
	{
	public:
		eIndicator InitAndAuthenticate(
			hBody &Body,
			bso::sBool Activate )
		{
			base::eIndicator Indicator = i_Undefined;
		qRH
			str::wString Username, Password;
			qCBUFFERh Buffer;
		qRB
			tol::Init( Username, Password );

			GetUsernameAndPassword_( Username, Password );

			misc::rVerboseIODriver::Init( registry::parameter::pop3::HostPort, Activate ? flx::cOut : flx::cNone );

			Indicator = base::Authenticate( Username, Password, *this, Body );
		qRR
		qRT
		qRE
			return Indicator;
		}
	};
}

void pop3::List( void )
{
qRH
	rVerboseIODriver_ VerboseDriver;
	muapo3::hBody Body;
	bso::sUInt Number = 0;
	bso::sBool Verbose = false;
qRB
	Verbose = misc::IsVerboseActivated();

	Handle_( VerboseDriver.InitAndAuthenticate( Body, Verbose ), Body );

	Number = sclm::OGetUInt( registry::parameter::MailID, 0 );

	Handle_( muapo3::base::List( Number, VerboseDriver, false, Body ), Body );

	misc::Dump( Body.GetDriver() );

	Handle_( muapo3::base::Quit( VerboseDriver, Body ), Body );
qRR
qRT
qRE
}

void pop3::Retrieve( void )
{
qRH
	rVerboseIODriver_ VerboseDriver;
	muapo3::hBody Body;
	bso::sUInt Number = 0;
	bso::sBool Verbose = false;
qRB
	Verbose = misc::IsVerboseActivated();

	Handle_( VerboseDriver.InitAndAuthenticate( Body, Verbose ), Body );

	Number = sclm::MGetUInt( registry::parameter::MailID );

	Handle_( muapo3::base::Retrieve( Number, VerboseDriver, !Verbose, Body ), Body );

	misc::Dump( Body.GetDriver() );

	Handle_( muapo3::base::Quit( VerboseDriver, Body ), Body );
qRR
qRT
qRE
}

void pop3::Top( void )
{
qRH
	rVerboseIODriver_ VerboseDriver;
	muapo3::hBody Body;
	bso::sUInt Number = 0, Lines = 0;
	bso::sBool Verbose = false;
qRB
	Verbose = misc::IsVerboseActivated();

	Handle_( VerboseDriver.InitAndAuthenticate( Body, Verbose ), Body );

	Number = sclm::MGetUInt( registry::parameter::MailID );
	Lines = sclm::MGetUInt( registry::parameter::Lines );

	Handle_( muapo3::base::Top( Number, Lines, VerboseDriver, !Verbose, Body ), Body );

	misc::Dump( Body.GetDriver() );

	Handle_( muapo3::base::Quit( VerboseDriver, Body ), Body );
qRR
qRT
qRE
}

void pop3::UIDL( void )
{
qRH
	rVerboseIODriver_ VerboseDriver;
	muapo3::hBody Body;
	bso::sUInt Number = 0;
	bso::sBool Verbose = false;
qRB
	Verbose = misc::IsVerboseActivated();

	Handle_( VerboseDriver.InitAndAuthenticate( Body, Verbose ), Body );

	Number = sclm::OGetUInt( registry::parameter::MailID, 0 );

	Handle_( muapo3::base::UIDL( Number, VerboseDriver, Body ), Body );

	misc::Dump( Body.GetDriver() );

	Handle_( muapo3::base::Quit( VerboseDriver, Body ), Body );
qRR
qRT
qRE
}
