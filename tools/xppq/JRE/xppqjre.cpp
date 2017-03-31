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

#include "jre.h"

#include "sclmisc.h"
#include "scljre.h"

#include "iof.h"
#include "xpp.h"
#include "lcl.h"

# define NAME_MC			"XPPqJRE"
# define NAME_LC			"xppqjre"
# define NAME_UC			"XPPQJRE"
# define WEBSITE_URL		"http://q37.info/"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"

static void Print_( const char *Text )
{
	jre::java::lang::sSystem::Out().Println( Text );
	jre::java::lang::sSystem::Out().Flush();
}


static void Print_( jobject Object )
{
	jre::java::lang::sSystem::Out().Println( Object );
	jre::java::lang::sSystem::Out().Flush();
}

SCLJRE_DEF( XPPQ );

namespace processing_ {
	namespace {
		class rProcessor_
		{
		private:
			jre::rInputStreamIDriver Input_;
			flw::sDressedIFlow<> Flow_;
			xtf::extended_text_iflow__ XFlow_;
			xpp::preprocessing_iflow___ PFlow_;
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Input_, Flow_, XFlow_, PFlow_ );
			}
			void Init( jobject InputStream )
			{
				Input_.Init( InputStream );
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

	jobject New(
		JNIEnv *Env,
		const scljre::sArguments &Args )
	{
		rProcessor_ *Processor = new rProcessor_;

		if ( Processor == NULL )
			qRAlc();

		Processor->Init( Args.Get() );

		return jre::java::lang::sLong( (jlong)Processor );
	}

	jobject Delete(
		JNIEnv *Env,
		const scljre::sArguments &Args )
	{
		delete (rProcessor_ *)jre::java::lang::sLong( Args.Get() ).LongValue();

		return NULL;
	}

	jobject Read(
		JNIEnv *Env,
		const scljre::sArguments &Args )
	{
		jint Char = 0;
	qRH
		lcl::meaning Meaning;
		lcl::locale Locale;
		str::wString Translation;
	qRB
		rProcessor_ &Processor = *(rProcessor_ *)jre::java::lang::sLong( Args.Get() ).LongValue();

		if ( !Processor().EndOfFlow() )
			Char = Processor().Get();
		else if ( Processor().Status() == xpp::sOK )
			Char = -1;
		else {
			Meaning.Init();
			xpp::GetMeaning(Processor(), Meaning );
			Locale.Init();
			Translation.Init();
			Locale.GetTranslation( Meaning, "", Translation );
			scljre::Throw( Translation );
		}
	qRR
	qRT
	qRE
		return jre::java::lang::sInteger( Char );
	}
}

namespace parsing_ {
	namespace {
		class rParser_
		{
		private:
			jre::rInputStreamIDriver Stream_;
			flw::sDressedIFlow<> IFlow_;
			xtf::sIFlow XFlow_;
			xml::rParser Parser_;
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Stream_, IFlow_, XFlow_, Parser_ );
			}
			qCDTOR( rParser_ );
			void Init( jobject Stream )
			{
				Stream_.Init( Stream );
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

	jobject New(
		JNIEnv *Env,
		const scljre::sArguments &Args )
	{
		rParser_ *Parser = new rParser_;

		if ( Parser == NULL )
			qRAlc();

		Parser->Init( Args.Get() );

		return jre::java::lang::sLong( (jlong)Parser );
	}

	jobject Delete(
		JNIEnv *Env,
		const scljre::sArguments &Args )
	{
		delete (rParser_ *)jre::java::lang::sLong( Args.Get() ).LongValue();

		return NULL;
	}

	jobject Parse(
		JNIEnv *Env,
		const scljre::sArguments &Args )
	{
		jint Token = 0;
	qRH
		lcl::wMeaning Meaning;
		lcl::locale Locale;
		str::wString Error;
		jre::sObject Data;
	qRB
		rParser_ &Parser = *(rParser_ *)jre::java::lang::sLong( Args.Get() ).LongValue();

		Data.Init( Args.Get(), "XPPQData" );

		switch ( Parser().Parse( xml::tfObvious ) ) {
		case xml::t_Error:
			Meaning.Init();
			xml::GetMeaning( Parser().GetStatus(), Parser().Flow().Position(), Meaning );
			Locale.Init();
			Error.Init();
			Locale.GetTranslation( Meaning, "", Error );
			scljre::Throw( Error );
			break;
		case xml::t_Processed:
			break;
		default:
			Data.Set( "tagName", jre::sString::Signature, jre::sString( Parser().TagName() ) );
			Data.Set( "attributeName", jre::sString::Signature, jre::sString( Parser().AttributeName() ) );
			Data.Set( "value", jre::sString::Signature, jre::sString( Parser().Value() ) );
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
		return jre::java::lang::sInteger( Token );
	}

}

void scljre::SCLJRERegister( sRegistrar &Registrar )
{
	Registrar.Register( processing_::New,  processing_::Delete,  processing_::Read );
	Registrar.Register( parsing_::New, parsing_::Delete, parsing_::Parse );
}


const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;
const char *scljre::SCLJREProductVersion = VERSION;
