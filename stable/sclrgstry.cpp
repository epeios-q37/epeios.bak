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

#include "sclerror.h"
#include "sclmisc.h"

using namespace sclrgstry;

#define REGISTRY_FILE_EXTENSION ".xcfg"

static rgstry::multi_level_registry Registry_;

static rgstry::level__ ConfigurationLevel_ = RGSTRY_UNDEFINED_LEVEL;
static rgstry::level__ ProjectLevel_ = RGSTRY_UNDEFINED_LEVEL;

rgstry::entry___ sclrgstry::Parameters( "Parameters" );

rgstry::entry___ sclrgstry::Definitions( "Definitions" );

rgstry::entry___ sclrgstry::Language( "Language", Parameters );

rgstry::entry___ sclrgstry::Locale( "Locale", Definitions );

rgstry::multi_level_registry_ &sclrgstry::GetRegistry( void )
{
	return Registry_;
}

rgstry::level__ sclrgstry::GetConfigurationLevel( void )
{
	return ConfigurationLevel_;
}

rgstry::level__ sclrgstry::GetProjectLevel( void )
{
	return ProjectLevel_;
}

static rgstry::status__ FillConfigurationRegistry_(
	xtf::extended_text_iflow__ &Flow,
	const char *Directory,
	const char *RootPath,
	rgstry::context___ &Context )
{
	Registry_.Init();
	ConfigurationLevel_ = Registry_.PushEmbeddedLevel( rgstry::name( "Configuration" ) );

	if ( ConfigurationLevel_ == RGSTRY_UNDEFINED_LEVEL )
		ERRFwk();

	return  Registry_.Fill( ConfigurationLevel_, Flow, xpp::criterions___( str::string( Directory ) ), RootPath );
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
	Context.Init();

	if ( FillConfigurationRegistry_( Flow, Directory, RootPath, Context ) != rgstry::sOK )
		ReportFileParsingErrorAndAbort_( SCLRGSTRY_NAME "_ConfigurationFileParsingError", Context );
ERRErr
ERREnd
ERREpilog
}

#define PROJECT_ROOT_PATH	"Projects/Project[@target=\"%1\"]"

void sclrgstry::LoadProject(
	const fnm::name___ &FileName,
	const char *Target )
{
ERRProlog
	str::string Path;
	TOL_CBUFFER___ Buffer;
	rgstry::context___ Context;
ERRBegin
	Path.Init( PROJECT_ROOT_PATH );
	str::ReplaceShortTag( Path, 1, str::string( Target ), '%' );

	Context.Init();

	if ( Registry_.Fill( ProjectLevel_, FileName, xpp::criterions___(), Path.Convert( Buffer ), Context ) != rgstry::sOK )
		ReportFileParsingErrorAndAbort_( SCLRGSTRY_NAME "_ConfigurationFileParsingError", Context );
ERRErr
ERREnd
ERREpilog
}

bso::bool__ sclrgstry::GetValue(
	const rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	return Registry_.GetValue( Entry, Value );
}

void sclrgstry::SetValue(
	const str::string_ &Value,
	const rgstry::tentry__ &Entry )
{
	Registry_.SetValue( Entry, Value );
}

void sclrgstry::SetValue(
	const str::string_ &Path,
	const str::string_ &Value,
	sdr::row__ *Error )
{
	Registry_.SetValue( Path, Value, Error );
}

bso::bool__ sclrgstry::GetValues(
	const rgstry::tentry__ &Entry,
	str::strings_ &Values )
{
	return Registry_.GetValues( Entry, Values );
}

const str::string_ &sclrgstry::GetOptionalValue(
	const rgstry::tentry__ &Entry,
	str::string_ &Value,
	bso::bool__ *Missing )
{
	if ( !GetValue( Entry, Value ) )
		if ( Missing != NULL )
			*Missing = true;

	return Value;
}

const char *sclrgstry::GetOptionalValue(
	const rgstry::tentry__ &Entry,
	TOL_CBUFFER___ &Buffer,
	bso::bool__ *Missing )
{
	ERRProlog
		str::string Value;
	bso::bool__ LocalMissing = false;
	ERRBegin
		Value.Init();

	GetOptionalValue( Entry, Value, &LocalMissing );

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

const str::string_ &sclrgstry::GetMandatoryValue(
	const rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	if ( !GetValue( Entry, Value ) )
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( Entry );

	return Value;
}

const char *sclrgstry::GetMandatoryValue(
	const rgstry::tentry__ &Entry,
	TOL_CBUFFER___ &Buffer )
{
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	GetMandatoryValue( Entry, Value );

	Value.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

static bso::xbool__ GetBoolean_( const str::string_ &Value )
{
	bso::xbool__ Boolean = bso::xb_Undefined;
ERRProlog
	str::string Buffer;
ERRBegin
	Buffer.Init( Value );
	str::ToLower( Buffer );

	if ( ( Buffer == "true" ) || ( Buffer == "yes" ) )
		Boolean = bso::xbTrue;
	else if ( ( Buffer == "false" ) || ( Buffer == "no" ) )
		Boolean = bso::xbFalse;
ERRErr
ERREnd
ERREpilog
	return Boolean;
}

bso::bool__ sclrgstry::GetBoolean(
	const rgstry::tentry__ &Entry,
	bso::bool__ DefaultValue )
{
	bso::bool__ &Result = DefaultValue;
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	if ( GetValue( Entry, Value ) ) {
		switch ( GetBoolean_( Value ) ) {
		case bso::xbFalse:
			Result = false;
			break;
		case bso::xbTrue:
			Result = true;
			break;
		case bso::xb_Undefined:
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

bso::bool__ sclrgstry::GetMandatoryBoolean( const rgstry::tentry___ &Entry )
{
	bso::bool__ Result = false;
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	switch ( GetBoolean_( GetMandatoryValue( Entry, Value ) ) ) {
	case bso::xbFalse:
		Result = false;
		break;
	case bso::xbTrue:
		Result = true;
		break;
	case bso::xb_Undefined:
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

	if ( !( Present = GetValue( Entry, RawValue ) ) )
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

	if ( !( Present = GetValue( Entry, RawValue ) ) )
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
	type sclrgstry::GetMandatory##name(\
	const rgstry::tentry__ &Entry,\
	type Limit  )\
{\
	type Value;\
	\
	if ( !GetUnsignedNumber_( Entry, Limit, Value ) )\
	sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( Entry );\
	\
	return Value;\
}\
	type sclrgstry::Get##name(\
	const rgstry::tentry__ &Entry,\
	type DefaultValue,\
	type Limit )\
{\
	type Value;\
	\
	if ( !GetUnsignedNumber_( Entry, Limit, Value ) )\
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
	type sclrgstry::GetMandatory##name(\
	const rgstry::tentry__ &Entry,\
	type Min,\
	type Max)\
{\
	type Value;\
	\
	if ( !GetSignedNumber_( Entry, Min, Max, Value ) )\
	sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( Entry );\
	\
	return Value;\
}\
	type sclrgstry::Get##name(\
	const rgstry::tentry__ &Entry,\
	type DefaultValue,\
	type Min,\
	type Max )\
{\
	type Value;\
	\
	if ( !GetSignedNumber_( Entry, Min, Max, Value ) )\
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
