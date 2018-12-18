/*
	Copyright (C) 2018 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

	'XDHq' is free software: you can redistribute it and/or modify it
	under the terms of the GNU Affero General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	'XDHq' is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLUGINS_INC_
# define PLUGINS_INC_

# include "str.h"
# include "tol.h"

# define PLUGINS_TOKEN_PLUGIN_TARGET	"Token"
# define PLUGINS_TOKEN_PLUGIN_VERSION	"0"

namespace plugins {
	static E_CDEF( char *, TokenPluginTarget, PLUGINS_TOKEN_PLUGIN_TARGET );
	static E_CDEF( char *, TokenPluginVersion, PLUGINS_TOKEN_PLUGIN_VERSION );


	class cToken
	{
	protected:
		// Returns true when the token is new.
		virtual bso::sBool PLUGINSHandle(
			const str::dString &Raw,
			str::dString &Normalized ) = 0;
	public:
		qCALLBACK( Token )
		bso::sBool Handle(
			const str::dString &Raw,
			str::dString &Normalized )
		{
			return PLUGINSHandle( Raw, Normalized );
		}
		bso::sBool Handle( str::dString &Token )
		{
			bso::sBool New = false;
		qRH;
			str::wString Normalized;
		qRB;
			Normalized.Init();

			New = Handle( Token, Normalized );

			Token = Normalized;
		qRR;
		qRT;
		qRE;
			return New;
		}
		static const char *Label( void );
	};
}

#endif
