/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#define SCLFRNTND__COMPILATION

#include "sclfrntnd.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "scllocale.h"
#include "sclmisc.h"

using namespace sclfrntnd;

using sclrgstry::registry_;

namespace parameter_ {
	rgstry::entry___ Backend_( "Backend", sclrgstry::Parameters );

	namespace backend_ {
		rgstry::entry___ AccessMode_( "@AccessMode", Backend_ );
		rgstry::entry___ Type_( "@Type", Backend_ );
		rgstry::entry___ PingDelay_( "PingDelay", Backend_ );
	}

	rgstry::entry___ Authentication_( "Authentication", sclrgstry::Parameters );

	namespace authentication_ {
		rgstry::entry___ CypherKey_( "@CypherKey", Authentication_ );
		rgstry::entry___ Mode_( "@Mode", Authentication_ );
		rgstry::entry___ Login_( "Login", Authentication_ );
		rgstry::entry___ Password_( "Password", Authentication_ );
	}

	rgstry::entry___ DefaultProjectType_("DefaultProjectType", sclrgstry::Parameters );
	rgstry::entry___ DefaultBackendType_("DefaultBackendType", sclrgstry::Parameters );
}

namespace definition_ {

	namespace tagged_project_ {
		rgstry::entry___ Alias_( "@Alias", sclrgstry::definition::TaggedProject );
	}

	rgstry::entry___ Backends_( "Backends", sclrgstry::Definitions );
	rgstry::entry___ DefaultBackendId_( "@Default", Backends_ );

	rgstry::entry___ Backend_( "Backend", Backends_ );

	namespace backend_ {
		rgstry::entry___ Id_( "@id", Backend_ );
	}

	rgstry::entry___ TaggedBackend_( RGSTRY_TAGGING_ATTRIBUTE( "id" ), Backend_);

	namespace tagged_backend_ {
		rgstry::entry___ Alias_( "@Alias", TaggedBackend_ );
		rgstry::entry___ Type_( "@Type", TaggedBackend_ );
	}
}

static rgstry::entry___ Internals_( "Internals" );
static rgstry::entry___ ProjectId_( "ProjectId", Internals_ );

#define C( name ) case bt##name: return #name; break

const char *sclfrntnd::GetLabel( backend_type__ BackendType )
{
	switch ( BackendType ) {
	C( None );
	C( Remote );
	C( Embedded );
	C( Predefined );
	default:
		qRFwk();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}

namespace {

	stsfsm::automat BackendAutomat_;

	void FillBackendAutomat_( void )
	{
		BackendAutomat_.Init();
		stsfsm::Fill( BackendAutomat_, bt_amount, GetLabel );
	}
}

backend_type__ sclfrntnd::GetBackendType( const str::string_ &Pattern )
{
	return stsfsm::GetId( Pattern, BackendAutomat_, bt_Undefined, bt_amount );
}


namespace {
	class kernel___
	{
	private:
		csducl::universal_client_core___ _ClientCore;
	public:
		void reset( bso::bool__ P = true )
		{
			_ClientCore.reset( P );
		}
		E_CVDTOR( kernel___ );
		bso::bool__ Init(
			const features___ &Features,
			csdsnc::log_callback__ *LogCallback = NULL )
		{
			bso::bool__ Success = false;
		qRH
			csdlec::library_data__ LibraryData;
			csdleo::mode__ Mode = csdleo::m_Undefined;
			TOL_CBUFFER___ Buffer;
		qRB
			LibraryData.Init( csdleo::cRegular, Features.Location.Convert( Buffer ), err::qRRor, sclerror::SCLERRORError );

			if ( !_ClientCore.Init( Features, LibraryData, LogCallback ) )
				qRReturn;

			Success = true;
		qRR
		qRT
		qRE
			return Success;
		}
		bso::bool__ Init(
			const features___ &Features,
			csdsnc::log_callback__ &LogCallback )
		{
			return Init( Features, &LogCallback );
		}
		csducl::universal_client_core___ &Core( void )
		{
			return _ClientCore;
		}
	} Kernel_;
}

stsfsm::automat PendingActionAutomat_;

# if 0
static void FillPendingActionAutomat_( void )
{
	PendingActionAutomat_.Init();
	stsfsm::Fill( PendingActionAutomat_, pa_amount, GetLabel );
}

#define A( name )	case pa##name : return #name; break

const char *sclfrntnd::GetLabel( pending_action__ PendingAction )
{
	switch ( PendingAction ) {
	A( None );
	A( Login );
	A( Connect );
	default:
		qRFwk();
		break;
	}

	return NULL;	// To avoid a 'warning'.
}
pending_action__ sclfrntnd::GetPendingAction( const str::string_ &Pattern )
{
	return stsfsm::GetId( Pattern, PendingActionAutomat_, pa_Undefined, pa_amount );
}
#endif

static const lcl::meaning_ &GetMeaning_(
	const char *Message,
	lcl::meaning &Meaning )
{
qRH
	str::string RefinedMessage;
qRB
	RefinedMessage.Init( "FRD_" );
	RefinedMessage.Append( Message );

	Meaning.SetValue( RefinedMessage );
qRR
qRT
qRE
	return Meaning;
}

static void GetPredefinedItem_(
	const rgstry::entry___ &ValueEntry,
	const rgstry::entry___ &AliasEntry,
	const str::string_ &Id,
	const registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	xml::writer_ &Writer )
{
qRH
	str::string Value;
	str::string Translation;
	rgstry::tags Tags;
	TOL_CBUFFER___ Buffer;
qRB
	Tags.Init();
	Tags.Append( Id );

	Value.Init();
	sclrgstry::MGetValue( Registry, rgstry::tentry__( AliasEntry, Tags ), Value );

	Translation.Init();
	Locale.GetTranslation( Value.Convert( Buffer ), Language, Translation );

	Writer.PutAttribute( "Alias", Translation );

	Value.Init();
	sclrgstry::MGetValue( Registry, rgstry::tentry__( ValueEntry, Tags ), Value );

	Writer.PutValue( Value );
qRR
qRT
qRE
}

static void GetPredefinedItems_(
	const char *Tag,
	const rgstry::entry___ &ValueEntry,
	const rgstry::entry___ &AliasEntry,
	const rgstry::values_ &Ids,
	const str::string_ &DefaultProjectId,
	const registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	xml::writer_ &Writer )
{
	ctn::E_CMITEM( rgstry::value_ ) Id;
	sdr::row__ Row = Ids.First();

	Id.Init( Ids );

	while ( Row != qNIL ) {
		Writer.PushTag( Tag );
		Writer.PutAttribute( "id", Id( Row ) );

		if ( DefaultProjectId == Id( Row ) )
			Writer.PutAttribute("Selected", "true" );

		GetPredefinedItem_( ValueEntry,  AliasEntry, Id( Row ), Registry, Locale, Language, Writer );

		Writer.PopTag();

		Row = Ids.Next( Row );
	}
}

static void GetPredefinedItems_(
	const char *Tag,
	const rgstry::entry___ &IdEntry,
	const rgstry::entry___ &DefaultEntry,
	const rgstry::entry___ &ValueEntry,
	const rgstry::entry___ &AliasEntry,
	const registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	xml::writer_ &Writer )
{
qRH
	rgstry::values Ids;
	str::string DefaultId;
qRB
	DefaultId.Init();
	sclrgstry::OGetValue( Registry, DefaultEntry, DefaultId );

	Ids.Init();
	sclrgstry::GetValues( Registry, IdEntry, Ids );

	GetPredefinedItems_( Tag, ValueEntry, AliasEntry, Ids, DefaultId, Registry, Locale, Language, Writer );
qRR
qRT
qRE
}

static void GetFeatures_(
	const char *ItemsTag,
	const char *ItemTag,
	const char *DefaultTypeTag,
	const rgstry::entry___ &DefaultTypeEntry,
	const rgstry::entry___ &IdEntry,
	const rgstry::entry___ &DefaultEntry,
	const rgstry::entry___ &ValueEntry,
	const rgstry::entry___ &AliasEntry,
	const char *Language,
	xml::writer_ &Writer )
{
qRH
	str::string DefaultType;
qRB
	DefaultType.Init();

	if ( sclrgstry::OGetValue( sclrgstry::GetCommonRegistry(), DefaultTypeEntry, DefaultType ) ) {
		Writer.PushTag( DefaultTypeTag );
		Writer.PutValue( DefaultType );
		Writer.PopTag();
	}

	Writer.PushTag( ItemsTag );
	GetPredefinedItems_( ItemTag, IdEntry, DefaultEntry, ValueEntry, AliasEntry, sclrgstry::GetCommonRegistry(), scllocale::GetLocale(), Language, Writer );
	Writer.PopTag();
qRR
qRT
qRE
}

bso::bool__ sclfrntnd::frontend___::Connect(
	const fblfrd::compatibility_informations__ &CompatibilityInformations,
	fblfrd::incompatibility_informations_ &IncompatibilityInformations )
{
	fblfrd::mode__ Mode = fblfrd::m_Undefined;

	switch ( Kernel_.Core().GetType() ) {
	case csducl::tNone:
		Mode = fblovl::mNone;
		break;
	case csducl::tLibrary:
		Mode = fblfrd::mEmbedded;
		break;
	case csducl::tDaemon:
		Mode = fblfrd::mRemote;
		break;
	default:
		qRFwk();
		break;
	}

	if ( Mode != fblovl::mNone )
		_Flow.Init( Kernel_.Core() );

	return _frontend___::Connect( L_(), _Flow, Mode, CompatibilityInformations, IncompatibilityInformations );
}

void sclfrntnd::GetProjectsFeatures(
	const char *Language,
	xml::writer_ &Writer )
{
qRH
	str::string Pattern;
qRB
	GetFeatures_( "Projects", "Project", "DefaultProjectType", parameter_::DefaultProjectType_, sclrgstry::definition::project::Id, sclrgstry::definition::DefaultProjectId, sclrgstry::definition::TaggedProject, definition_::tagged_project_::Alias_, Language, Writer );
qRR
qRT
qRE
}

void sclfrntnd::GetBackendsFeatures(
	const char *Language,
	xml::writer_ &Writer )
{
qRH
	str::string Backend, Type;
qRB
	Backend.Init();

	if ( sclrgstry::OGetValue( sclrgstry::GetCommonRegistry(), parameter_::Backend_, Backend ) ) {
		Type.Init();
		sclrgstry::MGetValue( sclrgstry::GetCommonRegistry(), parameter_::backend_::Type_, Type );

		Writer.PushTag( "Backend" );
		Writer.PutAttribute( "Type", Type );
		Writer.PutValue( Backend );
		Writer.PopTag();
	}

	GetFeatures_( "Backends", "Backend", "DefaultBackendType", parameter_::DefaultBackendType_, definition_::backend_::Id_, definition_::DefaultBackendId_, definition_::Backend_, definition_::tagged_backend_::Alias_, Language, Writer );
qRR
qRT
qRE
}

bso::uint__ sclfrntnd::GetBackendPingDelay( void )
{
	return sclrgstry::OGetUInt(sclrgstry::GetCommonRegistry(), parameter_::backend_::PingDelay_, 0 );
}

static void GetBackendFeatures_(
	const str::string_ &Id,
	features___ &Features )
{
qRH
	str::string Buffer;
	rgstry::tentry___ BackendTypeEntry;
qRB
	BackendTypeEntry.Init( definition_::tagged_backend_::Type_, Id );

	Buffer.Init();
	switch ( GetBackendType(sclrgstry::MGetValue(sclrgstry::GetCommonRegistry(), BackendTypeEntry, Buffer ) ) ) {
	case btRemote:
		Features.Type = csducl::tDaemon;
		break;
	case btEmbedded:
		Features.Type = csducl::tLibrary;
		break;
	default:
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( BackendTypeEntry );
		break;
	}

	sclrgstry::MGetValue(sclrgstry::GetCommonRegistry(), rgstry::tentry___( definition_::TaggedBackend_, Id ), Features.Location );
qRR
qRT
qRE
}

void sclfrntnd::Connect(
	backend_type__ BackendType,
	const str::string_ &BackendFeature )
{
qRH
	features___ Features;
qRB
	Features.Init();

	Features.PingDelay = GetBackendPingDelay();

	switch ( BackendType ) {
	case btNone:
		Features.Type = csducl::tNone;
		break;
	case btRemote:
		Features.Type = csducl::tDaemon;
		Features.Location = BackendFeature;
		break;
	case btEmbedded:
		Features.Type = csducl::tLibrary;
		Features.Location = BackendFeature;
		break;
	case btPredefined:
		GetBackendFeatures_( BackendFeature, Features );
		break;
	case bt_Undefined:
		qRFwk();
		break;
	default:
		qRFwk();
		break;
	}

	if ( !Kernel_.Init( Features ) )
		sclmisc::ReportAndAbort( SCLFRNTND_NAME "_UnableToConnectToBackend", Features.Location );
qRR
qRT
qRE
}

namespace{
	bso::bool__ Connect_( void )
	{
		bso::bool__ BackendDeclared = false;
	qRH
		backend_type__ Type = bt_Undefined;
		str::string Feature, RawType;
	qRB
		Feature.Init();

		if ( sclmisc::OGetValue( parameter_::Backend_, Feature ) ) {
			RawType.Init();
			sclmisc::MGetValue( parameter_::backend_::Type_, RawType );

			if ( ( Type = GetBackendType( RawType ) ) == bt_Undefined )
				sclmisc::ReportAndAbort( SCLFRNTND_NAME "_NoOrBadBackendType" );

			BackendDeclared = true;

			Connect( Type, Feature );
		}
	qRR
	qRT
	qRE
		return BackendDeclared;
	}
}

void sclfrntnd::Connect( void )
{
	if ( !Connect_() )
		sclmisc::ReportAndAbort( SCLFRNTND_NAME "_MissingBackendDeclaration" );
}

const str::string_ &sclfrntnd::GetBackendLocation( str::string_ &Location )
{
	Location.Append(Kernel_.Core().Location() );

	return Location;
}

Q37_GCTOR( sclfrntnd )
{
	FillBackendAutomat_();
}



#if 0
static void FillAutomats_( void )
{
	FillPendingActionAutomat_();
}

Q37_GCTOR( sclfrntnd )
{
	FillAutomats_();
}
#endif