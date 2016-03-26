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

#define PLGN__COMPILATION

#include "plgn.h"

using namespace plgn;

#include "sclmisc.h"

bso::bool__ plgn::rLooseRetriever::Initialize(
	const ntvstr::string___ &PluginPath,
	const char *Label,
	const char *Identifier,
	const rgstry::entry__ &Configuration,
	const str::string_ &Arguments,
	err::handling__ ErrHandling )
{
qRH
	plgncore::sData Data;
	sclmisc::sRack SCLRack;
qRB
	if ( !SubInitialize_( PluginPath, Label, Identifier, ErrHandling ) )
		return false;

	SCLRack.Init();

	Data.Init( SCLRack, Arguments );

	C_().Initialize( &Data, Configuration );

	Plugin_ = C_().RetrievePlugin();

	if ( ( Plugin_ == NULL) && ( ErrHandling == err::hThrowException ) )
		qRFwk();
qRR
qRT
qRE
	return Plugin_ != NULL;
}

bso::bool__ plgn::rLooseRetriever::Initialize(
	const ntvstr::string___ &PluginPath,
	const char *Label,
	const char *Identifier,
	const str::string_ &Arguments,
	err::handling__ ErrHandling )
{
qRH
	plgncore::sData Data;
	sclmisc::sRack SCLRack;
	fnm::name___ Location;
qRB
	if ( !SubInitialize_( PluginPath, Label, Identifier, ErrHandling ) )
		return false;

	Location.Init();
	fnm::GetLocation( PluginPath, Location );

	SCLRack.Init();
	Data.Init( SCLRack, Arguments );

	C_().Initialize( &Data, Location );

	// Temporary workaround for the shared data from twice loaded plugin.
	/* When a plugin is loaded several time by the same excutable, its data
	is common to all library. Actually, the 'Configuration' and 'Locale'
	registry are the one from the first loaded library. The 'Argument'
	registry is the one from the last one. So, the 'Configuration' and
	'Locale' section should be the same for all the plugin, and you should
	read what be needed from the registry before returning from here. */

	Plugin_ = C_().RetrievePlugin();

	if ( ( Plugin_ == NULL) && ( ErrHandling == err::hThrowException ) )
		qRFwk();
qRR
qRT
qRE
	return Plugin_ != NULL;
}



// 'qR...' to be sure that the recalled and removed retriever is deleted if an error occurs.
void plgn::Delete_( dRetrievers &Retrievers )
{
qRH
	sRow Row = qNIL, BufferRow = qNIL;
	rLooseRetriever Retriever;
qRB
	sRow Row = Retrievers.Last();

	while ( Row != qNIL ) {
		Retrievers.Recall( Row, Retriever );

		Retriever.reset();

		BufferRow = Row;

		Row = Retrievers.Previous( Row );

		Retrievers.Remove( BufferRow );
	}
qRR
qRT
qRE
}



