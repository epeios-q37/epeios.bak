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

#include "data_r.h"

#include "data_c.h"

#include "common.h"

#include "sclmisc.h"

#include "lcl.h"
#include "xpp.h"

using namespace data_r;

using namespace data_c;

using namespace dpkrcd;
using namespace dpktbl;

#define _( text ) #text

namespace {
	bso::bool__ BelongsToNamespace_( const str::string_ &Name )
	{
		if ( Name.Amount() > NamespaceLabel.Amount() )
			return str::Compare( Name, NamespaceLabel, 0, 0, NamespaceLabel.Amount() ) == 0;
		else
			return false;
	}

	qENUM( Item ) {
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

	qENUM( Disc )
	{	// '...riminant'.
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

	inline const char *GetLabel_( eDisc Disc )
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
	const char *GetBaseLabel_(
		eItem Item,
		bso::bool__ &PrependNamespace,
		eDisc &Disc )
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

	const char *GetLabel_( eItem Item )
	{
		static TOL_CBUFFER___ Buffer;
	qRH
		eDisc Disc = d_Undefined;
		str::string Label;
		bso::bool__ PrependNamespace = false;
		const char *BaseLabel = NULL;
	qRB
		BaseLabel = GetBaseLabel_( Item, PrependNamespace, Disc );

		Label.Init( GetLabel_( Disc ) );

		Label.Append( '$' );

		if ( PrependNamespace )
			Label.Append( NamespaceLabel );

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

	eItem GetItemId_(
		eDisc Disc,
		const str::string_ &Pattern )
	{
		eItem Item = i_Undefined;
	qRH
		str::string NormalizedPattern;
	qRB
		NormalizedPattern.Init( GetLabel_( Disc ) );
		NormalizedPattern.Append( '$' );
		NormalizedPattern.Append( Pattern );

		Item = stsfsm::GetId( NormalizedPattern, Automat_, i_Undefined, i_amount, err::hUserDefined );
	qRR
	qRT
	qRE
		return Item;
	}

	void ReportAndAbort_(
		const char *Error,
		const xpp::preprocessing_iflow___ &IFlow,
		eItem Item )
	{
	qRH
		lcl::meaning Meaning;
	qRB
		Meaning.Init();
		Meaning.SetValue( Error );

		Meaning.AddTag( GetLabel_( Item ) );

		common::ReportAndAbort( Meaning, IFlow );
	qRR
	qRT
	qRE
	}

	void ReportAliasTableAliasAndLabelUsedTogetherAndExit_( const xpp::preprocessing_iflow___ &IFlow )
	{
	qRH
		lcl::meaning Meaning;
	qRB
		Meaning.Init();
		Meaning.SetValue( _( CanNotBeUsedTogetherError ) );
		Meaning.AddTag( GetLabel_( iAliasesTableAliasAttribute ) );
		Meaning.AddTag( GetLabel_( iAliasesTableLabelAttribute ) );

		common::ReportAndAbort( Meaning, IFlow );
	qRR
	qRT
	qRE
	}

	void Insert_(
		const str::string_ &RecordLabel,
		const records_ &Records,
		xpp::preprocessing_iflow___ &IFlow,
		record_ &Record )

	{
		rrow__ Row = qNIL;

		Row = dpkrcd::SearchRecord( RecordLabel, Records );

		if ( Row == qNIL )
			common::ReportAndAbort( _( RecordNotFoundError ), IFlow );

		dpkrcd::Insert( Row, Records, Record );
	}

	void InsertUsingRecordAlias_(
		const str::string_ &RecordAlias,
		const record_aliases_ &Aliases,
		const tables_ &Tables,
		xpp::preprocessing_iflow___ &IFlow,
		record_ &Record )
	{
		sdr::row__ Row = qNIL;

		Row = FindAlias( RecordAlias, Aliases );

		if ( Row == qNIL )
			common::ReportAndAbort( _( RecordAliasNotFoundError ), IFlow );

		Insert( Row, Aliases, Tables, Record );
	}

	void Insert_(
		const str::string_ &RecordLabel,
		const table_ &Table,
		xpp::preprocessing_iflow___ &IFlow,
		record_ &Record )
	{
		Insert_( RecordLabel, Table.Records, IFlow, Record );
	}

	void Insert_(
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

	void Insert_(
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

	void InsertUsingTableAlias_(
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
			common::ReportAndAbort( _( TableAliasNotFoundError ), IFlow );

		Insert_( RecordLabel, Row, Aliases, Tables, IFlow, Record );
	}

	void InsertUsingLabels_(
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
			common::ReportAndAbort( _( TableNotFoundError ), IFlow );

		Table.Init( Tables );

		Insert_( RecordLabel, Table( Row ), IFlow, Record );
	}

	void Assign_(
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
			common::ReportAndAbort( Meaning, IFlow );
		}

		if ( Parser.Value().Amount() == 0 ) {
			Meaning.Init();
			Meaning.SetValue( _( ItemValueEmptyError ) );
			Meaning.AddTag( TargetWording );
			common::ReportAndAbort( Meaning, IFlow );
		}

		Target = Parser.Value();

	qRR
	qRT
	qRE
	}

	inline bso::bool__ Test_( const str::string_ &Value )
	{
		return Value.Amount() != 0;
	}

	void ReportInsertionErrorAndExit_(
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

		common::ReportAndAbort( Meaning, IFlow );
	qRR
	qRT
	qRE
	}

	// '...<dpkq:insert ...>...' -> '...</dpkq:insert>...'
	//                   ^                              ^
	void ProcessInsertion_(
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
				common::ReportAndAbort( _( TagNotAllowedHereError ), IFlow );
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
					common::ReportAndAbort( _( UnknownAttributeError ), IFlow );
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
						common::ReportAndAbort( _( MissingRecordLabelError ), IFlow );
					else
						InsertUsingTableAlias_( TableAlias, RecordLabel, Aliases.Tables, Tables, IFlow, Record );
				else if ( TL )
					if ( RA )
						ReportInsertionErrorAndExit_( _( TableLabelWording ), _( RecordAliasWording ), IFlow );
					else if ( !RL )
						common::ReportAndAbort( _( MissingRecordLabelError ), IFlow );
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
					common::ReportAndAbort( _( MissingInsertionParametersError ), IFlow );

				Continue = false;
				break;
			}
			case xml::tValue:
				common::ReportAndAbort( _( ValueNotAllowedHereError ), IFlow );
				break;
			case xml::t_Error:
				common::ReportAndAbort( IFlow );
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
	void ProcessRecord_(
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
						ProcessInsertion_( Parser, IFlow, Aliases, Tables, Record );	// '...<dpkq:insert ...
					else																//                   ^
						common::ReportAndAbort( _( ForbiddenTagError ), IFlow );
				} else {
					Level++;
					Record.Content.Append( Parser.DumpData() );
				}
				break;
			case xml::tAttribute:
				if ( BelongsToNamespace_( Parser.AttributeName() ) )
					common::ReportAndAbort( _( AttributeNotAllowedHereError ), IFlow );

				Record.Content.Append( Parser.DumpData() );
				break;
			case xml::tValue:
				if ( Level == 0 )
					common::ReportAndAbort( _( MissingTagError ), IFlow );

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
				common::ReportAndAbort( IFlow );
				break;
			default:
				qRGnr();
				break;
			}
		}
	}

	// '...<ercp:content ...><...' -> '...</dpkq:content>...'
	//                       ^                            ^
	void ProcessRecords_(
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
						common::ReportAndAbort( _( UnknownAttributeValueError ), IFlow );
						break;
					}
					break;
				default:
					if ( BelongsToNamespace_( Parser.AttributeName() ) )
						common::ReportAndAbort( _( UnknownAttributeError ), IFlow );
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
				common::ReportAndAbort( IFlow );
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

	// '...<dpkq::table ...>...<dpkq:content ...>...' -> '...</dpkq:content>...'
	//                                         ^                               ^
	void ProcessContent_(
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
					common::ReportAndAbort( _( UnknownAttributeError ), IFlow );
				break;
			case xml::tStartTagClosed:
				if ( GetItemId_( dNone, Parser.TagName() ) == iContentTag ) {
					ProcessRecords_( Parser, IFlow, DefaultRecordLabelTag, Aliases, Tables, Table.Records, Table.Skipped() );	// '<ercp:content ...><...' -> '</dpkq:content>...'
					Continue = false;
				}  else																						        			//                    ^                         ^
					qRGnr();
				break;
			case xml::tEndTag:
				Continue = false;	// '</dpkq:content>...'
				break;				//                  ^
			case xml::t_Error:
				common::ReportAndAbort( IFlow );
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
	// '...<dpkq:alias ...>...' -> '...</:alias>...'
	//                  ^                       ^
	void ProcessAlias_(
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
				common::ReportAndAbort( _( TagNotAllowedHereError ), IFlow );
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
					common::ReportAndAbort( _( UnknownAttributeError ), IFlow );
					break;
				}
				break;
			case xml::tStartTagClosed:
				if ( TableLabel.Amount() != 0 ) {
					if ( ( TableRow = SearchTable( TableLabel, Tables ) ) == qNIL )
						common::ReportAndAbort( _( TableNotFoundError ), IFlow );
				} else if ( TableAliasLabel.Amount() != 0 ) {
					if ( ( TableRow = FindTable( TableAliasLabel, TableAliases ) ) == qNIL )
						common::ReportAndAbort( _( TableNotFoundError ), IFlow );
				} else if ( DefaultTableRow != qNIL )
					TableRow = DefaultTableRow;
				else
					common::ReportAndAbort( _( MissingTableReferenceError ), IFlow );

				if ( AliasLabel.Amount() == 0 )
					common::ReportAndAbort( _( MissingAliasLabelError ), IFlow );

				if ( RecordLabel.Amount() ) {
					if ( ( RecordRow = SearchRecord( RecordLabel, TableRow, Tables ) ) == qNIL )
						common::ReportAndAbort( _( RecordNotFoundError ), IFlow );

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
				common::ReportAndAbort( _( ValueNotAllowedHereError ), IFlow );
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
					common::ReportAndAbort( _( IncompleteAliasDefinitionError ), IFlow );
					break;
				default:
					qRGnr();
					break;
				}
				Continue = false;
				break;
			case xml::t_Error:
				common::ReportAndAbort( IFlow );
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

	void ProcessGenericAlias_(
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
				common::ReportAndAbort( _( BadGenericRecordLabelAlias ), IFlow );

			if ( AliasLabel == GenericAliasLabel )
				common::ReportAndAbort( _( BadGenericRecordLabelAlias ), IFlow );

			Alias.Init( AliasLabel, TableRow, Row );
			dpkals::Store( Alias, Aliases );

			Row = Records.Next( Row );
		}
	qRR
	qRT
	qRE
	}

	void ProcessGenericAlias_(
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

	 // '<dpkq:table ...>...<dpkq:aliases ...>...'
	//                                     ^
	 void ProcessAliases_(
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
					ProcessAlias_( Parser, IFlow, Aliases.Tables, Tables, TableRow, Aliases );	// '...<dpkq:alias ...>...' -> '...</:alias>...'
				else
					common::ReportAndAbort( _( UnknownTagError ), IFlow );
				break;
			case xml::tAttribute:
				switch ( GetItemId_( dAliases, Parser.AttributeName() ) ) {
				case iAliasesTableAliasAttribute:
					if ( TableLabel.Amount() != 0 )
						ReportAliasTableAliasAndLabelUsedTogetherAndExit_( IFlow );

					Assign_( _( TableAliasWording ), Parser, IFlow, TableAliasLabel );

					if ( ( TableRow = FindTable( TableAliasLabel, Aliases.Tables ) ) == qNIL )
						common::ReportAndAbort( _( TableNotFoundError ), IFlow );

					break;
				case iAliasesTableLabelAttribute:
					if ( TableAliasLabel.Amount() != 0 )
						ReportAliasTableAliasAndLabelUsedTogetherAndExit_( IFlow );

					Assign_( _( TableAliasWording ), Parser, IFlow, TableLabel );

					if ( ( TableRow = SearchTable( TableLabel, Tables ) ) == qNIL )
						common::ReportAndAbort( _( TableNotFoundError ), IFlow );
					break;
				case iAliasesLabelAttribute:
					Assign_( _( AliasLabelWording ), Parser, IFlow, GenericAliasLabel );
					break;
				default:
					common::ReportAndAbort( _( UnknownAttributeError ), IFlow );
					break;
				}
				break;
			case xml::tEndTag:
				if ( GenericAliasLabel.Amount() != 0 ) {
					if ( TableRow == qNIL )
						common::ReportAndAbort( _( MissingTableReferenceError ), IFlow );

					ProcessGenericAlias_( TableRow, GenericAliasLabel, Tables, IFlow, Aliases.Records );
				}


				Continue = false;
				break;
			case xml::t_Error:
				common::ReportAndAbort( IFlow );
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

	// '...<dpkq:data><dpkq::table ...>' -> '..</dpkq:table>...'
	//                               ^                         ^
	void ProcessTable_(
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
					ProcessAliases_( Parser, IFlow, Tables, Aliases );	// '<dpkq:table ...>...<dpkq:aliases ...>...'
					break;
				case iContentTag:
					ProcessContent_( Parser, IFlow, Table, Tables, Aliases );	// '<dpkq:table ...>...<dpkq:content ...>...' -> '...</dpkq:content>...'
					break;
				default:
					common::ReportAndAbort( _( UnknownTagError ), IFlow );
					break;
				}
				break;
			case xml::tStartTagClosed:
				if ( Table.Label.Amount() == 0 )
					common::ReportAndAbort( _( MissingTableLabelError ), IFlow );
				break;
			case xml::tAttribute:
				if ( GetItemId_( dNone, Parser.TagName() ) != iTableTag )
					qRGnr();
				else if ( GetItemId_( dTable, Parser.AttributeName() ) ==  iTableLabelAttribute )
					Assign_( _( TableLabelWording ), Parser, IFlow, Table.Label );
				else
					common::ReportAndAbort( _( UnknownAttributeError ), IFlow );
				break;
			case xml::tEndTag:
				Continue = false;
				break;
			case xml::t_Error:
				common::ReportAndAbort( IFlow );
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

	// '...<dpkq:data><dpkq:table ...>' -> '...</dpkq:table>...'
	//                 ^                                       ^
	void ProcessData_(
		xml::parser___ &Parser,
		xpp::preprocessing_iflow___ &IFlow,
		dData &Data )
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
					ProcessTable_( Parser, IFlow, Table, Data );	// '...<dpkq::table ...><dpkq:content>...' -> '....</dpkq:table>...'
					Data.Append( Table );							//                   ^                                             ^
				} else
					ReportAndAbort_( _( MissingTableTagError ), IFlow, iTableTag );
				break;
			case xml::tEndTag:
				if ( !TableDetected ) {
					ReportAndAbort_( _( NoTableTagInDataFileError ), IFlow, iTableTag );
				} else
					Continue = false;
				break;
			case xml::t_Error:
				common::ReportAndAbort( IFlow );
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
}

void data_r::Initialize( void )
{
	if ( data_c::NamespaceLabel.Amount() == 0 )
		qRChk();

	Automat_.Init();
	FillAutomat_();
}

void data_r::Retrieve(
	const str::string_ &DataFileName,
	dData &Data )
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
				ProcessData_( Parser, IFlow, Data );	// '...<dpkq:data><dpkq:table ...>' -> '...</dpkq:table>...'
				DataDetected = true;					//                 ^                                       ^
			} else {
				common::ReportAndAbort( _( ForbiddenTagError ), IFlow );
			} 
			break;
		case xml::tAttribute:
			common::ReportAndAbort( _( UnexpectedAttributeError ), IFlow );
			break;
		case xml::t_Processed:
			if ( !DataDetected )
				sclmisc::ReportAndAbort( _( NoDataError ), DataFileName );
			else
				Continue = false;
			break;
		case xml::t_Error:
			common::ReportAndAbort( IFlow );
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

