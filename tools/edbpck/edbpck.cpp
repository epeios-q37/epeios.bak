/*
	'edbpck' by Claude SIMON (claude.simon@zeusw.org)
	XML Preprocessor.
	Copyright (C) 2007-2011 Claude SIMON

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'edbpck'.

    'edbpck' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'edbpck' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'edbpck'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "clnarg.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

#include "dbpals.h"
#include "dbprcd.h"
#include "dbptbl.h"
#include "dbpctx.h"

#include "registry.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

#define NAME_MC				"eDBPck"
#define NAME_LC				"edbpck"
#define NAME_UC				"EDBPCK"
#define WEBSITE_URL			"http://zeusw.org/intl/"
#define AUTHOR_NAME			"Claude SIMON"
#define AUTHOR_CONTACT		"http://zeusw.org/intl/contact.html"
#define OWNER_NAME			"Claude SIMON"
#define OWNER_CONTACT		"http://zeusw.org/intl/contact.html"
#define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	

typedef bso::uint__		id__;
#define ALL				BSO_UINT_MAX
#define UNDEFINED		ALL

#define _( name )	#name

using namespace registry;

/*
Ne pas modifier ; utilisé pour la génération automatique.
END
*/
/* Beginning of the part which handles command line arguments. */

enum exit_value__ {
	evSuccess = EXIT_SUCCESS,
	evGenericFailure = EXIT_FAILURE,
	// Erreur dans les paramètres d'entrée.
	evParameters,
	// Erreur lors de l'ouverture des fichiers d'entrée ou de sortie.
	evInputOutput,
	// Erreur lors du traitement.
	evProcessing,
	ev_amount
};

enum command__ {
	cProcess = scltool::c_amount,
	c_amount,
	c_Undefined
};

enum option {
};

#define STRING_PARAM___( name )	CLNARG_STRING_PARAM___( name )

struct parameters___ {
	STRING_PARAM___( Project );
	id__ Id;
	parameters___( void )
	{
		Id = 0;	// By default, a random record is displayed.
	}
};

static void PrintUsage_( const clnarg::description_ &Description )
{
ERRProlog
	STR_BUFFER___ TBuffer;
//	CLNARG_BUFFER__ Buffer;
	lcl::meaning Meaning;
	str::string Translation;
ERRBegin
	scltool::PrintDefaultCommandDescriptions( NAME_LC, Description );

	COut << NAME_LC << " <project-file> [id]";
	COut << txf::nl;
	Translation.Init();
	COut << txf::pad << scltool::GetTranslation( "PickCommandDescription", Translation ) << '.' << txf::nl;

	COut << txf::nl;

	COut << txf::pad << "project-file :" << txf::nl;
	COut << txf::tab;
	Translation.Init();
	COut << scltool::GetTranslation( "ProjectFileArgumentDescription", Translation ) << '.' << txf::nl;

	COut << txf::pad << "id :" << txf::nl;
	COut << txf::tab;
	Translation.Init();
	COut << scltool::GetTranslation( "IdArgumentDescription", Translation ) << '.' << txf::nl;


#if 0	// Exemples.
	// Commands.
	COut << NAME << " [" << Description.GetCommandLabels( cProcess, Buffer );
	COut << "] [" << Description.GetOptionLabels( oNamespace, Buffer ) << " <ns>]";
	COut << " [" << Description.GetOptionLabels( oNoIndent, Buffer );
	COut << "] [<src> [<dst>]]";
	COut << txf::nl;
	Translation.Init();
	COut << txf::pad << locale::GetProcessCommandDescriptionTranslation( Translation ) << '.' << txf::nl;

	COut << NAME << ' ' << Description.GetCommandLabels( cEncrypt, Buffer );
	COut << " [" << Description.GetOptionLabels( oNamespace, Buffer ) << " <ns>]";
	COut << " [" << Description.GetOptionLabels( oNoIndent, Buffer );
	COut << "] [<src> [<dst>]]";
	COut << txf::nl;
	Translation.Init();
	COut << txf::pad << locale::GetEncryptCommandDescriptionTranslation( Translation ) << '.' << txf::nl;

	COut << txf::nl;

// Options.
	Meaning.Init();
	clnarg::GetOptionsWordingMeaning( Meaning );
	Translation.Init();
	COut << scllocale::GetLocale().GetTranslation( Meaning, scltool::GetLanguage(), Translation ) << " :" << txf::nl;

	COut << txf::pad << Description.GetOptionLabels( oNamespace, Buffer ) << " <ns> :" << txf::nl;
	COut << txf::tab;
	Translation.Init();
	COut << locale::GetNamespaceOptionDescriptionTranslation( Translation ) << '.' << txf::nl;

	COut << txf::pad << Description.GetOptionLabels( oNoIndent, Buffer ) << " :" << txf::nl;
	COut << txf::tab;
	Translation.Init();
	COut << locale::GetNoIndentOptionDescriptionTranslation( Translation ) << '.' << txf::nl;

	COut << txf::nl;

// Arguments.
	Meaning.Init();
	clnarg::GetArgumentsWordingMeaning( Meaning );
	Translation.Init();
	COut << scllocale::GetLocale().GetTranslation( Meaning, scltool::GetLanguage(), Translation ) << " :" << txf::nl;

	COut << txf::pad << "<src> :" << txf::nl;
	COut << txf::tab;
	COut << locale::GetSourceFileArgumentDescriptionTranslation( Translation ) << '.' << txf::nl;

	COut << txf::pad << "<dst> :" << txf::nl;
	COut << txf::tab;
	COut << locale::GetDestFileArgumentDescriptionTranslation( Translation ) << '.' << txf::nl;
#endif

ERRErr
ERREnd
ERREpilog
}

static void PrintHeader_( void )
{
	COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
	COut << "Copyright " COPYRIGHT << txf::nl;
	COut << txf::pad << "Build : "__DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
}

static void AnalyzeOptions_(
	clnarg::analyzer___ &Analyzer,
	parameters___ &Parameters )
{
ERRProlog
	sdr::row__ P;
	clnarg::option_list Options;
	clnarg::id__ Option;
	const bso::char__ *Unknown = NULL;
	clnarg::argument Argument;
//	clnarg::buffer__ Buffer;
ERRBegin
	Options.Init();

	if ( ( Unknown = Analyzer.GetOptions( Options ) ) != NULL )
		scltool::ReportUnknownOptionErrorAndAbort( Unknown );

	P = Options.First();

	while( P != E_NIL ) {
		Argument.Init();

		switch( Option = Options( P ) ) {
		default:
			ERRFwk();
		}

		P = Options.Next( P );
	}

ERRErr
ERREnd
ERREpilog
}

static void AnalyzeFreeArguments_(
	clnarg::analyzer___ &Analyzer,
	parameters___ &Parameters )
{
ERRProlog
	clnarg::arguments Free;
	sdr::row__ P = E_NIL, Error = E_NIL;
	id__ Id = 0;
	lcl::meaning Meaning;
ERRBegin
	Free.Init();

	Analyzer.GetArguments( Free );

	P = Free.Last();

	switch( Free.Amount() ) {
	case 2:
		Id = Free( P ).ToUInt( &Error );

		if ( Error != E_NIL ) {
			Meaning.Init();
			Meaning.SetValue( _( BadIdError ) );
			scltool::ReportAndAbort( Meaning );
		}

		Parameters.Id = ( Id == 0 ? ALL : Id );
		P = Free.Previous( P );
	case 1:
		Free( P ).Convert( Parameters.Project );
		break;
	case 0:
	default:
		scltool::ReportWrongNumberOfArgumentsErrorAndAbort();
		break;
	}

ERRErr
ERREnd
ERREpilog
}

static command__ AnalyzeArgs_(
	int argc,
	const char *argv[],
	parameters___ &Parameters )
{
	clnarg::id__ Command = c_Undefined;
ERRProlog
	clnarg::description Description;
	clnarg::analyzer___ Analyzer;
ERRBegin
	Description.Init();

	scltool::AddDefaultCommands( Description );

//	Description.AddCommand( '', "", c );
//	Description.AddOption( '', "", o );

	Analyzer.Init( argc, argv, Description );

	switch ( Command = (command__)Analyzer.GetCommand() ) {
	case scltool::cVersion:
		PrintHeader_();
//		TTR.Advertise( COut );
		ERRAbort();
		break;
	case scltool::cHelp:
		PrintUsage_( Description );
		ERRAbort();
		break;
	case scltool::cLicense:
		epsmsc::PrintLicense( COut );
		ERRAbort();
		break;
//	case c:
	case CLNARG_NONE:
//		clnarg::ReportMissingCommandError( NAME, scllocale::GetLocale(), scltool::GetLanguage() );
		break;
	default:
		ERRFwk();
		break;
	}

	AnalyzeOptions_( Analyzer, Parameters );

	AnalyzeFreeArguments_( Analyzer, Parameters );

ERRErr
ERREnd
ERREpilog
	return (command__)Command;
}

/* End of the part which handles command line arguments. */

class random_parameter_
{
public:
	struct s {
		str::string_::s Name;
		bso::uint__ Limit;
	} &S_;
	str::string_ Name;
	random_parameter_( s &S )
	: S_( S ),
	  Name( S.Name )
	{}
	void reset( bso::bool__ P = true )
	{
		Name.reset( P );
		S_.Limit = 0;
	}
	void plug( sdr::E_SDRIVER__ &SD )
	{
		Name.plug( SD );
	}
	void plug( ags::E_ASTORAGE_ &AS )
	{
		Name.plug( AS );
	}
	random_parameter_ &operator =( const random_parameter_ &RP )
	{
		Name = RP.Name;
		S_.Limit = RP.S_.Limit;

		return *this;
	}
	void Init( void )
	{
		Name.Init();
		S_.Limit = 0;
	}
	void Init(
		const str::string_ &Name,
		bso::uint__ Limit )
	{
		this->Name.Init( Name );
		S_.Limit = Limit;
	}
	E_RODISCLOSE_( bso::uint__, Limit );
};

E_AUTO( random_parameter );

typedef ctn::E_MCONTAINER_( random_parameter_ ) random_parameters_;
E_AUTO( random_parameters );

static bso::uint__ GetRandomLimit_( const str::string_ &Name )
{
	bso::uint__ Limit = 0;
ERRProlog
	rgstry::tags Tags;
ERRBegin
	Tags.Init();
	Tags.Append( Name );

	Limit = scltool::GetMandatoryUInt( rgstry::tentry__( TaggedRandomLimit, Tags ), RAND_MAX );
ERRErr
ERREnd
ERREpilog
	return Limit;
}

static void GetRandoms_(
	const str::strings_ &Names,
	random_parameters_ &Randoms )
{
ERRProlog
	random_parameter Random;
	ctn::E_CMITEM( str::string_ ) Name;
	sdr::row__ Row = E_NIL;
ERRBegin
	Name.Init( Names );

	Row = Names.First();

	while ( Row != E_NIL ) {
		Random.Init( Name( Row ), GetRandomLimit_( Name( Row ) ) );
		Randoms.Append( Random );

		Row = Names.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

static void GetRandoms_( random_parameters_ &Randoms )
{
ERRProlog
	str::strings Names;
ERRBegin
	Names.Init();
	scltool::GetValues( RandomName, Names );

	GetRandoms_( Names, Randoms );
ERRErr
ERREnd
ERREpilog
}


using namespace dbpals;
using namespace dbprcd;
using namespace dbptbl;

typedef tables_	data_;
E_AUTO( data )

#define NAMESPACE	NAME_LC ":"

#define DATA_TAG	NAMESPACE "data"

#define TABLE_TAG				NAMESPACE "table"
#define TABLE_LABEL_ATTRIBUTE	"label"

#define ALIASES_TAG	NAMESPACE "aliases"

#define ALIAS_TAG									NAMESPACE "alias"
#define ALIAS_LABEL_ATTRIBUTE						"label"
#define ALIAS_TABLE_LABEL_ATTRIBUTE					"TableLabel"
#define ALIAS_TABLE_ALIAS_ATTRIBUTE					"TableAlias"
#define ALIAS_RECORD_LABEL_ATTRIBUTE				"RecordLabel"

#define CONTENT_TAG									NAMESPACE "content"
#define CONTENT_DEFAULT_RECORD_LABEL_TAG_ATTRIBUTE	"DefaultRecordLabelTag"

#define RECORD_LABEL_ATTRIBUTE					NAMESPACE "label"
#define RECORD_WEIGHT_ATTRIBUTE					NAMESPACE "Weight"
#define RECORD_HANDLING_ATTRIBUTE				NAMESPACE "Handling"
#define RECORD_HANDLING_ATTRIBUTE_IGNORE_VALUE	"Ignore"
#define RECORD_HANDLING_ATTRIBUTE_SKIP_VALUE	"Skip"

#define INSERTION_TAG						NAMESPACE "insert"
#define INSERTION_TABLE_LABEL_ATTRIBUTE		"TableLabel"
#define INSERTION_RECORD_LABEL_ATTRIBUTE	"RecordLabel"
#define INSERTION_TABLE_ALIAS_ATTRIBUTE		"TableAlias"
#define INSERTION_RECORD_ALIAS_ATTRIBUTE	"RecordAlias"


static bso::bool__ BelongsToNamespace_( const str::string_ &Name )
{
	return str::Compare( Name, str::string( NAMESPACE ), 0, 0, strlen( NAMESPACE ) ) == 0;
}

static void ReportAndExit_( const xpp::preprocessing_iflow___ &IFlow )
{
ERRProlog
	lcl::meaning Error, XPPError;
ERRBegin
	Error.Init();
	Error.SetValue( "GenericError" );

	XPPError.Init();
	xpp::GetMeaning( IFlow, XPPError );

	Error.AddTag( XPPError );

	sclerror::SetMeaning( Error );

	ERRAbort();
ERRErr
ERREnd
ERREpilog
}

static void GetMeaning_(
	const xpp::preprocessing_iflow___ &IFlow,
	lcl::meaning_ &Meaning )
{
ERRProlog
	xpp::context___ Context;
ERRBegin
	Context.Init();
	IFlow.GetContext( Context );

	if ( Context.Status != xpp::sOK )
		ERRFwk();

	xpp::GetMeaning( Context.Coordinates, Meaning );
ERRErr
ERREnd
ERREpilog
}

static void ReportAndExit_( 
	const lcl::meaning_ &Meaning,
	const xpp::preprocessing_iflow___ &IFlow )

{
ERRProlog
	lcl::meaning GenericMeaning;
	lcl::meaning MeaningBuffer;
ERRBegin
	GenericMeaning.Init();
	GenericMeaning.SetValue( _( GenericError ) );

//	GenericMeaning.AddTag( Meaning );
	
	MeaningBuffer.Init();
	GetMeaning_( IFlow, MeaningBuffer );
	MeaningBuffer.AddTag( Meaning );

	GenericMeaning.AddTag( MeaningBuffer );

	scltool::ReportAndAbort( GenericMeaning );
ERRErr
ERREnd
ERREpilog
}

static void ReportAndExit_( 
	const char *Error,
	const xpp::preprocessing_iflow___ &IFlow )

{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();
	Meaning.SetValue( Error );

	ReportAndExit_( Meaning, IFlow );
ERRErr
ERREnd
ERREpilog
}

static void ReportAndExit_(
	const char *Error,
	const xpp::preprocessing_iflow___ &IFlow,
	const char *Tag )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();
	Meaning.SetValue( Error );

	Meaning.AddTag( Tag );

	ReportAndExit_( Meaning, IFlow );
ERRErr
ERREnd
ERREpilog
}

static void ReportAndExit_(
	const char *Error,
	const xpp::preprocessing_iflow___ &IFlow,
	const char *Tag1,
	const char *Tag2 )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();
	Meaning.SetValue( Error );

	Meaning.AddTag( Tag1 );
	Meaning.AddTag( Tag2 );

	ReportAndExit_( Meaning, IFlow );
ERRErr
ERREnd
ERREpilog
}

static void Insert_(
	const str::string_ &RecordLabel,
	const records_ &Records,
	xpp::preprocessing_iflow___ &IFlow,
	record_ &Record )

{
	rrow__ Row = E_NIL;

	Row = dbprcd::SearchRecord( RecordLabel, Records );

	if ( Row == E_NIL )
		ReportAndExit_( _( RecordNotFoundError ), IFlow );

	dbprcd::Insert( Row, Records, Record );
}

static void InsertUsingRecordAlias_(
	const str::string_ &RecordAlias,
	const record_aliases_ &Aliases,
	const tables_ &Tables,
	xpp::preprocessing_iflow___ &IFlow,
	record_ &Record )
{
	sdr::row__ Row = E_NIL;

	Row = FindRecordAlias_( RecordAlias, Aliases );

	if ( Row == E_NIL )
		ReportAndExit_( _( RecordAliasNotFoundError ), IFlow );

	Insert( Row, Aliases, Tables, Record );
}

static void Insert_(
	const str::string_ &RecordLabel,
	const table_ &Table,
	xpp::preprocessing_iflow___ &IFlow,
	record_ &Record )
{
	Insert_( RecordLabel, Table.Records, IFlow, Record );
}

static void Insert_(
	const str::string_ &RecordLabel,	
	trow__ TableRow,
	const tables_ &Tables,
	xpp::preprocessing_iflow___ &IFlow,
	record_ &Record )
{
	ctn::E_CITEMt( table_, trow__ )Table;

	Table.Init( Tables );

	Insert_( RecordLabel, Table( TableRow ), IFlow, Record );
}

static void Insert_(
	const str::string_ &RecordLabel,
	sdr::row__ AliasRow,
	const table_aliases_ &Aliases,
	const tables_ &Tables,
	xpp::preprocessing_iflow___ &IFlow,
	record_ &Record )
{
	ctn::E_CMITEM( table_alias_ ) Alias;

	Alias.Init( Aliases );

	Insert_( RecordLabel, Alias( AliasRow ).TableRow(), Tables, IFlow, Record );
}

static void InsertUsingTableAlias_(
	const str::string_ &TableAlias,
	const str::string_ &RecordLabel,
	const table_aliases_ &Aliases,
	const tables_ &Tables,
	xpp::preprocessing_iflow___ &IFlow,
	record_ &Record )
{
	sdr::row__ Row = E_NIL;

	Row = FindTableAlias_( TableAlias, Aliases );

	if ( Row == E_NIL )
		ReportAndExit_( _( TableAliasNotFoundError ), IFlow );

	Insert_( RecordLabel, Row, Aliases, Tables, IFlow, Record );
}

static void InsertUsingLabels_(
	const str::string_ &TableLabel,
	const str::string_ &RecordLabel,
	const tables_ &Tables,
	xpp::preprocessing_iflow___ &IFlow,
	record_ &Record )
{
	trow__ Row = E_NIL;
	ctn::E_CITEMt( table_, trow__ ) Table;

	Row = SearchTable( TableLabel, Tables );

	if ( Row == E_NIL )
		ReportAndExit_( _( TableNotFoundError ), IFlow );

	Table.Init( Tables );

	Insert_( RecordLabel, Table( Row ), IFlow, Record );
}

static void Assign_(
	const char *TargetWording,
	xml::parser___ &Parser,
	xpp::preprocessing_iflow___ &IFlow,
	str::string_ &Target )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	if ( Target.Amount() != 0 ) {
		Meaning.Init();
		Meaning.SetValue( _( ItemAlreadyDefinedError ) );
		Meaning.AddTag( TargetWording );
		ReportAndExit_( Meaning, IFlow );
	}

	if ( Parser.Value().Amount() == 0 ) {
		Meaning.Init();
		Meaning.SetValue( _( ItemValueEmptyError ) );
		Meaning.AddTag( TargetWording );
		ReportAndExit_( Meaning, IFlow );
	}

	Target = Parser.Value();

ERRErr
ERREnd
ERREpilog
}

static inline bso::bool__ Test_( const str::string_ &Value )
{
	return Value.Amount() != 0;
}

static void ReportInsertionErrorAndExit_(
	const char *Item1Wording,
	const char *Item2Wording,
	xpp::preprocessing_iflow___ &IFlow )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();

	Meaning.SetValue( _( ItemsDefinedTogetherError ) );

	Meaning.AddTag( Item1Wording );
	Meaning.AddTag( Item2Wording );

	ReportAndExit_( Meaning, IFlow );
ERRErr
ERREnd
ERREpilog
}

// '...<erpck:insert ...>...' -> '...</erpck:insert>...'
//                   ^                              ^
static void ProcessInsertion_(
	xml::parser___ &Parser,
	xpp::preprocessing_iflow___ &IFlow,
	const aliases_ &Aliases,
	const tables_ &Tables,
	record_ &Record )
{
ERRProlog
	str::string TableLabel, RecordLabel, TableAlias, RecordAlias;
	bso::bool__ Continue = true;
ERRBegin
	TableLabel.Init();
	RecordLabel.Init();
	TableAlias.Init();
	RecordAlias.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTag |xml::tfAttribute | xml::tfValue | xml::tfEndTag ) ) {
		case xml::tStartTag:
			ReportAndExit_( _( TagNotAllowedHereError ), IFlow );
			break;
		case xml::tAttribute:
			if ( Parser.AttributeName() == INSERTION_TABLE_LABEL_ATTRIBUTE ) {
				Assign_( _( TableLabelWording ), Parser, IFlow, TableLabel );
			} else if ( Parser.AttributeName() == INSERTION_RECORD_LABEL_ATTRIBUTE ) {
				Assign_( _( RecordLabelWording ), Parser, IFlow, RecordLabel );
			} else if ( Parser.AttributeName() == INSERTION_TABLE_ALIAS_ATTRIBUTE ) {
				Assign_( _( TableAliasWording ), Parser, IFlow, TableAlias );
			} else if ( Parser.AttributeName() == INSERTION_RECORD_ALIAS_ATTRIBUTE ) {
				Assign_( _( RecordAliasWording ), Parser, IFlow, RecordAlias );
			} else
				ReportAndExit_( _( UnknownAttributeError ), IFlow );
			break;
		case xml::tEndTag:
		{
			bso::bool__ 
				TA = Test_( TableAlias ),
				TL = Test_( TableLabel ),
				RA = Test_( RecordAlias ),
				RL = Test_( RecordLabel );

			if ( TA )
				if ( TL )
					ReportInsertionErrorAndExit_( _( TableAliasWording ), _( TableLabelWording ), IFlow );
				else if ( RA )
					ReportInsertionErrorAndExit_( _( TableAliasWording ), _( RecordAliasWording ), IFlow );
				else if ( !RL )
					ReportAndExit_( _( MissingRecordLabelError ), IFlow );
				else
					InsertUsingTableAlias_( TableAlias, RecordLabel, Aliases.Tables, Tables, IFlow, Record );
			else if ( TL )
				if ( RA )
					ReportInsertionErrorAndExit_( _( TableLabelWording ), _( RecordAliasWording ), IFlow );
				else if ( !RL )
					ReportAndExit_( _( MissingRecordLabelError ), IFlow );
				else
					InsertUsingLabels_( TableLabel, RecordLabel, Tables, IFlow, Record );
			else if ( RL )
				if ( RA )
					ReportInsertionErrorAndExit_( _( RecordLabelWording) , _( RecordAliasWording ), IFlow );
				else
					ERRFwk();	// Normally already handled before.
			else if ( RA )
				InsertUsingRecordAlias_( RecordAlias, Aliases.Records, Tables, IFlow, Record );
			else
				ReportAndExit_( _( MissingInsertionParametersError ), IFlow );

			Continue = false;
			break;
		}
		case xml::tValue:
			ReportAndExit_( _( ValueNotAllowedHereError ), IFlow );
			break;
		case xml::t_Error:
			ReportAndExit_( IFlow );
			break;
		default:
			ERRFwk();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
}

// '...<ercp:content ...>...<TAG ...>...' -> '...</TAG>...'
//                                   ^                 ^
static void ProcessRecord_(
	xml::parser___ &Parser,
	xpp::preprocessing_iflow___ &IFlow,
	const str::string_ &DefaultRecordLabelTag,
	const aliases_ &Aliases,
	const tables_ &Tables,
	record_ &Record )
{
	bso::uint__ Level = 0;
	bso::bool__ Continue = true;

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTag | xml::tfAttribute | xml::tfValue | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( BelongsToNamespace_( Parser.TagName() ) ) {
				if ( Parser.TagName() == INSERTION_TAG )
					ProcessInsertion_( Parser, IFlow, Aliases, Tables, Record );	// '...<erpck:insert ...
				else																//                   ^
					ReportAndExit_( _( ForbiddenTagError ), IFlow );
			} else {
				Level++;
				Record.Content.Append( Parser.DumpData() );
			}
			break;
		case xml::tAttribute:
			if ( BelongsToNamespace_( Parser.AttributeName() ) )
				ReportAndExit_( _( AttributeNotAllowedHereError ), IFlow );

			Record.Content.Append( Parser.DumpData() );
			break;
		case xml::tValue:
			if ( Level == 0 )
				ReportAndExit_( _( MissingTagError ), IFlow );

			if ( Record.Label.Amount() == 0 )
				if ( Parser.TagName() == DefaultRecordLabelTag )
					Record.Label = Parser.Value();

			Record.Content.Append( Parser.DumpData() );
			break;
		case xml::tEndTag:
			switch ( Level ) {
			case 0:
				Continue = false;
				break;
			default:
				Level--;
				break;
			}
			Record.Content.Append( Parser.DumpData() );
			break;
		case xml::t_Error:
			ReportAndExit_( IFlow );
			break;
		default:
			ERRFwk();
			break;
		}
	}
}

// '...<ercp:content ...><...' -> '...</erpck:content>...'
//                       ^                            ^
static void ProcessRecords_(
	xml::parser___ &Parser,
	xpp::preprocessing_iflow___ &IFlow,
	const str::string_ &DefaultRecordLabelTag,
	const aliases_ &Aliases,
	const tables_ &Tables,
	records_ &Records,
	dbptbl::counter__ &Skipped )
{
ERRProlog
	bso::bool__ Continue = true;
	record Record;
	weight__ Weight = DEFAULT_WEIGHT;
	bso::integer_buffer__ Buffer;
	bso::bool__ Ignore = false;
ERRBegin
	Record.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTag | xml::tfStartTagClosed | xml::tfAttribute | xml::tfEndTag ) ) {
		case xml::tStartTag:	// Mandatory, otherwise the tag name is missed when there is an attribute.
			Record.Content.Append( Parser.DumpData() );
			break;
		case xml::tAttribute:
			if ( Parser.AttributeName() == RECORD_WEIGHT_ATTRIBUTE ) {
				sdr::row__ Error = E_NIL;

				Weight = Parser.Value().ToU8( &Error );

				if ( Error != E_NIL )
					ReportAndExit_( _( BadAttributeValueError ), IFlow );
			} else if ( Parser.AttributeName() == RECORD_LABEL_ATTRIBUTE ) {
				Assign_( _( RecordLabelWording ), Parser, IFlow, Record.Label );
			} else if ( Parser.AttributeName() == RECORD_HANDLING_ATTRIBUTE ) {
				if ( Parser.Value() == RECORD_HANDLING_ATTRIBUTE_SKIP_VALUE ) {
					if ( Skipped == DBPBSC_COUNTER_MAX )
						ERRLmt();
					Skipped++;

					Record.Skip() = true;
				}else if ( Parser.Value() == RECORD_HANDLING_ATTRIBUTE_IGNORE_VALUE )
					Ignore = true;
				else
					ReportAndExit_( _( UnknownAttributeValueError ), IFlow );
			} else if ( BelongsToNamespace_( Parser.AttributeName() ) ) {
				ReportAndExit_( _( UnknownAttributeError ), IFlow );
			} else
				Record.Content.Append( Parser.DumpData() );
			break;
		case xml::tStartTagClosed:
			Record.Content.Append( " id=\"" );
			Record.Content.Append( bso::Convert( Records.Amount() + 1, Buffer ) );
			Record.Content.Append( '"' );
			Record.Content.Append( Parser.DumpData() );
			ProcessRecord_( Parser, IFlow, DefaultRecordLabelTag, Aliases, Tables, Record );	// '...<ercp:content ...>...<TAG ...>...' -> '...</TAG>...'
			if ( !Ignore )
				Records.Append( Record );														//                                   ^                 ^	
			else
				Ignore = false;
			Record.Init();
			break;														
		case xml::tEndTag:
			Continue = false;	// '...<ercp:content ...></ercp:content>...'
			break;				//                                      ^
		case xml::t_Error:
			ReportAndExit_( IFlow );
			break;
		default:
			ERRFwk();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
}

// '...<erpck::table ...>...<erpck:content ...>...' -> '...</erpck:content>...'
//                                         ^                               ^
static void ProcessContent_(
	xml::parser___ &Parser,
	xpp::preprocessing_iflow___ &IFlow,
	table_ &Table,
	const tables_ &Tables,
	const aliases_ &Aliases )
{
ERRProlog
	bso::bool__ Continue = true;
	str::string DefaultRecordLabelTag;
ERRBegin
	DefaultRecordLabelTag.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfAttribute | xml::tfStartTagClosed | xml::tfEndTag ) ) {
		case xml::tAttribute:
			if ( Parser.TagName() != CONTENT_TAG )
				ERRFwk();

			if ( Parser.AttributeName() == CONTENT_DEFAULT_RECORD_LABEL_TAG_ATTRIBUTE ) {
				Assign_( _( DefaultRecordLabelTagWording ), Parser, IFlow, DefaultRecordLabelTag );
			} else
				ReportAndExit_( _( UnknownAttributeError ), IFlow );
			break;
		case xml::tStartTagClosed:
			if ( Parser.TagName() == CONTENT_TAG ) {
				ProcessRecords_( Parser, IFlow, DefaultRecordLabelTag, Aliases, Tables, Table.Records, Table.Skipped() );	// '<ercp:content ...><...' -> '</erpck:content>...'
				Continue = false;
			}  else																						        			//                    ^                         ^
				ERRFwk();
			break;
		case xml::tEndTag:
			Continue = false;	// '</erpck:content>...'
			break;				//                  ^
		case xml::t_Error:
			ReportAndExit_( IFlow );
			break;
		default:
			ERRFwk();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
}

enum alias_type__ {
	atRecord,
	atTable,
	at_amount,
	at_Undefined
};
// '...<erpck:alias ...>...' -> '...</alias>...'
//                  ^                       ^
static alias_type__ ProcessAlias_(
	xml::parser___ &Parser,
	xpp::preprocessing_iflow___ &IFlow,
	const table_aliases_ &TableAliases,
	const tables_ &Tables,
	record_alias_ &RecordAlias,
	table_alias_ &TableAlias )
{
	alias_type__ AliasType = at_Undefined;
ERRProlog
	bso::bool__ Continue = true;
	trow__ TableRow = E_NIL;
	rrow__ RecordRow = E_NIL;
	str::string TableAliasLabel, TableLabel, RecordLabel, AliasLabel;
ERRBegin
	TableAliasLabel.Init();
	TableLabel.Init();
	RecordLabel.Init();
	AliasLabel.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTag | xml::tfStartTagClosed | xml::tfAttribute | xml::tfValue | xml::tfEndTag ) ) {
		case xml::tStartTag:
			ReportAndExit_( _( TagNotAllowedHereError ), IFlow );
			break;
		case xml::tAttribute:
			if ( Parser.AttributeName() == ALIAS_TABLE_ALIAS_ATTRIBUTE ) {
				if ( TableLabel.Amount() != 0 )
					ReportAndExit_( _( CanNotBeUsedTogetherError ), IFlow, ALIAS_TABLE_LABEL_ATTRIBUTE,  ALIAS_TABLE_ALIAS_ATTRIBUTE );

				Assign_( _( TableAliasWording ), Parser, IFlow, TableAliasLabel );
			} else if ( Parser.AttributeName() == ALIAS_TABLE_LABEL_ATTRIBUTE ) {
				if ( TableAliasLabel.Amount() != 0 )
					ReportAndExit_( _( CanNotBeUsedTogetherError ), IFlow, ALIAS_TABLE_LABEL_ATTRIBUTE,  ALIAS_TABLE_ALIAS_ATTRIBUTE );

				Assign_( _( TableAliasWording ), Parser, IFlow, TableLabel );
			} else if ( Parser.AttributeName() == ALIAS_RECORD_LABEL_ATTRIBUTE ) {
				Assign_( _( RecordLabelWording ), Parser, IFlow, RecordLabel );
			} else if ( Parser.AttributeName() == ALIAS_LABEL_ATTRIBUTE ) {
				Assign_( _( AliasLabelWording ), Parser, IFlow, AliasLabel );
			} else
				ReportAndExit_( _( UnknownAttributeError ), IFlow );
			break;
		case xml::tStartTagClosed:
			if ( TableLabel.Amount() != 0 ) {
				if ( ( TableRow = SearchTable( TableLabel, Tables ) ) == E_NIL )
					ReportAndExit_( _( TableNotFoundError ), IFlow );
			} else if ( TableAliasLabel.Amount() != 0 ) {
				if ( ( TableRow = SearchTable( TableAliasLabel, TableAliases ) ) == E_NIL )
					ReportAndExit_( _( TableNotFoundError ), IFlow );
			} else
				ReportAndExit_( _( MissingTableReferenceError ), IFlow );

			if ( AliasLabel.Amount() == 0 )
				ReportAndExit_( _( MissingAliasLabelError ), IFlow );

			if ( RecordLabel.Amount() ) {
				if ( ( RecordRow = SearchRecord( RecordLabel, TableRow, Tables ) ) == E_NIL )
					ReportAndExit_( _( RecordNotFoundError ), IFlow );

				AliasType = atRecord;
			} else
				AliasType = atTable;

			switch ( AliasType ) {
			case atRecord:
				RecordAlias.Init( AliasLabel, TableRow, RecordRow );
				break;
			case atTable:
				TableAlias.Init( AliasLabel, TableRow );
				break;
			break;
			default:
				ERRFwk();
			}
			break;
		case xml::tValue:
			ReportAndExit_( _( ValueNotAllowedHereError ), IFlow );
			break;
		case xml::tEndTag:
			switch ( AliasType ) {
			case atRecord:
			case atTable:
				Continue = false;
				break;
			case at_Undefined:
				ReportAndExit_( _( IncompleteAliasDefinitionError ), IFlow );
				break;
			default:
				ERRFwk();
				break;
			}
			break;
		case xml::t_Error:
			ReportAndExit_( IFlow );
			break;
		default:
			ERRFwk();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
	return AliasType;
}

 // '<erpck:table ...>...<erpck:aliases ...>...'
//                                     ^
 static void ProcessAliases_(
	xml::parser___ &Parser,
	xpp::preprocessing_iflow___ &IFlow,
	const tables_ &Tables,
	aliases_ &Aliases )
{
ERRProlog
	bso::bool__ Continue = true;
	record_alias RecordAlias;
	table_alias TableAlias;
ERRBegin
	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTag | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == ALIAS_TAG ) {
				RecordAlias.Init();
				TableAlias.Init();

				switch ( ProcessAlias_( Parser, IFlow, Aliases.Tables, Tables, RecordAlias, TableAlias ) ) {	// '...<erpck:alias ...>...' -> '...</alias>...'
				case atRecord:																					//                  ^                       ^
					Aliases.Records.Append( RecordAlias );
					break;
				case atTable:
					Aliases.Tables.Append( TableAlias );
					break;
				default:
					ERRFwk();
					break;
				}
			} else
				ReportAndExit_( _( UnknownTagError ), IFlow );
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::t_Error:
			ReportAndExit_( IFlow );
			break;
		default:
			ERRFwk();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
}

// '...<erpck:data><erpck::table ...>' -> '..</erpck:table>...'
//                               ^                         ^
static void ProcessTable_(
	xml::parser___ &Parser,
	xpp::preprocessing_iflow___ &IFlow,
	table_ &Table,
	const tables_ &Tables )
{
ERRProlog
	bso::bool__ Continue = true;
	aliases Aliases;
ERRBegin
	Aliases.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTag | xml::tfStartTagClosed | xml::tfAttribute | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == ALIASES_TAG )
				ProcessAliases_( Parser, IFlow, Tables, Aliases );	// '<erpck:table ...>...<erpck:aliases ...>...'
			else if ( Parser.TagName() == CONTENT_TAG )			//                                     ^
				ProcessContent_( Parser, IFlow, Table, Tables, Aliases );	// '<erpck:table ...>...<erpck:content ...>...' -> '...</erpc:content>...'
			else															//                                     ^                              ^
				ReportAndExit_( _( UnknownTagError ), IFlow );
			break;
		case xml::tStartTagClosed:
			if ( Table.Label.Amount() == 0 )
				ReportAndExit_( _( MissingTableLabelError ), IFlow );
			break;
		case xml::tAttribute:
			if ( Parser.TagName() != TABLE_TAG )
				ERRFwk();

			if ( Parser.AttributeName() == TABLE_LABEL_ATTRIBUTE ) {
				Assign_( _( TableLabelWording ), Parser, IFlow, Table.Label );
			} else
				ReportAndExit_( _( UnknownAttributeError ), IFlow );
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::t_Error:
			ReportAndExit_( IFlow );
			break;
		default:
			ERRFwk();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
}

// '...<erpck:data><erpck:table ...>' -> '...</erpck:table>...'
//                 ^                                       ^
static void ProcessData_(
	xml::parser___ &Parser,
	xpp::preprocessing_iflow___ &IFlow,
	data_ &Data )
{
ERRProlog
	bso::bool__ Continue = true;
	bso::bool__ TableDetected = false;
	table Table;
ERRBegin
	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTag | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == TABLE_TAG ) {
				TableDetected = true;
				Table.Init();
				ProcessTable_( Parser, IFlow, Table, Data );	// '...<erpck::table ...><erpck:content>...' -> '....</erpck:table>...'
				Data.Append( Table );							//                   ^                                             ^
			} else
				ReportAndExit_( _( MissingTableTagError ), IFlow, TABLE_TAG );
			break;
		case xml::tEndTag:
			if ( !TableDetected ) {
				ReportAndExit_( _( NoTableTagInDataFileError ), IFlow, TABLE_TAG );
			} else
				Continue = false;
			break;
		case xml::t_Error:
			ReportAndExit_( IFlow );
			break;
		default:
			ERRFwk();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
}

static void RetrieveData_(
	const char *DataFileName,
	data_ &Data )
{
ERRProlog
	flf::file_iflow___ FFlow;
	xtf::extended_text_iflow__ XFlow;
	xpp::preprocessing_iflow___ IFlow;
	xml::parser___ Parser;
	bso::bool__ Continue = true;
	lcl::locale Locale;
	bso::bool__ DataDetected = false;
	FNM_BUFFER___ Buffer;
	xtf::extended_text_iflow__ PXFlow;
ERRBegin
	if ( FFlow.Init( DataFileName, err::hUserDefined ) != fil::sSuccess )
		sclmisc::ReportFileOpeningErrorAndAbort( DataFileName );

	XFlow.Init( FFlow, utf::f_Default );

	IFlow.Init( XFlow, xpp::criterions___( str::string( fnm::GetLocation( DataFileName, Buffer ) ) ) );

	PXFlow.Init( IFlow, utf::f_Default );

	Parser.Init( PXFlow, xml::eh_Default );

	Data.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTagClosed |xml::tfAttribute ) ) {
		case xml::tStartTagClosed:
			if ( ( Parser.TagName() == DATA_TAG ) ) {
				ProcessData_( Parser, IFlow, Data );	// '...<erpck:data><erpck:table ...>' -> '...</erpck:table>...'
				DataDetected = true;					//                 ^                                       ^
			} else {
				ReportAndExit_( _( ForbiddenTagError ), IFlow );
			} 
			break;
		case xml::tAttribute:
			ReportAndExit_( _( UnexpectedAttributeError ), IFlow );
			break;
		case xml::t_Processed:
			if ( !DataDetected )
				ReportAndExit_( _( NoDataError ), IFlow, DataFileName );
			else
				Continue = false;
			break;
		case xml::t_Error:
			ReportAndExit_( IFlow );
			break;
		default:
			ERRFwk();
			break;
		}
	}

	tol::InitializeRandomGenerator();
ERRErr
ERREnd
ERREpilog
}

static void Display_(
	const str::string_ &XML,
	xml::writer_ &Writer )
{
ERRProlog
	flx::E_STRING_IFLOW__ IFlow;
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	IFlow.Init( XML );
	XFlow.Init( IFlow, utf::f_Default );

	if ( xpp::Process( XFlow, Writer ) != xpp::sOK )
		ERRFwk();
ERRErr
ERREnd
ERREpilog
}

static void Display_(
	const record_ &Record,
	xml::writer_ &Writer )
{
	Display_( Record.Content, Writer );
}

static void Display_(
	rrow__ Row,						
	const records_ &Records,
	str::string_ &Label,
	xml::writer_ &Writer )
{
	ctn::E_CITEMt( record_, rrow__ ) Record;

	Record.Init( Records );

	Display_( Record( Row ), Writer );

	Label = Record().Label;
}

static void DisplayAll_(
	const records_ &Records,
	xml::writer_ &Writer )
{
	ctn::E_CITEMt( record_, rrow__ ) Record;
	rrow__ Row = Records.First();

	Record.Init( Records );

	while ( Row != E_NIL ) {
		Display_( Record( Row ), Writer );

		Row = Records.Next( Row );
	}
}

counter__ GetSkippedAmount_(
	dbpctx::amount__ SessionAmount,
	const rrows_ &RecordRows,
	const records_ &Records )
{
	counter__ Skipped = 0;
	sdr::row__ Row = RecordRows.Last();
	rrow__ RecordRow = E_NIL;
	ctn::E_CITEMt( record_, rrow__ ) Record;

	Record.Init( Records );

	while ( ( Row != E_NIL ) && ( SessionAmount-- ) ) {
		RecordRow = RecordRows( Row );

		if ( Records.Exists( RecordRow ) )
			if ( Record( RecordRow ).Skip() ) {
				if ( Skipped == DBPBSC_COUNTER_MAX )
					ERRLmt();
				Skipped++;
			}

		Row = RecordRows.Previous( Row );
	}

	return Skipped;

}

id__  Display_(
	id__ Id,
	counter__ Skipped,
	bso::uint__ SessionMaxDuration,
	const records_ &Records,
	str::string_ &Label,
	dbpctx::context_ &Context,
	xml::writer_ &Writer )
{
ERRProlog
	rrow__ Row = E_NIL;
	bso::integer_buffer__ Buffer;
	ctn::E_CITEMt( record_, rrow__ ) Record;
	counter__ Counter = 0;
	lcl::meaning Meaning;
ERRBegin
	Counter = Records.Amount();	// NOTA : there is at least one record.
	Record.Init( Records );

	Writer.PutAttribute( "TotalAmount", bso::Convert( Records.Amount(), Buffer ) );
	Writer.PutAttribute( "SkippedTotalAmount", bso::Convert( Skipped, Buffer ) );

	if ( Id == ALL ) {
		Writer.PutAttribute( "Amount", bso::Convert( Records.Amount(), Buffer ) );
		DisplayAll_( Records, Writer );
	} else {
		if ( Id == 0 ) {
/*
			tol::InitializeRandomGenerator();
			Row = Records.First( rand() % Records.Amount() );
*/
			do
				Row = Context.Pick( Records.Amount(), SessionMaxDuration );
			while ( Record( Row ).GetSkip() && ( --Counter ) );

			if ( !Counter ) {
				Writer.PutAttribute( "Amount", "0" );
				Row = E_NIL;
			} else if ( !Record( Row ).GetSkip() ) {
				Id = *Row + 1;
				Writer.PutAttribute( "Amount", "1" );
			}

			Writer.PutAttribute( "SessionAmount", bso::Convert( Context.S_.Session ) );
			Writer.PutAttribute( "SessionSkippedAmount", bso::Convert( GetSkippedAmount_( Context.S_.Session, Context.Pool, Records ) ) );

			Writer.PutAttribute( "CycleAmount", bso::Convert( Context.S_.Cycle ) );
			Writer.PutAttribute( "CycleSkippedAmount", bso::Convert( GetSkippedAmount_( Context.S_.Cycle, Context.Pool, Records ) ) );

		} else {
			if ( Id > Records.Amount() ) {
				Meaning.Init();
				Meaning.SetValue( _( NoRecordOfGivenIdError ) );
				Meaning.AddTag( bso::Convert( Id, Buffer ) );
				scltool::ReportAndAbort( Meaning );
			} else {
				Writer.PutAttribute( "Amount", "1" );
				Row = Id - 1;
			}
		}

		if ( Row != E_NIL )
			Display_( Row, Records, Label, Writer );
	}
ERRErr
ERREnd
ERREpilog
	return Id;
}

static id__ Display_(
	id__ Id,
	const table_ &Table,
	bso::uint__ SessionMaxDuration,
	str::string_ &Label,
	dbpctx::context_ &Context,
	xml::writer_ &Writer )
{
	Writer.PushTag( Table.Label );

	Id = Display_( Id, Table.Skipped(), SessionMaxDuration, Table.Records, Label, Context, Writer );	

	Writer.PopTag();

	return Id;
}

static void HandleRandom_(
	const random_parameter_ &Random,
	xml::writer_ &Writer )
{
	Writer.PushTag( "Random" );
	Writer.PutAttribute( "Name", Random.Name );
	xml::PutAttribute( "Limit", Random.Limit(), Writer );
	xml::PutAttribute( "Value", rand() % Random.GetLimit(), Writer );
	Writer.PopTag();
}

static void HandleRandoms_(
	const random_parameters_ &Randoms,
	xml::writer_ &Writer )
{
	ctn::E_CMITEM( random_parameter_ ) Random;
	sdr::row__ Row = E_NIL;

	if ( Randoms.Amount() == 0 )
		return;

	tol::InitializeRandomGenerator();

	Random.Init( Randoms );

	Row = Randoms.First();

	Writer.PushTag( "Parameters" );

	while ( Row != E_NIL ) {
		HandleRandom_( Random( Row ), Writer );

		Row = Randoms.Next( Row );
	}

	Writer.PopTag();
}

static id__ Display_(
	id__ Id,
	const data_ &Data,
	const str::string_ &XSLFileName,
	bso::uint__ SessionMaxDuration,
	str::string_ &Label,
	dbpctx::context_ &Context,
	txf::text_oflow__ &Output )
{
ERRProlog
	xml::writer Writer;
	ctn::E_CITEMt( table_, trow__ ) Table;
	random_parameters Randoms;
ERRBegin
	Randoms.Init();
	GetRandoms_( Randoms );

	xml::WriteXMLHeader( Output, xml::eISO_8859_1 );
	Output << txf::nl;

	if ( XSLFileName.Amount() != 0 ) {
		xml::WriteXSLDeclaration( XSLFileName, Output );
		Output << txf::nl;
	}

	Table.Init( Data );

	Writer.Init( Output, xml::oIndent, xml::e_None );

	Writer.PushTag( "Picking" );

	Writer.PushTag( "Misc" );

	Writer.PushTag( "Generator" );

	Writer.PushTag( "Name" );
	Writer.PutValue( NAME_MC );
	Writer.PopTag();

	Writer.PushTag( "Version" );
	Writer.PutValue( VERSION );
	Writer.PopTag();

	Writer.PushTag( "Author" );

	Writer.PushTag( "Name" );
	Writer.PutValue( AUTHOR_NAME );
	Writer.PopTag();

	Writer.PushTag( "Contact" );
	Writer.PutValue( AUTHOR_CONTACT );
	Writer.PopTag();

	Writer.PopTag();
	Writer.PopTag();

	Writer.PushTag( "Session" );
	Writer.PushTag( "MaxDuration" );
	Writer.PutValue( bso::Convert( SessionMaxDuration ) );
	Writer.PopTag();
	Writer.PopTag();
	Writer.PopTag();

	HandleRandoms_( Randoms, Writer );

	Writer.PushTag( "Data" );

	Id = Display_( Id, Table( Data.Last() ), SessionMaxDuration, Label, Context, Writer );

	Writer.PopTag();

	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
	return  Id;
}

static id__ DisplayWithoutBackup_(
	id__ Id,
	const data_ &Data,
	const str::string_ &XSLFileName,
	bso::uint__ SessionMaxDuration,
	str::string_ &Label,
	dbpctx::context_ &Context,
	const char *FileName )
{
ERRProlog
	flf::file_oflow___ FFlow;
	txf::text_oflow__ TFlow;
ERRBegin
	if ( FFlow.Init( FileName ) != fil::sSuccess )
		sclmisc::ReportFileOpeningErrorAndAbort( FileName );

	TFlow.Init( FFlow );

	Id = Display_( Id, Data, XSLFileName, SessionMaxDuration, Label, Context, TFlow );
ERRErr
ERREnd
ERREpilog
	return Id;
}

static id__ Display_(
	id__ Id,
	const data_ &Data,
	const str::string_ &XSLFileName,
	bso::uint__ SessionMaxDuration,
	str::string_ &Label,
	dbpctx::context_ &Context,
	const char *FileName )
{
ERRProlog
	bso::bool__ Backuped = false;
ERRBegin
	sclmisc::CreateBackupFile( FileName );

	Backuped = true;

	Id = DisplayWithoutBackup_( Id, Data, XSLFileName, SessionMaxDuration, Label, Context, FileName );
ERRErr
	if ( Backuped )
		sclmisc::RecoverBackupFile( FileName );
ERREnd
ERREpilog
	return Id;
}

static id__ Display_(
	id__ Id,
	const data_ &Data,
	const str::string_ &XSLFileName,
	bso::uint__ SessionMaxDuration,
	str::string_ &Label,
	dbpctx::context_ &Context,
	const str::string_ &OutputFileName )
{
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	if ( OutputFileName.Amount() == 0 )
		Id = Display_( Id, Data, XSLFileName, SessionMaxDuration, Label, Context, COut );
	else
		Id = Display_( Id, Data, XSLFileName, SessionMaxDuration, Label, Context, OutputFileName.Convert( Buffer ) );
ERRErr
ERREnd
ERREpilog
	return Id;
}

void LaunchCommand_(
	const str::string_ &Command,
	id__ Id,
	const str::string_ &Label,
	const str::string_ &OutputFileName )
{
ERRProlog
	str::string CompleteCommand;
	STR_BUFFER___ Buffer;
ERRBegin
	if ( ( Command.Amount() != 0 ) && ( OutputFileName.Amount() != 0 ) ) {
		CompleteCommand.Init( Command );
		str::ReplaceTag( CompleteCommand, 1, OutputFileName, '$' );
		str::ReplaceTag( CompleteCommand, 2, str::string( bso::Convert( Id ) ), '$' );
		str::ReplaceTag( CompleteCommand, 3, Label, '$' );
		COut << "Launching '" << CompleteCommand << "\'." << txf::nl << txf::commit;
		if ( system( CompleteCommand.Convert( Buffer ) ) == -1 )
			ERRLbr();
	}


ERRErr
ERREnd
ERREpilog
}

static void DumpContext_(
	const dbpctx::context_ &Context,
	xml::writer_ &Writer )
{
	Writer.PushTag( "Context" );
	Writer.PutAttribute( "Target", NAME_LC );
	Writer.PutAttribute( "TimeStamp", tol::DateAndTime() );

	Dump( Context, Writer );

	Writer.PopTag();
}

static void DumpContextWithoutBackup_(
	const dbpctx::context_ &Context,
	const char *FileName )
{
ERRProlog
	flf::file_oflow___ FFlow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
ERRBegin
	if ( FFlow.Init( FileName ) != fil::sSuccess )
		sclmisc::ReportFileOpeningErrorAndAbort( FileName );

	TFlow.Init( FFlow );

	Writer.Init( TFlow, xml::oIndent, xml::eISO_8859_1 );

	DumpContext_( Context, Writer ); 
ERRErr
ERREnd
ERREpilog
}


static void DumpContext_(
	const dbpctx::context_ &Context,
	const char *FileName )
{
ERRProlog
	bso::bool__ Backuped = false;
ERRBegin
	sclmisc::CreateBackupFile( FileName );

	Backuped = true;

	DumpContextWithoutBackup_( Context, FileName );
ERRErr
	if ( Backuped )
		sclmisc::RecoverBackupFile( FileName );
ERREnd
ERREpilog
}

static void RetrieveContext_(
	xml::parser___ &Parser,							 
	dbpctx::context_ &Context )
{
ERRProlog
	bso::bool__ Continue = true;
	str::string Target;
ERRBegin

	Target.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious | xml::tfStartTagClosed ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() != "Context" )
				ERRFwk();

			break;
		case xml::tAttribute:
			if ( Parser.AttributeName() == "Target" ) {
				if ( Target.Amount() != 0 )
					ERRFwk();

				Target = Parser.Value();
			} else if ( Parser.AttributeName() != "TimeStamp" )
				ERRFwk();
			break;
		case xml::tStartTagClosed:
			if ( Target != NAME_LC )
				ERRFwk();

			dbpctx::Retrieve( Parser, Context );
			break;
		case xml::t_Processed:
			Continue = false;
			break;
		default:
			ERRFwk();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
}

static void RetrieveContext_(
	const char *FileName,
	dbpctx::context_ &Context )
{
ERRProlog
	xml::parser___ Parser;
	flf::file_iflow___ FFlow;
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	if ( !fil::FileExists( FileName ) ) {
//		COut << "Unable to find context file '" << FileName << "'! It will be created at exit." << txf::nl;
		ERRReturn;
	}

	if ( FFlow.Init( FileName ) != fil::sSuccess )
		sclmisc::ReportFileOpeningErrorAndAbort( FileName );

	XFlow.Init( FFlow, utf::f_Default );

	Parser.Init( XFlow, xml::eh_Default );

	RetrieveContext_( Parser, Context );
ERRErr
ERREnd
ERREpilog
}

void Process_( id__ Id )
{
ERRProlog
	str::string DataFileName;
	STR_BUFFER___ Buffer;
	data Data;
	dbpctx::context Context;
	str::string OutputFileName;
	str::string XSLFileName;
	str::string Command;
	str::string ContextFileName;
	bso::uint__ SessionMaxDuration = 0;
	bso::bool__ Error = false;
	str::string Label;
ERRBegin
	DataFileName.Init();
	if ( !scltool::GetValue( registry::Data, DataFileName ) )
		scltool::ReportAndAbort( _( DataFileNotSpecifiedError ) );

	OutputFileName.Init();
	if ( !scltool::GetValue( Output, OutputFileName ) )
		scltool::ReportAndAbort( _( OutputFileNotSpecifiedError ) );

	XSLFileName.Init();
	scltool::GetValue( XSL, XSLFileName );

	ContextFileName.Init();
	if ( !scltool::GetValue( registry::Context, ContextFileName ) )
		scltool::ReportAndAbort( _( ContextFileNotSpecifiedError ) );

	Context.Init();
	RetrieveContext_( ContextFileName.Convert( Buffer ), Context );

	Data.Init();
	RetrieveData_( DataFileName.Convert( Buffer ), Data );

	SessionMaxDuration = scltool::GetUInt( registry::SessionMaxDuration, 0 );

	Label.Init();
	Id = Display_( Id, Data, XSLFileName, SessionMaxDuration, Label, Context, OutputFileName );

	Command.Init();
	scltool::GetValue( registry::Command, Command );

	DumpContext_( Context, ContextFileName.Convert( Buffer ) );

	LaunchCommand_( Command, Id, Label, OutputFileName );
ERRErr
ERREnd
ERREpilog
}

static void Process_(
	id__ Id,						
	const char *ProjectFileName )
{
	scltool::LoadProject( ProjectFileName, NAME_LC );

	Process_( Id );
}


static void Go_(
	command__ Command,
	const parameters___ &Parameters )
{
ERRProlog
ERRBegin
	Process_( Parameters.Id, Parameters.Project );
ERRErr
ERREnd
ERREpilog
}

const char *scltool::TargetName = NAME_LC;

void scltool::Main(
	int argc,
	const char *argv[] )
{
ERRProlog
	parameters___ Parameters;
	command__ Command = c_Undefined;
ERRBegin
	Command = AnalyzeArgs_( argc, argv, Parameters );

	Go_( Command, Parameters );
ERRErr
ERREnd
ERREpilog
}
