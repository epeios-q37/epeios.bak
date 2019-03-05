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

#define XPP__COMPILATION

#include "xpp.h"

#include "fnm.h"
#include "lcl.h"

using namespace xpp;

using xml::parser___;
using xml::token__;

#define DEFINE_TAG_NAME_ATTRIBUTE	"name"

#define EXPAND_TAG_SELECT_ATTRIBUTE		"select"
#define EXPAND_TAG_HREF_ATTRIBUTE		"href"
#define EXPAND_TAG_VALUE_OF_ATTRIBUTE	"value-of"

#define SET_TAG_NAME_ATTRIBUTE		"name"
#define SET_TAG_VALUE_ATTRIBUTE		"value"
#define SET_TAG_ENV_VAR_ATTRIBUTE	"env-var"

#define IFEQ_TAG_SELECT_ATTRIBUTE	"select"
#define IFEQ_TAG_VALUE_ATTRIBUTE	"value"

#define CYPHER_TAG_KEY_ATTRIBUTE	"key"
#define CYPHER_TAG_METHOD_ATTRIBUTE	"method"

#define BLOC_TAG_PRESERVE_ATTRIBUTE	"preserve"
#define BLOC_TAG_MARKER_ATTRIBUTE	"marker"

#define DEFINE_TAG			"define"
#define EXPAND_TAG			"expand"
#define IFEQ_TAG			"ifeq"
#define SET_TAG				"set"
#define BLOC_TAG			"bloc"
#define CDATA_TAG			"cdata"
#define CYPHER_TAG			"cypher"
#define ATTRIBUTE_ATTRIBUTE	"attribute"

static inline status__ Convert_( xml::status__ Status )
{
	return (status__)Status;
}

#define CASE( m )\
	case s##m:\
	return XPP_NAME "_" #m;\
	break

const char *xpp::GetLabel( status__ Status )
{
	if ( Status < Convert_( xml::s_amount ) )
		return xml::GetLabel( (xml::status__)Status );

	switch( Status ) {
	CASE( OK );
	CASE( NoTagsAllowedHere );
//	CASE( UnexpectedTag );
	CASE( UnexpectedValue );
	CASE( UnknownDirective );
//	CASE( AttributeAlreadyDefined );
	CASE( UnexpectedAttribute );
//	CASE( UnknownAttribute );
	CASE( MissingNameAttribute );
	CASE( EmptyNameAttributeValue );
	CASE( MissingSelectOrHRefAttribute );
	CASE( EmptySelectOrHRefAttributeValue );
	CASE( MissingSelectAttribute );
	CASE( EmptySelectAttributeValue );
	CASE( MissingValueAttribute );
	CASE( MissingValueOrValueOfAttribute );
	CASE( MissingNameAndValueAttributes );
	CASE( MissingSelectAndValueAttributes );
//	CASE( UnknownVariable );
	CASE( MustBeEmpty );
	CASE( TooManyTags );
	CASE( UnableToOpenFile );
//	CASE( NestingOverflow );
	CASE( UnknownMacro );
	CASE( BadAttributeDefinitionSyntax );
	CASE( BadCypherKey );
	CASE( MissingCypherKey );
	CASE( MissingKeyOrFormatAttribute );
	CASE( CDataNestingForbidden );
	CASE( UnknownVariable );
	default:
		qRFwk();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}

void xpp::GetMeaning(
	const coords___ &Coordinates,
	lcl::meaning_ &Meaning )
{
qRH
	TOL_CBUFFER___ CBuffer;
	bso::integer_buffer__ IBuffer;
qRB
	if ( !Coordinates.FileName.IsEmpty() ) {
		Meaning.SetValue( XPP_NAME "_ErrorInFileAtLineColumn" );
		Meaning.AddTag( Coordinates.FileName.UTF8( CBuffer ) );
	} else
		Meaning.SetValue( XPP_NAME "_ErrorAtLineColumn" );

	Meaning.AddTag( bso::Convert( Coordinates.Position.Line, IBuffer ) );
	Meaning.AddTag( bso::Convert( Coordinates.Position.Column, IBuffer ) );
qRR
qRT
qRE
}

void xpp::GetMeaning(
	const context___ &Context,
	lcl::meaning_ &Meaning )
{
qRH
	lcl::meaning MeaningBuffer;
qRB
	GetMeaning( Context.Coordinates, Meaning );

	MeaningBuffer.Init();

	if ( Context.Status < (xpp::status__)xml::s_amount )
		MeaningBuffer.SetValue( xml::GetLabel( (xml::status__)Context.Status ) );
	else
		MeaningBuffer.SetValue( GetLabel( Context.Status ) );

	Meaning.AddTag( MeaningBuffer );
qRR
qRT
qRE
}

void xpp::_qualified_preprocessor_directives___::Init( const str::string_ &Namespace )
{
	NamespaceWithSeparator.Init( Namespace );
	NamespaceWithSeparator.Append( ':' );

	DefineTag_.Init( NamespaceWithSeparator );
	DefineTag_.Append( DEFINE_TAG );

	ExpandTag.Init( NamespaceWithSeparator );
	ExpandTag.Append( EXPAND_TAG );

	SetTag.Init( NamespaceWithSeparator );
	SetTag.Append( SET_TAG );

	IfeqTag.Init( NamespaceWithSeparator );
	IfeqTag.Append( IFEQ_TAG );

	BlocTag.Init( NamespaceWithSeparator );
	BlocTag.Append( BLOC_TAG );

	CDataTag.Init( NamespaceWithSeparator );
	CDataTag.Append( CDATA_TAG );

	CypherTag.Init( NamespaceWithSeparator );
	CypherTag.Append( CYPHER_TAG );

	AttributeAttribute.Init( NamespaceWithSeparator );
	AttributeAttribute.Append( ATTRIBUTE_ATTRIBUTE );

	XMLNS.Init( "xmlns:" );
	XMLNS.Append( Namespace );
}

static sdr::size__ Fill_(
	sdr::byte__ *&Buffer,
	sdr::size__ &Wanted,
	const str::string_ &Data,
	sdr::size__ &Position )
{
	sdr::size__ Available = Data.Amount() - Position;

	if ( Wanted < Available )
		Available = Wanted;

	if ( Available ) {
		Data.Recall( Position, Available, (bso::char__ *&)Buffer );
		Position += Available;
		Wanted -= Available;
		Buffer += Available;
	}

	return Available;
}

static inline bso::bool__ BelongsToNamespace_(
	const str::string_ &Name,
	const str::string_ &NamespaceWithSeparator )
{
	if ( Name.Amount() == 0 )
		qRFwk();

	if ( Name.Amount() < NamespaceWithSeparator.Amount() )
		return false;

	return str::Compare( Name, NamespaceWithSeparator, Name.First(), NamespaceWithSeparator.First(), NamespaceWithSeparator.Amount() ) == 0;
}

enum directive__ {
	dNone,
	dUnknown,
	dDefine,
	dExpand,
	dIfeq,
	dBloc,
	dCData,
	dSet,
	dCypher,
	dAttribute,
	d_amount,
	d_Undefined
};



static inline directive__ GetDirective_(
	const str::string_ &Name,
	const _qualified_preprocessor_directives___ &Directives,
	level__ PreservationLevel )
{
	directive__ Directive = d_Undefined;

	if ( BelongsToNamespace_( Name, Directives.NamespaceWithSeparator ) )
		if ( Directives.DefineTag_ == Name )
			Directive = dDefine;
		else if ( Directives.ExpandTag == Name )
			Directive = dExpand;
		else if ( Directives.IfeqTag == Name )
			Directive = dIfeq;
		else if ( Directives.BlocTag == Name )
			Directive = dBloc;
		else if ( Directives.CDataTag == Name )
			Directive = dCData;
		else if ( Directives.SetTag == Name )
			Directive = dSet;
		else if ( Directives.CypherTag == Name )
			Directive = dCypher;
		else if ( Directives.AttributeAttribute == Name )
			Directive = dAttribute;
		else
			Directive = dUnknown;
	else
		Directive = dNone;

	if ( PreservationLevel != 0 )
		if ( Directive != dBloc )
			Directive = dNone;

	return Directive;
}

static status__ AwaitingToken_(
	parser___ &Parser,
	token__ AwaitedToken,
	status__ StatusIfNotAwaitedToken )
{
	token__ Token = xml::t_Undefined;

	switch ( Token = Parser.Parse( xml::tfAllButUseless ) ) {
		case xml::t_Processed:
			qRFwk();
			break;
		case xml::t_Error:
			return Convert_( Parser.Status() );
			break;
		default:
			if ( Token != AwaitedToken )
				return StatusIfNotAwaitedToken;
			break;
	}

	return sOK;
}

static status__ GetDefineNameAttributeValue_(
	parser___ &Parser,
	str::string_ &Value )
{
	status__ Status = AwaitingToken_( Parser, xml::tAttribute, sMissingNameAttribute );

	if ( Status != sOK )
		return Status;

	if ( Parser.AttributeName() != DEFINE_TAG_NAME_ATTRIBUTE )
		return sUnexpectedAttribute;

	Value = Parser.Value();

	if ( Value.Amount() == 0 )
		return sEmptyNameAttributeValue;

	return sOK;
}

void Dump_(
	const str::string_ &Data,
	flw::oflow__ &Flow )
{
qRH
	TOL_CBUFFER___ Buffer;
qRB
	Flow.Write( Data.Convert( Buffer ), Data.Amount() );
qRR
qRT
qRE
}

static status__ RetrieveTree_(
	parser___ &Parser,
	flw::oflow__ &Flow  )
{
	status__ Status = s_Undefined;
	bso::uint__ Nesting = 0;

	while ( Status == s_Undefined ) {
		switch ( Parser.Parse( xml::tfStartTag | xml::tfEndTag | xml::tfValue ) ) {
		case xml::tStartTag:
			Dump_( Parser.DumpData(), Flow );
			if ( Nesting == BSO_UINT_MAX )
				qRLmt();
			Nesting++;
			break;
		case xml::tValue:
			if( Nesting == 0 )
				return sUnexpectedValue;
			else
				Dump_( Parser.DumpData(), Flow );
			break;
		case xml::tEndTag:
			Dump_( Parser.DumpData(), Flow );
			switch ( Nesting ) {
			case 0:
				qRFwk();
				break;
			case 1:
				Status = AwaitingToken_( Parser, xml::tEndTag, sTooManyTags );
				break;
			default:
				Nesting--;
				break;
			}
			break;
		case xml::t_Error:
			return Convert_( Parser.Status() );
			break;
		case xml::t_Processed:
			qRFwk();
			break;
		default:
			qRFwk();
			break;
		}
	}

	return Status;
}

static status__ RetrieveTree_(
	parser___ &Parser,
	str::string_ &Tree )
{
	status__ Status = s_Undefined;
qRH
	flx::E_STRING_OFLOW___ Flow;
qRB
	Flow.Init( Tree );

	Status = RetrieveTree_( Parser, Flow);
qRR
qRT
qRE
	return Status;
}

static status__ GetDefineNameAndContent_(
	parser___ &Parser,
	str::string_ &Name,
	str::string_ &Content )
{
	status__ Status = sOK;

	if ( ( Status = GetDefineNameAttributeValue_( Parser, Name ) ) != sOK )
		return Status;

	if ( ( Status = AwaitingToken_( Parser, xml::tStartTagClosed, sUnexpectedAttribute ) ) != sOK )
		return Status;

	if ( ( Status = RetrieveTree_( Parser, Content ) ) != sOK )
		return Status;

/*	if ( ( Status = AwaitingToken_( Parser, xml::tEndTag, sTooManyTags ) ) != sOK )
		return Status;
*/
	return sOK;
}

status__ xpp::_extended_parser___::_HandleDefineDirective( _extended_parser___ *&Parser )	// 'Parser' est mis  'NULL', ce qui est normal. 
{
	status__ Status = s_Undefined;
qRH
	str::string Name, Content;
	xtf::pos__ Position;
qRB
	Parser = NULL;

	Position = _Parser.GetCurrentPosition();

	Name.Init();
	Content.Init();

	if ( ( Status = GetDefineNameAndContent_( _Parser, Name, Content ) ) == sOK )
		_Repository.Store( Name, Position, _LocalizedFileName, Content );
qRR
qRT
qRE
	return Status;
}

enum expand_type__ {
	etMacro,
	etFile,
	etVariable,
	et_amount,
	et_Undefined
};

static inline expand_type__ GetExpandTypeAndValue_(
	parser___ &Parser,
	str::string_ &Value,
	status__ &Status )	// Siginfiant seulement si valeur retourne == 'et_Undefined'.
{
	expand_type__ Type = et_Undefined;
qRH
	str::string AttributeName;
qRB
	AttributeName.Init();

	if ( ( Status = AwaitingToken_( Parser, xml::tAttribute, sMissingSelectOrHRefAttribute ) ) == sOK ) {
		Value = Parser.Value();

		if ( Value.Amount() == 0 ) {
			Status = sEmptySelectOrHRefAttributeValue;
		} else {
			if ( Parser.AttributeName() == EXPAND_TAG_HREF_ATTRIBUTE )
				Type = etFile;
			else if ( Parser.AttributeName() == EXPAND_TAG_SELECT_ATTRIBUTE )
				Type = etMacro;
			else if ( Parser.AttributeName() == EXPAND_TAG_VALUE_OF_ATTRIBUTE )
				Type = etVariable;
			else {
				Status = sUnexpectedAttribute;
			}
		}
	}
qRR
qRT
qRE
	return Type;
}

typedef tagsbs::long_tags_callback__ ltcallback_;

class attribute_value_substitution_callback
: public ltcallback_
{
private:
	qCRMV( _variables_, V_, Variables_ );
	qCRMV( fnm::name___, S_, SelfPath_ );
protected:
	virtual bso::bool__ TAGSBSGetTagValue(
		const str::string_ &Tag,
		str::string_ &Value ) override
	{
		return GetValue_(V_(), Tag, S_(), Value );
	}
public:
	void reset( bso::bool__ P = true )
	{
		ltcallback_::reset( P );
		Variables_ = NULL;
		SelfPath_ = NULL;

	}
	E_CVDTOR( attribute_value_substitution_callback );
	void Init(
		const _variables_ &Variables,
		const fnm::name___ &SelfPath )
	{
		ltcallback_::Init();
		Variables_ = &Variables;
		SelfPath_ = &SelfPath;
	}
};

status__ xpp::_extended_parser___::_HandleMacroExpand(
	const str::string_ &RawMacroName,
	_extended_parser___ *&Parser )
{
	status__ Status = sOK;
qRH
	str::string MacroName;
	str::string FileName;
	str::string Content;
	xtf::pos__ Position;
	attribute_value_substitution_callback Callback;
qRB
	FileName.Init();
	Content.Init();

	MacroName.Init();
	if ( SubstitutionMarker_() != 0 ) {
		Callback.Init( _Variables, _Directory );
		if ( !tagsbs::SubstituteLongTags( RawMacroName, Callback, MacroName, SubstitutionMarker_() ) ) {
			Status = sUnknownVariable;
		}
	} else
		MacroName = RawMacroName;

	if ( Status == sOK ) {
		if ( !_Repository.Get( MacroName, Position, FileName, Content ) ) {
			Status = sUnknownMacro;
		} else {
			Parser = NewParser( _Repository, _Variables, _Directives );

			Status = Parser->_InitWithContent( Content, FileName, Position, _Directory, _CypherKey, Preserve_, SubstitutionMarker_(), _Parser.GetFormat() );
		}
	}
qRR
	if ( Parser != NULL ) {
		delete Parser;
		Parser = NULL;
	}
qRT
	if ( Status != sOK ) {
		if ( Parser != NULL ) {
			delete Parser;
			Parser = NULL;
		}
	}
qRE
	return Status;
}

status__ xpp::_extended_parser___::_HandleFileExpand(
	const str::string_ &RawFilename,
	_extended_parser___ *&Parser )
{
	status__ Status = sOK;
qRH
	str::string Filename;
	attribute_value_substitution_callback Callback;
qRB
	Parser = NewParser( _Repository, _Variables, _Directives );

	Filename.Init();
	if ( SubstitutionMarker_() != 0 ) {
		Callback.Init( _Variables, _Directory );
		if ( !tagsbs::SubstituteLongTags(RawFilename, Callback, Filename, SubstitutionMarker_() ) ) {
			Status = sUnknownVariable;
		}
	} else
		Filename = RawFilename;

	if ( Status == sOK )
		Status = Parser->_InitWithFile( Filename, _Directory, _CypherKey, Preserve_, SubstitutionMarker_(), _Parser.GetFormat() );
qRR
	if ( Parser != NULL ) {
		delete Parser;
		Parser = NULL;
	}
qRT
	if ( Status != sOK ) {
		if ( Parser != NULL ) {
			delete Parser;
			Parser = NULL;
		}
	}
qRE
	return Status;
}

status__ xpp::_extended_parser___::_HandleVariableExpand(
	const str::string_ &VariableName,
	_extended_parser___ *&Parser )
{
	status__ Status = s_Undefined;
qRH
	str::string Content;
	xtf::pos__ Position;
qRB
	Content.Init();

	Content.Append( '<' );
	Content.Append( _Directives.BlocTag );
	Content.Append( '>' );

	if ( GetVariableValue_( VariableName, Content ) ) {
		Content.Append( "</" );
		Content.Append( _Directives.BlocTag );
		Content.Append( '>' );

		Parser = NewParser( _Repository, _Variables, _Directives );

		Status = Parser->_InitWithContent( Content, _LocalizedFileName, Position, _Directory, _CypherKey, Preserve_, SubstitutionMarker_(), _Parser.GetFormat() );
	}
	else
		Status = sUnknownVariable;

qRR
	if ( Parser != NULL ) {
		delete Parser;
		Parser = NULL;
	}
qRT
	if ( Status != sOK ) {
		if ( Parser != NULL ) {
			delete Parser;
			Parser = NULL;
		}
	}
qRE
	return Status;
}

status__ xpp::_extended_parser___::_HandleExpandDirective( _extended_parser___ *&Parser )
{
	status__ Status = s_Undefined;
qRH
	str::string Value;
qRB
	Value.Init();

	switch ( GetExpandTypeAndValue_( _Parser, Value, Status ) ) {
	case etMacro:
		Status = _HandleMacroExpand( Value, Parser );
		break;
	case etFile:
		Status = _HandleFileExpand( Value, Parser );
		break;
	case etVariable:
		Status = _HandleVariableExpand( Value, Parser );
		break;
	case et_Undefined:
		// 'Status' initialis par 'etExpandTypeAndValue_(...)'.
		break;
	default:
		qRFwk();
		break;
	}

	if ( Status == sOK )
		Status = AwaitingToken_( _Parser, xml::tStartTagClosed, sUnexpectedAttribute );

	if ( Status == sOK )
		Status = AwaitingToken_( _Parser, xml::tEndTag, sMustBeEmpty );

	_Parser.PurgeDumpData();

qRR
qRT
qRE
	return Status;
}

static void GetEnvVar_(
	const str::string_ &Name,
	str::string_ &Value )
{
qRH
	TOL_CBUFFER___ Buffer;
qRB
	Value.Append( getenv( Name.Convert( Buffer ) ) );
qRR
qRT
qRE
}

static status__ GetSetNameAndValue_(
	parser___ &Parser,
	str::string_ &Name,
	str::string_ &Value )
{
	status__ Status = sOK;

	if ( ( Status = AwaitingToken_( Parser, xml::tAttribute, sMissingNameAndValueAttributes ) ) == sOK ) {
		if ( Parser.AttributeName() == SET_TAG_NAME_ATTRIBUTE ) {
			Name.Append( Parser.Value() );

			if ( ( Status = AwaitingToken_( Parser, xml::tAttribute, sMissingValueOrValueOfAttribute ) ) == sOK ) {
				if ( Parser.AttributeName() == SET_TAG_VALUE_ATTRIBUTE )
					Value.Append( Parser.Value() );
				else if ( Parser.AttributeName() == SET_TAG_ENV_VAR_ATTRIBUTE )
					GetEnvVar_( Parser.Value(), Value );
				else
					Status = sUnexpectedAttribute;
			}
		} else if ( Parser.AttributeName() == SET_TAG_VALUE_ATTRIBUTE ) {
			Value.Append( Parser.Value() );

			if ( ( Status = AwaitingToken_( Parser, xml::tAttribute, sMissingNameAttribute ) ) == sOK ) {
				if ( Parser.AttributeName() == SET_TAG_NAME_ATTRIBUTE )
					if ( Parser.Value() == 0 )
						Status = sEmptyNameAttributeValue;
					else
						Name.Append( Parser.Value() );
				else
					Status = sUnexpectedAttribute;
			}
		} else if ( Parser.AttributeName() == SET_TAG_ENV_VAR_ATTRIBUTE ) {
			GetEnvVar_( Parser.Value(), Value );

			if ( ( Status = AwaitingToken_( Parser, xml::tAttribute, sMissingNameAttribute ) ) == sOK ) {
				if ( Parser.AttributeName() == SET_TAG_NAME_ATTRIBUTE )
					if ( Parser.Value() == 0 )
						Status = sEmptyNameAttributeValue;
					else
						Name.Append( Parser.Value() );
				else
					Status = sUnexpectedAttribute;
			}
		}
	}

	return Status;
}

status__ xpp::_extended_parser___::_HandleSetDirective( _extended_parser___ *&Parser )	// 'Parser' est mis  'NULL', ce qui est normal. 
{
	status__ Status = sOK;
qRH
	str::string Name, RawValue, Value;
	attribute_value_substitution_callback Callback;
qRB
	Parser = NULL;

	Name.Init();
	RawValue.Init();

	if ( (Status = GetSetNameAndValue_( _Parser, Name, RawValue )) == sOK ) {
		Value.Init();
		if ( SubstitutionMarker_() != 0 ) {
			Callback.Init( _Variables, _Directory );
			if ( !tagsbs::SubstituteLongTags( RawValue, Callback, Value, SubstitutionMarker_() ) ) {
				Status = sUnknownVariable;
			}
		} else
			Value = RawValue;

		if ( Status == sOK ) {
			_Variables.Set( Name, Value );

			if ( Status == sOK )
				Status = AwaitingToken_( _Parser, xml::tStartTagClosed, sUnexpectedAttribute );

			if ( Status == sOK )
				Status = AwaitingToken_( _Parser, xml::tEndTag, sMustBeEmpty );

			_Parser.PurgeDumpData();
		}
	}
qRR
qRT
qRE
	return Status;
}

static status__ GetIfeqSelectAndValue_(
	parser___ &Parser,
	str::string_ &Name,
	str::string_ &Value )
{
	status__ Status = sOK;

	if ( ( Status = AwaitingToken_( Parser, xml::tAttribute, sMissingSelectAndValueAttributes ) ) == sOK ) {
		if ( Parser.AttributeName() == IFEQ_TAG_SELECT_ATTRIBUTE ) {
			Name.Append( Parser.Value() );

			if ( ( Status = AwaitingToken_( Parser, xml::tAttribute, sMissingValueAttribute ) ) == sOK ) {
				if ( Parser.AttributeName() == IFEQ_TAG_VALUE_ATTRIBUTE )
					Value.Append( Parser.Value() );
				else
					Status = sUnexpectedAttribute;
			}
		} else if ( Parser.AttributeName() == IFEQ_TAG_VALUE_ATTRIBUTE ) {
			Value.Append( Parser.Value() );

			if ( ( Status = AwaitingToken_( Parser, xml::tAttribute, sMissingSelectAttribute ) ) == sOK ) {
				if ( Parser.AttributeName() == IFEQ_TAG_SELECT_ATTRIBUTE )
					if ( Parser.Value().Amount() == 0 )
						Status = sEmptySelectAttributeValue;
					else
						Name.Append( Parser.Value() );
				else
					Status = sUnexpectedAttribute;
			}
		}
	}

	return Status;
}


status__ xpp::_extended_parser___::_HandleIfeqDirective( _extended_parser___ *&Parser )
{
	status__ Status = s_Undefined;
qRH
	str::string Name, ExpectedValue, TrueValue, Content;
	xtf::pos__ Position;
qRB
	Parser = NULL;

	Name.Init();
	ExpectedValue.Init();

	if ( (Status = GetIfeqSelectAndValue_( _Parser, Name, ExpectedValue )) == sOK ) {
		if ( (Status = AwaitingToken_( _Parser, xml::tStartTagClosed, sUnexpectedAttribute )) == sOK ) {
			Content.Init();

			Position = _Parser.GetCurrentPosition();

			if ( (Status = RetrieveTree_( _Parser, Content )) == sOK ) {
				TrueValue.Init();

				if ( (GetVariableValue_( Name, TrueValue )) && (ExpectedValue == TrueValue) ) {
					Parser = NewParser( _Repository, _Variables, _Directives );

					Status = Parser->_InitWithContent( Content, _LocalizedFileName, Position, _Directory, _CypherKey, Preserve_, SubstitutionMarker_(), _Parser.GetFormat() );
				}
			}
		}
	}
qRR
	if ( Parser != NULL ) {
		delete Parser;
		Parser = NULL;
	}
qRT
	if ( Status != sOK ) {
		if ( Parser != NULL ) {
			delete Parser;
			Parser = NULL;
		}
	}
qRE
	return Status;
}

enum cypher_mode__
{
	cmOverriden,
	cmEncrypted,
	cm_amount,
	cm_Undefined
};

static inline cypher_mode__ GetCypherModeAndValue_(
	parser___ &Parser,
	str::string_ &Value,
	status__ &Status )	// Siginfiant seulement si valeur retourne == 'cm_Undefined'.
{
	cypher_mode__ Mode = cm_Undefined;
qRH
	str::string AttributeName;
qRB
	AttributeName.Init();

	if ( ( Status = AwaitingToken_( Parser, xml::tAttribute, sMissingKeyOrFormatAttribute ) ) == sOK ) {
		if ( Parser.AttributeName() == CYPHER_TAG_KEY_ATTRIBUTE )
			Mode = cmOverriden;
		else if ( Parser.AttributeName() == CYPHER_TAG_METHOD_ATTRIBUTE )
			Mode = cmEncrypted;
		else
			Status = sUnexpectedAttribute;

		if ( Status == sOK )
			Value = Parser.Value();
	}
qRR
qRT
qRE
	return Mode;
}

status__ xpp::_extended_parser___::_HandleCypherDecryption(
	const str::string_ &Version,
	_extended_parser___ *&Parser )
{
	Parser = NewParser( _Repository, _Variables, _Directives );

	return Parser->_InitCypher( _Parser.Flow().UndelyingFlow(), _LocalizedFileName, Position(), _Directory, _CypherKey, Preserve_, SubstitutionMarker_(), _Parser.GetFormat() );
}

status__ xpp::_extended_parser___::_HandleCypherOverride(
	const str::string_ &CypherKey,
	_extended_parser___ *&Parser )
{

	if ( ( _CypherKey.Amount() != 0  ) && ( _CypherKey !=  CypherKey ) )
		return sBadCypherKey;
	else {
		Parser = NewParser( _Repository, _Variables, _Directives );

		if ( PreservationLevel_ != 0 )
			qRFwk();

		return Parser->Init( _Parser.Flow(), _LocalizedFileName, _Directory, _CypherKey, Preserve_, SubstitutionMarker_() );
	}
}


status__ xpp::_extended_parser___::_HandleCypherDirective( _extended_parser___ *&Parser )
{
	status__ Status = s_Undefined;
qRH
	str::string Value;
qRB
	Value.Init();

	switch ( GetCypherModeAndValue_( _Parser, Value, Status ) ) {
	case cmEncrypted:
		if ( ( _CypherKey == NULL ) || ( *_CypherKey == 0 ) )
			Status = _HandleCypherOverride( Value, Parser );
		else
			Status = _HandleCypherDecryption( Value, Parser );
		break;
	case cmOverriden:
		Status = _HandleCypherOverride( Value, Parser );
		break;
	case cm_Undefined:
		// 'Status' initialis par 'etCypherModeAndValue_(...)'.
		break;
	default:
		qRFwk();
		break;
	}

	if ( Status == sOK )
		Status = AwaitingToken_( _Parser, xml::tStartTagClosed, sUnexpectedAttribute );

	_Parser.PurgeDumpData();
qRR
qRT
qRE
	return Status;
}


status__ xpp::_extended_parser___::_HandlePreprocessorDirective(
	int Directive,
	_extended_parser___ *&Parser )
{
	Parser = NULL;

	switch ( Directive ) {
	case dNone:
		qRFwk();
		break;
	case dUnknown:
		return sUnknownDirective;
		break;
	case dDefine:
		return _HandleDefineDirective( Parser );
		break;
	case dBloc:
		qRFwk();	// Trait en amont.
		break;
	case dCData:
		qRFwk();	// Trait en amont.
		break;
	case dExpand:
		return _HandleExpandDirective( Parser );
		break;
	case dSet:
		return _HandleSetDirective( Parser );
		break;
	case dIfeq:
		return _HandleIfeqDirective( Parser );
		break;
	case dCypher:
		return _HandleCypherDirective( Parser );
		break;
	default:
		qRFwk();
		break;
	}

	qRFwk();

	return s_Undefined;	// Pour viter un 'warning'.

}

static sdr::row__ ExtractAttributeName_(
	const str::string_ &Source,
	str::string_ &Value )
{
	sdr::row__ Row = Source.First();

	while ( ( Row != qNIL )
		    && ( isalnum( Source( Row ) ) || ( Source( Row ) == ':' ) || ( Source( Row ) == '_' ) ) ) {
		Value.Append( Source( Row ) );
		Row = Source.Next( Row );
	}

	return Row;
}

status__ xpp::_extended_parser___::HandleAtributeValueSubstitution_(
	const str::string_ &Source,
	bso::char__ Marker,
	str::string_ &Data )
{
	attribute_value_substitution_callback Callback;

	Callback.Init( _Variables, _Directory );

	if ( !tagsbs::SubstituteLongTags( Source, Callback, Data, Marker ) )
		return sUnknownVariable;
	else
		return sOK;
}

status__ xpp::_extended_parser___::_HandleAttributeDirective(
	const str::string_ &Parameters,
	_extended_parser___ *&Parser,
	str::string_ &Data )
{
	status__ Status = s_Undefined;
qRH
	str::string AttributeName, MacroName;
	sdr::row__ Row = qNIL;
qRB
	AttributeName.Init();

	Row = ExtractAttributeName_( Parameters, AttributeName );

	if ( ( Row == qNIL )
		  || ( AttributeName.Amount() == 0 )
		  || ( Parameters( Row ) != ',' ) ) {
		Status = sBadAttributeDefinitionSyntax;
	} else {
		MacroName.Init( Parameters );
		MacroName.Remove( MacroName.First(), *Row + 1 );

		Data.Append( AttributeName );
		Data.Append( "=\"" );
		Status = this->_HandleMacroExpand( MacroName, Parser );

		_AttributeDefinitionInProgress = true;
	}
qRR
qRT
qRE
	return Status;
}

status__ xpp::_extended_parser___::_InitWithFile(
	const fnm::name___ &FileName,
	const fnm::name___ &Directory,
	const str::string_ &CypherKey,
	bso::bool__ Preserve,
	bso::char__ SubstitutionMarker,
	utf::format__ Format )
{
	status__ Status = s_Undefined;
qRH
	str::string LocalizedFileNameBuffer;
	str::string LocationBuffer;
	fnm::name___ Path;
	fnm::name___ Location;
qRB
	Path.Init();
	fnm::BuildPath( Directory, FileName, "", Path );

	if ( _FFlow.Init( fnm::Normalize( Path ), fil::mReadOnly, err::hUserDefined ) != tol::rSuccess ) {
		Status = sUnableToOpenFile;
	} else {
		_XFlow.Init( _FFlow, Format );

		fnm::GetLocation( Path, Location );

		LocalizedFileNameBuffer.Init();
		LocationBuffer.Init();

		if ( (Status = Init( _XFlow, Path.UTF8( LocalizedFileNameBuffer ), Location.UTF8( LocationBuffer ), CypherKey, Preserve, SubstitutionMarker )) == sOK )
			_IgnorePreprocessingInstruction = true;
	}
qRR
qRT
qRE
	return Status;
}

status__ xpp::_extended_parser___::_InitWithContent(
	const str::string_ &Content,
	const fnm::name___ &NameOfTheCurrentFile,
	const xtf::pos__ &Position,
	const fnm::name___ &Directory,
	const str::string_ &CypherKey,
	bso::bool__ Preserve,
	bso::char__ SubstitutionMarker,
	utf::format__ Format )
{
	status__ Status = s_Undefined;
qRH
qRB
	_MacroContent.Init( Content );
	_SFlow.Init( _MacroContent );
//	_SFlow.EOFD( XTF_EOXT );

	_XFlow.Init( _SFlow, Format, Position );

	if ( ( Status = Init( _XFlow, NameOfTheCurrentFile, Directory, CypherKey, Preserve, SubstitutionMarker ) ) == sOK )
		_IgnorePreprocessingInstruction = false;
qRR
qRT
qRE
	return Status;
}

status__ xpp::_extended_parser___::_InitCypher(
	flw::iflow__ &Flow,
	const fnm::name___ &FileName,
	const xtf::pos__ &Position,
	const fnm::name___ &Directory,
	const str::string_ &CypherKey,
	bso::bool__ Preserve,
	bso::char__ SubstitutionMarker,
	utf::format__ Format )
{
	_Decoder.Init( Flow );
	_Decrypter.Init( _Decoder, CypherKey );
	_XFlow.Init( _Decrypter, Format, Position );

	if ( PreservationLevel_ != 0 )
		qRFwk();

	return Init( _XFlow, FileName, Directory, CypherKey, Preserve, SubstitutionMarker );
}


static bso::bool__ StripHeadingSpaces_(
	xml::token__ PreviousToken,
	const xml::parser___ &Parser,
	const str::string_ &NamespaceWithSeparator )
{
	return ( PreviousToken == xml::tValue )
		   || ( ( PreviousToken == xml::tEndTag )
		      && ( BelongsToNamespace_( Parser.TagName(), NamespaceWithSeparator )
			  || ( Parser.Token() == xml::tEndTag ) ) );
}

static void StripHeadingSpaces_( str::string_ &Data )
{
	while ( ( Data.First() != qNIL ) && ( isspace( Data( Data.First() ) ) ) )
		Data.Remove( Data.First() );
}

#define CDATA_NESTING_MAX	BSO_UINT_MAX	

status__ xpp::_extended_parser___::Handle(
	_extended_parser___ *&Parser,
	str::string_ &Data )
{
	status__ Status = s_Undefined;
	bso::bool__ Continue = true;
	xml::token__ PreviousToken = xml::t_Undefined;
	bso::bool__ StripHeadingSpaces = false;
	directive__ Directive = d_Undefined;
	bso::bool__ SubstitutionMarkerHandled = false;

	Parser = NULL;

	if ( _AttributeDefinitionInProgress ) {
		Data.Append( '"' );
		_AttributeDefinitionInProgress = false;
	}

	while ( Continue ) {
		Continue = false;
		PreviousToken = _Parser.Token();
		switch ( _Parser.Parse() ) {
		case  xml::tProcessingInstruction:
			if ( _IgnorePreprocessingInstruction ) {
				_Parser.PurgeDumpData();
				Continue = true;
			} else
				Status = sOK;
			break;
		case xml::tStartTag:
			Directive = GetDirective_( _Parser.TagName(), _Directives, PreservationLevel_ );

			if ( Directive == dCData ) {
				if ( _CDataNesting == CDATA_NESTING_MAX )
					qRLmt();

				if ( _CDataNesting == 0 )
					Continue = true;
				else
					Status = sOK;
			} else if ( _CDataNesting != 0 ) {
				Status = sOK;
			} else {
				switch ( Directive ) {
				case dNone:
					Status = sOK;
					break;
				case dBloc:
					if ( PreservationLevel_ == 0 )
						Continue = true;
					else if ( PreservationLevel_ == XPP_LEVEL_MAX )
						qRLmt();
					else {
						PreservationLevel_++;
						Status = sOK;
					}
					break;
				default:
					Status = _HandlePreprocessorDirective( Directive, Parser );
					
					if ( Parser == NULL )
						Continue = true;
					break;
				}
			}

			Directive = d_Undefined;
			break;
		case xml::tAttribute:
			if ( _CDataNesting == 0 ) {
				switch ( GetDirective_( _Parser.TagName(), _Directives, PreservationLevel_ ) ) {
				case dNone:
					switch ( GetDirective_( _Parser.AttributeName(), _Directives, PreservationLevel_ ) ) {
					case dNone:
						if ( SubstitutionMarker_() != 0 ) {
							_Parser.PurgeDumpData();
							Data.Append (_Parser.AttributeName() );
							Data.Append( "=\"") ;
							Status = HandleAtributeValueSubstitution_( _Parser.Value(), SubstitutionMarker_(), Data );
							Data.Append('"');
						} else
							Status = sOK;
						break;
					case dUnknown:
						Status = sUnknownDirective;
						break;
					case dAttribute:
						_Parser.PurgeDumpData();
						Status = _HandleAttributeDirective( _Parser.Value(), Parser, Data );
						break;
					default:
						Status = sUnknownDirective;
						break;
					}
					break;
				case dBloc:
					if ( _Parser.AttributeName() == BLOC_TAG_PRESERVE_ATTRIBUTE )
						if ( PreservationLevel_ == 0 ) {
							if ( _Parser.Value() == "yes" ) {
								if ( Preserve_ )
									PreservationLevel_ = 1;
								Continue = true;
							} else if ( _Parser.Value() != "no" )
								Status = sUnexpectedValue;
							else
								Continue = true;
						} else
							Status = sOK;
					else if ( _Parser.AttributeName() == BLOC_TAG_MARKER_ATTRIBUTE ) {
						if ( PreservationLevel_ == 0 ) {
							if ( _Parser.Value().Amount() > 1 )
								Status = sUnexpectedValue;
							else  {
								if ( _Parser.Value().Amount() == 0 )
									SubstitutionMarkers_.Push( 0 );
								else
									SubstitutionMarkers_.Push( _Parser.Value()( _Parser.Value().First() ) );

								SubstitutionMarkerHandled = true;
								Continue = true;
							}
						} else
							Status = sOK;
					} else
						Status = sUnexpectedAttribute;
					break;
				case dCData:
					Status = sUnexpectedAttribute;
					break;
				default:
					qRFwk();
					break;
				}
			} else
				Status = sOK;
			break;
		case xml::tSpecialAttribute:
			if ( _CDataNesting == 0 ) {
				switch ( GetDirective_( _Parser.TagName(), _Directives, PreservationLevel_ ) ) {
				case dNone:
					break;
				case dBloc:
				case dCData:
					_Parser.PurgeDumpData();
					// L'attribut n'est pas rpercut sur le flux de sortie.
					break;
				default:
					qRFwk();
					break;
				}
			}

			Status = sOK;
			break;
		case xml::tStartTagClosed:
			switch ( GetDirective_( _Parser.TagName(), _Directives, PreservationLevel_ ) ) {
			case dNone:
				Status = sOK;
				break;
			case dCData:
				if ( _CDataNesting == 0 )
					Continue = true;
				else
					Status = sOK;
				if ( _CDataNesting == 0 )
					Data.Append( "<![CDATA[" );

				_CDataNesting++;
				StripHeadingSpaces = true;
				break;
			case dBloc:
				if ( PreservationLevel_ > 1 )
					Status = sOK;
				else if ( _CDataNesting == 0 )
					Continue = true;
				else
					Status = sOK;

				if ( !SubstitutionMarkerHandled )
					SubstitutionMarkers_.Push( SubstitutionMarker_() );

				SubstitutionMarkerHandled = false;
				break;
			case dExpand:
				Status = sOK;
				break;
			default:
				qRFwk();
				break;
			}
			break;
		case xml::tEndTag:
			switch ( GetDirective_( _Parser.TagName(), _Directives, PreservationLevel_ ) ) {
			case dNone:
				if ( _CDataNesting == 0 )
					StripHeadingSpaces = StripHeadingSpaces_( PreviousToken, _Parser, _Directives.NamespaceWithSeparator );
				Status = sOK;
				break;
			case dCData:
				switch ( _CDataNesting ) {
				case 0:
					qRFwk();
					break;
				case 1:
					Continue = true;
					Data.Append( "]]>" );
					break;
				default:
					Status = sOK;
					break;
				}
				_CDataNesting--;
				break;
			case dBloc:
				if ( PreservationLevel_ != 0 ) {
					PreservationLevel_--;
					if ( PreservationLevel_ > 0 ) {
						Status = sOK;
						break;
					}
				}

				SubstitutionMarkers_.Pop();
			case dCypher:
				if ( _CDataNesting == 0 ) {
					StripHeadingSpaces = StripHeadingSpaces_( PreviousToken, _Parser, _Directives.NamespaceWithSeparator );
					Continue = true;
				} else
					Status = sOK;
				break;
			case dExpand:
				Status = sOK;
				break;
			default:
				qRFwk();
				break;
			}
		break;
		case xml::tValue:
			Status = sOK;
			break;
		case xml::tComment:
			if ( _CDataNesting == 0 ) {
				_Parser.PurgeDumpData();
				Continue = true;
			} else
				Status = sOK;
			break;
		case xml::tCData:
			if ( _CDataNesting == 0 ) {
				_Parser.PurgeDumpData();
				Data.Append( "<![CDATA[" );
				Data.Append( _Parser.Value() );
				Data.Append( "]]>" );
				Status = sOK;
			} else
				Status = sCDataNestingForbidden;
			break;
		case xml::t_Processed:
//			StripHeadingSpaces = StripHeadingSpaces_( PreviousToken, _Parser, _Directives.NamespaceWithSeparator );
			Status = s_Pending;
			break;
		case xml::t_Error:
			Status = Convert_( _Parser.Status() );
			break;
		default:
			qRFwk();
			break;
		}

		if ( ( Status != sOK ) && ( Status != s_Undefined ) )
			Continue = false;
	}

	if ( Parser == NULL ) {
		Data.Append( _Parser.DumpData() );

		if ( StripHeadingSpaces )
			StripHeadingSpaces_( Data );
	}

	return Status;
}

void xpp::_preprocessing_iflow_driver___::_DeleteParsers( void )
{
	if ( _CurrentParser != NULL )
		delete _CurrentParser;

	while ( _Parsers.Amount() )
		delete _Parsers.Pop();
}

sdr::size__ xpp::_preprocessing_iflow_driver___::FDRRead(
	sdr::size__ Maximum,
	sdr::byte__ *Buffer )
{
	_extended_parser___ *Parser = NULL;
	sdr::size__ PonctualRed = Fill_( Buffer, Maximum, _Data, _Position );
	sdr::size__ CumulativeRed = PonctualRed;


	while ( ( CumulativeRed == 0 ) && ( Maximum > CumulativeRed ) && ( _CurrentParser != NULL ) ) {
		_Data.Init();
		_Position = 0;

		Parser = NULL;

		_Status = _Parser().Handle( Parser, _Data );

		while ( _Status == s_Pending ) {
#ifdef XPP_DBG
			if ( Parser != NULL )
				qRFwk();
#endif
			if ( _Parsers.Amount() != 0 ) {
				tht::sTID Owner = _CurrentParser->Flow().UndelyingFlow().Owner();
				utf::format__ Format = _CurrentParser->GetFormat();
				delete _CurrentParser;
				_CurrentParser = _Parsers.Pop();
				_CurrentParser->Flow().UndelyingFlow().Take( Owner );
				if ( Format != utf::f_Guess ) {
					if ( _Parser().GetFormat() == utf::f_Guess )
						_Parser().SetFormat( Format );
					else if ( _Parser().GetFormat() != Format )
						_Status = ( xpp::status__ )xml::eEncodingDiscrepancy;
				}

				if ( _Status == s_Pending )
					_Status = _Parser().Handle( Parser, _Data );
			} else {
				Maximum = 0;	// Pour sortir de la boucle.
				_Status = (xpp::status__)xml::sOK;
			}

		} 
		
		if ( _Status != sOK ) {
#if 0
			*Buffer = XTF_EOXC;	// Pour provoquer une erreur.
			CumulativeRed++;
#else
			_Position = _Data.Amount();
#endif
			break;
		}

		if ( Parser != NULL ) {
			_CurrentParser->Flow().UndelyingFlow().Take( Parser->Flow().UndelyingFlow().Owner() );
			_Parsers.Push( _CurrentParser );
			_CurrentParser = Parser;
		}

		PonctualRed = Fill_( Buffer, Maximum, _Data, _Position );
		CumulativeRed += PonctualRed;
	}

	return CumulativeRed;
}

status__ xpp::Process(
	xtf::extended_text_iflow__ &XFlow,
	const criterions___ &Criterions,
	xml::rWriter &Writer,
	context___ &Context )
{
	status__ Status = sOK;
qRH
	preprocessing_iflow___ PFlow;
	xtf::extended_text_iflow__ RelayXFlow;
qRB
	PFlow.Init( XFlow, Criterions );
	RelayXFlow.Init( PFlow, XFlow.Format() );

	if ( !Writer.Put( RelayXFlow ) ) {
		PFlow.GetContext( Context );
		Status = Context.Status;
	}

	if ( RelayXFlow.Format() != utf::f_Guess )
		XFlow.SetFormat( RelayXFlow.Format() );
qRR
qRT
qRE
	return Status;
}

static status__ Encrypt_(
	xml::parser___ &Parser,
	const str::string_ &CypherKey,
	str::string_ &Target,
	xtf::pos__ &Position )
{
	status__ Status = s_Undefined;
qRH
	flx::E_STRING_OFLOW___ Flow;
	cdgb64::encoding_oflow___ Encoder;
	crptgr::encrypt_oflow___ Encrypter;
qRB
	Flow.Init( Target );
	Encoder.Init( Flow, cdgb64::fOriginal );
	Encrypter.Init( Encoder, CypherKey );

	Status = RetrieveTree_( Parser, Encrypter );

	Position = Parser.GetCurrentPosition();
qRR
qRT
qRE
	return Status;
}

static status__ Encrypt_(
	xml::parser___ &Parser,
	const str::string_ &Namespace,
	const str::string_ &CypherKey,
	xml::rWriter &Writer,
	xtf::pos__ &Position )
{
	status__ Status = s_Undefined;
qRH
	str::string Tree;
	_qualified_preprocessor_directives___ Directives;
qRB
	Directives.Init( Namespace );

	Writer.PushTag( Directives.CypherTag );

	Writer.PutAttribute( CYPHER_TAG_METHOD_ATTRIBUTE, "basic" );

	Tree.Init();

	Status = Encrypt_( Parser, CypherKey, Tree, Position );

	Writer.PutValue( Tree );

	Writer.PopTag();
qRR
qRT
qRE
	return Status;
}

// <xpp::cypher ...
static status__ HandleCypherDirective_(
	const str::string_ &Namespace,
	xml::parser___ &Parser,
	xml::rWriter &Writer,
	xtf::pos__ &Position )
{
	status__ Status = sOK;
qRH
	str::string CypherKey;
	bso::bool__ Continue = true;
qRB
	CypherKey.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfAll & ~xml::tfSpecialAttribute ) ) {
		case xml::tAttribute:
			if ( Parser.AttributeName() == CYPHER_TAG_KEY_ATTRIBUTE ) {
				if ( CypherKey.Amount() != 0 )
					Status = sUnexpectedAttribute;
				else
					CypherKey = Parser.Value();
			} else
				Status = sUnexpectedAttribute;
			break;
		case xml::tStartTagClosed:
			if ( CypherKey.Amount() == 0 )
				Status = sMissingCypherKey;
			else if ( ( Status = Encrypt_( Parser, Namespace, CypherKey, Writer, Position ) ) == sOK )
				Continue = false;
			break;
		default:
			qRFwk();
			break;
		}

		if ( Status != sOK )
			Continue = false;
	}

qRR
qRT
qRE
	return Status;
}

status__ xpp::Encrypt(
	const str::string_ &Namespace,
	flw::iflow__ &IFlow,
	utf::format__ Format,
	xml::rWriter &Writer,
	context___ &Context )
{
	status__ Status = sOK;
qRH
	xtf::extended_text_iflow__ XFlow;
	bso::bool__ Continue = true;
	xml::parser___ Parser;
	_qualified_preprocessor_directives___ Directives;
qRB
	Directives.Init( Namespace );

	XFlow.Init( IFlow, Format );

	Parser.Init( XFlow, xml::ehKeep );

	while ( Continue && ( Status == sOK ) ) {
		switch( Parser.Parse( xml::tfAll ) ) {
		case xml::tProcessingInstruction:
			Writer.GetFlow() << Parser.DumpData();
			
			if ( Writer.GetOutfit() == xml::oIndent )
				Writer.GetFlow() << txf::nl;

			break;
		case xml::tStartTag:
			if ( Parser.TagName() == Directives.CypherTag ) {
				if ( ( Status = HandleCypherDirective_( Namespace, Parser, Writer, Context.Coordinates.Position ) ) != sOK )
					Context.Status = Status;
			} else
				Writer.PushTag( Parser.TagName() );
			break;
		case xml::tStartTagClosed:
			// Pour purger le 'Dump' d'un ventuel '>' en attente (gnant lors du relayage d'un commentaire.
			break;
		case xml::tAttribute:
		case xml::tSpecialAttribute:
			Writer.PutAttribute( Parser.AttributeName(), Parser.Value(), xml::Convert( Parser.AttributeDelimiter() ) );
			break;
		case xml::tValue:
			Writer.PutValue( Parser.Value() );
			break;
		case xml::tEndTag:
			Writer.PopTag();
			break;
		case xml::tComment:
		case xml::tCData:
			Writer.PutValue( "" );	// Pour mettre un ventuel '>' en attente.
			Writer.GetFlow() << Parser.DumpData();
			break;
		case xml::t_Processed:
			Continue = false;
			break;
		case xml::t_Error:
			Context.Status = Status = _Convert( Parser.Status() );
			Context.Coordinates.Position = XFlow.Position();
			break;
		default:
			qRFwk();
			break;
		}
	}
qRR
qRT
qRE
	return Status;
}

status__ xpp::Encrypt(
	const str::string_ &Namespace,
	flw::iflow__ &IFlow,
	xml::outfit__ Outfit,
	utf::format__ Format,
	txf::text_oflow__ &OFlow,
	context___ &Context )
{
	status__ Status = sOK;
qRH
	xml::rWriter Writer;
qRB
	Writer.Init( OFlow, Outfit, xml::e_None, xml::schKeep );

	Status = Encrypt( Namespace, IFlow, Format, Writer, Context );
qRR
qRT
qRE
	return Status;
}


status__ xpp::Process(
	xtf::extended_text_iflow__ &XFlow,
	const criterions___ &Criterions,
	xml::outfit__ Outfit,
	txf::text_oflow__ &OFlow,
	context___ &Context )
{
	status__ Status = sOK;
qRH
	xml::rWriter Writer;
qRB
	Writer.Init( OFlow, Outfit, xml::e_None, Criterions.Level, xml::schKeep );

	Status = Process( XFlow, Criterions, Writer, Context );
qRR
qRT
qRE
	return Status;
}

void xpp::Process(
	const str::string_ &In,
	xml::outfit__ Outfit,
	str::string_ &Out,
	const criterions___ &Criterions )
{
qRH
	flx::E_STRING_IFLOW__ IFlow;
	xtf::extended_text_iflow__ XFlow;
	flx::E_STRING_OFLOW___ OFlow;
	txf::text_oflow__ TFlow;
qRB
	IFlow.Init( In );
	XFlow.Init( IFlow, utf::f_Guess );
	OFlow.Init( Out );
	TFlow.Init( OFlow );

	xpp::Process( XFlow, Criterions, xml::oIndent, TFlow );
qRR
qRT
qRE
}
