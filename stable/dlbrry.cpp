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

#define DLBRRY__COMPILATION

#include "dlbrry.h"

using namespace dlbrry;

#if defined( CPE_S_POSIX )
#	define TARGET_POSIX
#elif defined( CPE_S_WIN )
#	define TARGET_WIN
#else
#	error
#endif

#ifdef TARGET_WIN
# include <windows.h>
#elif defined ( TARGET_POSIX )
# include <dlfcn.h>
# include "cio.h"
# include "txf.h"
# include "fnm.h"
# include "str.h"
#else
# error
#endif

#ifdef TARGET_POSIX


# ifdef CPE_S_DARWIN
#  define PREFIX "lib"
#  define EXT ".dylib"
# else
#  define PREFIX "lib"
#  define EXT ".so"
# endif

static void BuildPath_(
	const str::string_ &Location,
	const str::string_ &Basename,
	fnm::name___ &Path )
{
qRH
	str::string NewBasename;
qRB
	NewBasename.Init();
	NewBasename.Init( PREFIX );
	NewBasename.Append( Basename );

	fnm::BuildPath( Location, NewBasename, EXT, Path );
qRR
qRT
qRE
}

static void BuildCompleteLibraryFileName_(
	const fnm::name___ &Path,
	fnm::name___ &NewPath )
{
qRH
	str::string Location, Basename, Extension;
	fnm::name___ Buffer;
qRB
	Buffer.Init();
	fnm::GetLocation( Path, Buffer );

	Location.Init();
	Buffer.UTF8( Location );

	Buffer.Init( fnm::GetBasename( Path ) );
	Basename.Init();
	Buffer.UTF8( Basename );

	Buffer.Init( fnm::GetExtension( Path ) );
	Extension.Init();
	Buffer.UTF8( Extension );

	if ( Extension == EXT )
		NewPath = Path;
	else
		BuildPath_( Location, Basename, NewPath );
qRR
qRT
qRE
}

static library_handler__ PosixLoadLibrary_( const ntvstr::string___ &Name )
{
	library_handler__ Handler = NULL;
qRH
	fnm::name___ Path;
qRB
	Path.Init();

	BuildCompleteLibraryFileName_( Name, Path );

	cio::COut << Path << txf::nl << txf::commit;

	Handler = dlopen( Path, RTLD_LAZY );
qRR
qRT
qRE
	return Handler;
}
# endif

bso::bool__ dlbrry::dynamic_library___::_LoadLibrary( const ntvstr::string___ &Name )
{
	if ( _LibraryHandler != NULL )
		qRFwk();

#ifdef TARGET_WIN
	if ( ( _LibraryHandler = LoadLibraryW( Name.Internal() ) ) == NULL )
		return false;
#elif defined( TARGET_POSIX )
	if ( ( _LibraryHandler = PosixLoadLibrary_( Name ) ) == NULL )
		return false;
#else
#	error
#endif
	return true;
}
bso::bool__ dlbrry::dynamic_library___::_UnloadLibrary( void  )
{
	if ( _LibraryHandler == NULL )
		qRFwk();

#ifdef TARGET_WIN
	if ( !FreeLibrary( (HMODULE)_LibraryHandler) )
		return false;
#elif defined( TARGET_POSIX )
	if ( dlclose( _LibraryHandler ) == -1 ) {
		const char *Error = dlerror();	// Facilite le dbogage.
		return false;
	}
#else
#	error
#endif
	_LibraryHandler = NULL;

	return true;
}

// 'MinGW' gnre une erreur lorsque '(void *)' est absent, mais pas 'MSVC4'. On suppose donc que la conversion en 'void *' ne pose pas de problme.
#ifdef TARGET_WIN
# ifdef CPE_C_MINGW
#  define FCAST	(void *)
# else
#  define FCAST
# endif
#endif

void * dlbrry::dynamic_library___::GetFunction( const char *FunctionName )
{
	void *Function = NULL;

	if ( !IsInitialized() )
		qRFwk();

#ifdef TARGET_WIN
	Function = FCAST GetProcAddress( (HMODULE)_LibraryHandler, FunctionName );
#elif defined( TARGET_POSIX )
	Function = dlsym( _LibraryHandler, FunctionName );

	if ( Function == NULL )
		const char *Error = dlerror();	// Facilite le dbogage.
#else
#	error
#endif

	return Function;
}
