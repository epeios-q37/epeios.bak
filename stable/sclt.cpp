/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define SCLT_COMPILATION_

#include "sclt.h"

#include "scla.h"
#include "scle.h"
#include "scll.h"
#include "sclm.h"

#include "cio.h"

using namespace sclt;

using cio::COut;
using scll::GetLocale;

static err::err___ qRRor_;
static scle::rError SCLError_;

bso::bool__ sclt::IgnoreUnknownArguments = false;

namespace {
	scll::rRack Locale_;
};

namespace {
	void FillRegistry_(
		int argc,
		ntvstr::char__ *argv[],
		bso::bool__ IgnoreUnknownArguments )
	{
	qRH
		str::strings Arguments;
		int i = 0;
		TOL_CBUFFER___ SBuffer;
	qRB
		Arguments.Init();

		while ( i < argc )
			Arguments.Append( str::string( ntvstr::string___( argv[i++] ).UTF8( SBuffer ) ) );

		scla::FillRegistry( Arguments, scla::faIsCommand, IgnoreUnknownArguments ? scla::uaIgnore : scla::uaReport );
	qRR
	qRT
	qRE
	}
}

static int main_(
	const oddities__ &Oddities,
	const cio::set__ &CIO )
{
	int ExitValue = EXIT_SUCCESS;
qRH
	str::string Command;
	sclm::sRack Rack;
	const scli::sInfo *Info = NULL;
qRB
	Info = &SCLTInfo();

	if ( Info == NULL )
		qRFwk();

	Rack.Init( qRRor_, SCLError_, CIO, Locale_);

	sclm::Initialize( Rack, (const char *)NULL, *Info );

	FillRegistry_( Oddities.argc, Oddities.argv, IgnoreUnknownArguments );

	sclm::LoadProject( *Info );

	sclm::FillSetupRegistry();

	sclm::RefreshBaseLanguage();

	Command.Init();

	if ( scla::GetCommand( Command ) == "Usage" )
		scla::PrintUsage( *Info, cio::COut );
	else
		ExitValue = SCLTMain( Command, Oddities );
qRR
	if ( ERRType >= err::t_amount ) {
		switch ( ERRType ) {
		case err::t_Abort:
			if ( sclm::DisplaySCLBasePendingError() )
				ExitValue = EXIT_FAILURE;
			ERRRst();
			break;
		case err::t_Free:
			ERRRst();
			qRFwk();
			break;
		case err::t_None:
			ERRRst();
			qRFwk();
			break;
		default:
			ERRRst();
			qRFwk();
			break;
		}
	}
	else {
		err::buffer__ Buffer;
		ExitValue = EXIT_FAILURE;
		cio::CErr << err::Message( Buffer ) << txf::nl;
		ERRRst();
	}
qRT
	cio::COut.Commit();
	cio::CErr.Commit();
	cio::CIn.Dismiss();

	sclm::DumpRegistriesAndOrLocalesIfRequired();

	if ( Info != NULL )
		sclm::Quit( *Info );
qRE
	return ExitValue;
}

#ifdef CPE_S_WIN

# undef system	// Defined in 'tol.h', referenced in below header file.

# include <iostream>

static void SetStdOutToNewConsole_( void )
{
	int hCrt;
	FILE *hf;

	AllocConsole();
	hCrt = _open_osfhandle(
		(intptr_t)GetStdHandle( STD_OUTPUT_HANDLE ),
		_O_TEXT
		);
	hf = _fdopen( hCrt, "w" );
	*stdout = *hf;
	setvbuf( stdout, NULL, _IONBF, 0 );
}

// MSVC will use either 'wmain' or 'wWinMain' (and ignore the other) depending of the value of 'SubSystem'.

int wmain(
	int argc,
	wchar_t *argv[] )
{
	int ExitValue = EXIT_SUCCESS;
qRFH
	oddities__ Oddities;
qRFB
	Oddities.argc = argc;
	Oddities.argv = argv;

	ExitValue = main_( Oddities, cio::GetSet( cio::t_Default ) );
qRFR
qRFT
qRFE( sclm::ErrorDefaultHandling() )
	return ExitValue;
}

int WINAPI wWinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		PWSTR pCmdLine,
		int nCmdShow )
{
	int ExitValue = EXIT_SUCCESS;
qRFH
	str::string SOut, SErr;
	flx::bunch_oflow_driver___<str::string_, bso::char__> FOut, FErr;
	flx::void_idriver___ FIn;
	cio::set__ CIO;
	oddities__ Oddities;
qRFB
	Oddities.argv = CommandLineToArgvW( GetCommandLineW(), &Oddities.argc );

	Oddities.hInstance = hInstance;
	Oddities.hPrevInstance = hPrevInstance;
	Oddities.nCmdShow = nCmdShow;
	Oddities.pCmdLine = pCmdLine;

	SOut.Init();
	FOut.Init( SOut, fdr::ts_Default );

	SErr.Init();
	FErr.Init( SErr, fdr::ts_Default );

	FIn.Init( fdr::ts_Default, flx::a_Default );

	CIO.Init( FIn, FOut, FErr, cio::t_Default );

	ExitValue = main_( Oddities, CIO );
qRFR
qRFT
	if ( Oddities.argv != NULL )
		LocalFree( Oddities.argv );

	cio::COut.reset();
	cio::CErr.reset();
	cio::CIn.reset();

	FOut.reset();
	FErr.reset();
	FIn.reset();

# if 0	/// Fait planter 'CEF'.
	if ( SOut.Amount() )
		MessageBoxW( NULL, ntvstr::string___( SOut ).Internal(), ntvstr::string___( sclm::SCLMTargetName ).Internal(), MB_OK );

	if ( SErr.Amount() )
		MessageBoxW( NULL, ntvstr::string___( SErr ).Internal(), ntvstr::string___( sclm::SCLMTargetName ).Internal(), MB_OK | MB_TASKMODAL );
# else
	if ( SOut.Amount() || SErr.Amount() )
	{
		SetStdOutToNewConsole_();

		if ( SOut.Amount() )
			std::wcout << (wchar_t *)ntvstr::string___( SOut ).Internal() << std::endl;

		if ( SErr.Amount() )
			std::wcout << (wchar_t *)ntvstr::string___( SErr ).Internal() << std::endl;
	}
# endif
qRFE( sclm::ErrorDefaultHandling() )
	return ExitValue;
}

#else
int main(
	int argc,
	char *argv[] )
{
	int ExitValue = EXIT_SUCCESS;
qRFH
	oddities__ Oddities;
qRFB
	Oddities.argv = argv;
	Oddities.argc = argc;

	ExitValue = main_( Oddities, cio::GetSet( cio::t_Default ) );
qRFR
qRFT
qRFE(sclm::ErrorDefaultHandling())
	return ExitValue;
}
#endif

Q37_GCTOR( sclt )
{
	sclm::ExitOnSignal();
	qRRor_.Init();
	SCLError_.Init();

	Locale_.Init();
}

