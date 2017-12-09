/*
	Copyright (C) 2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

    'XDHq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "server.h"

#include "prtcl.h"

#include "sclmisc.h"

using namespace server;

void server::Handshake(
	flw::sRFlow &Flow,
	str::dString & Language )
{
	csdcmn::sVersion Version = csdcmn::UndefinedVersion;

	if ( ( Version = csdcmn::GetProtocolVersion( prtcl::ProtocolId, Flow ) ) != prtcl::ProtocolVersion )
		qRGnr();

	prtcl::Get( Flow, Language );
	Flow.Dismiss();
}

void server::GetAction(
	flw::sRWFlow &Flow,
	str::dString &Id,
	str::dString &Action )
{
	prtcl::PutAnswer( prtcl::aOK_1, Flow );
	Flow.Commit();

	if ( prtcl::GetRequest( Flow ) != prtcl::rLaunch_1 )
		qRGnr();

	prtcl::Get( Flow, Id );
	prtcl::Get( Flow, Action );
	Flow.Dismiss();
}

namespace {
	void SetWithXSLContent_(
		prtcl::eAnswer Answer,
		const str::dString &Id,
		const str::dString &XML,
		const str::dString &XSL,
		flw::sWFlow &Flow )
	{
		prtcl::PutAnswer( Answer, Flow );
		prtcl::Put( Id, Flow );
		prtcl::Put( XML, Flow );
		prtcl::Put( XSL, Flow );
		Flow.Commit();
	}

	void Set_(
		prtcl::eAnswer Answer,
		const str::dString &Id,
		const str::dString &XML,
		const str::dString &XSLFilename,
		const char *Language,
		flw::sWFlow &Flow )
	{
	qRH;
		str::wString XSL;
	qRB;
		XSL.Init();
		sclmisc::LoadXMLAndTranslateTags( XSLFilename, Language, XSL, '#' );

		SetWithXSLContent_( Answer, Id, XML, XSL, Flow );
	qRR;
	qRT;
	qRE;
	}
}

void server::SetLayout(
	const str::dString &Id,
	const str::dString &XML,
	const str::dString &XSLFilename,
	const char *Language,
	flw::sWFlow &Flow )
{
	Set_( prtcl::aSetLayout_1, Id, XML, XSLFilename, Language, Flow );
}

void server::SetCasting(
	const str::dString &Id,
	const str::dString &XML,
	const str::dString &XSLFilename,
	const char *Language,
	flw::sWFlow &Flow )
{
	Set_( prtcl::aSetCasting_1, Id, XML, XSLFilename, Language, Flow );
}

void server::GetContent(
	const str::dString &Id,
	flw::sRWFlow &Flow,
	str::dString &Content )
{
	prtcl::PutAnswer( prtcl::aGetContent_1, Flow );
	prtcl::Put( Id, Flow );
	Flow.Commit();

	prtcl::Get( Flow, Content );
	Flow.Dismiss();
}

void server::SetContents(
	const str::dStrings &Ids,
	const str::dStrings &Contents,
	flw::sWFlow &Flow )
{
	prtcl::PutAnswer( prtcl::aSetContent_1, Flow );
	prtcl::Put( Ids, Flow );
	prtcl::Put( Contents, Flow );
	Flow.Commit();
}
