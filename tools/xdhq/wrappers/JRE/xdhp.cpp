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
	str::wString Arguments, NewSessionAction;
qRB;
	tol::Init( Arguments, NewSessionAction );
	Caller.Get( Arguments, NewSessionAction );

	sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

	Processing_.Init( NewSessionAction );
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

	scljre::sJObject Object = scljre::ConvertUO<rData_>( Env, Data );

	Data->Recv.ReadDismiss();

	return Object;
}

namespace {
	rData_ &GetData_( scljre::sCaller &Caller )
	{
		return scljre::GetUO<rData_>( Caller );
	}
}

SCLJRE_F( xdhp::Set )
{
	rData_ &Data = GetData_( Caller );

	Data.Object.Init( Caller.GetObject() );

	Data.Object.CallVoidMethod( "test", "()V" );

	return scljre::Null();
}

namespace {
	namespace {
		void Init_(
			scljre::sEnv *Env,
			scljre::java::lang::rString &String,
			const str::dString &Content )
		{
		qRH
			scljre::rJString CharsetName;
			scljre::rJByteArray Array;
		qRB
			CharsetName.Init( "UTF-8", n4jre::hOriginal );

			Array.Init( Content );

			String.Init( Env, Array, CharsetName );
		qRR
		qRT
		qRE
		}
	}

	void Set_(
		scljre::sEnv *Env,
		const char *Name,
		const str::dString &Value,
		scljre::rObject &Data )
	{
	qRH
		scljre::java::lang::rString String;
	qRB
		Init_( Env, String, Value );
		Data.Set( Name, "Ljava/lang/String;", String() );
	qRR
	qRT
	qRE
	}
}

SCLJRE_F( xdhp::GetAction )
{
qRH;
	scljre::rObject Event;
qRB;
	rData_ &Data = GetData_( Caller );

	if ( !Data.FirstCall ) {
		Data.Sent.WriteDismiss();
	} else
		Data.FirstCall = false;

	Event.Init( Caller.GetObject() );
	
	Data.Recv.ReadBegin();
	Set_( Env, "id", Data.Recv.Id, Event );
	Set_( Env, "action", Data.Recv.Action, Event );
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

SCLJRE_F( xdhp::Execute )
{
	scljre::sJObject Buffer = NULL;

	BEGIN( Execute );

	Caller.Get( Arguments.Script );

	SWITCH;

	Buffer = scljre::String( Env, Return.GetString() );

	END;

	return Buffer;
}

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
	bso::sBool Response = false;

	BEGIN( Confirm );

	Caller.Get( Arguments.Message );

	SWITCH;

	Response = Return.GetString() == "true";

	END;

	return scljre::Boolean( Env, Response );
}

SCLJRE_F( xdhp::SetLayout )
{
	BEGIN( SetLayout );

	Caller.Get( Arguments.Id );
	treep::GetXML( Caller, Arguments.XML );
	Caller.Get( Arguments.XSLFilename );

	SWITCH;
	END;

	return scljre::Null();
}

SCLJRE_F( xdhp::GetContents )
{
	scljre::sJObject Buffer = NULL;

	BEGIN( GetContents );

	Caller.Get( Arguments.Ids );

	SWITCH;

	Buffer = scljre::Strings( Env, Return.GetStrings() );

	END;

	return Buffer;
}

SCLJRE_F( xdhp::SetContents )
{
	BEGIN( SetContents );

	Caller.Get( Arguments.Ids, Arguments.Contents );

	SWITCH;
	END;

	return scljre::Null();
}

SCLJRE_F( xdhp::DressWidgets )
{
	BEGIN( DressWidgets );

	Caller.Get( Arguments.Id );

	SWITCH;
	END;

	return scljre::Null();
}

SCLJRE_F( xdhp::AddClasses )
{
	BEGIN( AddClasses );

	Caller.Get( Arguments.Ids, Arguments.Classes );

	SWITCH;
	END;

	return scljre::Null();
}

SCLJRE_F( xdhp::RemoveClasses )
{
	BEGIN( RemoveClasses );

	Caller.Get( Arguments.Ids, Arguments.Classes );

	SWITCH;
	END;

	return scljre::Null();
}

SCLJRE_F( xdhp::ToggleClasses )
{
	BEGIN( ToggleClasses );

	Caller.Get( Arguments.Ids, Arguments.Classes );

	SWITCH;
	END;

	return scljre::Null();
}

SCLJRE_F( xdhp::EnableElements )
{
	BEGIN( EnableElements );

	Caller.Get( Arguments.Ids );

	SWITCH;
	END;

	return scljre::Null();
}

SCLJRE_F( xdhp::DisableElements )
{
	BEGIN( DisableElements );

	Caller.Get( Arguments.Ids );

	SWITCH;
	END;

	return scljre::Null();
}

SCLJRE_F( xdhp::SetAttribute )
{
	BEGIN( SetAttribute );

	Caller.Get( Arguments.Id, Arguments.Name, Arguments.Value );

	SWITCH;
	END;

	return scljre::Null();
}

SCLJRE_F( xdhp::GetAttribute )
{
	scljre::sJObject Buffer = NULL;

	BEGIN( GetAttribute );

	Caller.Get( Arguments.Id, Arguments.Name );

	SWITCH;

	Buffer = scljre::String( Env, Return.GetString() );

	END;

	return Buffer;
}

SCLJRE_F( xdhp::RemoveAttribute )
{
	BEGIN( RemoveAttribute );

	Caller.Get( Arguments.Id, Arguments.Name );

	SWITCH;
	END;

	return scljre::Null();
}

SCLJRE_F( xdhp::SetProperty )
{
	BEGIN( SetAttribute );

	Caller.Get( Arguments.Id, Arguments.Name, Arguments.Value );

	SWITCH;
	END;

	return scljre::Null();
}

SCLJRE_F( xdhp::GetProperty )
{
	scljre::sJObject Buffer = NULL;

	BEGIN( GetProperty );

	Caller.Get( Arguments.Id, Arguments.Name );

	SWITCH;

	Buffer = scljre::String( Env, Return.GetString() );

	END;

	return Buffer;
}

SCLJRE_F( xdhp::Focus )
{
	BEGIN( Focus );

	Caller.Get( Arguments.Id );

	SWITCH;
	END;

	return scljre::Null();
}

qGCTOR( xdhp )
{
	tol::Init( Sharing_ );
}


