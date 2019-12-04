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

// OrGaniZer PLuGins

#ifndef OGZPLG_INC_
# define OGZPLG_INC_

# ifdef XXX_DBG
#	define OGZPLG__DBG
# endif

# include "ogzbsc.h"
# include "ogztyp.h"
# include "ogzdtb.h"

# include "plgn.h"

# define OGZPLG_TYPE_PLUGIN_TARGET	"Type"
# define OGZPLG_TYPE_PLUGIN_VERSION	"1"

# define OGZPLG_DATABASE_PLUGIN_TARGET	"Database"
# define OGZPLG_DATABASE_PLUGIN_VERSION	"1"

# define OGZPLG_AUTHENTICATION_PLUGIN_TARGET	"Authentication"
# define OGZPLG_AUTHENTICATION_PLUGIN_VERSION	"1"

namespace ogzplg {

	template <typename callback> class fCommon
	: public callback
	{
	public:
		void reset( bso::bool__ P = true )
		{
		}
		E_CVDTOR( fCommon );
		void Init( void )
		{
		}
	public:
		static const char *Label( void );
	};

	/* Type */

	static qCDEF( char *, TypePluginTarget, OGZPLG_TYPE_PLUGIN_TARGET );
	static qCDEF( char *, TypePluginVersion, OGZPLG_TYPE_PLUGIN_VERSION );

	typedef fCommon<ogztyp::cType> fTypePlugin;

	/* Database */

	static qCDEF( char *, DatabasePluginTarget, OGZPLG_DATABASE_PLUGIN_TARGET );
	static qCDEF( char *, DatabasePluginVersion, OGZPLG_DATABASE_PLUGIN_VERSION );

	typedef fCommon<ogzdtb::cDatabase> fDatabasePlugin;

	/* Authentication */

	static qCDEF( char *, AuthenticationPluginTarget, OGZPLG_AUTHENTICATION_PLUGIN_TARGET );
	static qCDEF( char *, AuthenticationPluginVersion, OGZPLG_AUTHENTICATION_PLUGIN_VERSION );

	typedef fCommon<ogzusr::cAuthentication> fAuthenticationPlugin;
}


#endif
