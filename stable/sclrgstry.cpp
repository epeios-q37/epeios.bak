/*
	'sclrgstry.cpp' by Claude SIMON (http://zeusw.org/).

	'sclrgstry' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#define SCLRGSTRY__COMPILATION

#include "sclrgstry.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "cio.h"
#include "lcl.h"
#include "dir.h"
#include "fnm.h"
#include "tagsbs.h"

#include "sclerror.h"
#include "sclmisc.h"

using namespace sclrgstry;

#define REGISTRY_FILE_EXTENSION ".xcfg"

static rgstry::multi_level_registry BaseRegistry_;

static rgstry::level__ ConfigurationRegistryLevel_ = RGSTRY_UNDEFINED_LEVEL;
static rgstry::level__ ProjectRegistryLevel_ = RGSTRY_UNDEFINED_LEVEL;

rgstry::entry___ sclrgstry::Parameters( ParametersTag );

rgstry::entry___ sclrgstry::Definitions( "Definitions" );

rgstry::entry___ sclrgstry::Language( "Language", Parameters );

rgstry::entry___ sclrgstry::Locale( "Locale", Definitions );

static rgstry::entry___ DefaultSetup_( "@DefaultSetup", Parameters );

registry_ &sclrgstry::GetRegistry( void )
{
	return BaseRegistry_;
}

rgstry::level__ sclrgstry::GetConfigurationRegistryLevel( void )
{
	return ConfigurationRegistryLevel_;
}

rgstry::level__ sclrgstry::GetProjectRegistryLevel( void )
{
	return ProjectRegistryLevel_;
}

static rgstry::status__ FillConfigurationRegistry_(
	xtf::extended_text_iflow__ &Flow,
	const char *Directory,
	const char *RootPath,
	rgstry::context___ &Context )
{
	BaseRegistry_.Erase( ConfigurationRegistryLevel_ );

	return BaseRegistry_.Fill( ConfigurationRegistryLevel_, Flow, xpp::criterions___( str::string( Directory ) ), RootPath );
}


void sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( const rgstry::tentry__ &Entry )
{
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();
	sclmisc::ReportAndAbort( SCLRGSTRY_NAME "_BadOrNoValueForEntry", Entry.GetPath( Path ) );
ERRErr
ERREnd
ERREpilog
}

static inline void ReportFileParsingErrorAndAbort_(
	const char *ErrorLabel,
	const rgstry::context___ &Context )
{
ERRProlog
	lcl::meaning Meaning;
	lcl::meaning MeaningBuffer;
ERRBegin
	Meaning.Init();
	Meaning.SetValue( ErrorLabel );

	MeaningBuffer.Init();
	rgstry::GetMeaning( Context, MeaningBuffer );

	Meaning.AddTag( MeaningBuffer );

	sclmisc::ReportAndAbort( Meaning );
ERRErr
ERREnd
ERREpilog
}

void sclrgstry::LoadConfiguration(
	xtf::extended_text_iflow__&Flow,
	const char *Directory,
	const char *RootPath )
{
ERRProlog
	rgstry::context___ Context;
ERRBegin
	BaseRegistry_.Erase( ConfigurationRegistryLevel_ );

	Context.Init();
	if ( FillConfigurationRegistry_( Flow, Directory, RootPath, Context ) != rgstry::sOK )
		ReportFileParsingErrorAndAbort_( SCLRGSTRY_NAME "_ConfigurationFileParsingError", Context );
ERRErr
ERREnd
ERREpilog
}

void sclrgstry::EraseProjectRegistry( void )
{
	BaseRegistry_.Erase( ProjectRegistryLevel_ );
}


#define PROJECT_ROOT_PATH	"Projects/Project[@target=\"%1\"]"

void sclrgstry::LoadProject(
	flw::iflow__ &Flow,
	const char *Target,
	str::string_ &Id )
{
ERRProlog
	str::string Path;
	TOL_CBUFFER___ Buffer;
	rgstry::context___ Context;
	bso::bool__ Missing = false;
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	Path.Init( PROJECT_ROOT_PATH );
	tagsbs::SubstituteShortTag( Path, 1, str::string( Target ), '%' );

	EraseProjectRegistry();

	XFlow.Init( Flow, utf::f_Guess );
	Context.Init();
	if ( BaseRegistry_.Fill( ProjectRegistryLevel_, XFlow, xpp::criterions___(), Path.Convert( Buffer ), Context ) != rgstry::sOK )
		ReportFileParsingErrorAndAbort_( SCLRGSTRY_NAME "_ProjectFileParsingError", Context );

	BaseRegistry_.GetValue( ProjectRegistryLevel_, rgstry::entry___( "@Id" ), Id );
ERRErr
ERREnd
ERREpilog
}



void sclrgstry::LoadProject(
	const fnm::name___ &FileName,
	const char *Target,
	str::string_ &Id )
{
ERRProlog
	flf::file_iflow___ Flow;
ERRBegin
	if ( !Flow.Init( FileName, err::hUserDefined ) )
		sclmisc::ReportFileOpeningErrorAndAbort( FileName );

	LoadProject( Flow, Target, Id );
ERRErr
ERREnd
ERREpilog
}

static const str::string_ &GetSelectedSetupContent_(
	const str::string_ &SetupId,
	str::string_ &Content )
{
ERRProlog
	flx::E_STRING_OFLOW___ OFlow;
	txf::text_oflow__ TFlow;
	rgstry::row__ Row = E_NIL;
	rgstry::level__ Level = rgstry::UndefinedLevel;
	str::string SetupPath;
ERRBegin
	if ( SetupId.Amount() == 0  )
		ERRReturn;

	SetupPath.Init( "Setups/Setup[id=\"" );
	SetupPath.Append( SetupId );
	SetupPath.Append( "\"]" );

	OFlow.Init( Content );
	TFlow.Init( OFlow );

	Row = sclrgstry::GetRegistry().Search( SetupPath, Level );

	if ( Row != E_NIL )
		sclrgstry::GetRegistry().Dump( Level, Row, false, xml::oCompact, xml::e_None, TFlow );
ERRErr
ERREnd
ERREpilog
	return Content;
}

void sclrgstry::FillRegistryWithSetup(
	registry_ &Registry,
	level__ Level,
	const str::string_ &RawSetupId )
{
ERRProlog
	str::string Setup;
	flx::E_STRING_IFLOW__ IFlow;
	xtf::extended_text_iflow__ XFlow;
	str::string EntryPath, SetupId;
ERRBegin
	SetupId.Init( RawSetupId );

	if ( SetupId.Amount() == 0 )
		Registry.GetValue( DefaultSetup_, SetupId );

	Setup.Init();

	Setup.Append('<' );
	sclrgstry::Parameters.GetPath( Setup );
	Setup.Append('>' );

	GetSelectedSetupContent_( SetupId, Setup );

	Setup.Append( "</" );
	sclrgstry::Parameters.GetPath( Setup );
	Setup.Append('>' );


	IFlow.Init( Setup );
	XFlow.Init( IFlow, utf::f_Default );
	Registry.Fill( Level, XFlow, xpp::criterions___(), NULL );
ERRErr
ERREnd
ERREpilog
}

bso::bool__ sclrgstry::BGetValue(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	return Registry.GetValue( Entry, Value );
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

const str::string_ &sclrgstry::OGetValue(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	str::string_ &Value,
	bso::bool__ *Missing )
{
	if ( !BGetValue( Registry, Entry, Value ) )
		if ( Missing != NULL )
			*Missing = true;

	return Value;
}

const char *sclrgstry::OGetValue(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	TOL_CBUFFER___ &Buffer,
	bso::bool__ *Missing )
{
ERRProlog
	str::string Value;
	bso::bool__ LocalMissing = false;
ERRBegin
	Value.Init();

	OGetValue( Registry, Entry, Value, &LocalMissing );

	if ( LocalMissing ) {
		if ( Missing != NULL )
			*Missing = true;
	}
	else
		Value.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
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
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	MGetValue( Registry, Entry, Value );

	Value.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

static tol::extended_boolean__ GetBoolean_( const str::string_ &Value )
{
	tol::xbool__ Boolean = tol::xb_Undefined;
ERRProlog
	str::string Buffer;
ERRBegin
	Buffer.Init( Value );
	str::ToLower( Buffer );

	if ( ( Buffer == "true" ) || ( Buffer == "yes" ) )
		Boolean = tol::xbTrue;
	else if ( ( Buffer == "false" ) || ( Buffer == "no" ) )
		Boolean = tol::xbFalse;
ERRErr
ERREnd
ERREpilog
	return Boolean;
}

bso::bool__ sclrgstry::BGetBoolean(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	bso::bool__ DefaultValue )
{
	bso::bool__ &Result = DefaultValue;
ERRProlog
	str::string Value;
ERRBegin
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
			ERRFwk();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
	return Result;
}

bso::bool__ sclrgstry::MGetBoolean(
	const registry_ &Registry,
	const rgstry::tentry___ &Entry )
{
	bso::bool__ Result = false;
ERRProlog
	str::string Value;
ERRBegin
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
		ERRFwk();
		break;
	}
ERRErr
ERREnd
ERREpilog
	return Result;
}


template <typename t> static bso::bool__ GetUnsignedNumber_(
	const registry_ &Registry,
	const rgstry::tentry__ &Entry,
	t Limit,
	t &Value )
{
	bso::bool__ Present = false;
	ERRProlog
		str::string RawValue;
	sdr::row__ Error = E_NIL;
	ERRBegin
		RawValue.Init();

	if ( !( Present = BGetValue( Registry, Entry, RawValue ) ) )
		ERRReturn;

	RawValue.ToNumber( Limit, Value, &Error );

	if ( Error != E_NIL )
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( Entry );
	ERRErr
		ERREnd
		ERREpilog
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
	ERRProlog
		str::string RawValue;
	sdr::row__ Error = E_NIL;
	ERRBegin
		RawValue.Init();

	if ( !( Present = BGetValue( Registry, Entry, RawValue ) ) )
		ERRReturn;

	RawValue.ToNumber( UpperLimit, LowerLimit, Value, &Error );

	if ( Error != E_NIL )
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( Entry );
	ERRErr
		ERREnd
		ERREpilog
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

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class sclrgstrypersonnalization
{
public:
	sclrgstrypersonnalization( void )
	{
		BaseRegistry_.Init();

		ConfigurationRegistryLevel_ = BaseRegistry_.PushEmbeddedLevel( rgstry::name( "Configuration" ) );
		ProjectRegistryLevel_ = BaseRegistry_.PushEmbeddedLevel( rgstry::name( "Project" ) );

		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sclrgstrypersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static sclrgstrypersonnalization Tutor;
