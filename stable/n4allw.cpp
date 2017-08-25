/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#define N4ALLW_COMPILATION_

#include "n4allw.h"

using namespace n4allw;

namespace {
	using n4all::cLauncher;

	cLauncher *Launcher_ = NULL;
}

void n4allw::SetLauncher( cLauncher *Launcher )
{
	if ( Launcher_ != NULL )
		qRGnr();

	if ( Launcher == NULL )
		qRGnr();

	Launcher_ = Launcher;
}

cLauncher &n4allw::GetLauncher( void )
{
	if ( Launcher_ == NULL )
		qRGnr();

	return *Launcher_;
}

bso::sBool n4allw::GetLauncherInfo( str::dString &Info )
{
	if ( Launcher_ != NULL ) {
		Launcher_->Info( Info );
		return true;
	} else
		return false;
}

void n4allw::DeleteLauncher( void )
{
	if ( Launcher_ != NULL )
		delete Launcher_;
}

namespace {
	namespace {
		bch::qBUNCHwl( void * ) Functions_;
		err::err___ qRRor_;
		sclerror::rError SCLError_;
		scllocale::rRack Locale_;
		sclmisc::sRack Rack_;
	}

	typedef n4all::cRegistrar cRegistrar_;

	class sRegistrar_
		: public cRegistrar_ {
	protected:
		virtual void N4ALLRegister( void *Function ) override
		{
			Functions_.Append( Function );
		}
	public:
		void Init( void )
		{}
	};

	namespace {
		dlbrry::rDynamicLibrary Library_;
	}

	extern "C" typedef n4all::fRegister fRegister_;

	bso::bool__ Register_(
		const fnm::rName &ComponentFilename,
		n4all::cRegistrar &Registrar,
		sclmisc::sRack &Rack,
		void *UP )
	{
		bso::bool__ Success = false;
	qRH
		n4all::sData Data;
		fnm::name___ Location;
		TOL_CBUFFER___ Buffer;
	qRB
		Location.Init();
		fnm::GetLocation( ComponentFilename, Location );
		Data.Init( Rack, Location, str::wString(), UP );

		//		cio::COut << __LOC__ << AddonFilename << txf::nl << txf::commit;

		Library_.Init( ComponentFilename, dlbrry::nExtOnly );

		//		cio::COut << __LOC__ << txf::nl << txf::commit;

		fRegister_ *Register = dlbrry::GetFunction<fRegister_ *>( E_STRING( N4ALL_REGISTER_FUNCTION_NAME ), Library_ );

		if ( Register == NULL )
			qRReturn;

		SetLauncher( Register( &Registrar, &Data ) );

		Success = true;
	qRR
	qRT
	qRE
		return Success;
	}

}

bso::sBool n4allw::Register(
	const fnm::rName &ComponentFilename,
	sclmisc::sRack &Rack,
	void *UP,
	qRPN )
{
	sRegistrar_ Registrar;

	Functions_.Init();

	Registrar.Init();

	if ( !Register_( ComponentFilename, Registrar, Rack, UP ) ) {
		if ( qRP == err::hThrowException )
			qRFwk();
		return false;
	} else
		return true;
}

void *n4allw::GetFunction( sdr::sRow Row )
{
	if ( !Functions_.Exists( Row ) )
		qRGnr();

	return Functions_( Row );
}




