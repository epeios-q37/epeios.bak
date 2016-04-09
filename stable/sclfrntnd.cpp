/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#include "scllocale.h"
#include "sclmisc.h"

using namespace sclfrntnd;

using sclrgstry::registry_;

namespace parameter_ {
	rgstry::entry___ Backend_( "Backend", sclrgstry::Parameters );

	namespace backend_ {
		rgstry::entry___ Type_( "@Type", Backend_ );
		rgstry::entry___ Path_( "@Path", Backend_ );
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

	namespace backends_ {
		rgstry::entry___ DefaultBackendId_( "@Default", Backends_ );

		rgstry::entry___ Backend_( "Backend", Backends_ );

		namespace backend_ {
			rgstry::entry___ Id_( "@id", Backend_ );
		}

		rgstry::entry___ TaggedBackend_( RGSTRY_TAGGING_ATTRIBUTE( "id" ), Backend_);

		namespace tagged_backend_ {
			rgstry::entry___ Alias_( "@Alias", TaggedBackend_ );
			rgstry::entry___ Type_( "@Type", TaggedBackend_ );
			rgstry::entry___ Path_( "@Path", TaggedBackend_ );
		}
	}

	rgstry::entry___ RemoteBackends_( "RemoteBackends", sclrgstry::Definitions );

	namespace remote_backends_ {
		rgstry::entry___ RemoteBackend_( "RemoteBackend", RemoteBackends_ );

		rgstry::entry___ TaggedRemoteBackend_( RGSTRY_TAGGING_ATTRIBUTE( "id" ), RemoteBackend_);

		namespace tagged_remote_backed_ {
			rgstry::entry___ PluginPath_( "PluginPath", TaggedRemoteBackend_ );
		}
	}
}

static rgstry::entry___ Internals_( "Internals" );
static rgstry::entry___ ProjectId_( "ProjectId", Internals_ );

void sclfrntnd::GetRemoteBackendPluginPath(
	const str::string_ &Id,
	str::string_ &Path )
{
	sclmisc::MGetValue( rgstry::tentry___( definition_::remote_backends_::tagged_remote_backed_::PluginPath_, Id ), Path );
}

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

#undef C

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

# if 0
stsfsm::automat PendingActionAutomat_;

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

#define C( name )	case bst##name : return #name; break
 
const char *sclfrntnd::GetLabel( eBackendSetupType Type )
{
	switch ( Type ) {
	C( Id );
	C( Content );
	default:
		qRFwk();
		break;
	}
 
	return NULL;	// To avoid a warning.
}
 
#undef C
 
namespace {
	stsfsm::wAutomat BackendSetupTypeAutomat_;
 
	void FillBackendSetupTypeAutomat_( void )
	{
		BackendSetupTypeAutomat_.Init();
		stsfsm::Fill<eBackendSetupType>( BackendSetupTypeAutomat_, bst_amount, GetLabel );
	}
}
 
eBackendSetupType sclfrntnd::GetBackendSetupType( const str::dString &Pattern )
{
	return stsfsm::GetId( Pattern, BackendSetupTypeAutomat_, bst_Undefined, bst_amount );
}

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
	sdr::row__ Row = Ids.First();

	while ( Row != qNIL ) {
		Writer.PushTag( Tag );
		Writer.PutAttribute( "id", Ids( Row ) );

		if ( DefaultProjectId == Ids( Row ) )
			Writer.PutAttribute("Selected", "true" );

		GetPredefinedItem_( ValueEntry,  AliasEntry, Ids( Row ), Registry, Locale, Language, Writer );

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

	switch ( K_().Core().GetType() ) {
	case csducl::tNone:
		Mode = fblovl::mNone;
		break;
	case csducl::tLibrary:
		Mode = fblfrd::mEmbedded;
		break;
	case csducl::tRemote:
		Mode = fblfrd::mRemote;
		break;
	default:
		qRFwk();
		break;
	}

	if ( Mode != fblovl::mNone )
		_Flow.Init( K_().Core() );

	return _frontend___::Connect( Language(), _Flow, Mode, CompatibilityInformations, IncompatibilityInformations );
}

void sclfrntnd::frontend___::Disconnect( void )
{
	_frontend___::Disconnect();

	_Flow.reset();
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

	GetFeatures_( "Backends", "Backend", "DefaultBackendType", parameter_::DefaultBackendType_, definition_::backends_::backend_::Id_, definition_::backends_::DefaultBackendId_, definition_::backends_::Backend_, definition_::backends_::tagged_backend_::Alias_, Language, Writer );
qRR
qRT
qRE
}

static void GetBackendFeatures_(
	const str::string_ &Id,
	features___ &Features )
{
qRH
	str::string Buffer;
	rgstry::tentry___ BackendTypeEntry;
qRB
	BackendTypeEntry.Init( definition_::backends_::tagged_backend_::Type_, Id );

	Buffer.Init();
	switch ( GetBackendType(sclrgstry::MGetValue(sclrgstry::GetCommonRegistry(), BackendTypeEntry, Buffer ) ) ) {
	case btNone:
		Features.Type = csducl::tNone;
		break;
	case btRemote:
		Features.Type = csducl::tRemote;
		sclrgstry::MGetValue(sclrgstry::GetCommonRegistry(), rgstry::tentry___( definition_::backends_::tagged_backend_::Path_, Id ), Features.Path );
		sclrgstry::OGetValue(sclrgstry::GetCommonRegistry(), rgstry::tentry___( definition_::backends_::TaggedBackend_, Id ), Features.Parameters );
		break;
	case btEmbedded:
		Features.Type = csducl::tLibrary;
		sclrgstry::MGetValue(sclrgstry::GetCommonRegistry(), rgstry::tentry___( definition_::backends_::tagged_backend_::Path_, Id ), Features.Path );
		sclrgstry::OGetValue(sclrgstry::GetCommonRegistry(), rgstry::tentry___( definition_::backends_::TaggedBackend_, Id ), Features.Parameters );
		break;
	default:
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( BackendTypeEntry );
		break;
	}

qRR
qRT
qRE
}

void sclfrntnd::SetBackendFeatures(
	backend_type__ BackendType,
	const str::string_ &Path,
	const str::string_ &Parameters,
	features___ &Features )
{
	switch ( BackendType ) {
	case btNone:
		Features.Type = csducl::tNone;
		break;
	case btEmbedded:
		Features.Type = csducl::tLibrary;
		Features.Path = Path;
		Features.Parameters = Parameters;
		break;
	case btRemote:
		Features.Type = csducl::tRemote;
		Features.Path = Path;
		Features.Parameters = Parameters;
		break;
	case btPredefined:
		GetBackendFeatures_( Parameters, Features );
		break;
	default:
		qRFwk();
		break;
	}
}

sdr::sRow sclfrntnd::kernel___::Init(
	const features___ &Features,
	const plgn::dAbstracts &Abstracts )
{
	sdr::sRow Row = qNIL;
qRH
	csdlec::library_data__ LibraryData;
	csdleo::mode__ Mode = csdleo::m_Undefined;
	TOL_CBUFFER___ Buffer;
	bso::bool__ Success = false;
	sclmisc::sRack SCLRack;
qRB
	SCLRack.Init();

	switch ( Features.Type ) {
	case csducl::tNone:
		if ( !_ClientCore.InitNone() )
			qRFwk();
		break;
	case csducl::tLibrary:
		LibraryData.Init( csdleo::cRegular, Features.Path.Convert( Buffer ), &SCLRack );
		if ( !_ClientCore.InitLibrary( Features.Path, LibraryData ) )
			qRFwk();
		break;
	case csducl::tRemote:
		Row = _ClientCore.InitRemote( Features.Path, Features.Identifier, Features.Parameters, Abstracts );
		break;
	default:
		qRFwk();
		break;
	}
qRR
qRT
qRE
	return Row;
}

namespace {
	const str::dString &BuildAbout_(
		csducl::type__ Type,
		const str::dString &Identifier,
		const str::dString &Details,
		str::dString &About )
	{
		switch ( Type ) {
		case csducl::tNone:
			sclmisc::GetBaseTranslation( SCLFRNTND_NAME "_NoBackend", About );
			break;
		case csducl::tLibrary:
			sclmisc::GetBaseTranslation( SCLFRNTND_NAME "_EmbeddedBackend", About );
			break;
		case csducl::tRemote:
			About.Append( Details );
			About.Append(" - {" );
			About.Append( Identifier );
			About.Append("}" );
			break;
		default:
			qRFwk();
			break;
		}

		return About;
	}

}

const str::dString &sclfrntnd::kernel___::AboutPlugin( str::dString &About )
{
	return BuildAbout_( _ClientCore.GetType(), str::wString( _ClientCore.RemoteDetails() ), str::wString( _ClientCore.RemoteIdentifier() ), About );
}

namespace{
	bso::bool__ GuessBackendFeatures_( features___ &Features )
	{
		bso::bool__ BackendFound = false;
	qRH
		backend_type__ Type = bt_Undefined;
		str::string RawType, Parameters, Path;
	qRB
		Parameters.Init();

		if ( sclmisc::OGetValue( parameter_::Backend_, Parameters ) ) {
			RawType.Init();
			sclmisc::MGetValue( parameter_::backend_::Type_, RawType );

			Path.Init();
			sclmisc::OGetValue( parameter_::backend_::Path_, Path  );

			if ( ( Type = GetBackendType( RawType ) ) == bt_Undefined )
				sclmisc::ReportAndAbort( SCLFRNTND_NAME "_NoOrBadBackendType" );

			SetBackendFeatures( Type, Path, Parameters, Features );
		}

		BackendFound = true;
	qRR
	qRT
	qRE
		return BackendFound;
	}
}

void sclfrntnd::GuessBackendFeatures( features___ &Features )
{
	if ( !GuessBackendFeatures_( Features ) )
		sclmisc::ReportAndAbort( SCLFRNTND_NAME "_MissingBackendDeclaration" );
}

const str::string_ &sclfrntnd::GetBackendPath(
	const kernel___ &Kernel,
	str::string_ &Location )
{
	Location.Append( Kernel.Core().Location() );

	return Location;
}

const str::dString &sclfrntnd::About(
	const rFeatures &Features,
	str::dString &About )
{
qRH
	sclmisc::sRack SCLRack;
	str::wString Identifier, Details;
qRB
	SCLRack.Init();

	Identifier.Init();
	Details.Init();

	switch ( Features.Type ) {
	case csducl::tNone:
	case csducl::tLibrary:
		break;
	case csducl::tRemote:
		Identifier.Init();
		Details.Init();
		plgn::IdentifierAndDetails( Features.Path, Identifier, Details );
		break;
	default:
		qRFwk();
		break;
	}

	BuildAbout_( Features.Type, Identifier, Details, About );
qRR
qRT
qRE
	return About;
}


Q37_GCTOR( sclfrntnd )
{
	FillBackendAutomat_();
	FillBackendSetupTypeAutomat_();
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