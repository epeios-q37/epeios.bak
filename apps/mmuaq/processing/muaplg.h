/*
	Copyright (C) 2016-2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

// MUA PLuGins

#ifndef MUAPLG_INC_
# define MUAPLG_INC_

# ifdef XXX_DBG
#  define MUAPLG__DBG
# endif

# include "muabsc.h"
# include "muaacc.h"

# define MUAPLG_AUTHENTICATION_PLUGIN_TARGET	"Authentication"
# define MUAPLG_AUTHENTICATION_PLUGIN_VERSION	"1"


namespace muaplg {
	template <typename callback> class sCommon_
	: public callback
	{
	public:
		void reset( bso::bool__ P = true )
		{
		}
		E_CVDTOR( sCommon_ );
		void Init( void )
		{
		}
	public:
		static const char *Label( void );
	};

	/* Authentication */

	qCDEF( char *, AuthenticationPluginTarget, MUAPLG_AUTHENTICATION_PLUGIN_TARGET );
	qCDEF( char *, AuthenticationPluginVersion, MUAPLG_AUTHENTICATION_PLUGIN_VERSION );

	typedef sCommon_<muaacc::cAuthentication> sAuthentication;
}


#endif
