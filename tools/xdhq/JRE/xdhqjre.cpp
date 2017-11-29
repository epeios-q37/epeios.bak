/*
	Copyright (C) 2017 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHq.

	XDHq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with xdhq. If not, see <http://www.gnu.org/licenses/>.
*/

#include "xdhqjre.h"

#include "prtcl.h"

#include "registry.h"

#include "csdbns.h"
#include "iof.h"
#include "xpp.h"
#include "lcl.h"
#include "scljre.h"

void scljre::SCLJREInfo( txf::sOFlow &Flow )
{
	Flow << NAME_MC << " v" << VERSION << txf::nl
		 << txf::pad << "Build : " __DATE__ " " __TIME__ " (" << cpe::GetDescription() << ')';
}

namespace {
	SCLJRE_F( ReturnArgument_ )
	{
		scljre::sJObject Return;
	qRH
		str::wString Input, Text;
	qRB
		Input.Init();
		Caller.Get( Input );

		Text.Init();
		sclmisc::GetBaseTranslation( "Argument", Text, Input );

		Return = scljre::String( Text );
	qRR
	qRT
	qRE
		return Return;
	}

	namespace {
		csdbns::rListener Listener_;

		struct rRack_ {
			sck::socket_ioflow___ Flow;
			qCBUFFERr Language;
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Flow, Language );
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
			qRR;
			qRT;
			qRE;
			}
		};
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

			Caller.Get( XML );
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

	SCLJRE_F( Initialize_ )
	{
	qRH;
		csdcmn::sVersion Version = csdcmn::UndefinedVersion;
		scljre::java::lang::rShort Port;
	qRB;
		Port.Init( Caller.Get() );

		Listener_.Init( Port.ShortValue() );
	qRR;
	qRT;
	qRE;
		return scljre::Integer( 0 );
	}

	SCLJRE_F( New_ )
	{
		rRack_ *Rack;
	qRH;
		const char *IP = NULL;
	qRB;
		Rack = new rRack_;

		if ( Rack == NULL )
			qRGnr();

		Rack->Init( Listener_.GetConnection( IP ) );
	qRR;
		if ( Rack != NULL )
			delete Rack;
	qRT;
	qRE;
		return scljre::Long( (scljre::sJLong)Rack );
	}

	SCLJRE_F( Delete_ )
	{
	qRH;
		scljre::java::lang::rLong Long;
		rRack_ *Rack = NULL;
	qRB;
		Long.Init( Caller.Get() );
		Rack = (rRack_ *)Long.LongValue();

		if ( Rack == NULL )
			qRGnr();

		delete Rack;
	qRR;
	qRT;
	qRE;
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

	namespace {
		rRack_ &GetRack_( scljre::sCaller &Caller )
		{
			rRack_ *Rack = NULL;
		qRH;
			scljre::java::lang::rLong Long;
		qRB;
			Long.Init( Caller.Get() );

			Rack = (rRack_ *)Long.LongValue();

			if ( Rack == NULL )
				qRGnr();
		qRR;
		qRT;
		qRE;
			return *Rack;
		}

		flw::sRWFlow &GetFlow_( scljre::sCaller &Caller )
		{
			return GetRack_( Caller ).Flow;
		}
	}

	SCLJRE_F( GetAction_ )
	{
		flw::sRWFlow &Flow = GetFlow_( Caller );

		prtcl::PutAnswer( prtcl::aOK_1, Flow );
		Flow.Commit();
		GetAction_( Flow, Caller );

		return scljre::Null();
	}

	SCLJRE_F( SetLayout_ )
	{
		rRack_ &Rack = GetRack_( Caller );

		SetElement_( prtcl::aSetLayout_1, Caller, Rack.Language, Rack.Flow );

		return scljre::Null();
	}

	SCLJRE_F( SetCasting_ )
	{
		rRack_ &Rack = GetRack_( Caller );

		SetElement_( prtcl::aSetCasting_1, Caller, Rack.Language, Rack.Flow );

		return scljre::Null();
	}

}

void scljre::SCLJRERegister( scljre::sRegistrar &Registrar )
{
	Registrar.Register( ReturnArgument_ );	// 0
	Registrar.Register( Initialize_, New_, Delete_ );	// 1 - 3
	Registrar.Register( GetAction_ );	// 4
	Registrar.Register( SetLayout_, SetCasting_ );	// 5 - 6
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;
