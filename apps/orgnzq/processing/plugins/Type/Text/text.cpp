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

// 'Text' field plugin.

# include "registry.h"

# include "typbse.h"

#include "sclmisc.h"
#include "sclplugin.h"

#include "ogzplg.h"

#include "uys.h"

#define PLUGIN_NAME	"Record"

typedef typbse::rPluginCommon rPlugin_;

class rPlugin
: public rPlugin_
{
private:
	qCBUFFERr LabelBuffer_;
protected:
	virtual void OGZTYPToXML(
		const ogzbsc::dDatum &Datum,
		str::dString &XML ) override
	{
		qRVct();
	}
public:
	void reset( bso::bool__ P = true )
	{
		rPlugin_::reset( P );
		LabelBuffer_.reset( P );
	}
	qCVDTOR( rPlugin );
	void Init( void )
	{
		rPlugin_::Init();
	}
	void SCLPLUGINInitialize( void )
	{
		Init();
	}
};

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
