/*
	Copyright (C) 2014-2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

    This file is part of 'xdhcefq'.

    'xdhcefq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhcefq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhcefq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "misc.h"
#include "registry.h"
#include "browser.h"
#include "render.h"
#include "files.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

#ifdef CPE_S_LINUX
# include <X11/Xlib.h>
#endif

using cio::CErr;
using cio::COut;
using cio::CIn;

static void PrintHeader_( void )
{
	COut << MISC_NAME_MC " V" MISC_VERSION << " (" MISC_WEBSITE_URL ")" << txf::nl;
	COut << "Copyright " MISC_COPYRIGHT << txf::nl;
	COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " - " << cpe::GetDescription() << txf::nl;
}

struct cef_setting_buffer___
{
public:
	misc::cef_string___ String;
	void reset( bso::bool__ P = true )
	{
		String.reset( P );
	}
	E_CDTOR( cef_setting_buffer___ );
	cef_string_t  Init( const rgstry::entry___ &Entry )
	{
	qRH
		misc::nstring___ NString;
		TOL_CBUFFER___ Buffer;
	qRB
		NString.Init( sclmisc::OGetValue( Entry, Buffer ) );
		String.Init( NString );
	qRR
	qRT
	qRE
		return String;
	}
};

struct cef_setting_buffers___
{
public:
	cef_setting_buffer___
		ResourceDirPath,
		CachePath,
		LocaleDirPath,
		Locale,
		LogFile,
		LogSeverity;
	void reset( bso::bool__ P = true )
	{
		ResourceDirPath.reset( P );
		CachePath.reset( P );
		LocaleDirPath.reset( P );
		Locale.reset( P );
		LogFile.reset( P );
		LogSeverity.reset( P );
	}
	E_CDTOR( cef_setting_buffers___ );
	// Initializations occurs per member when needed.
};

class rack__ {
	public:
		cef_app_t App;
		cef_setting_buffers___ CEFSettingBuffers;
		void reset( bso::bool__ P = true )
		{
			CEFSettingBuffers.reset( P );
		}
		E_CDTOR( rack__ );
		// The members are intiialized when required.
} Rack_;

#ifdef SANDBOX
# include "cef_sandbox_win.h"
#endif

void FillSettings_( cef_settings_t &Settings )
{
	Settings.single_process = sclmisc::BGetBoolean( registry::cef::SingleProcess );
	Settings.no_sandbox = sclmisc::BGetBoolean( registry::cef::NoSandbox );
	Settings.resources_dir_path = Rack_.CEFSettingBuffers.ResourceDirPath.Init( registry::cef::ResourcesDirPath );
	Settings.locales_dir_path = Rack_.CEFSettingBuffers.LocaleDirPath.Init( registry::cef::LocaleDirPath );
	Settings.locale = Rack_.CEFSettingBuffers.Locale.Init( registry::cef::Locale );
	Settings.cache_path = Rack_.CEFSettingBuffers.CachePath.Init( registry::cef::CachePath );
//	Settings.log_file = Rack_.CEFSettingBuffers.LogFile.Init( registry::cef::LogFile );
//	Settings.log_severity = LOGSEVERITY_VERBOSE;
}

#ifdef CPE_S_OSX
// NOTA : functions are defined in file 'cefosx.mm'.
# define F( name ) void name( void );
#else
# define F( name ) inline void name( void ) {} 
#endif

F( Pre );
F( Main );
F( Post );

#ifdef CPE_S_LINUX
int XErrorHandlerImpl(Display *display, XErrorEvent *event) {
  return 0;
}

int XIOErrorHandlerImpl(Display *display) {
  return 0;
}
#endif


static int Launch_( const scltool::oddities__ &Oddities )
{
	int ExitValue = EXIT_SUCCESS;
qRH
	cef_main_args_t MainArgs;
	cef_settings_t Settings;
	void *sandbox_info = NULL;
	str::string ProcessType;
qRB
#ifdef CPE_S_WIN
	MainArgs.instance = Oddities.hInstance;
#elif defined( CPE_S_POSIX )
	MainArgs.argc = Oddities.argc;
	MainArgs.argv = Oddities.argv;
#else
# error
#endif
	misc::Set( &Rack_.App );
	Rack_.App.get_browser_process_handler = browser::GetBrowserProcessHandler;
	Rack_.App.get_render_process_handler = render::GetRenderProcessHandler;

	misc::Set( &Settings );
	FillSettings_( Settings );

#ifdef SANDBOX
	sandbox_info = cef_sandbox_info_create();
#endif

	ProcessType.Init();

	sclmisc::OGetValue( registry::cef::ProcessType, ProcessType );

	if ( ( ProcessType.Amount() == 0 ) || ( ProcessType == "Sub" ) ) {

		ExitValue = cef_execute_process( &MainArgs, &Rack_.App, NULL );

		if ( ExitValue >= 0 )
			qRReturn;
	}

#ifdef CEP_S_LINUX
	XSetErrorHandler(XErrorHandlerImpl);
	XSetIOErrorHandler(XIOErrorHandlerImpl);
#endif

	
	if ( ( ProcessType.Amount() == 0 ) || ( ProcessType == "Main" ) ) {
		Pre();

		cef_initialize( &MainArgs, &Settings, &Rack_.App, NULL );

		Main();

		cef_run_message_loop();

		cef_shutdown();

		Post();
	}

qRR
qRT
#ifdef SANDBOX
	if ( sandbox_info == NULL )
		cef_sandbox_info_destroy( sandbox_info );
#endif
qRE
	return ExitValue;
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

int scltool::SCLTOOLMain(
	const str::string_ &Command,
	const scltool::oddities__ &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH
qRB
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( MISC_NAME_MC );
	else if ( Command == "Launch" )
		Launch_( Oddities );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}

const char *sclmisc::SCLMISCTargetName = MISC_NAME_LC;

Q37_GCTOR( xdhcefq )
{
	scltool::IgnoreUnknownArguments = true;
}
