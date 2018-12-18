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

#include "plugins.h"

#define C( name )	case s##name : return #name; break

const char *plugins::GetLabel( eStatus Status )
{
	switch ( Status ) {
	case sNew:
	case sPending:
		qRGnr();	// This values are not errors, so we do not have to retrieve their labels.
		break;
	C( Bad );
	C( Forbidden );
	C( BadCredentials );
	C( WrongCredentials );
	default:
		qRFwk();
		break;
	}

	return NULL;	// To avoid a warning.
}

#undef C

const char *plugins::cToken::Label( void )
{
	return PLUGINS_TOKEN_PLUGIN_TARGET " V" PLUGINS_TOKEN_PLUGIN_VERSION " " CPE_DESCRIPTION;
}