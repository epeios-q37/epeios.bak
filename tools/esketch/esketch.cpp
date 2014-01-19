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

#include "registry.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

#define BEGIN
// Ne pas enlever ce qu'il y a entre le 'BEGIN' ci-dessus et le 'END' ci-dessous !
/* PLACEHOLDER */

#define NAME_MC			"eSketch"
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

#pragma warning( disable: 101 )
static void PrintUsage_( void )
{
ERRProlog
	TOL_CBUFFER___ TBuffer;
	lcl::meaning Meaning;
	str::string Translation, ShortLong;
ERRBegin
	scltool::PrintDefaultCommandDescriptions( NAME_LC );

	ShortLong.Init();
	cio::COut << NAME_LC << " " << scltool::GetArgumentShortLong( "Test", ShortLong ) << txf::nl;
	Translation.Init();
	cio::COut << txf::pad << scltool::GetArgumentDescriptionTranslation( "Test", Translation ) << txf::nl;

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
#pragma warning( default: 101 )

static void PrintHeader_( void )
{
	COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
	COut << "Copyright " COPYRIGHT << txf::nl;
	COut << txf::pad << "Build : "__DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
}

/* End of the part which handles command line arguments. */

static void Go_( const str::string_ &Command )
{
ERRProlog
ERRBegin
	if ( Command == "Test" )
		cio::COut << "Test" << txf::nl;
	else
		scltool::ReportAndAbort( "BadCommand" );
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
	str::string Command;
ERRBegin
	Command.Init();
	scltool::GetCommand( Command );

	if ( Command == "Usage" )
		PrintUsage_();
	else if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense();
	else
		Go_( Command );
ERRErr
ERREnd
ERREpilog
}
