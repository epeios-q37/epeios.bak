/*
	'expp' by Claude SIMON (claude.simon@zeusw.org)
	XML Preprocessor.
	Copyright (C) 2007-2011 Claude SIMON

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'expp'.

    'expp' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'expp' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'expp'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "details.h"

#include "locale.h"
#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "clnarg.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

#define NAME "expp"

const char *scltool::TargetName = NAME;

#define DEFAULT_NAMESPACE	XPP__PREPROCESSOR_DEFAULT_NAMESPACE

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
	cEncrypt,
	c_amount,
	c_Undefined
};

enum option {
	// Définition du namespace.
	oNamespace,
	// Suppression de l'indentation
	oNoIndent

};

#define STRING_PARAM___( name )	CLNARG_STRING_PARAM___( name )

struct parameters___ {
	STRING_PARAM___( Namespace );
	STRING_PARAM___(  Source );
	STRING_PARAM___( Destination );
	bso::bool__ NoIndent;
	command__ Command;
	parameters___( void )
	{
		NoIndent = false;
		Command = c_Undefined;
	}
};

static void PrintUsage_( const clnarg::description_ &Description )
{
ERRProlog
	STR_BUFFER___ TBuffer;
	CLNARG_BUFFER__ Buffer;
	lcl::meaning Meaning;
	str::string Translation;
ERRBegin
	scltool::PrintDefaultCommandDescriptions( NAME, Description );

	// Commands.
	COut << NAME << " [" << Description.GetCommandLabels( cProcess, Buffer );
	COut << "] [" << Description.GetOptionLabels( oNamespace, Buffer ) << " <ns>]";
	COut << " [" << Description.GetOptionLabels( oNoIndent, Buffer );
	COut << "] [<src> [<dst>]]";
	COut << txf::nl;
	Translation.Init();
	COut << txf::pad << scllocale::GetTranslation( locale::Label( locale::tProcessCommandDescription ), scltool::GetLanguage(), Translation ) << '.' << txf::nl;

	COut << NAME << ' ' << Description.GetCommandLabels( cEncrypt, Buffer );
	COut << " [" << Description.GetOptionLabels( oNamespace, Buffer ) << " <ns>]";
	COut << " [" << Description.GetOptionLabels( oNoIndent, Buffer );
	COut << "] [<src> [<dst>]]";
	COut << txf::nl;
	Translation.Init();
	COut << txf::pad << scllocale::GetTranslation( locale::Label( locale::tEncryptCommandDescription ), scltool::GetLanguage(), Translation ) << '.' << txf::nl;

	COut << txf::nl;

// Options.
	Meaning.Init();
	clnarg::GetOptionsWordingMeaning( Meaning );
	Translation.Init();
	COut << scllocale::GetTranslation( Meaning, scltool::GetLanguage(), Translation ) << " :" << txf::nl;

	COut << txf::pad << Description.GetOptionLabels( oNamespace, Buffer ) << " <ns> :" << txf::nl;
	COut << txf::tab;
	Meaning.Init();
	locale::GetNamespaceOptionDescriptionMeaning( DEFAULT_NAMESPACE, Meaning );
	Translation.Init();
	COut << scllocale::GetTranslation( Meaning, scltool::GetLanguage(), Translation ) << '.' << txf::nl;

	COut << txf::pad << Description.GetOptionLabels( oNoIndent, Buffer ) << " :" << txf::nl;
	COut << txf::tab;
	Translation.Init();
	COut << locale::GetNoIndentOptionDescriptionTranslation( Translation ) << '.' << txf::nl;

	COut << txf::nl;

// Arguments.
	Meaning.Init();
	clnarg::GetArgumentsWordingMeaning( Meaning );
	Translation.Init();
	COut << scllocale::GetTranslation( Meaning, scltool::GetLanguage(), Translation ) << " :" << txf::nl;

	COut << txf::pad << "<src> :" << txf::nl;
	COut << txf::tab;
	Translation.Init();
	COut << locale::GetSourceFileArgumentDescriptionTranslation( Translation ) << '.' << txf::nl;

	COut << txf::pad << "<dst> :" << txf::nl;
	COut << txf::tab;
	Translation.Init();
	COut << locale::GetDestFileArgumentDescriptionTranslation( Translation ) << '.' << txf::nl;

ERRErr
ERREnd
ERREpilog
}


#include "tht.h"

static void PrintHeader_( void )
{
	COut << NAME " V" VERSION << " (" APP_URL ")" << txf::nl;
	COut << COPYRIGHT << txf::nl;
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
	clnarg::buffer__ Buffer;
ERRBegin
	Options.Init();

	if ( ( Unknown = Analyzer.GetOptions( Options ) ) != NULL )
		clnarg::ReportUnknownOptionError( Unknown, NAME, scllocale::GetLocale(), scltool::GetLanguage() );

	P = Options.First();

	while( P != E_NIL ) {
		Argument.Init();

		switch( Option = Options( P ) ) {
		case oNamespace:
			Analyzer.GetArgument( Option, Argument );

			if ( Argument.Amount() == 0 )
				clnarg::ReportMissingOptionArgumentError( Analyzer.Description().GetOptionLabels( oNamespace, Buffer ), NAME, scllocale::GetLocale(), scltool::GetLanguage() );

			Argument.Convert( Parameters.Namespace );

			break;
		case oNoIndent:
			Parameters.NoIndent = true;
			break;
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
	case 2:
		Free( P ).Convert( Parameters.Destination );
		P = Free.Previous( P );
	case 1:
		Free( P ).Convert( Parameters.Source );
		break;
	case 0:
		break;
	default:
		clnarg::ReportWrongNumberOfArgumentsError( NAME, scllocale::GetLocale(), scltool::GetLanguage() );
		break;
	}

ERRErr
ERREnd
ERREpilog
}

static void AnalyzeArgs_(
	int argc,
	const char *argv[],
	parameters___ &Parameters )
{
ERRProlog
	clnarg::description Description;
	clnarg::analyzer___ Analyzer;
ERRBegin
	Description.Init();

	scltool::AddDefaultCommands( Description );

//	Description.AddCommand( '', "", c );
	Description.AddCommand( 'p', "process", cProcess );
	Description.AddCommand( CLNARG_NO_SHORT, "encrypt", cEncrypt );
//	Description.AddOption( '', "", o );
	Description.AddOption( 'n', "namespace", oNamespace );
	Description.AddOption( CLNARG_NO_SHORT, "no-indent", oNoIndent );

	Analyzer.Init( argc, argv, Description );

	switch ( Analyzer.GetCommand() ) {
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
	case cProcess:
	case cEncrypt:
		Parameters.Command = (command__)Analyzer.GetCommand();
		break;
	case CLNARG_NONE:
		Parameters.Command = cProcess;
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
}

/* End of the part which handles command line arguments. */

static void Process_(
	flw::iflow__ &IFlow,
	const char *Namespace,
	const char *Directory,
	xml::outfit__ Outfit,
	txf::text_oflow__ &OFlow )
{
ERRProlog
	xpp::status__ Status = xpp::s_Undefined;
	xpp::context___ Context;
	xtf::extended_text_iflow__ XFlow;
	lcl::meaning Meaning;
	bom::byte_order_marker__ BOM;
	bom::bom__ BOMContent;
ERRBegin
	Context.Init();

	BOM = XFlow.Init( IFlow, utf::f_Default );

	if ( BOM != bom::bom_UnknownOrNone ) {
		BOMContent.Init();

		BOMContent = bom::GetBOM( BOM );
		OFlow.Put( (const fdr::datum__ *)BOMContent.Data, BOMContent.Size );
	}

	if ( ( Status = xpp::Process( XFlow, xpp::criterions___( str::string( Directory == NULL ? (const char *)"" : Directory ), str::string(),
															 str::string( Namespace == NULL ? DEFAULT_NAMESPACE : Namespace ) ),
								  Outfit, OFlow,  Context ) ) != xpp::sOK )	{
		Meaning.Init();

		locale::GetProcessingErrorMeaning( Context, Meaning );

		sclerror::SetMeaning( Meaning );

		ERRExit( EXIT_FAILURE );
	}

ERRErr
ERREnd
ERREpilog
}

static void Process_(
	const char *Source,
	const char *Destination,
	const char *Namespace,
	bso::bool__ Indent )
{
ERRProlog
	flf::file_oflow___ OFlow;
	txf::text_oflow__ TOFlow;
	flf::file_iflow___ IFlow;
	const char *Directory = NULL;
	bso::bool__ BackedUp = false;
	FNM_BUFFER___ Buffer;
ERRBegin
	if ( Source != NULL ) {
		if ( IFlow.Init( Source, err::hUserDefined ) != fil::sSuccess )
			sclmisc::ReportFileOpeningErrorAndAbort( Source );

		Directory = fnm::GetLocation( Source, Buffer );
	}

	if ( Destination != NULL ) {
		sclmisc::CreateBackupFile( Destination );

		BackedUp = true;

		if ( OFlow.Init( Destination, err::hUserDefined ) != fil::sSuccess )
			sclmisc::ReportFileOpeningErrorAndAbort( Destination );

		TOFlow.Init( OFlow );
	}

	Process_( Source == NULL ? CIn.Flow() : IFlow, Namespace, Directory, Indent ? xml::oIndent : xml::oCompact, Destination == NULL ? COut : TOFlow );

ERRErr
	if ( BackedUp ) {
		TOFlow.reset();
		OFlow.reset();
		sclmisc::RecoverBackupFile( Destination );
	}
ERREnd
ERREpilog
}

static void Encrypt_(
	const char *Source,
	const char *Destination,
	const char *Namespace,
	bso::bool__ Indent )
{
ERRProlog
	flf::file_oflow___ OFlow;
	txf::text_oflow__ TOFlow;
	flf::file_iflow___ IFlow;
	bso::bool__ BackedUp = false;
	xpp::context___ Context;
	lcl::meaning Meaning;
ERRBegin
	if ( Source != NULL )
		if ( IFlow.Init( Source, err::hUserDefined ) != fil::sSuccess )
			sclmisc::ReportFileOpeningErrorAndAbort( Source );

	if ( Destination != NULL ) {
		sclmisc::CreateBackupFile( Destination );

		BackedUp = true;

		if ( OFlow.Init( Destination, err::hUserDefined ) != fil::sSuccess )
			sclmisc::ReportFileOpeningErrorAndAbort( Destination );

		TOFlow.Init( OFlow );
	}

	Context.Init();

	if ( xpp::Encrypt( str::string( Namespace == NULL ? DEFAULT_NAMESPACE : Namespace ),
								  IFlow,
								  Indent ? xml::oIndent : xml::oCompact,
								  utf::f_Default,
								  ( Destination == NULL ? COut : TOFlow ),  Context ) != xpp::sOK )	{
		Meaning.Init();

		locale::GetEncryptionErrorMeaning( Context, Meaning );

		sclerror::SetMeaning( Meaning );

		ERRExit( EXIT_FAILURE );
	}
ERRErr
	if ( BackedUp )
		sclmisc::RecoverBackupFile( Destination );
ERREnd
ERREpilog
}

static void Go_( const parameters___ &Parameters )
{
ERRProlog
ERRBegin
	switch ( Parameters.Command ) {
	case cProcess:
		Process_( Parameters.Source, Parameters.Destination, Parameters.Namespace, !Parameters.NoIndent );
		break;
	case cEncrypt:
		Encrypt_( Parameters.Source, Parameters.Destination, Parameters.Namespace, !Parameters.NoIndent );
		break;
	default:
		ERRFwk();
		break;
}

ERRErr
ERREnd
ERREpilog
}

void scltool::Main(
	int argc,
	const char *argv[] )
{
ERRProlog
	parameters___ Parameters;
ERRBegin
	AnalyzeArgs_( argc, argv, Parameters );

	Go_( Parameters );
ERRErr
ERREnd
ERREpilog
}
