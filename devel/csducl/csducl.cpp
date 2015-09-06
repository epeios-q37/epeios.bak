/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
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
	case tNone:
		return "None";
		break;
	case tDaemon:
		return "Daemon";
		break;
	case tLibrary:
		return "Library";
		break;
	default:
		qRFwk();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}

bso::bool__ csducl::universal_client_core___::Init(
	const features___ &Features,
	csdlec::library_data__ &LibraryData,
	csdsnc::log_callback__ *Log )
{
	bso::bool__ Success = false;
qRH
	TOL_CBUFFER___ Buffer;
qRB
	reset();

	switch ( Features.Type ) {
	case tNone:
		Success = true;
		break;
	case tDaemon:
		Success = _DaemonAccess.Init( Features.Location.Convert( Buffer ), Features.PingDelay, Log );
		break;
	case tLibrary:
		Success = _LibraryAccess.Init( Features.Location.Convert( Buffer ), LibraryData, err::hUserDefined );
		break;
	default:
		qRFwk();
		break;
	}

	_Type = Features.Type;
	_Location.Init( Features.Location );
qRR
qRT
qRE
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
