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

#define XML__COMPILATION

#include "xml.h"

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
		qRFwk();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}

#undef CASE

void xml::GetMeaning(
	status__ Status,
	const pos__ &Position,
	lcl::meaning_ &Meaning )
{
qRH
	lcl::meaning MeaningBuffer;
	bso::integer_buffer__ IBuffer;
qRB
	Meaning.SetValue( XML_NAME "_ErrorAtLineColumn" );

	MeaningBuffer.Init();
	MeaningBuffer.SetValue( GetLabel( Status ) );

	Meaning.AddTag( MeaningBuffer );

	Meaning.AddTag( bso::Convert( Position.Line, IBuffer ) );
	Meaning.AddTag( bso::Convert( Position.Column, IBuffer ) );
qRR
qRT
qRE
}

static status__ SkipSpaces_( _flow___ &Flow )
{
	while ( !Flow.EndOfFlow() && isspace( Flow.View() ) )
		Flow.Get();

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	return sOK;
}

static status__ HandleProcessingInstruction_( _flow___ &Flow )	// Gre aussi le prologue '<?xml ... ?>'
{
	if ( Flow.Get() != '?' )
		qRFwk();

	while ( !Flow.EndOfFlow() && ( Flow.Get() != '>' ) );

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

	if ( Flow.Get() != C )
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

	if ( Flow.Get() != '-' )
		qRFwk();

	if ( ( Status = Test_( Flow, '-' ) ) != sOK )
		return Status;

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	while ( Continue ) {
		while ( !Flow.EndOfFlow() && ( Flow.View() != '-' ) ) {
			xtf::utf__ UTF;

			UTF.Init();

			Flow.Get( UTF );
			Content.Append( (bso::char__ *)UTF.Data, UTF.Size );
		}
		
		if ( Flow.EndOfFlow() )
			return sUnexpectedEOF;

		Flow.Get();	// Pour passer le '-' de dbut de fin de commentaire.

		if ( Flow.EndOfFlow() )
			return sUnexpectedEOF;

		if ( Flow.View() == '-' ) {
			Flow.Get();

			if ( Flow.EndOfFlow() )
				return sUnexpectedEOF;

			if ( Flow.View() == '>' ) {
				Flow.Get();

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

	if ( Flow.Get() != '[' )
		qRFwk();

	if ( ( Status = Test_( Flow, "CDATA[" ) ) != sOK )
		return Status;

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	while ( Continue ) {
		while ( !Flow.EndOfFlow() && ( Flow.View() != ']' ) )
		{
			xtf::utf__ UTF;

			UTF.Init();

			Flow.Get( UTF );

			Content.Append( (bso::char__ *)UTF.Data, UTF.Size );
		}

		if ( Flow.EndOfFlow() )
			return sUnexpectedEOF;

		Flow.Get();	// Pour passer le ']' de dbut de fin de 'CDATA'.

		if ( Flow.EndOfFlow() )
			return sUnexpectedEOF;

		if ( Flow.View() == ']' ) {
			Flow.Get();

			if ( Flow.EndOfFlow() )
				return sUnexpectedEOF;

			if ( Flow.View() == '>' ) {
				Flow.Get();

				Continue = false;
			} else
				Content.Append( "]]" );
		} else
			Content.Append( ']' );
	}

	return sOK;
}

static status__ GetCommentOrCData_(
	_flow___ &Flow,
	str::string_ &Content,
	bso::bool__ &IsCData )
{
	if ( Flow.Get() != '!' )
		qRFwk();

	if ( Flow.EndOfFlow() )
		return sUnexpectedEOF;

	if ( Flow.View() == '-' ) {
		IsCData = false;
		return GetComment_( Flow, Content );
	} else if ( Flow.View() == '[' ) {
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

	while ( !Flow.EndOfFlow() && ( isalnum( Flow.View() ) || Flow.View() == ':' || Flow.View() == '_' || Flow.View() == '-' ) ) {
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
			switch( Flow.Get() ) {
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
			if ( Flow.Get() != 't' )
				return ENTITY_ERROR_VALUE;

			State = ehsLT;
			break;
		case ehsLT:
			if ( Flow.Get() != ';' )
				return ENTITY_ERROR_VALUE;

			return '<';
			break;
		case ehsG:
			if ( Flow.Get() != 't' )
				return ENTITY_ERROR_VALUE;

			State = ehsGT;
			break;
		case ehsGT:
			if ( Flow.Get() != ';' )
				return ENTITY_ERROR_VALUE;

			return '>';
			break;
		case ehsA:
			switch ( Flow.Get() ) {
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
			if ( Flow.Get() != 'p' )
				return ENTITY_ERROR_VALUE;

			State = ehsAMP;
			break;
		case ehsAMP:
			if ( Flow.Get() != ';' )
				return ENTITY_ERROR_VALUE;

			return '&';
			break;
		case ehsAP:
			if ( Flow.Get() != 'o' )
				return ENTITY_ERROR_VALUE;

			State = ehsAPO;
			break;
		case ehsAPO:
			if ( Flow.Get() != 's' )
				return ENTITY_ERROR_VALUE;

			State = ehsAPOS;
			break;
		case ehsAPOS:
			if ( Flow.Get() != ';' )
				return ENTITY_ERROR_VALUE;

			return '\'';
			break;
		case ehsQ:
			if ( Flow.Get() != 'u' )
				return ENTITY_ERROR_VALUE;

			State = ehsQU;
			break;
		case ehsQU:
			if ( Flow.Get() != 'o' )
				return ENTITY_ERROR_VALUE;

			State = ehsQUO;
		case ehsQUO:
			if ( Flow.Get() != 't' )
				return ENTITY_ERROR_VALUE;

			State = ehsQUOT;
		case ehsQUOT:
			if ( Flow.Get() != ';' )
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

	while ( !Flow.EndOfFlow() && ( Flow.View() != Delimiter ) ) {

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
		Flow.Get();	// Pour ajuster les coordonnes de l'erreur.
		return sUnexpectedCharacter;
	}

	HANDLE( SkipSpaces_( Flow ) );

	if ( Flow.Get() != '=' )
		return sMissingEqualSign;

	HANDLE( SkipSpaces_( Flow ) );

	Delimiter = Flow.Get();

	if ( ( Delimiter != '"' ) && ( Delimiter != '\'' ) )
		return sBadAttributeValueDelimiter;

	HANDLE( GetAttributeValue_( Flow, Delimiter, EntitiesHandling, Value ) );

	Flow.Get();	// To skip the '"' or '''.
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
		qRReturn;\
	}

#define RETURN( V )\
	{\
		_Status = V;\
		_Token = t_Error;\
		qRReturn;\
	}

token__ xml::parser___::Parse( int TokenToReport )
{
qRH
	bso::bool__ OnlySpaces = false, Continue = true, TEOX = true;	// 'TEOX' : Test EOX.
qRB

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
				else if ( _Flow.View() != '<' )
					RETURN( sUnexpectedCharacter )
				else {
					_Flow.Get();	// Pour enlever le '<'.

					if ( _Flow.View() == '?' ) {
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
				qRFwk();
				break;
			}
			break;
		case cTagExpected:
			if ( _Flow.Get() != '<' )
				RETURN( sUnexpectedCharacter )
		case cTagConfirmed:
			HANDLE( SkipSpaces_( _Flow ) );

			if ( _Flow.View() == '/' ) {
				_Context = cClosingTag;
				_Flow.Get();
			} else
				_Context = cOpeningTag;
			break;
		case cOpeningTag:
			switch ( _Token ) {
			case tComment:
			case tCData:
			case t_Undefined:
				if ( _Flow.View() == '!' ) {
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
					if ( isspace( _Flow.View() ) )
						_Flow.Get();

					if ( ( 1 << _Token ) & TokenToReport )
						Continue = false;
				}
				break;
			case tStartTag:
				_Token = t_Undefined;

				HANDLE( SkipSpaces_( _Flow ) );

				switch( _Flow.View() ) {
				case '/':
					_Flow.Get();

					HANDLE( SkipSpaces_( _Flow ) );

					if ( _Flow.Get() != '>' )
						RETURN( sUnexpectedCharacter );

					if ( _Tags.IsEmpty() )
						qRFwk();

					_TagName.Init();

					_Tags.Top( _TagName );

					_Token = tStartTagClosed;

					if ( ( 1 << _Token ) & TokenToReport )
						Continue = false;

					_EmptyTag = true;

				break;
				case '>':
					_Flow.Get();

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
				qRFwk();
			}
			break;
		case cAttribute:
			switch ( _Token ) {
			case t_Undefined:
				_AttributeName.Init();
				_Value.Init();

				HANDLE( GetAttribute_( _Flow, _EntitiesHandling, _AttributeName, _Value ) );

				if ( _Tags.IsEmpty() )
					qRFwk();

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

				if ( _Flow.View() == '/' ) {

					_Flow.Get();

					HANDLE( SkipSpaces_( _Flow ) );

					if ( _Flow.View() == '>' ) {

						_Flow.Get();

						if ( _Tags.IsEmpty() )
							qRFwk();

						_TagName.Init();

						_Tags.Top( _TagName );

						_Token = tStartTagClosed;

						if ( ( 1 << _Token ) & TokenToReport )
							Continue = false;

						_EmptyTag = true;


					} else {
						_Flow.Get();	// Pour la mise  jour des coordonnes.
						RETURN( sUnexpectedCharacter );
					}
				} else if ( _Flow.View() == '>' ) {
					_Flow.Get();

					_TagName.Init();

					_Tags.Top( _TagName );

					_Token = tStartTagClosed;

					if ( ( 1 << _Token ) & TokenToReport )
						Continue = false;

					_EmptyTag = false;
				}/* else {
					_Flow.Get();	// Pour la mise  jour des coordonnes.
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
				qRFwk();
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

				if ( _Flow.Get() != '>' )
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
				qRFwk();
				break;
			}
			break;
		case cValueExpected:
			switch( _Token ) {
			case t_Undefined:
				if ( _Flow.View() != '<' ) {
					_Value.Init();
					
					HANDLE( GetTagValue_( _Flow, _Value, _EntitiesHandling, OnlySpaces ) );

					if ( !OnlySpaces ) {

						_TagName.Init();

						if ( _Tags.IsEmpty() )
							qRFwk();
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
				qRFwk();
				break;
			}
			break;
		default:
			qRFwk();
			break;
		}
	}

	if ( _Tags.IsEmpty() )
		if ( _Token == t_Undefined )
			_Token = t_Processed;

	_Status = sOK;
qRR
		if ( ERRType == err::t_Free ) {
			xtf::error__ Error = xtf::e_NoError;

			if ( _Flow.Flow().EndOfFlow( Error ) && ( Error != xtf::e_NoError ) ) {
				_Status = (status__)( s_FirstXTFError + Error );
				_Token = t_Error;
				ERRRst();
			}
		}
qRT
qRE
	return _Token;
}

void xml::parser___::Skip( void )
{
	if ( _Tags.IsEmpty() )
		qRFwk();

	sdr::size__ Limit = _Tags.Amount();

	do {
		switch( Parse( tfEndTag ) ) {
		case tEndTag:
			break;
		default:
			qRFwk();
			break;
		}
	} while ( _Tags.Amount() > Limit );

	if ( _Tags.Amount() != Limit )
		qRFwk();
}

status__ xml::Parse(
	xtf::extended_text_iflow__ &UserFlow,
	entities_handling__ EntitiesHandling,
	callback__ &Callback,
	qRPN )
{
	status__ Status = s_Undefined;
qRH
	parser___ Parser;
	str::string TagName, AttributeName, Value;	
	bso::bool__ Stop = false;
	xml::dump Dump;
qRB
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
			if ( qRPT )
				qRFwk();
			else
				Stop = true;
			break;
		case t_Processed:
			Stop = true;
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

typedef fdr::iflow_driver___<> rIFlowDriver_;

namespace {
	qCDEF( char *, APOS_, "&apos;" );
	qCDEF( char *, QUOT_, "&quot;");
	qCDEF(char *, LT_, "&lt;" );
	qCDEF(char *, GT_, "&gt;" );
	qCDEF(char *, AMP_, "&amp;" );

	class rIFlowDriver
	: public rIFlowDriver_
	{
	private:
		qRMV( flw::iflow__, F_, Flow_ );
		const char *Pending_;
		bso::sBool EOF_;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			bso::char__ C = 0;
			fdr::size__ Size = 0;

			if ( EOF_ )
				return 0;
			while ( ( Size < Maximum ) & !EOF_ ) {
				if ( Pending_ != NULL ) {
					Buffer[Size++] = *(Pending_++);

					if ( *Pending_ == 0 )
						Pending_ = NULL;
				} else {
					if ( F_().EndOfFlow() )
						EOF_ = true;
					else {
						switch ( C = F_().Get() ) {
						case '\'':
							Pending_ = APOS_;
							break;
						case '"':
							Pending_ = QUOT_;
							break;
						case '<':
							Pending_ = LT_;
							break;
						case '>':
							Pending_ = GT_;
							break;
						case '&':
							Pending_ = AMP_;
							break;
						default:
							Buffer[Size++] = C;
							break;
						}
					}
				}
			}

			return Size;
		}
		virtual void FDRDismiss( bso::sBool Unlock ) override
		{
			F_().Dismiss( Unlock );
		}
		virtual fdr::sTID FDRITake( fdr::sTID Owner ) override
		{
			return F_().IDriver().ITake( Owner );
		}
	public:
		void reset( bso::sBool P = true )
		{
			rIFlowDriver_::reset( P );
			Flow_ = NULL;
			Pending_ = NULL;
			EOF_ = false;
		}
		qCVDTOR( rIFlowDriver );
		void Init( flw::iflow__ &Flow )
		{
			rIFlowDriver_::Init( fdr::ts_Default );
			Flow_ = &Flow;
			Pending_ = NULL;
			EOF_ = false;
		}
	};
};

void xml::TransformUsingEntities(
	const str::dString &Source,
	str::dString &Target )
{
qRH
	flx::sStringIFlow IFlow;
	flx::rStringOFlow OFlow;
qRB
	IFlow.Init( Source );
	OFlow.Init( Target );

	flw::Copy( IFlow, OFlow );
qRR
qRT
qRE
}

void xml::writer_::_CloseAllTags( void )
{
	while ( Tags.Amount() != 0 )
		PopTag();
}

void xml::writer_::_Indent( bso::size__ Amount ) const
{
	while ( Amount-- )
		*S_.Flow << ' ';
}

void xml::writer_::PutRawValue( flw::sRFlow &Flow )
{
	if ( S_.TagNameInProgress ) {
		*S_.Flow << '>';
		S_.TagNameInProgress = false;
	}

	flx::Copy( Flow, RF_() );

	S_.TagValueInProgress = true;

	_Commit();
}

namespace {
	void TransformAndPutValue_(
		flw::sRFlow &Flow,
		dWriter &Writer )
	{
	qRH
		flw::standalone_iflow__<> TFlow;
		rIFlowDriver Driver;
	qRB
		Driver.Init( Flow );
		TFlow.Init( Driver );

		Writer.PutRawValue( TFlow );
	qRR
	qRT
	qRE
	}
}

void xml::writer_::PutValue( flw::sRFlow &Flow )
{
	switch ( S_.SpecialCharHandling ) {
	case schReplace:
		TransformAndPutValue_( Flow, *this );
		break;
	case schKeep:
		PutRawValue( Flow );
		break;
	default:
		qRFwk();
		break;
	}
}

void xml::writer_::PuRawValue( const value_ &Value )
{
	flx::sStringIFlow Flow;

	Flow.Init( Value );
	PutRawValue( Flow );
}


void xml::writer_::PutValue( const value_ &Value )
{
	flx::sStringIFlow Flow;

	Flow.Init( Value );
	PutValue( Flow );
}

void xml::writer_::PutRawAttribute(
	const name_ &Name,
	flw::sRFlow &Flow )
{
	if ( !S_.TagNameInProgress )
		qRFwk();

	F_() << ' ' << Name << "=\"";

	flx::Copy( Flow, RF_() );
		
	F_() << '"';

	_Commit();
}

namespace {
	void TransformAndPutAttribute_(
		const name_ &Name,
		flw::sRFlow &Flow,
		dWriter &Writer )
	{
	qRH
		flw::standalone_iflow__<> TFlow;
		rIFlowDriver Driver;
	qRB
		Driver.Init( Flow );
		TFlow.Init( Driver );

		Writer.PutRawAttribute( Name, TFlow );
	qRR
	qRT
	qRE
	}
}

void xml::writer_::PutAttribute(
	const name_ &Name,
	flw::sRFlow &Flow )
{
	switch ( S_.SpecialCharHandling ) {
	case schReplace:
		TransformAndPutAttribute_( Name, Flow, *this );
		break;
	case schKeep:
		PutRawAttribute( Name, Flow );
		break;
	default:
		qRFwk();
		break;
	}
}

void xml::writer_::PutAttribute(
	const name_ &Name,
	const value_ &Value )
{
	flx::sStringIFlow Flow;

	Flow.Init( Value );
	PutAttribute( Name, *Flow );
}

void xml::writer_::PutRawAttribute(
	const name_ &Name,
	const value_ &Value )
{
	flx::sStringIFlow Flow;

	Flow.Init( Value );
	PutRawAttribute( Name, Flow );
}

void xml::writer_::PutCData( flw::sRFlow &Flow )
{
	if ( S_.TagNameInProgress ) {
		*S_.Flow << '>';
		S_.TagNameInProgress = false;
	}
	
	F_() << "<![CDATA[";

	flw::Copy( Flow, RF_() );
		
	F_() << "]]>";

	_Commit();
}

void xml::writer_::PutCData( const value_ &Value )
{
	flx::sStringIFlow Flow;

	Flow.Init( Value );
	PutCData( Flow );
}

mark__ xml::writer_::PopTag( mark__ Mark )
{
qRH
	name Name;
qRB
	if ( Tags.IsEmpty() )
		qRFwk();

	Name.Init();

	if ( Mark != Tags.Pop( Name ) )
		if ( Mark != qNIL )
			qRFwk();

	if ( S_.TagNameInProgress )
		*S_.Flow << "/>";
	else {
		if ( !S_.TagValueInProgress && ( S_.Outfit == oIndent ) )
			_Indent( Tags.Amount() );
		*S_.Flow << "</" << Name << ">";
	}

	if ( S_.Outfit == oIndent )
		*S_.Flow << txf::nl;

	S_.TagNameInProgress = false;
	S_.TagValueInProgress = false;

	_Commit();
qRR
qRT
qRE
	return Tags.Last();
}

void xml::writer_::Rewind( mark__ Mark )
{
	while ( PopTag() != Mark );
}

bso::sBool dWriter::Put( rParser &Parser )
{
	bso::sBool Continue = true, Success = false;

	while ( Continue ) {
		switch( Parser.Parse( xml::tfAll & ~xml::tfStartTagClosed ) ) {
		case xml::tProcessingInstruction:
			GetFlow() << Parser.DumpData();
			
			if ( GetOutfit() == xml::oIndent )
				GetFlow() << txf::nl;

			break;
		case xml::tStartTag:
			PushTag( Parser.TagName() );
			break;
		case xml::tAttribute:
		case xml::tSpecialAttribute:
			PutAttribute( Parser.AttributeName(), Parser.Value() );
			break;
		case xml::tValue:
			PutValue( Parser.Value() );
			break;
		case xml::tEndTag:
			PopTag();
			break;
		case xml::tCData:
			PutCData( Parser.Value() );
			break;
		case xml::t_Processed:
			Continue = false;
			Success = true;
			break;
		case xml::t_Error:
			Success = false;
			Continue = false;
			break;
		default:
			qRFwk();
			break;
		}
	}

	return Success;
}

bso::sBool xml::dWriter::Put( xtf::extended_text_iflow__ &XFlow )
{
	bso::sBool Success = true;
qRH
	rParser Parser;
qRB
	Parser.Init( XFlow, ehKeep );

	Success = Put( Parser );
qRR
qRT
qRE
	return Success;
}

bso::sBool xml::dWriter::Put( const str::dString &XML )
{
	bso::sBool Success = false;
qRH
	flx::sStringIFlow SFlow;
	xtf::extended_text_iflow__ XFlow;
qRB
	SFlow.Init( XML );
	XFlow.Init( SFlow, utf::f_Default );

	Success = Put( XFlow );
qRR
qRT
qRE
	return Success;
}

Q37_GCTOR( xml )
{
	if ( ( s_FirstNonXTFError - s_FirstXTFError ) != xtf::e_amount )
		qRChk();
}
