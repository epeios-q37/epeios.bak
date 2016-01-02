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

#define SCLRGSTRY__COMPILATION

#include "sclrgstry.h"

#include "cio.h"
#include "lcl.h"
#include "dir.h"
#include "fnm.h"
#include "tagsbs.h"

#include "sclerror.h"
#include "sclmisc.h"

using namespace sclrgstry;

#define REGISTRY_FILE_EXTENSION ".xcfg"

static rgstry::multi_level_registry Registry_;

static rgstry::level__ ConfigurationLevel_ = rgstry::UndefinedLevel;
static rgstry::level__ ProjectLevel_ = rgstry::UndefinedLevel;
static rgstry::level__ SetupLevel_ = rgstry::UndefinedLevel;
static rgstry::level__ ArgumentsLevel_ = rgstry::UndefinedLevel;

rgstry::entry___ sclrgstry::Parameters( ParametersTag );
rgstry::entry___ sclrgstry::Definitions( "Definitions" );
rgstry::entry___ sclrgstry::Locale( "Locale" );
rgstry::entry___ sclrgstry::Arguments( "Arguments" );
rgstry::entry___ sclrgstry::Debug( "Debug", Parameters );

rgstry::entry___ sclrgstry::parameter::Language( "Language", Parameters );

# if 0
	rgstry::entry___ ProjectFeature_( "Project", sclrgstry::Parameters );
	rgstry::entry___ ProjectType_( "@Type", ProjectFeature_ );
	rgstry::entry___ ProjectAction_("@Action", ProjectFeature_ );
	rgstry::entry___ PredefinedProjects_( "PredefinedProjects", sclrgstry::Definitions );
	rgstry::entry___ DefaultPredefinedProject_( "@Default", PredefinedProjects_ );
	rgstry::entry___ FreePredefinedProject_( "PredefinedProject", PredefinedProjects_ ); 
	rgstry::entry___ PredefinedProjectId_( "@id", FreePredefinedProject_ );
	rgstry::entry___ PredefinedProject_( RGSTRY_TAGGING_ATTRIBUTE( "id" ), FreePredefinedProject_);
	rgstry::entry___ PredefinedProjectAlias_( "@Alias", PredefinedProject_ );
# endif

rgstry::entry___ sclrgstry::parameter::Project("Project", Parameters );
rgstry::entry___ sclrgstry::parameter::project::Type( "@Type", parameter::Project );
rgstry::entry___ sclrgstry::parameter::project::Feature( parameter::Project );

namespace definition_ {
	rgstry::entry___ Projects_( "Projects", sclrgstry::Definitions );
}

rgstry::entry___ sclrgstry::definition::DefaultProjectId( "@Default", definition_::Projects_ );
rgstry::entry___ sclrgstry::definition::Project( "Project", definition_::Projects_ ); 
rgstry::entry___ sclrgstry::definition::project::Id( "@id", Project );
rgstry::entry___ sclrgstry::definition::TaggedProject( RGSTRY_TAGGING_ATTRIBUTE( "id" ), definition::Project );


namespace parameter_ {
	rgstry::entry___ Plugins_( "Plugins", sclrgstry::Parameters );
}

rgstry::entry___ sclrgstry::parameter::Plugin( RGSTRY_TAGGED_ENTRY( "Plugin", "target" ), parameter_::Plugins_ );

namespace definition {
	static rgstry::entry___ Plugins_( RGSTRY_TAGGED_ENTRY( "Plugins", "target" ), sclrgstry::Definitions );
	static rgstry::entry___ Plugin_( RGSTRY_TAGGED_ENTRY( "Plugin", "id" ), Plugins_ );
}

rgstry::entry___ sclrgstry::definition::plugin::Filename( "Filename", ::definition::Plugin_ );
rgstry::entry___ sclrgstry::definition::plugin::Configuration( "Configuration", ::definition::Plugin_ );
rgstry::entry___ sclrgstry::definition::plugin::Locale( "Locale", ::definition::Plugin_ );
rgstry::entry___ sclrgstry::definition::plugin::Arguments( "Arguments", ::definition::Plugin_ );

static rgstry::entry___ Setup_( "@Setup", sclrgstry::Parameters );

registry_ &sclrgstry::GetCommonRegistry( void )
{
	return Registry_;
}

#define C( name )	case n##name : return #name; break
 
const char *sclrgstry::GetLabel( name__ Name )
{
	switch ( Name ) {
	C( Configuration );
	C( Project );
	C( Setup );
	C( Arguments );
	default:
		qRFwk();
		break;
	}
 
	return NULL;	// To avoid a warning.
}
 
#undef C

#define C( name )\
case n##name:\
	return name##Level_;\
	break

rgstry::level__ sclrgstry::GetLevel( name__ Name )
{
	switch ( Name ) {
	C( Configuration );
	C( Project );
	C( Setup );
	C( Arguments );
	default:
		qRFwk();
		break;
	}

	return rgstry::UndefinedLevel;	// To avoid a warning.
}

#undef C

const char *sclrgstry::GetLanguage_(
	const registry_ &Registry,
	TOL_CBUFFER___ &Buffer )
{
	sclrgstry::OGetValue( Registry, parameter::Language, Buffer );

	if ( Buffer == NULL )
		return sclmisc::GetBaseLanguage();
	else
		return Buffer;
}

static rgstry::status__ FillConfigurationRegistry_(
	xtf::extended_text_iflow__ &Flow,
	const char *Directory,
	const char *RootPath,
	rgstry::context___ &Context )
{
	return Registry_.Fill( ConfigurationLevel_, Flow, xpp::criterions___( str::string( Directory ) ), RootPath );
}

void sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( const rgstry::tentry__ &Entry )
{
qRH
	str::string Path;
qRB
	Path.Init();
	sclmisc::ReportAndAbort( SCLRGSTRY_NAME "_BadOrNoValueForEntry", Entry.GetPath( Path ) );
qRR
qRT
qRE
}

static inline void ReportFileParsingErrorAndAbort_(
	const char *ErrorLabel,
	const rgstry::context___ &Context )
{
qRH
	lcl::meaning Meaning;
	lcl::meaning MeaningBuffer;
qRB
	Meaning.Init();
	Meaning.SetValue( ErrorLabel );

	MeaningBuffer.Init();
	rgstry::GetMeaning( Context, MeaningBuffer );

	Meaning.AddTag( MeaningBuffer );

	sclmisc::ReportAndAbort( Meaning );
qRR
qRT
qRE
}

void sclrgstry::SetConfiguration( const rgstry::entry__ &Entry )
{
	Registry_.Erase( ConfigurationLevel_ );

	Registry_.Set( ConfigurationLevel_, Entry );
}

void sclrgstry::LoadConfiguration(
	xtf::extended_text_iflow__&Flow,
	const char *Directory,
	const char *RootPath )
{
qRH
	rgstry::context___ Context;
qRB
	Registry_.Erase( ConfigurationLevel_ );

	Context.Init();

	if ( FillConfigurationRegistry_( Flow, Directory, RootPath, Context ) != rgstry::sOK )
		ReportFileParsingErrorAndAbort_( SCLRGSTRY_NAME "_ConfigurationFileParsingError", Context );
qRR
qRT
qRE
}

void sclrgstry::EraseProjectRegistry( void )
{
	Registry_.Erase( ProjectLevel_ );
}

#define PROJECT_ROOT_PATH	"Projects/Project[@target=\"%1\"]"

template <typename source> static void LoadProject_(
	source &Source,
	const char *Target,
	str::string_ &Id )
{
qRH
	str::string Path;
	TOL_CBUFFER___ Buffer;
	rgstry::context___ Context;
qRB
	Path.Init( PROJECT_ROOT_PATH );
	tagsbs::SubstituteShortTag( Path, 1, str::string( Target ), '%' );

	EraseProjectRegistry();

	Context.Init();
	if ( Registry_.Fill( ProjectLevel_, Source, xpp::criterions___(), Path.Convert( Buffer ), Context ) != rgstry::sOK )
		ReportFileParsingErrorAndAbort_( SCLRGSTRY_NAME "_ProjectFileParsingError", Context );

	Registry_.GetValue( ProjectLevel_, rgstry::entry___( "@Id" ), Id );
qRR
qRT
qRE
}

void sclrgstry::LoadProject(
	flw::iflow__ &Flow,
	const char *Target,
	str::string_ &Id )
{
qRH
	xtf::extended_text_iflow__ XFlow;
qRB
	XFlow.Init( Flow, utf::f_Guess );

	LoadProject_( XFlow, Target, Id );
qRR
qRT
qRE
}

void sclrgstry::LoadProject(
	const fnm::name___ &Filename,
	const char *Target,
	str::string_ &Id )
{
	LoadProject_( Filename, Target, Id );
}

void sclrgstry::EraseSetupRegistry( void )
{
	Registry_.Erase( SetupLevel_ );
}

static const str::string_ &GetSelectedSetupContent_(
	const str::string_ &SetupId,
	str::string_ &Content )
{
qRH
	flx::E_STRING_OFLOW___ OFlow;
	txf::text_oflow__ TFlow;
	rgstry::row__ Row = qNIL;
	rgstry::level__ Level = rgstry::UndefinedLevel;
	str::string SetupPath;
qRB
	if ( SetupId.Amount() == 0  )
		qRReturn;

	SetupPath.Init( "Setups/Setup[id=\"" );
	SetupPath.Append( SetupId );
	SetupPath.Append( "\"]" );

	OFlow.Init( Content );
	TFlow.Init( OFlow );

	Row = Registry_.Search( SetupPath, Level );

	if ( Row != qNIL )
		Registry_.Dump( Level, Row, false, xml::oCompact, xml::e_None, TFlow );
qRR
qRT
qRE
	return Content;
}

void sclrgstry::FillWithSetup(
	registry_ &Registry,
	rgstry::level__ Level,
	const str::string_ &Id )
{
qRH
	str::string Setup;
	flx::E_STRING_IFLOW__ IFlow;
	xtf::extended_text_iflow__ XFlow;
	str::string EntryPath;
qRB
	if ( Id.Amount() == 0 )
		qRFwk();

	Setup.Init();

	Setup.Append('<' );
	sclrgstry::Parameters.GetPath( Setup );
	Setup.Append('>' );

	GetSelectedSetupContent_( Id, Setup );

	Setup.Append( "</" );
	sclrgstry::Parameters.GetPath( Setup );
	Setup.Append('>' );


	IFlow.Init( Setup );
	XFlow.Init( IFlow, utf::f_Default );
	Registry.Fill( Level, XFlow, xpp::criterions___(), NULL );
qRR
qRT
qRE
}

void sclrgstry::FillWithSetup(
	registry_ &Registry,
	rgstry::level__ Level )
{
qRH
	str::string Id;
qRB
	Id.Init();

	if ( OGetValue( Registry_, Setup_, Id ) )
		FillWithSetup( Registry, Level, Id );
qRR
qRT
qRE
}

void sclrgstry::ReportIfNoSetupId( void )
{
	if ( !Registry_.Exists( Setup_  ) )
		ReportBadOrNoValueForEntryErrorAndAbort( Setup_ );
}

bso::bool__ sclrgstry::BGetValue(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	return Registry.GetValue( Entry, Value );
}

void sclrgstry::AddValue(
	registry_ &Registry,
	const str::string_ &Value,
	const rgstry::tentry__ &Entry )
{
	Registry.AddValue( Entry, Value );
}

void sclrgstry::AddValue(
	registry_ &Registry,
	const str::string_ &Path,
	const str::string_ &Value,
	sdr::row__ *Error )
{
	Registry.AddValue( Path, Value, Error );
}

void sclrgstry::SetValue(
	registry_ &Registry,
	const str::string_ &Value,
	const rgstry::tentry__ &Entry )
{
	Registry.SetValue( Entry, Value );
}

void sclrgstry::SetValue(
	registry_ &Registry,
	const str::string_ &Path,
	const str::string_ &Value,
	sdr::row__ *Error )
{
	Registry.SetValue( Path, Value, Error );
}

bso::bool__ sclrgstry::GetValues(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	str::strings_ &Values )
{
	return Registry.GetValues( Entry, Values );
}

bso:: bool__ sclrgstry::OGetValue(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	return BGetValue( Registry, Entry, Value );
}

const char *sclrgstry::OGetValue(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	TOL_CBUFFER___ &Buffer )
{
	const char *ReturnedValue = NULL;
qRH
	str::string Value;
	bso::bool__ Missing = false;
qRB
	Value.Init();

	if ( OGetValue( Registry, Entry, Value ) )
		ReturnedValue = Value.Convert( Buffer );
qRR
qRT
qRE
	return ReturnedValue;
}

const str::string_ &sclrgstry::MGetValue(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	if ( !BGetValue( Registry, Entry, Value ) )
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( Entry );

	return Value;
}

const char *sclrgstry::MGetValue(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	TOL_CBUFFER___ &Buffer )
{
qRH
	str::string Value;
qRB
	Value.Init();

	MGetValue( Registry, Entry, Value );

	Value.Convert( Buffer );
qRR
qRT
qRE
	return Buffer;
}

static tol::extended_boolean__ GetBoolean_( const str::string_ &Value )
{
	tol::xbool__ Boolean = tol::xb_Undefined;
qRH
	str::string Buffer;
qRB
	Buffer.Init( Value );
	str::ToLower( Buffer );

	if ( ( Buffer == "true" ) || ( Buffer == "yes" ) )
		Boolean = tol::xbTrue;
	else if ( ( Buffer == "false" ) || ( Buffer == "no" ) )
		Boolean = tol::xbFalse;
qRR
qRT
qRE
	return Boolean;
}

bso::bool__ sclrgstry::BGetBoolean(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	bso::bool__ DefaultValue )
{
	bso::bool__ &Result = DefaultValue;
qRH
	str::string Value;
qRB
	Value.Init();

	if ( BGetValue( Registry, Entry, Value ) ) {
		switch ( GetBoolean_( Value ) ) {
		case tol::xbFalse:
			Result = false;
			break;
		case tol::xbTrue:
			Result = true;
			break;
		case tol::xb_Undefined:
			sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( Entry );
			break;
		default:
			qRFwk();
			break;
		}
	}
qRR
qRT
qRE
	return Result;
}

bso::bool__ sclrgstry::MGetBoolean(
	const registry_ &Registry,
	const rgstry::tentry___ &Entry )
{
	bso::bool__ Result = false;
qRH
	str::string Value;
qRB
	Value.Init();

	switch ( GetBoolean_( MGetValue( Registry, Entry, Value ) ) ) {
	case tol::xbFalse:
		Result = false;
		break;
	case tol::xbTrue:
		Result = true;
		break;
	case tol::xb_Undefined:
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( Entry );
		break;
	default:
		qRFwk();
		break;
	}
qRR
qRT
qRE
	return Result;
}


template <typename t> static bso::bool__ GetUnsignedNumber_(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	t Limit,
	t &Value )
{
	bso::bool__ Present = false;
qRH
	str::string RawValue;
	sdr::row__ Error = qNIL;
qRB
	RawValue.Init();

	if ( !( Present = BGetValue( Registry, Entry, RawValue ) ) )
		qRReturn;

	RawValue.ToNumber( Value, Limit, &Error );

	if ( Error != qNIL )
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( Entry );
qRR
qRT
qRE
	return Present;
}

template <typename t> static bso::bool__ GetSignedNumber_(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	t LowerLimit,
	t UpperLimit,
	t &Value )
{
	bso::bool__ Present = false;
qRH
	str::string RawValue;
	sdr::row__ Error = qNIL;
qRB
	RawValue.Init();

	if ( !( Present = BGetValue( Registry, Entry, RawValue ) ) )
		qRReturn;

	RawValue.ToNumber( Value, UpperLimit, LowerLimit, &Error );

	if ( Error != qNIL )
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( Entry );
qRR
qRT
qRE
	return Present;
}

#define UN( name, type )\
type sclrgstry::MGet##name(\
	const registry_ &Registry,\
	const rgstry::tentry__ &Entry,\
	type Limit  )\
{\
	type Value;\
	\
	if ( !GetUnsignedNumber_( Registry, Entry, Limit, Value ) )\
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( Entry );\
	\
	return Value;\
}\
type sclrgstry::OGet##name(\
	const registry_ &Registry,\
	const rgstry::tentry__ &Entry,\
	type DefaultValue,\
	type Limit )\
{\
	type Value;\
	\
	if ( !GetUnsignedNumber_( Registry, Entry, Limit, Value ) )\
		Value = DefaultValue;\
	\
	return Value;\
}


UN( UInt, bso::uint__ )
#ifdef BSO__64BITS_ENABLED
UN( U64, bso::u64__ )
#endif
UN( U32, bso::u32__ )
UN( U16, bso::u16__ )
UN( U8, bso::u8__ )

#define SN( name, type )\
type sclrgstry::MGet##name(\
	const registry_ &Registry,\
	const rgstry::tentry__ &Entry,\
	type Min,\
	type Max)\
{\
	type Value;\
	\
	if ( !GetSignedNumber_( Registry, Entry, Min, Max, Value ) )\
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( Entry );\
	\
	return Value;\
}\
type sclrgstry::OGet##name(\
	const registry_ &Registry,\
	const rgstry::tentry__ &Entry,\
	type DefaultValue,\
	type Min,\
	type Max )\
{\
	type Value;\
	\
	if ( !GetSignedNumber_( Registry, Entry, Min, Max, Value ) )\
		Value = DefaultValue;\
	\
	return Value;\
}

SN( SInt, bso::sint__ )
#ifdef BSO__64BITS_ENABLED
SN( S64, bso::s64__ )
#endif
SN( S32, bso::s32__ )
SN( S16, bso::s16__ )
SN( S8, bso::s8__ )

Q37_GCTOR( sclrgstry )
{
	Registry_.Init();

	// 3 firsts not as 'embedded', due to the fact that plugins use the registry of the main program.
	ConfigurationLevel_ = Registry_.Create();
	ProjectLevel_ = Registry_.Create();
	SetupLevel_ = Registry_.Create();
	ArgumentsLevel_ = Registry_.CreateEmbedded();
}
