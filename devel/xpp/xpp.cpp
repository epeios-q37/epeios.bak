/*
	'xpp.cpp' by Claude SIMON (http://zeusw.org/).

	'xpp' is part of the Epeios framework.

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

#define XPP__COMPILATION

#include "xpp.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "fnm.h"
#include "lcl.h"

using namespace xpp;

using xml::parser___;
using xml::token__;

#define DEFINE_TAG_NAME_ATTRIBUTE	"name"

#define EXPAND_TAG_SELECT_ATTRIBUTE	"select"
#define EXPAND_TAG_HREF_ATTRIBUTE	"href"

#define SET_TAG_NAME_ATTRIBUTE	"name"
#define SET_TAG_VALUE_ATTRIBUTE	"value"

#define IFEQ_TAG_SELECT_ATTRIBUTE	"select"
#define IFEQ_TAG_VALUE_ATTRIBUTE	"value"

#define CYPHER_TAG_KEY_ATTRIBUTE	"key"
#define CYPHER_TAG_METHOD_ATTRIBUTE	"method"

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
	CASE( UnknownAttribute );
	CASE( MissingNameAttribute );
	CASE( EmptyNameAttributeValue );
	CASE( MissingSelectOrHRefAttribute );
	CASE( EmptySelectOrHRefAttributeValue );
	CASE( MissingSelectAttribute );
	CASE( EmptySelectAttributeValue );
	CASE( MissingValueAttribute );
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
	default:
		ERRPrm();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

void xpp::GetMeaning(
	const coords___ &Coordinates,
	lcl::meaning_ &Meaning )
{
ERRProlog
	TOL_CBUFFER___ CBuffer;
	bso::integer_buffer__ IBuffer;
ERRBegin
	if ( Coordinates.FileName.Size() != 0 ) {
		Meaning.SetValue( XPP_NAME "_ErrorInFileAtLineColumn" );
		Meaning.AddTag( Coordinates.FileName.UTF8( CBuffer ) );
	} else
		Meaning.SetValue( XPP_NAME "_ErrorAtLineColumn" );

	Meaning.AddTag( bso::Convert( Coordinates.Position.Line, IBuffer ) );
	Meaning.AddTag( bso::Convert( Coordinates.Position.Column, IBuffer ) );
ERRErr
ERREnd
ERREpilog
}

void xpp::GetMeaning(
	const context___ &Context,
	lcl::meaning_ &Meaning )
{
ERRProlog
	lcl::meaning MeaningBuffer;
ERRBegin
	GetMeaning( Context.Coordinates, Meaning );

	MeaningBuffer.Init();

	if ( Context.Status < (xpp::status__)xml::s_amount )
		MeaningBuffer.SetValue( xml::GetLabel( (xml::status__)Context.Status ) );
	else
		MeaningBuffer.SetValue( GetLabel( Context.Status ) );

	Meaning.AddTag( MeaningBuffer );
ERRErr
ERREnd
ERREpilog
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
	sdr::datum__ *&Buffer,
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

static inline bso::bool__ BelongsToNamespace___(
	const str::string_ &Name,
	const str::string_ &NamespaceWithSeparator )
{
	if ( Name.Amount() == 0 )
		ERRPrm();

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
	const _qualified_preprocessor_directives___ &Directives )
{
	if ( BelongsToNamespace___( Name, Directives.NamespaceWithSeparator ) )
		if ( Directives.DefineTag_ == Name )
			return dDefine;
		else if ( Directives.ExpandTag == Name )
			return dExpand;
		else if ( Directives.IfeqTag == Name )
			return dIfeq;
		else if ( Directives.BlocTag == Name )
			return dBloc;
		else if ( Directives.CDataTag == Name )
			return dCData;
		else if ( Directives.SetTag == Name )
			return dSet;
		else if ( Directives.CypherTag == Name )
			return dCypher;
		else if ( Directives.AttributeAttribute == Name )
			return dAttribute;
		else
			return dUnknown;
	else
		return dNone;
}

static status__ AwaitingToken_(
	parser___ &Parser,
	token__ AwaitedToken,
	status__ StatusIfNotAwaitedToken )
{
	token__ Token = xml::t_Undefined;

	switch ( Token = Parser.Parse( xml::tfAllButUseless ) ) {
		case xml::t_Processed:
			ERRFwk();
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
ERRProlog
	TOL_CBUFFER___ Buffer;
ERRBegin
	Flow.Write( Data.Convert( Buffer ), Data.Amount() );
ERRErr
ERREnd
ERREpilog
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
				ERRLmt();
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
				ERRFwk();
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
			ERRFwk();
			break;
		default:
			ERRFwk();
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
ERRProlog
	flx::E_STRING_OFLOW___ Flow;
ERRBegin
	Flow.Init( Tree );

	Status = RetrieveTree_( Parser, Flow);
ERRErr
ERREnd
ERREpilog
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

status__ xpp::_extended_parser___::_HandleDefineDirective( _extended_parser___ *&Parser )	// 'Parser' est mis à 'NULL', ce qui est normal. 
{
	status__ Status = s_Undefined;
ERRProlog
	str::string Name, Content;
	xtf::pos__ Position;
ERRBegin
	Parser = NULL;

	Position = _Parser.GetCurrentPosition();

	Name.Init();
	Content.Init();

	if ( ( Status = GetDefineNameAndContent_( _Parser, Name, Content ) ) != sOK )
		ERRReturn;

	_Repository.Store( Name, Position, _LocalizedFileName, Content );
ERRErr
ERREnd
ERREpilog
	return Status;
}

enum expand_type__ {
	etMacro,
	etFile,
	et_amount,
	et_Undefined
};

static inline expand_type__ GetExpandTypeAndValue_(
	parser___ &Parser,
	str::string_ &Value,
	status__ &Status )	// Siginfiant seulement si valeur retournée == 'et_Undefined'.
{
	expand_type__ Type = et_Undefined;
ERRProlog
	str::string AttributeName;
ERRBegin
	AttributeName.Init();

	if ( ( Status = AwaitingToken_( Parser, xml::tAttribute, sMissingSelectOrHRefAttribute ) ) != sOK )
		ERRReturn;

	Value = Parser.Value();

	if ( Value.Amount() == 0 ) {
		Status = sEmptySelectOrHRefAttributeValue;
		ERRReturn;
	}

	if ( Parser.AttributeName() == EXPAND_TAG_HREF_ATTRIBUTE )
		Type = etFile;
	else if ( Parser.AttributeName() == EXPAND_TAG_SELECT_ATTRIBUTE )
		Type = etMacro;
	else {
		Status = sUnknownAttribute;
		ERRReturn;
	}

ERRErr
ERREnd
ERREpilog
	return Type;
}

status__ xpp::_extended_parser___::_HandleMacroExpand(
	const str::string_ &MacroName,
	_extended_parser___ *&Parser )
{
	status__ Status = s_Undefined;
ERRProlog
	str::string FileName;
	str::string Content;
	xtf::pos__ Position;
ERRBegin
	FileName.Init();
	Content.Init();

	if ( !_Repository.Get( MacroName, Position, FileName, Content ) ) {
		Status = sUnknownMacro;
		ERRReturn;
	}

	Parser = NewParser( _Repository, _Variables, _Directives );

	Status = Parser->_InitWithContent( Content, FileName, Position, _Directory, _CypherKey, _Parser.GetFormat() );
ERRErr
	if ( Parser != NULL ) {
		delete Parser;
		Parser = NULL;
	}
ERREnd
	if ( Status != sOK ) {
		if ( Parser != NULL ) {
			delete Parser;
			Parser = NULL;
		}
	}
ERREpilog
	return Status;
}

status__ xpp::_extended_parser___::_HandleFileExpand(
	const str::string_ &FileName,
	_extended_parser___ *&Parser )
{
	status__ Status = s_Undefined;
ERRProlog
ERRBegin
	Parser = NewParser( _Repository, _Variables, _Directives );

	Status = Parser->_InitWithFile( FileName, _Directory, _CypherKey, _Parser.GetFormat() );
ERRErr
	if ( Parser != NULL ) {
		delete Parser;
		Parser = NULL;
	}
ERREnd
	if ( Status != sOK ) {
		if ( Parser != NULL ) {
			delete Parser;
			Parser = NULL;
		}
	}
ERREpilog
	return Status;
}

status__ xpp::_extended_parser___::_HandleExpandDirective( _extended_parser___ *&Parser )
{
	status__ Status = s_Undefined;
ERRProlog
	expand_type__ Type = et_Undefined;
	str::string Value;
ERRBegin
	Value.Init();

	switch ( GetExpandTypeAndValue_( _Parser, Value, Status ) ) {
	case etMacro:
		Status = _HandleMacroExpand( Value, Parser );
		break;
	case etFile:
		Status = _HandleFileExpand( Value, Parser );
		break;
	case et_Undefined:
		// 'Status' initialisé par 'etExpandTypeAndValue_(...)'.
		break;
	default:
		ERRFwk();
		break;
	}

	if ( Status == sOK )
		Status = AwaitingToken_( _Parser, xml::tStartTagClosed, sUnexpectedAttribute );

	if ( Status == sOK )
		Status = AwaitingToken_( _Parser, xml::tEndTag, sMustBeEmpty );

	_Parser.PurgeDumpData();

ERRErr
ERREnd
ERREpilog
	return Status;
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

			if ( ( Status = AwaitingToken_( Parser, xml::tAttribute, sMissingValueAttribute ) ) == sOK ) {
				if ( Parser.AttributeName() == SET_TAG_VALUE_ATTRIBUTE )
					Value.Append( Parser.Value() );
				else
					Status = sUnknownAttribute;
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
					Status = sUnknownAttribute;
			}
		}
	}

	return Status;
}

status__ xpp::_extended_parser___::_HandleSetDirective( _extended_parser___ *&Parser )	// 'Parser' est mis à 'NULL', ce qui est normal. 
{
	status__ Status = s_Undefined;
ERRProlog
	str::string Name, Value;
ERRBegin
	Parser = NULL;

	Name.Init();
	Value.Init();

	if ( ( Status = GetSetNameAndValue_( _Parser, Name, Value ) ) != sOK )
		ERRReturn;

	_Variables.Set( Name, Value );

	if ( Status == sOK )
		Status = AwaitingToken_( _Parser, xml::tStartTagClosed, sUnexpectedAttribute );

	if ( Status == sOK )
		Status = AwaitingToken_( _Parser, xml::tEndTag, sMustBeEmpty );

	_Parser.PurgeDumpData();

ERRErr
ERREnd
ERREpilog
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
					Status = sUnknownAttribute;
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
					Status = sUnknownAttribute;
			}
		}
	}

	return Status;
}


status__ xpp::_extended_parser___::_HandleIfeqDirective( _extended_parser___ *&Parser )
{
	status__ Status = s_Undefined;
ERRProlog
	str::string Name, ExpectedValue, TrueValue, Content;
	xtf::pos__ Position;
ERRBegin
	Parser = NULL;

	Name.Init();
	ExpectedValue.Init();

	if ( ( Status = GetIfeqSelectAndValue_( _Parser, Name, ExpectedValue ) ) != sOK )
		ERRReturn;

	if ( ( Status = AwaitingToken_( _Parser, xml::tStartTagClosed, sUnexpectedAttribute ) ) != sOK )
		ERRReturn;


	Content.Init();

	Position = _Parser.GetCurrentPosition();

	if ( ( Status = RetrieveTree_( _Parser, Content ) ) != sOK)
		ERRReturn;


	TrueValue.Init();

	if ( ( _Variables.Get( Name, TrueValue ) ) && ( ExpectedValue == TrueValue ) ) {
		Parser = NewParser( _Repository, _Variables, _Directives );

		Status = Parser->_InitWithContent( Content, _LocalizedFileName, Position, _Directory, _CypherKey, _Parser.GetFormat() );
	}
ERRErr
	if ( Parser != NULL ) {
		delete Parser;
		Parser = NULL;
	}
ERREnd
	if ( Status != sOK ) {
		if ( Parser != NULL ) {
			delete Parser;
			Parser = NULL;
		}
	}
ERREpilog
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
	status__ &Status )	// Siginfiant seulement si valeur retournée == 'et_Undefined'.
{
	cypher_mode__ Mode = cm_Undefined;
ERRProlog
	str::string AttributeName;
ERRBegin
	AttributeName.Init();

	if ( ( Status = AwaitingToken_( Parser, xml::tAttribute, sMissingKeyOrFormatAttribute ) ) != sOK )
		ERRReturn;

	if ( Parser.AttributeName() == CYPHER_TAG_KEY_ATTRIBUTE )
		Mode = cmOverriden;
	else if ( Parser.AttributeName() == CYPHER_TAG_METHOD_ATTRIBUTE )
		Mode = cmEncrypted;
	else {
		Status = sUnknownAttribute;
		ERRReturn;
	}

	Value = Parser.Value();
ERRErr
ERREnd
ERREpilog
	return Mode;
}

status__ xpp::_extended_parser___::_HandleCypherDecryption(
	const str::string_ &Version,
	_extended_parser___ *&Parser )
{
	Parser = NewParser( _Repository, _Variables, _Directives );

	return Parser->_InitCypher( _Parser.Flow().UndelyingFlow(), _LocalizedFileName, Position(), _Directory, _CypherKey, _Parser.GetFormat() );
}

status__ xpp::_extended_parser___::_HandleCypherOverride(
	const str::string_ &CypherKey,
	_extended_parser___ *&Parser )
{

	if ( ( _CypherKey.Amount() != 0  ) && ( _CypherKey !=  CypherKey ) )
		return sBadCypherKey;
	else {
		Parser = NewParser( _Repository, _Variables, _Directives );

		return Parser->Init( _Parser.Flow(), _LocalizedFileName, _Directory, _CypherKey );
	}
}


status__ xpp::_extended_parser___::_HandleCypherDirective( _extended_parser___ *&Parser )
{
	status__ Status = s_Undefined;
ERRProlog
	cypher_mode__ Mode = cm_Undefined;
	str::string Value;
ERRBegin
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
	case et_Undefined:
		// 'Status' initialisé par 'etCypherModeAndValue_(...)'.
		break;
	default:
		ERRFwk();
		break;
	}

	if ( Status == sOK )
		Status = AwaitingToken_( _Parser, xml::tStartTagClosed, sUnexpectedAttribute );

	_Parser.PurgeDumpData();
ERRErr
ERREnd
ERREpilog
	return Status;
}


status__ xpp::_extended_parser___::_HandlePreprocessorDirective(
	int Directive,
	_extended_parser___ *&Parser )
{
	Parser = NULL;

	switch ( Directive ) {
	case dNone:
		ERRPrm();
		break;
	case dUnknown:
		return sUnknownDirective;
		break;
	case dDefine:
		return _HandleDefineDirective( Parser );
		break;
	case dBloc:
		ERRFwk();	// Traité en amont.
		break;
	case dCData:
		ERRFwk();	// Traité en amont.
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
		ERRFwk();
		break;
	}

	ERRFwk();

	return s_Undefined;	// Pour éviter un 'warning'.

}

static sdr::row__ ExtractAttributeName_(
	const str::string_ &Source,
	str::string_ &Value )
{
	sdr::row__ Row = Source.First();

	while ( ( Row != E_NIL )
		    && ( isalnum( Source( Row ) ) || ( Source( Row ) == ':' ) || ( Source( Row ) == '_' ) ) ) {
		Value.Append( Source( Row ) );
		Row = Source.Next( Row );
	}

	return Row;
}

status__ xpp::_extended_parser___::_HandleAttributeDirective(
	const str::string_ &Parameters,
	_extended_parser___ *&Parser,
	str::string_ &Data )
{
	status__ Status = s_Undefined;
ERRProlog
	str::string AttributeName, MacroName;
	sdr::row__ Row = E_NIL;
ERRBegin
	AttributeName.Init();

	Row = ExtractAttributeName_( Parameters, AttributeName );

	if ( ( Row == E_NIL )
		  || ( AttributeName.Amount() == 0 )
		  || ( Parameters( Row ) != ',' ) ) {
		Status = sBadAttributeDefinitionSyntax;
		ERRReturn;
	}

	MacroName.Init( Parameters );
	MacroName.Remove( MacroName.First(), *Row + 1 );

	Data.Append( AttributeName );
	Data.Append( "=\"" );
	Status = this->_HandleMacroExpand( MacroName, Parser );

	_AttributeDefinitionInProgress = true;
ERRErr
ERREnd
ERREpilog
	return Status;
}

status__ xpp::_extended_parser___::_InitWithFile(
	const str::string_ &FileName,
	const str::string_ &Directory,
	const str::string_ &CypherKey,
	utf::format__ Format )
{
	status__ Status = s_Undefined;
ERRProlog
	str::string LocalizedFileNameBuffer;
	str::string LocationBuffer;
	fnm::name___ LocalizedFileName;
	fnm::name___ Location;
ERRBegin
	LocalizedFileName.Init();
	fnm::BuildFileName( fnm::name___( Directory ), fnm::name___( FileName ), fnm::name___( "" ), LocalizedFileName );

	if ( _FFlow.Init( fnm::CorrectLocation( LocalizedFileName ), fil::mReadOnly, err::hUserDefined ) != fil::sSuccess ) {
		Status = sUnableToOpenFile;
		ERRReturn;
	}

	_XFlow.Init( _FFlow, Format );

	fnm::GetLocation( LocalizedFileName, Location );

	LocalizedFileNameBuffer.Init();
	LocationBuffer.Init();

	if ( ( Status = Init( _XFlow, LocalizedFileName.UTF8( LocalizedFileNameBuffer ), Location.UTF8( LocationBuffer ), CypherKey ) ) != sOK )
		ERRReturn;

	_IgnorePreprocessingInstruction = true;

	Status = sOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

status__ xpp::_extended_parser___::_InitWithContent(
	const str::string_ &Content,
	const str::string_ &NameOfTheCurrentFile,
	const xtf::pos__ &Position,
	const str::string_ &Directory,
	const str::string_ &CypherKey,
	utf::format__ Format )
{
	status__ Status = s_Undefined;
ERRProlog
ERRBegin
	_MacroContent.Init( Content );
	_SFlow.Init( _MacroContent );
//	_SFlow.EOFD( XTF_EOXT );

	_XFlow.Init( _SFlow, Format, Position );

	if ( ( Status = Init( _XFlow, NameOfTheCurrentFile, Directory, CypherKey ) ) != sOK )
		ERRReturn;

	_IgnorePreprocessingInstruction = false;
ERRErr
ERREnd
ERREpilog
	return Status;
}

status__ xpp::_extended_parser___::_InitCypher(
	flw::iflow__ &Flow,
	const str::string_ &FileName,
	const xtf::pos__ &Position,
	const str::string_ &Directory,
	const str::string_ &CypherKey,
	utf::format__ Format )
{
	_Decoder.Init( Flow );
	_Decrypter.Init( _Decoder, CypherKey );
	_XFlow.Init( _Decrypter, Format, Position );

	return Init( _XFlow, FileName, Directory, CypherKey );
}


static bso::bool__ StripHeadingSpaces_(
	xml::token__ PreviousToken,
	const xml::parser___ &Parser,
	const str::string_ &NamespaceWithSeparator )
{
	return ( PreviousToken == xml::tValue )
		   || ( ( PreviousToken == xml::tEndTag )
		      && ( BelongsToNamespace___( Parser.TagName(), NamespaceWithSeparator )
			  || ( Parser.Token() == xml::tEndTag ) ) );
}

static void StripHeadingSpaces_( str::string_ &Data )
{
	while ( ( Data.First() != E_NIL ) && ( isspace( Data( Data.First() ) ) ) )
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
			Directive = GetDirective_( _Parser.TagName(), _Directives );

			if ( Directive == dCData ) {
				if ( _CDataNesting == CDATA_NESTING_MAX )
					ERRLmt();

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
					Continue = true;
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
				switch ( GetDirective_( _Parser.TagName(), _Directives ) ) {
				case dNone:
					switch ( GetDirective_( _Parser.AttributeName(), _Directives ) ) {
					case dNone:
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
				case dCData:
					Status = sUnexpectedAttribute;
					break;
				default:
					ERRFwk();
					break;
				}
			} else
				Status = sOK;
			break;
		case xml::tSpecialAttribute:
			if ( _CDataNesting == 0 ) {
				switch ( GetDirective_( _Parser.TagName(), _Directives ) ) {
				case dNone:
					break;
				case dBloc:
				case dCData:
					_Parser.PurgeDumpData();
					// L'attribut n'est pas répercuté sur le flux de sortie.
					break;
				default:
					ERRFwk();
					break;
				}
			}

			Status = sOK;
			break;
		case xml::tStartTagClosed:
			switch ( GetDirective_( _Parser.TagName(), _Directives ) ) {
			case dNone:
				Status = sOK;
				break;
			case dCData:
				if ( _CDataNesting == 0 )
					Continue = true;
				else
					Status = sOK;
				_CDataNesting++;
				Data.Append( "<![CDATA[" );
				StripHeadingSpaces = true;
				break;
			case dBloc:
				if ( _CDataNesting == 0 )
					Continue = true;
				else
					Status = sOK;
				break;
			default:
				ERRFwk();
				break;
			}
			break;
		case xml::tEndTag:
				switch ( GetDirective_( _Parser.TagName(), _Directives ) ) {
				case dNone:
					if ( _CDataNesting == 0 )
						StripHeadingSpaces = StripHeadingSpaces_( PreviousToken, _Parser, _Directives.NamespaceWithSeparator );
					Status = sOK;
					break;
				case dCData:
					switch( _CDataNesting ) {
					case 0:
						ERRFwk();
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
				case dCypher:
					if ( _CDataNesting == 0 ) {
						StripHeadingSpaces = StripHeadingSpaces_( PreviousToken, _Parser, _Directives.NamespaceWithSeparator );
						Continue = true;
					} else
						Status = sOK;
					break;
				default:
					ERRFwk();
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
			ERRFwk();
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
	sdr::datum__ *Buffer )
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
				ERRFwk();
#endif
			if ( _Parsers.Amount() != 0 ) {
				utf::format__ Format = _CurrentParser->GetFormat();
				delete _CurrentParser;
				_CurrentParser = _Parsers.Pop();
				if ( Format != utf::f_Guess )
					if (_Parser().GetFormat() == utf::f_Guess )
						_Parser().SetFormat( Format );
					else if ( _Parser().GetFormat() != Format )
						_Status = (xpp::status__)xml::eEncodingDiscrepancy;

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
	xml::writer_ &Writer,
	context___ &Context )
{
	status__ Status = sOK;
ERRProlog
	preprocessing_iflow___ PFlow;
	xml::token__ Token = xml::t_Undefined;
	bso::bool__ Continue = true;
	xml::parser___ Parser;
	xtf::extended_text_iflow__ RelayXFlow;
ERRBegin
	PFlow.Init( XFlow, Criterions );
	RelayXFlow.Init( PFlow, XFlow.Format() );

	Parser.Init( RelayXFlow, xml::ehKeep );

	while ( Continue ) {
		switch( Parser.Parse( xml::tfAll & ~xml::tfStartTagClosed ) ) {
		case xml::tProcessingInstruction:
			Writer.GetFlow() << Parser.DumpData();
			
			if ( Writer.GetOutfit() == xml::oIndent )
				Writer.GetFlow() << txf::nl;

			break;
		case xml::tStartTag:
			Writer.PushTag( Parser.TagName() );
			break;
		case xml::tAttribute:
		case xml::tSpecialAttribute:
			Writer.PutAttribute( Parser.AttributeName(), Parser.Value() );
			break;
		case xml::tValue:
			Writer.PutValue( Parser.Value() );
			break;
		case xml::tEndTag:
			Writer.PopTag();
			break;
		case xml::tCData:
			Writer.PutCData( Parser.Value() );
			break;
		case xml::t_Processed:
			Continue = false;
			break;
		case xml::t_Error:
			PFlow.GetContext( Context );
			Status = Context.Status;
			Continue = false;
			break;
		default:
			ERRFwk();
			break;
		}
	}

	if ( RelayXFlow.Format() != utf::f_Guess )
		XFlow.SetFormat( RelayXFlow.Format() );
ERRErr
ERREnd
ERREpilog
	return Status;
}

static status__ Encrypt_(
	xml::parser___ &Parser,
	const str::string_ &CypherKey,
	str::string_ &Target,
	xtf::pos__ &Position )
{
	status__ Status = s_Undefined;
ERRProlog
	flx::E_STRING_OFLOW___ Flow;
	cdgb64::encoding_oflow___ Encoder;
	crptgr::encrypt_oflow___ Encrypter;
ERRBegin
	Flow.Init( Target );
	Encoder.Init( Flow );
	Encrypter.Init( Encoder, CypherKey );

	Status = RetrieveTree_( Parser, Encrypter );

	Position = Parser.GetCurrentPosition();
ERRErr
ERREnd
ERREpilog
	return Status;
}

static status__ Encrypt_(
	xml::parser___ &Parser,
	const str::string_ &Namespace,
	const str::string_ &CypherKey,
	xml::writer_ &Writer,
	xtf::pos__ &Position )
{
	status__ Status = s_Undefined;
ERRProlog
	str::string Tree;
	_qualified_preprocessor_directives___ Directives;
ERRBegin
	Directives.Init( Namespace );

	Writer.PushTag( Directives.CypherTag );

	Writer.PutAttribute( CYPHER_TAG_METHOD_ATTRIBUTE, "basic" );

	Tree.Init();

	Status = Encrypt_( Parser, CypherKey, Tree, Position );

	Writer.PutValue( Tree );

	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
	return Status;
}

// <xpp::cypher ...
static status__ HandleCypherDirective_(
	const str::string_ &Namespace,
	xml::parser___ &Parser,
	xml::writer_ &Writer,
	xtf::pos__ &Position )
{
	status__ Status = s_Undefined;
ERRProlog
	str::string CypherKey;
	bso::bool__ CypheringComplete = false;
	bso::bool__ Continue = true;
ERRBegin
	CypherKey.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfAll & ~xml::tfSpecialAttribute ) ) {
		case xml::tAttribute:
			if ( Parser.AttributeName() == CYPHER_TAG_KEY_ATTRIBUTE ) {
				if ( CypherKey.Amount() != 0 ) {
					Status = sUnexpectedAttribute;
					ERRReturn;
				}

				CypherKey = Parser.Value();
			} else {
				Status = sUnexpectedAttribute;
				ERRReturn;
			}
			break;
		case xml::tStartTagClosed:
			if ( CypherKey.Amount() == 0 ) {
				Status = sMissingCypherKey;
				ERRReturn;
			}
			if ( ( Status = Encrypt_( Parser, Namespace, CypherKey, Writer, Position ) ) != sOK )
				ERRReturn;
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
	return Status;
}

status__ xpp::Encrypt(
	const str::string_ &Namespace,
	flw::iflow__ &IFlow,
	utf::format__ Format,
	xml::writer_ &Writer,
	context___ &Context )
{
	status__ Status = s_Undefined;
ERRProlog
	xtf::extended_text_iflow__ XFlow;
	xml::token__ Token = xml::t_Undefined;
	bso::bool__ Continue = true;
	xml::parser___ Parser;
	_qualified_preprocessor_directives___ Directives;
ERRBegin
	Directives.Init( Namespace );

	XFlow.Init( IFlow, Format );

	Parser.Init( XFlow, xml::ehKeep );

	while ( Continue ) {
		switch( Parser.Parse( xml::tfAll ) ) {
		case xml::tProcessingInstruction:
			Writer.GetFlow() << Parser.DumpData();
			
			if ( Writer.GetOutfit() == xml::oIndent )
				Writer.GetFlow() << txf::nl;

			break;
		case xml::tStartTag:
			if ( Parser.TagName() == Directives.CypherTag ) {
				if ( ( Status = HandleCypherDirective_( Namespace, Parser, Writer, Context.Coordinates.Position ) ) != sOK ) {
					Context.Status = Status;
					ERRReturn;
				}
			} else
				Writer.PushTag( Parser.TagName() );
			break;
		case xml::tStartTagClosed:
			// Pour purger le 'Dump' d'un éventuel '>' en attente (gênant lors du relayage d'un commentaire.
			break;
		case xml::tAttribute:
		case xml::tSpecialAttribute:
			Writer.PutAttribute( Parser.AttributeName(), Parser.Value() );
			break;
		case xml::tValue:
			Writer.PutValue( Parser.Value() );
			break;
		case xml::tEndTag:
			Writer.PopTag();
			break;
		case xml::tComment:
		case xml::tCData:
			Writer.PutValue( "" );	// Pour mettre un éventuel '>' en attente.
			Writer.GetFlow() << Parser.DumpData();
			break;
		case xml::t_Processed:
			Continue = false;
			break;
		case xml::t_Error:
			Context.Status = Status = _Convert( Parser.Status() );
			Context.Coordinates.Position = XFlow.Position();
			ERRReturn;
			break;
		default:
			ERRFwk();
			break;
		}
	}

	Status = sOK;
ERRErr
ERREnd
ERREpilog
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
ERRProlog
	xml::writer Writer;
ERRBegin
	Writer.Init( OFlow, Outfit, xml::e_None, xml::schKeep );

	Status = Encrypt( Namespace, IFlow, Format, Writer, Context );
ERRErr
ERREnd
ERREpilog
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
ERRProlog
	xml::writer Writer;
ERRBegin
	Writer.Init( OFlow, Outfit, xml::e_None, xml::schKeep );

	Status = Process( XFlow, Criterions, Writer, Context );
ERRErr
ERREnd
ERREpilog
	return Status;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xpppersonnalization
{
public:
	xpppersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xpppersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static xpppersonnalization Tutor;
