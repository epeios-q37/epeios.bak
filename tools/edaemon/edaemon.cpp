/*
	'edaemon' by Claude SIMON (claude.simon@zeusw.org)
	XML Preprocessor.
	Copyright (C) 2007-2011 Claude SIMON

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'edaemon'.

    'edaemon' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'edaemon' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'edaemon'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "clnarg.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "dir.h"
#include "mtk.h"

#include "csdleo.h"
#include "csdbns.h"
#include "csdsns.h"
#include "csdlec.h"

#include "registry.h"


using cio::CErr;
using cio::COut;
using cio::CIn;

#define NAME_MC				"eDaemon"
#define NAME_LC				"edaemon"
#define NAME_UC				"EDAEMON"
#define WEBSITE_URL			"http://zeusw.org/intl/"
#define AUTHOR_NAME			"Claude SIMON"
#define AUTHOR_CONTACT		"http://zeusw.org/intl/contact.html"
#define OWNER_NAME			"Claude SIMON"
#define OWNER_CONTACT		"http://zeusw.org/intl/contact.html"
#define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	

/*
Ne pas modifier ; utilisé pour la génération automatique.
END
*/
/* Beginning of the part which handles command line arguments. */

enum exit_value__ {
	evSuccess = EXIT_SUCCESS,
	evGenericFailure = EXIT_FAILURE,
	// Erreur dans les paramètres d'entrée.
	evParameters,
	// Erreur lors de l'ouverture des fichiers d'entrée ou de sortie.
	evInputOutput,
	// Erreur lors du traitement.
	evProcessing,
	ev_amount
};

enum command__ {
	cProcess = scltool::c_amount,
	c_amount,
	c_Undefined
};

enum option {
};

#define STRING_PARAM___( name )	CLNARG_STRING_PARAM___( name )

struct parameters___ {
	STRING_PARAM___( Project );
	parameters___( void )
	{
	}
};

#pragma warning ( disable : 4101 ) 
static void PrintUsage_( const clnarg::description_ &Description )
{
ERRProlog
	str::string Translation;
ERRBegin
	scltool::PrintDefaultCommandDescriptions( NAME_MC, Description );

	COut << NAME_LC << " [<project-file>]";
	COut << txf::nl;
	COut << txf::nl;

	COut << txf::pad << "project-file :" << txf::nl;
	COut << txf::tab;
	Translation.Init();
	COut << scltool::GetTranslation( "ProjectFileArgumentDescription", Translation ) << '.' << txf::nl;
ERRErr
ERREnd
ERREpilog
}
#pragma warning ( default : 4101 ) 

static void PrintHeader_( void )
{
	COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
	COut << "Copyright " COPYRIGHT << txf::nl;
	COut << txf::pad << "Build : "__DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
}

static void AnalyzeOptions_(
	clnarg::analyzer___ &Analyzer,
	parameters___ &Parameters )
{
ERRProlog
	sdr::row__ P;
	clnarg::option_list Options;
	clnarg::id__ Option;
	const bso::char__ *Unknown = NULL;
	clnarg::argument Argument;
//	clnarg::buffer__ Buffer;
ERRBegin
	Options.Init();

	if ( ( Unknown = Analyzer.GetOptions( Options ) ) != NULL )
		clnarg::ReportUnknownOptionError( Unknown, NAME_LC, scllocale::GetLocale(), scltool::GetLanguage() );

	P = Options.First();

	while( P != E_NIL ) {
		Argument.Init();

		switch( Option = Options( P ) ) {
		default:
			ERRFwk();
		}

		P = Options.Next( P );
	}

ERRErr
ERREnd
ERREpilog
}

static void AnalyzeFreeArguments_(
	clnarg::analyzer___ &Analyzer,
	parameters___ &Parameters )
{
ERRProlog
	clnarg::arguments Free;
	sdr::row__ P;
ERRBegin
	Free.Init();

	Analyzer.GetArguments( Free );

	P = Free.Last();

	switch( Free.Amount() ) {
	case 1:
		Free.Get( Free.Last() ).Convert( Parameters.Project );
		break;
	case 0:
		break;
	default:
		clnarg::ReportWrongNumberOfArgumentsError( NAME_LC, scllocale::GetLocale(), scltool::GetLanguage() );
		break;
	}

ERRErr
ERREnd
ERREpilog
}

static command__ AnalyzeArgs_(
	int argc,
	const char *argv[],
	parameters___ &Parameters )
{
	command__ Command = c_Undefined;
ERRProlog
	clnarg::description Description;
	clnarg::analyzer___ Analyzer;
ERRBegin
	Description.Init();

	scltool::AddDefaultCommands( Description );

//	Description.AddCommand( '', "", c );
//	Description.AddOption( '', "", o );

	Analyzer.Init( argc, argv, Description );

	switch ( Command = (command__)Analyzer.GetCommand() ) {
	case scltool::cVersion:
		PrintHeader_();
//		TTR.Advertise( COut );
		ERRExit( EXIT_SUCCESS );
		break;
	case scltool::cHelp:
		PrintUsage_( Description );
		ERRExit( EXIT_SUCCESS );
		break;
	case scltool::cLicense:
		epsmsc::PrintLicense( COut );
		ERRExit( EXIT_SUCCESS );
		break;
//	case c:
	case CLNARG_NONE:
//		clnarg::ReportMissingCommandError( NAME, scllocale::GetLocale(), scltool::GetLanguage() );
		break;
	default:
		ERRFwk();
		break;
	}

	AnalyzeOptions_( Analyzer, Parameters );

	AnalyzeFreeArguments_( Analyzer, Parameters );

ERRErr
ERREnd
ERREpilog
	return Command;
}

/* End of the part which handles command line arguments. */

enum module_connection_type__ {
	mctStraight,	
	mctSwitched,
	mct_amount,
	mct_Undefined
};

static module_connection_type__ GetModuleConnectionType_( void )
{
	module_connection_type__ Type = mct_Undefined;
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	registry::GetRawModuleServiceType( Value );

	if ( Value == "Straight" )
		Type = mctStraight;
	else if ( Value == "Switched" )
		Type = mctSwitched;
	else {
		Value.Init();
		sclrgstry::ReportBadOrNoValueForEntryError( registry::ModuleServiceType );
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
	return Type;
}

enum log_file_handling__ {
	lfhAppend,	// New log are appended to the current ones.
	lfhDrop,		// Old logs are destroyed.
	lfh_amount,
	lfh_Undefined,
};

static log_file_handling__ GetLogFileHandling_( void )
{
	log_file_handling__ Handling = lfh_Undefined;
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	registry::GetRawModuleLogMode( Value );

	if ( ( Value.Amount() == 0 ) ||( Value == "Append" ) )
		Handling = lfhAppend;
	else if ( Value == "Drop" )
		Handling = lfhDrop;
	else {
		sclrgstry::ReportBadOrNoValueForEntryError( registry::ModuleLogMode );
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
	return Handling;
}

static void UseStraightConnections_(
	csdsuf::user_functions__ &UserFunctions,
	const bso::char__ *Module,
	csdbns::port__ Port )
{
ERRProlog
	csdbns::server___ Server;
ERRBegin
	Server.Init( Port, UserFunctions );

	Server.Process();
ERRErr
ERREnd
ERREpilog
}

static void UseSwitchingConnections_(
	csdsuf::user_functions__ &UserFunctions,
	csdsns::log_functions__ &LogFunctions,
	const bso::char__ *Module,
	csdbns::port__ Port )
{
ERRProlog
	csdsns::server___ Server;
ERRBegin
	Server.Init( Port, UserFunctions, LogFunctions );

	Server.Process();
ERRErr
ERREnd
ERREpilog
}

typedef csdsns::log_functions__ _log_functions__;

class log_functions__
: public _log_functions__
{
private:
	txf::text_oflow__ *_Flow;
protected:
	virtual void CSDSNSLog(
		csdsns::log__ Log,
		csdsns::id__ Id,
		void *UP,
		sdr::size__ Amount )
	{
		tol::buffer__ Buffer;

		if ( _Flow == NULL )
			ERRFwk();

		*_Flow << '[' << tol::DateAndTime( Buffer ) << "] " << csdsns::GetLogLabel( Log ) << ' ' << Id << '/' << Amount << txf::nl << txf::commit;
	}
public:
	void reset( bso::bool__ P = true )
	{
		_log_functions__::reset( P );
		_Flow = NULL;
	}
	log_functions__( void )
	{
		reset( false );
	}
	~log_functions__( void )
	{
		reset();
	}
	void Init( txf::text_oflow__ &Flow )
	{
		_log_functions__::Init();
		_Flow = &Flow;
	}
};

static void UseSwitchingConnections_(
	csdsuf::user_functions__ &UserFunctions,
	const char *LogFileName,
	log_file_handling__ LogFileHandling,
	const bso::char__ *Module,
	csdbns::port__ Port )
{
ERRProlog
	flf::file_oflow___ FFlow;
	txf::text_oflow__ TFlow;
	log_functions__ LogFunctions;
	fil::mode__ Mode = fil::m_Undefined;
	lcl::meaning ErrorMeaning;
	str::string ErrorTranslation;
ERRBegin

	if ( LogFileName != NULL ) {
		switch ( LogFileHandling ) {
		case lfhAppend:
			fil::mAppend;
			break;
		case lfhDrop:
			fil::mRemove;
			break;
		default:
			if ( LogFileName != NULL )
			ERRPrm();
			break;
		}

		if ( FFlow.Init( LogFileName, Mode, err::hUserDefined ) != fil::sSuccess ) {
			ErrorMeaning.Init();
			ErrorMeaning.SetValue( "UnableToOpenLogFileError" );
			ErrorMeaning.AddTag( LogFileName );

			ErrorTranslation.Init();
			CErr << scllocale::GetTranslation( ErrorMeaning, scltool::GetLanguage(), ErrorTranslation ) << txf::nl << txf::commit;
			LogFileName = NULL;	// To notify no to use log functions.
		} else
			TFlow.Init( FFlow );
	}

	UseSwitchingConnections_( UserFunctions, LogFileName == NULL ? *(csdsns::log_functions__ *)NULL : LogFunctions, Module, Port );
ERRErr
ERREnd
ERREpilog
}

static csdlec::library_embedded_client_core__ *Go_(
	const bso::char__ *ModuleFileName,
	csdbns::port__ Port,
	module_connection_type__ ConnectionType,
	const char *LogFileName,
	log_file_handling__ LogFileHandling )
{
	csdlec::library_embedded_client_core__ *Core = NULL;
ERRProlog
	lcl::locale SharedLocale;
	rgstry::registry SharedRegistry;
	csdlec::library_data__ LibraryData;
	lcl::meaning Meaning;
	str::string Translation;
	err::buffer__ ErrBuffer;
ERRBegin
	SharedLocale.Init();
	SharedRegistry.Init();

	LibraryData.Init( csdleo::mRemote, cio::COutDriver, cio::CErrDriver, false, (void *)ModuleFileName );

	if ( ( Core = new csdlec::library_embedded_client_core__ ) == NULL )
		ERRAlc();

	if ( !Core->Init( ModuleFileName, LibraryData, err::hUserDefined ) ) {
		Meaning.Init();
		Meaning.SetValue( "UnableToLoadModule" );
		Meaning.AddTag( ModuleFileName );
		sclerror::SetMeaning( Meaning );
		ERRExit( EXIT_FAILURE );
	}

	switch ( ConnectionType ) {
	case mctStraight:
		UseStraightConnections_( Core->GetSteering(), ModuleFileName, Port );
		break;
	case mctSwitched:
		UseSwitchingConnections_( Core->GetSteering(), LogFileName, LogFileHandling, ModuleFileName, Port );
		break;
	default:
		ERRFwk();
		break;
	}
ERRErr
	Meaning.Init();
	Meaning.SetValue( "ModuleError" );
	Meaning.SetValue( ModuleFileName );

	if ( ERRType >= err::t_amount ) {
		if ( sclerror::IsErrorPending() )
			Meaning.AddTag( sclerror::GetMeaning() );
		else {
			Translation.Init();
			Meaning.AddTag( scllocale::GetLocale().GetTranslation( "UnkonwnError", scltool::GetLanguage(), Translation ) );
		}
	} else {
		Meaning.AddTag( err::Message( ErrBuffer ) );
	}

	Translation.Init();
	scllocale::GetTranslation( Meaning, scltool::GetLanguage(), Translation );

	cio::CErr << Translation << txf::nl << txf::commit;
ERREnd
ERREpilog
	return Core;
}

static inline csdbns::port__ GetService_( void )
{
	return registry::GetRawModuleService();
}

static csdlec::library_embedded_client_core__ *Go_(
	const char *LogFileName,
	log_file_handling__ LogFileHandling )
{
	csdlec::library_embedded_client_core__ *Core = NULL;
ERRProlog
	str::string ModuleFileName;
	STR_BUFFER___ Buffer;
ERRBegin
	ModuleFileName.Init();

	Core = Go_( registry::GetModuleFileName( ModuleFileName ).Convert( Buffer ), GetService_(), GetModuleConnectionType_(), LogFileName, LogFileHandling );
ERRErr
ERREnd
ERREpilog
	return Core;
}

static csdlec::library_embedded_client_core__ *Core_ = NULL;

static csdlec::library_embedded_client_core__ *Go_( const char *ProjectFilename )
{
	csdlec::library_embedded_client_core__ *Core = NULL;
ERRProlog
	STR_BUFFER___ Buffer;
	const char *LogFileName = NULL;
ERRBegin
	if ( ( ProjectFilename != NULL ) && ( *ProjectFilename ) )
		scltool::LoadProject( ProjectFilename, NAME_LC );

	Core = Go_( registry::GetModuleLogFileName( Buffer ), GetLogFileHandling_() );
ERRErr
ERREnd
ERREpilog
	return Core;
}

static void ExitFunction_( void )
{
	if ( Core_ != NULL )
		delete Core_;

	Core_ = NULL;
}

static void Go_( 
	command__ Command,
	const parameters___ &Parameters )
{
	atexit( ExitFunction_ );

	Core_ = Go_( Parameters.Project );

	while ( 1 )
		tht::Suspend( 1000 );
}

const char *scltool::TargetName = NAME_LC;

void scltool::Main(
	int argc,
	const char *argv[] )
{
ERRProlog
	parameters___ Parameters;
	command__ Command = c_Undefined;
ERRBegin
	Command = AnalyzeArgs_( argc, argv, Parameters );

	Go_( Command, Parameters );
ERRErr
ERREnd
ERREpilog
}
