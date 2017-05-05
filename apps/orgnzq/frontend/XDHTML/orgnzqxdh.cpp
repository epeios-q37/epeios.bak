/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "sclxdhtml.h"
#include "sclmisc.h"

#include "frdrgstry.h"

#include "core.h"

const char *sclmisc::SCLMISCTargetName = BASE_NAME;
const char *sclmisc::SCLMISCProductName = OGZINF_MC_AFFIX;

void sclxdhtml::SCLXDHTMLInitialization( xdhcmn::mode__ Mode )
{
qRH
	sclfrntnd::rFeatures Features;
qRB
	core::Core.Init( Mode );
	frdmisc::LoadPlugins();

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
	xdhcmn::cProxy *ProxyCallback )
{
	core::rSession *Session = new core::rSession;

	if ( Session == NULL )
		qRGnr();

	Session->Init( core::Kernel(), Language, ProxyCallback );

	switch ( core::Core.Mode() ) {
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

