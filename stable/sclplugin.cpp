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
	E_CDEF( char, EscapeChar, '\\' );

	void HandleArguments_( const str::string_ &MergedArguments )
	{
	qRH
		str::strings Arguments;
		str::string Argument;
		sdr::row__ Row = qNIL;
		bso::bool__ Escape = false;
		bso::char__ C = 0;
	qRB
		Row = MergedArguments.First();

		Arguments.Init();
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
					Arguments.Append( Argument );

				Argument.Init();
			} else
				Argument.Append( C );

			Row = MergedArguments.Next( Row );
		}

		if ( Argument.Amount() != 0 )
			Arguments.Append( Argument );

		sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );
	qRR
	qRT
	qRE
	}
}

void sclplugin::callback__::PLGNCOREInitialize(
	const plgncore::data__ *Data,
	const rgstry::entry__ &Configuration )
{
	if ( Data == NULL )
		qRFwk();

	if ( strcmp( Data->Version, PLGNCORE_SHARED_DATA_VERSION ) )
		qRFwk();

	if ( Data->ControlValue != plgncore::data__::Control() )
		qRFwk();

	if ( !sclmisc::IsInitialized() )
		sclmisc::Initialize( Data->qRRor, Data->SCLError, *Data->CIO, Configuration );

	if ( Data->Arguments->Amount() != 0 )
		HandleArguments_( *Data->Arguments );

}

void sclplugin::callback__::PLGNCOREInitialize(
	const plgncore::data__ *Data,
	const fnm::name___ &Directory,
	str::string_ &Locale )
{
	if ( Data == NULL )
		qRFwk();

	if ( strcmp( Data->Version, PLGNCORE_SHARED_DATA_VERSION ) )
		qRFwk();

	if ( Data->ControlValue != plgncore::data__::Control() )
		qRFwk();

	if ( !sclmisc::IsInitialized() )
		sclmisc::Initialize( Data->qRRor, Data->SCLError, *Data->CIO, Directory, Locale );

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
