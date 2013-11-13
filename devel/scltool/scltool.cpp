/*
	'scltool.cpp' by Claude SIMON (http://zeusw.org/).

	'scltool' is part of the Epeios framework.

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

#define SCLTOOL__COMPILATION

#include "scltool.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "cio.h"

#include "sclrgstry.h"
#include "scllocale.h"
#include "sclmisc.h"
#include "sclerror.h"

using namespace scltool;

using cio::COut;
using scllocale::GetLocale;

static STR_BUFFER___ Language_;

#define DEFAULT_LANGUAGE	"en"

static rgstry::multi_level_registry Registry_;

static rgstry::level__ RegistryProjectLevel_ = RGSTRY_UNDEFINED_LEVEL;
static rgstry::level__ ArgumentsProjectLevel_ = RGSTRY_UNDEFINED_LEVEL;

const char *scltool::GetLanguage( void )
{
	if ( Language_ == NULL )
		return DEFAULT_LANGUAGE;

	return Language_;
}

void scltool::AddDefaultCommands( clnarg::description_ &Description )
{
	Description.AddCommand( CLNARG_NO_SHORT, "version", cVersion );
	Description.AddCommand( CLNARG_NO_SHORT, "help", cHelp );
	Description.AddCommand( CLNARG_NO_SHORT, "license", cLicense );
}


void scltool::PrintDefaultCommandDescriptions(
	const char *ProgramName,
	const clnarg::description_ &Description )
{
ERRProlog
	CLNARG_BUFFER__ Buffer;
	lcl::meaning Meaning;
	str::string Translation;
ERRBegin
	Translation.Init();
	COut << scllocale::GetLocale().GetTranslation( "ProgramDescription", GetLanguage(), Translation ) << txf::nl;
	COut << txf::nl;

	COut << ProgramName << ' ' << Description.GetCommandLabels( cVersion, Buffer ) << txf::nl;
	Meaning.Init();
	clnarg::GetVersionCommandDescription( Meaning );
	Translation.Init();
	GetLocale().GetTranslation( Meaning, GetLanguage(), Translation );
	COut << txf::pad << Translation << '.' << txf::nl;

	COut << ProgramName << ' ' << Description.GetCommandLabels( cLicense, Buffer ) << txf::nl;
	Meaning.Init();
	clnarg::GetLicenseCommandDescription( Meaning );
	Translation.Init();
	GetLocale().GetTranslation( Meaning, GetLanguage(), Translation );
	COut << txf::pad << Translation << '.' << txf::nl;

	COut << ProgramName << ' ' << Description.GetCommandLabels( cHelp, Buffer ) << txf::nl;
	Meaning.Init();
	clnarg::GetHelpCommandDescription( Meaning );
	Translation.Init();
	GetLocale().GetTranslation( Meaning, GetLanguage(), Translation );
	COut << txf::pad << Translation << '.' << txf::nl;

ERRErr
ERREnd
ERREpilog
}

void scltool::ReportAndAbort( const char *Text )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();

	Meaning.SetValue( Text );

	ReportAndAbort( Meaning );
ERRErr
ERREnd
ERREpilog
}

void scltool::ReportUnknownOptionErrorAndAbort( const char *Option )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();

	clnarg::GetUnknownOptionErrorMeaning( Option, Meaning );

	ReportAndAbort( Meaning );
ERRErr
ERREnd
ERREpilog
}

void scltool::ReportWrongNumberOfArgumentsErrorAndAbort( void )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();

	clnarg::GetWrongNumberOfArgumentsErrorMeaning( Meaning );

	ReportAndAbort( Meaning );
ERRErr
ERREnd
ERREpilog
}

void scltool::ReportMissingCommandErrorAndAbort( void )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();

	clnarg::GetMissingCommandErrorMeaning( Meaning );

	ReportAndAbort( Meaning );
ERRErr
ERREnd
ERREpilog
}

enum type__ {
	tShort,	// '-...'
	tLong,	// '--...'
	t_amount,
	t_Undefined
};

class flag_
{
public:
	struct s {
		type__ Type;
		str::string_::s Name;
	} &S_;
	str::string_ Name;
	flag_( s &S )
	: S_( S ),
	  Name( S.Name )
	{}
	void reset( bso::bool__ P = true )
	{
		S_.Type = t_Undefined;
		Name.reset( P );
	}
	void plug( sdr::E_SDRIVER__ &SD )
	{
		Name.plug( SD );
	}
	void plug( ags::E_ASTORAGE_ &AS )
	{
		Name.plug( AS );
	}
	flag_ &operator =( const flag_ &F )
	{
		S_.Type = F.S_.Type;
		Name = F.Name;

		return *this;
	}
	void Init( type__ Type = t_Undefined )
	{
		S_.Type = Type;
		Name.Init();
	}
	void Init(
		type__ Type,
		const str::string_ &Name )
	{
		Init( Type );

		this->Name = Name;
	}
};

E_AUTO( flag );

typedef ctn::E_MCONTAINER_( flag_ ) flags_;
E_AUTO( flags );

class option_
: public flag_
{
public:
	struct s 
	: public flag_::s
	{
		str::string_::s Value;
	};
	str::string_ Value;
	option_( s &S )
	: flag_( S ),
		Value( S.Value )
	{}
	void reset( bso::bool__ P = true )
	{
		flag_::reset( P );

		Value.reset( P );
	}
	void plug( ags::E_ASTORAGE_ &AS )
	{
		flag_::plug( AS );

		Value.plug( AS );
	}
	option_ &operator =( const option_ &O )
	{
		flag_::operator=( O );

		Value = O.Value;

		return *this;
	}
	void Init( void )
	{
		flag_::Init();

		Value.Init();
	}
	void Init(
		type__ Type,
		const str::string_ &Name,
		const char *Value )
	{
		Init();

		flag_::Init( Type, Name );

		this->Value = Value;
	}
};

E_AUTO( option );

typedef ctn::E_MCONTAINER_( option_ ) options_;
E_AUTO( options );


typedef str::string_ argument_;
E_AUTO( argument );

typedef ctn::E_MCONTAINER_( argument_ ) arguments_;
E_AUTO( arguments );

void FillWithShort_( 
	const char *Arg,
	flags_ &Flags,
	options_ &Options,
	arguments_ &Arguments )
{
	const char *Egal = strchr( Arg, '=' );
	const char *Quote = strchr( Arg, '"' );

	if ( Egal == NULL )
		Fill_( Arg, Flags );

	if ( Egal < Quote )
		Fill_( Arg, Options );
}

bso::bool__ Fill_(
	const char *Arg,
	flags_ &Flags,
	options_ &Options,
	arguments_ &Arguments )
{
	size_t Size = strlen( Arg );

	switch ( Size ) {
	case 0:
		ERRDta();
		break;
	case 1:
		Arguments.Append( Arg );
		break;
	case 2:
		if ( ( Arg[0] == '-' ) && ( Arg[1] == '-' ) )
			return true;

		FillWithShort_( Arg + 1, Options, Flags ); );

		break;
	default:
		if ( Arg[0] == '-' )
			if ( Arg[1] == '-' )
				FillWithLong_( Arg + 2, Options, Flags );
			else
				FillWithShort_( Arg + 1, Options, Flags );
		else
			Arguments.Append( Arg );
		break;
	}

	return false;
}


void Fill_(
	int argc,
	const char **argv,
	flags_ &Flags,
	options_ &Options,
	arguments_ &Arguments )
{
	int Current = 1;
	bso::bool__ FreeArgumentsOnly = false;

	while ( ( Current < argc ) && ( !FreeArgumentsOnly ) )
	{
		FreeArgumentsOnly = Fill_( argv[Current++] );
	}
}


static bso::bool__ ReportSCLPendingError_( void )
{
	return sclerror::ReportPendingError( GetLanguage() );
}

static bso::bool__ main_(
	int argc,
	const char *argv[] )
{
	bso::bool__ Success = false;
ERRProlog
	str::string Language;
ERRBegin
	sclmisc::Initialize( TargetName, NULL );

	Language.Init();

	if ( sclrgstry::GetRegistry().GetValue( sclrgstry::Language, sclrgstry::GetRoot(), Language ) ) 
		Language.Convert( Language_ );

	Registry_.Init();

	Registry_.PushImportedLevel( sclrgstry::GetRegistry(), sclrgstry::GetRoot() );
	RegistryProjectLevel_ = Registry_.PushEmbeddedLevel( str::string( "Project" ) );
	ArgumentsProjectLevel_ = Registry_.PushEmbeddedLevel( str::string( "Arguments" ) );

	Main( argc, argv );
ERRErr
	if ( ERRType >= err::t_amount ) {
		switch ( ERRType ) {
		case err::t_Abort:
			Success = !ReportSCLPendingError_();
			ERRRst();
			break;
		case err::t_Free:
			ERRRst();
			ERRFwk();
			break;
		case err::t_None:
			ERRRst();
			ERRFwk();
			break;
		case err::t_Return:
			ERRRst();
			ERRFwk();
			break;
		default:
			ERRRst();
			ERRFwk();
			break;
		}
	}
ERREnd
ERREpilog
	return Success;
}

int main(
	int argc,
	const char *argv[] )
{
	int ExitValue = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	if ( !main_( argc, argv ) )
		ExitValue = EXIT_FAILURE;
ERRFErr
ERRFEnd
ERRFEpilog
	return ExitValue;
}

#define PROJECT_ROOT_PATH	"Projects/Project[@target=\"%1\"]"

void scltool::LoadProject(
	const char *FileName,
	const char *Target )
{
ERRProlog
	str::string Path;
	STR_BUFFER___ Buffer;
	rgstry::context___ Context;
	lcl::meaning Meaning;
ERRBegin
	Path.Init( PROJECT_ROOT_PATH );
	str::ReplaceTag( Path, 1, str::string( Target ), '%' );

	if ( Registry_.Fill( RegistryProjectLevel_, FileName, xpp::criterions___(), Path.Convert( Buffer ), Context ) != rgstry::sOK ) {
		Meaning.Init();
		rgstry::GetMeaning( Context, Meaning );
		ReportAndAbort( Meaning );
	};

ERRErr
ERREnd
ERREpilog
}

void scltool::LoadProject(
	const str::string_ &FileName,
	const char *Target )
{
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	LoadProject( FileName.Convert( Buffer ), Target );
ERRErr
ERREnd
ERREpilog
}

bso::bool__ scltool::GetValue(
	const rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	return Registry_.GetValue( Entry, Value );
}

bso::bool__ scltool::GetValues(
	const rgstry::tentry__ &Entry,
	str::strings_ &Values )
{
	return Registry_.GetValues( Entry, Values );
}

const str::string_ &scltool::GetOptionalValue(
	const rgstry::tentry__ &Entry,
	str::string_ &Value,
	bso::bool__ *Missing )
{
	if ( !GetValue( Entry, Value ) )
		if ( Missing != NULL )
			*Missing = true;
	
	return Value;
}

const char *scltool::GetOptionalValue(
	const rgstry::tentry__ &Entry,
	STR_BUFFER___ &Buffer,
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
	} else
		Value.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

const str::string_ &scltool::GetMandatoryValue(
	const rgstry::tentry__ &Entry,
	str::string_ &Value )
{
	if ( !GetValue( Entry, Value ) )
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( Entry );

	return Value;
}

const char *scltool::GetMandatoryValue(
	const rgstry::tentry__ &Entry,
	STR_BUFFER___ &Buffer )
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
	type scltool::GetMandatory##name(\
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
	type scltool::Get##name(\
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
	type scltool::GetMandatory##name(\
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
	type scltool::Get##name(\
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

class scltoolpersonnalization
{
public:
	scltoolpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~scltoolpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static scltoolpersonnalization Tutor;
