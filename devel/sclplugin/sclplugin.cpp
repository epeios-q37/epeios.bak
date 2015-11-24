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

#define SCLPLUGIN__COMPILATION

#include "sclplugin.h"

#include "sclmisc.h"
#include "sclargmnt.h"

#include "strmrg.h"

using namespace sclplugin;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

#ifdef CPE_S_WIN
# define FUNCTION_SPEC __declspec(dllexport)
#else
#define FUNCTION_SPEC
# endif

#define DEF( name, function ) extern "C" FUNCTION_SPEC function name

DEF( PLGNCORE_PLUGIN_IDENTIFICATION_FUNCTION_NAME, plgncore::plugin_identification );
DEF( PLGNCORE_RETRIEVE_CALLBACK_FUNCTION_NAME, plgncore::retrieve_callback );

const char *PLGNCORE_PLUGIN_IDENTIFICATION_FUNCTION_NAME( void )
{
	return sclplugin::SCLPLUGINPluginIdentification();
}

static callback__ Callback_;

_callback__ &PLGNCORE_RETRIEVE_CALLBACK_FUNCTION_NAME( void )
{
	Callback_.Init();

	return Callback_;
}

namespace {
	void HandleArguments_( strmrg::retriever__ &Arguments )
	{
	qRH
		str::strings Strings;
		str::string String;
	qRB
		Strings.Init();
		while ( !Arguments.Availability() == strmrg::aNone ) {
			String.Init()	;
			Arguments.GetString( String );
			Strings.Append( String );
		}

		sclargmnt::FillRegistry( Strings, sclargmnt::faIsArgument, sclargmnt::uaReport );
	qRR
	qRT
	qRE
	}

	void HandleArguments_( const strmrg::table_ &Arguments )
	{
		strmrg::retriever__ Retriever;

		Retriever.Init( Arguments );

		HandleArguments_( Retriever );
	}

	void HandleArguments_( const str::string_ &Arguments )
	{
	qRH
		strmrg::table Splitted;
		
	qRB
		Splitted.Init();
		strmrg::Split( Arguments, Splitted );
	qRR
	qRT
	qRE
	}
}

void sclplugin::callback__::PLGNCOREInitialize( const plgncore::data__ *Data )
{
	if ( Data == NULL )
		qRFwk();

	if ( strcmp( Data->Version, PLGNCORE_SHARED_DATA_VERSION ) )
		qRFwk();

	if ( Data->ControlValue != plgncore::data__::Control() )
		qRFwk();

	if ( !sclmisc::IsInitialized() )
		if ( Data->Location->Amount() == 0 )
			sclmisc::Initialize( Data->qRRor, Data->SCLError, *Data->CIO, Data->Configuration, Data->Locale );
		else
			sclmisc::Initialize( Data->qRRor, Data->SCLError, *Data->CIO, *Data->Location );

	if ( Data->Arguments->Amount() != 0 )
		HandleArguments_( *Data->Arguments );

}

void *sclplugin::callback__::PLGNCORERetrievePlugin( void )
{
	return sclplugin::SCLPLUGINRetrievePlugin();
}

void sclplugin::callback__::PLGNCOREReleasePlugin( void *Plugin )
{
	sclplugin::SCLPLUGINReleasePlugin( Plugin );
}
