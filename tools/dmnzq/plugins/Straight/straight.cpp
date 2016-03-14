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

#include "sclplugin.h"
#include "sclmisc.h"

#define PLUGIN_NAME	"straight"

using misc::cHandler;

using misc::sModule;

namespace {
	csdbns::port__ GetPort_( void )
	{
		return sclmisc::MGetU16( registry::Service );
	}

	class rPlugin
	: public cHandler
	{
	private:
		csdbns::server___ Server_;
	protected:
		virtual void MISCHandle( sModule &Module ) override
		{
			Server_.Init( GetPort_(), Module );
			Server_.Process();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			Server_.reset( P );
		}
		E_CVDTOR( rPlugin );
		void Init( void )
		{
			Server_.reset();	// See above for initialization will be made later.
		}
		void SCLPLUGINInitialize( void )
		{
			// Made in 'MISCProcess(...)'.
		}
	};
}

SCLPLUGIN_DEF( rPlugin );

const char *sclmisc::SCLMISCTargetName = PLUGIN_NAME;

void sclplugin::SCLPLUGINPluginIdentifier( str::dString &Identifier )
{
	Identifier.Append( IDENTIFIER );
}

void sclplugin::SCLPLUGINAboutPlugin( str::dString &About )
{
	About.Append( PLUGIN_NAME " V" VERSION " - Build " __DATE__ " " __TIME__ " (" );
	About.Append( cpe::GetDescription() );
	About.Append( ')' );
}

