/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define PLGN_COMPILATION_

#include "plgn.h"

using namespace plgn;

#include "sclm.h"
#include "scla.h"

namespace {
	class sDummyAbstract_
	: public sAbstract
	{
	protected:
		virtual const char *PLGNCOREIdentifier( void ) override
		{
			return NULL;
		}
	} DummyAbstract_;

}

sAbstract *plgn::NonNullAbstractPointer = &DummyAbstract_;
wAbstracts plgn::EmptyAbstracts;

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
		dlbrry::dynamic_library___ &Library )
	{
		plgncore::retrieve_callback *Function = NULL;

		if ( !Library.Init( PluginPath, err::hUserDefined ) )
			sclm::ReportAndAbort( PLGN_NAME "_UnableToLoadLibrary", PluginPath );

		Function = dlbrry::GetFunction<plgncore::retrieve_callback *>( E_STRING( PLGNCORE_RETRIEVE_CALLBACK_FUNCTION_NAME ), Library );

		if ( Function == NULL )
			sclm::ReportAndAbort( PLGN_NAME "_BadLibrary", PluginPath );

		return &Function();
	}
}

void plgn::rLooseRetriever::SubInitialize_(
	const ntvstr::string___ &PluginPath,
	const char *Label,
	const char *Identifier )
{
	if ( Plugin_ != NULL )
		qRFwk();

	Callback_ = ::SubInitialize_( PluginPath, Library_ );

	if ( Callback_ == NULL )	// An error should already be handled.
			qRFwk();

	if ( !_IsCompatible( Label, Identifier, Library_ ) )
		sclm::ReportAndAbort( PLGN_NAME "_LibraryNotCompatible", PluginPath );
}

namespace {
	sdr::sRow GetAbstract_(
		plgncore::callback__ &Callback,
		const dAbstracts &Abstracts,
		sAbstract *&Abstract )
	{
		sdr::sRow Row = Abstracts.First();
		const char *Identifier = Callback.PluginIdentifier();

		while ( ( Row != qNIL ) && ( strcmp( Abstracts( Row )->Identifier(), Identifier ) ) )
			Row = Abstracts.Next( Row );

		if ( Row != qNIL )
			Abstract = Abstracts( Row );

		return Row;
	}
}

sdr::sRow plgn::rLooseRetriever::Initialize(
	const ntvstr::string___ &PluginPath,
	const char *Label,
	const char *Identifier,
	const rgstry::entry__ &Configuration,
	const str::string_ &RawArguments,
	const dAbstracts &Abstracts )
{
	sdr::sRow Row = qNIL;
qRH
	plgncore::sData Data;
	sclm::sRack SCLRack;
	sAbstract *Abstract = NULL;
	str::wString NormalizedArguments;
qRB
	SubInitialize_( PluginPath, Label, Identifier );

	SCLRack.Init();

	NormalizedArguments.Init();
	scla::Normalize( RawArguments, NormalizedArguments );

	Data.Init( SCLRack, NormalizedArguments );

	C_().Initialize( &Data, Configuration );

	Row = GetAbstract_( C_(), Abstracts, Abstract );

	if ( ( Plugin_ = C_().RetrievePlugin( Abstract ) ) != NULL )
		Row = qNIL;
qRR
qRT
qRE
	return Row;
}

sdr::sRow plgn::rLooseRetriever::Initialize(
	const ntvstr::string___ &PluginPath,
	const char *Label,
	const char *Identifier,
	const str::string_ &RawArguments,
	const dAbstracts &Abstracts )
{
	sdr::sRow Row = qNIL;
qRH
	plgncore::sData Data;
	sclm::sRack SCLRack;
	fnm::name___ Location;
	sAbstract *Abstract = NULL;
	str::wString NormalizedArguments;
qRB
	SubInitialize_( PluginPath, Label, Identifier );

	Location.Init();
	fnm::GetLocation( PluginPath, Location );

	SCLRack.Init();

	NormalizedArguments.Init();
	scla::Normalize( RawArguments, NormalizedArguments );

	Data.Init( SCLRack, NormalizedArguments );

	C_().Initialize( &Data, Location );

	// Temporary workaround for the shared data from twice loaded plugin.
	/* When a plugin is loaded several time by the same excutable, its data
	is common to all library. Actually, the 'Configuration' and 'Locale'
	registry are the one from the first loaded library. The 'Argument'
	registry is the one from the last one. So, the 'Configuration' and
	'Locale' section should be the same for all the plugin, and you should
	read what be needed from the registry before returning from here. */

	Row = GetAbstract_( C_(), Abstracts, Abstract );

	if ( ( Plugin_ = C_().RetrievePlugin( Abstract ) ) != NULL )
		Row = qNIL;
qRR
qRT
qRE
	return Row;
}

// 'qR...' to be sure that the recalled and removed retriever is deleted if an error occurs.
void plgn::Delete_( dRetrievers &Retrievers )
{
qRH
	sRow Row = qNIL, BufferRow = qNIL;
	rLooseRetriever Retriever;
qRB
	Row = Retrievers.Last();

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

void plgn::IdentifierAndDetails(
	const ntvstr::string___ &PluginPath,
	str::dString &Identifier,
	str::dString &Details )
{
qRH
	dlbrry::dynamic_library___ Library;
	plgncore::callback__ *Callback = NULL;
qRB
	Callback = SubInitialize_( PluginPath, Library );

	if ( Callback == NULL )
		qRFwk();

	Identifier.Append( Callback->PluginIdentifier() );
	Details.Append( Callback->PluginDetails() );
qRR
qRT
qRE
}

qGCTOR( plgn )
{
	EmptyAbstracts.Init();
}

