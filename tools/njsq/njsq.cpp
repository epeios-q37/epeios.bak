/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'NJSq'.

    'NJSq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'NJSq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'NJSq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "registrar.h"
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

# define NAME_MC			"NJSq"
# define NAME_LC			"njsq"
# define NAME_UC			"NJSQ"
# define WEBSITE_URL		"http://q37.info"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	

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

	void GetAddonInfo_( const v8::FunctionCallbackInfo<v8::Value>& Args )
	{
	qRH
		str::wString Info;
	qRB
		Info.Init();

		wrapper::GetLauncherInfo( Info );

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

extern "C" typedef njs::fRegister fRegister_;

dlbrry::rDynamicLibrary Library_;

namespace {
	err::err___ qRRor_;
	sclerror::rError SCLError_;
	scllocale::rRack Locale_;
	sclmisc::sRack Rack_;
}

namespace {
	bso::bool__ Register_(
		const str::string_ &AddonFilename,
		njs::cRegistrar &Registrar )
	{
		bso::bool__ Success = false;
	qRH
		njs::sData Data;
		fnm::name___ Location;
		TOL_CBUFFER___ Buffer;
	qRB
		Location.Init();
		fnm::GetLocation( AddonFilename, Location );
		Data.Init( Rack_, Location, str::wString() );

		Library_.Init( AddonFilename );

		fRegister_ *Register = dlbrry::GetFunction<fRegister_ *>( E_STRING( NJS_REGISTER_FUNCTION_NAME ), Library_ );

		if ( Register == NULL )
			qRReturn;

		wrapper::SetLauncher( Register( &Registrar, &Data ) );

		Success = true;
	qRR
	qRT
	qRE
		return Success;
	}

	void Register_( const v8::FunctionCallbackInfo<v8::Value>& Info )
	{
	qRFH
		v8q::sString RawArguments;
		str::wString Arguments;
		str::wString AddonFilename;
		registrar::sRegistrar Registrar;
	qRFB
		RawArguments.Init( Info[0] );
		
		Arguments.Init();
		RawArguments.Get( Arguments );

		sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

		AddonFilename.Init();
		sclmisc::MGetValue( registry::parameter::AddonFilename, AddonFilename );

		Registrar.Init();
		Register_( AddonFilename, Registrar );
	qRFR
	qRFT
	qRFE( common::ErrFinal() )
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
	NODE_SET_METHOD( Exports, "addonInfo", GetAddonInfo_ );
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

	common::Functions.Init();
	/*
	error_::Initialize();

	error_::Launch();
	*/
qRFR
qRFT
qRFE( common::ErrFinal() )
}

NODE_MODULE( njsq, Start );

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;

