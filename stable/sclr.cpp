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

#define SCLR_COMPILATION_

#include "sclr.h"

#include "sclm.h"

using namespace sclr;

E_CDEF( char *, sclr::ParametersTag, "Parameters" );

#define REGISTRY_FILE_EXTENSION ".xcfg"

static rgstry::multi_layer_registry Registry_;

static rgstry::layer__ MainLayer_ = rgstry::UndefinedLayer;
static rgstry::layer__ LastingLayer_ = rgstry::UndefinedLayer;	// Application data.
static rgstry::layer__ ProjectLayer_ = rgstry::UndefinedLayer;
static rgstry::layer__ SetupLayer_ = rgstry::UndefinedLayer;
static rgstry::layer__ ArgumentsLayer_ = rgstry::UndefinedLayer;
static rgstry::layer__ RuntimeLayer_ = rgstry::UndefinedLayer;	// Temporary registry.

rgstry::entry___ sclr::Parameters( ParametersTag );
rgstry::entry___ sclr::Definitions( "Definitions" );
rgstry::entry___ sclr::Locale( "Locale" );
rgstry::entry___ sclr::Arguments( "Arguments" );
rgstry::entry___ sclr::Debug( "Debug", Parameters );

rgstry::entry___ sclr::parameter::Language( "Language", Parameters );

rgstry::entry___ sclr::parameter::Project("Project", Parameters );
rgstry::entry___ sclr::parameter::project::Type( "@Type", parameter::Project );
rgstry::entry___ sclr::parameter::project::Feature( parameter::Project );

rgstry::rEntry sclr::parameter::DefaultSetupId("@Setup", sclr::Parameters);

namespace definition_ {
	rgstry::entry___ Projects_( "Projects", sclr::Definitions );
}

rgstry::entry___ sclr::definition::DefaultProjectId( "@Default", definition_::Projects_ );
rgstry::entry___ sclr::definition::Project( "Project", definition_::Projects_ );
rgstry::entry___ sclr::definition::project::Id( "@id", Project );
rgstry::entry___ sclr::definition::TaggedProject( RGSTRY_TAGGING_ATTRIBUTE( "id" ), definition::Project );


namespace parameter_ {
	rgstry::entry___ Plugins_( "Plugins", sclr::Parameters );
}

rgstry::entry___ sclr::parameter::TargetedPlugin( RGSTRY_TAGGED_ENTRY( "Plugin", "target" ), parameter_::Plugins_ );
rgstry::entry___ sclr::parameter::targeted_plugin::Id( "@Id", ::parameter::TargetedPlugin );

namespace parameter_ {
	rgstry::entry___ TargetedPluginsSet_( RGSTRY_TAGGED_ENTRY( "Set", "target" ), parameter_::Plugins_ );
}

rgstry::entry___ sclr::parameter::LoosePluginItem( "Plugin", parameter_::TargetedPluginsSet_ );

rgstry::entry___ sclr::parameter::loose_plugin_item::Id( "@Id", parameter::LoosePluginItem );

rgstry::entry___ sclr::parameter::PluginItem( RGSTRY_TAGGING_ATTRIBUTE( "Id" ), parameter::LoosePluginItem );

rgstry::entry___ sclr::parameter::debug::DumpRegistries( "DumpRegistries", Debug );
rgstry::entry___ sclr::parameter::debug::DumpLocales( "DumpLocales", Debug );

namespace definition_ {
	namespace {
		rgstry::entry___ Plugins_( RGSTRY_TAGGED_ENTRY( "Plugins", "target" ), sclr::Definitions );
	}

	static rgstry::entry___ Plugin( RGSTRY_TAGGED_ENTRY( "Plugin", "id" ), Plugins_ );
}

rgstry::entry___ sclr::definition::plugin::Filename( "Filename", ::definition_::Plugin );
rgstry::entry___ sclr::definition::plugin::Configuration( "Configuration", ::definition_::Plugin );
rgstry::entry___ sclr::definition::plugin::Locale( "Locale", ::definition_::Plugin );

namespace setup_ {
	namespace {
		rgstry::rEntry
			Set_("Setups"),
			Loose_("Setup", Set_);
	}

	rgstry::rEntry
		Default("@Default", Set_),
		Tagged(RGSTRY_TAGGING_ATTRIBUTE("id"), Loose_),
		Id("@id", Loose_),
		Alias("@Alias",Tagged);
}

rgstry::rEntry sclr::setup::Setup(setup_::Tagged);

namespace {
	tht::rLocker GlobalLocker_;
	tht::rLocker LastingRegistryLocker_;

	typedef rgstry::cLocker cLocker_;

	class sLastingRegistryLockerCallback_
	: public cLocker_
	{
	protected:
		virtual void RGSTRYLock( void ) override
		{
			LastingRegistryLocker_.Lock();
		}
		virtual void RGSTRYUnlock( void ) override
		{
			LastingRegistryLocker_.Unlock();
		}
	public:
		void reset( bso::sBool P = true )
		{
			if ( P )
				if ( LastingRegistryLocker_.IsLocked() )
					LastingRegistryLocker_.Lock();
		}
		qCVDTOR( sLastingRegistryLockerCallback_ );
		void Init( void )
		{
			reset();
		}
	} LastingRegistryLockerCallback_;
}

registry_ &sclr::GetRWCommonRegistry( void )
{
	if ( !GlobalLocker_.IsLocked() )
		qRFwk();

	return Registry_;
}

const registry_ &sclr::GetCommonRegistry( void )
{
	if ( GlobalLocker_.IsLocked() )
		qRFwk();

	return Registry_;
}

tht::rLocker &sclr::GetCommonRegistryLocker( void )
{
	return GlobalLocker_;
}

#define C( name )	case l##name : return #name; break

const char *sclr::GetLabel( eLayer Layer )
{
	switch ( Layer ) {
	C( Main );
	C( Lasting );
	C( Project );
	C( Setup );
	C( Arguments );
	C( Runtime );
	default:
		qRFwk();
		break;
	}

	return NULL;	// To avoid a warning.
}

#undef C

#define C( name )\
case l##name:\
	return name##Layer_;\
	break

rgstry::layer__ sclr::GetRawLayer( eLayer Layer )
{
	switch ( Layer ) {
	C( Main );
	C( Lasting );
	C( Project );
	C( Setup );
	C( Arguments );
	C( Runtime );
	default:
		qRFwk();
		break;
	}

	return rgstry::UndefinedLayer;	// To avoid a warning.
}

#undef C

const char *sclr::GetLanguage(
	const registry_ &Registry,
	TOL_CBUFFER___ &Buffer )
{
	sclr::OGetValue( Registry, parameter::Language, Buffer );

	if ( Buffer == NULL )
		return sclm::GetBaseLanguage();
	else
		return Buffer;
}

#if 0
static bso::sBool FillRegistry_(
	eLayer Layer,
	xtf::extended_text_iflow__ &Flow,
	const fnm::name___&Directory,
	const char *RootPath,
	rgstry::context___ &Context )
{
	return Registry_.Fill( Layer, Flow, xpp::criterions___( Directory ), RootPath, Context );
}
#endif

void sclr::ReportBadOrNoValueForEntryErrorAndAbort( const rgstry::tentry__ &Entry )
{
qRH
	str::string Path;
qRB
	Path.Init();
	sclm::ReportAndAbort( SCLR_NAME "_BadOrNoValueForEntry", Entry.GetPath( Path ) );
qRR
qRT
qRE
}

void sclr::Erase( eLayer Layer )
{
	Registry_.Erase( GetRawLayer( Layer ) );
}

void sclr::Reset( eLayer Layer )
{
	Registry_.Erase( GetRawLayer( Layer ) );

	Registry_.Set( GetRawLayer( Layer ) );
}

void sclr::Reset(
	eLayer Layer,
	const rgstry::entry__ &Entry )
{
	Registry_.Erase( GetRawLayer( Layer ) );

	Registry_.Set( GetRawLayer( Layer ), Entry );
}

namespace {
	void ReportErrorAndAbort_(
		const str::dString &Source,
		const bso::sChar *Label,
		const rgstry::context___ &Context )
	{
	qRH
		lcl::meaning Meaning;
		lcl::meaning MeaningBuffer;
	qRB
		Meaning.Init();
		Meaning.SetValue( Label );

		Meaning.AddTag( Source );

		MeaningBuffer.Init();
		rgstry::GetMeaning( Context, MeaningBuffer );

		Meaning.AddTag( MeaningBuffer );

		sclm::ReportAndAbort( Meaning );
	qRR
	qRT
	qRE
	}

	void ReportErrorAndAbort_(
		const str::dString &Source,
		const rgstry::context___ &Context )
	{
		ReportErrorAndAbort_( Source, ( Context.Status == rgstry::sUnableToOpenFile ? SCLR_NAME "_UnableToOpenFile" : SCLR_NAME "_ParsingError" ), Context );
	}

	void ReportErrorAndAbort_(
		xtf::extended_text_iflow__ &,
		eLayer Layer,
		const rgstry::context___ &Context )
	{
		ReportErrorAndAbort_( str::wString( GetLabel( Layer ) ), Context );
	}

	void ReportErrorAndAbort_(
		const fnm::rName &Filename,
		eLayer,
		const rgstry::context___ &Context )
	{
	qRH
		str::wString Buffer;
	qRB
		Buffer.Init();
		ReportErrorAndAbort_( Filename.UTF8( Buffer ), Context );
	qRR
	qRE
	qRT
	}

	template <typename source> static void Load_(
		eLayer Layer,
		source &Source,
		const char *RootPath,
		const fnm::name___ &Directory )
	{
	qRH
		rgstry::context___ Context;
	qRB
		Erase( Layer );

		Context.Init();
		if ( !Registry_.Fill( Layer, Source, xpp::criterions___( Directory ), RootPath, Context ) )
			ReportErrorAndAbort_( Source, Layer, Context );
	qRR
	qRT
	qRE
	}
}

void sclr::Load(
	eLayer Layer,
	xtf::extended_text_iflow__ &Flow,
	const fnm::name___ &Directory,
	const char *RootPath )
{
	Load_(Layer, Flow, RootPath, Directory );
}

void sclr::Load(
	eLayer Layer,
	flw::rRFlow &Flow,
	const fnm::name___ &Directory,
	const char *RootPath )
{
qRH
	xtf::extended_text_iflow__ XFlow;
qRB
	XFlow.Init( Flow, utf::f_Default );

	Load_(Layer, XFlow, RootPath, Directory );
qRR
qRT
qRE
}

namespace {
	template <typename source> static void LoadProject_(
		source &Source,
		const char *Target,
		const fnm::rName &Dir,
		str::string_ &Id )
	{
	qRH
		str::string Path;
		TOL_CBUFFER___ Buffer;
	qRB
		Path.Init();

		BuildRootPath( "Project", Target, Path );

		Load_( lProject, Source, Path.Convert( Buffer ), Dir );

		Registry_.GetValue( ProjectLayer_, rgstry::entry___( "@Id" ), Id );
	qRR
	qRT
	qRE
	}
}

void sclr::LoadProject(
	flw::iflow__ &Flow,
	const char *Target,
	const fnm::name___ &Directory,
	str::string_ &Id )
{
qRH
	xtf::extended_text_iflow__ XFlow;
qRB
	XFlow.Init( Flow, utf::f_Default );

	LoadProject_( XFlow, Target, Directory, Id );
qRR
qRT
qRE
}

void sclr::LoadProject(
	const fnm::name___ &Filename,
	const char *Target,
	str::string_ &Id )
{
	LoadProject_( Filename, Target, str::wString( "" ), Id );
}

static const str::string_ &GetSelectedSetupContent_(
	const str::string_ &SetupId,
	str::string_ &Content )
{
qRH
	flx::E_STRING_OFLOW___ OFlow;
	txf::text_oflow__ TFlow;
	rgstry::row__ Row = qNIL;
	rgstry::layer__ Layer = rgstry::UndefinedLayer;
qRB
	if ( SetupId.Amount() != 0  ) {
		OFlow.Init( Content );
		TFlow.Init( OFlow );

		Row = Registry_.Search(rgstry::rTEntry(setup_::Tagged, SetupId), Layer);

		if ( Row != qNIL )
			Registry_.Dump( Layer, Row, false, xml::oCompact, xml::e_None, TFlow );
	}
qRR
qRT
qRE
	return Content;
}

void sclr::FillWithSetupOfId(
	registry_ &Registry,
	rgstry::layer__ Layer,
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

	Setup.Append("<_><" );
	sclr::Parameters.GetPath( Setup );
	Setup.Append('>' );

	GetSelectedSetupContent_( Id, Setup );

	Setup.Append( "</" );
	sclr::Parameters.GetPath( Setup );
	Setup.Append( "></_>" );

	Registry.Erase( Layer );

	IFlow.Init( Setup );
	XFlow.Init( IFlow, utf::f_Default );
	Registry.Fill( Layer, XFlow, xpp::criterions___( fnm::name___() ), NULL );
qRR
qRT
qRE
}

bso::sBool sclr::GetDefaultSetupId(
	const dRegistry &Registry,
	str::dString &Id)
{
	return OGetValue(Registry, parameter::DefaultSetupId, Id) || OGetValue(Registry, setup_::Default, Id);
}

void sclr::FillWithGivenSetup(
	registry_ &Registry,
	rgstry::layer__ Layer )
{
qRH
	str::string Id;
qRB
	Id.Init();

	if ( GetDefaultSetupId(Registry, Id) )
		FillWithSetupOfId( Registry, Layer, Id );
qRR
qRT
qRE
}

void sclr::GetSetupIds(
	const dRegistry &Registry,
	str::dStrings &Ids)
{
		GetValues(Registry, setup_::Id, Ids);
}

void sclr::GetSetupAliases(
	const dRegistry &Registry,
	const str::dStrings &Ids,
	str::dStrings &Aliases)
{
qRH;
	str::wStrings Ids;
	sdr::sRow Row = qNIL;
	str::wString Alias;
qRB;
	Ids.Init();

	GetSetupIds(Registry, Ids);

	Row = Ids.First();

	while ( Row != qNIL ) {
		Alias.Init();
		OGetValue(Registry, rgstry::rTEntry(setup_::Alias, Ids(Row)), Alias);
		Aliases.Append(Alias);

		Row = Ids.Next(Row);
	}
qRR;
qRT;
qRE;
}

void sclr::ReportIfNoParametersSetupId( void )
{
	if ( !Registry_.Exists(parameter::DefaultSetupId) )
		ReportBadOrNoValueForEntryErrorAndAbort(parameter::DefaultSetupId);
}

void sclr::FillWithContent(
	registry_ &Registry,
	rgstry::layer__ Layer,
	const str::dString &BinPath,
	const str::string_ &RawContent )
{
qRH
	str::string Content;
	flx::E_STRING_IFLOW__ IFlow;
	xtf::extended_text_iflow__ XFlow;
	str::string EntryPath;
qRB
	Content.Init();

	Content.Append("<_>" );
	Content.Append( RawContent );
	Content.Append( "</_>" );

	Registry.Erase( Layer );

	IFlow.Init( Content );
	XFlow.Init( IFlow, utf::f_Default );
	Registry.Fill( Layer, XFlow, xpp::criterions___( BinPath ), NULL );
qRR
qRT
qRE
}

void sclr::AddValue(
	registry_ &Registry,
	const str::string_ &Value,
	const rgstry::tentry__ &Entry )
{
	Registry.AddValue( Entry, Value );
}

void sclr::AddValue(
	registry_ &Registry,
	const str::string_ &Path,
	const str::string_ &Value,
	sdr::row__ *Error )
{
	Registry.AddValue( Path, Value, Error );
}

void sclr::SetValue(
	registry_ &Registry,
	const str::string_ &Value,
	const rgstry::tentry__ &Entry )
{
	Registry.SetValue( Entry, Value );
}

void sclr::SetValue(
	registry_ &Registry,
	const str::string_ &Path,
	const str::string_ &Value,
	sdr::row__ *Error )
{
	Registry.SetValue( Path, Value, Error );
}

bso::bool__ sclr::GetValues(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	str::dStrings &Values )
{
	return Registry.GetValues( Entry, Values );
}

bso:: bool__ sclr::OGetValue(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	return Registry.GetValue( Entry, Value );
}

const char *sclr::OGetValue(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	TOL_CBUFFER___ &Buffer )
{
	const char *ReturnedValue = NULL;
qRH
	str::string Value;
qRB
	Value.Init();

	if ( OGetValue( Registry, Entry, Value ) )
		ReturnedValue = Value.Convert( Buffer );
qRR
qRT
qRE
	return ReturnedValue;
}

const str::string_ &sclr::MGetValue(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	if ( !BGetValue( Registry, Entry, Value, qRPU ) )
		sclr::ReportBadOrNoValueForEntryErrorAndAbort( Entry );

	return Value;
}

const char *sclr::MGetValue(
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

bso::sBool sclr::OGetChar(
  const registry_ &Registry,
  const rgstry::tentry__ &Entry,
  bso::sChar &Char)
{
  bso::sBool Found = false;
qRH;
  str::wString Value;
qRB;
  Value.Init();

  // '( (…) ) ' to avoid warning.
  if ( ( Found = OGetValue(Registry, Entry, Value) ) ) {
    if ( Value.Amount() != 1 )
      ReportBadOrNoValueForEntryErrorAndAbort(Entry);
    else
      Char = Value(Value.First());
  }

qRR;
qRT;
qRE;
  return Found;
}

static tol::extended_boolean__ GetBoolean_( const str::string_ &Value )
{
	tol::xbool__ Boolean = tol::xb_Undefined;
qRH
	str::string Buffer;
qRB
	Buffer.Init( Value );
	Buffer.StripChars(' ');
	str::ToLower( Buffer );

	if ( ( Buffer == "true" ) || ( Buffer == "yes" ) || ( Buffer == "1" ) )
		Boolean = tol::xbTrue;
	else if ( ( Buffer == "false" ) || ( Buffer == "no" ) || ( Buffer == "0" ) )
		Boolean = tol::xbFalse;
qRR
qRT
qRE
	return Boolean;
}

bso::bool__ sclr::BGetBoolean(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	bso::sBool &Result,
	bso::bool__ DefaultValue,
	qRPN )
{
	bso::sBool Return = false;
qRH
	str::string Value;
qRB
	Value.Init();

	if ( ( Return = BGetValue( Registry, Entry, Value, qRP ) ) ) {
		switch ( GetBoolean_( Value ) ) {
		case tol::xbFalse:
			Result = false;
			break;
		case tol::xbTrue:
			Result = true;
			break;
		case tol::xb_Undefined:
			// We don't care about the 'qRP' parameter, because the entry have to be corrected!
			sclr::ReportBadOrNoValueForEntryErrorAndAbort( Entry );
			break;
		default:
			qRFwk();
			break;
		}
	}
qRR
qRT
qRE
	return Return;
}

bso::bool__ sclr::MGetBoolean(
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
		sclr::ReportBadOrNoValueForEntryErrorAndAbort( Entry );
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
	t &Value,
	qRPN )
{
	bso::bool__ Present = false;
qRH
	str::string RawValue;
	sdr::row__ Error = qNIL;
qRB
	RawValue.Init();

	if ( ( Present = BGetValue(Registry, Entry, RawValue, qRP) ) ) {
 		RawValue.ToNumber( Value, str::sULimit<t>(Limit), &Error );

		if ( Error != qNIL )
			sclr::ReportBadOrNoValueForEntryErrorAndAbort( Entry );
	}
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
	t &Value,
	qRPN )
{
	bso::bool__ Present = false;
qRH
	str::string RawValue;
	sdr::row__ Error = qNIL;
qRB
	RawValue.Init();

	if ( ( Present = BGetValue(Registry, Entry, RawValue, qRP) ) ) {
		RawValue.ToNumber( Value, str::sSLimits<t>(UpperLimit, LowerLimit), &Error );

		if ( Error != qNIL )
			sclr::ReportBadOrNoValueForEntryErrorAndAbort( Entry );
	}
qRR
qRT
qRE
	return Present;
}

#define UN( name, type )\
type sclr::MGet##name(\
	const registry_ &Registry,\
	const rgstry::tentry__ &Entry,\
	type Limit  )\
{\
	type Value;\
	\
	if ( !GetUnsignedNumber_(Registry, Entry, Limit, Value, qRPU) )\
		sclr::ReportBadOrNoValueForEntryErrorAndAbort( Entry );\
	\
	return Value;\
}\
type sclr::OGet##name(\
	const registry_ &Registry,\
	const rgstry::tentry__ &Entry,\
	type DefaultValue,\
	type Limit )\
{\
	type Value;\
	\
	if ( !GetUnsignedNumber_(Registry, Entry, Limit, Value, qRPU) )\
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
type sclr::MGet##name(\
	const registry_ &Registry,\
	const rgstry::tentry__ &Entry,\
	type Min,\
	type Max)\
{\
	type Value;\
	\
	if ( !GetSignedNumber_( Registry, Entry, Min, Max, Value, qRPU ) )\
		sclr::ReportBadOrNoValueForEntryErrorAndAbort( Entry );\
	\
	return Value;\
}\
type sclr::OGet##name(\
	const registry_ &Registry,\
	const rgstry::tentry__ &Entry,\
	type DefaultValue,\
	type Min,\
	type Max )\
{\
	type Value;\
	\
	if ( !GetSignedNumber_( Registry, Entry, Min, Max, Value, qRPU ) )\
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

Q37_GCTOR( sclr )
{
	Registry_.Init();
	GlobalLocker_.Init();
	LastingRegistryLocker_.Init();
	LastingRegistryLockerCallback_.Init();

	// 3 firsts not as 'embedded', due to the fact that plugins use the registry of the main program.
	MainLayer_ = Registry_.Create();
	LastingLayer_ = Registry_.CreateEmbedded( LastingRegistryLockerCallback_ );
	ProjectLayer_ = Registry_.Create();
	SetupLayer_ = Registry_.Create();
	ArgumentsLayer_ = Registry_.CreateEmbedded();
	RuntimeLayer_ = Registry_.CreateEmbedded();
}
