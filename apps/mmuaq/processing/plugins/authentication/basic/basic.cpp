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

#include "muaplg.h"
#include "muainf.h"

#include "uys.h"
#include "lstcrt.h"

#define PLUGIN_NAME	"Dummy"

typedef muaplg::sAuthentication sPlugin_;

namespace {
	qROW( Row );

	sRow Search_(
		const str::dString &Pattern,
		const lstcrt::qLMCRATEd( str::dString, sRow ) &Crate )
		{
			sRow Row = Crate.First();

			while ( ( Row != qNIL ) && ( Crate( Row ) != Pattern ) )
				Row = Crate.Next(Row );

			return Row;
		}
}

class sPlugin
: public sPlugin_
{
private:
	lstcrt::qLMCRATEw( str::dString, sRow )
		Usernames_,
		Passwords_;
	lstbch::qLBUNCHw( muaacc::sRow, sRow ) Accounts_;
protected:
	virtual muaacc::sRow MUAACCAuthenticate(
		const str::dString &Username,
		const str::dString &Password,
		muaacc::lProvider &Provider ) override
	{
		sRow Row = qNIL;

		if ( Username.IsBlank() )
			return qNIL;

		if ( Password.IsBlank() )
			return qNIL;

		Row = Search_( Username, Usernames_ );

		if ( Row != qNIL ) {
			if ( Passwords_( Row ) != Password )
				Row = qNIL;
		} else {
			Row = Usernames_.New();

			Usernames_( Row ).Init( Username );

			if ( Row != Passwords_.New() )
				qRGnr();

			Passwords_(Row).Init( Password );

			if ( Accounts_.Add( Provider.Provide() ) != Row )
				qRGnr();
		}

		if ( Row != qNIL )
			return Accounts_( Row );
		else
			return qNIL;
	}
public:
	void reset( bso::bool__ P = true )
	{
		sPlugin_::reset( P );

		tol::reset( P, Usernames_, Passwords_, Accounts_ );
	}
	qCVDTOR( sPlugin );
	void Init( void )
	{
		sPlugin_::Init();

		tol::Init( Usernames_, Passwords_, Accounts_ );
	}
	bso::sBool SCLPLUGINInitialize( plgn::sAbstract *Abstract )
	{
		if ( Abstract != NULL )
			qRGnr();

		Init();

		return true;
	}
};

SCLPLUGIN_DEF( sPlugin );

const char *sclmisc::SCLMISCTargetName = PLUGIN_NAME;
const char *sclmisc::SCLMISCProductName = MUAINF_MC_AFFIX;

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


