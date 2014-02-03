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
#include "scllocale.h"

using namespace sclrgstry;

#define REGISTRY_FILE_EXTENSION ".xcfg"

static rgstry::registry ConfigurationRegistry_;
static rgstry::multi_level_registry Registry_;

static rgstry::level__ ConfigurationLevel_ = RGSTRY_UNDEFINED_LEVEL;
static rgstry::level__ ProjectLevel_ = RGSTRY_UNDEFINED_LEVEL;

static rgstry::row__ Root_ = E_NIL;
static TOL_CBUFFER___ Translation_;

rgstry::entry___ sclrgstry::Parameters( "Parameters" );

rgstry::entry___ sclrgstry::Definitions( "Definitions" );

rgstry::entry___ sclrgstry::Language( "Language", Parameters );

rgstry::entry___ sclrgstry::Locale( "Locale", Definitions );

bso::bool__ sclrgstry::IsReady( void )
{
	return Root_ != E_NIL;
}

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
	flw::iflow__ &Flow,
	const char *Directory,
	const char *RootPath,
	rgstry::context___ &Context )
{
	rgstry::status__ Status = rgstry::s_Undefined;
ERRProlog
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	if ( Root_ != E_NIL )
		ERRFwk();

	XFlow.Init( Flow, utf::f_Default );

	Status = rgstry::FillRegistry( XFlow, xpp::criterions___( str::string( Directory ) ), RootPath, ConfigurationRegistry_, Root_, Context );

	if ( Status == rgstry::sOK ) {
		ConfigurationLevel_ = Registry_.PushImportedLevel( ConfigurationRegistry_, Root_ );
		ProjectLevel_ = Registry_.PushEmbeddedLevel( str::string( "Project" ) );
	}
ERRErr
ERREnd
ERREpilog
	return Status;
}


void sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( const rgstry::tentry__ &Entry )
{
ERRProlog
	lcl::meaning Meaning;
	str::string Path;
ERRBegin
	Meaning.Init();
	Meaning.SetValue( SCLRGSTRY_NAME "_BadOrNoValueForEntry" );

	Path.Init();
	Meaning.AddTag( Entry.GetPath( Path ) );

	sclerror::SetMeaning( Meaning );

	ERRAbort();
ERRErr
ERREnd
ERREpilog
}

static inline void ReportConfigurationFileParsingError_( const rgstry::context___ &Context )
{
ERRProlog
	lcl::meaning Meaning;
	lcl::meaning MeaningBuffer;
ERRBegin
	Meaning.Init();
	Meaning.SetValue( SCLRGSTRY_NAME "_ConfigurationFileParsingError" );

	MeaningBuffer.Init();
	rgstry::GetMeaning( Context, MeaningBuffer );

	Meaning.AddTag( MeaningBuffer );

	sclerror::SetMeaning( Meaning );
ERRErr
ERREnd
ERREpilog
}

void sclrgstry::LoadConfiguration(
	flw::iflow__ &Flow,
	const char *Directory,
	const char *RootPath )
{
ERRProlog
	rgstry::context___ Context;
ERRBegin
	Context.Init();

	if ( FillConfigurationRegistry_( Flow, Directory, RootPath, Context ) != rgstry::sOK ) {
		ReportConfigurationFileParsingError_( Context );
		ERRAbort();
	}
ERRErr
ERREnd
ERREpilog
}

static bso::bool__ LoadProjectLocale_(
	const str::string_ &LocaleContent,
	lcl::context___ &Context )
{
	bso::bool__ Success = false;
ERRProlog
	flx::E_STRING_IFLOW__ Flow;
ERRBegin
	Flow.Init( LocaleContent );

	if ( scllocale::Push( Flow, NULL, "Locale", utf::f_Default, Context ) != LCL_UNDEFINED_LEVEL )
		Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}

static tol::report__ LoadProjectLocale_( rgstry::context___ &Context )
{
	tol::report__ Report = tol::r_Undefined;
ERRProlog
	str::string Locale;
ERRBegin
	Locale.Init();

	Registry_.GetValue( ProjectLevel_, Locale, Locale );

	if ( Locale.Amount() != 0 ) {
		if ( !LoadProjectLocale_( Locale, Context ) )
			Report = tol::rFailure;
		else
			Report = tol::rSuccess;
		}
		else
			Report = tol::rSuccess;

ERRErr
ERREnd
ERREpilog
	return Report;
}


#define PROJECT_ROOT_PATH	"Projects/Project[@target=\"%1\"]"

tol::report__ sclrgstry::LoadProject(
	const char *FileName,
	const char *Target,
	rgstry::context___ &Context )
{
	 tol::report__ Report = tol::r_Undefined;
ERRProlog
	str::string Path;
	TOL_CBUFFER___ Buffer;
ERRBegin
	Path.Init( PROJECT_ROOT_PATH );
	str::ReplaceShortTag( Path, 1, str::string( Target ), '%' );

	if ( Registry_.Fill( ProjectLevel_, FileName, xpp::criterions___(), Path.Convert( Buffer ), Context ) == rgstry::sOK )
		Report = tol::rSuccess;
	else
		Report = tol::rFailure;
ERRErr
ERREnd
ERREpilog
	return Report;
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
		ConfigurationRegistry_.Init();
		Registry_.Init();
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
