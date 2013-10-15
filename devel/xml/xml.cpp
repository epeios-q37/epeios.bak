/*
	'xml.cpp' by Claude SIMON (http://zeusw.org/).

	'xml' is part of the Epeios framework.

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

#define XML__COMPILATION

#include "xml.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "lstctn.h"
#include "flx.h"
#include "flf.h"
#include "fnm.h"
#include "lcl.h"

using namespace xml;

#define CASE( m )\
	case s##m:\
	return XML_NAME "_" #m;\
	break

const char *xml::GetLabel( status__ Status )
{
	if ( ( Status >= s_FirstXTFError ) && ( Status < s_FirstNonXTFError ) )
		return xtf::GetLabel( (xtf::error__)( Status - s_FirstXTFError ) );

	switch( Status ) {
	CASE( OK );
	CASE( UnexpectedEOF );
	CASE( UnknownEntity );
	CASE( MissingEqualSign );
	CASE( BadAttributeValueDelimiter );
	CASE( UnexpectedCharacter );
	CASE( EmptyTagName );
	CASE( MismatchedTag );
	default:
		ERRPrm();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

#undef CASE

void xml::GetMeaning(
	status__ Status,
	const pos__ &Position,
	lcl::meaning_ &Meaning )
{
ERRProlog
	lcl::meaning MeaningBuffer;
	bso::integer_buffer__ IBuffer;
ERRBegin
	Meaning.SetValue( XML_NAME "_ErrorAtLineColumn" );

	MeaningBuffer.Init();
	MeaningBuffer.SetValue( GetLabel( Status ) );

	Meaning.AddTag( MeaningBuffer );

	Meaning.AddTag( bso::Convert( Position.Line, IBuffer ) );
	Meaning.AddTag( bso::Convert( Position.Column, IBuffer ) );
ERRErr
ERREnd
ERREpilog
}

static status__ SkipSpaces_( _flow___ &Flow )
{
	while ( !Flow.EndOfFlow() && isspace( Flow.View_() ) )
		Flow.Get_();

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	return sOK;
}

static status__ HandleProcessingInstruction_( _flow___ &Flow )	// Gère aussi le prologue '<?xml ... ?>'
{
	if ( Flow.Get_() != '?' )
		ERRDta();

	while ( !Flow.EndOfFlow() && ( Flow.Get_() != '>' ) );

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	return sOK;
}

static status__ Test_(
	_flow___ &Flow,
	const char C )
{
	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	if ( Flow.Get_() != C )
		return sUnexpectedCharacter;

	return sOK;
}

static status__ Test_(
	_flow___ &Flow,
	const char *S )
{
	status__ Status = sOK;

	while ( ( Status == sOK ) && ( *S != 0 ) ) {
		Status = Test_( Flow, *S );
		S++;
	}

	return Status;
}

static status__ GetComment_(
	_flow___ &Flow,
	str::string_ &Content )
{
	status__ Status = s_Undefined;
	bso::bool__ Continue = true;

	if ( Flow.Get_() != '-' )
		ERRDta();

	if ( ( Status = Test_( Flow, '-' ) ) != sOK )
		return Status;

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	while ( Continue ) {
		while ( !Flow.EndOfFlow() && ( Flow.View_() != '-' ) ) {
			xtf::utf__ UTF;

			UTF.Init();

			Flow.Get( UTF );
			Content.Append( (bso::char__ *)UTF.Data, UTF.Size );
		}
		
		if ( Flow.EndOfFlow() )
			return sUnexpectedEOF;

		Flow.Get_();	// Pour passer le '-' de début de fin de commentaire.

		if ( Flow.EndOfFlow() )
			return sUnexpectedEOF;

		if ( Flow.View_() == '-' ) {
			Flow.Get_();

			if ( Flow.EndOfFlow() )
				return sUnexpectedEOF;

			if ( Flow.View_() == '>' ) {
				Flow.Get_();

				Continue = false;
			} else
				Content.Append( "--" );
		}
	}

	return sOK;
}

static status__ GetCData_(
	_flow___ &Flow,
	str::string_ &Content )
{
	status__ Status = s_Undefined;
	bso::bool__ Continue = true;

	if ( Flow.Get_() != '[' )
		ERRDta();

	if ( ( Status = Test_( Flow, "CDATA[" ) ) != sOK )
		return Status;

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	while ( Continue ) {
		while ( !Flow.EndOfFlow() && ( Flow.View_() != ']' ) )
		{
			xtf::utf__ UTF;

			UTF.Init();

			Flow.Get( UTF );

			Content.Append( (bso::char__ *)UTF.Data, UTF.Size );
		}

		if ( Flow.EndOfFlow() )
			return sUnexpectedEOF;

		Flow.Get_();	// Pour passer le ']' de début de fin de 'CDATA'.

		if ( Flow.EndOfFlow() )
			return sUnexpectedEOF;

		if ( Flow.View_() == ']' ) {
			Flow.Get_();

			if ( Flow.EndOfFlow() )
				return sUnexpectedEOF;

			if ( Flow.View_() == '>' ) {
				Flow.Get_();

				Continue = false;
			} else
				Content.Append( "]]" );
		}
	}

	return sOK;
}

static status__ GetCommentOrCData_(
	_flow___ &Flow,
	str::string_ &Content,
	bso::bool__ &IsCData )
{
	bso::bool__ Continue = true;

	if ( Flow.Get_() != '!' )
		ERRDta();

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	if ( Flow.View_() == '-' ) {
		IsCData = false;
		return GetComment_( Flow, Content );
	} else if ( Flow.View_() == '[' ) {
		IsCData = true;
		return GetCData_( Flow, Content );
	}	else
		return sUnexpectedCharacter;
}

static sdr::size__ GetId_(
	_flow___ &Flow,
	str::string_ &Id )
{
	bso::size__ Size = 0;

	while ( !Flow.EndOfFlow() && ( isalnum( Flow.View_() ) || Flow.View_() == ':' || Flow.View_() == '_' || Flow.View_() == '-' ) ) {
		xtf::utf__ UTF;

		UTF.Init();

		Flow.Get( UTF );

		Id.Append( (bso::char__ *)UTF.Data, UTF.Size );

		Size += UTF.Size;
	}

	return Size;
}

static inline sdr::size__ GetName_( 
	_flow___ &Flow,
	str::string_ &Name )
{
	return GetId_( Flow, Name );
}

enum entity_handling_state__ {
	ehsStart,
	ehsL,
	ehsLT,
	ehsG,
	ehsGT,
	ehsA,
	ehsAM,
	ehsAMP,
	ehsAP,
	ehsAPO,
	ehsAPOS,
	ehsQ,
	ehsQU,
	ehsQUO,
	ehsQUOT,
	ehs_amount,
	ehs_Undefined
};

#define ENTITY_ERROR_VALUE	0

static unsigned char HandleEntity_( _flow___ &Flow )
{
	entity_handling_state__ State = ehsStart;

	while ( !Flow.EndOfFlow() ) {

		switch ( State ) {
		case ehsStart:
			switch( Flow.Get_() ) {
			case 'l':
				State = ehsL;
				break;
			case 'g':
				State = ehsG;
				break;
			case 'a':
				State = ehsA;
				break;
			case 'q':
				State = ehsQ;
				break;
			default:
				return ENTITY_ERROR_VALUE;
				break;
			}
			break;
		case ehsL:
			if ( Flow.Get_() != 't' )
				return ENTITY_ERROR_VALUE;

			State = ehsLT;
			break;
		case ehsLT:
			if ( Flow.Get_() != ';' )
				return ENTITY_ERROR_VALUE;

			return '<';
			break;
		case ehsG:
			if ( Flow.Get_() != 't' )
				return ENTITY_ERROR_VALUE;

			State = ehsGT;
			break;
		case ehsGT:
			if ( Flow.Get_() != ';' )
				return ENTITY_ERROR_VALUE;

			return '>';
			break;
		case ehsA:
			switch ( Flow.Get_() ) {
			case 'm':
				State = ehsAM;
				break;
			case 'p':
				State = ehsAP;
				break;
			default:
				return ENTITY_ERROR_VALUE;
				break;
			}
			break;
		case ehsAM:
			if ( Flow.Get_() != 'p' )
				return ENTITY_ERROR_VALUE;

			State = ehsAMP;
			break;
		case ehsAMP:
			if ( Flow.Get_() != ';' )
				return ENTITY_ERROR_VALUE;

			return '&';
			break;
		case ehsAP:
			if ( Flow.Get_() != 'o' )
				return ENTITY_ERROR_VALUE;

			State = ehsAPO;
			break;
		case ehsAPO:
			if ( Flow.Get_() != 's' )
				return ENTITY_ERROR_VALUE;

			State = ehsAPOS;
			break;
		case ehsAPOS:
			if ( Flow.Get_() != ';' )
				return ENTITY_ERROR_VALUE;

			return '\'';
			break;
		case ehsQ:
			if ( Flow.Get_() != 'u' )
				return ENTITY_ERROR_VALUE;

			State = ehsQU;
			break;
		case ehsQU:
			if ( Flow.Get_() != 'o' )
				return ENTITY_ERROR_VALUE;

			State = ehsQUO;
		case ehsQUO:
			if ( Flow.Get_() != 't' )
				return ENTITY_ERROR_VALUE;

			State = ehsQUOT;
		case ehsQUOT:
			if ( Flow.Get_() != ';' )
				return ENTITY_ERROR_VALUE;

			return '"';
			break;
		default:
				return ENTITY_ERROR_VALUE;
			break;
		}
	}

	return ENTITY_ERROR_VALUE;
}

static status__ GetValue_(
	_flow___ &Flow,
	bso::char__ Delimiter,
	bso::bool__ ErrorIfSpaceInValue,
	entities_handling__ EntitiesHandling,
	str::string_ &Value,
	bso::bool__ &OnlySpaces )
{
	unsigned char C;
	OnlySpaces = true;

	while ( !Flow.EndOfFlow() && ( Flow.View_() != Delimiter ) ) {

		xtf::utf__ UTF;

		UTF.Init();

		if ( !isspace( C = Flow.Get( UTF ) ) )
			OnlySpaces = false;
		else if ( !OnlySpaces && ErrorIfSpaceInValue && ( C != ' ' ) )
			return sUnexpectedCharacter;

		if ( ( C == '&' ) && ( EntitiesHandling == ehReplace ) ) {
			C = HandleEntity_( Flow );

			if ( C == ENTITY_ERROR_VALUE )
				return sUnknownEntity;

			UTF.Data[0] = C;
			UTF.Size = 1;
		}

		Value.Append( (bso::char__ *)UTF.Data, UTF.Size );
	}

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	return sOK;
}

inline status__ GetAttributeValue_(
	_flow___ &Flow,
	char Delimiter,
	entities_handling__ EntitiesHandling,
	str::string_ &Value )
{	
	bso::bool__ Dummy;

	return GetValue_( Flow, Delimiter, true, EntitiesHandling, Value, Dummy );
}

#define HANDLE( F )\
	if ( ( Status = ( F ) ) != sOK )\
		return ( Status );\
	else\
		Status = s_Undefined;

static status__ GetAttribute_(
	_flow___ &Flow,
	entities_handling__ EntitiesHandling,
	str::string_ &Name,
	str::string_ &Value )
{
	char Delimiter;
	status__ Status = s_Undefined;

	if ( GetName_( Flow, Name ) == 0 ) {
		Flow.Get_();	// Pour ajuster les coordonnées de l'erreur.
		return sUnexpectedCharacter;
	}

	HANDLE( SkipSpaces_( Flow ) );

	if ( Flow.Get_() != '=' )
		return sMissingEqualSign;

	HANDLE( SkipSpaces_( Flow ) );

	Delimiter = Flow.Get_();

	if ( ( Delimiter != '"' ) && ( Delimiter != '\'' ) )
		return sBadAttributeValueDelimiter;

	HANDLE( GetAttributeValue_( Flow, Delimiter, EntitiesHandling, Value ) );

	Flow.Get_();	// To skip the '"' or '''.
/*
	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	if ( !isspace( Flow.View() ) && ( Flow.View() != '/' ) && ( Flow.View() != '>' ) )
		return sUnexpectedCharacter;

	HANDLE( SkipSpaces_( Flow ) );
*/
	return sOK;
}

inline static status__ GetTagValue_(
	_flow___ &Flow,
	str::string_ &Value,
	entities_handling__ EntitiesHandling,
	bso::bool__ &OnlySpaces )
{
	return GetValue_( Flow, '<', false, EntitiesHandling, Value, OnlySpaces );
}

#define XMLNS	"xmlns"

inline static bso::bool__ IsSpecialAttribute_( const str::string_ &Attribute )
{
	bso::size__ Length = strlen( XMLNS );

	if ( Attribute.Amount() < Length )
		return false;

	if ( str::Compare( str::string( XMLNS ), Attribute, 0, 0, Length ) )
		return false;

	if ( ( Attribute.Amount() > Length ) && ( Attribute( Length ) != ':' ) )
		return false;

	return true;
}

#undef HANDLE

#define HANDLE( F )\
	if ( ( _Status = ( F ) ) != sOK )\
	{\
		_Token = t_Error;\
		ERRReturn;\
	}

#define RETURN( V )\
	{\
		_Status = V;\
		_Token = t_Error;\
		ERRReturn;\
	}

token__ xml::parser___::Parse( int TokenToReport )
{
ERRProlog
	bso::bool__ OnlySpaces = false, Continue = true, TEOX = true;	// 'TEOX' : Test EOX.
ERRBegin

	_Flow.Purge();

	while ( Continue ) {
		 TEOX = true;

		switch ( _Token ) {
		case tStartTagClosed:
			TEOX = false;
			break;
		case tEndTag:
			if ( _Tags.Amount() <= 1 )
				TEOX = false;
			break;
		default:
			break;
		}

		if ( TEOX && _Flow.EndOfFlow() )
			if ( _Token != tEndTag )
				RETURN( sUnexpectedEOF );

		switch ( _Context ) {
		case cHeaderExpected:

			HANDLE( SkipSpaces_( _Flow ) );

			switch ( _Token ) {
			case t_Undefined:
				if ( _Flow.EndOfFlow() )
					Continue = false;	// Fichier vide ou presque.
				else if ( _Flow.View_() != '<' )
					RETURN( sUnexpectedCharacter )
				else {
					_Flow.Get_();	// Pour enlever le '<'.

					if ( _Flow.View_() == '?' ) {
						HANDLE( HandleProcessingInstruction_( _Flow ) );

						_Token = tProcessingInstruction;

						if ( ( 1 << _Token ) & TokenToReport )
							Continue = false;

					} else {
						_Context = cTagConfirmed;
					}
				}
				break;
			case tProcessingInstruction:
				_Token = t_Undefined;

				HANDLE( SkipSpaces_( _Flow ) );

				_Context = cTagExpected;
				break;
			default:
				ERRFwk();
				break;
			}
			break;
		case cTagExpected:
			if ( _Flow.Get_() != '<' )
				RETURN( sUnexpectedCharacter )
		case cTagConfirmed:
			HANDLE( SkipSpaces_( _Flow ) );

			if ( _Flow.View_() == '/' ) {
				_Context = cClosingTag;
				_Flow.Get_();
			} else
				_Context = cOpeningTag;
			break;
		case cOpeningTag:
			switch ( _Token ) {
			case tComment:
			case tCData:
			case t_Undefined:
				if ( _Flow.View_() == '!' ) {
					bso::bool__ IsCData = false;

					_Value.Init();

					HANDLE( GetCommentOrCData_( _Flow, _Value, IsCData ) );
					_Context = cTagExpected;

					if ( IsCData )
						_Token = tCData;
					else
						_Token = tComment;

					if ( ( 1 << _Token ) & TokenToReport )
						Continue = false;

					HANDLE( SkipSpaces_( _Flow ) );
				} else {
					_TagName.Init();

					GetName_( _Flow, _TagName );

					if ( _TagName.Amount() == 0 )
						RETURN( sEmptyTagName );

					_Tags.Push( _TagName );

					_Token = tStartTag;

					// Pour faciliter la localisation d'une erreur.
					if ( isspace( _Flow.View_() ) )
						_Flow.Get_();

					if ( ( 1 << _Token ) & TokenToReport )
						Continue = false;
				}
				break;
			case tStartTag:
				_Token = t_Undefined;

				HANDLE( SkipSpaces_( _Flow ) );

				switch( _Flow.View_() ) {
				case '/':
					_Flow.Get_();

					HANDLE( SkipSpaces_( _Flow ) );

					if ( _Flow.Get_() != '>' )
						RETURN( sUnexpectedCharacter );

					if ( _Tags.IsEmpty() )
						ERRFwk();

					_TagName.Init();

					_Tags.Top( _TagName );

					_Token = tStartTagClosed;

					if ( ( 1 << _Token ) & TokenToReport )
						Continue = false;

					_EmptyTag = true;

				break;
				case '>':
					_Flow.Get_();

					_TagName.Init();

					_Tags.Top( _TagName );

					_Token = tStartTagClosed;

					_EmptyTag = false;

					if ( ( 1 << _Token ) & TokenToReport )
						Continue = false;

					break;
				default:
					_Context = cAttribute;
					break;
				}
				break;
			case tStartTagClosed:
				if ( _EmptyTag ) {
					_TagName.Init();

					_Tags.Top( _TagName );

					_Token = tEndTag;
	
					if ( ( 1 << _Token ) & TokenToReport )
						Continue = false;
				} else {
					_Token = t_Undefined;

					_Context = cValueExpected;
				}
				break;
			case tEndTag:
				_Token = t_Undefined;

				_TagName.Init();

				_Tags.Pop( _TagName );

				if ( _Tags.Amount() == 0 ) {
					_Token = t_Processed;
					Continue = false;
				} else
					_Context = cValueExpected;
				break;
			default:
				ERRFwk();
			}
			break;
		case cAttribute:
			switch ( _Token ) {
			case t_Undefined:
				_AttributeName.Init();
				_Value.Init();

				HANDLE( GetAttribute_( _Flow, _EntitiesHandling, _AttributeName, _Value ) );

				if ( _Tags.IsEmpty() )
					ERRFwk();

				_TagName.Init();

				_Tags.Top( _TagName );

				if ( IsSpecialAttribute_( _AttributeName ) )
					_Token = tSpecialAttribute;
				else
					_Token = tAttribute;

				if ( ( 1 << _Token ) & TokenToReport )
					Continue = false;

				break;
			case tAttribute:
			case tSpecialAttribute:
				_Token = t_Undefined;

				HANDLE( SkipSpaces_( _Flow ) );

				if ( _Flow.View_() == '/' ) {

					_Flow.Get_();

					HANDLE( SkipSpaces_( _Flow ) );

					if ( _Flow.View_() == '>' ) {

						_Flow.Get_();

						if ( _Tags.IsEmpty() )
							ERRFwk();

						_TagName.Init();

						_Tags.Top( _TagName );

						_Token = tStartTagClosed;

						if ( ( 1 << _Token ) & TokenToReport )
							Continue = false;

						_EmptyTag = true;


					} else {
						_Flow.Get_();	// Pour la mise à jour des coordonnées.
						RETURN( sUnexpectedCharacter );
					}
				} else if ( _Flow.View_() == '>' ) {
					_Flow.Get_();

					_TagName.Init();

					_Tags.Top( _TagName );

					_Token = tStartTagClosed;

					if ( ( 1 << _Token ) & TokenToReport )
						Continue = false;

					_EmptyTag = false;
				}/* else {
					_Flow.Get();	// Pour la mise à jour des coordonnées.
					RETURN( sUnexpectedCharacter );
				}
*/				break;
			case tStartTagClosed:
				if ( _EmptyTag ) {
					_TagName.Init();

					_Tags.Top( _TagName );

/*					if ( !EOT )
						HANDLE( SkipSpaces_( _Flow ) );
*/
					_Token = tEndTag;

					if ( ( 1 << _Token ) & TokenToReport )
						Continue = false;
				} else {
					_Token = t_Undefined;

					_Context = cValueExpected;
				}
				break;
			case tEndTag:
				_Token = t_Undefined;

				_TagName.Init();

				_Tags.Pop( _TagName );

				if ( _Tags.Amount() == 0 ) {
					_Token = t_Processed;
					Continue = false;
				} else
					_Context = cValueExpected;
				break;
			default:
				ERRFwk();
				break;
			}
			break;
		case cClosingTag:
			switch ( _Token ) {
			case t_Undefined:
			case tValue:
			case tComment:
			case tCData:
				HANDLE( SkipSpaces_( _Flow ) );

				_TagName.Init();

				GetName_( _Flow, _TagName );

				HANDLE( SkipSpaces_( _Flow ) );

				if ( _Flow.Get_() != '>' )
					RETURN( sUnexpectedCharacter );

				if ( _Tags.IsEmpty() )
					RETURN( sMismatchedTag );

				{
					str::string_ &Buffer = _Value;

					Buffer.Init();

					_Tags.Top( Buffer );

					if ( Buffer != _TagName )
						RETURN( sMismatchedTag );

					Buffer.Init();
				}

				_Token = tEndTag;

				if ( ( _Token == tCData )
					 && !( ( 1 << tCData ) & TokenToReport ) )
					_Token = tValue;	// si on ne demande pas le 'CDATA' en tant que tel, on le reporte en tant que 'tValue'.

				if ( ( 1 << _Token ) & TokenToReport )
					Continue = false;

				break;
			case tEndTag:
				_Token = t_Undefined;

				_TagName.Init();

				_Tags.Pop( _TagName );

				_Context = cValueExpected;

				if ( _Tags.IsEmpty() )
					Continue = false;
				break;
			default:
				ERRFwk();
				break;
			}
			break;
		case cValueExpected:
			switch( _Token ) {
			case t_Undefined:
				if ( _Flow.View_() != '<' ) {
					_Value.Init();
					
					HANDLE( GetTagValue_( _Flow, _Value, _EntitiesHandling, OnlySpaces ) );

					if ( !OnlySpaces ) {

						_TagName.Init();

						if ( _Tags.IsEmpty() )
							ERRFwk();
						else
							_Tags.Top( _TagName );

						_Token = tValue;

						if ( ( 1 << _Token ) & TokenToReport )
							Continue = false;
					}
				} else
					_Context = cTagExpected;
				break;
			case tValue:
				_Token  = t_Undefined;

				_Context = cTagExpected;
			break;
			default:
				ERRFwk();
				break;
			}
			break;
		default:
			ERRFwk();
			break;
		}
	}

	if ( _Tags.IsEmpty() )
		if ( _Token == t_Undefined )
			_Token = t_Processed;

	_Status = sOK;
ERRErr
		if ( ERRType == err::t_Free ) {
			xtf::error__ Error = xtf::e_NoError;

			if ( _Flow.Flow().EndOfFlow( Error ) && ( Error != xtf::e_NoError ) ) {
				_Status = (status__)( s_FirstXTFError + Error );
				_Token = t_Error;
				ERRRst();
			}
		}
ERREnd
ERREpilog
	return _Token;
}

status__ xml::Parse(
	xtf::extended_text_iflow__ &UserFlow,
	entities_handling__ EntitiesHandling,
	callback__ &Callback )
{
	status__ Status = s_Undefined;
ERRProlog
	parser___ Parser;
	str::string TagName, AttributeName, Value;	
	bso::bool__ Stop = false;
	xml::dump Dump;
ERRBegin
	Parser.Init( UserFlow, EntitiesHandling );

	while ( !Stop ) {
		TagName.Init();
		AttributeName.Init();
		Value.Init();
		Dump.PurgeData();

		switch ( Parser.Parse( TagName, AttributeName, Value, Dump, Status ) ) {
		case tProcessingInstruction:
			Stop = !Callback.XMLProcessingInstruction( Dump );
			break;
		case tStartTag:
			Stop = !Callback.XMLStartTag( TagName, Dump );
			break;
		case tStartTagClosed:
			Stop = !Callback.XMLStartTagClosed( TagName, Dump );
			break;
		case tAttribute:
			Stop = !Callback.XMLAttribute( TagName, AttributeName, Value, Dump );
			break;
		case tSpecialAttribute:
			Stop = !Callback.XMLSpecialAttribute( TagName, AttributeName, Value, Dump );
			break;
		case tValue:
		case tCData:
			Stop = !Callback.XMLValue( TagName, Value, Dump );
			break;
		case tEndTag:
			Stop = !Callback.XMLEndTag( TagName, Dump );
			break;
		case tComment:
			Stop = !Callback.XMLComment( Value, Dump );
			break;
		case t_Error:
		case t_Processed:
			Stop = true;
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

void xml::TransformUsingEntities(
	str::string_ &Target,
	bso::bool__ DelimiterOnly )
{
ERRProlog
	sdr::row__ Position = Target.First();
	bso::char__ C;
	str::string Buffer;
ERRBegin
	while( Position != E_NIL ) {
		switch ( C = Target( Position ) ) {
		case '\'':
			if ( !DelimiterOnly ) {
				Buffer.Init( "&apos;" );
				Target.Remove( Position );
				Target.Insert( Buffer, Position );
			}
			break;
		case '"':
			Buffer.Init( "&quot;" );
			Target.Remove( Position );
			Target.Insert( Buffer, Position );
			break;
		case '<':
			if ( !DelimiterOnly ) {
				Buffer.Init( "&lt;" );
				Target.Remove( Position );
				Target.Insert( Buffer, Position );
			}
			break;
		case '>':
			if ( !DelimiterOnly ) {
				Buffer.Init( "&gt;" );
				Target.Remove( Position );
				Target.Insert( Buffer, Position );
			}
			break;
		case '&':
			if ( !DelimiterOnly ) {
				Buffer.Init( "&amp;" );
				Target.Remove( Position );
				Target.Insert( Buffer, Position );
			}
			break;
		default:
			break;
		}

		Position = Target.Next( Position );	// Could be dangerous, but actually works.
	}
ERRErr
ERREnd
ERREpilog
}

void xml::writer_::_CloseAllTags( void )
{
	while ( Tags.Amount() != 0 )
		PopTag();
}

void xml::writer_::_WriteTabs( bso::size__ Amount ) const
{
	while ( Amount-- )
		*S_.Flow << txf::tab;
}

void xml::writer_::PutValue( const value_ &Value )
{
ERRProlog
	value TransformedValue;
ERRBegin
	TransformedValue.Init();

	switch ( S_.SpecialCharHandling ) {
	case schReplace:
		TransformUsingEntities( Value, false, TransformedValue );
		break;
	case schKeep:
		TransformedValue = Value;
		break;
	default:
		ERRFwk();
		break;
	}
	
	if ( S_.TagNameInProgress ) {
		*S_.Flow << '>';
		S_.TagNameInProgress = false;
	}

	*S_.Flow << TransformedValue;

	S_.TagValueInProgress = true;
ERRErr
ERREnd
ERREpilog
}

void xml::writer_::PutAttribute(
	const name_ &Name,
	const value_ &Value )
{
ERRProlog
	value TransformedValue;
ERRBegin
	TransformedValue.Init();

	TransformUsingEntities( Value, S_.SpecialCharHandling == schKeep, TransformedValue );

	if ( !S_.TagNameInProgress )
		ERRFwk();

	*S_.Flow << ' ' << Name << "=\"" << TransformedValue << '"';
ERRErr
ERREnd
ERREpilog
}

void xml::writer_::PutCData( const value_ &Value )
{
	if ( S_.TagNameInProgress ) {
		*S_.Flow << '>';
		S_.TagNameInProgress = false;
	}
	
	*S_.Flow << "<![CDATA[" << Value << "]]>";
}


mark__ xml::writer_::PopTag( mark__ Mark )
{
ERRProlog
	name Name;
ERRBegin
	if ( Tags.IsEmpty() )
		ERRFwk();

	Name.Init();

	if ( Mark != Tags.Pop( Name ) )
		if ( Mark != E_NIL )
			ERRFwk();

	if ( S_.TagNameInProgress )
		*S_.Flow << "/>";
	else {
		if ( !S_.TagValueInProgress && ( S_.Outfit == oIndent ) )
			_WriteTabs( Tags.Amount() );
		*S_.Flow << "</" << Name << ">";
	}

	if ( S_.Outfit == oIndent )
		*S_.Flow << txf::nl;

	S_.TagNameInProgress = false;
	S_.TagValueInProgress = false;
ERRErr
ERREnd
ERREpilog
	return Tags.Last();
}

void xml::writer_::Rewind( mark__ Mark )
{
	while ( PopTag() != Mark );
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xmlpersonnalization
{
public:
	xmlpersonnalization( void )
	{
		if ( ( s_FirstNonXTFError - s_FirstXTFError ) != xtf::e_amount )
			ERRChk();

		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xmlpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static xmlpersonnalization Tutor;
