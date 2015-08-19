/*
	'esketchxht.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "sclxdhtml.h"
#include "sclmisc.h"

#include "frdrgstry.h"

#include "core.h"

#include <new.h>

const char *sclmisc::SCLMISCTargetName = BASE_NAME;


void sclxdhtml::SCLXDHTMLInitialization( frdbse::mode__ Mode )
{
	core::Core.Init( Mode );
}

xdhcbk::session_callback__ *sclxdhtml::SCLXDHTMLRetrieveCallback(
	const char *Language,
	xdhcbk::proxy_callback__ *ProxyCallback )
{
#if 0
	core::session___ *Session = (core::session___ *)malloc( sizeof( core::session___ ) );
	new(Session) core::session___();
#else
	core::session___ *Session = new core::session___;
#endif

	if ( Session == NULL )
		qRGnr();

	Session->Init( Language, ProxyCallback );

	switch ( core::Core.Mode() ) {
	case frdbse::mMonoUser:
		::prolog::SetLayout( *Session );
		break;
	case frdbse::mMultiUser:
//		login::SetLayout( *Session );
		break;
	default:
		qRGnr();
		break;
	}

	return Session;
}

void sclxdhtml::SCLXDHTMLReleaseCallback( xdhcbk::session_callback__ *Callback )
{
	if ( Callback == NULL )
		qRGnr();

	delete Callback;
}

