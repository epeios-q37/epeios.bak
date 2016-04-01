/*
	Copyright (C) 2010-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of dpkq.

    dpkq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    dpkq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with dpkq.  If not, see <http://www.gnu.org/licenses/>
*/

#include "dpkals.h"
#include "dpkrcd.h"
#include "dpktbl.h"
#include "dpkctx.h"

#include "registry.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

# define NAME_MC			"dpkq"
# define NAME_LC			"dpkq"
# define NAME_UC			"DPKQ"
# define WEBSITE_URL		"http://q37.info/computing/epeios/tools/dpkq/"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	

static void PrintHeader_( void )
{
	COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
	COut << "Copyright (C) " COPYRIGHT << txf::nl;
	COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
}

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
	void plug( str::cHook &Hook )
	{
		Name.plug( Hook );
	}
	void plug( qASd *AS )
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
qRH
	rgstry::tags Tags;
qRB
	Tags.Init();
	Tags.Append( Name );

	Limit = sclmisc::MGetUInt( rgstry::tentry__( registry::TaggedRandomLimit, Tags ), RAND_MAX );
qRR
qRT
qRE
	return Limit;
}

static void GetRandoms_(
	const str::strings_ &Names,
	random_parameters_ &Randoms )
{
qRH
	random_parameter Random;
	ctn::qCMITEMsl( str::string_ ) Name;
	sdr::row__ Row = qNIL;
qRB
	Name.Init( Names );

	Row = Names.First();

	while ( Row != qNIL ) {
		Random.Init( Name( Row ), GetRandomLimit_( Name( Row ) ) );
		Randoms.Append( Random );

		Row = Names.Next( Row );
	}
qRR
qRT
qRE
}

static void GetRandoms_( random_parameters_ &Randoms )
{
qRH
	str::strings Names;
qRB
	Names.Init();
	sclmisc::GetValues( registry::RandomName, Names );

	GetRandoms_( Names, Randoms );
qRR
qRT
qRE
}


using namespace dpkals;
using namespace dpkrcd;
using namespace dpktbl;

typedef tables_	data_;
E_AUTO( data )

namespace {
	str::string NamespaceLabel_;	// With tailing ':' !

	E_ENUM( disc ) {	// '...riminant'.
		dNone,	// No discriminant.
		dTable,
		dAliases,
		dAlias,
		dContent,
		dRecord,
		dInsertion,
		d_amount,
		d_Undefined
	};

#define C( disc )\
	case d##disc:\
		return #disc;\
		break;

	inline const char *GetLabel_( disc__ Disc )
	{
		switch ( Disc ) {
		case dNone:
			return "";
			break;
		C( Table );
		C( Aliases );
		C( Alias );
		C( Content );
		C( Record );
		C( Insertion );
		default:
			qRGnr();
			break;
		}

		return NULL;	// To avoid a 'warning'.
	}

#undef C

	E_ENUM( item ) {
		iDataTag,

		iTableTag,
		iTableLabelAttribute, 

		iAliasesTag,
		iAliasesLabelAttribute,
		iAliasesTableLabelAttribute,
		iAliasesTableAliasAttribute,

		iAliasTag,
		iAliasLabelAttribute,
		iAliasTableLabelAttribute,
		iAliasTableAliasAttribute,
		iAliasRecordLabelAttribute,

		iContentTag,
		iContentDefaultRecordLabelTagAttribute,

		iRecordLabelAttribute,
		iRecordHandlingAttribute,
		iRecordHandlingAttributeIgnoreValue,
		iRecordHandlingAttributeSkipValue,

		iInsertionTag,
		iInsertionTableLabelAttribute,
		iInsertionRecordLabelAttribute,
		iInsertionTableAliasAttribute,
		iInsertionRecordAliasAttribute,

		i_amount,
		i_Undefined
	};

#define A( item, value, disc )\
	case i##item:\
		PrependNamespace = false;\
		Disc = disc;\
		return value;\
		break;

#define ANS( item, value, disc )\
	case i##item:\
		PrependNamespace = true;\
		Disc = disc;\
		return value;\
		break;

	const char *GetBaseLabel_(
		item__ Item,
		bso::bool__ &PrependNamespace,
		disc__ &Disc )
	{
		switch ( Item ) {
		ANS( DataTag, "data", dNone );

		ANS( TableTag, "table", dNone );
		A( TableLabelAttribute, "label", dTable );

		ANS( AliasesTag, "aliases", dNone );
		A( AliasesLabelAttribute, "label", dAliases );
		A( AliasesTableLabelAttribute, "TableLabel", dAliases );
		A( AliasesTableAliasAttribute, "TableAlias", dAliases );

		ANS( AliasTag, "alias", dNone );
		A( AliasLabelAttribute, "label", dAlias );
		A( AliasTableLabelAttribute, "TableLabel", dAlias );
		A( AliasTableAliasAttribute, "TableAlias", dAlias );
		A( AliasRecordLabelAttribute, "RecordLabel", dAlias );

		ANS( ContentTag, "content", dNone );
		A( ContentDefaultRecordLabelTagAttribute, "DefaultRecordLabelTag", dContent );

		ANS( RecordLabelAttribute, "label", dRecord );
		ANS( RecordHandlingAttribute, "Handling", dRecord );
		A( RecordHandlingAttributeIgnoreValue, "Ignore", dNone );
		A( RecordHandlingAttributeSkipValue, "Skip", dNone );

		ANS( InsertionTag, "insert", dNone );
		A( InsertionTableLabelAttribute, "TableLabel", dInsertion );
		A( InsertionRecordLabelAttribute, "RecordLabel", dInsertion );
		A( InsertionTableAliasAttribute, "TableAlias", dInsertion );
		A( InsertionRecordAliasAttribute, "RecordAlias", dInsertion );
		default:
			qRGnr();
			break;
		}

		return NULL;	// To avoid a warning.
	}

	const char *GetLabel_( item__ Item )
	{
		static TOL_CBUFFER___ Buffer;
	qRH
		disc__ Disc = d_Undefined;
		str::string Label;
		bso::bool__ PrependNamespace = false;
		const char *BaseLabel = NULL;
	qRB
		BaseLabel = GetBaseLabel_( Item, PrependNamespace, Disc );

		Label.Init( GetLabel_( Disc ) );

		Label.Append('$');

		if ( PrependNamespace )
			Label.Append( NamespaceLabel_ );

		Label.Append( BaseLabel );

		Label.Convert( Buffer );
	qRR
	qRT
	qRE
		return Buffer;
	}

	stsfsm::automat Automat_;

	void FillAutomat_( void )
	{
		stsfsm::Fill( Automat_, i_amount, GetLabel_ );
	}

	item__ GetItemId_(
		disc__ Disc,
		const str::string_ &Pattern )
	{
		item__ Item = i_Undefined;
	qRH
		str::string NormalizedPattern;
	qRB
		NormalizedPattern.Init( GetLabel_( Disc ) );
		NormalizedPattern.Append( '$' );
		NormalizedPattern.Append( Pattern );

		Item = stsfsm::GetId( NormalizedPattern, Automat_, i_Undefined, i_amount );
	qRR
	qRT
	qRE
		return Item;
	}
}

#define _( name )	#name

typedef bso::uint__		id__;
#define ALL				BSO_UINT_MAX
#define UNDEFINED		ALL

static bso::bool__ BelongsToNamespace_( const str::string_ &Name )
{
	if ( Name.Amount() > NamespaceLabel_.Amount() )
		return str::Compare( Name, NamespaceLabel_, 0, 0, NamespaceLabel_.Amount() ) == 0;
	else
		return false;
}

static void ReportAndExit_( const xpp::preprocessing_iflow___ &IFlow )
{
qRH
	lcl::meaning Error, XPPError;
qRB
	Error.Init();
	Error.SetValue( "GenericError" );

	XPPError.Init();
	xpp::GetMeaning( IFlow, XPPError );

	Error.AddTag( XPPError );

	sclerror::SetMeaning( Error );

	qRAbort();
qRR
qRT
qRE
}

static void GetMeaning_(
	const xpp::preprocessing_iflow___ &IFlow,
	lcl::meaning_ &Meaning )
{
qRH
	xpp::context___ Context;
qRB
	Context.Init();
	IFlow.GetContext( Context );

	if ( Context.Status != xpp::sOK )
		qRGnr();

	xpp::GetMeaning( Context.Coordinates, Meaning );
qRR
qRT
qRE
}

static void ReportAndExit_( 
	const lcl::meaning_ &Meaning,
	const xpp::preprocessing_iflow___ &IFlow )

{
qRH
	lcl::meaning GenericMeaning;
	lcl::meaning MeaningBuffer;
qRB
	GenericMeaning.Init();
	GenericMeaning.SetValue( _( registry::GenericError ) );

//	GenericMeaning.AddTag( Meaning );
	
	MeaningBuffer.Init();
	GetMeaning_( IFlow, MeaningBuffer );
	MeaningBuffer.AddTag( Meaning );

	GenericMeaning.AddTag( MeaningBuffer );

	sclmisc::ReportAndAbort( GenericMeaning );
qRR
qRT
qRE
}

static void ReportAndExit_( 
	const char *Error,
	const xpp::preprocessing_iflow___ &IFlow )

{
qRH
	lcl::meaning Meaning;
qRB
	Meaning.Init();
	Meaning.SetValue( Error );

	ReportAndExit_( Meaning, IFlow );
qRR
qRT
qRE
}

static void ReportAndExit_(
	const char *Error,
	const xpp::preprocessing_iflow___ &IFlow,
	item__ Item )
{
qRH
	lcl::meaning Meaning;
qRB
	Meaning.Init();
	Meaning.SetValue( Error );

	Meaning.AddTag( GetLabel_( Item ) );

	ReportAndExit_( Meaning, IFlow );
qRR
qRT
qRE
}

static void ReportAliasTableAliasAndLabelUsedTogetherAndExit_( const xpp::preprocessing_iflow___ &IFlow )
{
qRH
	lcl::meaning Meaning;
qRB
	Meaning.Init();
	Meaning.SetValue( _( CanNotBeUsedTogetherError ) );
	Meaning.AddTag( GetLabel_( iAliasesTableAliasAttribute ) );
	Meaning.AddTag( GetLabel_( iAliasesTableLabelAttribute ) );

	ReportAndExit_( Meaning, IFlow );
qRR
qRT
qRE
}

static void Insert_(
	const str::string_ &RecordLabel,
	const records_ &Records,
	xpp::preprocessing_iflow___ &IFlow,
	record_ &Record )

{
	rrow__ Row = qNIL;

	Row = dpkrcd::SearchRecord( RecordLabel, Records );

	if ( Row == qNIL )
		ReportAndExit_( _( RecordNotFoundError ), IFlow );

	dpkrcd::Insert( Row, Records, Record );
}

static void InsertUsingRecordAlias_(
	const str::string_ &RecordAlias,
	const record_aliases_ &Aliases,
	const tables_ &Tables,
	xpp::preprocessing_iflow___ &IFlow,
	record_ &Record )
{
	sdr::row__ Row = qNIL;

	Row = FindAlias( RecordAlias, Aliases );

	if ( Row == qNIL )
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
	ctn::qCITEMs( table_, trow__ )Table;

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
	ctn::qCMITEMsl( table_alias_ ) Alias;

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
	sdr::row__ Row = qNIL;

	Row = FindAlias( TableAlias, Aliases );

	if ( Row == qNIL )
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
	trow__ Row = qNIL;
	ctn::qCITEMs( table_, trow__ ) Table;

	Row = SearchTable( TableLabel, Tables );

	if ( Row == qNIL )
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
qRH
	lcl::meaning Meaning;
qRB
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

qRR
qRT
qRE
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
qRH
	lcl::meaning Meaning;
qRB
	Meaning.Init();

	Meaning.SetValue( _( ItemsDefinedTogetherError ) );

	Meaning.AddTag( Item1Wording );
	Meaning.AddTag( Item2Wording );

	ReportAndExit_( Meaning, IFlow );
qRR
qRT
qRE
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
qRH
	str::string TableLabel, RecordLabel, TableAlias, RecordAlias;
	bso::bool__ Continue = true;
qRB
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
			switch ( GetItemId_( dInsertion, Parser.AttributeName() ) ) {
			case iInsertionTableLabelAttribute:
				Assign_( _( TableLabelWording ), Parser, IFlow, TableLabel );
				break;
			case iInsertionRecordLabelAttribute:
				Assign_( _( RecordLabelWording ), Parser, IFlow, RecordLabel );
				break;
			case iInsertionTableAliasAttribute:
				Assign_( _( TableAliasWording ), Parser, IFlow, TableAlias );
				break;
			case iInsertionRecordAliasAttribute:
				Assign_( _( RecordAliasWording ), Parser, IFlow, RecordAlias );
				break;
			default:
				ReportAndExit_( _( UnknownAttributeError ), IFlow );
				break;
			}
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
					qRGnr();	// Normally already handled before.
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
			qRGnr();
			break;
		}
	}
qRR
qRT
qRE
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
				if ( GetItemId_( dNone, Parser.TagName() ) == iInsertionTag )
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
			qRGnr();
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
	dpktbl::counter__ &Skipped )
{
qRH
	bso::bool__ Continue = true;
	record Record;
	bso::integer_buffer__ Buffer;
	bso::bool__ Ignore = false;
qRB
	Record.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTag | xml::tfStartTagClosed | xml::tfAttribute | xml::tfEndTag ) ) {
		case xml::tStartTag:	// Mandatory, otherwise the tag name is missed when there is an attribute.
			Record.Content.Append( Parser.DumpData() );
			break;
		case xml::tAttribute:
			switch ( GetItemId_( dRecord, Parser.AttributeName() ) ) {
			case iRecordLabelAttribute:
				Assign_( _( RecordLabelWording ), Parser, IFlow, Record.Label );
				Record.Content.Append( Parser.DumpData() );
				break;
			case iRecordHandlingAttribute:
				switch ( GetItemId_( dNone, Parser.Value() ) ) {
				case iRecordHandlingAttributeSkipValue:
					if ( Skipped == DPKBSC_COUNTER_MAX )
						qRLmt();
					Skipped++;

					Record.Skip() = true;
					Record.Content.Append( Parser.DumpData() );
					break;
				case iRecordHandlingAttributeIgnoreValue:
					Ignore = true;
					break;
				default:
					ReportAndExit_( _( UnknownAttributeValueError ), IFlow );
					break;
				}
				break;
			default:
				if ( BelongsToNamespace_( Parser.AttributeName() ) )
					ReportAndExit_( _( UnknownAttributeError ), IFlow );
				else
					Record.Content.Append( Parser.DumpData() );
				break;
			}
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
			qRGnr();
			break;
		}
	}
qRR
qRT
qRE
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
qRH
	bso::bool__ Continue = true;
	str::string DefaultRecordLabelTag;
qRB
	DefaultRecordLabelTag.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfAttribute | xml::tfStartTagClosed | xml::tfEndTag ) ) {
		case xml::tAttribute:
			if ( GetItemId_( dNone, Parser.TagName() ) != iContentTag )
				qRGnr();

			if ( GetItemId_( dContent, Parser.AttributeName() ) == iContentDefaultRecordLabelTagAttribute ) {
				Assign_( _( DefaultRecordLabelTagWording ), Parser, IFlow, DefaultRecordLabelTag );
			} else
				ReportAndExit_( _( UnknownAttributeError ), IFlow );
			break;
		case xml::tStartTagClosed:
			if ( GetItemId_( dNone, Parser.TagName() ) == iContentTag ) {
				ProcessRecords_( Parser, IFlow, DefaultRecordLabelTag, Aliases, Tables, Table.Records, Table.Skipped() );	// '<ercp:content ...><...' -> '</erpck:content>...'
				Continue = false;
			}  else																						        			//                    ^                         ^
				qRGnr();
			break;
		case xml::tEndTag:
			Continue = false;	// '</erpck:content>...'
			break;				//                  ^
		case xml::t_Error:
			ReportAndExit_( IFlow );
			break;
		default:
			qRGnr();
			break;
		}
	}
qRR
qRT
qRE
}

enum alias_type__ {
	atRecord,
	atTable,
	at_amount,
	at_Undefined
};
// '...<erpck:alias ...>...' -> '...</:alias>...'
//                  ^                       ^
static void ProcessAlias_(
	xml::parser___ &Parser,
	xpp::preprocessing_iflow___ &IFlow,
	const table_aliases_ &TableAliases,
	const tables_ &Tables,
	trow__ DefaultTableRow,
	aliases_ &Aliases )
{
qRH
	bso::bool__ Continue = true;
	alias_type__ AliasType = at_Undefined;
	trow__ TableRow = qNIL;
	rrow__ RecordRow = qNIL;
	record_alias RecordAlias;
	table_alias TableAlias;
	str::string TableAliasLabel, TableLabel, RecordLabel, AliasLabel;
qRB
	TableAliasLabel.Init();
	TableLabel.Init();
	RecordLabel.Init();
	AliasLabel.Init();

	RecordAlias.Init();
	TableAlias.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTag | xml::tfStartTagClosed | xml::tfAttribute | xml::tfValue | xml::tfEndTag ) ) {
		case xml::tStartTag:
			ReportAndExit_( _( TagNotAllowedHereError ), IFlow );
			break;
		case xml::tAttribute:
			switch ( GetItemId_( dAlias, Parser.AttributeName() ) ) {
			case iAliasTableAliasAttribute:
				if ( TableLabel.Amount() != 0 )
					ReportAliasTableAliasAndLabelUsedTogetherAndExit_( IFlow );

				Assign_( _( TableAliasWording ), Parser, IFlow, TableAliasLabel );
				break;
			case iAliasTableLabelAttribute:
				if ( TableAliasLabel.Amount() != 0 )
					ReportAliasTableAliasAndLabelUsedTogetherAndExit_( IFlow );

				Assign_( _( TableAliasWording ), Parser, IFlow, TableLabel );
				break;
			case iAliasRecordLabelAttribute:
				Assign_( _( RecordLabelWording ), Parser, IFlow, RecordLabel );
				break;
			case iAliasLabelAttribute:
				Assign_( _( AliasLabelWording ), Parser, IFlow, AliasLabel );
				break;
			default:
				ReportAndExit_( _( UnknownAttributeError ), IFlow );
				break;
			}
			break;
		case xml::tStartTagClosed:
			if ( TableLabel.Amount() != 0 ) {
				if ( ( TableRow = SearchTable( TableLabel, Tables ) ) == qNIL )
					ReportAndExit_( _( TableNotFoundError ), IFlow );
			} else if ( TableAliasLabel.Amount() != 0 ) {
				if ( ( TableRow = FindTable( TableAliasLabel, TableAliases ) ) == qNIL )
					ReportAndExit_( _( TableNotFoundError ), IFlow );
			} else if ( DefaultTableRow != qNIL )
				TableRow = DefaultTableRow;
			else
				ReportAndExit_( _( MissingTableReferenceError ), IFlow );

			if ( AliasLabel.Amount() == 0 )
				ReportAndExit_( _( MissingAliasLabelError ), IFlow );

			if ( RecordLabel.Amount() ) {
				if ( ( RecordRow = SearchRecord( RecordLabel, TableRow, Tables ) ) == qNIL )
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
				qRGnr();
			}
			break;
		case xml::tValue:
			ReportAndExit_( _( ValueNotAllowedHereError ), IFlow );
			break;
		case xml::tEndTag:
			switch ( AliasType ) {
			case atRecord:
				Aliases.Records.Append( RecordAlias );
				break;
			case atTable:
				Aliases.Tables.Append( TableAlias );
				break;
			case at_Undefined:
				ReportAndExit_( _( IncompleteAliasDefinitionError ), IFlow );
				break;
			default:
				qRGnr();
				break;
			}
			Continue = false;
			break;
		case xml::t_Error:
			ReportAndExit_( IFlow );
			break;
		default:
			qRGnr();
			break;
		}
	}
qRR
qRT
qRE
}

static void ProcessGenericAlias_(
	const records_ &Records,
	trow__ TableRow,
	const str::string_ &GenericAliasLabel,
	const str::strings_ &CommonTags,
	const str::strings_ &CommonValues,
	xpp::preprocessing_iflow___ &IFlow,
	record_aliases_ &Aliases )
{
qRH
	record_alias Alias;
	ctn::qCITEMs( record_, rrow__ ) Record;
	rrow__ Row = qNIL;
	str::string AliasLabel;
	str::strings Tags, Values;
	bso::integer_buffer__ Buffer;
qRB
	Tags.Init();
	Tags = CommonTags;

	Tags.Append(str::string( "RI" ) );
	Tags.Append( str::string( "RL" ) );

	Row = Records.First();
	Record.Init( Records );

	while ( Row != qNIL ) {
		Values.Init();
		Values = CommonValues;

		Values.Append(str::string(  bso::Convert( *Row + 1, Buffer ) ) );
		Values.Append( Record( Row ).Label );

		AliasLabel.Init();
		AliasLabel = GenericAliasLabel;

		if ( !tagsbs::SubstituteLongTags( AliasLabel, Tags, Values ) )
			ReportAndExit_( _( BadGenericRecordLabelAlias ), IFlow );

		if ( AliasLabel == GenericAliasLabel )
			ReportAndExit_( _( BadGenericRecordLabelAlias ), IFlow );

		Alias.Init( AliasLabel, TableRow, Row );
		dpkals::Store( Alias, Aliases );

		Row = Records.Next( Row );
	}
qRR
qRT
qRE
}

static void ProcessGenericAlias_(
	trow__ TableRow,
	const str::string_ &GenericAliasLabel,
	const tables_ &Tables,
	xpp::preprocessing_iflow___ &IFlow,
	record_aliases_ &Aliases )
{
qRH
	ctn::qCITEMs( table_, trow__ ) Table;
	str::strings Tags, Values;
	bso::integer_buffer__ Buffer;
qRB
	Tags.Init();
	Values.Init();

	Tags.Append(str::string("TI" ) );
	Values.Append( str::string( bso::Convert( *TableRow + 1, Buffer ) ) );

	Table.Init( Tables );

	Tags.Append(str::string("TL" ) );
	Values.Append( Table( TableRow ).Label );


	ProcessGenericAlias_( Table( TableRow ).Records, TableRow, GenericAliasLabel, Tags, Values, IFlow, Aliases );
qRR
qRT
qRE
}

 // '<erpck:table ...>...<erpck:aliases ...>...'
//                                     ^
 static void ProcessAliases_(
	xml::parser___ &Parser,
	xpp::preprocessing_iflow___ &IFlow,
	const tables_ &Tables,
	aliases_ &Aliases )
{
qRH
	bso::bool__ Continue = true;
	str::string TableAliasLabel, TableLabel, GenericAliasLabel;
	trow__ TableRow = qNIL;
qRB
	TableAliasLabel.Init();
	TableLabel.Init();
	GenericAliasLabel.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTag | xml::tfEndTag | xml::tfAttribute) ) {
		case xml::tStartTag:
			if ( GetItemId_( dNone, Parser.TagName() ) == iAliasTag )
				ProcessAlias_( Parser, IFlow, Aliases.Tables, Tables, TableRow, Aliases );	// '...<erpck:alias ...>...' -> '...</:alias>...'
			else
				ReportAndExit_( _( UnknownTagError ), IFlow );
			break;
		case xml::tAttribute:
			switch ( GetItemId_( dAliases, Parser.AttributeName() ) ) {
			case iAliasesTableAliasAttribute:
				if ( TableLabel.Amount() != 0 )
					ReportAliasTableAliasAndLabelUsedTogetherAndExit_( IFlow );

				Assign_( _( TableAliasWording ), Parser, IFlow, TableAliasLabel );

				if ( ( TableRow = FindTable( TableAliasLabel, Aliases.Tables ) ) == qNIL )
					ReportAndExit_( _( TableNotFoundError ), IFlow );

				break;
			case iAliasesTableLabelAttribute:
				if ( TableAliasLabel.Amount() != 0 )
					ReportAliasTableAliasAndLabelUsedTogetherAndExit_( IFlow );

				Assign_( _( TableAliasWording ), Parser, IFlow, TableLabel );

				if ( ( TableRow = SearchTable( TableLabel, Tables ) ) == qNIL )
					ReportAndExit_( _( TableNotFoundError ), IFlow );
				break;
			case iAliasesLabelAttribute:
				Assign_( _( AliasLabelWording ), Parser, IFlow, GenericAliasLabel );
				break;
			default:
				ReportAndExit_( _( UnknownAttributeError ), IFlow );
				break;
			}
			break;
		case xml::tEndTag:
			if ( GenericAliasLabel.Amount() != 0 ) {
				if ( TableRow == qNIL )
					ReportAndExit_( _( MissingTableReferenceError ), IFlow );

				ProcessGenericAlias_( TableRow, GenericAliasLabel, Tables, IFlow, Aliases.Records );
			}


			Continue = false;
			break;
		case xml::t_Error:
			ReportAndExit_( IFlow );
			break;
		default:
			qRGnr();
			break;
		}
	}
qRR
qRT
qRE
}

// '...<erpck:data><erpck::table ...>' -> '..</erpck:table>...'
//                               ^                         ^
static void ProcessTable_(
	xml::parser___ &Parser,
	xpp::preprocessing_iflow___ &IFlow,
	table_ &Table,
	const tables_ &Tables )
{
qRH
	bso::bool__ Continue = true;
	aliases Aliases;
qRB
	Aliases.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTag | xml::tfStartTagClosed | xml::tfAttribute | xml::tfEndTag ) ) {
		case xml::tStartTag:
			switch ( GetItemId_( dNone, Parser.TagName() ) ) {
			case iAliasesTag:
				ProcessAliases_( Parser, IFlow, Tables, Aliases );	// '<erpck:table ...>...<erpck:aliases ...>...'
				break;
			case iContentTag:
				ProcessContent_( Parser, IFlow, Table, Tables, Aliases );	// '<erpck:table ...>...<erpck:content ...>...' -> '...</erpc:content>...'
				break;
			default:
				ReportAndExit_( _( UnknownTagError ), IFlow );
				break;
			}
			break;
		case xml::tStartTagClosed:
			if ( Table.Label.Amount() == 0 )
				ReportAndExit_( _( MissingTableLabelError ), IFlow );
			break;
		case xml::tAttribute:
			if ( GetItemId_( dNone, Parser.TagName() ) != iTableTag )
				qRGnr();
			else if ( GetItemId_( dTable, Parser.AttributeName() ) ==  iTableLabelAttribute )
				Assign_( _( TableLabelWording ), Parser, IFlow, Table.Label );
			else
				ReportAndExit_( _( UnknownAttributeError ), IFlow );
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::t_Error:
			ReportAndExit_( IFlow );
			break;
		default:
			qRGnr();
			break;
		}
	}
qRR
qRT
qRE
}

// '...<erpck:data><erpck:table ...>' -> '...</erpck:table>...'
//                 ^                                       ^
static void ProcessData_(
	xml::parser___ &Parser,
	xpp::preprocessing_iflow___ &IFlow,
	data_ &Data )
{
qRH
	bso::bool__ Continue = true;
	bso::bool__ TableDetected = false;
	table Table;
qRB
	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTag | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( GetItemId_( dNone, Parser.TagName() ) == iTableTag ) {
				TableDetected = true;
				Table.Init();
				ProcessTable_( Parser, IFlow, Table, Data );	// '...<erpck::table ...><erpck:content>...' -> '....</erpck:table>...'
				Data.Append( Table );							//                   ^                                             ^
			} else
				ReportAndExit_( _( MissingTableTagError ), IFlow, iTableTag );
			break;
		case xml::tEndTag:
			if ( !TableDetected ) {
				ReportAndExit_( _( NoTableTagInDataFileError ), IFlow, iTableTag );
			} else
				Continue = false;
			break;
		case xml::t_Error:
			ReportAndExit_( IFlow );
			break;
		default:
			qRGnr();
			break;
		}
	}
qRR
qRT
qRE
}

static void RetrieveData_(
	const str::string_ &DataFileName,
	data_ &Data )
{
qRH
	flf::file_iflow___ FFlow;
	xtf::extended_text_iflow__ XFlow;
	xpp::preprocessing_iflow___ IFlow;
	xml::parser___ Parser;
	bso::bool__ Continue = true;
	lcl::locale Locale;
	bso::bool__ DataDetected = false;
	TOL_CBUFFER___ Buffer;
	xtf::extended_text_iflow__ PXFlow;
	fnm::name___ Location;
qRB
	if ( FFlow.Init( DataFileName, err::hUserDefined ) != tol::rSuccess )
		sclmisc::ReportFileOpeningErrorAndAbort( DataFileName );

	XFlow.Init( FFlow, utf::f_Default );

	Location.Init();
	IFlow.Init( XFlow, xpp::criterions___( str::string( fnm::GetLocation( DataFileName, Location ).UTF8( Buffer ) ) ) );

	PXFlow.Init( IFlow, utf::f_Default );

	Parser.Init( PXFlow, xml::eh_Default );

	Data.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTagClosed |xml::tfAttribute ) ) {
		case xml::tStartTagClosed:
			if ( ( GetItemId_( dNone, Parser.TagName() ) == iDataTag ) ) {
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
				sclmisc::ReportAndAbort( _( NoDataError ), DataFileName );
			else
				Continue = false;
			break;
		case xml::t_Error:
			ReportAndExit_( IFlow );
			break;
		default:
			qRGnr();
			break;
		}
	}

	tol::InitializeRandomGenerator();
qRR
qRT
qRE
}

static void Display_(
	const str::string_ &XML,
	xml::writer_ &Writer )
{
qRH
	flx::E_STRING_IFLOW__ IFlow;
	xtf::extended_text_iflow__ XFlow;
qRB
	IFlow.Init( XML );
	XFlow.Init( IFlow, utf::f_Default );

	if ( xpp::Process( XFlow, Writer ) != xpp::sOK )
		qRGnr();
qRR
qRT
qRE
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
	ctn::qCITEMs( record_, rrow__ ) Record;

	Record.Init( Records );

	Display_( Record( Row ), Writer );

	Label = Record().Label;
}

static void DisplayAll_(
	const records_ &Records,
	xml::writer_ &Writer )
{
	ctn::qCITEMs( record_, rrow__ ) Record;
	rrow__ Row = Records.First();

	Record.Init( Records );

	while ( Row != qNIL ) {
		Display_( Record( Row ), Writer );

		Row = Records.Next( Row );
	}
}

counter__ GetSkippedAmount_(
	dpkctx::amount__ SessionAmount,
	const rrows_ &RecordRows,
	const records_ &Records )
{
	counter__ Skipped = 0;
	sdr::row__ Row = RecordRows.Last();
	rrow__ RecordRow = qNIL;
	ctn::qCITEMs( record_, rrow__ ) Record;

	Record.Init( Records );

	while ( ( Row != qNIL ) && ( SessionAmount-- ) ) {
		RecordRow = RecordRows( Row );

		if ( Records.Exists( RecordRow ) )
			if ( Record( RecordRow ).Skip() ) {
				if ( Skipped == DPKBSC_COUNTER_MAX )
					qRLmt();
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
	dpkctx::context_ &Context,
	xml::writer_ &Writer )
{
qRH
	rrow__ Row = qNIL;
	bso::integer_buffer__ Buffer;
	ctn::qCITEMs( record_, rrow__ ) Record;
	counter__ Counter = 0;
	lcl::meaning Meaning;
qRB
	Counter = Records.Amount();	// NOTA : there is at least one record.
	Record.Init( Records );

	Writer.PutAttribute( "TotalAmount", bso::Convert( Records.Amount(), Buffer ) );
	Writer.PutAttribute( "SkippedTotalAmount", bso::Convert( Skipped, Buffer ) );

	if ( Id == ALL ) {
		Writer.PutAttribute( "Amount", bso::Convert( Records.Amount(), Buffer ) );
		DisplayAll_( Records, Writer );
	} else {
		if ( Id == 0 ) {
			do
				Row = Context.Pick( Records.Amount(), SessionMaxDuration );
			while ( Record( Row ).GetSkip() && ( --Counter ) );

			if ( !Counter ) {
				Writer.PutAttribute( "Amount", "0" );
				Row = qNIL;
			} else if ( !Record( Row ).GetSkip() ) {
				Id = *Row + 1;
				Writer.PutAttribute( "Amount", "1" );
			}

			xml::PutAttribute( "SessionAmount", Context.S_.Session, Writer );
			xml::PutAttribute( "SessionSkippedAmount", GetSkippedAmount_( Context.S_.Session, Context.Pool, Records ), Writer );

			xml::PutAttribute( "CycleAmount", Context.S_.Cycle, Writer );
			xml::PutAttribute( "CycleSkippedAmount", GetSkippedAmount_( Context.S_.Cycle, Context.Pool, Records ), Writer );

		} else {
			if ( Id > Records.Amount() ) {
				Meaning.Init();
				Meaning.SetValue( _( NoRecordOfGivenIdError ) );
				Meaning.AddTag( bso::Convert( Id, Buffer ) );
				sclmisc::ReportAndAbort( Meaning );
			} else {
				Writer.PutAttribute( "Amount", "1" );
				Row = Id - 1;
			}
		}

		if ( Row != qNIL )
			Display_( Row, Records, Label, Writer );
	}
qRR
qRT
qRE
	return Id;
}

static id__ Display_(
	id__ Id,
	const table_ &Table,
	bso::uint__ SessionMaxDuration,
	str::string_ &Label,
	str::string_ &TableLabel,
	dpkctx::context_ &Context,
	xml::writer_ &Writer )
{
	Writer.PushTag( TableLabel = Table.Label );

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
	ctn::qCMITEMsl( random_parameter_ ) Random;
	sdr::row__ Row = qNIL;

	if ( Randoms.Amount() == 0 )
		return;

	tol::InitializeRandomGenerator();

	Random.Init( Randoms );

	Row = Randoms.First();

	Writer.PushTag( "Parameters" );

	while ( Row != qNIL ) {
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
	str::string_ &TableLabel,
	dpkctx::context_ &Context,
	txf::text_oflow__ &Output )
{
qRH
	xml::writer Writer;
	ctn::qCITEMs( table_, trow__ ) Table;
	random_parameters Randoms;
	str::string NS, URI;
	tol::buffer__ Buffer;
qRB
	Randoms.Init();
	GetRandoms_( Randoms );

	xml::WriteXMLHeader( Output, xml::eUTF_8 );
	Output << txf::nl;

	if ( XSLFileName.Amount() != 0 ) {
		xml::WriteXSLDeclaration( XSLFileName, Output );
		Output << txf::nl;
	}

	Table.Init( Data );

	Writer.Init( Output, xml::oIndent, xml::e_None );

	Writer.PushTag( "Picking" );

	NS.Init( "xmlns:" );
	NS.Append( NamespaceLabel_ );
	NS.Truncate();

	URI.Init();
	sclmisc::MGetValue( registry::NamespaceURI, URI );

	Writer.PutAttribute( NS, URI );

	Writer.PushTag( "Misc" );

	Writer.PutValue(tol::DateAndTime(Buffer), "TimeStamp" );

	Writer.PushTag( "Generator" );

	Writer.PushTag( "Name" );
	Writer.PutValue( NAME_MC );
	Writer.PopTag();

	Writer.PushTag( "Version" );
	Writer.PutValue( VERSION );
	Writer.PopTag();

	Writer.PushTag( "URL" );
	Writer.PutValue( WEBSITE_URL );
	Writer.PopTag();

	Writer.PopTag();

	Writer.PushTag( "Session" );
	Writer.PushTag( "MaxDuration" );
	xml::PutValue( SessionMaxDuration, Writer );
	Writer.PopTag();
	Writer.PopTag();
	Writer.PopTag();

	HandleRandoms_( Randoms, Writer );

	Writer.PushTag( "Data" );

	Id = Display_( Id, Table( Data.Last() ), SessionMaxDuration, Label, TableLabel, Context, Writer );

	Writer.PopTag();

	Writer.PopTag();
qRR
qRT
qRE
	return  Id;
}

static id__ DisplayWithoutBackup_(
	id__ Id,
	const data_ &Data,
	const str::string_ &XSLFileName,
	bso::uint__ SessionMaxDuration,
	str::string_ &Label,
	str::string_ &TableLabel,
	dpkctx::context_ &Context,
	const char *FileName )
{
qRH
	flf::file_oflow___ FFlow;
	txf::text_oflow__ TFlow;
qRB
	if ( FFlow.Init( FileName ) != tol::rSuccess )
		sclmisc::ReportFileOpeningErrorAndAbort( FileName );

	TFlow.Init( FFlow );

	Id = Display_( Id, Data, XSLFileName, SessionMaxDuration, Label, TableLabel, Context, TFlow );
qRR
qRT
qRE
	return Id;
}

static id__ Display_(
	id__ Id,
	const data_ &Data,
	const str::string_ &XSLFileName,
	bso::uint__ SessionMaxDuration,
	str::string_ &Label,
	str::string_ &TableLabel,
	dpkctx::context_ &Context,
	const char *FileName )
{
qRH
	bso::bool__ Backuped = false;
qRB
	sclmisc::CreateBackupFile( FileName );

	Backuped = true;

	Id = DisplayWithoutBackup_( Id, Data, XSLFileName, SessionMaxDuration, Label, TableLabel, Context, FileName );
qRR
	if ( Backuped )
		sclmisc::RecoverBackupFile( FileName );
qRT
qRE
	return Id;
}

static id__ Display_(
	id__ Id,
	const data_ &Data,
	const str::string_ &XSLFileName,
	bso::uint__ SessionMaxDuration,
	str::string_ &Label,
	str::string_ &TableLabel,
	dpkctx::context_ &Context,
	const str::string_ &OutputFileName )
{
qRH
	TOL_CBUFFER___ Buffer;
qRB
	if ( OutputFileName.Amount() == 0 )
		Id = Display_( Id, Data, XSLFileName, SessionMaxDuration, Label, TableLabel, Context, COut );
	else
		Id = Display_( Id, Data, XSLFileName, SessionMaxDuration, Label, TableLabel, Context, OutputFileName.Convert( Buffer ) );
qRR
qRT
qRE
	return Id;
}

void LaunchViewer_(
	id__ RecordId,
	id__ TableId,
	const str::string_ &RecordLabel,
	const str::string_ &TableLabel,
	const str::string &DataFilename,
	const str::string &OutputFilename,
	const str::string_ &XSLFilename )
{
qRH
	str::string Viewer;
	TOL_CBUFFER___ SBuffer;
	bso::integer_buffer__ IBuffer;
	tagsbs::tvalues TaggedValues;
qRB
	Viewer.Init();
	sclmisc::OGetValue( registry::Viewer, Viewer );

	if ( ( Viewer.Amount() != 0 ) && ( OutputFilename.Amount() != 0 ) ) {
		TaggedValues.Init();
		TaggedValues.Append("RI", bso::Convert( RecordId, IBuffer ) );
		TaggedValues.Append("RL", RecordLabel );
		TaggedValues.Append("TI", bso::Convert( TableId, IBuffer ) );
		TaggedValues.Append("TL", TableLabel);
		TaggedValues.Append("Data",DataFilename );
		TaggedValues.Append("XSL",XSLFilename );
		TaggedValues.Append("Output",OutputFilename );
		tagsbs::SubstituteLongTags( Viewer, TaggedValues, '$' );
		tol::System( Viewer.Convert( SBuffer ) );
	}
qRR
qRT
qRE
}

static void DumpContext_(
	const dpkctx::context_ &Context,
	xml::writer_ &Writer )
{
	tol::buffer__ Buffer;

	Writer.PushTag( "Context" );
	Writer.PutAttribute( "Target", NAME_LC );
	Writer.PutAttribute( "TimeStamp", tol::DateAndTime( Buffer ) );

	Dump( Context, Writer );

	Writer.PopTag();
}

static void DumpContextWithoutBackup_(
	const dpkctx::context_ &Context,
	const str::string_ &FileName )
{
qRH
	flf::file_oflow___ FFlow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
qRB
	if ( FFlow.Init( FileName ) != tol::rSuccess )
		sclmisc::ReportFileOpeningErrorAndAbort( FileName );

	TFlow.Init( FFlow );

	Writer.Init( TFlow, xml::oIndent, xml::eUTF_8 );

	DumpContext_( Context, Writer ); 
qRR
qRT
qRE
}


static void DumpContext_(
	const dpkctx::context_ &Context,
	const str::string_ &FileName )
{
qRH
	bso::bool__ Backuped = false;
qRB
	sclmisc::CreateBackupFile( FileName );

	Backuped = true;

	DumpContextWithoutBackup_( Context, FileName );
qRR
	if ( Backuped )
		sclmisc::RecoverBackupFile( FileName );
qRT
qRE
}

static void RetrieveContext_(
	xml::parser___ &Parser,							 
	dpkctx::context_ &Context )
{
qRH
	bso::bool__ Continue = true;
	str::string Target;
qRB

	Target.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious | xml::tfStartTagClosed ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() != "Context" )
				qRGnr();

			break;
		case xml::tAttribute:
			if ( Parser.AttributeName() == "Target" ) {
				if ( Target.Amount() != 0 )
					qRGnr();

				Target = Parser.Value();
			} else if ( Parser.AttributeName() != "TimeStamp" )
				qRGnr();
			break;
		case xml::tStartTagClosed:
			if ( Target != NAME_LC )
				qRGnr();

			dpkctx::Retrieve( Parser, Context );
			break;
		case xml::t_Processed:
			Continue = false;
			break;
		default:
			qRGnr();
			break;
		}
	}
qRR
qRT
qRE
}

static void RetrieveContext_(
	const str::string_ &FileName,
	dpkctx::context_ &Context )
{
qRH
	xml::parser___ Parser;
	flf::file_iflow___ FFlow;
	xtf::extended_text_iflow__ XFlow;
qRB
	if ( !fil::Exists( FileName ) ) {
//		COut << "Unable to find context file '" << FileName << "'! It will be created at exit." << txf::nl;
		qRReturn;
	}

	if ( FFlow.Init( FileName ) != tol::rSuccess )
		sclmisc::ReportFileOpeningErrorAndAbort( FileName );

	XFlow.Init( FFlow, utf::f_Default );

	Parser.Init( XFlow, xml::eh_Default );

	RetrieveContext_( Parser, Context );
qRR
qRT
qRE
}

void Process_( void )
{
qRH
	str::string DataFilename;
	data Data;
	dpkctx::context Context;
	str::string OutputFilename;
	str::string XSLFilename;
	str::string ContextFilename;
	bso::uint__ SessionMaxDuration = 0;
	bso::bool__ Error = false;
	str::string Label, TableLabel;
	id__ Id = 0;
qRB
	NamespaceLabel_.Init();
	sclmisc::MGetValue( registry::NamespaceLabel, NamespaceLabel_ );
	NamespaceLabel_.Append( ':' );

	Automat_.Init();
	FillAutomat_();

	Id = sclmisc::OGetUInt( registry::Id, ALL );

	switch ( Id ) {
	case UNDEFINED:
		Id = 0;
		break;
	case 0:
		Id = ALL;
		break;
	default:
		break;
	}

	DataFilename.Init();
	if ( !sclmisc::BGetValue( registry::Data, DataFilename ) )
		sclmisc::ReportAndAbort( _( DataFileNotSpecifiedError ) );

	OutputFilename.Init();
	if ( !sclmisc::BGetValue( registry::Output, OutputFilename ) )
		sclmisc::ReportAndAbort( _( OutputFileNotSpecifiedError ) );

	XSLFilename.Init();
	sclmisc::OGetValue( registry::XSL, XSLFilename );

	ContextFilename.Init();
	if ( !sclmisc::BGetValue( registry::Context, ContextFilename ) )
		sclmisc::ReportAndAbort( _( ContextFileNotSpecifiedError ) );

	Context.Init();
	RetrieveContext_( ContextFilename, Context );

	Data.Init();
	RetrieveData_( DataFilename, Data );

	SessionMaxDuration = sclmisc::OGetUInt( registry::SessionMaxDuration, 0 );

	Label.Init();
	TableLabel.Init();
	Id = Display_( Id, Data, XSLFilename, SessionMaxDuration, Label, TableLabel, Context, OutputFilename );

	DumpContext_( Context, ContextFilename );

	LaunchViewer_( Id, *Data.Last() + 1, Label, TableLabel, DataFilename, OutputFilename, XSLFilename );
qRR
qRT
qRE
}

#define C( name )\
	else if ( Command == #name )\
		name##_()
		
int scltool::SCLTOOLMain(
	const str::string_ &Command,
	const scltool::oddities__ &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH
qRB
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( Process );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;

