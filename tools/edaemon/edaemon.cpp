/*
	'edaemon' by Claude SIMON (simon.claude@zeusw.org)
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

static void PrintHeader_( void )
{
	COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
	COut << "Copyright " COPYRIGHT << txf::nl;
	COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
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
	else
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( registry::ModuleServiceType );
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
	else
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( registry::ModuleLogMode );
ERRErr
ERREnd
ERREpilog
	return Handling;
}

static void UseStraightConnections_(
	csdscb::callback__ &Callback,
	const bso::char__ *Module,
	csdbns::port__ Port )
{
ERRProlog
	csdbns::server___ Server;
ERRBegin
	Server.Init( Port, Callback );

	Server.Process();
ERRErr
ERREnd
ERREpilog
}

static void UseSwitchingConnections_(
	csdscb::callback__ &Callback,
	csdsns::log_functions__ &LogFunctions,
	const bso::char__ *Module,
	csdbns::port__ Port )
{
ERRProlog
	csdsns::server___ Server;
ERRBegin
	Server.Init( Port, Callback, LogFunctions );

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
	csdscb::callback__ &Callback,
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

		if ( FFlow.Init( LogFileName, Mode, err::hUserDefined ) != tol::rSuccess ) {
			ErrorMeaning.Init();
			ErrorMeaning.SetValue( "UnableToOpenLogFileError" );
			ErrorMeaning.AddTag( LogFileName );

			ErrorTranslation.Init();
			CErr << sclmisc::GetBaseTranslation( ErrorMeaning, ErrorTranslation ) << txf::nl << txf::commit;
			LogFileName = NULL;	// To notify no to use log functions.
		} else
			TFlow.Init( FFlow );
	}

	UseSwitchingConnections_( Callback, LogFileName == NULL ? *(csdsns::log_functions__ *)NULL : LogFunctions, Module, Port );
ERRErr
ERREnd
ERREpilog
}

static csdlec::library_embedded_client_core__ *Core_ = NULL;

static void Go_(
	const bso::char__ *ModuleFileName,
	csdbns::port__ Port,
	module_connection_type__ ConnectionType,
	const char *LogFileName,
	log_file_handling__ LogFileHandling )
{
ERRProlog
	lcl::locale SharedLocale;
	rgstry::registry SharedRegistry;
	csdlec::library_data__ LibraryData;
	lcl::meaning Meaning, MeaningBuffer;
	str::string Translation;
	err::buffer__ ErrBuffer;
ERRBegin
	SharedLocale.Init();
	SharedRegistry.Init();

	LibraryData.Init( csdleo::mRemote, cio::SOutDriver, cio::SErrDriver, csdleo::cRegular, (void *)ModuleFileName );

	if ( ( Core_ = new csdlec::library_embedded_client_core__ ) == NULL )
		ERRAlc();

	if ( !Core_->Init( ModuleFileName, LibraryData, err::hUserDefined ) ) {
		Meaning.Init();
		Meaning.SetValue( "UnableToLoadModule" );
		Meaning.AddTag( ModuleFileName );
		sclerror::SetMeaning( Meaning );
		ERRAbort();
	}

	switch ( ConnectionType ) {
	case mctStraight:
		UseStraightConnections_( Core_->GetCallback(), ModuleFileName, Port );
		break;
	case mctSwitched:
		UseSwitchingConnections_( Core_->GetCallback(), LogFileName, LogFileHandling, ModuleFileName, Port );
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
		if ( sclerror::IsErrorPending() ) {
			MeaningBuffer.Init();
			Meaning.AddTag( sclerror::GetMeaning( MeaningBuffer ) );
		} else {
			Translation.Init();
			Meaning.AddTag( sclmisc::GetBaseTranslation( "UnkonwnError", Translation ) );
		}
	} else {
		Meaning.AddTag( err::Message( ErrBuffer ) );
	}

	Translation.Init();
	sclmisc::GetBaseTranslation( Meaning, Translation );

	cio::CErr << Translation << txf::nl << txf::commit;
ERREnd
ERREpilog
}

static inline csdbns::port__ GetService_( void )
{
	return registry::GetRawModuleService();
}

static void Go_(
	const char *LogFileName,
	log_file_handling__ LogFileHandling )
{
ERRProlog
	str::string ModuleFileName;
	TOL_CBUFFER___ Buffer;
ERRBegin
	ModuleFileName.Init();

	Go_( registry::GetModuleFileName( ModuleFileName ).Convert( Buffer ), GetService_(), GetModuleConnectionType_(), LogFileName, LogFileHandling );
ERRErr
ERREnd
ERREpilog
}

static void ExitFunction_( void )
{
	if ( Core_ != NULL ) {
		COut << "Terminating module..." << txf::nl << txf::commit;
		delete Core_;
		COut << "Teminated." << txf::nl;
	}

	Core_ = NULL;
}

static void Go_( void )
{
ERRProlog
	TOL_CBUFFER___ Buffer;
ERRBegin
	atexit( ExitFunction_ );

//	scltool::GetRegistry().Dump( sclrgstry::GetProjectLevel(), E_NIL, true, xml::oIndent, xml::e_Default, cio::COut );

	cio::COut.Commit();

	Go_( registry::GetModuleLogFileName( Buffer ), GetLogFileHandling_() );
ERRErr
ERREnd
ERREpilog
}

int scltool::SCLTOOLMain(
	const str::string_ &Command,
	const scltool::oddities__ &Oddities )
{
	int Exit = EXIT_FAILURE;
ERRProlog
ERRBegin
	if ( Command == "Process" )
		Go_();
	else if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		sclmisc::ReportAndAbort( "BadCommand" );
	else
		ERRFwk();

	Exit = EXIT_SUCCESS;
	ERRErr
		ERREnd
		ERREpilog
		return Exit;
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
