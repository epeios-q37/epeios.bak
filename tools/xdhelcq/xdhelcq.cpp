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

#include "scle.h"
#include "scla.h"

#include "dir.h"
#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "xdhups.h"
#include "xdhutl.h"
#include "xdhbrd.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( xdhelcq, NAME_LC, NAME_MC );

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
	scle::rError SCLError_;
	scll::rRack Locale_;
	sclm::sRack Rack_;
}

namespace {
	typedef xdhcuc::cSingle cSingle_;

	class rSingle_
	: public cSingle_
	{
	protected:
		virtual bso::sBool XDHCUCProcess(
			const str::string_ &Script,
			tht::rBlocker *Blocker,
			str::dString *ReturnedValue ) override
		{
			bso::sBool Success = true;
		qRH;
			qCBUFFERh Buffer;
			v8::Local<v8::Value> V8Return;
			v8::Local<v8::String> String;
			bso::integer_buffer__ IBuffer;
			v8::Isolate *Isolate = NULL;
		qRB;
//			cio::COut << "Script:" << Script << txf::nl << txf::commit;

			Isolate = v8q::GetIsolate();

			V8Return = v8q::Execute(Script.Convert( Buffer ), Isolate);

			if ( ReturnedValue != NULL ) {
				if ( Blocker != NULL) {
					Blocker->Unblock();
					Blocker = NULL;	// To avoid unblocking twice below.
				}
				if ( !V8Return->IsNull() && !V8Return->IsUndefined() ) {
					if ( V8Return->IsString() ) {
						if ( !V8Return->ToString(v8q::GetContext(Isolate)).ToLocal(&String) )
							qRGnr();

						if ( !String.IsEmpty() ) {
							Buffer.Malloc(String->Utf8Length(Isolate) + 1);	// '+ 1' for the NULL termination character.
							String->WriteUtf8(Isolate, Buffer);
							ReturnedValue->Append(Buffer);
						}
					} else if ( V8Return->IsInt32() ) {
						long int Temp = 0;

						Temp = V8Return->IntegerValue(v8q::GetContext(Isolate)).FromJust();

						ReturnedValue->Append( bso::Convert( (bso::int__ )Temp, IBuffer) );
					} else if ( V8Return ->IsBoolean() ) {
						if ( V8Return->ToBoolean(Isolate)->IsTrue() )
							ReturnedValue->Append( "true" );
						else
							ReturnedValue->Append( "false" );
					}
				}
			}
		qRFR
			Success = false;
		qRFT
				if ( Blocker != NULL)
					Blocker->Unblock();
		qRFE(sclm::ErrorDefaultHandling())
			return Success;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			// Standardization.
		}
		E_CVDTOR( rSingle_ );
		void Init(void)
		{
			// Standardization.
		}
	};

	rSingle_ Single_;
	xdhups::sSession Session_;
	xdhups::rAgent Agent_;
	TOL_CBUFFER___ LanguageBuffer_, IdentificationBuffer_;

	namespace faas_ {
		using namespace xdhcmn::faas;
	}

	namespace {
		namespace {
			typedef xdhcuc::cGlobal cGlobal_;

			class sGlobal_
			: public cGlobal_
			{
			protected:
				virtual faas_::sRow XDHCUCCreate(const str::dString &Token) override
				{
					return xdhbrd::Create(Token);
				}
				virtual void XDHCUCRemove(faas_::sRow Row) override
				{
					return xdhbrd::Remove(Row);
				}
				virtual void XDHCUCBroadcast(
					const str::dString &Script,
					faas_::sRow Row) override
				{
					xdhbrd::Broadcast(Script, Row);
				}
			public:
				void reset(bso::sBool = true)
				{}
				qCVDTOR(sGlobal_)
				void Init(void)
				{}
			};
		}

		sGlobal_ Global_;
	}

	xdhbrd::rXCallback Broadcaster_;

	bso::sBool InitializeSession_( const str::dString &Token )	// IF empty, PROD, otherwise DEMO.
	{
	  bso::sBool Return = false;
	qRH;
		str::wString ModuleFilename, Identification;
	qRB;
		Identification.Init( NAME_LC " V" VERSION );
		Identification.Append( " - Node v" NODE_VERSION_STRING "; ABI v" NODE_STRINGIFY( NODE_MODULE_VERSION ) " - " );
		Identification.Append( "Build " __DATE__ " " __TIME__ " - " );
		Identification.Append( cpe::GetDescription() );

		ModuleFilename.Init();
		if ( sclm::OGetValue( registry::parameter::ModuleFilename, ModuleFilename ) ) {
			Single_.Init();
			Global_.Init();

			// Library compiled with 'node-gyp', which doesn't put the 'lib' prefix on 'POSIX' systems, hence 'dlbrry::nExtOnly'.
			Agent_.Init( Global_, xdhcdc::mMonoUser, ModuleFilename, dlbrry::nExtOnly,  Identification.Convert( IdentificationBuffer_ ) );

	//		Session_.Init( Agent_.FetchCallback( Agent_.BaseLanguage( LanguageBuffer_ ), Token, Single_ ) );
			Session_.Init( *Agent_.FetchCallback() );
			Session_.Initialize( Single_, LanguageBuffer_, Token, str::Empty ) && (Broadcaster_.Init(Single_, Token) != qNIL);
			sclm::SetBaseLanguage( str::wString( LanguageBuffer_ ) );
			Return = true;
		}
	qRR;
	qRT;
	qRE;
    return Return;
	}

	void ErrFinal_(v8::Isolate *Isolate = NULL)
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
		} else if ( scle::IsErrorPending() )
			sclm::GetSCLBasePendingErrorTranslation( Message );

		if ( Isolate != NULL )
			Isolate->ThrowException( v8::Exception::Error( v8q::ToString( Message ) ) );

		xdhcmn::Escape( Message, 0 );

//		v8q::console::Log( Isolate, Message );

		cio::COut << txf::nl << Message << txf::nl << txf::commit;

		Message.InsertAt( "window.alert( '" );
		Message.Append( "');" );
		Single_.Process(Message);
	qRR
		ERRRst();
	qRT
	qRE
	}

	void Initialize_( const v8::FunctionCallbackInfo<v8::Value>& Args )
	{
	qRFH;
		v8q::sLString RawArguments;
		str::wString Arguments, NormalizedArguments, Token;
	qRFB;
		RawArguments.Init( Args[0] );

		Arguments.Init();
		RawArguments.Get( Arguments );

		NormalizedArguments.Init();
		scla::Normalize( Arguments, NormalizedArguments );

		scla::FillRegistry( NormalizedArguments, scla::faIsCommand, scla::uaIgnore );

		Token.Init();
		sclm::OGetValue( registry::parameter::Token, Token );

		if ( InitializeSession_( Token ) )
      Args.GetReturnValue().Set(true);
    else
      Args.GetReturnValue().Set(false);
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

		Single_.Process(Script);
	qRFR;
	qRFT;
	qRFE( ErrFinal_() );
	}
}

namespace {
	void GetModuleInfo_( const v8::FunctionCallbackInfo<v8::Value>& Args )
	{
	qRH;
		str::wString Info;
		v8q::sLString String;
	qRB;
		Info.Init();

		/*
			if ( !wrapper::GetLauncherInfo( Info ) )
				sclmisc::GetBaseTranslation( "NoRegisteredComponent", Info );
		*/

		CPq;

		Agent_.Info( Info );

		CPq;

		String.Init( Info );

		Args.GetReturnValue().Set( String.Core() );
	qRR;
	qRT;
	qRE;
	}

	void GetHead_( const v8::FunctionCallbackInfo<v8::Value>& Args )
	{
	qRFH;
		str::wString Head, Token;
		v8q::sLString String, RawToken;
	qRFB;
		tol::Init( Token, Head );

		if ( Args.Length() ) {
			RawToken.Init(Args[0]);
			RawToken.Get( Token );
		}

		Agent_.GetHead( Token, Head );

		String.Init( Head );

		Args.GetReturnValue().Set( String.Core() );
	qRFR;
	qRFT;
	qRFE( ErrFinal_() );
	}
}

namespace {
	void Get_(
		const xdhcmn::digest_ &Args,
		str::string_ &Title,
		str::string_ &Default,
		str::strings_ &Types )
	{
		xdhcmn::retriever__ Retriever;

		Retriever.Init( Args );

		Retriever.GetString( Title );

		if ( Retriever.Availability() != strmrg::aNone )
			Retriever.GetStrings( Types );

		if ( Retriever.Availability() != strmrg::aNone )
			Retriever.GetString( Default );
	}

	void Test_( const xdhcmn::dDigest &Digest )
	{
	qRH
		str::string Title, Default;
		str::strings Types;
		str::wString Out;
		flx::rStringWDriver Driver;
		txf::rWFlow TFlow;
	qRB
		Title.Init();
		Default.Init();
		Types.Init();

		Get_( Digest, Title, Default, Types );

		Out.Init();
		Driver.Init( Out );
		TFlow.Init( Driver );

		TFlow << "console.log( '" << Title << txf::tab << Default << txf::tab  << txf::tab << "');" << txf::commit;

		tol::reset( Driver, TFlow );

		Single_.Process(Out);
	qRR
	qRT
	qRE
	}

	namespace {
		void HandlePredefinedAction_(
			xdhutl::action__ Action,
			const str::string_ &UserAction,
			const str::string_ &TargetId,
			const xdhcmn::digest_ &Args )
		{
		qRH
		qRB
			switch ( Action ) {
			case xdhutl::aOpenFile:
			case xdhutl::aOpenFiles:
			case xdhutl::aSaveFile:
				qRVct();
//				JS_.Execute( str::wString( "const {dialog} = require('electron').remote;console.log( dialog.showOpenDialog( {properties: ['openFile', 'openDirectory', 'multiSelections']} ) )" ) );
//				JS_.Execute( str::wString( "const dialog = require('electron').remote;console.log( dialog.dialog.showOpenDialog( {properties: ['openFile', 'openDirectory', 'multiSelections']} ) )" ) );
//				JS_.Execute( str::wString( "console.log( require('electron').remote.dialog.showOpenDialog( {properties: ['openFile', 'openDirectory', 'multiSelections']} ) )" ) );
				Test_( Args );
				break;
			default:
				qRGnr();
				break;
			}
		qRR
		qRT
		qRE

		}
	}

	namespace {
#if 0
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
			if ( Digest.Amount() != 0 ) {
				if ( xdhutl::FetchEventAbstract( Digest, Id, Abstract ) ) {
					if ( xdhutl::IsPredefined( Abstract.Action() ) )
						HandlePredefinedAction_( Abstract.Action(), Abstract.UserAction, Id, Abstract.Args );
					else if ( Abstract.Action() == xdhutl::a_User )
						Stop = Session_.Launch( Id.Convert( IdBuffer ), Abstract.UserAction.Convert( ActionBuffer ) );
					else
						qRGnr();
				}
			} else
				Stop = Session_.Launch( IdBuffer, ActionBuffer );

		qRR
		qRT
		qRE
			return Stop;
		}
#endif
		bso::bool__ HandleEvent_( const str::string_  &Digest )
		{
			bso::bool__ Stop = true;
		qRH
			TOL_CBUFFER___ Buffer;
		qRB
			Stop = Session_.Handle(Digest.Convert(Buffer));
		qRR
		qRT
		qRE
			return Stop;
		}	}

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
qRFH;
	str::wString Location;
	str::wString Filename;
qRFB;
	NODE_SET_METHOD( Exports, "wrapperInfo", GetWrapperInfo_ );
	NODE_SET_METHOD( Exports, "moduleInfo", GetModuleInfo_ );
	NODE_SET_METHOD( Exports, "initialize", Initialize_ );
	NODE_SET_METHOD( Exports, "getHead", GetHead_ );
	NODE_SET_METHOD( Exports, "execute", Execute_ );
	NODE_SET_METHOD( Exports, "launchEvent", LaunchEvent_ );

	cio::Initialize( cio::GetConsoleSet() );

	qRRor_.Init();
	SCLError_.Init();
	Locale_.Init();

	Rack_.Init( qRRor_, SCLError_, cio::GetSet( cio::t_Default ), Locale_ );

	Location.Init();
	GetAddonLocation_( Module, Location );

	sclm::Initialize( Rack_, Location, xdhelcq::Info );

//	node::AtExit(NULL, OnExit_, NULL);
qRFR;
qRFT;
qRFE( ErrFinal_() );
}

NODE_MODULE( xdhelcq, Start );
