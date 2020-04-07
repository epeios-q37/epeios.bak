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

#define SCLP_COMPILATION_

#include "sclp.h"

#include "scla.h"
#include "sclm.h"

using namespace sclp;

#ifdef CPE_S_WIN
# define FUNCTION_SPEC __declspec(dllexport)
#else
#define FUNCTION_SPEC
# endif

namespace {
	qCBUFFERr Identifier_, Details_;
}

#define DEF( name, function ) extern "C" FUNCTION_SPEC function name

#ifdef CPE_C_CLANG
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

DEF( PLGNCORE_PLUGIN_LABEL_FUNCTION_NAME, plgncore::plugin_label );
DEF( PLGNCORE_PLUGIN_IDENTIFIER_FUNCTION_NAME, plgncore::plugin_identifier );
DEF( PLGNCORE_RETRIEVE_CALLBACK_FUNCTION_NAME, plgncore::retrieve_callback );

#ifdef CPE_C_CLANG
# pragma clang diagnostic pop
#endif

const char *PLGNCORE_PLUGIN_LABEL_FUNCTION_NAME( void )
{
	return sclp::SCLPPluginLabel();
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

void sclp::callback__::PLGNCOREInitialize(
	const plgncore::sData *Data,
	const rgstry::entry__ &Configuration )
{
qRH
qRB
	PreInitialize_( Data );

	if ( !sclm::IsInitialized() )
		sclm::Initialize( *Data->SCLRack, Configuration );
	else // Same plugin loaded several times from same executable ; data is common,
		// so the 'Arguments' registry from the previous loading is erased.
		sclr::Reset( sclr::lArguments );

	if ( Data->Arguments->Amount() != 0 )
		scla::FillRegistry( *Data->Arguments, scla::faIsArgument, scla::uaReport );

	sclm::DumpRegistriesAndOrLocalesIfRequired();
qRR
qRT
qRE
}

void sclp::callback__::PLGNCOREInitialize(
	const plgncore::sData *Data,
	const fnm::name___ &Directory )
{
qRH
qRB
	PreInitialize_( Data );

	if ( !sclm::IsInitialized() )
		sclm::Initialize( *Data->SCLRack, Directory, SCLPInfo() );
	else // Same plugin loaded several times from same executable ; data is common,
		// so the 'Arguments' registry from the previous loading is erased.
		sclr::Reset( sclr::lArguments );

	if ( Data->Arguments->Amount() != 0 )
		scla::FillRegistry( *Data->Arguments, scla::faIsArgument, scla::uaReport );

	sclm::DumpRegistriesAndOrLocalesIfRequired();
qRR
qRT
qRE
}

void *sclp::callback__::PLGNCORERetrievePlugin( plgncore::sAbstract *Abstract )
{
	return sclp::SCLPRetrievePlugin( Abstract );
}

bso::sBool sclp::callback__::PLGNCOREReleasePlugin( void *Plugin )
{
	bso::sBool Result = sclp::SCLPReleasePlugin( Plugin );

	sclm::Quit( SCLPInfo() );

	return Result;
}

const char *sclp::callback__::PLGNCOREPluginIdentifier( void )
{
	return Identifier_;
}

const char *sclp::callback__::PLGNCOREPluginDetails( void )
{
	return Details_;
}

qGCTOR( sclpugin )
{
qRH
	str::string Buffer;
qRB
	Buffer.Init();
	SCLPPluginIdentifier( Buffer );
	Buffer.Convert( Identifier_ );

	Buffer.Init();
	SCLPPluginDetails( Buffer );
	Buffer.Convert( Details_ );
qRR
qRT
qRE
}
