/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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
	qCBUFFERr Identifier_, Details_;
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
	void HandleArguments_(
		const str::string_ &MergedArguments,
		str::dStrings &SplittedArguments	)
	{
		sclargmnt::Split( MergedArguments, SplittedArguments );
	}

	void PreInitialize_( const plgncore::sData *Data )
	{
	if ( Data == NULL )
		qRFwk();

	if ( strcmp( Data->Version, PLGNCORE_SHARED_DATA_VERSION ) )
		qRFwk();

	if ( Data->ControlValue != plgncore::sData::Control() )
		qRFwk();
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

	PreInitialize_( Data );

	if ( !sclmisc::IsInitialized() )
		sclmisc::Initialize( *Data->SCLRack, Configuration );
	else // Same plugin loaded several times from same excutable ; data is common,
		// so the 'Arguments' registry from the previous loading is erased.
		sclrgstry::Reset( sclrgstry::lArguments );

	if ( Data->Arguments->Amount() != 0 )
		HandleArguments_( *Data->Arguments, Arguments );

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
	PreInitialize_( Data );

	if ( !sclmisc::IsInitialized() )
		sclmisc::Initialize( *Data->SCLRack, Directory );
	else // Same plugin loaded several times from same excutable ; data is common,
		// so the 'Arguments' registry from the previous loading is erased.
		sclrgstry::Reset( sclrgstry::lArguments );

	if ( Data->Arguments->Amount() != 0 )
		HandleArguments_( *Data->Arguments, Arguments );

	if ( Arguments.Amount() != 0 )
		sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

	sclmisc::DumpRegistriesAndOrLocalesIfRequired();
qRR
qRT
qRE
}

void *sclplugin::callback__::PLGNCORERetrievePlugin( plgncore::sAbstract *Abstract )
{
	return sclplugin::SCLPLUGINRetrievePlugin( Abstract );
}

bso::sBool sclplugin::callback__::PLGNCOREReleasePlugin( void *Plugin )
{
	bso::sBool Result = sclplugin::SCLPLUGINReleasePlugin( Plugin );

	sclmisc::Quit();

	return Result;
}

const char *sclplugin::callback__::PLGNCOREPluginIdentifier( void )
{
	return Identifier_;
}

const char *sclplugin::callback__::PLGNCOREPluginDetails( void )
{
	return Details_;
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
	SCLPLUGINPluginDetails( Buffer );
	Buffer.Convert( Details_ );
qRR
qRT
qRE
}