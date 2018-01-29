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
	public:
		bso::sBool FirstCall;
		scljre::rObject Object;
		void reset( bso::sBool P = true )
		{
			rPData_::reset( P );
			tol::reset( P, Object );
			FirstCall = false;
		}
		qCDTOR( rData_ );
		void Init( void )
		{
			rPData_::Init();
			FirstCall = true;
			// 'Object' will be initialized later.
		}
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

SCLJRE_F( xdhp::Listen )
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
	return scljre::Null();
}


SCLJRE_F( xdhp::New )
{
	rData_ *Data = Sharing_.Read();

	return scljre::ConvertUO<rData_>( Sharing_.Read() );
}

SCLJRE_F( xdhp::Delete )
{
	return scljre::DeleteUO<rData_>( Caller );
}

namespace {
	rData_ &GetData_( scljre::sCaller &Caller )
	{
		return scljre::GetUO<rData_>( Caller );
	}
}

#define DATA	rData_ &Data = GetData_( Caller )

SCLJRE_F( xdhp::Set )
{
	DATA;

	Data.Object.Init( Caller.Get() );

	Data.Object.CallVoidMethod( "test", "()V" );

	return scljre::Null();
}

namespace {
	namespace {
		void Init_(
			scljre::java::lang::rString &String,
			const str::dString &Content )
		{
		qRH
			scljre::rJString CharsetName;
			scljre::rJByteArray Array;
		qRB
			CharsetName.Init( "UTF-8", n4jre::hOriginal );

			Array.Init( Content );

			String.Init( Array, CharsetName );
		qRR
		qRT
		qRE
		}
	}

	void Set_(
		const char *Name,
		const str::dString &Value,
		scljre::rObject &Data )
	{
	qRH
		scljre::java::lang::rString String;
	qRB
		Init_( String, Value );
		Data.Set( Name, "Ljava/lang/String;", String() );
	qRR
	qRT
	qRE
	}
}

SCLJRE_F( xdhp::GetAction )
{
qRH;
	scljre::rObject Object;
qRB;
	DATA;

	if ( !Data.FirstCall ) {
		Data.Sent.WriteDismiss();
	} else
		Data.FirstCall = false;

	Object.Init( Caller.Get() );
	
	Data.Recv.ReadBegin();
	Set_( "id", Data.Recv.Id, Object );
	Set_( "action", Data.Recv.Action, Object );
	Data.Recv.ReadEnd();
qRR;
qRT;
qRE;
	return scljre::Null();
}

#define BEGIN( request )\
	rData_ &Data = GetData_( Caller );\
	Data.Sent.WriteBegin();\
	Data.Request = prxy_cmn::r##request;\
	proxy::rArguments &Arguments = Data.Sent.Arguments;\
	Arguments.Init();

#define SWITCH	Data.Sent.WriteEnd();Data.Recv.ReadBegin(); proxy::rReturn &Return = Data.Recv.Return

#define END	Data.Recv.ReadEnd()

SCLJRE_F( xdhp::Alert )
{
	BEGIN( Alert );

	Caller.Get( Arguments.Message );

	SWITCH;

	END;

	return scljre::Null();
}

SCLJRE_F( xdhp::Confirm )
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

	Return = Response == "true";
qRR;
qRT;
qRE;
	return scljre::Boolean( Return );
}

namespace {
	void SetLayout_(
		const str::dString &Id,
		scljre::sCaller &Caller,
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
		scljre::sCaller &Caller,
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

SCLJRE_F( xdhp::SetLayout )
{
	RACK;

	SetLayout_( Caller, Rack.Language, Rack.Flow );

	return scljre::Null();
}

SCLJRE_F( xdhp::GetContents )
{
	scljre::sJObject Buffer = NULL;
qRH;
	str::wStrings Ids, Contents;
qRB;
	FLOW;

	Ids.Init();
	Caller.Get( Ids );

	Contents.Init();
	server::contents::Get( Ids, Flow, Contents );

	Buffer = scljre::Strings( Contents );
qRR;
qRT;
qRE;
	return Buffer;
}

SCLJRE_F( xdhp::SetContents )
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
	return scljre::Null();
}

SCLJRE_F( xdhp::DressWidgets )
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
	return scljre::Null();
}

namespace {
	scljre::sJObject HandleClasses_(
		scljre::sCaller &Caller,
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
		return scljre::Null();
	}
}

SCLJRE_F( xdhp::AddClasses )
{
	return HandleClasses_( Caller, server::classes::Add );
}

SCLJRE_F( xdhp::RemoveClasses )
{
	return HandleClasses_( Caller, server::classes::Remove );
}

SCLJRE_F( xdhp::ToggleClasses )
{
	return HandleClasses_( Caller, server::classes::Toggle );
}

namespace {
	scljre::sJObject HandleElements_(
		scljre::sCaller &Caller,
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
		return scljre::Null();
	}
}

SCLJRE_F( xdhp::EnableElements )
{
	return HandleElements_( Caller, server::elements::Enable );
}

SCLJRE_F( xdhp::DisableElements )
{
	return HandleElements_( Caller, server::elements::Disable );
}

namespace {
	scljre::sJObject Get_(
		scljre::sCaller &Caller,
		void( *Function )(
			const str::dString &Id,
			const str::dString &Name,
			flw::sRWFlow &Flow,
			str::dString &Value ) )
	{
		scljre::sJObject Return = NULL;
	qRH;
		str::wString Id, Name, Value;
	qRB;
		FLOW;

		tol::Init( Id, Name );
		Caller.Get( Id, Name );

		tol::Init( Value );
		Function( Id, Name, Flow, Value );

		Return = scljre::String( Value );
	qRR;
	qRT;
	qRE;
		return Return;
	}

	scljre::sJObject Set_(
		scljre::sCaller &Caller,
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
		return scljre::Null();
	}

}

SCLJRE_F( xdhp::GetAttribute )
{
	return Get_( Caller, server::attribute::Get );
}

SCLJRE_F( xdhp::SetAttribute )
{
	return Set_( Caller, server::attribute::Set );
}

SCLJRE_F( xdhp::GetProperty )
{
	return Get_( Caller, server::property::Get );
}

SCLJRE_F( xdhp::SetProperty )
{
	return Set_( Caller, server::property::Set );
}



