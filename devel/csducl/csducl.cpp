/*
	'csducl' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'csducl' header file ('csducl.h').
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#define CSDUCL__COMPILATION

#include "csducl.h"

#include "cio.h"

using namespace csducl;

static stsfsm::automat TypeAutomat_;

static void FillTypeAutomat_( void )
{
	TypeAutomat_.Init();
	stsfsm::Fill( TypeAutomat_, t_amount, GetLabel );
}

type__ csducl::GetType( const str::string_ &Pattern )
{
	return stsfsm::GetId( Pattern, TypeAutomat_, t_Undefined, t_amount );
}

const char *csducl::GetLabel( type__ Type )
{
	switch ( Type ) {
	case tDaemon:
		return "Daemon";
		break;
	case tPlugin:
		return "Plug-in";
		break;
	default:
		ERRFwk();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}

bso::bool__ csducl::universal_client_core::Init(
	const char *Backend,
	csdlec::library_data__ &LibraryData,
	csdsnc::log_functions__ &Log,
	type__ Type,
	bso::uint__ PingDelay )	// Dlai maximum d'inactivit entre deux 'ping'.
{
	reset();

	bso::bool__ Success = false;

	switch ( Type ) {
	case tDaemon:
		Success = _DaemonAccess.Init( Backend, Log, PingDelay );
		break;
	case tPlugin:
		Success = _LibraryAccess.Init( Backend, LibraryData, err::hUserDefined );
		break;
	default:
		ERRPrm();
		break;
	}

	_Type = Type;

	return Success;
}

static void FillAutomats_( void )
{
	FillTypeAutomat_();
}

Q37_GCTOR( csducl )
{
	FillAutomats_();
}
