/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#include "registry.h"

#include "muapo3.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "csdbnc.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

# define NAME_MC			"MMUAq"
# define NAME_LC			"mmuaq"
# define NAME_UC			"MMUAQ"
# define WEBSITE_URL		"http://q37.info"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}

	void Test_( void )
	{
		cio::COut << "Test" << txf::nl;
	}
	void B64Encode_( void )
	{
	qRH
		str::wString Input, Output;
		sclmisc::rIDriverRack IRack;
		sclmisc::rODriverRack ORack;
		cdgb64::rEncodingODriver Encoder;
	qRB
		Input.Init();
		sclmisc::OGetValue( registry::parameter::Input, Input );

		Output.Init();
		sclmisc::OGetValue( registry::parameter::Output, Output );

		Encoder.Init( ORack.Init( Output ), cdgb64::fOriginal );
		fdr::Copy( IRack.Init( Input ), Encoder );
	qRR
		IRack.HandleError();
		ORack.HandleError();
	qRT
	qRE
	}

	void B64Decode_( void )
	{
	qRH
		str::wString Input, Output;
		sclmisc::rIDriverRack IRack;
		sclmisc::rODriverRack ORack;
		cdgb64::rDecodingIDriver Decoder;
	qRB
		Input.Init();
		sclmisc::OGetValue( registry::parameter::Input, Input );

		Output.Init();
		sclmisc::OGetValue( registry::parameter::Output, Output );

		Decoder.Init( IRack.Init( Input ) );
		fdr::Copy( Decoder, ORack.Init( Output ) );
	qRR
		IRack.HandleError();
		ORack.HandleError();
	qRT
	qRE
	}

	namespace { 
		void Dump_( flw::sIFlow &Flow )
		{
			while ( !Flow.EndOfFlow() )
				cio::COut << Flow.Get() << txf::commit;

			Flow.Dismiss();
		}

		void Dump_( fdr::rIDriver &Driver )
		{
		qRH
			flw::sIFlow Flow;
		qRB
			Flow.Init( Driver );

			Dump_( Flow );
		qRR
		qRT
		qRE
		}
	}

	namespace pop3_ {
		bso::sBool InitAndAuthenticate_(
			csdbnc::rIODriver &Server,
			muapo3::hBody &Body )
		{
			bso::sBool Success = false;
		qRH
			str::wString HostPort, Username, Password;
			qCBUFFERr Buffer;
		qRB
			HostPort.Init();
			sclmisc::MGetValue( registry::parameter::HostPort, HostPort );

			Username.Init();
			sclmisc::MGetValue( registry::parameter::Username, Username );

			Password.Init();
			sclmisc::MGetValue( registry::parameter::Password, Password );

//			Buffer.Init();
			Server.Init( HostPort.Convert( Buffer ) , SCK_INFINITE, err::hThrowException );

			Success = muapo3::Authenticate( Username, Password, Server, Body );
		qRR
		qRT
		qRE
			return Success;
		}

		void Report( muapo3::hBody &Body )
		{
			cio::COut << Body.Message << txf::nl << txf::commit;
			qRAbort();
		}
	}

#define REPORT pop3_::Report( Body )

	void POP3List_( void )
	{
	qRH
		csdbnc::rIODriver Server;
		muapo3::hBody Body;
	qRB
		if ( !pop3_::InitAndAuthenticate_( Server, Body ) )
			REPORT;

		if ( !muapo3::List( Server, Body ) )
			REPORT;

		Dump_( Body.GetFlow() );

		if ( !muapo3::Quit( Server, Body ) )
			REPORT;
	qRR
	qRT
	qRE
	}

	void POP3Retrieve_( void )
	{
	qRH
		csdbnc::rIODriver Server;	
		muapo3::hBody Body;
		bso::sUInt Number = 0;
	qRB
		if ( !pop3_::InitAndAuthenticate_( Server, Body ) )
			REPORT;

		Number = sclmisc::MGetUInt( registry::parameter::Message );

		if ( !muapo3::Retrieve( Number, Server, Body ) )
			REPORT;

		Dump_( Body.GetFlow() );

		if ( !muapo3::Quit( Server, Body ) )
			REPORT;
	qRR
	qRT
	qRE
	}

	void POP3Top_( void )
	{
	qRH
		csdbnc::rIODriver Server;	
		muapo3::hBody Body;
		bso::sUInt Number = 0, Lines = 0;
	qRB
		if ( !pop3_::InitAndAuthenticate_( Server, Body ) )
			REPORT;

		Number = sclmisc::MGetUInt( registry::parameter::Message );
		Lines = sclmisc::MGetUInt( registry::parameter::Lines );

		if ( !muapo3::Top( Number, Lines, Server, Body ) )
			REPORT;

		Dump_( Body.GetFlow() );

		if ( !muapo3::Quit( Server, Body ) )
			REPORT;
	qRR
	qRT
	qRE
	}
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

int scltool::SCLTOOLMain(
	const str::dString &Command,
	const scltool::fOddities &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH
qRB
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( Test );
	C( B64Encode );
	C( B64Decode );
	C( POP3List );
	C( POP3Retrieve );
	C( POP3Top );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;

