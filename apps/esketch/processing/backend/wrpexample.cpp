/*
	'wrpexample.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "wrpexample.h"
#include "registry.h"
#include "dir.h"
#include "fnm.h"

#include "sktinf.h"

#include "common.h"

using namespace wrpexample;
using namespace sktxmp;
using common::stuff___;

const char *wrpexample::myobject_::PREFIX = WRPEXAMPLE_MYOBJECT_PREFIX;
const char *wrpexample::myobject_::NAME = WRPEXAMPLE_MYOBJECT_NAME;

enum message__ {
	mTestMessage,

	m_amount,
	m_Undefined,
	m_OK
};

#define CASE( i )	\
	case m##i:\
		Message = WRPEXAMPLE_MYOBJECT_NAME "_" #i;\
		break

static const char *GetRawMessage_( message__ MessageId )
{
	const char *Message = NULL;

	switch ( MessageId ) {
	CASE( TestMessage );
	break;
	default:
		ERRPrm();
		break;
	}

	return Message;
}

#if 0
static const char *GetMessage_(
	message__ MessageId,
	fblbkd::backend &Backend,
	STR_BUFFER___ &Buffer )
{
	return Backend.GetTranslation( GetRawMessage_( MessageId ), Buffer );
}
#endif

#define ARGS (\
	_myobject_ &MyObject,\
	fblbkd::backend___ &Backend,\
	fblbkd::request__ &Request,\
	stuff___ &Stuff )\

typedef void (* f_manager ) ARGS;

static void Report_(
	message__ Message,
	const fblbkd::backend___ &Backend,
	fblbrq::request__ &Request )
{
ERRProlog
	str::string Translation;
	TOL_CBUFFER___ Buffer;
ERRBegin
	Translation.Init();

	Backend.Locale().GetTranslation( GetRawMessage_( Message ), Backend.Language(), Translation );
	Request.ReportRequestError( Translation.Convert( Buffer ) );
ERRErr
ERREnd
ERREpilog
}

void wrpexample::myobject_::HANDLE(
	fblbkd::backend___ &Backend,
	fblbkd::untyped_module &Module,
	fblbkd::index__ Index,
	fblbkd::command__ Command,
	fblbkd::request__ &Request,
	void *UP )
{
	((f_manager)Module.UPs( Command ))( *this, Backend, Request, *(stuff___ *)UP );
}

#define REPORT( v )	Report_( ( v ), Backend, Request )

static void Handle_(
	message__ Message,
	const fblbkd::backend___ &Backend,
	fblbrq::request__ &Request )
{
	if ( Message != m_OK )
		REPORT( Message );
}

#define HANDLE( f )	Handle_( ( f ), Backend, Request )


#define DEC( name )	static void exported##name ARGS

DEC( Test )
{
	message__ Message = m_OK;
ERRProlog
ERRBegin
	Message = mTestMessage;
ERRErr
ERREnd
	HANDLE( Message );
ERREpilog
}

DEC( ToUC )
{
	message__ Message = m_OK;
ERRProlog
	str::string String;
ERRBegin
	String.Init(Request.StringIn() );

	str::ToUpper( String );

	Request.StringOut() = String;
ERRErr
ERREnd
	HANDLE( Message );
ERREpilog
}

#define D( name )	#name, (void *)exported##name

void wrpexample::myobject_::NOTIFY(
	fblbkd::untyped_module &Module,
	common::stuff___ &Data )
{
	Module.Add( D( ToUC ),
			fblbkd::cString,
		fblbkd::cEnd,
			fblbkd::cString,
		fblbkd::cEnd );

	Module.Add( D( Test ),
		fblbkd::cEnd,
		fblbkd::cEnd );
}

