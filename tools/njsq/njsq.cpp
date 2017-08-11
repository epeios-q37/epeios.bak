/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#include "sclerror.h"
#include "sclargmnt.h"

#include "dir.h"
#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

namespace {
	namespace {
		void GetWrapperInfo_( str::dString &Info )
		{
		qRH
			flx::rStringOFlow BaseFlow;
			txf::sOFlow Flow;
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
	qRB
		Info.Init();

		GetWrapperInfo_( Info );

		Args.GetReturnValue().Set( v8q::sString( Info ).Core() );
	qRR
	qRT
	qRE
	}

	void GetComponentInfo_( const v8::FunctionCallbackInfo<v8::Value>& Args )
	{
	qRH
		str::wString Info;
	qRB
		Info.Init();

		if ( !wrapper::GetLauncherInfo( Info ) )
			sclmisc::GetBaseTranslation( "NoRegisteredComponent", Info );

		Args.GetReturnValue().Set( v8q::sString( Info ).Core() );
	qRR
	qRT
	qRE
	}
}

namespace {

	void InitWithModuleFilename_(
		v8::Local<v8::Value> Module,
		v8q::sString &Filename )
	{
		Filename.Init( v8q::sObject( Module ).Get( "filename" ) );
	}

	void GetModuleFilename_(
		v8::Local<v8::Value> Module,
		str::dString &Filename )
	{
	qRH
		v8q::sString V8Filename;
	qRB
		InitWithModuleFilename_( Module, V8Filename );

		V8Filename.Get( Filename );
	qRR
	qRT
	qRE
	}

	void GetParentModuleFilename_(
		v8::Local<v8::Value> Module,
		str::dString &Filename )
	{
		GetModuleFilename_( v8q::sObject( Module ).Get( "parent" ), Filename );
	}

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
		wrapper::DeleteLauncher();
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

	void Register_( const v8::FunctionCallbackInfo<v8::Value>& Info )
	{
	qRFH
		v8q::sString RawArguments;
		str::wString Arguments;
		str::wString ComponentFilename;
	qRFB
		RawArguments.Init( Info[0] );
		
		Arguments.Init();
		RawArguments.Get( Arguments );

		sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

		ComponentFilename.Init();
		sclmisc::MGetValue( registry::parameter::ComponentFilename, ComponentFilename );

		wrapper::Register( ComponentFilename, Rack_ );
	qRFR
	qRFT
	qRFE( ErrFinal_() )
	}

	void Launch_( const v8::FunctionCallbackInfo<v8::Value>& Info )
	{
	qRFH
	qRFB
		wrapper::Launch( Info );
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
	NODE_SET_METHOD( Exports, "wrapperInfo", GetWrapperInfo_ );
	NODE_SET_METHOD( Exports, "componentInfo", GetComponentInfo_ );
	NODE_SET_METHOD( Exports, "register", Register_ );
	NODE_SET_METHOD( Exports, "_wrapper", wrapper::Launch );

	cio::Initialize( cio::GetConsoleSet() );

	qRRor_.Init();
	SCLError_.Init();
	Locale_.Init();

	Rack_.Init( qRRor_, SCLError_, cio::GetSet( cio::t_Default ), Locale_ );

	Location.Init();
	GetAddonLocation_( Module, Location );

	sclmisc::Initialize( Rack_, Location );

	node::AtExit( OnExit_, NULL );
	/*
	error_::Initialize();

	error_::Launch();
	*/
qRFR
qRFT
qRFE( ErrFinal_() )
}

NODE_MODULE( njsq, Start );

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;

