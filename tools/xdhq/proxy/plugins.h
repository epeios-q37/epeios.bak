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

	// Here, the client is NOT the web browser, but the app. using a the Atlas library.
	qENUM( Status )
	{
		sOK,				// New client ; normal operation.
		// Below value stop the client and display an error message.
		sBad,				// Bad token (unrecognized format).
		sForbidden,			// Such token are forbidden,
		sBadCredentials,	// The credentials are not provided in the proper format.
		sWrongCredentials,	// Credentials are wrong.
		s_amount,
		s_Undefined
	};

	const char *GetLabel( eStatus Status );

	class cToken
	{
	protected:
		virtual eStatus PLUGINSHandle(
			const str::dString &Raw,
			str::dString &Normalized ) = 0;
	public:
		qCALLBACK( Token )
		eStatus Handle(
			const str::dString &Raw,
			str::dString &Normalized )
		{
			return PLUGINSHandle( Raw, Normalized );
		}
		eStatus Handle( str::dString &Token )
		{
			eStatus Status = s_Undefined;
		qRH;
			str::wString Normalized;
		qRB;
			Normalized.Init();

			Status = Handle( Token, Normalized );

			Token = Normalized;
		qRR;
		qRT;
		qRE;
			return Status;
		}
		static const char *Label( void );
	};
}

#endif
