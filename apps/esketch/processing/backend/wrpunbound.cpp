/*
	'wrpunbound.cpp' by Claude SIMON (http://zeusw.org/).

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

// $Id$

#include "wrpunbound.h"
#include "registry.h"
#include "dir.h"
#include "fnm.h"
#include "sktinf.h"

using namespace wrpunbound;

enum message__ {
	mTestMessage,

	m_amount,
	m_Undefined,
	m_OK
};

#define CASE( i )	\
	case m##i:\
		Message = SKTINF_UC_AFFIX #i;\
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

#define DEC( name )\
	static inline void name(\
		fblbkd::backend___ &Backend,\
		fblbkd::untyped_module &,\
		fblbkd::index__,\
		fblbkd::command__,\
		fblbkd::request__ &Request,\
		bso::bool__ &,\
		void * )

DEC( Test )
{
}

#define D( name )	SKTINF_UC_SHORT #name, ::name

void wrpunbound::Inform( fblbkd::backend___ &Backend )
{
	Backend.Add( D( Test ),
		fblbkd::cEnd,
		fblbkd::cEnd );
}

