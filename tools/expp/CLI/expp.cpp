/*
	'expp' by Claude SIMON (simon.claude@zeusw.org)
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

#include "registry.h"
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

const char *sclmisc::SCLMISCTargetName = NAME;

#define DEFAULT_NAMESPACE	XPP__PREPROCESSOR_DEFAULT_NAMESPACE

/* Beginning of the part which handles command line arguments. */

enum exit_value__ {
	evSuccess = EXIT_SUCCESS,
	evGenericFailure = EXIT_FAILURE,
	// Erreur dans les paramtres d'entre.
	evParameters,
	// Erreur lors de l'ouverture des fichiers d'entre ou de sortie.
	evInputOutput,
	// Erreur lors du traitement.
	evProcessing,
	ev_amount
};

// #include "tht.h"

static void PrintHeader_( void )
{
	COut << NAME " V" VERSION << " (" APP_URL ")" << txf::nl;
	COut << COPYRIGHT << txf::nl;
	COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
}


/* End of the part which handles command line arguments. */

static void Process_(
	flw::iflow__ &IFlow,
	const char *Namespace,
	const char *Directory,
	bso::bool__ Preserve,
	xml::outfit__ Outfit,
	txf::text_oflow__ &OFlow )
{
ERRProlog
	xpp::status__ Status = xpp::s_Undefined;
	xpp::context___ Context;
	xtf::extended_text_iflow__ XFlow;
	lcl::meaning Meaning;
	bomhdl::byte_order_marker__ BOM;
	bomhdl::bom__ BOMContent;
ERRBegin
	Context.Init();

	BOM = XFlow.Init( IFlow, utf::f_Default );

	if ( BOM != bomhdl::bom_UnknownOrNone ) {
		BOMContent.Init();

		BOMContent = bomhdl::GetBOM( BOM );
		OFlow.Put( (const fdr::datum__ *)BOMContent.Data, BOMContent.Size );
	}

	if ( ( Status = xpp::Process( XFlow, xpp::criterions___( str::string( Directory == NULL ? (const char *)"" : Directory ), str::string(),
															 str::string( Namespace == NULL ? DEFAULT_NAMESPACE : Namespace ), Preserve ),
								  Outfit, OFlow,  Context ) ) != xpp::sOK )	{
		Meaning.Init();

		locale::GetProcessingErrorMeaning( Context, Meaning );

		sclerror::SetMeaning( Meaning );

		ERRAbort();
	}

ERRErr
ERREnd
ERREpilog
}

static void Process_(
	const char *Source,
	const char *Destination,
	const char *Namespace,
	bso::bool__ Preserve,
	bso::bool__ Indent )
{
ERRProlog
	flf::file_oflow___ OFlow;
	txf::text_oflow__ TOFlow;
	flf::file_iflow___ IFlow;
	fnm::name___ Directory;
	bso::bool__ BackedUp = false;
	TOL_CBUFFER___ Buffer;
ERRBegin
	Directory.Init();

	if ( Source != NULL ) {
		if ( IFlow.Init( Source, err::hUserDefined ) != tol::rSuccess )
			sclmisc::ReportFileOpeningErrorAndAbort( Source );

		fnm::GetLocation(Source, Directory );
	}

	if ( Destination != NULL ) {
		sclmisc::CreateBackupFile( Destination );

		BackedUp = true;

		if ( OFlow.Init( Destination, err::hUserDefined ) != tol::rSuccess )
			sclmisc::ReportFileOpeningErrorAndAbort( Destination );

		TOFlow.Init( OFlow );
	}

	Process_( Source == NULL ? CIn.Flow() : IFlow, Namespace, Directory.UTF8( Buffer ), Preserve, Indent ? xml::oIndent : xml::oCompact, Destination == NULL ? COut : TOFlow );

ERRErr
	if ( BackedUp ) {
		TOFlow.reset();
		OFlow.reset();
		sclmisc::RecoverBackupFile( Destination );
	}
ERREnd
ERREpilog
}

static void Process_( void )
{
ERRProlog
	str::string Source, Destination, NameSpace;
	TOL_CBUFFER___ SourceBuffer, DestinationBuffer, NameSpaceBuffer;
ERRBegin
	Source.Init();
	sclmisc::OGetValue( registry::Source, Source );

	Destination.Init();
	sclmisc::OGetValue( registry::Destination, Destination );

	NameSpace.Init();
	sclmisc::OGetValue( registry::NameSpace, NameSpace );

	Process_(
		Source.Amount() != 0 ? Source.Convert( SourceBuffer ) : NULL,
		Destination.Amount() != 0 ? Destination.Convert( DestinationBuffer ) : NULL,
		NameSpace.Amount() != 0 ? NameSpace.Convert( NameSpaceBuffer ) : NULL,
		sclmisc::BGetBoolean( registry::Preserve ),
		sclmisc::BGetBoolean( registry::Indentation, true ) );
ERRErr
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
		if ( IFlow.Init( Source, err::hUserDefined ) != tol::rSuccess )
			sclmisc::ReportFileOpeningErrorAndAbort( Source );

	if ( Destination != NULL ) {
		sclmisc::CreateBackupFile( Destination );

		BackedUp = true;

		if ( OFlow.Init( Destination, err::hUserDefined ) != tol::rSuccess )
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

		ERRAbort();
	}
ERRErr
	if ( BackedUp )
		sclmisc::RecoverBackupFile( Destination );
ERREnd
ERREpilog
}

static void Encrypt_( void )
{
ERRProlog
	str::string Source, Destination, NameSpace, Indentation;
	TOL_CBUFFER___ SourceBuffer, DestinationBuffer, NameSpaceBuffer;
ERRBegin
	Source.Init();
	sclmisc::OGetValue( registry::Source, Source );

	Destination.Init();
	sclmisc::OGetValue( registry::Destination, Destination );

	NameSpace.Init();
	sclmisc::OGetValue( registry::NameSpace, NameSpace );

	Indentation.Init();
	sclmisc::OGetValue( registry::Indentation, Indentation );

	Encrypt_( Source.Amount() != 0 ? Source.Convert( SourceBuffer ) : NULL, Destination.Amount() != 0 ? Destination.Convert( DestinationBuffer ) : NULL, NameSpace.Amount() != 0 ? NameSpace.Convert( NameSpaceBuffer ) :  NULL, Indentation == "Yes" );
ERRErr
ERREnd
ERREpilog
}

int scltool::SCLTOOLMain(
	const str::string_ &Command,
	const scltool::oddities__ &Oddities )
{
	int ExitValue = EXIT_FAILURE;
ERRProlog
	str::string Translation;
ERRBegin
	if ( Command == "Process" )
		Process_();
	else if ( Command == "Encrypt" )
		Encrypt_();
	else if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" ) {
		Translation.Init();
		sclmisc::GetBaseTranslation( "License", Translation );
		cio::COut << Translation << txf::nl;
	}
	else
		ERRFwk();

	ExitValue = EXIT_SUCCESS;
ERRErr
ERREnd
ERREpilog

	return ExitValue;
}
