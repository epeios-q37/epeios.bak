/*
	Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHWebQ.

	XDHWebQ is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHWebQ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHWebQ. If not, see <http://www.gnu.org/licenses/>.
*/

#include "xdhwebqznd.h"

#include "registry.h"

#include "xdwmain.h"

#include "iof.h"
#include "xpp.h"
#include "lcl.h"
#include "sclargmnt.h"
#include "sclznd.h"

SCLI_DEF( xdhwebqznd, NAME_LC, "XDHWebQ" );


void sclznd::SCLZNDInfo( txf::sWFlow &Flow )
{
	Flow << NAME_MC << " v" << VERSION << txf::nl
		 << txf::pad << "Build : " __DATE__ " " __TIME__ " (" << cpe::GetDescription() << ')';
}

namespace {
	SCLZND_F( ReturnArgument_ )
	{
	qRH
		str::wString Input, Text;
	qRB
		Input.Init();
		Caller.Get( Input );

		Text.Init();
		sclmisc::GetBaseTranslation( "Argument", Text, Input );

		Caller.SetReturnValue( Text );
	qRR
	qRT
	qRE
	}

	namespace {
		xdwmain::rAgent Agent_;
		xdwmain::wSessions UnprotectedSessions_;
		xdwmain::rSessions Sessions_;
		bso::sBool Initialized_ = false;
	}

	SCLZND_F( Init_ )
	{
	qRH;
		str::wString Arguments, Identification, ModuleFilename;
		qCBUFFERr Buffer;
	qRB;
		if ( !Initialized_ ) {
			Initialized_ = true;
			Arguments.Init();
			Caller.Get( Arguments );

			sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

			Identification.Init( NAME_LC " V" VERSION " Build " __DATE__ " " __TIME__ " - " );
			Identification.Append( cpe::GetDescription() );

			ModuleFilename.Init();
			sclmisc::MGetValue( registry::parameter::ModuleFilename, ModuleFilename );

			Agent_.Init( xdhcmn::mMultiUser, ModuleFilename, dlbrry::nExtOnly, Identification.Convert( Buffer ) );
			UnprotectedSessions_.Init( 0, 0, Agent_ );
			Sessions_.Init( UnprotectedSessions_ );
			}
	qRR;
	qRT;
	qRE;
	}

	SCLZND_F( Handle_ )
	{
	qRH;
		str::wStrings Keys, Values;
		xdwmain::wPairs Pairs;
		str::wString Response;
	qRB;
		tol::Init( Keys, Values );
		Caller.Get( Keys, Values );

		Pairs.Init();
		Pairs.Add( Keys, Values );

		Response.Init();
		xdwmain::Handle( Pairs, Sessions_, Response );

		Caller.SetReturnValue( Response );
	qRR;
	qRT;
	qRE;
	}

}

const scli::sInfo &sclznd::SCLZNDRegister( sclznd::sRegistrar &Registrar )
{
	Registrar.Register( ReturnArgument_ );
	Registrar.Register( Init_, Handle_ );

	return xdhwebqznd::Info;
}
