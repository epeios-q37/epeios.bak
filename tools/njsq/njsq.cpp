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

#include "scltool.h"
#include "sclerror.h"

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
		void GetExtendedInfo_( str::dString &Info )
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
		void GetInfo_( str::dString &Info )
		{
			qRH
				flx::rStringOFlow BaseFlow;
			txf::sOFlow Flow;
			qRB
				BaseFlow.Init( Info );
			Flow.Init( BaseFlow );

			Flow << NAME_MC << " v" << VERSION << " - Build : " __DATE__ " " __TIME__;
			qRR
				qRT
				qRE
		}
	}

	void GetExtendedInfo_( const v8::FunctionCallbackInfo<v8::Value>& Args )
	{
		qRH
			str::wString Info;
		qRB
			Info.Init();

		GetExtendedInfo_( Info );

		Args.GetReturnValue().Set( v8q::sString( Info ).Core() );
		qRR
			qRT
			qRE
	}

	void GetInfo_( const v8::FunctionCallbackInfo<v8::Value>& Args )
	{
		qRH
			str::wString Info;
		qRB
			Info.Init();

		GetInfo_( Info );

		Args.GetReturnValue().Set( v8q::sString( Info ).Core() );
		qRR
			qRT
			qRE
	}
}

namespace {
	void GetParentModuleFilename_(
		v8::Local<v8::Value> Module,
		str::dString &Filename )
	{
		qRH
			char *Buffer = NULL;
		v8q::sString String;
		qRB
			String.Init( v8q::sObject( v8q::sObject( Module ).Get( "parent" ) ).Get( "filename" ) );
		Buffer = (char *)malloc( String.Size() + 1 );

		if ( Buffer == NULL )
			qRAlc();

		String.Get( Buffer );

		Filename.Append( Buffer, String.Size() );
		qRR
			qRT
			if ( Buffer != NULL )
				free( Buffer );
		qRE
	}
}

void GetParentModuleLocation_(
	v8::Local<v8::Value> Module,
	str::dString &Location )
{
	qRH
		str::wString Filename;
	fnm::rName Path;
	qRB
		Filename.Init();

	GetParentModuleFilename_( Module, Filename );

	Path.Init();
	fnm::GetLocation( Filename, Path );

	Path.UTF8( Location );
	qRR
		qRT
		qRE
}

namespace {
	err::err___ Error_;
	sclerror::rError SCLError_;
	scllocale::rRack Locale_;
	sclmisc::sRack Rack_;
}

extern "C" typedef njs::fRegister fRegister_;

dlbrry::rDynamicLibrary Library_;

bso::bool__ Register_(
	const str::string_ &AddonFilename,
	njs::cRegistrar &Registrar )
{
	bso::bool__ Success = false;
qRH
	njs::sSharedData Data;
	fnm::name___ Location;
	TOL_CBUFFER___ Buffer;
qRB
	Location.Init();
	Data.Init();

	Library_.Init( AddonFilename );

	fRegister_ *Register = dlbrry::GetFunction<fRegister_ *>( E_STRING( NJSCMN_REGISTER_FUNCTION_NAME ), Library_ );

	if ( Register == NULL )
		qRReturn;

	wrapper::SetLauncher( Register( &Registrar, &Data ) );

	Success = true;
qRR
qRT
qRE
	return Success;
}


void Register(
	v8::Local<v8::Object> Exports,
	v8::Local<v8::Value> Module,
	void* priv )
{
qRFH
	str::wString AddonFilename;
	str::wString Location;
	registrar::sRegistrar Registrar;
qRFB
	AddonFilename.Init();
	sclmisc::MGetValue( registry::parameter::AddonFilename, AddonFilename );

	NODE_SET_METHOD( Exports, "extendedInfo", GetExtendedInfo_ );
	NODE_SET_METHOD( Exports, "info", GetInfo_ );
	NODE_SET_METHOD( Exports, "_wrapper", wrapper::Launch );

	cio::Initialize( cio::GetConsoleSet() );
	Rack_.Init( Error_, SCLError_, cio::GetSet( cio::t_Default ), Locale_ );

	Location.Init();
	GetParentModuleLocation_( Module, Location );

	sclmisc::Initialize( Rack_, Location, qRPU );

	Registrar.Init();

	common::Functions.Init();
	Register_( AddonFilename, Registrar );
	/*
	error_::Initialize();

	error_::Launch();
	*/
qRFR
qRFT
qRFE( common::ErrFinal() )
}

NODE_MODULE( njsq, Register );

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;

