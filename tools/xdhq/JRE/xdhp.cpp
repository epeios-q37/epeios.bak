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

#include "prtcl.h"

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
	csdcmn::sVersion Version = csdcmn::UndefinedVersion;
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
		sck::socket_ioflow___ Flow;
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
			csdcmn::sVersion Version = csdcmn::UndefinedVersion;
			str::wString Language;
		qRB;
			Flow.Init( Socket, true );

			if ( ( Version = csdcmn::GetProtocolVersion( prtcl::ProtocolId, Flow ) ) != prtcl::ProtocolVersion )
				qRGnr();

			Language.Init();
			prtcl::Get( Flow, Language );

			Flow.Dismiss();
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
	void GetAction_(
		flw::sRFlow &Flow,
		str::wString &Id,
		str::wString &Action )
	{
		if ( prtcl::GetRequest( Flow ) != prtcl::rLaunch_1 )
			qRGnr();

		prtcl::Get( Flow, Id );
		prtcl::Get( Flow, Action );
		Flow.Dismiss();
	}

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
		flw::sRFlow &Flow,
		scljre::sCaller &Caller )
	{
	qRH;
		str::wString Id, Action;
		scljre::rObject Data;
	qRB;
		tol::Init( Id, Action );
		GetAction_( Flow, Id, Action );

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

	prtcl::PutAnswer( prtcl::aOK_1, Flow );
	Flow.Commit();
	GetAction_( Flow, Caller );

	return scljre::Null();
}

namespace {
	void SetElementWithXSLContent_(
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

	void SetElement_(
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
		sclmisc::LoadAndTranslateTags( XSLFilename, Language, XSL, '#' );

		SetElementWithXSLContent_( Answer, Id, XML, XSL, Flow );
	qRR;
	qRT;
	qRE;
	}

	void SetElement_(
		prtcl::eAnswer Answer,
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

		SetElement_( Answer, Id, XML, XSLFilename, Language, Flow );
	qRR;
	qRT;
	qRE;
	}

	void SetElement_(
		prtcl::eAnswer Answer,
		scljre::sCaller &Caller,
		const char *Language,
		flw::sWFlow &Flow )
	{
	qRH;
		str::wString Id;
	qRB;
		tol::Init( Id );
		Caller.Get( Id );

		SetElement_( Answer, Id, Caller, Language, Flow );
	qRR;
	qRT;
	qRE;
	}
}


SCLJRE_F( xdhp::SetLayout )
{
	rRack_ &Rack = GetRack_( Caller );

	SetElement_( prtcl::aSetLayout_1, Caller, Rack.Language, Rack.Flow );

	return scljre::Null();
}

SCLJRE_F( xdhp::SetCasting )
{
	rRack_ &Rack = GetRack_( Caller );

	SetElement_( prtcl::aSetCasting_1, Caller, Rack.Language, Rack.Flow );

	return scljre::Null();
}






