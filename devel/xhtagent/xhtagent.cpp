/*
	'xhtagent.cpp' by Claude SIMON (http://zeusw.org/).

	'xhtagent' is part of the Epeios framework.

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

#define XHTAGENT__COMPILATION

#include "xhtagent.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "xml.h"
#include "flx.h"

using namespace xhtagent;

void xhtagent::agent___::SetString(
	const nstring___ &Id,
	const nstring___ &Name,
	const str::string_ &RawValue )
{
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();

	xhtcllbk::Escape( RawValue, Value );

	Value.InsertAt( '"' );
	Value.Append( '"' );

	_C().SetProperty( Id, Name, Value );
ERRErr
ERREnd
ERREpilog
}

const str::string_ &xhtagent::agent___::GetSelectValue(
	const char *Id,
	str::string_ &Buffer )
{
ERRProlog
	TOL_CBUFFER___ CBuffer;
ERRBegin
	Buffer.Append( GetSelectValue( Id, CBuffer ) );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

void xhtagent::agent___::Alert(
	const str::string_ &RawXML,
	const str::string_ &RawXSL )
{
ERRProlog
	str::string XML, XSL;
	TOL_CBUFFER___ Buffer;
ERRBegin
	XML.Init();
	xhtcllbk::Escape( RawXML, XML );

	XSL.Init();
	xhtcllbk::Escape( RawXSL, XSL );

	_C().OpenDialog( RawXML, RawXSL, Buffer );
ERRErr
ERREnd
ERREpilog
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

void xhtagent::agent___::Alert( const str::string_ &Message )
{
ERRProlog
	str::string XML, XSL;
ERRBegin
	XML.Init();

	SetXML_( Message, XML );
	
	XSL.Init( "<?xml version=\"1.0\" encoding=\"UTF-8\"?><xsl:stylesheet version=\"2.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\"	xmlns=\"http://www.w3.org/1999/xhtml\">" );
	XSL.Append("<xsl:output method=\"xml\" encoding=\"UTF-8\"/><xsl:template match=\"/\"><span><xsl:value-of select=\".\"></span></xsl:template></xsl:stylesheet>");

	Alert( XML, XSL );
ERRErr
ERREnd
ERREpilog
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xhtagentpersonnalization
{
public:
	xhtagentpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xhtagentpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static xhtagentpersonnalization Tutor;
