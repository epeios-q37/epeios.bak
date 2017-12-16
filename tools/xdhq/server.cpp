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
		const str::dString &Language,
		flw::sWFlow &Flow )
	{
	qRH;
		str::wString XSL;
		qCBUFFERr Buffer;
	qRB;
		XSL.Init();
		sclmisc::LoadXMLAndTranslateTags( XSLFilename, Language.Convert( Buffer ), XSL, '#' );

		SetWithXSLContent_( Answer, Id, XML, XSL, Flow );
	qRR;
	qRT;
	qRE;
	}
}

void server::layout::set::S(
	const str::dString &Id,
	const str::dString &XML,
	const str::dString &XSLFilename,
	const str::dString &Language,
	flw::sWFlow &Flow )
{
	Set_( prtcl::aSetLayout_1, Id, XML, XSLFilename, Language, Flow );
}

void server::GetContents(
	const str::dStrings &Ids,
	flw::sRWFlow &Flow,
	str::dStrings &Contents )
{
	prtcl::PutAnswer( prtcl::aGetContents_1, Flow );
	prtcl::Put( Ids, Flow );
	Flow.Commit();

	prtcl::Get( Flow, Contents );
	Flow.Dismiss();
}

void server::SetContents_(
	const str::dStrings &Ids,
	const str::dStrings &Contents,
	flw::sWFlow &Flow )
{
	prtcl::PutAnswer( prtcl::aSetContents__1, Flow );
	prtcl::Put( Ids, Flow );
	prtcl::Put( Contents, Flow );
	Flow.Commit();
}

void server::SetWidgets(
	const str::dString &Id,
	flw::sWFlow &Flow )
{
	prtcl::PutAnswer( prtcl::aSetWidgets_1, Flow );
	prtcl::Put( Id, Flow );
	Flow.Commit();
}

void server::SetCasts(
	const str::dString &Id,
	const str::dStrings &Tags,
	const str::dStrings &Values,
	flw::sWFlow &Flow )
{
	prtcl::PutAnswer( prtcl::aSetCasts_1, Flow );
	prtcl::Put( Id, Flow );
	prtcl::Put( Tags, Flow );
	prtcl::Put( Values, Flow );
	Flow.Commit();
}

void server::GetAttribute(
	const str::dString &Id,
	const str::dString &Name,
	flw::sRWFlow &Flow,
	str::dString &Value )
{
	prtcl::PutAnswer( prtcl::aGetAttribute_1, Flow );
	prtcl::Put( Id, Flow );
	prtcl::Put( Name, Flow );
	Flow.Commit();

	prtcl::Get( Flow, Value );
	Flow.Dismiss();
}

void server::SetAttribute(
	const str::dString &Id,
	const str::dString &Name,
	const str::dString &Value,
	flw::sWFlow &Flow )
{
	prtcl::PutAnswer( prtcl::aSetAttribute_1, Flow );
	prtcl::Put( Id, Flow );
	prtcl::Put( Name, Flow );
	prtcl::Put( Value, Flow );
	Flow.Commit();
}

void server::GetProperty(
	const str::dString &Id,
	const str::dString &Name,
	flw::sRWFlow &Flow,
	str::dString &Value )
{
	prtcl::PutAnswer( prtcl::aGetProperty_1, Flow );
	prtcl::Put( Id, Flow );
	prtcl::Put( Name, Flow );
	Flow.Commit();

	prtcl::Get( Flow, Value );
	Flow.Dismiss();
}

void server::SetProperty(
	const str::dString &Id,
	const str::dString &Name,
	const str::dString &Value,
	flw::sWFlow &Flow )
{
	prtcl::PutAnswer( prtcl::aSetProperty_1, Flow );
	prtcl::Put( Id, Flow );
	prtcl::Put( Name, Flow );
	prtcl::Put( Value, Flow );
	Flow.Commit();
}
