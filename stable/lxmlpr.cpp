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

#define LXMLPR__COMPILATION

#include "lxmlpr.h"

using namespace lxmlpr;

static void SkipSpaces_( xtf::extended_text_iflow__ &Flow )
{
	while ( !Flow.EOX() && isspace( Flow.View() ) )
		Flow.Get();

	if ( Flow.EOX() )
		ERRI( iBeam );
}

static void GetId_(
	xtf::extended_text_iflow__ &Flow,
	str::string_ &Id )
{
	while ( !Flow.EOX() && isalnum( Flow.View() ) )
		Id.Append( Flow.Get() );
}

static void GetPrefixAndName_( 
	xtf::extended_text_iflow__ &Flow,
	str::string_ &Prefix,
	str::string_ &Name )
{
ERRProlog
	str::string Id;
ERRBegin
	Id.Init();

	GetId_( Flow, Id );

	if ( !Flow.EOX() )
		if ( Flow.View() == ':' ) {
			Prefix = Id;
			Flow.Get();
			GetId_( Flow, Name );
		} else
			Name = Id;
ERRErr
ERREnd
ERREpilog
}

static bso::bool__ GetValue_(
	xtf::extended_text_iflow__ &Flow,
	bso::char__ Delimiter,
	str::string_ &Value )
{
	char C;
	bso::bool__ IsSpace = true;

	while ( !Flow.EOX() && ( Flow.View() != Delimiter ) ) {

		if ( !isspace( C = Flow.Get() ) )
			IsSpace = false;

		Value.Append( C );
	}

	return !IsSpace;
}

inline static bso::bool__ GetTagValue_(
	xtf::extended_text_iflow__ &Flow,
	str::string_ &Value )
{
	return GetValue_( Flow, '<', Value );
}

inline static void GetAttributeValue_(
	xtf::extended_text_iflow__ &Flow,
	str::string_ &Value )
{
	GetValue_( Flow, '"', Value );
}

static void GetAttribute_(
	xtf::extended_text_iflow__ &Flow,
	str::string_ &Prefix,
	str::string_ &Name,
	str::string_ &Value )
{
	GetPrefixAndName_( Flow, Prefix, Name );

	SkipSpaces_( Flow );

	if ( Flow.Get() != '=' )
		ERRI( iBeam );

	SkipSpaces_( Flow );

	if ( Flow.Get() != '"' )
		ERRI( iBeam );

	GetAttributeValue_( Flow, Value );

	Flow.Get();	// To skip the '"'.

	SkipSpaces_( Flow );
}

static void SkipComment_( xtf::extended_text_iflow__ &Flow )
{
	bso::bool__ Continue = true;

	if ( Flow.Get() != '!' )
		ERRI( iBeam );

	if ( Flow.EOX() )
		ERRI( iBeam );

	if ( Flow.Get() != '-' )
		ERRI( iBeam );

	if ( Flow.EOX() )
		ERRI( iBeam );

	if ( Flow.Get() != '-' )
		ERRI( iBeam );

	if ( Flow.EOX() )
		ERRI( iBeam );

	while ( Continue ) {
		while ( !Flow.EOX() && ( Flow.Get() != '-' ) );

		if ( Flow.EOX() )
			ERRI( iBeam );

		if ( Flow.Get() == '-' ) {
			if ( Flow.EOX() )
				ERRI( iBeam );

			if ( Flow.Get() == '>' )
				Continue = false;
		}
	}
}

enum state__ {
	TagExpected,
	OpeningTag,
	ClosingTag,
	Attribute
};

using xtf::location__;

E_ROW( srow__ );

struct tag__ {
	srow__ Prefix;
	srow__ Name;
};

bso::bool__ lxmlpr::Parse(
	xtf::extended_text_iflow__ &Flow,
	callback__ &Callback,
	xtf::location__ &ErrorLine,
	xtf::location__ &ErrorColumn )
{
	bso::bool__ Success = true;
ERRProlog
	state__ State = TagExpected;
	str::string Name, Prefix, Value;
	tag__ Tag;
	lstctn::E_LXMCONTAINERt( str::string_, srow__ ) Strings;
	stk::E_BSTACK( tag__ ) Tags;
ERRBegin
	Strings.Init();
	Tags.Init();

	SkipSpaces_( Flow );

	while ( !Flow.EOX() ) {
		switch ( State ) {
		case TagExpected:
			if ( Flow.Get() != '<' )
				ERRI( iBeam );

			SkipSpaces_( Flow );

			if ( Flow.View() == '/' ) {
				State = ClosingTag;
				Flow.Get();
			} else
				State = OpeningTag;
			break;
		case OpeningTag:
			if ( Flow.View() == '!' ) {
				SkipComment_( Flow );
				State = TagExpected;
				SkipSpaces_( Flow );
				break;
			}

			Prefix.Init();
			Name.Init();

			GetPrefixAndName_( Flow, Prefix, Name );

			Tag.Prefix = Strings.Add( Prefix );
			Tag.Name = Strings.Add( Name );

			Callback.LXMLPRTag( Prefix, Name );

			Tags.Push( Tag );

			SkipSpaces_( Flow );

			switch( Flow.View() ) {
			case '/':
				Flow.Get();
				SkipSpaces_( Flow );

				if ( Flow.Get() != '>' )
					ERRI( iBeam );

				if ( Tags.IsEmpty() )
					ERRI( iBeam );

				Tags.Pop();
				Callback.LXMLPRTagClosed();
				State = TagExpected;
				SkipSpaces_( Flow );
				break;
			case '>':
				Flow.Get();
				if ( Flow.View() != '<' ) {
					Value.Init();
					if ( GetTagValue_( Flow, Value ) )
						Callback.LXMLPRValue( Value );
				}
				State = TagExpected;
				break;
			default:
				State = Attribute;
				break;
			}
			break;
		case Attribute:
			Prefix.Init();
			Name.Init();
			Value.Init();

			GetAttribute_( Flow, Prefix, Name, Value );

			Callback.LXMLPRAttribute( Prefix, Name, Value );

			SkipSpaces_( Flow );

			if ( Flow.View() == '/' ) {

				Flow.Get();

				SkipSpaces_( Flow );

				if ( Flow.View() == '>' ) {

					if ( Tags.IsEmpty() )
						ERRI( iBeam );

					Tags.Pop();

					Callback.LXMLPRTagClosed();
					State = TagExpected;
					Flow.Get();
					SkipSpaces_( Flow );
				} else
					ERRI( iBeam );
			} else if ( Flow.View() == '>' ) {
				Flow.Get();
				if ( Flow.View() != '<' ) {
					Value.Init();
					if ( GetTagValue_( Flow, Value ) )
						Callback.LXMLPRValue( Value );
				}
				State = TagExpected;
			}

			break;
		case ClosingTag:
			SkipSpaces_( Flow );

			Prefix.Init();
			Name.Init();

			GetPrefixAndName_( Flow, Prefix, Name );

			SkipSpaces_( Flow );

			if ( Flow.Get() != '>' )
				ERRI( iBeam );

			Callback.LXMLPRTagClosed();

			if ( Tags.IsEmpty() )
				ERRI( iBeam );

			Tags.Pop();

			if ( Flow.View() != '<' ) {
				Value.Init();
				if ( GetTagValue_( Flow, Value ) )
					Callback.LXMLPRValue( Value );
			}
			State = TagExpected;
			break;
		default:
			ERRc();
			break;
		}
	}

	if ( Tags.Amount() != 0 )
		ERRI( iBeam );
ERRErr
	if ( ( ERRMajor == err::itn ) && ( ERRMinor == err::iBeam ) ) {
		Success = false;
		ErrorLine = Flow.Line();
		ErrorColumn = Flow.Column();
		ERRRst();
	}
ERREnd
ERREpilog
	return Success;
}

void lxmlpr::Transform( str::string_ &Target )
{
ERRProlog
	epeios::row__ Position = Target.First();
	bso::char__ C;
	str::string Buffer;
ERRBegin
	while( Position != NONE ) {
		switch ( C = Target( Position ) ) {
		case '"':
			Buffer.Init( "&#34;" );
			Target.Remove( Position );
			Target.Insert( Buffer, Position );
			break;
		case '<':
			Buffer.Init( "&lt;" );
			Target.Remove( Position );
			Target.Insert( Buffer, Position );
			break;
		case '>':
			Buffer.Init( "&gt;" );
			Target.Remove( Position );
			Target.Insert( Buffer, Position );
			break;
		case '&':
			Buffer.Init( "&amp;" );
			Target.Remove( Position );
			Target.Insert( Buffer, Position );
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

void lxmlpr::writer_::_CloseAllTags( void )
{
	while ( Tags.Amount() != 0 )
		PopTag();
}


void lxmlpr::writer_::PutValue( const value_ &Value )
{
ERRProlog
	value TransformedValue;
ERRBegin
	TransformedValue.Init();

	Convert( Value, TransformedValue );


	if ( S_.TagNameInProgress ) {
		*S_.Flow << '>';
		S_.TagNameInProgress = false;
	}

	*S_.Flow << TransformedValue;
ERRErr
ERREnd
ERREpilog
}


void lxmlpr::writer_::PopTag( void )
{
ERRProlog
	name Name;
ERRBegin
	if ( Tags.IsEmpty() )
		ERRu();

	Name.Init();

	Tags.Pop( Name );

	if ( S_.TagNameInProgress )
		*S_.Flow << "/>";
	else
		*S_.Flow << "</" << Name << ">";

	S_.TagNameInProgress = false;
ERRErr
ERREnd
ERREpilog
}

