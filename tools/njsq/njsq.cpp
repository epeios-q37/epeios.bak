/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of NJSq.

    NJSq is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    NJSq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with NJSq.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "njsq.h"

#include "registry.h"
#include "wrapper.h"

#include "nodeq.h"

#include "scle.h"
#include "scla.h"

#include "dir.h"
#include "err.h"
#include "cio.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( njsq, NAME_LC, NAME_MC );

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
		} else if ( scle::IsErrorPending() )
			sclm::GetSCLBasePendingErrorTranslation( Message );

		if ( Isolate != NULL )
			Isolate->ThrowException( v8::Exception::Error( v8q::ToString( Message ) ) );
		else
			cio::CErr << txf::nl << Message << txf::nl;
	qRR
		ERRRst();
	qRT
	qRE
	}

	wrapper::rLauncher &GetLauncher_( const v8::FunctionCallbackInfo<v8::Value>& Args )
	{
		if ( Args.Length() < 1 )
			qRGnr();

		if ( !Args[0]->IsExternal() )
			qRGnr();

		return *(wrapper::rLauncher *)v8q::ToExternal( Args[0] )->Value();
	}
}

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

		Args.GetReturnValue().Set( String.Core());
	qRR
	qRT
	qRE
	}

	void GetComponentInfo_( const v8::FunctionCallbackInfo<v8::Value>& Args )
	{
	qRFH;
		str::wString Info;
		v8q::sLString String;
	qRFB;
		Info.Init();

		if ( !GetLauncher_( Args ).GetInfo( Info ) )
			sclm::GetBaseTranslation( "NoRegisteredComponent", Info );

		String.Init( Info );

		Args.GetReturnValue().Set( String.Core() );
	qRFR;
	qRFT;
	qRFE( ErrFinal_() );
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
	err::err___ qRRor_;
	scle::rError SCLError_;
	scll::rRack Locale_;
	sclm::sRack Rack_;
}

namespace {
	namespace {
		void Async_( n4njs::cAsync &Async )
		{
			uvq::Launch( Async );
		}

		n4njs::gShared Shared_;
	}

	void Register_( const v8::FunctionCallbackInfo<v8::Value>& Args )
	{
	qRFH;
		v8q::sLString RawArguments;
		str::wString Arguments;
		str::wString ComponentFilename;
		wrapper::rLauncher *Launcher = NULL;
	qRFB;
		RawArguments.Init( Args[0] );

		Arguments.Init();
		RawArguments.Get( Arguments );

		scla::FillRegistry( Arguments, scla::faIsArgument, scla::uaReport );

		ComponentFilename.Init();
		sclm::MGetValue( registry::parameter::ComponentFilename, ComponentFilename );

		Shared_.Async = Async_;

		if ( ( Launcher = new wrapper::rLauncher) == NULL )
			qRAlc();

		Launcher->Register( ComponentFilename, Rack_, Shared_ );

		Args.GetReturnValue().Set( v8q::ToExternal( Launcher ) );
	qRFR;
		if ( Launcher != 0 )
			delete Launcher;
	qRFT;
	qRFE( ErrFinal_() );

	}

	void Call_( const v8::FunctionCallbackInfo<v8::Value>& Args )
	{
	qRFH
	qRFB
		GetLauncher_( Args ).Call( Args );
	qRFR
	qRFT
	qRFE( ErrFinal_() )
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
	NODE_SET_METHOD( Exports, "_wrapperInfo", GetWrapperInfo_ );
	NODE_SET_METHOD( Exports, "_componentInfo", GetComponentInfo_ );
	NODE_SET_METHOD( Exports, "_register", Register_ );
	NODE_SET_METHOD( Exports, "_call", Call_ );

	cio::Initialize( cio::GetConsoleSet() );

	qRRor_.Init();
	SCLError_.Init();
	Locale_.Init();

	Rack_.Init( qRRor_, SCLError_, cio::GetSet( cio::t_Default ), Locale_ );

	Location.Init();
	GetAddonLocation_( Module, Location );

	sclm::Initialize( Rack_, Location, njsq::Info );

	/*
	node::AtExit( OnExit_, NULL );
	error_::Initialize();

	error_::Launch();
	*/
qRFR
qRFT
qRFE( ErrFinal_() )
}

NODE_MODULE( njsq, Start );
