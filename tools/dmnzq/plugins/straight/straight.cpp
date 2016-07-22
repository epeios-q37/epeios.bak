/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of dmnzq.

    dmnzq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    dmnzq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with dmnzq.  If not, see <http://www.gnu.org/licenses/>
*/

// 'straight' slot plugin.

#include "misc.h"
#include "registry.h"

#include "csdbns.h"
#include "csdmxs.h"

#include "sclplugin.h"
#include "sclmisc.h"

#define PLUGIN_NAME	"straight"

using misc::cHandler;

using misc::sModule;

namespace {
	csdmxs::rLogCallback LogCallback_;

	csdbns::port__ GetPort_( void )
	{
		return sclmisc::MGetU16( registry::Service );
	}

	class rPlugin
	: public cHandler
	{
	private:
		csdbns::server___ Server_;
		csdmxs::rCallback Muxer_;
	protected:
		virtual void MISCHandle( sModule &Module ) override
		{
//			LogCallback_.Init("h:/temp/MXSLog.txt" );

			Muxer_.Init( Module );
			Server_.Init( GetPort_(), Muxer_ );
			Server_.Process( NULL );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			Server_.reset( P );
			Muxer_.reset();
		}
		E_CVDTOR( rPlugin );
		void Init( void )
		{
			reset();	// See above for initialization.
		}
		bso::sBool SCLPLUGINInitialize( plgn::sAbstract *Abstract )
		{
			if ( Abstract != NULL )
				qRGnr();

			// Made in 'MISCProcess(...)'.

			return true;
		}
	};
}

SCLPLUGIN_DEF( rPlugin );

const char *sclmisc::SCLMISCTargetName = PLUGIN_NAME;

void sclplugin::SCLPLUGINPluginIdentifier( str::dString &Identifier )
{
	Identifier.Append( IDENTIFIER );
}

void sclplugin::SCLPLUGINPluginDetails( str::dString &Details )
{
	Details.Append( PLUGIN_NAME " V" VERSION " - Build " __DATE__ " " __TIME__ " (" );
	Details.Append( cpe::GetDescription() );
	Details.Append( ')' );
}

