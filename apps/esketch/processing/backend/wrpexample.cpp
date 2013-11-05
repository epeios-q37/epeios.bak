/*
	'c.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "wrpcommon.h"

using namespace wrpexample;
using namespace sktxmp;
using wrpcommon::data___;

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
	data___ &Data )\

typedef void (* f_manager ) ARGS;

void wrpexample::myobject_::HANDLE(
	fblbkd::backend___ &Backend,
	fblbkd::untyped_module &Module,
	fblbkd::index__ Index,
	fblbkd::command__ Command,
	fblbkd::request__ &Request,
	void *UP )
{
	((f_manager)Module.UPs( Command ))( *this, Backend, Request, *(data___ *)UP );
}

#define DEC( name )	static message__ exported##name ARGS

DEC( Test )
{
	message__ Message = m_OK;
ERRProlog
ERRBegin
	Message = mTestMessage;
ERRErr
ERREnd
ERREpilog
	return Message;
}

#define D( name )	#name, (void *)exported##name

void wrpexample::myobject_::NOTIFY( fblbkd::untyped_module &Module )
{
	Module.Add( D( Test ),
		fblbkd::cEnd,
		fblbkd::cEnd );
}

