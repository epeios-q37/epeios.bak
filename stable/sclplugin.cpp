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

#define SCLPLUGIN__COMPILATION

#include "sclplugin.h"

#include "sclmisc.h"
#include "sclargmnt.h"

using namespace sclplugin;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

#ifdef CPE_S_WIN
# define FUNCTION_SPEC __declspec(dllexport)
#else
#define FUNCTION_SPEC
# endif

namespace {
	qCBUFFERr Identifier_, About_;
}

#define DEF( name, function ) extern "C" FUNCTION_SPEC function name

DEF( PLGNCORE_PLUGIN_LABEL_FUNCTION_NAME, plgncore::plugin_label );
DEF( PLGNCORE_PLUGIN_IDENTIFIER_FUNCTION_NAME, plgncore::plugin_identifier );
DEF( PLGNCORE_RETRIEVE_CALLBACK_FUNCTION_NAME, plgncore::retrieve_callback );

const char *PLGNCORE_PLUGIN_LABEL_FUNCTION_NAME( void )
{
	return sclplugin::SCLPLUGINPluginLabel();
}

const char *PLGNCORE_PLUGIN_IDENTIFIER_FUNCTION_NAME( void )
{
	return Identifier_;
}

static callback__ Callback_;

_callback__ &PLGNCORE_RETRIEVE_CALLBACK_FUNCTION_NAME( void )
{
	Callback_.Init();                             

	return Callback_;
}

namespace {
	E_CDEF( char, EscapeChar, '\\' );

	void HandleArguments_(
		const str::string_ &MergedArguments,
		str::dStrings &SplittedArguments	)
	{
	qRH
		str::string Argument;
		sdr::row__ Row = qNIL;
		bso::bool__ Escape = false;
		bso::char__ C = 0;
	qRB
		Row = MergedArguments.First();

		Argument.Init();

		while ( Row != qNIL ) {
			if ( ( C = MergedArguments(Row) ) == EscapeChar ) {
				if ( Escape ) {
					Escape = false;
					Argument.Append( EscapeChar );
				} else 
					Escape = true;
			} else if ( Escape ) {
				if ( C == ' ' )
					Argument.Append( ' ' );
				else
					sclmisc::ReportAndAbort(SCLPLUGIN_NAME "_BadArguments" );

				Escape = false;
			} else if ( C == ' ' ) {
				if ( Argument.Amount() != 0 )
					SplittedArguments.Append( Argument );

				Argument.Init();
			} else
				Argument.Append( C );

			Row = MergedArguments.Next( Row );
		}

		if ( Argument.Amount() != 0 )
			SplittedArguments.Append( Argument );

	qRR
	qRT
	qRE
	}

	void PreInitialize_(
		const plgncore::sData *Data,
		str::dStrings &Arguments )
	{
	if ( Data == NULL )
		qRFwk();

	if ( strcmp( Data->Version, PLGNCORE_SHARED_DATA_VERSION ) )
		qRFwk();

	if ( Data->ControlValue != plgncore::sData::Control() )
		qRFwk();

	if ( Data->Arguments->Amount() != 0 )
		HandleArguments_( *Data->Arguments, Arguments );
	}
}

void sclplugin::callback__::PLGNCOREInitialize(
	const plgncore::sData *Data,
	const rgstry::entry__ &Configuration )
{
qRH
	str::wStrings Arguments;
qRB
	Arguments.Init();

	PreInitialize_( Data, Arguments );

	if ( !sclmisc::IsInitialized() )
		sclmisc::Initialize( *Data->SCLRack, Configuration );
	else // Same plugin loaded several times from same excutable ; data is common,
		// so the 'Arguments' registry from the previous loading is erased.
		sclrgstry::EraseArgumentsRegistry();

	if ( Arguments.Amount() != 0 )
		sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

	sclmisc::DumpRegistriesAndOrLocalesIfRequired();
qRR
qRT
qRE
}

void sclplugin::callback__::PLGNCOREInitialize(
	const plgncore::sData *Data,
	const fnm::name___ &Directory )
{
qRH
	str::wStrings Arguments;
qRB
	Arguments.Init();

	PreInitialize_( Data, Arguments );

	if ( !sclmisc::IsInitialized() )
		sclmisc::Initialize( *Data->SCLRack, Directory );
	else // Same plugin loaded several times from same excutable ; data is common,
		// so the 'Arguments' registry from the previous loading is erased.
		sclrgstry::EraseArgumentsRegistry();

	if ( Arguments.Amount() != 0 )
		sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

	sclmisc::DumpRegistriesAndOrLocalesIfRequired();
qRR
qRT
qRE
}

void *sclplugin::callback__::PLGNCORERetrievePlugin( void )
{
	return sclplugin::SCLPLUGINRetrievePlugin();
}

void sclplugin::callback__::PLGNCOREReleasePlugin( void *Plugin )
{
	sclplugin::SCLPLUGINReleasePlugin( Plugin );
}

const char *sclplugin::callback__::PLGNCOREPluginIdentifier( void )
{
	return Identifier_;
}

const char *sclplugin::callback__::PLGNCOREPluginDetails( void )
{
	return About_;
}

qGCTOR( sclpugin )
{
qRH
	str::string Buffer;
qRB
	Buffer.Init();
	SCLPLUGINPluginIdentifier( Buffer );
	Buffer.Convert( Identifier_ );

	Buffer.Init();
	SCLPLUGINAboutPlugin( Buffer );
	Buffer.Convert( About_ );
qRR
qRT
qRE
}