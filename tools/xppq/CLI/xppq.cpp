/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of XPPq.

	XPPq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XPPq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XPPq. If not, see <http://www.gnu.org/licenses/>.
*/

#include "xppq.h"

#include "registry.h"

#include "i18n.h"

#include "sclt.h"
#include "scle.h"
#include "sclm.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( xppq, NAME_LC, NAME_MC );

// #define VERSION "0.0.0"
// #define COPYRIGHT_YEARS "1969"

static void PrintHeader_( void )
{
	COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
	COut << "Copyright (C) " COPYRIGHT << txf::nl;
	COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
}

#if 0
static void Test_( void )
{
	cio::COut << "Test" << txf::nl;
}
# endif

E_CDEF( char *, DefaultNamespace, XPP__PREPROCESSOR_DEFAULT_NAMESPACE );

namespace {
	void Process_(
		flw::iflow__ &IFlow,
		const char *Namespace,
		const char *Directory,
		bso::bool__ Preserve,
		xml::outfit__ Outfit,
		txf::text_oflow__ &OFlow )
	{
	qRH
		xpp::status__ Status = xpp::s_Undefined;
		xpp::context___ Context;
		xtf::extended_text_iflow__ XFlow;
		lcl::meaning Meaning;
		bomhdl::byte_order_marker__ BOM;
		bomhdl::bom__ BOMContent;
	qRB
		Context.Init();

		BOM = XFlow.Init( IFlow, utf::f_Default );

		if ( BOM != bomhdl::bom_UnknownOrNone ) {
			BOMContent.Init();

			BOMContent = bomhdl::GetBOM( BOM );
			OFlow.Put( (const fdr::byte__ *)BOMContent.Data, BOMContent.Size );
		}

		if ( ( Status = xpp::Process( XFlow, xpp::criterions___( Directory == NULL ? "" : Directory, 0, str::string(),
																 str::string( Namespace == NULL ? DefaultNamespace : Namespace ), Preserve ),
									  Outfit, OFlow,  Context ) ) != xpp::sOK )	{
			Meaning.Init();

			i18n::GetProcessingErrorMeaning( Context, Meaning );

			scle::SetMeaning( Meaning );

			qRAbort();
		}

	qRR
	qRT
	qRE
	}

	void Process_(
		const char *Source,
		const char *Destination,
		const char *Namespace,
		bso::bool__ Preserve,
		bso::bool__ Indent )
	{
	qRH
		flf::file_oflow___ OFlow;
		txf::text_oflow__ TOFlow;
		flf::file_iflow___ IFlow;
		fnm::name___ Directory;
		bso::bool__ BackedUp = false;
		TOL_CBUFFER___ Buffer;
	qRB
		Directory.Init();

		if ( Source != NULL ) {
			if ( IFlow.Init( Source, err::hUserDefined ) != tol::rSuccess )
				sclm::ReportFileOpeningErrorAndAbort( Source );

			fnm::GetLocation(Source, Directory );
		}

		if ( Destination != NULL ) {
			sclm::CreateBackupFile( Destination );

			BackedUp = true;

			if ( OFlow.Init( Destination, err::hUserDefined ) != tol::rSuccess )
				sclm::ReportFileOpeningErrorAndAbort( Destination );

			TOFlow.Init( OFlow );
		}

		Process_( Source == NULL ? CIn.Flow() : IFlow, Namespace, Directory.UTF8( Buffer ), Preserve, Indent ? xml::oIndent : xml::oCompact, Destination == NULL ? COut : TOFlow );

	qRR
		if ( BackedUp ) {
			TOFlow.reset();
			OFlow.reset();
			sclm::RecoverBackupFile( Destination );
		}
	qRT
	qRE
	}

	void Process_( void )
	{
	qRH
		str::string Input, Output, Namespace;
		TOL_CBUFFER___ InputBuffer, OutputBuffer, NamespaceBuffer;
	qRB
		Input.Init();
		sclm::OGetValue( registry::Input, Input );

		Output.Init();
		sclm::OGetValue( registry::Output, Output );

		Namespace.Init();
		sclm::OGetValue( registry::Namespace, Namespace );

		Process_(
			Input.Amount() != 0 ? Input.Convert( InputBuffer ) : NULL,
			Output.Amount() != 0 ? Output.Convert( OutputBuffer ) : NULL,
			Namespace.Amount() != 0 ? Namespace.Convert( NamespaceBuffer ) : NULL,
			sclm::OGetBoolean(registry::Preserve, false),
			sclm::OGetBoolean(registry::Indentation, true) );
	qRR
	qRT
	qRE
	}

	void Encrypt_(
		const char *Source,
		const char *Destination,
		const char *Namespace,
		bso::bool__ Indent )
	{
	qRH
		flf::file_oflow___ OFlow;
		txf::text_oflow__ TOFlow;
		flf::file_iflow___ IFlow;
		bso::bool__ BackedUp = false;
		xpp::context___ Context;
		lcl::meaning Meaning;
	qRB
		if ( Source != NULL )
			if ( IFlow.Init( Source, err::hUserDefined ) != tol::rSuccess )
				sclm::ReportFileOpeningErrorAndAbort( Source );

		if ( Destination != NULL ) {
			sclm::CreateBackupFile( Destination );

			BackedUp = true;

			if ( OFlow.Init( Destination, err::hUserDefined ) != tol::rSuccess )
				sclm::ReportFileOpeningErrorAndAbort( Destination );

			TOFlow.Init( OFlow );
		}

		Context.Init();

		if ( xpp::Encrypt( str::string( Namespace == NULL ? DefaultNamespace : Namespace ),
									  IFlow,
									  Indent ? xml::oIndent : xml::oCompact,
									  utf::f_Default,
									  ( Destination == NULL ? COut : TOFlow ),  Context ) != xpp::sOK )	{
			Meaning.Init();

			i18n::GetEncryptionErrorMeaning( Context, Meaning );

			scle::SetMeaning( Meaning );

			qRAbort();
		}
	qRR
		if ( BackedUp )
			sclm::RecoverBackupFile( Destination );
	qRT
	qRE
	}

	void Encrypt_( void )
	{
	qRH
		str::string Input, Output, Namespace, Indentation;
		TOL_CBUFFER___ InputBuffer, OutputBuffer, NamespaceBuffer;
	qRB
		Input.Init();
		sclm::OGetValue( registry::Input, Input );

		Output.Init();
		sclm::OGetValue( registry::Output, Output );

		Namespace.Init();
		sclm::OGetValue( registry::Namespace, Namespace );

		Indentation.Init();
		sclm::OGetValue( registry::Indentation, Indentation );

		Encrypt_( Input.Amount() != 0 ? Input.Convert( InputBuffer ) : NULL, Output.Amount() != 0 ? Output.Convert( OutputBuffer ) : NULL, Namespace.Amount() != 0 ? Namespace.Convert( NamespaceBuffer ) :  NULL, Indentation == "Yes" );
	qRR
	qRT
	qRE
	}
}

const scli::sInfo &sclt::SCLTInfo( void )
{
	return xppq::Info;
}

#define C( name )\
	else if ( Command == #name )\
		name##_()


int sclt::SCLTMain(
	const str::string_ &Command,
	const sclt::oddities__ &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH
qRB
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( Process );
	C( Encrypt );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}
