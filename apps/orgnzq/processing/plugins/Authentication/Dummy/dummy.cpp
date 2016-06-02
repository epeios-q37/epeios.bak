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

// 'Dummy' authetication plugin.

# include "registry.h"

#include "sclmisc.h"
#include "sclplugin.h"

#include "ogzplg.h"

#include "uys.h"

#define PLUGIN_NAME	"Dummy"

typedef ogzplg::fAuthenticationPlugin fPlugin_;

class fPlugin
: public fPlugin_
{
protected:
	virtual ogzusr::sRow OGZUSRAuthenticate(
		const str::dString &Username,
		const str::dString &Password )
	{
		return 0;
	}
public:
	void reset( bso::bool__ P = true )
	{
		fPlugin_::reset( P );
	}
	qCVDTOR( fPlugin );
	void Init( void )
	{
		fPlugin_::Init();
	}
	bso::sBool SCLPLUGINInitialize( plgn::sAbstract *Abstract )
	{
		if ( Abstract != NULL )
			qRGnr();

		Init();

		return true;
	}
};

SCLPLUGIN_DEF( fPlugin );

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


