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

namespace {
	template <typename function> inline const char *GetAndExecute_(
		const char *FunctionName,
		dlbrry::dynamic_library___ &Library )
	{
		function *Function = dlbrry::GetFunction<function *>( FunctionName, Library );

		if ( Function == NULL )
			qRFwk();

		return Function();
	}

	bso::bool__ _IsCompatible(
		const char *Label,
		const char *Identifier,	// When == 'NULL', not used for comparison.
		dlbrry::dynamic_library___ &Library )
	{
		if ( !strcmp( Label, GetAndExecute_<plgncore::plugin_label>( E_STRING( PLGNCORE_PLUGIN_LABEL_FUNCTION_NAME ), Library ) ) ) {
			if ( Identifier != NULL )
				return !strcmp( Identifier, GetAndExecute_<plgncore::plugin_identifier>( E_STRING( PLGNCORE_PLUGIN_IDENTIFIER_FUNCTION_NAME ), Library ) );
			else
				return true;
		} else
			return false;
	}

	plgncore::callback__ *SubInitialize_(
		const ntvstr::string___ &PluginPath,
		dlbrry::dynamic_library___ &Library,
		err::handling__ ErrHandling )
	{
		plgncore::retrieve_callback *Function = NULL;

		if ( !Library.Init( PluginPath, ErrHandling ) ) {
			if ( ErrHandling == err::hThrowException )
				qRFwk();
			else
				return NULL;
		}

		Function = dlbrry::GetFunction<plgncore::retrieve_callback *>( E_STRING( PLGNCORE_RETRIEVE_CALLBACK_FUNCTION_NAME ), Library );

		if ( Function == NULL ) {
			if ( ErrHandling == err::hThrowException )
				qRFwk();
			else
				return false;
		}

		return &Function();
	}
}

bso::bool__ plgn::rLooseRetriever::SubInitialize_(
	const ntvstr::string___ &PluginPath,
	const char *Label,
	const char *Identifier,
	err::handling__ ErrHandling )
{
	if ( Plugin_ != NULL )
		qRFwk();

	Callback_ = ::SubInitialize_( PluginPath, Library_, ErrHandling );

	if ( Callback_ == NULL ) {
		if ( ErrHandling == err::hThrowException )
			qRFwk();
		else
			return false;
	}

	if ( !_IsCompatible( Label, Identifier, Library_ ) ) {
		if ( ErrHandling == err::hThrowException )
			qRFwk();
		else
			return false;
	}


	return true;
}

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
		qRReturn;

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
		qRReturn;

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

bso::sBool plgn::IdentifierAndDetails(
	const ntvstr::string___ &PluginPath,
	str::dString &Identifier,
	str::dString &Details,
	err::handling__ ErrHandling )
{
	bso::sBool Success = false;
qRH
	dlbrry::dynamic_library___ Library;
	plgncore::callback__ *Callback = NULL;
qRB
	Callback = SubInitialize_( PluginPath, Library, ErrHandling );

	if ( Callback == NULL ) {
		if ( ErrHandling == err::hThrowException )
			qRFwk();
		else
			qRReturn;
	}

	Identifier.Append( Callback->PluginIdentifier() );
	Details.Append( Callback->PluginDetails() );

	Success = true;
qRR
qRT
qRE
	return Success;
}


