/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

// 'Regular' database plugin.

# include "registry.h"

#include "sclmisc.h"
#include "sclplugin.h"

#include "ogzplg.h"

#include "uys.h"

#define PLUGIN_NAME	"Database"

namespace {
	typedef ogzplg::fDatabasePlugin fPlugin_;

	class rPlugin
	: public fPlugin_
	{
	private:
		ogzdta::rRegularCallback Data_;
		ogzclm::rRegularCallback Column_;
		ogzfld::rRegularCallback Field_;
		ogzrcd::rRegularCallback Record_;
		ogzusr::rRegularCallback User_;
	protected:
		virtual ogzdta::cData &OGZDTBGetDataCallback( void ) override
		{
			return Data_;
		}
		virtual ogzclm::cColumn &OGZDTBGetColumnCallback( void ) override
		{
			return Column_;
		}
		virtual ogzfld::cField &OGZDTBGetFieldCallback( void ) override
		{
			return Field_;
		}
		virtual ogzrcd::cRecord &OGZDTBGetRecordCallback( void ) override
		{
			return Record_;
		}
		virtual ogzusr::cUser &OGZDTBGetUserCallback( void ) override
		{
			return User_;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			fPlugin_::reset( P );
			Data_.reset( P );
			Column_.reset( P );
			Field_.reset( P );
			Record_.reset( P );
			User_.reset( P );
		}
		qCVDTOR( rPlugin );
		void Init( void )
		{
			fPlugin_::Init();
			Data_.Init();
			Column_.Init();
			Field_.Init();
			Record_.Init();
			User_.Init();
		}
		bso::sBool SCLPLUGINInitialize( plgn::sAbstract *Abstract )
		{
			if ( Abstract != NULL )
				qRGnr();

			Init();

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