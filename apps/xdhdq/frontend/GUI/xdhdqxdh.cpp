/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHDq' software.

    'XDHDq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHDq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHDq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "xdhdqxdh.h"

#include "sclxdhtml.h"
#include "sclmisc.h"

#include "frdrgstry.h"

#include "core.h"
#include "login.h"
#include "main.h"
#include "prolog.h"

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

	Session->Init( core::Kernel(), Language, ProxyCallback, core::Core );

	switch ( Mode ) {
	case xdhcmn::mMonoUser:
		switch ( sclfrntnd::HandleProject() ) {
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
			Session->User.Init( *Session );
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

const char *sclmisc::SCLMISCTargetName = NAME;
const char *sclmisc::SCLMISCProductName = XDDINF_MC_AFFIX;
