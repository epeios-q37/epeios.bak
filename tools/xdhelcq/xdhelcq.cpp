/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XDHELCq.

    XDHELCq is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XDHELCq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XDHELCq.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "xdhelcq.h"

#include "registry.h"

#include "nodeq.h"

#include "sclerror.h"
#include "sclargmnt.h"
#include "sclmisc.h"

#include "dir.h"
#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "xdhujp.h"
#include "xdhups.h"
#include "xdhutl.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

namespace {
	namespace {
		void GetWrapperInfo_( str::dString &Info )
		{
		qRH
			flx::rStringOFlow BaseFlow;
			txf::sWFlow Flow;
		qRB
			BaseFlow.Init( Info );
			Flow.Init( BaseFlow );

			Flow << NAME_MC << " v" << VERSION << " - Node v" NODE_VERSION_STRING " ABI v" NODE_STRINGIFY( NODE_MODULE_VERSION ) << txf::nl
			<< txf::pad << "Build : " __DATE__ " " __TIME__ " (" << cpe::GetDescription() << ')';
		qRR
		qRT
		qRE
		}
	}

	void GetWrapperInfo_( const v8::FunctionCallbackInfo<v8::Value>& Args )
	{
	qRH
		str::wString Info;
		v8q::sLString String;
	qRB
		Info.Init();

		GetWrapperInfo_( Info );

		String.Init( Info );

		Args.GetReturnValue().Set( String.Core() );
	qRR
	qRT
	qRE
	}

	void GetModuleInfo_( const v8::FunctionCallbackInfo<v8::Value>& Args )
	{
	qRH
		str::wString Info;
		v8q::sLString String;
	qRB
		Info.Init();

//		if ( !wrapper::GetLauncherInfo( Info ) )
			sclmisc::GetBaseTranslation( "NoRegisteredComponent", Info );

		String.Init( Info );

		Args.GetReturnValue().Set( String.Core() );
	qRR
	qRT
	qRE
	}
}

namespace {

	void InitWithModuleFilename_(
		v8::Local<v8::Value> Module,
		v8q::sLString &Filename )
	{
		nodeq::sLObject Object;

		Object.Init( Module );

		Filename.Init( Object.Get( "filename" ) );
	}

	void GetModuleFilename_(
		v8::Local<v8::Value> Module,
		str::dString &Filename )
	{
	qRH
		v8q::sLString V8Filename;
	qRB
		InitWithModuleFilename_( Module, V8Filename );

		V8Filename.Get( Filename );
	qRR
	qRT
	qRE
	}
#if 0
	void GetParentModuleFilename_(
		v8::Local<v8::Value> Module,
		str::dString &Filename )
	{
		nodeq::sLObject Object;

		Object.Init( Module );

		GetModuleFilename_( Object.Get( "parent" ), Filename );
	}
#endif
	// Returns true if 'Filename' ends with '.node'.
	bso::sBool IsNJSq_( const fnm::rName &Filename )
	{
		bso::sBool Is = false;
	qRH
		fnm::rName Basename;
		str::wString Buffer;
	qRB
		Basename.Init();
		fnm::GetBasename( Filename, Basename );

		Buffer.Init();
		Basename.UTF8( Buffer );

		Is = Buffer == NAME_LC ".node";
	qRR
	qRT
	qRE
		return Is;
	}
	void GetAddonFilename_(
		v8::Local<v8::Value> Module,
		str::dString &Filename )
	{
		Filename.Init();
		GetModuleFilename_( Module, Filename );

		if ( !IsNJSq_( Filename ) )
			qRGnr();
	}
}

void GetAddonLocation_(
	v8::Local<v8::Value> Module,
	str::dString &Location )
{
qRH
	str::wString Filename;
	fnm::rName Path;
qRB
	Filename.Init();

	GetAddonFilename_( Module, Filename );

	Path.Init();
	fnm::GetLocation( Filename, Path );

	Path.UTF8( Location );
qRR
qRT
qRE
}

namespace {
	void OnExit_( void *UP )
	{
		qRVct();
//		wrapper::DeleteLauncher();
	}
}

namespace {
	err::err___ qRRor_;
	sclerror::rError SCLError_;
	scllocale::rRack Locale_;
	sclmisc::sRack Rack_;
}

namespace {
	void ErrFinal_( v8::Isolate *Isolate = NULL )
	{
	qRH
		str::wString Message;
		err::buffer__ Buffer;
	qRB
		Isolate = v8q::GetIsolate( Isolate );

		Message.Init();

		if ( ERRType != err::t_Abort ) {
			Message.Append( err::Message( Buffer ) );

			ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.
		} else if ( sclerror::IsErrorPending() )
			sclmisc::GetSCLBasePendingErrorTranslation( Message );

		if ( Isolate != NULL )
			Isolate->ThrowException( v8::Exception::Error( v8q::ToString( Message ) ) );
		else
			cio::CErr << txf::nl << Message << txf::nl;
	qRR
		ERRRst();
	qRT
	qRE
	}
}

namespace {
	typedef xdhujp::cJS cJS_;

	class sJS
	: public cJS_ {
	protected:
		virtual void XDHUJPExecute(
			const str::string_ &Script,
			TOL_CBUFFER___ *Return ) override
		{
		qRH;
			qCBUFFERr Buffer;
			v8::Local<v8::Value> V8Return;
			v8::Local<v8::String> String;
			bso::integer_buffer__ IBuffer;
		qRB;
			V8Return = v8q::Execute( Script.Convert( Buffer ), v8q::GetIsolate() );

			if ( Return != NULL ) {
				if ( !V8Return->IsNullOrUndefined() ) {
					if ( V8Return->IsString() ) {
						if ( !( String = V8Return->ToString() ).IsEmpty() ) {
							Return->Malloc( String->Utf8Length() + 1 );	// '+ 1' for the NULL termination character.
							String->WriteUtf8( *Return );
						} else
							Return->reset();
					} else if ( V8Return->IsInt32() ) {
						str::string( bso::Convert( ( bso::int__ )V8Return->IntegerValue(), IBuffer ) ).Convert( *Return );
					} else if ( V8Return ->IsBoolean() ) {
						if ( V8Return->ToBoolean()->IsTrue() )
							str::string( "true" ).Convert( *Return );
						else
							str::string( "false" ).Convert( *Return );
					} else
						Return->reset();
				} else
					Return->reset();
			}
		qRR;
		qRT;
		qRE;
		}
		virtual void XDHUJPGetWidgetAttributeName( TOL_CBUFFER___ &Buffer ) override
		{
			sclmisc::MGetValue( registry::custom_item::attribute_name::Widget, Buffer );
		}
		virtual void XDHUJPGetResultAttributeName( TOL_CBUFFER___ &Buffer ) override
		{
			sclmisc::MGetValue( registry::custom_item::attribute_name::Result, Buffer );
		}
		/*
		virtual void XDHJSPHandleExtensions( const xdhcbk::nstring___ &Id ) override
		{
		HandleExtensions_( Id, _A() );
		}
		virtual void XDHJSPHandleCastings( const xdhcbk::nstring___ &Id ) override
		{
		HandleCastings_(Id, _A() );
		}
		*/
	public:
		void reset( bso::bool__ P = true )
		{
			// Standardization.
		}
		E_CVDTOR( sJS );
		void Init( void )
		{
			// Standardization.
		}
	};

	sJS JS_;
	xdhups::sSession Session_;
	xdhups::agent___ Agent_;
	TOL_CBUFFER___ LanguageBuffer_, IdentificationBuffer_;

	void InitializeSession_( void )
	{
	qRH;
		xdhujp::sProxyCallback *ProxyCallback = NULL;
		qCBUFFERr Buffer;
		str::wString ModuleFilename, Identification;
	qRB;
		ModuleFilename.Init();
		sclmisc::MGetValue( registry::parameter::ModuleFilename, ModuleFilename );

		ProxyCallback = new xdhujp::sProxyCallback;	// Destruction is made by '_Session'.

		if ( ProxyCallback == NULL )
			qRGnr();

		::JS_.Init();

		ProxyCallback->Init( ::JS_ );

		Identification.Init( NAME_LC " V" VERSION );
		Identification.Append( " - Node v" NODE_VERSION_STRING "; ABI v" NODE_STRINGIFY( NODE_MODULE_VERSION ) " - " );
		Identification.Append( "Build " __DATE__ " " __TIME__ " - " );
		Identification.Append( cpe::GetDescription() );
		Agent_.Init( xdhcmn::mMonoUser, ModuleFilename, dlbrry::n_Default, Identification.Convert( IdentificationBuffer_ ) );

		Session_.Init( Agent_.RetrieveCallback( Agent_.BaseLanguage( LanguageBuffer_ ), ProxyCallback ) );
		sclmisc::SetBaseLanguage( str::wString( Agent_.BaseLanguage( Buffer ) ) );
	qRR;
		if ( ProxyCallback != NULL )
			delete ProxyCallback;
	qRT;
	qRE;
	}

	void Initialize_( const v8::FunctionCallbackInfo<v8::Value>& Args )
	{
	qRFH;
		v8q::sLString RawArguments;
		str::wString Arguments, NormalizedArguments;
	qRFB;
		RawArguments.Init( Args[0]);

		Arguments.Init();
		RawArguments.Get( Arguments );

		NormalizedArguments.Init();
		sclargmnt::Normalize( Arguments, NormalizedArguments );

		sclargmnt::FillRegistry( NormalizedArguments, sclargmnt::faIsCommand, sclargmnt::uaIgnore );

		InitializeSession_();
	qRFR;
	qRFT;
	qRFE( ErrFinal_() );
	}


	void Execute_( const v8::FunctionCallbackInfo<v8::Value>& Args )
	{
	qRFH;
		v8q::sLString String;
		str::wString Script;
	qRFB;
		String.Init( Args[0] );

		Script.Init();
		String.Get( Script );

		JS_.Execute( Script );
	qRFR;
	qRFT;
	qRFE( ErrFinal_() );
	}
}

namespace {
	namespace {
		bso::bool__ HandleEvent_( const str::string_  &Digest )
		{
			bso::bool__ Stop = true;
		qRH
			str::string Id;
			xdhutl::event_abstract Abstract;
			TOL_CBUFFER___ IdBuffer, ActionBuffer;
		qRB
			Id.Init();
			Abstract.Init();
			if ( xdhutl::FetchEventAbstract(Digest, Id, Abstract) ) {
				if ( xdhutl::IsPredefined( Abstract.Action() ) )
					qRVct();	//  HandlePredefinedAction_( Abstract.Action(), Abstract.UserAction, _B(), Id, Abstract.Args );
				else if ( Abstract.Action() == xdhutl::a_User )
					Stop = Session_.Launch( Id.Convert( IdBuffer ), Abstract.UserAction.Convert( ActionBuffer ) );
				else
					qRGnr();
			}
		qRR
		qRT
		qRE
			return Stop;
		}
	}

	void LaunchEvent_( const v8::FunctionCallbackInfo<v8::Value>& Args )
	{
	qRFH;
		v8q::sLString String;
		str::wString Digest;
	qRFB;
		String.Init( Args[0] );

		Digest.Init();
		String.Get( Digest );
	
		HandleEvent_( Digest );
	qRFR;
	qRFT;
	qRFE( ErrFinal_() );
	}
}

void Start(
	v8::Local<v8::Object> Exports,
	v8::Local<v8::Value> Module,
	void* priv )
{
qRFH
	str::wString Location;
	str::wString Filename;
qRFB
	NODE_SET_METHOD( Exports, "wrapperInfo", GetWrapperInfo_ );
	NODE_SET_METHOD( Exports, "moduleInfo", GetModuleInfo_ );
	NODE_SET_METHOD( Exports, "initialize", Initialize_ );
	NODE_SET_METHOD( Exports, "execute", Execute_ );
	NODE_SET_METHOD( Exports, "launchEvent", LaunchEvent_ );

	cio::Initialize( cio::GetConsoleSet() );

	qRRor_.Init();
	SCLError_.Init();
	Locale_.Init();

	Rack_.Init( qRRor_, SCLError_, cio::GetSet( cio::t_Default ), Locale_ );

	Location.Init();
	GetAddonLocation_( Module, Location );

	sclmisc::Initialize( Rack_, Location );

	node::AtExit( OnExit_, NULL );
qRFR
qRFT
qRFE( ErrFinal_() )
}

NODE_MODULE( xdhelcq, Start );

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;

