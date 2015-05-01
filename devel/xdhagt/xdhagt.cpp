/*
	'xdhagt.cpp' by Claude SIMON (http://zeusw.org/).

	'xdhagt' is part of the Epeios framework.

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

#define XDHAGT__COMPILATION

#include "xdhagt.h"

using namespace xdhagt;

void xdhagt::agent___::SetString(
	const nstring___ &Id,
	const nstring___ &Name,
	const str::string_ &RawValue )
{
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	xdhcbk::Escape( RawValue, Value, '"' );

	Value.InsertAt( '"' );
	Value.Append( '"' );

	_C().SetProperty( Id, Name, Value );
ERRErr
ERREnd
ERREpilog
}

void xdhagt::agent___::Alert(
	const str::string_ &RawXML,
	const str::string_ &RawXSL,
	const str::string_ &RawTitle )
{
ERRProlog
	str::string XML, XSL, Title;
ERRBegin
	XML.Init();
	xdhcbk::Escape( RawXML, XML, '"' );

	XSL.Init();
	xdhcbk::Escape( RawXSL, XSL, '"' );

	Title.Init();
	xdhcbk::Escape( RawTitle, Title, '"' );

	_C().Alert( RawXML, RawXSL, Title );
ERRErr
ERREnd
ERREpilog
}

bso::bool__ xdhagt::agent___::Confirm(
	const str::string_ &RawXML,
	const str::string_ &RawXSL,
	const str::string_ &RawTitle )
{
	bso::bool__ Confirmed = false;
ERRProlog
	str::string XML, XSL, Title;
	TOL_CBUFFER___ Buffer;
ERRBegin
	XML.Init();
	xdhcbk::Escape( RawXML, XML, '"' );

	XSL.Init();
	xdhcbk::Escape( RawXSL, XSL, '"' );

	Title.Init();
	xdhcbk::Escape( RawTitle, Title, '"' );

	_C().Confirm( RawXML, RawXSL, Title, Buffer );

	Confirmed = !strcmp( "true", Buffer );
ERRErr
ERREnd
ERREpilog
	return Confirmed;
}

static void SetXML_(
	const str::string_ &Message,
	str::string_ &XML )
{
ERRProlog
	flx::E_STRING_TOFLOW___ STOFlow;
	xml::writer Writer;
ERRBegin
	STOFlow.Init( XML );
	Writer.Init( STOFlow, xml::oCompact, xml::e_Default );

	Writer.PutValue( Message, "Content" );
ERRErr
ERREnd
ERREpilog
}

void xdhagt::agent___::RawAlert( const str::string_ &Message )
{
ERRProlog
	str::string XML, XSL;
ERRBegin
	XML.Init();

	SetXML_( Message, XML );
	
	XSL.Init( "<?xml version=\"1.0\" encoding=\"UTF-8\"?><xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\"	xmlns=\"http://www.w3.org/1999/xhtml\">" );
	XSL.Append("<xsl:output method=\"xml\" encoding=\"UTF-8\"/><xsl:template match=\"/\"><span><xsl:value-of select=\".\"></span></xsl:template></xsl:stylesheet>");

	Alert( XML, XSL );
ERRErr
ERREnd
ERREpilog
}

bso::bool__ xdhagt::agent___::RawConfirm( const str::string_ &Message )
{
	bso::bool__ Confirmed = false;
ERRProlog
	str::string XML, XSL;
ERRBegin
	XML.Init();

	SetXML_( Message, XML );
	
	XSL.Init( "<?xml version=\"1.0\" encoding=\"UTF-8\"?><xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\"	xmlns=\"http://www.w3.org/1999/xhtml\">" );
	XSL.Append("<xsl:output method=\"xml\" encoding=\"UTF-8\"/><xsl:template match=\"/\"><span><xsl:value-of select=\".\"></span></xsl:template></xsl:stylesheet>");

	Confirmed = Confirm( XML, XSL );
ERRErr
ERREnd
ERREpilog
	return Confirmed;
}

void xdhagt::agent___::Log( const str::string_ &RawMessage )
{
ERRProlog
	str::string Message;
ERRBegin
	Message.Init();
	xdhcbk::Escape( RawMessage, Message, '"' );

	_C().Log( Message );
ERRErr
ERREnd
ERREpilog
}

