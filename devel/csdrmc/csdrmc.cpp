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

#define CSDRMC__COMPILATION

#include "csdrmc.h"

using namespace csdrmc;

const char *csdrmc::callback__::Identification( void )
{
	return CSDRMC_PLUGIN_NAME " V" CSDRMC_PLUGIN_API_VERSION " " CPE_DESCRIPTION;
}

bso::bool__ csdrmc::core___::Init(
	const ntvstr::string___ &PluginNameAndLocation,
	const str::string_ &Arguments,
	err::handling__ ErrHandling )
{
	bso::bool__ Success = false;
qRH
	plgn::retriever___<callback__> Plugin;
qRB
	if ( Success = Plugin.Init( PluginNameAndLocation, Arguments, ErrHandling) )
		Driver_ = Plugin.Plugin().GetFlow();
qRR
qRT
qRE
	return Success;
}


