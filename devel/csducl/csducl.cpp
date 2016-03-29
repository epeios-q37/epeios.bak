/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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
	case tLibrary:
		return "Library";
		break;
	case tRemote:
		return "Remote";
		break;
	default:
		qRFwk();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}

bso::bool__ csducl::universal_client_core___::InitLibrary(
	const str::string_ &LibraryPath,
	csdlec::library_data__ &LibraryData )
{
	bso::bool__ Success = false;
qRH
	TOL_CBUFFER___ Buffer;
qRB
	reset();

	if ( ( Success = _LibraryAccess.Init( LibraryPath.Convert( Buffer ), LibraryData, err::hUserDefined ) ) ) {
		_Type = tLibrary;
		_Location.Init( LibraryPath );
	}
qRR
qRT
qRE
	return Success;
}

sdr::sRow csducl::universal_client_core___::InitRemote(
	const str::string_ &PluginPath,
	const char *PluginIdentifier,
	const str::string_ &Parameters,
	const plgn::dAbstracts &Abstracts )
{
	sdr::sRow Row = qNIL;

	reset();

	if ( ( Row = _RemoteAccess.Init( PluginPath, PluginIdentifier, Parameters, Abstracts ) ) == qNIL ) {
		_Type = tRemote;
		_Location.Init( PluginPath );
	}

	return Row;
}

static void FillAutomats_( void )
{
	FillTypeAutomat_();
}

Q37_GCTOR( csducl )
{
	FillAutomats_();
}
