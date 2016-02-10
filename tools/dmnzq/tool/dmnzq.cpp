/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

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

#include "misc.h"

#include "registry.h"

#include "scltool.h"
#include "sclerror.h"

#include "csdlec.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "plgn.h"

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

namespace {
	using misc::module__;

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

	using misc::callback__;

	void Process_(
		callback__ &Callback,
		module__ &Module )
	{
		Callback.Process( Module );
	}

	void Process_( const bso::char__ *ModuleFilename )
	{
	qRH
		lcl::locale SharedLocale;
		rgstry::registry SharedRegistry;
		csdlec::library_data__ LibraryData;
		lcl::meaning Meaning, MeaningBuffer;
		str::string Translation;
		err::buffer__ ErrBuffer;
		plgn::retriever___<callback__> Retriever;
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

		Retriever.Init();

		sclmisc::Plug( misc::SlotPluginTarget, NULL, Retriever );

		Process_( Retriever.Plugin(), Core_->GetCallback() );
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

	void Process_( void )
	{
	qRH
		TOL_CBUFFER___ Buffer;
	qRB
		atexit( ExitFunction_ );

		cio::COut.Commit();

		Process_( sclmisc::MGetValue( registry::Module, Buffer ) );
	qRR
	qRT
	qRE
	}
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

#undef C

const char *sclmisc::SCLMISCTargetName = NAME_LC;

Q37_GCTOR( dmnzq )
{
}