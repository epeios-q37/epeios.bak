/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

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

#include "xdhwebqnjs.h"

#include "registry.h"

#include "xdhwebq.h"

#include "sclargmnt.h"
#include "sclmisc.h"
#include "sclnjs.h"

void sclnjs::SCLNJSInfo( txf::sWFlow &Flow )
{
	Flow << NAME_MC << " v" << VERSION << txf::nl
		 << txf::pad << "Build : " __DATE__ " " __TIME__ " (" << cpe::GetDescription() << ')';
}

namespace {
	SCLNJS_F( ReturnArgument_ )
	{
	qRH;
		str::wString Input, Text;
	qRB;
		Input.Init();
		Caller.GetArgument( Input );

		Text.Init();
		sclmisc::GetBaseTranslation( "Argument", Text, Input );

		Caller.SetReturnValue( Text );
	qRR;
	qRT;
	qRE;
	}

	namespace {
		xdhwebq::rAgent Agent_;
		xdhwebq::wSessions UnprotectedSessions_;
		xdhwebq::rSessions Sessions_;
	}

	SCLNJS_F( Init )
	{
	qRH;
		str::wString Arguments, Identification, ModuleFilename;
		qCBUFFERr Buffer;
	qRB;
		Arguments.Init();
		Caller.GetArgument( Arguments );

		sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

		Identification.Init( NAME_LC " V" VERSION " Build " __DATE__ " " __TIME__ " - " );
		Identification.Append( cpe::GetDescription() );

		ModuleFilename.Init();
		sclmisc::MGetValue( registry::parameter::ModuleFilename, ModuleFilename );

		Agent_.Init( xdhcmn::mMultiUser, ModuleFilename, dlbrry::nExtOnly, Identification.Convert( Buffer ) );
		UnprotectedSessions_.Init( 0, 0, Agent_ );
		Sessions_.Init( UnprotectedSessions_ );
	qRR;
	qRT;
	qRE;
	}

	SCLNJS_F( HandleS_ )
	{
	qRH;
		str::wStrings Keys, Values;
		xdhwebq::wPairs Pairs;
		str::wString Response;
	qRB;
		tol::Init( Keys, Values );
		Caller.GetArgument( Keys, Values );

		Pairs.Init();
		Pairs.Add( Keys, Values );

		Response.Init();
		xdhwebq::Handle( Pairs, Sessions_, Response );

		Caller.SetReturnValue( Response );
	qRR;
	qRT;
	qRE;
	}

	namespace {
		struct rRack_
			: public sclnjs::cAsync
		{
		protected:
			virtual void UVQWork( void ) override
			{
				cio::COut << txf::tab << ">>>>>> " __LOC__ << txf::nl << txf::commit;
				xdhwebq::Handle( Pairs, Sessions_, Response );
				cio::COut << txf::tab << "<<<<<< " __LOC__ << txf::nl << txf::commit;
			}
			virtual sclnjs::eBehavior UVQAfter( void ) override
			{
				cio::COut << txf::tab << ">>>>>> " __LOC__ << txf::nl << txf::commit;
				Callback.VoidLaunch( Response );
				cio::COut << txf::tab << "<<<<<< " __LOC__ << txf::nl << txf::commit;
				return sclnjs::bExitAndDelete;
			}
		public:
			xdhwebq::wPairs Pairs;
			sclnjs::rCallback Callback;
			str::wString Response;
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Pairs, Callback, Response );
			}
			qCVDTOR( rRack_ );
			void Init( void )
			{
				tol::Init( Pairs, Callback, Response );
			}
		};
	}

	SCLNJS_F( HandleA_ )
	{
	qRH;
		str::wStrings Keys, Values;
		rRack_ *Rack = NULL;
	qRB;
		tol::Init( Keys, Values );
		Caller.GetArgument( Keys, Values );

		Rack = new rRack_;

		if ( Rack == NULL )
			qRAlc();

		Rack->Init();

		Caller.GetArgument( Rack->Callback );
		Rack->Pairs.Add( Keys, Values );
#if 0
		Rack->Work();
		Rack->After();
		delete Rack;
#else
		sclnjs::Launch( *Rack );
#endif
	qRR;
		if ( Rack != NULL )
			delete Rack;
	qRT;
	qRE;
	}
}

void sclnjs::SCLNJSRegister( sclnjs::sRegistrar &Registrar )
{
	Registrar.Register( ReturnArgument_ );
	Registrar.Register( Init, HandleS_, HandleA_ );
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;
// const char *scln4a::SCLN4AProductVersion = VERSION;
