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

#include "proxy.h"

#include "sclargmnt.h"

#include "csdmns.h"
#include "csdcmn.h"
#include "mtk.h"

using namespace xdhp;

namespace {

	namespace {
		typedef proxy::rData rPData_;
	}

	class rData_
	: public rPData_
	{
	private:

	};

	namespace {
		typedef proxy::rProcessing<rData_> rPProcessing_;
	}

	proxy::rSharing<rData_> Sharing_;

	class rProcessing_
	: public rPProcessing_
	{
	protected:
		rData_ *PRXYNew( void ) override
		{
			rData_ *Data = new rData_;

			Data->Init();

			Sharing_.Write( Data );

			return Data;
		}
		void PRXYOnAction( rData_ *Data ) override
		{}
		void PRXYOnPending( rData_ *Data ) override
		{}
	} Processing_;

	csdmns::rServer Server_;

	void Process_( void * )
	{
		Server_.Process();
	}
}

SCLZND_F( xdhp::Listen )
{
qRH;
	str::wString Arguments;
qRB;
	Arguments.Init();
	Caller.Get( Arguments );

	sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

	Processing_.Init();
	Server_.Init( sclmisc::MGetU16( registry::parameter::Service ), Processing_ );

	mtk::RawLaunch( Process_, NULL );
qRR;
qRT;
qRE;
}

SCLZND_F( xdhp::New )
{
	Caller.SetReturnValue( (bso::sS64)Sharing_.Read() );
}

SCLZND_F( xdhp::Delete )
{
	bso::sS64 Long = 0;

	Caller.Get( Long );

	if ( Long == 0 )
		qRGnr();

	delete (rData_ * )Long;
}

namespace {
	rData_ &GetData_( sclznd::sCaller &Caller )
	{
		bso::sS64 Long = 0;

		Caller.Get( Long );

		if ( Long == 0 )
			qRGnr();

		return *(rData_ *)Long;
	}
}

#define ARGS_BEGIN\
	rData_ &Data = GetData_( Caller );\
	Data.Sent.WriteBegin();\
	proxy::rArguments &Arguments = Data.Sent.Arguments;\
	Arguments.Init();

#define ARGS_END	Data.Sent.WriteEnd()

SCLZND_F( xdhp::GetAction )
{
qRH;
	str::wStrings Strings;
qRB;
	ARGS_BEGIN;
	Data.Recv.ReadBegin();

	Strings.Append( Data.Recv.Id );
	Strings.Append( Data.Recv.Id );

	Caller.SetReturnValue( Strings );

	Data.Recv.ReadEnd();
	ARGS_END;
qRR;
qRT;
qRE;
}

SCLZND_F( xdhp::Alert )
{
qRH;
	str::wString Message;
qRB;
	ARGS_BEGIN;

	Message.Init();
	Caller.Get( Message );

	server::Alert( Message, Flow );

	ARGS_END;
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

	SetLayout_( Caller, Rack.Language(), Rack.Flow );
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

qCDTOR( xdhp )
{
	tol::Init( Sharing_, Processing_  );
}



