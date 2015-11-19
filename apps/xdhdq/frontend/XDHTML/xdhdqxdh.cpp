/*
	'xdhdqxht.cpp' by Claude SIMON (http://q37.info/contact/).

	 This file is part of 'xdhdq' software.

    'xdhdq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhdq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhdq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "sclxdhtml.h"
#include "sclmisc.h"

#include "frdrgstry.h"

#include "core.h"

const char *sclmisc::SCLMISCTargetName = BASE_NAME;


void sclxdhtml::SCLXDHTMLInitialization( xdhcmn::mode__ Mode )
{
	core::Core.Init( Mode );
}

xdhcmn::session_callback__ *sclxdhtml::SCLXDHTMLRetrieveCallback(
	const char *Language,
	xdhcmn::proxy_callback__ *ProxyCallback )
{
#if 0
	core::session___ *Session = (core::session___ *)malloc( sizeof( core::session___ ) );
	new(Session) core::session___();
#else
	core::session___ *Session = new core::session___;
#endif

	if ( Session == NULL )
		qRGnr();

	Session->Init( core::Kernel(), Language, ProxyCallback );

	switch ( core::Core.Mode() ) {
	case xdhcmn::mMonoUser:
		::prolog::SetLayout( *Session );
		break;
	case xdhcmn::mMultiUser:
//		login::SetLayout( *Session );
		break;
	default:
		qRGnr();
		break;
	}

	return Session;
}

void sclxdhtml::SCLXDHTMLReleaseCallback( xdhcmn::session_callback__ *Callback )
{
	if ( Callback == NULL )
		qRGnr();

	delete Callback;
}

