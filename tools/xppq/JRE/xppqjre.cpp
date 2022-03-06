/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of xppq.

	xppq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	xppq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with xppq. If not, see <http://www.gnu.org/licenses/>.
*/

#include "xppqjre.h"

#include "sclm.h"
#include "scljre.h"

#include "iof.h"
#include "xpp.h"
#include "lcl.h"

using namespace scljre;

void scljre::SCLJREInfo( txf::sWFlow &Flow )
{
	Flow << NAME_MC << " v" << VERSION << txf::nl
		<< txf::pad << "Build : " __DATE__ " " __TIME__ " (" << cpe::GetDescription() << ')';
}

namespace parsing_ {
	namespace {
		class rParser_
		{
		private:
			rInputStreamRDriver Stream_;
			flw::rDressedRFlow<> IFlow_;
			xtf::sRFlow XFlow_;
			xml::rParser Parser_;
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Stream_, IFlow_, XFlow_, Parser_ );
			}
			qCDTOR( rParser_ );
			void Init( sCaller &Caller )
			{
				Stream_.Init( Caller );
				IFlow_.Init( Stream_ );
				XFlow_.Init( IFlow_, utf::f_Default );
				Parser_.Init( XFlow_, xml::eh_Default );
			}
			xml::rParser &operator()( void )
			{
				return Parser_;
			}
		};
	}

	SCLJRE_F( New )
	{
		rParser_ *Parser = NULL;
	qRH
	qRB
		Parser = qNEW(rParser_);

		Parser->Init( Caller );
	qRR
		qDELETE(Parser);
	qRT
	qRE
		return Long( Env, (scljre::sJLong)Parser );
	}

	SCLJRE_F( Delete )
	{
	qRH
		scljre::java::lang::rLong Long;
	qRB
		Long.Init( Caller.GetObject() );

		delete (rParser_ *)Long.LongValue();
	qRR
	qRT
	qRE
		return Null();
	}

	namespace {
		void Init_(
			sEnv *Env,
			scljre::java::lang::rString &String,
			const str::dString &Content )
		{
		qRH
			rJString CharsetName;
			scljre::rJByteArray Array;
		qRB
			CharsetName.Init( "UTF-8", n4jre::hOriginal );

			Array.Init( Content );

			String.Init( Env, Array, CharsetName );
		qRR
		qRT
		qRE
		}

		void Set_(
			sEnv *Env,
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

	SCLJRE_F( Parse )
	{
		sJInt Token = 0;
	qRH
		scljre::java::lang::rLong Long;
		lcl::wMeaning Meaning;
		lcl::locale Locale;
		str::wString Error;
		scljre::rObject Data;
	qRB
		Long.Init( Caller.GetObject() );
		rParser_ &Parser = *(rParser_ *)Long.LongValue();

		Data.Init( Caller.GetObject() );

		switch ( Parser().Parse( xml::tfObvious ) ) {
		case xml::t_Error:
			Meaning.Init();
			xml::GetMeaning( Parser().GetStatus(), Parser().Flow().Position(), Meaning );
			Locale.Init();
			Error.Init();
			Locale.GetTranslation( Meaning, "", Error );
			Throw( Env, Error );
			break;
		case xml::t_Processed:
			break;
		default:
			Set_( Env, "tagName", Parser().TagName(), Data );
			Set_( Env, "attributeName", Parser().AttributeName(), Data );
			Set_( Env, "value", Parser().Value(), Data );
			break;
		}

		// If modified, modify also Java source file.
		switch ( Parser().Token() ) {
		case xml::t_Error:
			break;
		case xml::t_Processed:
			Token = 0;
			break;
		case xml::tStartTag:
			Token = 1;
			break;
		case xml::tAttribute:
			Token = 2;
			break;
		case xml::tValue:
			Token = 3;
			break;
		case xml::tEndTag:
			Token = 4;
			break;
		default:
			qRGnr();
			break;
		}

	qRR
	qRT
	qRE
		return Integer( Env, Token );
	}

}

namespace processing_ {
	namespace {
		class rProcessor_
		{
		private:
			scljre::rInputStreamRDriver Input_;
			flw::rDressedRFlow<> Flow_;
			xtf::sRFlow XFlow_;
			xpp::rRFlow PFlow_;
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Input_, Flow_, XFlow_, PFlow_ );
			}
			void Init( sCaller &Caller )
			{
				Input_.Init( Caller );
				Flow_.Init( Input_ );
				XFlow_.Init( Flow_, utf::f_Default );
				PFlow_.Init(XFlow_, xpp::criterions___( str::wString() ) );
			}
			xpp::preprocessing_iflow___ &operator()( void )
			{
				return PFlow_;
			}
		};
	}

	SCLJRE_F( New )
	{
		rProcessor_ *Processor = NULL;
	qRH
	qRB
		Processor = qNEW(rProcessor_);

		Processor->Init( Caller );
	qRR
		qDELETE(Processor);
	qRT
	qRE
		return scljre::Long( Env, (scljre::sJLong)Processor );
	}

	SCLJRE_F( Delete )
	{
	qRH
		scljre::java::lang::rLong Long;
	qRB
		Long.Init( Caller.GetObject() );

		delete (rProcessor_ *)Long.LongValue();
	qRR
	qRT
	qRE
		return Null();
	}

	SCLJRE_F( Read )
	{
		sJByte Char = 0;
	qRH
		scljre::java::lang::rLong Long;
		lcl::meaning Meaning;
		lcl::locale Locale;
		str::wString Translation;
	qRB
		Long.Init( Caller.GetObject() );

		rProcessor_ &Processor = *(rProcessor_ *)Long.LongValue();

		if ( !Processor().EndOfFlow() )
			Char = Processor().Get();
		else if ( Processor().Status() == xpp::sOK )
			Char = -1;
		else {
			Meaning.Init();
			xpp::GetMeaning(Processor(), Meaning );
			Locale.Init();
			Translation.Init();
			sclm::GetBaseTranslation( Meaning, Translation );
			scljre::Throw( Env, Translation );
		}
	qRR
	qRT
	qRE
		return Integer( Env, Char );
	}
}

const scli::sInfo &scljre::SCLJRERegister( sRegistrar &Registrar )
{
	static scli::sInfo Info(NAME_LC, NAME_MC, "q37.info");

	Registrar.Register( 1, parsing_::New, parsing_::Delete, parsing_::Parse );
	Registrar.Register( 11, processing_::New, processing_::Delete,  processing_::Read );

	return Info;
}

