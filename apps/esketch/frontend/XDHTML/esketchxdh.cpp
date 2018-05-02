/*
	Copyright (C) 2018 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "esketchxdh.h"

#include "sclxdhtml.h"
#include "sclmisc.h"

#include "frdrgstry.h"

#include "core.h"
#include "login.h"
#include "main.h"
#include "prolog.h"

SCLI_DEF( esketchxdh, NAME, SKTINF_MC );

const scli::sInfo &sclxdhtml::SCLXDHTMLInfo( void )
{
	return esketchxdh::Info;
}

void sclxdhtml::SCLXDHTMLInitialization( xdhcmn::eMode Mode )
{
qRH
	sclfrntnd::rFeatures Features;
qRB
	core::Core.Init( Mode );

	if ( Mode == xdhcmn::mMultiUser ) {
		Features.Init();
		sclfrntnd::GuessBackendFeatures( Features );
		core::Kernel().Init( Features, plgn::EmptyAbstracts );
	}
qRR
qRT
qRE
}

xdhcmn::cSession *sclxdhtml::SCLXDHTMLRetrieveCallback(
	const char *Language,
	xdhcmn::eMode Mode,
	xdhcmn::cProxy *ProxyCallback )
{
	core::rSession *Session = new core::rSession;

	if ( Session == NULL )
		qRGnr();

	Session->Init( core::Kernel(), Language, ProxyCallback, core::Core, esketchxdh::Info );

	switch ( Mode ) {
	case xdhcmn::mMonoUser:
		switch ( sclfrntnd::HandleProject( esketchxdh::Info ) ) {
		case sclfrntnd::phNone:
			::prolog::Display( *Session );
			break;
		case sclfrntnd::phLoad:
			::login::Display( *Session );
			break;
		case sclfrntnd::phLogin:
			Session->SetBackendVisibility( sclxdhtml::bvHide );
			::login::Display( *Session );
			break;
		case sclfrntnd::phRun:
			::main::Display( *Session );
			break;
		default:
			qRGnr();
			break;
		}
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

void sclxdhtml::SCLXDHTMLReleaseCallback( xdhcmn::cSession *Callback )
{
	if ( Callback == NULL )
		qRGnr();

	delete Callback;
}
