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
		rgstry::entry___ Feature_( Backend_ );
	}

	namespace project_ {
		using namespace sclrgstry::parameter::project;

		rgstry::entry___ Handling_( "@Handling", sclrgstry::parameter::Project );
	}

	rgstry::entry___ Login_( "Login", sclrgstry::Parameters );

	namespace login_ {
		rgstry::entry___ CypherKey_( "@CypherKey", Login_ );
		rgstry::entry___ Mode_( "@Mode", Login_ );
		rgstry::entry___ UserID_( "UserID", Login_ );
		rgstry::entry___ Password_( "Password", Login_ );
	}

	rgstry::rEntry Watchdog_( "Watchdog", sclrgstry::Parameters );

	namespace watchdog {
		sclrgstry::rEntry
			Key_( "Key", Watchdog_ ),
			Code_( "Code", Watchdog_ );
	}
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

	rgstry::entry___ RemotePlugins_( "RemotePlugins", sclrgstry::Definitions );

	namespace remote_plugins_ {
		rgstry::entry___ RemotePlugin_( "RemotePlugin", RemotePlugins_ );

		rgstry::entry___ TaggedRemotePlugin_( RGSTRY_TAGGING_ATTRIBUTE( "id" ), RemotePlugin_);

		namespace tagged_remote_plugin_ {
			rgstry::entry___ Path_( TaggedRemotePlugin_ );
		}
	}
}

void sclfrntnd::GetRemotePluginPath(
	const str::string_ &Id,
	str::string_ &Path )
{
	sclmisc::MGetValue( rgstry::tentry___( definition_::remote_plugins_::tagged_remote_plugin_::Path_, Id ), Path );
}


#define C( name )	case l##name : return #name; break
 
const char *sclfrntnd::GetLabel( eLogin Login )
{
	switch ( Login ) {
	C( Blank );
	C( Partial );
	C( Full );
	C( Automatic );
	default:
		qRFwk();
		break;
	}
 
	return NULL;	// To avoid a warning.
}
 
#undef C
 
namespace {
	stsfsm::wAutomat LoginAutomat_;
 
	void FillLoginAutomat_( void )
	{
		LoginAutomat_.Init();
		stsfsm::Fill<eLogin>( LoginAutomat_, l_amount, GetLabel );
	}
}
 
eLogin sclfrntnd::GetLogin( const str::dString &Pattern )
{
	return stsfsm::GetId( Pattern, LoginAutomat_, l_Undefined, l_amount );
}

eLogin sclfrntnd::GetLoginParameters(
	str::dString &UserID,
	str::dString &Password )
{
	eLogin Login = l_Undefined;
qRH
	str::wString Mode;
qRB
	Mode.Init();

	if ( !sclmisc::OGetValue( parameter_::login_::Mode_, Mode ) ) {
		Login = lBlank;
		qRReturn;
	}

	switch ( Login = GetLogin( Mode ) ) {
	case lBlank:
		break;
	case lFull:
	case lAutomatic:
		sclmisc::MGetValue( parameter_::login_::Password_, Password );
	case lPartial:
		sclmisc::MGetValue( parameter_::login_::UserID_, UserID );
		break;
	default:
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( parameter_::login_::Mode_ );
		break;
	}
qRR
qRT
qRE
	return Login;
}

eLogin sclfrntnd::GetLoginFeatures( xml::dWriter &Writer )
{
	eLogin Login = l_Undefined;
qRH
	str::wString UserID, Password;
qRB
	UserID.Init();
	Password.Init();
	Login = GetLoginParameters( UserID, Password );

	Writer.PushTag( "Login" );
	Writer.PutAttribute("Mode", GetLabel( Login ) );

	switch ( Login ) {
	case lBlank:
		break;
	case lFull:
	case lAutomatic:
		Writer.PutValue( Password, "Password" );
	case lPartial:
		Writer.PutValue( UserID, "UserID" );
		break;
	default:
		qRFwk();
		break;
	}
qRR
qRT
qRE
	return Login;
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
	const rgstry::entry___ &ParameterDefaultEntry,
	const rgstry::entry___ &DefinitionDefaultEntry,
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
	if ( !sclrgstry::OGetValue(Registry, ParameterDefaultEntry, DefaultId)  || ( DefaultId.Amount() == 0 ) )
		sclrgstry::OGetValue( Registry, DefinitionDefaultEntry, DefaultId );

	Ids.Init();
	sclrgstry::GetValues( Registry, IdEntry, Ids );

	Writer.PutAttribute( "Amount", Ids.Amount() );

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
	const rgstry::entry___ &ParameterDefaultEntry,
	const rgstry::entry___ &DefinitionDefaultEntry,
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
	GetPredefinedItems_( ItemTag, IdEntry, ParameterDefaultEntry, DefinitionDefaultEntry, ValueEntry, AliasEntry, sclrgstry::GetCommonRegistry(), scllocale::GetLocale(), Language, Writer );
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

void sclfrntnd::frontend___::Init(
	kernel___ &Kernel,
	const char *Language,
	fblfrd::reporting_callback__ &ReportingCallback,
	const rgstry::multi_level_registry_ &Registry )
{
qRH
	str::wString Key;
qRB
	// _Flow.Init(...);	// Made on connection.
	_Registry.Init();
	_Registry.Push( Registry );
	_RegistryLevel = _Registry.CreateEmbedded( rgstry::name( "Session" ) );

	if ( (Language != NULL) && *Language )
		sclrgstry::SetValue( _Registry, str::string( Language ), rgstry::tentry___( sclrgstry::parameter::Language ) );

	Kernel_ = &Kernel;

	Key.Init();
	sclmisc::OGetValue( parameter_::watchdog::Key_, Key );
	_frontend___::Init( Key, ReportingCallback );
qRR
qRT
qRE
}

void sclfrntnd::frontend___::Ping( void )
{
qRH
	str::wString Code;
qRB
	Code.Init();
	sclmisc::OGetValue( parameter_::watchdog::Code_, Code );
	_frontend___::Ping( Code );	// If 'Code' not empty and correct, the backend doesn't return (watchdog testing purpose).
qRR
qRT
qRE
}

void sclfrntnd::frontend___::Crash( void )
{
qRH
	str::wString Code;
qRB
	Code.Init();
	sclmisc::MGetValue( parameter_::watchdog::Code_, Code );
	_frontend___::Crash( Code );
qRR
qRT
qRE
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
	GetFeatures_( "Projects", "Project", "DefaultProjectType", parameter_::project_::Type, sclrgstry::definition::project::Id, sclrgstry::parameter::project::Feature, sclrgstry::definition::DefaultProjectId, sclrgstry::definition::TaggedProject, definition_::tagged_project_::Alias_, Language, Writer );
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

	if ( sclrgstry::OGetValue( sclrgstry::GetCommonRegistry(), parameter_::backend_::Feature_, Backend ) ) {
		Type.Init();
		sclrgstry::MGetValue( sclrgstry::GetCommonRegistry(), parameter_::backend_::Type_, Type );

		Writer.PushTag( "Backend" );
		Writer.PutAttribute( "Type", Type );
		Writer.PutValue( Backend );
		Writer.PopTag();
	}

	GetFeatures_( "Backends", "Backend", "DefaultBackendType", parameter_::backend_::Type_, definition_::backends_::backend_::Id_,parameter_::backend_::Feature_, definition_::backends_::DefaultBackendId_, definition_::backends_::Backend_, definition_::backends_::tagged_backend_::Alias_, Language, Writer );
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
qRH
	str::wString Id;
qRB
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
		Id.Init( Parameters );

		if ( Id.Amount() == 0 )
			sclmisc::MGetValue( definition_::backends_::DefaultBackendId_, Id );

		GetBackendFeatures_( Id, Features );
		break;
	default:
		qRFwk();
		break;
	}
qRR
qRT
qRE
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

		if ( sclmisc::OGetValue( parameter_::backend_::Feature_, Parameters ) ) {
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

#define C( name )	case ph##name : return #name; break
 
const char *sclfrntnd::GetLabel( eProjectHandling Handling )
{
	switch ( Handling ) {
	C( None );
	C( Load );
	C( Run );
	C( Login );
	default:
		qRFwk();
		break;
	}
 
	return NULL;	// To avoid a warning.
}
 
#undef C
 
namespace {
	stsfsm::wAutomat ProjectHandlingAutomat_;
 
	void FillProjectHandlingAutomat_( void )
	{
		ProjectHandlingAutomat_.Init();
		stsfsm::Fill<eProjectHandling>( ProjectHandlingAutomat_, ph_amount, GetLabel );
	}
}
 
eProjectHandling sclfrntnd::GetProjectHandling( const str::dString &Pattern )
{
	return stsfsm::GetId( Pattern, ProjectHandlingAutomat_, ph_Undefined, ph_amount );
}

eProjectHandling sclfrntnd::HandleProject( void )
{
	eProjectHandling Handling = phNone;;
qRH
	str::wString RawHandling;
qRB
	RawHandling.Init();

	if ( ( sclmisc::OGetValue( parameter_::project_::Handling_, RawHandling ) )
		  && ( RawHandling.Amount() != 0 ) ) {
		Handling = GetProjectHandling( RawHandling );

		if ( Handling == ph_Undefined )
			sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( parameter_::project_::Handling_ );

		switch ( Handling ) {
		case phLoad:
		case phRun:
		case phLogin:
			sclmisc::LoadProject();
			break;
		default:
			qRGnr();
			break;
		}
	}
qRR
qRT
qRE
	return Handling;
}



namespace {
	void FillAutomats_( void )
	{
		FillLoginAutomat_();
		FillBackendAutomat_();
		FillBackendSetupTypeAutomat_();
		FillProjectHandlingAutomat_();
	}
}

Q37_GCTOR( sclfrntnd )
{
	FillAutomats_();
}

