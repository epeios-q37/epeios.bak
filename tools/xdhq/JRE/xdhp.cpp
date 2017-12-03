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

SCLJRE_F( xdhp::Listen )
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
	return scljre::Integer( 0 );
}

namespace {
	struct rRack_ {
		sck::rRWFlow Flow;
		qCBUFFERr Language;
		scljre::rObject Object;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Flow, Language, Object );
		}
		qCDTOR( rRack_ );
		void Init( sck::sSocket Socket )
		{
		qRH;
			str::wString Language;
		qRB;
			Flow.Init( Socket, true );

			Language.Init();

			server::Handshake( Flow, Language );
			Language.Convert( this->Language );

			// Object is intialized specifically in the 'Set()' method.
		qRR;
		qRT;
		qRE;
		}
		void Set( scljre::cObject *Object )
		{
			this->Object.Init( Object );
		}
	};
}

SCLJRE_F( xdhp::New )
{
	scljre::sJObject Object = NULL;
qRH;
	const char *IP = NULL;
	sck::sSocket Socket = sck::Undefined;
qRB;
	Socket = Listener_.GetConnection( IP );

	Object = scljre::CreateUO<rRack_>( Socket );
qRR;
	if ( Socket != sck::Undefined )
		sck::Close( Socket );
qRT;
qRE;
	return Object;	
}

SCLJRE_F( xdhp::Delete )
{
	return scljre::DeleteUO<rRack_>( Caller );
}

namespace {
	rRack_ &GetRack_( scljre::sCaller &Caller )
	{
		return scljre::GetUO<rRack_>( Caller );
	}

	flw::sRWFlow &GetFlow_( scljre::sCaller &Caller )
	{
		return GetRack_( Caller ).Flow;
	}
}

SCLJRE_F( xdhp::Set )
{
	rRack_ &Rack = GetRack_( Caller );

	Rack.Set( Caller.Get() );

	Rack.Object.CallVoidMethod( "test", "()V" );

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

	void GetAction_(
		flw::sRWFlow &Flow,
		scljre::sCaller &Caller )
	{
	qRH;
		str::wString Id, Action;
		scljre::rObject Data;
	qRB;
		tol::Init( Id, Action );
		server::GetAction( Flow, Id, Action );

		Data.Init( Caller.Get() );
		Set_( "id", Id, Data );
		Set_( "action", Action, Data );
	qRR;
	qRT;
	qRE;
	}
}

SCLJRE_F( xdhp::GetAction )
{
	flw::sRWFlow &Flow = GetFlow_( Caller );
	GetAction_( Flow, Caller );

	return scljre::Null();
}

namespace {
	void SetElement_(
		server::fSet Set,
		const str::dString &Id,
		scljre::sCaller &Caller,
		const char *Language,
		flw::sWFlow &Flow )
	{
	qRH;
		str::wString XML, XSLFilename;
	qRB;
		tol::Init( XML, XSLFilename );

		treep::GetXML( Caller, XML );
		Caller.Get( XSLFilename );

		Set( Id, XML, XSLFilename, Language, Flow );
	qRR;
	qRT;
	qRE;
	}

	void SetElement_(
		server::fSet Set,
		scljre::sCaller &Caller,
		const char *Language,
		flw::sWFlow &Flow )
	{
	qRH;
		str::wString Id;
	qRB;
		tol::Init( Id );
		Caller.Get( Id );

		SetElement_( Set, Id, Caller, Language, Flow );
	qRR;
	qRT;
	qRE;
	}
}


SCLJRE_F( xdhp::SetLayout )
{
	rRack_ &Rack = GetRack_( Caller );

	SetElement_( server::SetElementLayout, Caller, Rack.Language, Rack.Flow );

	return scljre::Null();
}

SCLJRE_F( xdhp::SetCasting )
{
	rRack_ &Rack = GetRack_( Caller );

	SetElement_( server::SetElementCasting, Caller, Rack.Language, Rack.Flow );

	return scljre::Null();
}






