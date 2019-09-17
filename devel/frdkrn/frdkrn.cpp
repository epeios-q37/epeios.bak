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

#define FRDKRN_COMPILATION_

#include "frdkrn.h"

# include "frdrgy.h"

using namespace frdkrn;

using rgstry::tentry__;


#if 0
#define PROJECT_TYPE_NEW		"New"
#define PROJECT_TYPE_PREDEFINED	"Predefined"
#define PROJECT_TYPE_USER		"User"

project_type__ frdkrn::GetProjectType(
	const str::string_ &Label,
	err::handling__ ErrHandling )
{
	if ( Label == PROJECT_TYPE_NEW )
		return ptNew;
	else if ( Label == PROJECT_TYPE_PREDEFINED )
		return ptPredefined;
	else if ( Label == PROJECT_TYPE_USER )
		return ptUser;
	else if ( ErrHandling = err::hThrowException )
		qRFwk();
	else if ( ErrHandling != err::hUserDefined )
		qRFwk();

	return pt_Undefined;
}

const str::string_ &frdkrn::GetPredefinedProjectLocation(
	const str::string_ &Id,
	const registry_ &Registry,
	str::string_ &Location )
{
qRH
	rgstry::tags Tags;
qRB
	Tags.Init();
	Tags.Append( Id );

	Registry.GetValue( tentry__( frdrgy::PredefinedProject, Tags ), Location );
qRR
qRT
qRE
	return Location;
}


#define BACKEND_EXTENDED_TYPE_NONE		"None"
#define BACKEND_EXTENDED_TYPE_EMBEDDED		"Embedded"
#define BACKEND_EXTENDED_TYPE_DAEMON		"Daemon"
#define BACKEND_EXTENDED_TYPE_PREDEFINED	"Predefined"

backend_extended_type__ frdkrn::GetBackendExtendedType( const str::string_ &RawType )
{
	if ( RawType == BACKEND_EXTENDED_TYPE_EMBEDDED )
		return bxtEmbedded;
	else if ( RawType == BACKEND_EXTENDED_TYPE_DAEMON )
		return bxtDaemon;
	else if ( RawType == BACKEND_EXTENDED_TYPE_PREDEFINED )
		return bxtPredefined;
	else if ( RawType == BACKEND_EXTENDED_TYPE_NONE )
		return bxtNone;
	else
		return bxt_Undefined;
}

static const char *GetBackendExtendedTypeLabel_( backend_extended_type__ Type )
{
	switch ( Type ) {
	case bxtNone:
		return BACKEND_EXTENDED_TYPE_NONE;
		break;
	case bxtPredefined:
		return BACKEND_EXTENDED_TYPE_PREDEFINED;
		break;
	case bxtDaemon:
		return BACKEND_EXTENDED_TYPE_DAEMON;
		break;
	case bxtEmbedded:
		return BACKEND_EXTENDED_TYPE_EMBEDDED;
		break;
	default:
		qRFwk();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}


backend_extended_type__ frdkrn::GetBackendExtendedType(	const registry_ &Registry )
{
	backend_extended_type__ Type = bxt_Undefined;
qRH
	str::string RawType;
qRB
	RawType.Init();

	Registry.GetValue( frdrgy::BackendType, RawType );

	Type = GetBackendExtendedType( RawType );
qRR
qRT
qRE
	return Type;
}

void frdkrn::SetBackendExtendedType(
	registry_ &Registry,
	backend_extended_type__ Type )
{
	Registry.SetValue( frdrgy::BackendType, str::string( GetBackendExtendedTypeLabel_( Type ) ) );
}


bso::bool__ frdkrn::GetDefaultConfigurationFileName(
	const char *Affix,
	str::string_ &FileName )
{
	bso::bool__ Exists = false;
qRH
	TOL_CBUFFER___ Buffer;
qRB
	FileName.Init( Affix );
	FileName.Append( '.' );
	FileName.Append( FRDKRN_CONFIGURATION_FILE_EXTENSION );

	Exists = fil::Exists( FileName.Convert( Buffer ) );
qRR
qRT
qRE
	return Exists;
}

static str::string_ &AppendTargetAttributePathItem_(
	const char *TargetName,
	str::string_ &Target )
{
	Target.Append( "[target=\"" );
	Target.Append( TargetName );
	Target.Append( "\"]" );

	return Target;
}
#endif

#if 0
void frdkrn::reporting_callback__::FBLFRDReport(
	fblovl::reply__ Reply,
	const char *Message )
{
qRH
	lcl::meaning Meaning;
	str::string Translation;
qRB
	if ( _Locale == NULL )
		qRFwk();

	if ( _Language = NULL )
		qRFwk();

	Meaning.Init();
	Meaning.SetValue( fblovl::GetLabel( Reply ) );
	Meaning.AddTag( Message );

	Translation.Init();
	_Locale->GetTranslation( Meaning, _Language, Translation );

	this->FRDKRNReport( Translation );
qRR
qRT
qRE
}
#endif


#if 0
static inline bso::uint__ GetBackendPingDelay_( const registry_ &Registry )
{
	return rgstry::GetUInt( Registry, frdrgy::BackendPingDelay, 0 );
}
#endif

bso::bool__ frdkrn::kernel___::_Connect(
	const features___ &Features,
	const compatibility_informations__ &CompatibilityInformations,
	error_set___ &ErrorSet,
	csdsnc::log_callback__ *LogCallback )
{
	bso::bool__ Success = false;
qRH
	csdlec::library_data__ LibraryData;
	csdleo::mode__ Mode = csdleo::m_Undefined;
	TOL_CBUFFER___ Buffer;
qRB
	LibraryData.Init( csdleo::cRegular, Features.Location.Convert( Buffer ), err::qRRor, Features.UP );

	if ( !_ClientCore.Init( Features, LibraryData, LogCallback ) )
		qRReturn;

	if ( !_Frontend.Init( Features.Language, CompatibilityInformations, _ClientCore, *_ReportingCallback, ErrorSet.IncompatibilityInformations ) )
		qRReturn;

	FRDKRNConnection( _Frontend );

	Success = true;
qRR
qRT
	if ( !Success )
		ErrorSet.BackendLocation.Init(  Features.Location );
qRE
	return Success;
}

static bso::bool__ IsProjectIdValid_( const str::string_ &Id )
{
	sdr::row__ Row = Id.First();

	if ( Id.Amount() == 0 )
		return false;

	while ( Row != qNIL ) {
		if ( !isalnum( Id( Row ) ) && ( Id( Row ) != '_' ) )
			return false;

		Row = Id.Next( Row );
	}

	return true;
}

#define PROJECT_ROOT_PATH	"Projects/Project[@target=\"%1\"]"

#define PROJECT_ID_RELATIVE_PATH "@id"

bso::bool__ frdkrn::kernel___::Launch(
	const features___ &Features,
	const compatibility_informations__ &CompatibilityInformations )
{
	bso::bool__ Success = false;
qRH
	error_set___ ErrorSet;
qRB
	ErrorSet.Init();

	if ( !Launch( Features, CompatibilityInformations, ErrorSet ) ) {
		_ErrorMeaning.Init();
		frdkrn::GetMeaning( ErrorSet, _ErrorMeaning );
		qRReturn;
	}

	Success = true;
qRR
qRT
qRE
	return Success;
}

static stsfsm::automat AuthenticationAutomat_;

void InitAuthenticationAutomat_( void )
{
	AuthenticationAutomat_.Init();
	stsfsm::Fill( AuthenticationAutomat_, apm_amount, GetLabel );
}

#define APM( name )	case apm##name : return #name; break

const char *frdkrn::GetLabel( authentication_prompt_mode__ Mode )
{
	switch ( Mode ) {
		APM( None );
		APM( Auto );
		APM( Empty );
		APM( Partial );
		APM( Full );
	default:
		qRFwk();
		break;
	}

	return NULL;	// To avoid a 'warning'.
}

authentication_prompt_mode__ frdkrn::GetAuthenticationPromptMode( const str::string_ &Pattern )
{
	return stsfsm::GetId( Pattern, AuthenticationAutomat_, apm_Undefined, apm_amount );
}

static void InitAutomats_( void )
{
	InitAuthenticationAutomat_();
}

Q37_GCTOR( frdkrn )
{
	InitAutomats_();
}