/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#define SCLBACKND_COMPILATION_

#include "sclbacknd.h"

#include "sclmisc.h"
#include "scllocale.h"
#include "sclerror.h"
#include "scldaemon.h"

#include  "lcl.h"
#include "cio.h"

#include "csdleo.h"
#include "csdles.h"

// #include "fblbur.h"

#include "fnm.h"

using namespace sclbacknd;

namespace {
	sclrgstry::rEntry
		Watchdog_( "Watchdog", sclrgstry::Parameters ),
		Code_( "Code", Watchdog_ ),
		Key_( "Key", Watchdog_ );
}

#define DEC( name, version )\
	static inline void name##_##version##_(\
		rBackend_ &BaseBackend,\
		fblbkd::rRequest &Request )

DEC( LoadSetupOfId, 1 )
{
qRH
qRB
	sclbacknd::rBackend &Backend = *(sclbacknd::rBackend *)BaseBackend.UP();

	Backend.FillSetupRegistryFollowingId( Request.StringIn() );
qRR
qRT
qRE
}

DEC( LoadSetupContent, 1 )
{
qRH
	str::wString Content;
qRB
	sclbacknd::rBackend &Backend = *(sclbacknd::rBackend *)BaseBackend.UP();

	const str::string_ &RawContent = Request.StringIn();

	Content.Init( "<" );
	sclrgstry::Parameters.GetPath( Content );
	Content.Append( '>' );
	Content.Append( RawContent  );
	Content.Append( "</" );
	sclrgstry::Parameters.GetPath( Content );
	Content.Append( '>' );


	Backend.FillSetupRegistryWithContent( Content );
qRR
qRT
qRE
}

#define D( name, version )	#name "_" #version, ::name##_##version##_

namespace {
	void AppendFunctions_( rBackend_ &Backend )
	{
		Backend.Add( D( LoadSetupOfId, 1 ),
				fblbkd::cString,	// Id of the setup to load.
			fblbkd::cEnd,
			fblbkd::cEnd );

		Backend.Add( D( LoadSetupContent,1  ),
				fblbkd::cString,	// Content of the setup to load.
			fblbkd::cEnd,
			fblbkd::cEnd );
	}
}



void sclbacknd::backend___::Init(
	fblovl::eMode Mode,
	const char *APIVersion,
	const ntvstr::char__ *ClientOrigin,
	const char *BackendLabel,
	const char *BackendInformations,
	const char *BackendCopyright,
	const char *SoftwareInformations )
{
qRH
	str::wString Code, Key;
qRB
	CompatibilityTested_ = false;

	Code.Init();
	sclmisc::OGetValue( ::Code_, Code );

	Key.Init();
	sclmisc::OGetValue( ::Key_, Key );

	rBackend_::Init( Mode, APIVersion, ClientOrigin, BackendLabel, scllocale::GetLocale(), BackendInformations, BackendCopyright, SoftwareInformations, Code, Key );
	_VoidFlowDriver.Init( fdr::tsDisabled, flx::aAllowed );
	_RequestLogFunctions.Init( _VoidFlowDriver );
	_Registry.Init();
	_Registry.Push( sclmisc::GetRegistry() );
	_RegistrySetupLevel = _Registry.Create();

	AppendFunctions_( *this );
qRR
qRT
qRE
}


scldaemon::rCallback *scldaemon::SCLDAEMONGetCallback(
	csdleo::context__ Context,
	csdleo::mode__ CSDMode )
{
	sclbacknd::callback__ *Callback = NULL;
qRH
	fblovl::eMode FBLMode = fblovl::m_Undefined;
qRB
	switch ( CSDMode ) {
	case csdleo::mEmbedded:
		FBLMode = fblovl::mReferenced;
		break;
	case csdleo::mRemote:
		FBLMode = fblovl::mSerialized;
		break;
	default:
		qRFwk();
		break;
	}

	Callback = SCLBACKNDGetCallback( Context, FBLMode );
qRR
	if ( Callback != NULL )
		delete Callback;

	Callback = NULL;
qRT
qRE
	return Callback;
}

namespace {
	void ReportRequestError_(
		const char *Language,
		fdr::rWDriver &ODriver )
	{
		qRH
			str::string Translation;
			TOL_CBUFFER___ Buffer;
			flw::rDressedWFlow<> Flow;
		qRB
			Flow.Init( ODriver );

			Translation.Init();
			sclerror::GetPendingErrorTranslation( Language, Translation );

			sclerror::ResetPendingError();

			fblbrq::Report( fblovl::rRequestError, Translation.Convert( Buffer ), Flow );

			Flow.Commit( false );
		qRR
		qRT
		qRE
	}

	void ReportSoftwareError_(
		const char *Language,
		fdr::rWDriver &ODriver )
	{
	qRH
		err::buffer__ ErrorBuffer;
//		const char *ErrMsg = NULL;
		lcl::meaning Meaning;
		str::wString Translation;
		qCBUFFERr Buffer;
		flw::rDressedWFlow<> Flow;
	qRB
		Flow.Init( ODriver );

		Meaning.Init();

		Meaning.SetValue( SCLBACKND_NAME "_BackendError" );
		Meaning.AddTag( err::Message( ErrorBuffer ) );

		Translation.Init();
		scllocale::GetTranslation( Meaning, Language, Translation );

		fblbrq::Report( fblovl::rSoftwareError, Translation.Convert( Buffer ), Flow );

		Flow.Commit( false );
	qRR
	qRT
	qRE
	}
}

bso::sBool sclbacknd::backend___::SCLDAEMONProcess( fdr::rRWDriver *IODriver )
{
	bso::bool__ Continue = true;
qRH
qRB
	if ( CompatibilityTested_ )
		Continue = HandleRequest( *IODriver, _RequestLogFunctions );
	else {
		Continue = TestCompatibility( *IODriver );
		CompatibilityTested_ = true;
	}
qRR
	if ( ERRType == err::t_Abort )
		ReportRequestError_ (Language(), *IODriver );
	else
		ReportSoftwareError_( Language(), *IODriver );

	ERRRst();
qRT
qRE
	return Continue;
}
