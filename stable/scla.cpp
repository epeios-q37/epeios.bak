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

#define SCLA_COMPILATION_

#include "scla.h"

#include "sclm.h"

using namespace scla;

using namespace sclm;
using scli::sInfo;

E_CDEF(bso::char__, ExplicitOptionMarker_, '#' );	// Marker of an option where the path is explicitly given.

str::string ParametersTag_;	// See tail.

static rgstry::entry___ Command_( "Command", sclr::Parameters );

static rgstry::entry___ &Arguments_ = sclr::Arguments;
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

/*
static const str::string_ &GetLongTaggedArguemntId_(
	str::string_ &Name,
	str::string_ &Id )
{
	return MGetValue( rgstry::tentry___( LongTaggedArgumentId_, Name ), Id );
}

static const str::string_ &GetShortTaggedArguemntId_(
	str::string_ &Name,
	str::string_ &Id )
{
	return MGetValue( rgstry::tentry___( ShortTaggedArgumentId_, Name ), Id );
}
*/
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
	void plug( uys::sHook &Hook )
	{
		Name.plug( Hook );
	}
	void plug( qASd *AS )
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
	void plug( qASd *AS )
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
qRH
	lcl::meaning Meaning;
qRB
	Meaning.Init();
	Meaning.SetValue( SCLA_NAME "_BadArgument" );
	Meaning.AddTag( Arg );
	sclm::ReportAndAbort( Meaning );
qRR
qRT
qRE
}

void FillShort_(
	const char *First,
	const char *Last,
	flags_ &Flags )
{
qRH
	const char *Current = First;
	flag Flag;
qRB
	while ( Current <= Last ) {
		Flag.Init( sShort, str::string( Current[0] ) );

		Flags.Append( Flag );

		Current++;
	}
qRR
qRT
qRE
}

void FillShort_(
	const char *Arg,
	options_ &Options )
{
qRH
	const char *Equal = NULL, *Last = NULL;
	option Option;
	str::string Name, Value;
qRB
	Equal = strchr( Arg, '=' );

	if ( ( Equal - Arg  ) != 1 )
		qRFwk();

	Last = Arg + strlen( Arg );

	Name.Init();
	Name.Append( *Arg );

	Value.Init();
	Value.Append( Equal + 1, Last - Equal -1 );

	Option.Init( sShort, Name, Value );

	Options.Append( Option );
qRR
qRT
qRE
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
qRH
	flag Flag;
qRB
	Flag.Init( sLong, str::string( Arg ) );

	Flags.Append( Flag );
qRR
qRT
qRE
}

void FillLong_(
	const char *Arg,
	options_ &Options )
{
qRH
	const char *Equal = NULL, *Last = NULL;
	option Option;
	str::string Name, Value;
qRB
	Equal = strchr( Arg, '=' );

	if ( Equal == Arg  )
		ReportBadArgumentAndAbort_( Arg );

	if ( Arg[0] == ExplicitOptionMarker_ ) {

		if ( strchr(Arg, '[' ) )
			Equal = strchr(Equal + 1, '=' );

		if ( Equal == NULL )
			ReportBadArgumentAndAbort_( Arg );
	}

	Last = Arg + strlen( Arg );

	Name.Init();
	Name.Append( Arg, Equal - Arg );

	Value.Init();
	Value.Append( Equal + 1, Last - Equal - 1 );

	Option.Init( sLong, Name, Value );

	Options.Append( Option );
qRR
qRT
qRE
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
	const str::strings_ &Arguments,
	flags_ &Flags,
	options_ &Options,
	arguments_ &Free,
	bso::bool__ IgnoreFirstArgument )
{
qRH
	sdr::row__ Row = qNIL;
	bso::bool__ FreeArgumentsOnly = false;
	TOL_CBUFFER___ Buffer;
qRB
	Row = Arguments.First( IgnoreFirstArgument ? 1 : 0 );

	while ( Row != qNIL && ( !FreeArgumentsOnly ) ) {
		FreeArgumentsOnly = Fill_( Arguments( Row ).Convert( Buffer ), Flags, Options, Free );
		Row = Arguments.Next( Row );
	}

	while ( Row != qNIL ) {
		Free.Append( Arguments( Row ) );
		Row = Arguments.Next( Row );
	}
qRR
qRT
qRE
}

static const str::string_ &GetId_(
	const str::string_ &Name,
	rgstry::entry___ &Entry,
	str::string_ &Id )
{
qRH
	rgstry::tags Tags;
	str::string Path;
qRB
	Tags.Init();
	Tags.Append( Name );

	Path.Init();
	OGetValue( rgstry::tentry__( Entry, Tags ), Id );
qRR
qRT
qRE
	return Id;
}

static const str::string_ &GetId_(
	const str::string_ &Name,
	stamp__ Stamp,
	str::string_ &Id )
{
	if ( Name(Name.First()) != ExplicitOptionMarker_ ) {
		switch ( Stamp ) {
		case sShort:
			GetId_( Name, ShortTaggedArgumentId_, Id );
			break;
		case sLong:
			GetId_( Name, LongTaggedArgumentId_, Id );
			break;
		default:
			qRFwk();
			break;
		}
	} else {
		Id.Append( Name );
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
qRH
	str::wStrings Tags;
qRB
	Tags.Init();
	Tags.Append( Id );

	OGetValue( rgstry::tentry__( Entry, Tags ), Value );
qRR
qRT
qRE
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
qRH
	str::string Label;
	TOL_CBUFFER___ Buffer;
qRB
	Label.Init();
	GetLabel_( Id, Label );

	sclm::GetBaseTranslation( Label.Convert( Buffer ),Translation );
qRR
qRT
qRE
	return Translation;
}

static const str::string_ &GetArgumentDescriptionTranslation_(
	const str::string_ &Id,
	str::string_ &Translation )
{
qRH
	str::string Description;
	TOL_CBUFFER___ Buffer;
qRB
	Description.Init();
	GetDescription_( Id, Description );

	sclm::GetBaseTranslation( Description.Convert( Buffer ),Translation );
qRR
qRT
qRE
	return Translation;
}

static const str::string_ &GetShortLong_(
	const str::string_ &Id,
	str::string_ &LongShort )
{
qRH
	str::string Long, Short;
qRB
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

qRR
qRT
qRE
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
qRH
	str::string Translation, LongShort;
qRB
	LongShort.Init();
	cio::COut << ProgramName << " " << GetArgumentShortLong( Id, LongShort ) << txf::nl;
	Translation.Init();
	cio::COut << txf::pad << GetArgumentDescriptionTranslation_( Id, Translation ) << txf::nl;
qRR
qRT
qRE
}

void scltool::PrintDefaultCommandDescriptions( const char *ProgramName )
{
qRH
	str::string Translation;
qRB
	Translation.Init();
	COut << scllocale::GetLocale().GetTranslation( "ProgramDescription", GetLanguage(), Translation ) << txf::nl;
	COut << txf::nl;

	PrintCommandDescription_( ProgramName, str::string( "Usage" ) );
	PrintCommandDescription_( ProgramName, str::string( "Version" ) );
	PrintCommandDescription_( ProgramName, str::string( "License" ) );

qRR
qRT
qRE
}
#endif

static const str::string_ &GetPath_(
	const str::string_ &Id,
	str::string_ &Path )
{
	if ( Id( Id.First() ) != ExplicitOptionMarker_ ) {
		GetIdTagged_( Id, IdTaggedArgumentPath_, Path );

		if ( Path.Amount() != 0  )
			if ( Path(Path.First()) != '/' )
				Path.InsertAt( ParametersTag_ );
	} else {
		Path.Append( ParametersTag_ );
		Path.Append( Id, Id.Next(Id.First() ) );
	}

	return Path;
}

static const str::string_ &GetValue_(
	const str::string_ &Id,
	str::string_ &Value )
{
	return GetIdTagged_( Id, IdTaggedArgumentValue_, Value );
}

namespace {
	void SetValue_(
		const str::dString &Path,
		const str::dString &Value,
		sdr::sRow *Error = NULL )
	{
	qRH
		sclm::rLocker_ Locker;
	qRB
		Locker.Init();
		sclm::GetRWRegistry().SetValue( sclr::GetRawLayer( sclr::lArguments ), Path, Value, Error );
	qRR
	qRT
	qRE
	}

	void AddValue_(
		const str::dString &Path,
		const str::dString &Value,
		sdr::sRow *Error = NULL )
	{
	qRH
		sclm::rLocker_ Locker;
	qRB
		Locker.Init();
		sclm::GetRWRegistry().AddValue( sclr::GetRawLayer( sclr::lArguments ), Path, Value, Error );
	qRR
	qRT
	qRE
	}
}

static void FillRegistry_(
	sdr::row__,	// Pas utile.
	const flag_ &Flag,
	bso::bool__ IgnoreUnknownArguments )
{
qRH
	str::string Id;
	lcl::meaning  Meaning;
	str::strings Tags;
	str::string Path;
	str::string Value;
	sdr::row__ Error = qNIL;
	str::string Name, EntryPath;
	bso::sBool Skip = false;
qRB
	Id.Init();

	if ( GetId_( Flag, Id ).Amount() == 0 ) {
		if ( IgnoreUnknownArguments )
			Skip = true;
		else {
			Meaning.Init();
			Meaning.SetValue( SCLA_NAME "_UnknownFlag" );

			Name.Init();
			switch ( Flag.Stamp() ) {
			case sShort:
				Name.Append( '-' );
				break;
			case sLong:
				Name.Append( "--" );
				break;
			default:
				qRFwk();
				break;
			}
			Name.Append( Flag.Name );
			Meaning.AddTag( Name );

			scle::SetMeaning( Meaning );
			qRAbort();
		}
	}

	if ( !Skip ) {
		Path.Init();
		GetPath_( Id, Path );

		if ( Path.Amount() == 0 )	// Il s'agit d'une commande.
			Command_.GetPath( Path );

		Value.Init();
		GetValue_( Id, Value );

		if ( Value.Amount() == 0 ) {
			Meaning.Init();
			Meaning.SetValue( SCLA_NAME "_NoValueForFlag" );
			Meaning.AddTag( Flag.Name );
			scle::SetMeaning( Meaning );
			qRAbort();
		}

		SetValue_( Path, Value, &Error );

		if ( Error != qNIL ) {
			Meaning.Init();
			Meaning.SetValue( SCLA_NAME "_BadPathForFlag" );
			Meaning.AddTag( Flag.Name );
			scle::SetMeaning( Meaning );
			qRAbort();
		}
	}
qRR
qRT
qRE
}

static void FillRegistry_(
	sdr::row__,	// Pas utile.
	const option_ &Option,
	bso::bool__ IgnoreUnknownArguments )
{
qRH
	str::string Id;
	lcl::meaning  Meaning;
	str::strings Tags;
	str::string Path;
	sdr::row__ Error = qNIL;
	str::string Name;
	bso::sBool Skip = false;
qRB
	Id.Init();

	if ( GetId_( Option, Id ).Amount() == 0 ) {
		if ( IgnoreUnknownArguments )
			Skip = true;
		else {
			Meaning.Init();
			Meaning.SetValue( SCLA_NAME "_UnknownOption" );

			Name.Init();
			switch ( Option.Stamp() ) {
			case sShort:
				Name.Append( '-' );
				break;
			case sLong:
				Name.Append( "--" );
				break;
			default:
				qRFwk();
				break;
			}
			Name.Append( Option.Name );
			Meaning.AddTag( Name );

			scle::SetMeaning( Meaning );
			qRAbort();
		}
	}

	if ( !Skip ) {
		Path.Init();
		GetPath_( Id, Path );

		if ( Path.Amount() == 0 ) {
			Meaning.Init();
			Meaning.SetValue( SCLA_NAME "_NoPathForOption" );
			Meaning.AddTag( Option.Name );
			scle::SetMeaning( Meaning );
			qRAbort();
		}

		SetValue_( Path, Option.Value, &Error );

		if ( Error != qNIL ) {
			Meaning.Init();
			Meaning.SetValue( SCLA_NAME "_BadPathForOption" );
			Meaning.AddTag( Option.Name );
			scle::SetMeaning( Meaning );
			qRAbort();
		}
	}
qRR
qRT
qRE
}

static void FillRegistry_(
	sdr::row__ Index,
	const argument_ &Argument,
	bso::bool__ IgnoreUnknownArguments )
{
qRH
	str::string Id;
	str::wStrings Tags;
	str::string Command;
	str::string Path;
	sdr::row__ Error = qNIL;
	bso::integer_buffer__ Buffer;
	bso::bool__ AdditionalArg = false;
qRB
	if ( !IgnoreUnknownArguments ) {
		Id.Init();

		Command.Init();
		MGetValue( Command_, Command );

		Tags.Init();
		Tags.Append( Command );
		Tags.Append( str::string( bso::Convert( *Index, Buffer ) ) );

		Id.Init();
		if ( !BGetValue(rgstry::tentry__(IndexTaggedArgumentLinkTarget_, Tags), Id, qRPU) ) {
			Tags.Init();
			Tags.Append( Command );
			Tags.Append( str::string() );
			AdditionalArg = true;
			if ( !BGetValue(rgstry::tentry__(IndexTaggedArgumentLinkTarget_, Tags), Id, qRPU) ) {
				sclm::ReportAndAbort( SCLA_NAME "_BadAmountOfArguments" );
			}
		}

		Tags.Init();
		Tags.Append( Command );
		Tags.Append( Id );

		Path.Init();
		GetPath_( Id, Path );

		if ( Path.Amount() == 0 )
			sclm::ReportAndAbort( SCLA_NAME "_NoPathForArgument", Id );

		if ( AdditionalArg )
			AddValue_( Path, Argument, &Error );
		else
			SetValue_( Path, Argument, &Error );

		if (Error != qNIL)
			sclm::ReportAndAbort( SCLA_NAME "_BadPathForArgument", Id );
	}
qRR
qRT
qRE
}

template <typename c> static void FillRegistry_(
	const c &Conteneur,
	bso::bool__ IgnoreUnknownArguments )
{
	sdr::row__ Row = Conteneur.First();

	while ( Row != qNIL ) {
		FillRegistry_( Row, Conteneur( Row ), IgnoreUnknownArguments );

		Row = Conteneur.Next( Row );
	}
}

static void FillRegistry_(
	flags_ &Flags,
	options_ &Options,
	arguments_ &Arguments,
	bso::bool__ IgnoreUnknownArguments )
{
	// Otions before flags, so the special options ('--#...') are handled first.
	FillRegistry_<options_>( Options, IgnoreUnknownArguments );
	FillRegistry_<flags_>( Flags, IgnoreUnknownArguments );
	FillRegistry_<arguments_>( Arguments, IgnoreUnknownArguments );
}

#define CLI_ARGUMENTS	"_/CLIArguments"
#define RAW	CLI_ARGUMENTS "/Raw"
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

static void DumpInRegistry_( const str::strings_ &Arguments )
{
qRH
	bso::integer_buffer__ IBuffer;
	str::string Path;
	TOL_CBUFFER___ SBuffer;
	sdr::row__ Row = qNIL;
qRB
	SetValue_( str::string( RAW "/@" AMOUNT_ATTRIBUTE ), str::string( bso::Convert( Arguments.Amount(), IBuffer ) ) );

	Row = Arguments.First();

	while ( Row != qNIL ) {
		Path.Init();
		PutIndice_( RAW_ARGUMENT, *Row, "", Path );

		SetValue_( Path, Arguments( Row ) );

		Row = Arguments.Next( Row );
	}
qRR
qRT
qRE
}

#define ARGUMENT_FLAGS	CLI_ARGUMENTS "/Flags"
#define ARGUMENT_FLAG	ARGUMENT_FLAGS "/Flag"

static void DumpInRegistry_(
	bso::int__ Indice,
	const flag_ &Flag )
{
qRH
	str::string Path;
qRB
	Path.Init();
	PutIndice_( ARGUMENT_FLAG, Indice, "", Path );
	SetValue_( Path, Flag.Name );
qRR
qRT
qRE
}

#define ARGUMENT_OPTIONS	CLI_ARGUMENTS "/Options"
#define ARGUMENT_OPTION		ARGUMENT_OPTIONS "/Option"

static void DumpInRegistry_(
	bso::int__ Indice,
	const option_ &Option )
{
qRH
	str::string Path;
qRB
	Path.Init();
	PutIndice_( ARGUMENT_OPTION, Indice, "Name", Path );
	SetValue_( Path, Option.Name );

	Path.Init();
	PutIndice_( ARGUMENT_OPTION, Indice, "Value", Path );
	SetValue_( Path, Option.Value );
qRR
qRT
qRE
}

#define ARGUMENT_FREES	CLI_ARGUMENTS "/Frees"
#define ARGUMENT_FREE	ARGUMENT_FREES "/Free"

static void DumpInRegistry_(
	bso::int__ Indice,
	const argument_ &Argument )
{
qRH
	str::string Path;
qRB
	Path.Init();
	PutIndice_( ARGUMENT_FREE, Indice, "", Path );
	SetValue_( Path, Argument );
qRR
qRT
qRE
}

static rgstry::entry___ FreeArguments_( ARGUMENT_FREES );
static rgstry::entry___ FreeArgumentsAmount_( "@" AMOUNT_ATTRIBUTE, FreeArguments_ );

static rgstry::entry___ TaggedFreeArgument_( RGSTRY_TAGGED_ENTRY( ARGUMENT_FREE, INDICE_ATTRIBUTE ) );

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
qRH
	str::string Dummy;
qRB
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
qRR
qRT
qRE
	return Type;
}

template <typename c> static void DumpInRegistry_(
	const char *Prefix,
	const c &Conteneur )
{
qRH
	sdr::row__ Row = Conteneur.First();
	bso::integer_buffer__ Buffer;
	str::string Path;
qRB
	Path.Init( Prefix );
	Path.Append( "/@" AMOUNT_ATTRIBUTE );

	SetValue_( Path, str::string( bso::Convert( Conteneur.Amount(), Buffer ) ) );

	while ( Row != qNIL ) {
		DumpInRegistry_( *Row, Conteneur( Row ) );

		Row = Conteneur.Next( Row );
	}
qRR
qRT
qRE
}

static void DumpInRegistry_(
	const str::strings_ &Arguments,
	const flags_ &Flags,
	const options_ &Options,
	const arguments_ &Free )
{
	DumpInRegistry_( Arguments );

	DumpInRegistry_<flags_>( ARGUMENT_FLAGS, Flags );
	DumpInRegistry_<options_>( ARGUMENT_OPTIONS, Options );
	DumpInRegistry_<arguments_>( ARGUMENT_FREES, Free );
}

void scla::FillRegistry(
	const str::strings_&Arguments,
	first_argument__ FirstArgument,
	unknown_arguments__ UnknownArguments )
{
qRH
	flags Flags;
	options Options;
	arguments Free;
qRB
	Flags.Init();
	Options.Init();
	Free.Init();

	Fill_( Arguments, Flags, Options, Free, FirstArgument == faIsCommand ? true : false );

	DumpInRegistry_( Arguments, Flags, Options, Free );
qRR
qRT
	// Put here, so what already be handled is put in registry, event if an error occurs.
	FillRegistry_( Flags, Options, Free, UnknownArguments == uaIgnore ? true : false );
qRE
}

namespace {
	E_CDEF( char, EscapeChar_, '\\' );
}

namespace {
	namespace {
		void Split_(
			flw::rRFlow &Flow,
			str::dStrings &Splitted )
		{
		qRH
			str::string Argument;
			bso::bool__ Escape = false, Quoted = false;
			bso::sChar C = 0;  // 0 if not quoted, or the used quote char (' or ").
		qRB
			Argument.Init();

			while ( !Flow.EndOfFlow() ) {
				C = Flow.Get();

				if ( C == EscapeChar_ ) {
					if ( Escape ) {
						if ( Quoted )
							Argument.Append( EscapeChar_ );

						Argument.Append( EscapeChar_ );
						Escape = false;
					} else
						Escape = true;
				} else if ( Escape ) {
					if ( C == '"' )
						Argument.Append( '"' );
					else if ( Quoted )
						sclm::ReportAndAbort( SCLA_NAME "_BadArguments" );
					else if ( C == ' ' )
						Argument.Append( ' ' );
					else if ( C == '"' )
						Argument.Append( '"' );
					else
						sclm::ReportAndAbort( SCLA_NAME "_BadArguments" );

					Escape = false;
				} else if ( C == '"' ) {
					if ( Argument.Amount() != 0 )
						Splitted.Append( Argument );

					Quoted = !Quoted;
				} else if ( ( C == ' ' ) && ( !Quoted ) ) {
					if ( Argument.Amount() != 0 )
						Splitted.Append( Argument );

					Argument.Init();
				} else
					Argument.Append( C );
			}

			if ( Escape || Quoted )
				sclm::ReportAndAbort( SCLA_NAME "_BadArguments" );

			if ( Argument.Amount() != 0 )
				Splitted.Append( Argument );
		qRR
		qRT
		qRE
		}
	}

	void Split_(
		const str::dString &Merged,
		str::dStrings &Splitted )
	{
	qRH
		flx::sStringIFlow Flow;
	qRB
		Flow.Init( Merged );

		Split_( Flow, Splitted );
	qRR
	qRT
	qRE
	}
}

void scla::FillRegistry(
	const str::dString &Arguments,
	first_argument__ FirstArgument,
	unknown_arguments__ UnknownArguments )
{
qRH
	str::wStrings SplittedArguments;
qRB
	SplittedArguments.Init();

	Split_( Arguments, SplittedArguments );

	FillRegistry( SplittedArguments, FirstArgument, UnknownArguments );
qRR
qRE
qRT
}

const str::string_ &scla::GetCommand( str::string_ &Command )
{
	return MGetValue( Command_, Command );
}

typedef tagsbs::long_tags_callback__ _callback__;

namespace {
	class callback__
	: public _callback__
	{
	protected:
		bso::bool__ TAGSBSGetTagValue(
			const str::string_ &Tag,
			str::string_ &Value )
		{
			switch ( IdentifyArgument_( Tag ) ) {
			case tCommand:
				qRFwk();
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
				qRFwk();
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

static const str::string_ &GetCommand_( str::string_ &Command )
{
	OGetValue( Command_, Command );

	return Command;
}

static void PrintCommandUsage_(
	const str::string_ &Id,
	const sInfo &Info )
{
qRH
	str::string Dummy, Command;
	str::string_
		&Value = Dummy,
		&ShortLong = Dummy,
		&Translation = Dummy,
		&Usage = Dummy;
	bso::bool__ DefaultOne = false;
	callback__ Callback;
qRB
	Value.Init();
	GetValue_( Id, Value );

	Command.Init();
	GetCommand_( Command );

	DefaultOne = Command == Value;

	cio::COut << Info.Target() << " ";

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

		if ( !tagsbs::SubstituteLongTags( Usage, Callback, '%' ) )
			qRFwk();

		cio::COut << ' ' << Usage;
	}


	Translation.Init();
	cio::COut << txf::nl << txf::pad << GetArgumentDescriptionTranslation_( Id, Translation );

qRR
qRT
qRE

}

static void PrintFlagUsage_( const str::string_ &Id )
{
qRH
	str::string Dummy;
	str::string_
		&Value = Dummy,
		&ShortLong = Dummy,
		&Translation = Dummy,
		&Path = Dummy;
qRB
	ShortLong.Init();
	cio::COut << GetShortLong_( Id, ShortLong ) << ' ';

	Path.Init();
	cio::COut << "('" << GetPath_( Id, Path ) << '\'';

	Value.Init();
	cio::COut << "='" << GetValue_( Id, Value ) << "')";

	cio::COut << ':' << txf::nl;

	Translation.Init();
	cio::COut << txf::pad << GetArgumentDescriptionTranslation_( Id, Translation );
qRR
qRT
qRE

}

static void PrintOptionUsage_( const str::string_ &Id )
{
qRH
	str::string Dummy, Value;
	str::string_
		&Label = Dummy,
		&Description = Dummy,
		&Path = Dummy;
		TOL_CBUFFER___ Buffer;
qRB
	Label.Init();
	cio::COut << '<' << GetArgumentLabelTranslation_( Id, Label ) << "> ";

	Path.Init();
	cio::COut << "('" << GetPath_( Id, Path ) << '\'';

	Value.Init();
	OGetValue( rgstry::entry___( Path.Convert( Buffer ) ), Value );

	if ( Value.Amount() != 0 )
		cio::COut << "(='" << Value << "')";

	cio::COut << "):" << txf::nl;

	Description.Init();
	cio::COut << txf::pad << GetArgumentDescriptionTranslation_( Id, Description );
qRR
qRT
qRE
}

static void PrintFreeUsage_( const str::string_ &Id )
{
qRH
	str::string Dummy;
	str::string_
		&Label = Dummy,
		&Description = Dummy,
		&Path = Dummy;
qRB
	Label.Init();
	cio::COut << '<' << GetArgumentLabelTranslation_( Id, Label ) << "> ";

	Path.Init();
	cio::COut << "('" << GetPath_( Id, Path ) << "')";

	cio::COut << ':' << txf::nl;

	Description.Init();
	cio::COut << txf::pad << GetArgumentDescriptionTranslation_( Id, Description );
qRR
qRT
qRE
}


static void PrintUsage_(
	const str::string_ &Id,
	type__ Type,
	const sInfo &Info )
{
	switch ( Type ) {
	case tCommand:
		PrintCommandUsage_( Id, Info );
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
		qRFwk();
		break;
	}
}

static void PrintUsage_(
	type__ Type,
	const str::strings_ &Ids,
	const sInfo &Info )
{
	sdr::row__ Row = Ids.First();

	while ( Row != qNIL ) {
		PrintUsage_( Ids( Row ), Type, Info );

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
	sdr::row__ Row = Ids.First();

	while ( Row != qNIL ) {

//		cio::COut << Id( Row ) << " : " << txf::commit;

		switch ( IdentifyArgument_( Ids( Row ) ) )  {
		case tCommand:
//			cio::COut << "Command";
			Commands.Append( Ids( Row ) );
			break;
		case tFlag:
//			cio::COut << "Flag";
			Flags.Append( Ids( Row ) );
			break;
		case tOption:
//			cio::COut << "Option";
			Options.Append( Ids( Row ) );
			break;
		case tFree:
//			cio::COut << "Free";
			Frees.Append( Ids( Row ) );
			break;
		default:
			qRFwk();
			break;
		}

//		cio::COut << txf::pad;

		Row = Ids.Next( Row );
	}
}

void scla::PrintUsage(
	const sInfo &Info,
	txf::text_oflow__ &Flow )
{
qRH
	str::wStrings Ids, Commands, Flags, Options, Frees;
	str::string  ProgramDescription;
	str::string EntryPath;
	sclm::rLocker_ Locker;
qRB
	EntryPath.Init();
	Locker.Init();
	sclm::GetRWRegistry().Delete( Command_.GetPath( EntryPath ), sclr::GetRawLayer( sclr::lArguments ) );	// Pour pouvoir rcuprer la valeur correspondant  ce 'Path' tel qu'ventuellement dfini dans le fichier de configuration.
	Locker.Unlock();

	Ids.Init();
	GetValues( ArgumentId_, Ids );

	Commands.Init();
	Flags.Init();
	Options.Init();
	Frees.Init();
	IdentifyArguments_( Ids, Commands, Flags, Options, Frees );

	ProgramDescription.Init();
	Flow << sclm::GetBaseTranslation( "ProgramDescription", ProgramDescription ) << txf::nl << txf::nl;

	PrintUsage_( tCommand, Commands, Info );

	if ( ( Flags.Amount() != 0 )
	     || ( Options.Amount() != 0 )
		 || ( Frees.Amount() != 0 ) )
	cio::COut << txf::nl;

	PrintUsage_( tFlag, Flags, Info );
	PrintUsage_( tOption, Options, Info );
	PrintUsage_( tFree, Frees, Info );
qRR
qRT
qRE
}

namespace {
	void Normalize_(
		flw::rRFlow &Flow,
		str::dString &Normalized )
	{
		char C = 0;

		while ( !Flow.EndOfFlow() ) {
			C = Flow.Get();

			if ( C == EscapeChar_ )
				Normalized.Append( EscapeChar_ );

			Normalized.Append( C );
		}
	}
}


void scla::Normalize(
	const str::dString &Raw,
	str::dString &Normalized )
{
qRH
	flx::sStringIFlow Flow;
qRB
	Flow.Init( Raw );

	Normalize_( Flow, Normalized );
qRR
qRT
qRE
}


Q37_GCTOR( scla )
{
	ParametersTag_.Init( sclr::ParametersTag );
	ParametersTag_.Append('/' );
}
