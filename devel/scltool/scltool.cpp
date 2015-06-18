/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#define SCLTOOL__COMPILATION

#include "scltool.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "cio.h"
#include "tagsbs.h"

#include "sclrgstry.h"
#include "scllocale.h"
#include "sclmisc.h"
#include "sclerror.h"

using namespace scltool;

using namespace sclmisc;

using cio::COut;
using scllocale::GetLocale;

static err::err___ qRRor_;
static sclerror::error___ SCLError_;

bso::bool__ scltool::IgnoreUnknownArguments = false;

str::string ParametersTag_;	// Voir tout en bas.

static rgstry::entry___ Command_( "Command", sclrgstry::Parameters );
static rgstry::entry___ ProjectFileName_( "ProjectFileName", sclrgstry::Parameters );

static rgstry::entry___ &Arguments_ = sclrgstry::Arguments;
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
	return MGetValue( rgstry::tentry___( LongTaggedArgumentId_, Name ), Id );
}

static const str::string_ &GetShortTaggedArguemntId_(
	str::string_ &Name,
	str::string_ &Id )
{
	return MGetValue( rgstry::tentry___( ShortTaggedArgumentId_, Name ), Id );
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
	void plug( qSD__ &SD )
	{
		Name.plug( SD );
	}
	void plug( qAS_ &AS )
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
	void plug( qAS_ &AS )
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
	Meaning.SetValue( SCLTOOL_NAME "_BadArgument" );
	Meaning.AddTag( Arg );
	sclmisc::ReportAndAbort( Meaning );
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
		Flag.Init( sShort, str::string( Current ) );

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
		qRFwk();
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
	ntvstr::char__ *argv[],
	flags_ &Flags,
	options_ &Options,
	arguments_ &Arguments )
{
qRH
	int Current = 1;
	bso::bool__ FreeArgumentsOnly = false;
	TOL_CBUFFER___ Buffer;
qRB
	while ( ( Current < argc ) && ( !FreeArgumentsOnly ) ) {
		FreeArgumentsOnly = Fill_( ntvstr::string___( argv[Current++] ).UTF8( Buffer ), Flags, Options, Arguments );
	}

	while ( Current < argc )
		Arguments.Append( str::string( ntvstr::string___( argv[Current++] ).UTF8( Buffer ) ) );
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

E_CDEF(bso::char__, ExplicitOptionMarker_, '#' );	// Marqueur d'une option dans laquelle on prcise explicitement le chemin.

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
	str::strings Tags;
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

	sclmisc::GetBaseTranslation( Label.Convert( Buffer ),Translation );
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

	sclmisc::GetBaseTranslation( Description.Convert( Buffer ),Translation );
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
	str::string_ &Path,
	bso::bool__ Short )	// Si  'true', retourne la version raccourcie.
{
	if ( Id( Id.First() ) != ExplicitOptionMarker_ ) {
		GetIdTagged_( Id, IdTaggedArgumentPath_, Path );

		if ( Path.Amount() != 0  )
			if ( Path(Path.First()) != '/' )
				Path.InsertAt( ParametersTag_ );	
	}
	else {
		Path.Append( ParametersTag_ );
		Path.Append(Id, Id.Next(Id.First() ) );
	}

	return Path;
}

static const str::string_ &GetValue_(
	const str::string_ &Id,
	str::string_ &Value )
{
	return GetIdTagged_( Id, IdTaggedArgumentValue_, Value );
}

static const str::string_ &GetCommand_( str::string_ &Command )
{
	OGetValue( Command_, Command );

	return Command;
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
qRB
	Id.Init();

	if ( GetId_( Flag, Id ).Amount() == 0 ) {
		if ( IgnoreUnknownArguments )
			qRReturn;
		else {
			Meaning.Init();
			Meaning.SetValue( SCLTOOL_NAME "_UnknownFlag" );

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

			sclerror::SetMeaning( Meaning );
			qRAbort();
		}
	}

	Path.Init();
	GetPath_( Id, Path, false );

#if 1
	if ( Path.Amount() == 0 )	// Il s'agit d'une commande.
		Command_.GetPath( Path );

#else // Obsolete
	if ( Path.Amount() == 0 ) {
		Meaning.Init();
		Meaning.SetValue( SCLTOOL_NAME "_NoPathForFlag" );
		Meaning.AddTag( Flag.Name );
		sclerror::SetMeaning( Meaning );
		qRAbort();
	}
#endif

	Value.Init();
	GetValue_( Id, Value );

	if ( Value.Amount() == 0 ) {
		Meaning.Init();
		Meaning.SetValue( SCLTOOL_NAME "_NoValueForFlag" );
		Meaning.AddTag( Flag.Name );
		sclerror::SetMeaning( Meaning );
		qRAbort();
	}

	SetValue( Path, Value, &Error );

	if ( Error != qNIL ) {
		Meaning.Init();
		Meaning.SetValue( SCLTOOL_NAME "_BadPathForFlag" );
		Meaning.AddTag( Flag.Name );
		sclerror::SetMeaning( Meaning );
		qRAbort();
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
qRB
	Id.Init();

	if ( GetId_( Option, Id ).Amount() == 0 ) {
		if ( IgnoreUnknownArguments )
			qRReturn;
		else {
			Meaning.Init();
			Meaning.SetValue( SCLTOOL_NAME "_UnknownOption" );

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

			sclerror::SetMeaning( Meaning );
			qRAbort();
		}
	}

	Path.Init();
	GetPath_( Id, Path, false );

	if ( Path.Amount() == 0 ) {
		Meaning.Init();
		Meaning.SetValue( SCLTOOL_NAME "_NoPathForOption" );
		Meaning.AddTag( Option.Name );
		sclerror::SetMeaning( Meaning );
		qRAbort();
	}

	SetValue( Path, Option.Value, &Error );

	if ( Error != qNIL ) {
		Meaning.Init();
		Meaning.SetValue( SCLTOOL_NAME "_BadPathForOption" );
		Meaning.AddTag( Option.Name );
		sclerror::SetMeaning( Meaning );
		qRAbort();
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
	str::strings Tags;
	str::string Command;
	str::string Path;
	sdr::row__ Error = qNIL;
	bso::integer_buffer__ Buffer;
	bso::bool__ AdditionalArg = false;
qRB
	if ( IgnoreUnknownArguments )
		qRReturn;

	Id.Init();

	Command.Init();
	MGetValue( Command_, Command );

	Tags.Init();
	Tags.Append( Command );
	Tags.Append( str::string( bso::Convert( *Index, Buffer ) ) );

	Id.Init();
	if ( !BGetValue(rgstry::tentry__(IndexTaggedArgumentLinkTarget_, Tags), Id) ) {
		Tags.Init();
		Tags.Append( Command );
		Tags.Append( str::string() );
		AdditionalArg = true;
		if ( !BGetValue(rgstry::tentry__(IndexTaggedArgumentLinkTarget_, Tags), Id) ) {
			sclmisc::ReportAndAbort( SCLTOOL_NAME "_BadAmountOfArguments" );
		}
	}

	Tags.Init();
	Tags.Append( Command );
	Tags.Append( Id );

	Path.Init();
	GetPath_( Id, Path, false );

	if ( Path.Amount() == 0 )
		sclmisc::ReportAndAbort( SCLTOOL_NAME "_NoPathForArgument", Id );

	if ( AdditionalArg )
		AddValue( Path, Argument, &Error );
	else
		SetValue( Path, Argument, &Error );

	if (Error != qNIL)
		sclmisc::ReportAndAbort( SCLTOOL_NAME "_BadPathForArgument", Id );
qRR
qRT
qRE
}

template <typename c, typename i> static void FillRegistry_(
	const c &Conteneur,
	bso::bool__ IgnoreUnknownArguments )
{
	i Item;
	sdr::row__ Row = Conteneur.First();

	Item.Init( Conteneur );

	while ( Row != qNIL ) {
		FillRegistry_( Row, Item( Row ), IgnoreUnknownArguments );

		Row = Conteneur.Next( Row );
	}
}

static void FillRegistry_(
	flags_ &Flags,
	options_ &Options,
	arguments_ &Arguments,
	bso::bool__ IgnoreUnknownArguments )
{
	FillRegistry_<flags_, ctn::E_CMITEM( flag_ )>( Flags, IgnoreUnknownArguments );
	FillRegistry_<options_, ctn::E_CITEM( option_ )>( Options, IgnoreUnknownArguments );
	FillRegistry_<arguments_, ctn::E_CMITEM( argument_ )>( Arguments, IgnoreUnknownArguments );
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

static void DumpInRegistry_(
	int argc,
	ntvstr::char__ *argv[] )
{
qRH
	bso::integer_buffer__ IBuffer;
	int i = 0;
	str::string Path;
	TOL_CBUFFER___ SBuffer;
qRB
	SetValue( str::string( RAW "/@" AMOUNT_ATTRIBUTE ), str::string( bso::Convert( (bso::int__)argc, IBuffer ) ) );

	while ( i < argc ) {
		Path.Init();
		PutIndice_( RAW_ARGUMENT, i, "", Path );

		SetValue( Path, str::string( ntvstr::string___( argv[i++] ).UTF8( SBuffer ) ) );
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
	SetValue( Path, Flag.Name );
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
	SetValue( Path, Option.Name );

	Path.Init();
	PutIndice_( ARGUMENT_OPTION, Indice, "Value", Path );
	SetValue( Path, Option.Value );
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
	SetValue( Path, Argument );
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
	GetPath_( Id, Dummy, false );

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

static void PrintCommandUsage_( const str::string_ &Id )
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

	cio::COut << sclmisc::SCLMISCTargetName << " ";

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
	cio::COut << "('" << GetPath_( Id, Path, true ) << '\'';

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
	cio::COut << "('" << GetPath_( Id, Path, true ) << '\'';

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
		&Value = Dummy,
		&Label = Dummy,
		&Description = Dummy,
		&Path = Dummy;
qRB
	Label.Init();
	cio::COut << '<' << GetArgumentLabelTranslation_( Id, Label ) << "> ";

	Path.Init();
	cio::COut << "('" << GetPath_( Id, Path, true ) << "')";

	cio::COut << ':' << txf::nl;

	Description.Init();
	cio::COut << txf::pad << GetArgumentDescriptionTranslation_( Id, Description );
qRR
qRT
qRE
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
		qRFwk();
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

	while ( Row != qNIL ) {
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

	while ( Row != qNIL ) {

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
			qRFwk();
			break;
		}
		
//		cio::COut << txf::pad;

		Row = Ids.Next( Row );
	}
}

static void PrintUsage_( void )
{
qRH
	str::strings Ids, Commands, Flags, Options, Frees;
	str::string  ProgramDescription;
	str::string EntryPath;
qRB
	EntryPath.Init();
	sclrgstry::GetCommonRegistry().Delete( Command_.GetPath( EntryPath ), sclrgstry::GetArgumentsLevel() );	// Pour pouvoir rcuprer la valeur correspondant  ce 'Path' tel qu'ventuellement dfini dans le fichier de configuration.

	Ids.Init();
	GetValues( ArgumentId_, Ids );

	Commands.Init();
	Flags.Init();
	Options.Init();
	Frees.Init();
	IdentifyArguments_( Ids, Commands, Flags, Options, Frees );

	ProgramDescription.Init();
	COut << sclmisc::GetBaseTranslation( "ProgramDescription", ProgramDescription ) << txf::nl << txf::nl;

	PrintUsage_( tCommand, Commands );

	if ( ( Flags.Amount() != 0 )
	     || ( Options.Amount() != 0 )
		 || ( Frees.Amount() != 0 ) )
	cio::COut << txf::nl;

	PrintUsage_( tFlag, Flags );
	PrintUsage_( tOption, Options );
	PrintUsage_( tFree, Frees );
qRR
qRT
qRE
}

template <typename c, typename i> static void DumpInRegistry_(
	const char *Prefix,
	const c &Conteneur )
{
qRH
	i Item;
	sdr::row__ Row = Conteneur.First();
	bso::integer_buffer__ Buffer;
	str::string Path;
qRB
	Path.Init( Prefix );
	Path.Append( "/@" AMOUNT_ATTRIBUTE );

	SetValue( Path, str::string( bso::Convert( Conteneur.Amount(), Buffer ) ) );

	Item.Init( Conteneur );

	while ( Row != qNIL ) {
		DumpInRegistry_( *Row, Item( Row ) );

		Row = Conteneur.Next( Row );
	}
qRR
qRT
qRE
}


static void DumpInRegistry_(
	int argc,
	ntvstr::char__ *argv[],
	const flags_ &Flags,
	const options_ &Options,
	const arguments_ &Arguments )
{
	DumpInRegistry_( argc, argv );

	DumpInRegistry_<flags_, ctn::E_CMITEM( flag_ )>( ARGUMENT_FLAGS, Flags );
	DumpInRegistry_<options_, ctn::E_CITEM( option_ )>( ARGUMENT_OPTIONS, Options );
	DumpInRegistry_<arguments_, ctn::E_CMITEM( argument_ )>( ARGUMENT_FREES, Arguments );
}

static void FillRegistry_(
	int argc,
	ntvstr::char__ *argv[],
	bso::bool__ IgnoreUnknownArguments )
{
qRH
	flags Flags;
	options Options;
	arguments Arguments;
qRB
	Flags.Init();
	Options.Init();
	Arguments.Init();

	Fill_( argc, argv, Flags, Options, Arguments );

	FillRegistry_( Flags, Options, Arguments, IgnoreUnknownArguments );

	DumpInRegistry_( argc, argv, Flags, Options, Arguments );
qRR
qRT
qRE
}

static inline bso::bool__ ReportSCLPendingError_( void )
{
	bso::bool__ Exists = false;
qRH
	str::string Translation;
qRB
	Translation.Init();
	
	Exists = sclmisc::GetSCLBasePendingErrorTranslation( Translation );

	cio::CErr << Translation << txf::nl;
qRR
qRT
qRE
	return Exists;
}

static int main_(
	const oddities__ &Oddities,
	const cio::set__ &CIO )
{
	int ExitValue = EXIT_SUCCESS;
qRH
	str::string ProjectFileName;
	str::string Command;
	str::string ProjectId;
qRB
	sclmisc::Initialize( &qRRor_, &SCLError_, CIO, (const char *)NULL );

	FillRegistry_( Oddities.argc, Oddities.argv, IgnoreUnknownArguments );

	ProjectFileName.Init();
	OGetValue( ProjectFileName_, ProjectFileName );

	ProjectId.Init();

	if ( ProjectFileName.Amount() != 0 )
		sclmisc::LoadProject( ProjectFileName, ProjectId );

	sclrgstry::FillSetupRegistry();

	sclmisc::RefreshBaseLanguage();

	Command.Init();

	if ( MGetValue( Command_, Command ) == "Usage" )
		PrintUsage_();
	else
		ExitValue = SCLTOOLMain( Command, Oddities );
qRR
	if ( ERRType >= err::t_amount ) {
		switch ( ERRType ) {
		case err::t_Abort:
			if ( ReportSCLPendingError_() )
				ExitValue = EXIT_FAILURE;
			ERRRst();
			break;
		case err::t_Free:
			ERRRst();
			qRFwk();
			break;
		case err::t_None:
			ERRRst();
			qRFwk();
			break;
		case err::t_Return:
			ERRRst();
			qRFwk();
			break;
		default:
			ERRRst();
			qRFwk();
			break;
		}
	}
qRT
qRE
	return ExitValue;
}

static void ErrFinal_( void )
{

	if ( ERRType != err::t_Abort ) {
		err::buffer__ Buffer;

		const char *Message = err::Message( Buffer );

		ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.

		qRH
		qRB
			if ( cio::IsInitialized() ) {
				if ( cio::Target() == cio::tConsole ) {
					cio::COut << txf::commit;
					cio::CErr << txf::nl << txf::tab;
				}

				cio::CErr << "{ " << Message << " }";

				if ( cio::Target() == cio::tConsole )
					cio::CErr << txf::nl;

				cio::CErr << txf::commit;
			} else
				qRFwk();
		qRR
		qRT
		qRE
	} else
		ERRRst();
}



#ifdef CPE_S_WIN

#undef system	// Dfini dans 'tol.h', rfrenc dans le fichier ci-dessous.

#include <iostream>

static void SetStdOutToNewConsole_( void )
{
	int hCrt;
	FILE *hf;

	AllocConsole();
	hCrt = _open_osfhandle(
		(intptr_t)GetStdHandle( STD_OUTPUT_HANDLE ),
		_O_TEXT
		);
	hf = _fdopen( hCrt, "w" );
	*stdout = *hf;
	setvbuf( stdout, NULL, _IONBF, 0 );
}

// MSVC va utiliser soit 'wmain' soit 'wWinMain' (et ignorer l'autre) selon la valeur de 'SubSystem'.

int wmain(
	int argc,
	wchar_t *argv[] )
{
	int ExitValue = EXIT_SUCCESS;
qRFH
	oddities__ Oddities;
qRFB
	Oddities.argc = argc;
	Oddities.argv = argv;

	ExitValue = main_( Oddities, cio::GetSet( cio::t_Default ) );
qRFR
qRFT	
	cio::COut.Commit();
	cio::CErr.Commit();
	cio::CIn.Dismiss();
qRFE( ErrFinal_() )
	return ExitValue;
}

int WINAPI wWinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		PWSTR pCmdLine,
		int nCmdShow )
{
	int ExitValue = EXIT_SUCCESS;
qRFH
	str::string SOut, SErr;
	flx::bunch_oflow_driver___<str::string_, bso::char__> FOut, FErr;
	flx::void_iflow_driver___ FIn;
	cio::set__ CIO;
	oddities__ Oddities;
qRFB
	Oddities.argv = CommandLineToArgvW( GetCommandLineW(), &Oddities.argc );

	Oddities.hInstance = hInstance;
	Oddities.hPrevInstance = hPrevInstance;
	Oddities.nCmdShow = nCmdShow;
	Oddities.pCmdLine = pCmdLine;

	SOut.Init();
	FOut.Init( SOut, fdr::ts_Default );

	SErr.Init();
	FErr.Init( SErr, fdr::ts_Default );

	FIn.Init( fdr::ts_Default, flx::a_Default );

	CIO.Init( FIn, FOut, FErr );

	ExitValue = main_( Oddities, CIO );
qRFR
qRFT
	if ( Oddities.argv != NULL )
		LocalFree( Oddities.argv );

	cio::COut.reset();
	cio::CErr.reset();
	cio::CIn.reset();

	FOut.reset();
	FErr.reset();
	FIn.reset();

#if 0	/// Fait planter 'CEF'.
	if ( SOut.Amount() )
		MessageBoxW( NULL, ntvstr::string___( SOut ).Internal(), ntvstr::string___( sclmisc::SCLMISCTargetName ).Internal(), MB_OK );

	if ( SErr.Amount() )
		MessageBoxW( NULL, ntvstr::string___( SErr ).Internal(), ntvstr::string___( sclmisc::SCLMISCTargetName ).Internal(), MB_OK | MB_TASKMODAL );
#else
	if ( SOut.Amount() || SErr.Amount() )
	{
		SetStdOutToNewConsole_();

		if ( SOut.Amount() )
			std::wcout << (wchar_t *)ntvstr::string___( SOut ).Internal() << std::endl;

		if ( SErr.Amount() )
			std::wcout << (wchar_t *)ntvstr::string___( SErr ).Internal() << std::endl;
	}
#endif
qRFE( ErrFinal_() )
	return ExitValue;
}

#else
int main(
	int argc,
	char *argv[] )
{
	int ExitValue = EXIT_SUCCESS;
qRFH
	oddities__ Oddities;
qRFB
	Oddities.argv = argv;
	Oddities.argc = argc;

	if ( !main_( Oddities, cio::GetSet( cio::t_Default ) ) )
		ExitValue = EXIT_FAILURE;
qRFR
qRFT	
	cio::COut.Commit();
	cio::CErr.Commit();
	cio::CIn.Dismiss();
qRFE( ErrFinal_() )
	return ExitValue;
}
#endif

txf::text_oflow__ &scltool::text_oflow_rack___::Init( const fnm::name___ &FileName )
{
	_FileName.Init( FileName );

	if ( _FileName.IsEmpty() ) {
		_BackedUp = false;
		return cio::COut;
	} else {
		sclmisc::CreateBackupFile( _FileName );
		_BackedUp = true;

		if ( _Flow.Init( _FileName ) != tol::rSuccess )
			sclmisc::ReportFileOpeningErrorAndAbort( _FileName );

		_TFlow.Init( _Flow );

		return _TFlow;
	}
}

void scltool::text_oflow_rack___::HandleError( void )
{
	if ( _BackedUp )
		sclmisc::RecoverBackupFile( _FileName );
}

static inline void signal_( int s )
{
	exit( EXIT_SUCCESS );
}

static inline void ExitOnSignal_( void )
{
#if defined( TOL__MAC ) || defined( TOL__POSIX )
	signal( SIGHUP, signal_ );
#elif defined( TOL__WIN )
	signal( SIGBREAK, signal_ );
#else
#	error "Undefined target !"
#endif

	signal( SIGTERM, signal_ );
	signal( SIGABRT, signal_ );
	signal( SIGINT, signal_ );	// Documentations about this signal not very clear, but this handles Ctrl-C.
}

Q37_GCTOR( scltool )
{
	ExitOnSignal_();
	qRRor_.Init();
	SCLError_.Init();
	ParametersTag_.Init( sclrgstry::ParametersTag );
	ParametersTag_.Append('/' );
}
