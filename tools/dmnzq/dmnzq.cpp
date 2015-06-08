/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of dmnzq.

    dmnzq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    dmnzq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with dmnzq.  If not, see <http://www.gnu.org/licenses/>
*/

#include "registry.h"

#include "scltool.h"
#include "sclerror.h"

#include "csdlec.h"
#include "csdbns.h"
#include "csdsns.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

# define NAME_MC			"dmnzq"
# define NAME_LC			"dmnzq"
# define NAME_UC			"DMNZQ"
# define WEBSITE_URL		"http://q37.info/"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	

static void PrintHeader_( void )
{
	COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
	COut << "Copyright (C) " COPYRIGHT << txf::nl;
	COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

namespace {
	csdlec::library_embedded_client_core__ *Core_ = NULL;

	void ExitFunction_( void )
	{
	qRH
		str::string Message;
	qRB
		if ( Core_ != NULL ) {
			Message.Init();
			COut << sclmisc::GetBaseTranslation( "TerminatingModule", Message ) << txf::nl << txf::commit;
			delete Core_;
			COut << sclmisc::GetBaseTranslation( "ModuleTerminated", Message ) << txf::nl << txf::commit;
		}

		Core_ = NULL;
	qRR
	qRT
	qRE
	}

	E_ENUM( log_file_handling ) {
		lfhAppend,	// New log are appended to the current ones.
		lfhDrop,		// Old logs are destroyed.
		lfh_amount,
		lfh_Undefined,
	};

	log_file_handling__ GetLogFileHandling_( void )
	{
		log_file_handling__ Handling = lfh_Undefined;
	qRH
		str::string Value;
	qRB
		Value.Init();

		registry::GetRawModuleLogMode( Value );

		if ( ( Value.Amount() == 0 ) ||( Value == "Append" ) )
			Handling = lfhAppend;
		else if ( Value == "Drop" )
			Handling = lfhDrop;
		else
			sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( registry::ModuleLogMode );
	qRR
	qRT
	qRE
		return Handling;
	}

	inline csdbns::port__ GetService_( void )
	{
		return registry::GetRawModuleService();
	}

	E_ENUM( module_connection_type ) {
		mctStraight,	
		mctSwitched,
		mct_amount,
		mct_Undefined
	};

	module_connection_type__ GetModuleConnectionType_( void )
	{
		module_connection_type__ Type = mct_Undefined;
	qRH
		str::string Value;
	qRB
		Value.Init();

		registry::GetRawModuleServiceType( Value );

		if ( Value == "Straight" )
			Type = mctStraight;
		else if ( Value == "Switched" )
			Type = mctSwitched;
		else
			sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( registry::ModuleServiceType );
	qRR
	qRT
	qRE
		return Type;
	}

	void UseStraightConnections_(
		csdscb::callback__ &Callback,
		const bso::char__ *Module,
		csdbns::port__ Port )
	{
	qRH
		csdbns::server___ Server;
	qRB
		Server.Init( Port, Callback );

		Server.Process();
	qRR
	qRT
	qRE
	}

	void UseSwitchingConnections_(
		csdscb::callback__ &Callback,
		csdsns::log_functions__ &LogFunctions,
		csdbns::port__ Port )
	{
	qRH
		csdsns::server___ Server;
	qRB
		Server.Init( Port, Callback, LogFunctions );

		Server.Process();
	qRR
	qRT
	qRE
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
				qRGnr();

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

	void UseSwitchingConnections_(
		csdscb::callback__ &Callback,
		const char *LogFileName,
		log_file_handling__ LogFileHandling,
		csdbns::port__ Port )
	{
	qRH
		flf::file_oflow___ FFlow;
		txf::text_oflow__ TFlow;
		log_functions__ LogFunctions;
		fil::mode__ Mode = fil::m_Undefined;
		lcl::meaning ErrorMeaning;
		str::string ErrorTranslation;
	qRB

		if ( LogFileName != NULL ) {
			switch ( LogFileHandling ) {
			case lfhAppend:
				Mode = fil::mAppend;
				break;
			case lfhDrop:
				Mode = fil::mRemove;
				break;
			default:
				if ( LogFileName != NULL )
					qRGnr();
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

		UseSwitchingConnections_( Callback, LogFileName == NULL ? *(csdsns::log_functions__ *)NULL : LogFunctions, Port );
	qRR
	qRT
	qRE
	}

	void Process_(
		const bso::char__ *ModuleFilename,
		csdbns::port__ Port,
		module_connection_type__ ConnectionType,
		const char *LogFileName,
		log_file_handling__ LogFileHandling )
	{
	qRH
		lcl::locale SharedLocale;
		rgstry::registry SharedRegistry;
		csdlec::library_data__ LibraryData;
		lcl::meaning Meaning, MeaningBuffer;
		str::string Translation;
		err::buffer__ ErrBuffer;
	qRB
		SharedLocale.Init();
		SharedRegistry.Init();

		LibraryData.Init( csdleo::cRegular, ModuleFilename, err::qRRor, sclerror::SCLERRORError, csdleo::mRemote );

		if ( ( Core_ = new csdlec::library_embedded_client_core__ ) == NULL )
			qRAlc();

		if ( !Core_->Init( ModuleFilename, LibraryData, err::hUserDefined ) ) {
			Meaning.Init();
			Meaning.SetValue( "UnableToLoadModule" );
			Meaning.AddTag( ModuleFilename );
			sclerror::SetMeaning( Meaning );
			qRAbort();
		}

		switch ( ConnectionType ) {
		case mctStraight:
			UseStraightConnections_( Core_->GetCallback(), ModuleFilename, Port );
			break;
		case mctSwitched:
			UseSwitchingConnections_( Core_->GetCallback(), LogFileName, LogFileHandling, Port );
			break;
		default:
			qRGnr();
			break;
		}
	qRR
		Meaning.Init();
		Meaning.SetValue( "ModuleError" );
		Meaning.SetValue( ModuleFilename );

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
	qRT
	qRE
	}

	void Process_(
		const char *LogFileName,
		log_file_handling__ LogFileHandling )
	{
	qRH
		str::string ModuleFileName;
		TOL_CBUFFER___ Buffer;
	qRB
		ModuleFileName.Init();

		Process_( registry::GetModuleFileName( ModuleFileName ).Convert( Buffer ), GetService_(), GetModuleConnectionType_(), LogFileName, LogFileHandling );
	qRR
	qRT
	qRE
	}

	void Process_( void )
	{
	qRH
		TOL_CBUFFER___ Buffer;
	qRB
		atexit( ExitFunction_ );

		cio::COut.Commit();

		Process_( registry::GetModuleLogFilename( Buffer ), GetLogFileHandling_() );
	qRR
	qRT
	qRE
	}
}

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
		epsmsc::PrintLicense( NAME_MC );
	C( Process );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;

