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

static TOL_CBUFFER___ Language_;

#define DEFAULT_LANGUAGE	"en"

static rgstry::level__ RegistrySetupLevel_ = RGSTRY_UNDEFINED_LEVEL;

rgstry::level__ scltool::GetRegistrySetupLevel( void )
{
	return RegistrySetupLevel_;
}

static rgstry::entry___ Command_( "Command", sclrgstry::Parameters );
static rgstry::entry___ ProjectFileName_( "ProjectFileName", sclrgstry::Parameters );

static rgstry::entry___ Arguments_( "Arguments", sclrgstry::Definitions );
#define ARGUMENT_TAG "Argument"
#define ARGUMENT_ID_ATTRIBUTE "id"
static rgstry::entry___ ArgumentId_( ARGUMENT_TAG "/@" ARGUMENT_ID_ATTRIBUTE, Arguments_ );
#define ARGUMENT_LONG_ATTRIBUTE		"long"
#define ARGUMENT_SHORT_ATTRIBUTE	"short"
static rgstry::entry___ ShortTaggedArgument_( RGSTRY_TAGGED_ENTRY( ARGUMENT_TAG, ARGUMENT_SHORT_ATTRIBUTE ), Arguments_ );
static rgstry::entry___ LongTaggedArgument_( RGSTRY_TAGGED_ENTRY( ARGUMENT_TAG, ARGUMENT_LONG_ATTRIBUTE ), Arguments_ );
static rgstry::entry___ IdTaggedArgument_( RGSTRY_TAGGED_ENTRY( ARGUMENT_TAG, ARGUMENT_ID_ATTRIBUTE ), Arguments_ );
#define ARGUMENT_TAGGING_ID_ATTRIBUTE "@"  ARGUMENT_ID_ATTRIBUTE
static rgstry::entry___ LongTaggedArgumentId_( ARGUMENT_TAGGING_ID_ATTRIBUTE, LongTaggedArgument_ );
static rgstry::entry___ ShortTaggedArgumentId_( ARGUMENT_TAGGING_ID_ATTRIBUTE, ShortTaggedArgument_ );
static rgstry::entry___ IdTaggedArgumentLong_( "@" ARGUMENT_LONG_ATTRIBUTE, IdTaggedArgument_ );
static rgstry::entry___ IdTaggedArgumentShort_( "@" ARGUMENT_SHORT_ATTRIBUTE, IdTaggedArgument_ );
static rgstry::entry___ IdTaggedArgumentDescription_( "@Description", IdTaggedArgument_ );
static rgstry::entry___ IdTaggedArgumentPath_( "@Path", IdTaggedArgument_ );
static rgstry::entry___ IdTaggedArgumentValue_( "@Value", IdTaggedArgument_ );
static rgstry::entry___ IdTaggedArgumentUsage_( "@Usage", IdTaggedArgument_ );
static rgstry::entry___ IdTaggedArgumentLabel_( "@Label", IdTaggedArgument_ );

static rgstry::entry___ ArgumentsLayouts_( "Layouts", Arguments_ );
static rgstry::entry___ UntaggedArgumentsLayout_( "Layout", ArgumentsLayouts_ );
static rgstry::entry___ CommandTaggedArgumentsLayout_( RGSTRY_TAGGING_ATTRIBUTE( "command" ), UntaggedArgumentsLayout_ );
static rgstry::entry___ UntaggedArgumentLink_( "Link", CommandTaggedArgumentsLayout_ );
static rgstry::entry___ IndexTaggedArgumentLink_( RGSTRY_TAGGING_ATTRIBUTE( "index" ), UntaggedArgumentLink_ );
static rgstry::entry___ IndexTaggedArgumentLinkTarget_( "@Target", IndexTaggedArgumentLink_ );

static const str::string_ &GetLongTaggedArguemntId_(
	str::string_ &Name,
	str::string_ &Id )
{
	return sclrgstry::GetMandatoryValue( rgstry::tentry___( LongTaggedArgumentId_, Name ), Id );
}

static const str::string_ &GetShortTaggedArguemntId_(
	str::string_ &Name,
	str::string_ &Id )
{
	return sclrgstry::GetMandatoryValue( rgstry::tentry___( ShortTaggedArgumentId_, Name ), Id );
}


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


void scltool::OldPrintDefaultCommandDescriptions(
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

enum stamp__ {
	sShort,	// '-...'
	sLong,	// '--...'
	s_amount,
	s_Undefined
};

class flag_
{
public:
	struct s {
		stamp__ Stamp;
		str::string_::s Name;
	} &S_;
	str::string_ Name;
	flag_( s &S )
	: S_( S ),
	  Name( S.Name )
	{}
	void reset( bso::bool__ P = true )
	{
		S_.Stamp = s_Undefined;
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
		S_.Stamp = F.S_.Stamp;
		Name = F.Name;

		return *this;
	}
	void Init( stamp__ Stamp = s_Undefined )
	{
		S_.Stamp = Stamp;
		Name.Init();
	}
	void Init(
		stamp__ Stamp,
		const str::string_ &Name )
	{
		Init( Stamp );

		this->Name = Name;
	}
	E_RODISCLOSE_( stamp__, Stamp );
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
		stamp__ Stamp,
		const str::string_ &Name,
		const char *Value )
	{
		Init();

		flag_::Init( Stamp, Name );

		this->Value = Value;
	}
	void Init(
		stamp__ Stamp,
		const str::string_ &Name,
		const str::string_ &Value )
	{
		flag_::Init( Stamp, Name );

		this->Value.Init( Value );
	}
};

E_AUTO( option );

typedef ctn::E_CONTAINER_( option_ ) options_;
E_AUTO( options );


typedef str::string_ argument_;
E_AUTO( argument );

typedef ctn::E_MCONTAINER_( argument_ ) arguments_;
E_AUTO( arguments );

static void ReportBadArgumentAndAbort_(	const char *Arg )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();
	Meaning.SetValue( SCLTOOL_NAME "_BadArgument" );
	Meaning.AddTag( Arg );
	ReportAndAbort( Meaning );
ERRErr
ERREnd
ERREpilog
}

void FillShort_(
	const char *First,
	const char *Last,
	flags_ &Flags )
{
ERRProlog
	const char *Current = First;
	flag Flag;
ERRBegin
	while ( Current <= Last ) {
		Flag.Init( sShort, str::string( Current ) );

		Flags.Append( Flag );

		Current++;
	}
ERRErr
ERREnd
ERREpilog
}

void FillShort_(
	const char *Arg,
	options_ &Options )
{
ERRProlog
	const char *Equal = NULL, *Last = NULL;
	option Option;
	str::string Name, Value;
ERRBegin
	Equal = strchr( Arg, '=' );

	if ( ( Equal - Arg  ) != 1 )
		ERRFwk();

	Last = Arg + strlen( Arg );

	Name.Init();
	Name.Append( *Arg );

	Value.Init();
	Value.Append( Equal + 1, Last - Equal -1 );

	Option.Init( sShort, Name, Value );

	Options.Append( Option );
ERRErr
ERREnd
ERREpilog
}

void FillShort_( 
	const char *Arg,
	flags_ &Flags,
	options_ &Options )
{
	const char
		*Equal = strchr( Arg, '=' ),
		*Last = Arg + strlen( Arg ) - 1;

	if ( Equal == NULL )
		FillShort_( Arg, Last, Flags );
	else {
		if ( Equal == Arg )
			ReportBadArgumentAndAbort_( Arg );

		FillShort_( Arg, Equal - 2, Flags );
		FillShort_( Equal - 1, Options );
	}
}

void FillLong_(
	const char *Arg,
	flags_ &Flags )
{
ERRProlog
	flag Flag;
ERRBegin
	Flag.Init( sLong, str::string( Arg ) );

	Flags.Append( Flag );
ERRErr
ERREnd
ERREpilog
}

void FillLong_(
	const char *Arg,
	options_ &Options )
{
ERRProlog
	const char *Equal = NULL, *Last = NULL;
	option Option;
	str::string Name, Value;
ERRBegin
	Equal = strchr( Arg, '=' );

	if ( Equal == Arg  )
		ReportBadArgumentAndAbort_( Arg );

	Last = Arg + strlen( Arg );

	Name.Init();
	Name.Append( Arg, Equal - Arg );

	Value.Init();
	Value.Append( Equal + 1, Last - Equal - 1 );

	Option.Init( sLong, Name, Value );

	Options.Append( Option );
ERRErr
ERREnd
ERREpilog
}


static void FillLong_( 
	const char *Arg,
	flags_ &Flags,
	options_ &Options )
{
	const char *Equal = strchr( Arg, '=' );

	if ( Equal == NULL )
		FillLong_( Arg, Flags );
	else
		FillLong_( Arg, Options );

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
		Arguments.Append( str::string( Arg ) );
		break;
	case 2:
		if (  Arg[0] == '-' )
			if ( Arg[1] == '-' )
				return true;
			else
				FillShort_( Arg + 1, Flags, Options );
		else
			Arguments.Append( str::string( Arg ) );

		break;
	default:
		if ( Arg[0] == '-' )
			if ( Arg[1] == '-' )
				FillLong_( Arg + 2, Flags, Options );
			else
				FillShort_( Arg + 1, Flags, Options );
		else
			Arguments.Append( str::string( Arg ) );
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
		FreeArgumentsOnly = Fill_( argv[Current++], Flags, Options, Arguments );
	}

	while ( Current < argc )
		Arguments.Append( str::string( argv[Current++] ) );
}

static const str::string_ &GetId_(
	const str::string_ &Name,
	rgstry::entry___ &Entry,
	str::string_ &Id )
{
ERRProlog
	rgstry::tags Tags;
	str::string Path;
ERRBegin
	Tags.Init();
	Tags.Append( Name );

	Path.Init();
	sclrgstry::GetValue( rgstry::tentry__( Entry, Tags ), Id );
ERRErr
ERREnd
ERREpilog
	return Id;
}

static const str::string_ &GetId_(
	const str::string_ &Name,
	stamp__ Stamp,
	str::string_ &Id )
{
	switch ( Stamp ) {
	case sShort:
		GetId_( Name, ShortTaggedArgumentId_, Id );
		break;
	case sLong:
		GetId_( Name, LongTaggedArgumentId_, Id );
		break;
	default:
		ERRFwk();
		break;
	}

	return Id;
}


static const str::string_ &GetId_(
	const flag_ &Flag,
	str::string_ &Id )
{
	return GetId_( Flag.Name, Flag.Stamp(), Id );
}

const str::string_ &GetIdTagged_(
	const str::string_ &Id,
	rgstry::entry___ &Entry,
	str::string_ &Value )
{
ERRProlog
	str::strings Tags;
ERRBegin
	Tags.Init();
	Tags.Append( Id );

	sclrgstry::GetValue( rgstry::tentry__( Entry, Tags ), Value );
ERRErr
ERREnd
ERREpilog
	return Value;
}

const str::string_ &GetLong_(
	const str::string_ &Id,
	str::string_ &Long )
{
	return GetIdTagged_( Id, IdTaggedArgumentLong_, Long );
}

const str::string_ &GetShort_(
	const str::string_ &Id,
	str::string_ &Short )
{
	return GetIdTagged_( Id, IdTaggedArgumentShort_, Short );
}

const str::string_ &GetLabel_(
	const str::string_ &Id,
	str::string_ &Label )
{
	return GetIdTagged_( Id, IdTaggedArgumentLabel_, Label );
}

const str::string_ &GetDescription_(
	const str::string_ &Id,
	str::string_ &Description )
{
	return GetIdTagged_( Id, IdTaggedArgumentDescription_, Description );
}

static const str::string_ &GetArgumentLabelTranslation_(
	const str::string_ &Id,
	str::string_ &Translation )
{
ERRProlog
	str::string Label;
	TOL_CBUFFER___ Buffer;
ERRBegin
	Label.Init();
	GetLabel_( Id, Label );

	scllocale::GetTranslation( Label.Convert( Buffer ), GetLanguage(), Translation );
ERRErr
ERREnd
ERREpilog
	return Translation;
}

static const str::string_ &GetArgumentDescriptionTranslation_(
	const str::string_ &Id,
	str::string_ &Translation )
{
ERRProlog
	str::string Description;
	TOL_CBUFFER___ Buffer;
ERRBegin
	Description.Init();
	GetDescription_( Id, Description );

	scllocale::GetTranslation( Description.Convert( Buffer ), GetLanguage(), Translation );
ERRErr
ERREnd
ERREpilog
	return Translation;
}

static const str::string_ &GetShortLong_(
	const str::string_ &Id,
	str::string_ &LongShort )
{
ERRProlog
	str::string Long, Short;
ERRBegin
	Long.Init();
	GetLong_( Id, Long );

	Short.Init();
	GetShort_( Id, Short );

	if ( Short.Amount() ) {
		LongShort.Append( '-' );
		LongShort.Append( Short );
		if ( Long.Amount() )
			LongShort.Append( '|' );
	}

	if ( Long.Amount() ) {
		LongShort.Append( "--" );
		LongShort.Append( Long );
	}

ERRErr
ERREnd
ERREpilog
	return LongShort;
}

static const str::string_ &GetShortLongAndLabel_(
	const str::string_ &Id,
	str::string_ &Value )
{
	GetShortLong_( Id, Value );

	Value.Append( "=<" );

	GetArgumentLabelTranslation_(Id, Value );

	Value.Append( '>' );

	return Value;
}


#if 0	// Obsolete
const str::string_ &scltool::GetArgumentShortLong(
	const str::string_ &Id,
	str::string_ &LongShort )
{
	return GetShortLong_( Id, LongShort );
}

static void PrintCommandDescription_(
	const char *ProgramName,
	const str::string_ &Id )
{
ERRProlog
	str::string Translation, LongShort;
ERRBegin
	LongShort.Init();
	cio::COut << ProgramName << " " << GetArgumentShortLong( Id, LongShort ) << txf::nl;
	Translation.Init();
	cio::COut << txf::pad << GetArgumentDescriptionTranslation_( Id, Translation ) << txf::nl;
ERRErr
ERREnd
ERREpilog
}

void scltool::PrintDefaultCommandDescriptions( const char *ProgramName )
{
ERRProlog
	str::string Translation;
ERRBegin
	Translation.Init();
	COut << scllocale::GetLocale().GetTranslation( "ProgramDescription", GetLanguage(), Translation ) << txf::nl;
	COut << txf::nl;

	PrintCommandDescription_( ProgramName, str::string( "Usage" ) );
	PrintCommandDescription_( ProgramName, str::string( "Version" ) );
	PrintCommandDescription_( ProgramName, str::string( "License" ) );

ERRErr
ERREnd
ERREpilog
}
#endif

static const str::string_ &GetPath_(
	const str::string_ &Id,
	str::string_ &Path )
{
	return GetIdTagged_( Id, IdTaggedArgumentPath_, Path );
}

static const str::string_ &GetValue_(
	const str::string_ &Id,
	str::string_ &Value )
{
	return GetIdTagged_( Id, IdTaggedArgumentValue_, Value );
}

static const str::string_ &GetCommand_( str::string_ &Command )
{
	sclrgstry::GetValue( Command_, Command );

	return Command;
}

#define COMMAND_PATH	"Parameters/Command"

static void FillSetupRegistry_(
	sdr::row__,	// Pas utile.
	const flag_ &Flag )
{
ERRProlog
	str::string Id;
	lcl::meaning  Meaning;
	str::strings Tags;
	str::string Path;
	str::string Value;
	sdr::row__ Error = E_NIL;
ERRBegin
	Id.Init();

	if ( GetId_( Flag, Id ).Amount() == 0 ) {
		Meaning.Init();
		Meaning.SetValue( SCLTOOL_NAME "_UnknownFlag" );
		Meaning.AddTag( Flag.Name );
		sclerror::SetMeaning( Meaning );
		ERRAbort();
	}

	Path.Init();
	GetPath_( Id, Path );

#if 1
	if ( Path.Amount() == 0 )	// Il s'agit d'une commande.
		Path = COMMAND_PATH;

#else // Obsolete
	if ( Path.Amount() == 0 ) {
		Meaning.Init();
		Meaning.SetValue( SCLTOOL_NAME "_NoPathForFlag" );
		Meaning.AddTag( Flag.Name );
		sclerror::SetMeaning( Meaning );
		ERRAbort();
	}
#endif

	Value.Init();
	GetValue_( Id, Value );

	if ( Value.Amount() == 0 ) {
		Meaning.Init();
		Meaning.SetValue( SCLTOOL_NAME "_NoValueForFlag" );
		Meaning.AddTag( Flag.Name );
		sclerror::SetMeaning( Meaning );
		ERRAbort();
	}

	sclrgstry::SetValue( Path, Value, &Error );

	if ( Error != E_NIL ) {
		Meaning.Init();
		Meaning.SetValue( SCLTOOL_NAME "_BadPathForFlag" );
		Meaning.AddTag( Flag.Name );
		sclerror::SetMeaning( Meaning );
		ERRAbort();
	}
ERRErr
ERREnd
ERREpilog
}

static void FillSetupRegistry_(
	sdr::row__,	// Pas utile.
	const option_ &Option )
{
ERRProlog
	str::string Id;
	lcl::meaning  Meaning;
	str::strings Tags;
	str::string Path;
	sdr::row__ Error = E_NIL;
ERRBegin
	Id.Init();

	if ( GetId_( Option, Id ).Amount() == 0 ) {
		Meaning.Init();
		Meaning.SetValue( SCLTOOL_NAME "_UnknownOption" );
		Meaning.AddTag( Option.Name );
		sclerror::SetMeaning( Meaning );
		ERRAbort();
	}

	Path.Init();
	GetPath_( Id, Path );

	if ( Path.Amount() == 0 ) {
		Meaning.Init();
		Meaning.SetValue( SCLTOOL_NAME "_NoPathForOption" );
		Meaning.AddTag( Option.Name );
		sclerror::SetMeaning( Meaning );
		ERRAbort();
	}

	sclrgstry::SetValue( Path, Option.Value, &Error );

	if ( Error != E_NIL ) {
		Meaning.Init();
		Meaning.SetValue( SCLTOOL_NAME "_BadPathForOption" );
		Meaning.AddTag( Option.Name );
		sclerror::SetMeaning( Meaning );
		ERRAbort();
	}
ERRErr
ERREnd
ERREpilog
}

static void FillSetupRegistry_(
	sdr::row__ Index,
	const argument_ &Argument )
{
ERRProlog
	str::string Id;
	lcl::meaning  Meaning;
	str::strings Tags;
	str::string Command;
	str::string Path;
	sdr::row__ Error = E_NIL;
	bso::integer_buffer__ Buffer;
ERRBegin
	Id.Init();

	Command.Init();
	sclrgstry::GetMandatoryValue( Command_, Command );

	Tags.Init();
	Tags.Append( Command );
	Tags.Append( str::string( bso::Convert( *Index, Buffer ) ) );

	Id.Init();
	sclrgstry::GetMandatoryValue( rgstry::tentry__( IndexTaggedArgumentLinkTarget_, Tags ), Id );

	Tags.Init();
	Tags.Append( Command );
	Tags.Append( Id );

	Path.Init();
	GetPath_( Id, Path );

	if ( Path.Amount() == 0 ) {
		Meaning.Init();
		Meaning.SetValue( SCLTOOL_NAME "_NoPathForArgument" );
		Meaning.AddTag( Id );
		sclerror::SetMeaning( Meaning );
		ERRAbort();
	}

	sclrgstry::SetValue( Path, Argument, &Error );

	if (Error != E_NIL) {
		Meaning.Init();
		Meaning.SetValue( SCLTOOL_NAME "_BadPathForArgument" );
		Meaning.AddTag( Id );
		sclerror::SetMeaning(Meaning);
		ERRAbort();
	}
ERRErr
ERREnd
ERREpilog
}

template <typename c, typename i> static void FillSetupRegistry_(const c &Conteneur)
{
	i Item;
	sdr::row__ Row = Conteneur.First();

	Item.Init( Conteneur );

	while ( Row != E_NIL ) {
		FillSetupRegistry_( Row, Item( Row ) );

		Row = Conteneur.Next( Row );
	}
}

static void FillSetupRegistry_(
	flags_ &Flags,
	options_ &Options,
	arguments_ &Arguments )
{
	FillSetupRegistry_<flags_, ctn::E_CMITEM( flag_ )>( Flags );
	FillSetupRegistry_<options_, ctn::E_CITEM( option_ )>( Options );
	FillSetupRegistry_<arguments_, ctn::E_CMITEM( argument_ )>( Arguments);
}

#define ARGUMENTS	"_/Arguments"
#define RAW	ARGUMENTS "/Raw"
#define RAW_ARGUMENT	RAW "/Argument"
#define INDICE_ATTRIBUTE	"indice"

static void PutIndice_(
	const char *Before,
	bso::uint__ Indice,
	const char *After,
	str::string_ &Result )
{
	bso::integer_buffer__ Buffer;

	Result.Append( Before );
	Result.Append( "[" INDICE_ATTRIBUTE "=\"" );
	Result.Append( bso::Convert( Indice, Buffer ) );
	Result.Append( "\"]" );

	if ( ( After != NULL ) && ( *After ) ) {
		Result.Append( '/' );
		Result.Append( After );
	}
}

#define AMOUNT_ATTRIBUTE	"Amount"

static void DumpInSetupRegistry_(
	int argc,
	const char **argv )
{
ERRProlog
	bso::integer_buffer__ Buffer;
	int i = 0;
	str::string Path;
ERRBegin
	sclrgstry::SetValue( str::string( RAW "/@" AMOUNT_ATTRIBUTE ), str::string( bso::Convert( (bso::int__)argc, Buffer ) ) );

	while ( i < argc ) {
		Path.Init();
		PutIndice_( RAW_ARGUMENT, i, "", Path );

		sclrgstry::SetValue( Path, str::string( argv[i++] ) );
	}
ERRErr
ERREnd
ERREpilog
}

#define ARGUMENT_FLAGS	ARGUMENTS "/Flags"
#define ARGUMENT_FLAG	ARGUMENT_FLAGS "/Flag"

static void DumpInSetupRegistry_(
	bso::int__ Indice,
	const flag_ &Flag )
{
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();
	PutIndice_( ARGUMENT_FLAG, Indice, "", Path );
	sclrgstry::SetValue( Path, Flag.Name );
ERRErr
ERREnd
ERREpilog
}

#define ARGUMENT_OPTIONS	ARGUMENTS "/Options"
#define ARGUMENT_OPTION		ARGUMENT_OPTIONS "/Option"

static void DumpInSetupRegistry_(
	bso::int__ Indice,
	const option_ &Option )
{
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();
	PutIndice_( ARGUMENT_OPTION, Indice, "Name", Path );
	sclrgstry::SetValue( Path, Option.Name );

	Path.Init();
	PutIndice_( ARGUMENT_OPTION, Indice, "Value", Path );
	sclrgstry::SetValue( Path, Option.Value );
ERRErr
ERREnd
ERREpilog
}

#define ARGUMENT_FREES	ARGUMENTS "/Frees"
#define ARGUMENT_FREE	ARGUMENT_FREES "/Free"

static void DumpInSetupRegistry_(
	bso::int__ Indice,
	const argument_ &Argument )
{
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();
	PutIndice_( ARGUMENT_FREE, Indice, "", Path );
	sclrgstry::SetValue( Path, Argument );
ERRErr
ERREnd
ERREpilog
}

static rgstry::entry___ FreeArguments_( ARGUMENT_FREES );
static rgstry::entry___ FreeArgumentsAmount_( "@" AMOUNT_ATTRIBUTE, FreeArguments_ );

#if 0	// Obsolete
bso::int__ scltool::GetFreeArgumentsAmount( void )
{
	return sclrgstry::GetMandatoryUInt( FreeArgumentsAmount_ );
}
#endif

static rgstry::entry___ TaggedFreeArgument_( RGSTRY_TAGGED_ENTRY( ARGUMENT_FREE, INDICE_ATTRIBUTE ) );

#if 0	// Obsolete
const str::string_ &scltool::GetFreeArgument(
	bso::uint__ Indice,
	str::string_ &Argument )
{
	bso::integer_buffer__ Buffer;

	return sclrgstry::GetMandatoryValue( rgstry::tentry___( TaggedFreeArgument_, bso::Convert( Indice, Buffer ) ), Argument );
}

void scltool::PutFreeArgumentTo(
	bso::uint__ Indice,
	const rgstry::tentry__ &Entry )
{
ERRProlog
	str::string Argument;
ERRBegin
	Argument.Init();
	GetFreeArgument( Indice, Argument );

	sclrgstry::SetValue( Argument, Entry );
ERRErr
ERREnd
ERREpilog
}
#endif

enum type__ {
	tCommand,
	tFlag,
	tOption,
	tFree,
	t_amount,
	t_Undefined
};

static type__ IdentifyArgument_( const str::string_ &Id )
{
	type__ Type = t_Undefined;
ERRProlog
	str::string Dummy;
ERRBegin
	Dummy.Init();
	GetPath_( Id, Dummy );

	if ( Dummy.Amount() == 0 )
		Type = tCommand;
	else {
		Dummy.Init();
		GetValue_( Id, Dummy );

		if ( Dummy.Amount() != 0 )
			Type = tFlag;
		else {
			Dummy.Init();
			GetShortLong_( Id, Dummy );

			if ( Dummy.Amount() == 0 )
				Type = tFree;
			else
				Type = tOption;
		}
	}
ERRErr
ERREnd
ERREpilog
	return Type;
}


typedef str::replace_callback__ _callback__;

namespace {
	class callback__
	: public _callback__
	{
	protected:
		bso::bool__ STRGetTagValue(
			const str::string_ &Tag,
			str::string_ &Value )
		{
			switch ( IdentifyArgument_( Tag ) ) {
			case tCommand:
				ERRFwk();
				break;
			case tFlag:
				GetShortLong_( Tag, Value );
				break;
			case tOption:
				GetShortLongAndLabel_( Tag, Value );
				break;
			case tFree:
				Value.Append( '<' );
				GetArgumentLabelTranslation_( Tag, Value );
				Value.Append( '>' );
				break;
			default:
				ERRFwk();
				break;
			}

			return true;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_callback__::reset(  P );
		}
		E_CDTOR( callback__ );
		void Init( void )
		{
			_callback__::Init();
		}
	};
}

static void PrintCommandUsage_( const str::string_ &Id )
{
ERRProlog
	str::string Dummy, Command;
	str::string_
		&Value = Dummy,
		&ShortLong = Dummy,
		&Translation = Dummy,
		&Usage = Dummy;
	bso::bool__ DefaultOne = false;
	callback__ Callback;
ERRBegin
	Value.Init();
	GetValue_( Id, Value );
	
	Command.Init();
	GetCommand_( Command );

	DefaultOne = Command == Value;

	cio::COut << scltool::TargetName << " ";

	if ( DefaultOne )
		cio::COut << '[';

	ShortLong.Init();
	cio::COut << GetShortLong_( Id, ShortLong );

	if ( DefaultOne )
		cio::COut << ']';

	Usage.Init();
	GetIdTagged_( Id, IdTaggedArgumentUsage_, Usage );

	if ( Usage.Amount() != 0 ) {
		Callback.Init();

		if ( !str::ReplaceLongTags( Usage, Callback, '%' ) )
			ERRFwk();

		cio::COut << ' ' << Usage;
	}


	Translation.Init();
	cio::COut << txf::nl << txf::pad << GetArgumentDescriptionTranslation_( Id, Translation );

ERRErr
ERREnd
ERREpilog

}

static void PrintFlagUsage_( const str::string_ &Id )
{
ERRProlog
	str::string Dummy;
	str::string_
		&Value = Dummy,
		&ShortLong = Dummy,
		&Translation = Dummy,
		&Path = Dummy;
ERRBegin
	ShortLong.Init();
	cio::COut << GetShortLong_( Id, ShortLong ) << ' ';

	Path.Init();
	cio::COut << "('" << GetPath_( Id, Path ) << '\'';

	Value.Init();
	cio::COut << "='" << GetValue_( Id, Value ) << "')";

	cio::COut << ':' << txf::nl;

	Translation.Init();
	cio::COut << txf::pad << GetArgumentDescriptionTranslation_( Id, Translation );
ERRErr
ERREnd
ERREpilog

}

static void PrintOptionUsage_( const str::string_ &Id )
{
ERRProlog
	str::string Dummy, Value;
	str::string_
		&Label = Dummy,
		&Description = Dummy,
		&Path = Dummy;
		TOL_CBUFFER___ Buffer;
ERRBegin
	Label.Init();
	cio::COut << '<' << GetArgumentLabelTranslation_( Id, Label ) << "> ";

	Path.Init();
	cio::COut << "('" << GetPath_( Id, Path ) << '\'';

	Value.Init();
	sclrgstry::GetValue( rgstry::entry___( Path.Convert( Buffer ) ), Value );

	if ( Value.Amount() != 0 )
		cio::COut << "(='" << Value << "')";

	cio::COut << "):" << txf::nl;

	Description.Init();
	cio::COut << txf::pad << GetArgumentDescriptionTranslation_( Id, Description );
ERRErr
ERREnd
ERREpilog
}

static void PrintFreeUsage_( const str::string_ &Id )
{
ERRProlog
	str::string Dummy;
	str::string_
		&Value = Dummy,
		&Label = Dummy,
		&Description = Dummy,
		&Path = Dummy;
ERRBegin
	Label.Init();
	cio::COut << '<' << GetArgumentLabelTranslation_( Id, Label ) << "> ";

	Path.Init();
	cio::COut << "('" << GetPath_( Id, Path ) << "')";

	cio::COut << ':' << txf::nl;

	Description.Init();
	cio::COut << txf::pad << GetArgumentDescriptionTranslation_( Id, Description );
ERRErr
ERREnd
ERREpilog
}


static void PrintUsage_(
	const str::string_ &Id,
	type__ Type )
{
	switch ( Type ) {
	case tCommand:
		PrintCommandUsage_( Id );
		break;
	case tFlag:
		PrintFlagUsage_( Id );
		break;
	case tOption:
		PrintOptionUsage_( Id );
		break;
	case tFree:
		PrintFreeUsage_( Id );
		break;
	default:
		ERRFwk();
		break;
	}
}

static void PrintUsage_(
	type__ Type,
	const str::strings_ &Ids )
{
	ctn::E_CMITEM( str::string_ ) Id;
	sdr::row__ Row = Ids.First();

	Id.Init( Ids );

	while ( Row != E_NIL ) {
		PrintUsage_( Id( Row ), Type );

		Row = Ids.Next( Row );

		cio::COut << txf::nl;
	}
}

static void IdentifyArguments_(
	const str::strings_ &Ids,
	str::strings_ &Commands,
	str::strings_ &Flags,
	str::strings_ &Options,
	str::strings_ &Frees )
{
	ctn::E_CMITEM( str::string_ ) Id;
	sdr::row__ Row = Ids.First();

	Id.Init( Ids );

	while ( Row != E_NIL ) {

//		cio::COut << Id( Row ) << " : " << txf::commit;

		switch ( IdentifyArgument_( Id( Row ) ) )  {
		case tCommand:
//			cio::COut << "Command";
			Commands.Append( Id( Row ) );
			break;
		case tFlag:
//			cio::COut << "Flag";
			Flags.Append( Id( Row ) );
			break;
		case tOption:
//			cio::COut << "Option";
			Options.Append( Id( Row ) );
			break;
		case tFree:
//			cio::COut << "Free";
			Frees.Append( Id( Row ) );
			break;
		default:
			ERRFwk();
			break;
		}
		
//		cio::COut << txf::pad;

		Row = Ids.Next( Row );
	}
}

static void PrintUsage_( void )
{
ERRProlog
	str::strings Ids, Commands, Flags, Options, Frees;
	str::string  ProgramDescription;
ERRBegin
	sclrgstry::GetRegistry().Delete( "Parameters/Command", RegistrySetupLevel_ );	// Pour pouvoir récupèrer la valeur correspondant à ce 'PAth' tel qu'éventuellement défini dans le fichier de configuration.

	Ids.Init();
	sclrgstry::GetValues( ArgumentId_, Ids );

	Commands.Init();
	Flags.Init();
	Options.Init();
	Frees.Init();
	IdentifyArguments_( Ids, Commands, Flags, Options, Frees );

	ProgramDescription.Init();
	COut << GetTranslation( "ProgramDescription", ProgramDescription ) << txf::nl << txf::nl;

	PrintUsage_( tCommand, Commands );

	if ( ( Flags.Amount() != 0 )
	     || ( Options.Amount() != 0 )
		 || ( Frees.Amount() != 0 ) )
	cio::COut << txf::nl;

	PrintUsage_( tFlag, Flags );
	PrintUsage_( tOption, Options );
	PrintUsage_( tFree, Frees );
	ERRErr
ERREnd
ERREpilog
}

#if 0	// Obsolete
void scltool::PrintCommandDescription(
	const char *CommandId,
	const char *OptionsAndFlags )
{
ERRProlog
	str::string Translation, ShortLong;
	str::string OptionsAndFlagsBuffer;
	callback__ Callback;
ERRBegin
	if ( ( CommandId == NULL ) || ( *CommandId == 0 ) )
		ERRFwk();

	ShortLong.Init();
	cio::COut << TargetName << " " << GetArgumentShortLong( CommandId, ShortLong );

	if ( ( OptionsAndFlags != NULL ) && ( *OptionsAndFlags != 0 ) ) {
		cio::COut << ' ';
		OptionsAndFlagsBuffer.Init( OptionsAndFlags );

		Callback.Init();

		if ( !str::ReplaceLongTags( OptionsAndFlagsBuffer, Callback, '%' ) )
			ERRFwk();

		cio::COut << OptionsAndFlagsBuffer;
	}
	
	cio::COut << txf::nl;

	Translation.Init();
	cio::COut << txf::pad << GetArgumentDescriptionTranslation_( CommandId, Translation ) << txf::nl;
ERRErr
ERREnd
ERREpilog
}
#endif

#if 0	// Obsolete
void scltool::PrintFlagDescription( const char *FlagId )
{
ERRProlog
	str::string Translation, ShortLong;
ERRBegin
	Translation.Init();
	ShortLong.Init();
	cio::COut << txf::pad <<GetArgumentShortLong( FlagId, ShortLong ) << ": ";
	Translation.Init();
	cio::COut << scltool::GetArgumentDescriptionTranslation( FlagId, Translation ) << txf::nl;
ERRErr
ERREnd
ERREpilog
}

void scltool::PrintOptionDescription(
	const char *Label,
	const char *OptionId )
{
ERRProlog
	str::string Translation;
ERRBegin
	Translation.Init();
	cio::COut << txf::pad << Label << ": " << GetArgumentDescriptionTranslation( OptionId, Translation ) << txf::nl;
ERRErr
ERREnd
ERREpilog
}
#endif

#if 0	// Obsolete
void scltool::PrintArgumentDescription(
	const char *Label,
	const char *ArgumentDescriptionLabel )
{
ERRProlog
	str::string Translation;
ERRBegin
	Translation.Init();
	cio::COut << txf::pad << Label << ": " << GetTranslation( ArgumentDescriptionLabel, Translation ) << txf::nl;
ERRErr
ERREnd
ERREpilog
}
#endif

template <typename c, typename i> static void DumpInSetupRegistry_(
	const char *Prefix,
	const c &Conteneur )
{
ERRProlog
	i Item;
	sdr::row__ Row = Conteneur.First();
	bso::integer_buffer__ Buffer;
	str::string Path;
ERRBegin
	Path.Init( Prefix );
	Path.Append( "/@" AMOUNT_ATTRIBUTE );

	sclrgstry::SetValue( Path, str::string( bso::Convert( Conteneur.Amount(), Buffer ) ) );

	Item.Init( Conteneur );

	while ( Row != E_NIL ) {
		DumpInSetupRegistry_( *Row, Item( Row ) );

		Row = Conteneur.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}


static void DumpInSetupRegistry_(
	int argc,
	const char **argv,
	const flags_ &Flags,
	const options_ &Options,
	const arguments_ &Arguments )
{
	DumpInSetupRegistry_( argc, argv );

	DumpInSetupRegistry_<flags_, ctn::E_CMITEM( flag_ )>( ARGUMENT_FLAGS, Flags );
	DumpInSetupRegistry_<options_, ctn::E_CITEM( option_ )>( ARGUMENT_OPTIONS, Options );
	DumpInSetupRegistry_<arguments_, ctn::E_CMITEM( argument_ )>( ARGUMENT_FREES, Arguments );
}

static void FillSetupRegistry_(
	int argc,
	const char **argv )
{
ERRProlog
	flags Flags;
	options Options;
	arguments Arguments;
ERRBegin
	Flags.Init();
	Options.Init();
	Arguments.Init();

	Fill_( argc, argv, Flags, Options, Arguments );

	FillSetupRegistry_( Flags, Options, Arguments );

	DumpInSetupRegistry_( argc, argv, Flags, Options, Arguments );
ERRErr
ERREnd
ERREpilog
}

static bso::bool__ ReportSCLPendingError_( void )
{
	return sclerror::ReportPendingError( GetLanguage() );
}

static void LoadProject_( const char *FileName )
{
ERRProlog
	rgstry::context___ Context;
	lcl::meaning Meaning;
ERRBegin
	Context.Init();

	if ( sclrgstry::LoadProject( FileName, scltool::TargetName, Context ) != tol::rSuccess ) {
		Meaning.Init();
		rgstry::GetMeaning( Context, Meaning );
		ReportAndAbort( Meaning );
	}

ERRErr
ERREnd
ERREpilog
}

static void LoadProject_( const str::string_ &FileName )
{
ERRProlog
	TOL_CBUFFER___ Buffer;
ERRBegin
	LoadProject_( FileName.Convert( Buffer ) );
ERRErr
ERREnd
ERREpilog
}

static bso::bool__ main_(
	int argc,
	const char *argv[] )
{
	bso::bool__ Success = false;
ERRProlog
	str::string Language;
	str::string ProjectFileName;
	str::string Command;
ERRBegin
	sclmisc::Initialize( TargetName, NULL );

	Language.Init();

	if ( sclrgstry::GetValue( sclrgstry::Language, Language ) ) 
		Language.Convert( Language_ );

	RegistrySetupLevel_ = sclrgstry::GetRegistry().PushEmbeddedLevel( str::string( "Setup" ) );

	FillSetupRegistry_( argc, argv );

	ProjectFileName.Init();
	sclrgstry::GetValue( ProjectFileName_, ProjectFileName );

	if ( ProjectFileName.Amount() != 0 )
		LoadProject_( ProjectFileName );

	Command.Init();

	if ( sclrgstry::GetMandatoryValue( Command_, Command ) == "Usage" )
		PrintUsage_();
	else
		Main( Command );
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
	cio::COut.Commit();
	cio::CErr.Commit();
ERRFEpilog
	return ExitValue;
}

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
