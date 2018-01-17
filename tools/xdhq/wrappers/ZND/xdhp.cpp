/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XDHq.

    XDHq is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XDHq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XDHq.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "xdhp.h"

#include "registry.h"
#include "treep.h"

#include "server.h"

#include "sclargmnt.h"

#include "csdbns.h"
#include "csdcmn.h"

using namespace xdhp;

namespace {
	csdbns::rListener Listener_;
}

SCLZND_F( xdhp::Listen )
{
qRH;
	str::wString Arguments;
qRB;
	Arguments.Init();
	Caller.Get( Arguments );

	sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

	Listener_.Init( sclmisc::MGetU16( registry::parameter::Service ) );
qRR;
qRT;
qRE;
}

namespace {
	struct rRack_ {
		sck::rRWFlow Flow;
		str::wString Language;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Flow, Language );
		}
		qCDTOR( rRack_ );
		void Init( sck::sSocket Socket )
		{
			Flow.Init( Socket, true );

			Language.Init();

			server::Handshake( Flow, Language );
			// Object is initialized specifically in the 'Set()' method.
		}
	};
}

SCLZND_F( xdhp::New )
{
qRH;
	const char *IP = NULL;
	sck::sSocket Socket = sck::Undefined;
	rRack_ *Rack = NULL;
qRB;
	Socket = Listener_.GetConnection( IP );

	if ( (Rack = new rRack_) == NULL )
		qRAlc();

	Rack->Init( Socket );

	Caller.SetReturnValue( Rack );
qRR;
	if ( Socket != sck::Undefined )
		sck::Close( Socket );

	if ( Rack != NULL )
		delete Rack;
qRT;
qRE;
}

SCLZND_F( xdhp::Delete )
{
	bso::sS64 Long = 0;

	Caller.Get( Long );

	if ( Long == 0 )
		qRGnr();

	delete (rRack_ * )Long;
}

namespace {
	rRack_ &GetRack_( sclznd::sCaller &Caller )
	{
		bso::sS64 Long = 0;

		Caller.Get( Long );

		if ( Long == 0 )
			qRGnr();

		return *(rRack_ *)Long;
	}

	flw::sRWFlow &GetFlow_( sclznd::sCaller &Caller )
	{
		return GetRack_( Caller ).Flow;
	}
}

#define RACK	rRack_ &Rack = GetRack_( Caller )
#define FLOW	flw::sRWFlow &Flow = GetFlow_( Caller )

SCLZND_F( xdhp::GetAction )
{
qRH;
	str::wString Id, Action;
	str::wStrings Strings;
qRB;
	FLOW;

	tol::Init( Id, Action );
	server::GetAction( Flow, Id, Action );

	Strings.Init();

	Strings.Append( Id );
	Strings.Append( Action );

	Caller.SetReturnValue( Strings );
qRR;
qRT;
qRE;
}

SCLZND_F( xdhp::Alert )
{
qRH;
	str::wString Message;
qRB;
	FLOW;

	Message.Init();
	Caller.Get( Message );

	server::Alert( Message, Flow );
qRR;
qRT;
qRE;
}

SCLZND_F( xdhp::Confirm )
{
	bso::sBool Return = false;
qRH;
	str::wString Message, Response;
qRB;
	FLOW;

	Message.Init();
	Caller.Get( Message );

	Response.Init();
	server::Confirm( Message, Flow, Response );

	Caller.SetReturnValue( Response == "true" );
qRR;
qRT;
qRE;
}

namespace {
	void SetLayout_(
		const str::dString &Id,
		sclznd::sCaller &Caller,
		const str::dString &Language,
		flw::sRWFlow &Flow )
	{
	qRH;
		str::wString XML, XSLFilename;
	qRB;
		tol::Init( XML, XSLFilename );

		treep::GetXML( Caller, XML );
		Caller.Get( XSLFilename );

		server::layout::Set( Id, XML, XSLFilename, Language, Flow );
	qRR;
	qRT;
	qRE;
	}

	void SetLayout_(
		sclznd::sCaller &Caller,
		const str::dString &Language,
		flw::sRWFlow &Flow )
	{
	qRH;
		str::wString Id;
	qRB;
		tol::Init( Id );
		Caller.Get( Id );

		SetLayout_( Id, Caller, Language, Flow );
	qRR;
	qRT;
	qRE;
	}
}

SCLZND_F( xdhp::SetLayout )
{
	RACK;

	SetLayout_( Caller, Rack.Language, Rack.Flow );
}

SCLZND_F( xdhp::GetContents )
{
qRH;
	str::wStrings Ids, Contents;
qRB;
	FLOW;

	Ids.Init();
	Caller.Get( Ids );

	Contents.Init();
	server::contents::Get( Ids, Flow, Contents );

	Caller.SetReturnValue( Contents );
qRR;
qRT;
qRE;
}

SCLZND_F( xdhp::SetContents )
{
qRH;
	str::wStrings Ids, Contents;
qRB;
	FLOW;

	tol::Init( Ids, Contents );
	Caller.Get( Ids, Contents );

	server::contents::Set( Ids, Contents, Flow );
qRR;
qRT;
qRE;
}

SCLZND_F( xdhp::DressWidgets )
{
qRH;
	str::wString Id;
qRB;
	FLOW;

	tol::Init( Id );
	Caller.Get( Id );

	server::widgets::Dress( Id, Flow );
qRR;
qRT;
qRE;
}

namespace {
	void HandleClasses_(
		sclznd::sCaller &Caller,
		void( *Function )(
			const str::dStrings &Ids,
			const str::dStrings &Classes,
			flw::sRWFlow &Flow) )
	{
	qRH;
		str::wStrings Ids, Classes;
	qRB;
		FLOW;

		tol::Init( Ids, Classes );
		Caller.Get( Ids, Classes );

		Function( Ids, Classes, Flow );
	qRR;
	qRT;
	qRE;
	}
}

SCLZND_F( xdhp::AddClasses )
{
	HandleClasses_( Caller, server::classes::Add );
}

SCLZND_F( xdhp::RemoveClasses )
{
	HandleClasses_( Caller, server::classes::Remove );
}

SCLZND_F( xdhp::ToggleClasses )
{
	HandleClasses_( Caller, server::classes::Toggle );
}

namespace {
	void HandleElements_(
		sclznd::sCaller &Caller,
		void( *Function )(
			const str::dStrings &Ids,
			flw::sRWFlow &Flow) )
	{
	qRH;
		str::wStrings Ids;
	qRB;
		FLOW;

		tol::Init( Ids );
		Caller.Get( Ids );

		Function( Ids, Flow );
	qRR;
	qRT;
	qRE;
	}
}

SCLZND_F( xdhp::EnableElements )
{
	HandleElements_( Caller, server::elements::Enable );
}

SCLZND_F( xdhp::DisableElements )
{
	HandleElements_( Caller, server::elements::Disable );
}

namespace {
	void Get_(
		sclznd::sCaller &Caller,
		void( *Function )(
			const str::dString &Id,
			const str::dString &Name,
			flw::sRWFlow &Flow,
			str::dString &Value ) )
	{
	qRH;
		str::wString Id, Name, Value;
	qRB;
		FLOW;

		tol::Init( Id, Name );
		Caller.Get( Id, Name );

		tol::Init( Value );
		Function( Id, Name, Flow, Value );

		Caller.SetReturnValue( Value );
	qRR;
	qRT;
	qRE;
	}

	void Set_(
		sclznd::sCaller &Caller,
		void( *Function )(
			const str::dString &Id,
			const str::dString &Name,
			const str::dString &Value,
			flw::sRWFlow &Flow ) )
	{
	qRH;
		str::wString Id, Name, Value;
	qRB;
		FLOW;

		tol::Init( Id, Name, Value );
		Caller.Get( Id, Name, Value );

		Function( Id, Name, Value, Flow );
	qRR;
	qRT;
	qRE;
	}

}

SCLZND_F( xdhp::GetAttribute )
{
	Get_( Caller, server::attribute::Get );
}

SCLZND_F( xdhp::SetAttribute )
{
	Set_( Caller, server::attribute::Set );
}

SCLZND_F( xdhp::GetProperty )
{
	Get_( Caller, server::property::Get );
}

SCLZND_F( xdhp::SetProperty )
{
	Set_( Caller, server::property::Set );
}



