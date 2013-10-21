/*
	'esketch' by Claude SIMON (claude.simon@zeusw.org)
	XML Preprocessor.
	Copyright (C) 2007-2011 Claude SIMON

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'esketch'.

    'esketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'esketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'esketch'.  If not, see <http://www.gnu.org/licenses/>.
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

using cio::CErr;
using cio::COut;
using cio::CIn;

#define BEGIN
// Ne pas enlever ce qu'il y a entre le 'BEGIN' ci-dessus et le 'END' ci-dessous !
/* PLACEHOLDER */

#define NAME			"eSketch"
#define NAME_UC			"ESKETCH"
#define NAME_LC			"esketch"
#define WEBSITE_URL		"http://zeusw.org/intl/"
#define AUTHOR_NAME		"Claude SIMON"
#define AUTHOR_CONTACT	"http://zeusw.org/intl/contact.html"
#define OWNER_NAME		"Claude SIMON"
#define OWNER_CONTACT	"http://zeusw.org/intl/contact.html"
#define COPYRIGHT		COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	

#define END

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
	parameters___( void )
	{
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

#if 0	// Exemples.
	// Commands.
	COut << NAME << " [" << Description.GetCommandLabels( cProcess, Buffer );
	COut << "] [" << Description.GetOptionLabels( oNamespace, Buffer ) << " <ns>]";
	COut << " [" << Description.GetOptionLabels( oNoIndent, Buffer );
	COut << "] [<src> [<dst>]]";
	COut << txf::nl;
	Translation.Init();
	COut << txf::pad << locale::GetProcessCommandDescriptionTranslation( Translation ) << '.' << txf::nl;

	COut << NAME << ' ' << Description.GetCommandLabels( cEncrypt, Buffer );
	COut << " [" << Description.GetOptionLabels( oNamespace, Buffer ) << " <ns>]";
	COut << " [" << Description.GetOptionLabels( oNoIndent, Buffer );
	COut << "] [<src> [<dst>]]";
	COut << txf::nl;
	Translation.Init();
	COut << txf::pad << locale::GetEncryptCommandDescriptionTranslation( Translation ) << '.' << txf::nl;

	COut << txf::nl;

// Options.
	Meaning.Init();
	clnarg::GetOptionsWordingMeaning( Meaning );
	Translation.Init();
	COut << scllocale::GetLocale().GetTranslation( Meaning, scltool::GetLanguage(), Translation ) << " :" << txf::nl;

	COut << txf::pad << Description.GetOptionLabels( oNamespace, Buffer ) << " <ns> :" << txf::nl;
	COut << txf::tab;
	Translation.Init();
	COut << locale::GetNamespaceOptionDescriptionTranslation( Translation ) << '.' << txf::nl;

	COut << txf::pad << Description.GetOptionLabels( oNoIndent, Buffer ) << " :" << txf::nl;
	COut << txf::tab;
	Translation.Init();
	COut << locale::GetNoIndentOptionDescriptionTranslation( Translation ) << '.' << txf::nl;

	COut << txf::nl;

// Arguments.
	Meaning.Init();
	clnarg::GetArgumentsWordingMeaning( Meaning );
	Translation.Init();
	COut << scllocale::GetLocale().GetTranslation( Meaning, scltool::GetLanguage(), Translation ) << " :" << txf::nl;

	COut << txf::pad << "<src> :" << txf::nl;
	COut << txf::tab;
	COut << locale::GetSourceFileArgumentDescriptionTranslation( Translation ) << '.' << txf::nl;

	COut << txf::pad << "<dst> :" << txf::nl;
	COut << txf::tab;
	COut << locale::GetDestFileArgumentDescriptionTranslation( Translation ) << '.' << txf::nl;
#endif

ERRErr
ERREnd
ERREpilog
}

static void PrintHeader_( void )
{
	COut << NAME " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
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
	clnarg::buffer__ Buffer;
ERRBegin
	Options.Init();

	if ( ( Unknown = Analyzer.GetOptions( Options ) ) != NULL )
		clnarg::ReportUnknownOptionError( Unknown, NAME, scllocale::GetLocale(), scltool::GetLanguage() );

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

static void Go_(
	command__ Command,
	const parameters___ &Parameters )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
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
