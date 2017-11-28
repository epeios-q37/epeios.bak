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
		sck::socket_ioflow___ Flow_;
	}

	namespace {
		void GetFile_(
			const fnm::rName &Filename,
			str::dString &Content )
		{
		qRH;
			flf::rIFlow FFlow;
			flx::rStringOFlow SFlow;
		qRB;
			FFlow.Init( Filename, err::h_Default );
			SFlow.Init( Content );
			flw::Copy( FFlow, SFlow );
		qRR
		qRT;
		qRE;
		}

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
			flw::sWFlow &Flow )
		{
		qRH;
			str::wString XSL;
		qRB;
			XSL.Init();
			GetFile_( XSLFilename, XSL );

			SetElementWithXSLContent_( Answer, Id, XML, XSL, Flow );
		qRR;
		qRT;
		qRE;
		}

		void SetElement_(
			prtcl::eAnswer Answer,
			const str::dString &Id,
			scljre::sCaller &Caller,
			flw::sWFlow &Flow )
		{
		qRH;
			str::wString XML, XSLFilename;
		qRB;
			tol::Init( XML, XSLFilename );

			Caller.Get( XML );
			Caller.Get( XSLFilename );

			SetElement_( Answer, Id, XML, XSLFilename, Flow );
		qRR;
		qRT;
		qRE;
		}

		void SetElement_(
			prtcl::eAnswer Answer,
			scljre::sCaller &Caller,
			flw::sWFlow &Flow )
		{
		qRH;
			str::wString Id;
		qRB;
			tol::Init( Id );
			Caller.Get( Id );

			SetElement_( Answer, Id, Caller, Flow );
		qRR;
		qRT;
		qRE;
		}

		void SetDocument_(
			prtcl::eAnswer Answer,
			scljre::sCaller &Caller,
			flw::sWFlow &Flow )
		{
			SetElement_( Answer, str::wString( "Root" ), Caller, Flow );
		}
	}

	SCLJRE_F( Initialize_ )
	{
	qRH;
		const char *IP = NULL;
		csdcmn::sVersion Version = csdcmn::UndefinedVersion;
	qRB;
		Listener_.Init( 12345 );
		Flow_.Init( Listener_.GetConnection( IP ), false, sck::NoTimeout );

		if ( ( Version = csdcmn::GetProtocolVersion( prtcl::ProtocolId, Flow_ ) ) != prtcl::ProtocolVersion )
			qRGnr();

		Flow_.Dismiss();

		SetDocument_( prtcl::aSetLayout_1, Caller, Flow_ );
	qRR;
	qRT;
	qRE;
		return scljre::Integer( 0 );
	}

	SCLJRE_F( Finalize_ )
	{
		Flow_.reset();
		Listener_.reset();

		return scljre::Integer( 0 );
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

	SCLJRE_F( GetAction_ )
	{
		prtcl::PutAnswer( prtcl::aOK_1, Flow_ );
		Flow_.Commit();
		GetAction_( Flow_, Caller );

		return scljre::Integer( 0 );
	}

	SCLJRE_F( SetLayout_ )
	{
		SetDocument_( prtcl::aSetLayout_1, Caller, Flow_ );

		return scljre::Integer( 0 );
	}


	SCLJRE_F( SetCasting_ )
	{
		SetDocument_( prtcl::aSetCasting_1, Caller, Flow_ );

		return scljre::Integer( 0 );
	}
}

void scljre::SCLJRERegister( scljre::sRegistrar &Registrar )
{
	Registrar.Register( ReturnArgument_ );
	Registrar.Register( Initialize_, Finalize_ );
	Registrar.Register( GetAction_ );
	Registrar.Register( SetLayout_, SetCasting_ );
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;
