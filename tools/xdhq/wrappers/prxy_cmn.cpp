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

#include "prxy_cmn.h"

#include "sclmisc.h"

using namespace prxy_cmn;

void prxy_cmn::execute::S(
	const str::dString &Script,
	flw::sWFlow &Flow )
{
	prtcl::SendCommand( prtcl::cExecute_1, Flow );
	prtcl::Put( Script, Flow );
	Flow.Commit();
}

void prxy_cmn::execute::R(
	flw::sRFlow &Flow,
	str::dString &Return )
{
	prtcl::Get( Flow, Return );
	Flow.Dismiss();
}

void prxy_cmn::alert::S(
	const str::dString &Message,
	flw::sWFlow &Flow )
{
	prtcl::SendCommand( prtcl::cAlert_1, Flow );
	prtcl::Put( Message, Flow );
	Flow.Commit();
}

void prxy_cmn::confirm::S(
	const str::dString &Message,
	flw::sWFlow &Flow )
{
	prtcl::SendCommand( prtcl::cConfirm_1, Flow );
	prtcl::Put( Message, Flow );
	Flow.Commit();
}

void prxy_cmn::confirm::R(
	flw::sRFlow &Flow,
	str::dString &Response )
{
	prtcl::Get( Flow, Response );
	Flow.Dismiss();
}

namespace {
	void SetWithXSLContentOrFilename_(
		prtcl::eCommand Command,
		const str::dString &Id,
		const str::dString &XML,
		const str::dString &XSL,
		flw::sWFlow &Flow )
	{
		prtcl::SendCommand( Command, Flow );
		prtcl::Put( Id, Flow );
		prtcl::Put( XML, Flow );
		prtcl::Put( XSL, Flow );
		Flow.Commit();
	}

	// The XSL content is send.
	void SetWithXSLContent_(
		prtcl::eCommand Command,
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

		SetWithXSLContentOrFilename_( Command, Id, XML, XSL, Flow );
	qRR;
	qRT;
	qRE;
	}

	// The XSL filename is send.
	void SetWithXSLFilename_(
		prtcl::eCommand Command,
		const str::dString &Id,
		const str::dString &XML,
		const str::dString &XSLFilename,
		const str::dString &Language,
		flw::sWFlow &Flow )
	{
		SetWithXSLContentOrFilename_( Command, Id, XML, XSLFilename, Flow );
	}
}

void prxy_cmn::layout::set::S(
	const str::dString &Id,
	const str::dString &XML,
	const str::dString &XSLFilename,
	const str::dString &Language,
	flw::sWFlow &Flow )
{
	//	SetWithXSLContent_( prtcl::aSetLayout_1, Id, XML, XSLFilename, Language, Flow );
	SetWithXSLFilename_( prtcl::cSetLayout_1, Id, XML, XSLFilename, Language, Flow );
}

void prxy_cmn::contents::get::S(
	const str::dStrings &Ids,
	flw::sWFlow &Flow )
{
	prtcl::SendCommand( prtcl::cGetContents_1, Flow );
	prtcl::Put( Ids, Flow );
	Flow.Commit();
}

void prxy_cmn::contents::get::R(
	flw::sRFlow &Flow,
	str::dStrings &Contents )
{
	prtcl::Get( Flow, Contents );
	Flow.Dismiss();
}

void prxy_cmn::contents::set::S(
	const str::dStrings & Ids,
	const str::dStrings & Contents,
	flw::sWFlow & Flow )
{
	prtcl::SendCommand( prtcl::cSetContents_1, Flow );
	prtcl::Put( Ids, Flow );
	prtcl::Put( Contents, Flow );
	Flow.Commit();
}

void prxy_cmn::widgets::dress::S(
	const str::dString &Id,
	flw::sWFlow &Flow )
{
	prtcl::SendCommand( prtcl::cDressWidgets_1, Flow );
	prtcl::Put( Id, Flow );
	Flow.Commit();
}

namespace {
	void HandleClasses_(
		const str::dStrings &Ids,
		const str::dStrings &Classes,
		prtcl::eCommand Command,
		flw::sWFlow & Flow )
	{
		prtcl::SendCommand( Command, Flow );
		prtcl::Put( Ids, Flow );
		prtcl::Put( Classes, Flow );
		Flow.Commit();
	}
}

void prxy_cmn::classes::add::S(
	const str::dStrings &Ids,
	const str::dStrings &Classes,
	flw::sWFlow & Flow )
{
	HandleClasses_( Ids, Classes, prtcl::cAddClasses_1, Flow );
}

void prxy_cmn::classes::remove::S(
	const str::dStrings &Ids,
	const str::dStrings &Classes,
	flw::sWFlow & Flow )
{
	HandleClasses_( Ids, Classes, prtcl::cRemoveClasses_1, Flow );
}

void prxy_cmn::classes::toggle::S(
	const str::dStrings &Ids,
	const str::dStrings &Classes,
	flw::sWFlow & Flow )
{
	HandleClasses_( Ids, Classes, prtcl::cToggleClasses_1, Flow );
}

namespace {
	void HandleElements_(
		const str::dStrings &Ids,
		prtcl::eCommand Command,
		flw::sWFlow & Flow )
	{
		prtcl::SendCommand( Command, Flow );
		prtcl::Put( Ids, Flow );
		Flow.Commit();
	}
}

void prxy_cmn::elements::enable::S(
	const str::dStrings &Ids,
	flw::sWFlow & Flow )
{
	HandleElements_( Ids, prtcl::cEnableElements_1, Flow );
}

void prxy_cmn::elements::disable::S(
	const str::dStrings &Ids,
	flw::sWFlow & Flow )
{
	HandleElements_( Ids, prtcl::cDisableElements_1, Flow );
}


void prxy_cmn::ap_::set::S(
	prtcl::eCommand Command,
	const str::dString &Id,
	const str::dString &Name,
	const str::dString &Value,
	flw::sWFlow &Flow )
{
	prtcl::SendCommand( Command, Flow );
	prtcl::Put( Id, Flow );
	prtcl::Put( Name, Flow );
	prtcl::Put( Value, Flow );
	Flow.Commit();
}

void prxy_cmn::ap_::get::S(
	prtcl::eCommand Command,
	const str::dString &Id,
	const str::dString &Name,
	flw::sWFlow &Flow )
{
	prtcl::SendCommand( Command, Flow );
	prtcl::Put( Id, Flow );
	prtcl::Put( Name, Flow );
	Flow.Commit();
}

void prxy_cmn::ap_::get::R(
	flw::sRFlow &Flow,
	str::dString &Value )
{
	prtcl::Get( Flow, Value );
	Flow.Dismiss();
}

void prxy_cmn::attribute::remove::S(
	const str::dString &Id,
	const str::dString &Name,
	flw::sWFlow &Flow )
{
	prtcl::SendCommand( prtcl::cRemoveAttribute_1, Flow );
	prtcl::Put( Id, Flow );
	prtcl::Put( Name, Flow );
	Flow.Commit();
}

void prxy_cmn::focus::S(
	const str::dString &Id,
	flw::sWFlow &Flow )
{
	prtcl::SendCommand( prtcl::cFocus_1, Flow );
	prtcl::Put( Id, Flow );
	Flow.Commit();
}

