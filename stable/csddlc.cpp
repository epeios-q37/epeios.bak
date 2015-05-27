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

#define CSDDLC__COMPILATION

#include "csddlc.h"

#include "cpe.h"

#define FUNCTION_NAME	"CSDDLEntry"

#if defined( CPE__T_LINUX ) || defined( CPE__T_BEOS ) || defined( CPE__T_CYGWIN ) || defined( CPE__T_MAC )
#	define CSDDLC__POSIX
#elif defined( CPE__T_MS )
#	define CSDDLC__MS
#else
#	error "Unknown target !"
#endif

#ifdef CSDDLC__MS
#	include <windows.h>
#elif defined ( CSDDLC__POSIX )
#	include <dlfcn.h>
#else
#	error
#endif



using namespace csddlc;

typedef csdscm::user_functions__ *(*f)( void *);

void csddlc::dynamic_library_client_core::reset( bso::bool__ P )
{
	if ( P ) {
		if ( _LibraryHandler != NULL )
#ifdef CSDDLC__MS
			if ( !FreeLibrary( (HMODULE)_LibraryHandler ) )
				ERRs();
#elif defined( CSDDLC__POSIX )
			if ( dlclose( _LibraryHandler ) == -1 )
				ERRs();
#else
#	error
#endif
	}

	_UserFunctions = NULL;
	_LibraryHandler = NULL;
}


bso::bool__ csddlc::dynamic_library_client_core::Init(
	const char *LibraryName,
	void *UP )
{
	bso::bool__ Success = false;
ERRProlog
	f CSDDLGet;
ERRBegin
	reset();

#ifdef CSDDLC__MS
	if ( ( _LibraryHandler = LoadLibrary( LibraryName ) ) == NULL )
		ERRu();

	if ( ( CSDDLGet = (f)GetProcAddress( (HMODULE)_LibraryHandler, FUNCTION_NAME ) ) == NULL )
		ERRs();
#elif defined( CSDDLC__POSIX )
	if ( ( _LibraryHandler = dlopen( LibraryName, RTLD_LAZY ) ) == NULL )
		ERRu();

	CSDDLGet = (f)dlsym( _LibraryHandler, FUNCTION_NAME );

	if ( dlerror() != NULL )
		ERRs();
#else
#	error
#endif

	Success = ( _UserFunctions = CSDDLGet( UP ) ) != NULL;
ERRErr
ERREnd
	if ( !Success )
		reset();
ERREpilog
	return Success;
}

