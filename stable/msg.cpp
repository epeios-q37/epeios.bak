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

#define MSG__COMPILATION

#include "msg.h"

using namespace msg;

enum state__ {
	sUnknow,
	sEnd,
	sRaw,
	sTranslation
};

static state__ GetFirstNonEmptyLine_(
	xtf::extended_text_iflow__ &Flow,
	str::string_ &Line,
	bso::bool__ Last )
{
	state__ State = sUnknow;

	while ( State == sUnknow ) {

		if ( Flow.EOX( true ) )
			State = sEnd;
		else {
			Line.Init();

			if ( Last && ( Flow.View( true ) != '#' ) && ( Flow .View( true ) != '\t' ) )
				State = sEnd;
			else {
				Flow.GetLine( Line );

				if ( ( Line.Amount() != 0 ) && ( Line( 0 ) != '#' ) ) {
					if ( Line( 0 ) == '\t' )
						State = sTranslation;
					else
						State = sRaw;
				}
			}
		}
	}

	return State;
}

xtf::location__ msg::i18_messages_::LoadMessages_(
	xtf::extended_text_iflow__ &Flow,
	const messages_ &RawMessages )
{
	xtf::location__ Location = 0;
ERRProlog
	str::string Line;
	state__ State = sUnknow;
	int Language = 1;
	epeios::row__ Row;
	ctn::E_CMITEM( message_ ) RawMessage;
ERRBegin
	Row = RawMessages.First();
	RawMessage.Init( RawMessages );

	Line.Init();

	do {
		Line.Init();

		switch ( State = GetFirstNonEmptyLine_( Flow, Line, Row == NONE ) ) {
		case sEnd:
			break;
		case sRaw:
			if ( ( Row == NONE ) || ( RawMessage( Row ) != Line ) ) {
				State = sEnd;
			} else {
				English.New();
				French.New();
				German.New();
				Spanish.New();
				Language = 1;
				Row = RawMessages.Next( Row );
			}
			break;
		case sTranslation:
#ifdef MSG_DBG
			if ( Line.Amount() < 1 )
				ERRc();
#endif
			Line.Remove( Line.First() );
			if ( Line.Amount() > MSG_MESSAGE_LENGTH_MAX )
				ERRl();
			switch ( Language ) {
			case lgg::lEnglish:
				English( English.Last() ) = Line;
				English.Flush();
				break;
			case lgg::lFrench:
				French( French.Last() ) = Line;
				French.Flush();
				break;
			case lgg::lGerman:
				German( German.Last() ) = Line;
				German.Flush();
				break;
			case lgg::lSpanish:
				Spanish( Spanish.Last() ) = Line;
				Spanish.Flush();
				break;
			default:
				State = sEnd;
				Location = Flow.Coord().Line;
				break;
			}
			Language++;
		}
	} while ( State != sEnd );

	if ( Row != NONE )
		Location = Flow.Coord().Line;
ERRErr
ERREnd
ERREpilog
	return Location;
}

xtf::location__ msg::i18_messages_::LoadMessages( xtf::extended_text_iflow__ &Flow )
{
	xtf::location__ Location = 0;
ERRProlog
	messages RawMessages;
ERRBegin
	RawMessages.Init();

	for ( int i = 0; i < S_.Amount ; i++ )
		RawMessages.Append( str::string( _GetRawMessage( i ) ) );

	Location = LoadMessages_( Flow, RawMessages );
ERRErr
ERREnd
ERREpilog
	return Location;
}

const messages_ &msg::i18_messages_::_GetMessages( lgg::language__ Language ) const
{
	switch ( Language ) {
	case lgg::lEnglish:
		return English;
		break;
	case lgg::lFrench:
		return French;
		break;
	case lgg::lGerman:
		return German;
		break;
	case lgg::lSpanish:
		return Spanish;
		break;
	default:
		ERRc();
		return *(const messages_ *)NULL;	// Only to avoid a warning.
		break;
	}
}

const char *msg::i18_messages_::_GetMessage(
	int MessageId,
	lgg::language__ Language,
	buffer__ &Buffer ) const
{
	ctn::E_CMITEM( message_ ) Message;

	Buffer.Data[0] = 0;

	if ( _GetMessages( Language ).Amount() > (epeios::size__)MessageId ) {
		Message.Init( _GetMessages( Language ) );

#ifdef MSG_DBG
		if ( Message( MessageId ).Amount() >= MSG_MESSAGE_LENGTH_MAX )
			ERRl();
#endif

		Message().Recall( 0, Message( MessageId ).Amount(), Buffer.Data );

		Buffer.Data[Message().Amount()]=0;
	}

	return Buffer.Data;
}

const char *msg::i18_messages_::GetMessage(
	int MessageId,
	lgg::language__ Language,
	buffer__ &Buffer ) const
{
	const char *Message = NULL;

	if ( Language == lgg::lRaw )
		Message = _GetRawMessage( MessageId );
	else {
		Message = _GetMessage( MessageId, Language, Buffer );

		if ( Message[0] == 0 )
			Message = _GetRawMessage( MessageId );
	}

	return Message;
}

void msg::i18_messages_::DumpRawMessages( txf::text_oflow__ &Flow ) const
{
	for ( int i = 0; i < S_.Amount ; i++ )
		Flow << _GetRawMessage( i ) << txf::nl;
}

void msg::i18_messages_::DumpRawMessages( messages_ &Messages ) const
{
	for ( int i = 0; i < S_.Amount ; i++ )
		Messages.Append( message( _GetRawMessage( i ) ) );
}

