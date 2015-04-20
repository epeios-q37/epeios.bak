/*
	'dlbrry' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'dlbrry' header file ('dlbrry.h').
	Copyright (C) 20112004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id: dlbrry.cpp,v 1.8 2013/04/09 08:03:08 csimon Exp $

#define DLBRRY__COMPILATION

#include "dlbrry.h"

class dlbrrytutor
: public ttr_tutor
{
public:
	dlbrrytutor( void )
	: ttr_tutor( DLBRRY_NAME )
	{
#ifdef DLBRRY_DBG
		Version = DLBRRY_VERSION "\b\bD $";
#else
		Version = DLBRRY_VERSION;
#endif
		Owner = DLBRRY_OWNER;
		Date = "$Date: 2013/04/09 08:03:08 $";
	}
	virtual ~dlbrrytutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace dlbrry;

#if defined( CPE_POSIX )
#	define TARGET_POSIX
#elif defined( CPE_WIN )
#	define TARGET_WIN
#else
#	error "Unknown target !"
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


# ifdef CPE_XCODE
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
ERRProlog
	str::string NewBasename;
ERRBegin
	NewBasename.Init();
	NewBasename.Init( PREFIX );
	NewBasename.Append( Basename );

	fnm::BuildPath( Location, NewBasename, EXT, Path );
ERRErr
ERREnd
ERREpilog
}

static void BuildCompleteLibraryFileName_(
	const fnm::name___ &Path,
	fnm::name___ &NewPath )
{
ERRProlog
	str::string Location, Basename, Extension;
	fnm::name___ Buffer;
ERRBegin
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
ERRErr
ERREnd
ERREpilog
}

static library_handler__ PosixLoadLibrary_( const ntvstr::string___ &Name )
{
	library_handler__ Handler = NULL;
ERRProlog
	fnm::name___ Path;
ERRBegin
	Path.Init();

	BuildCompleteLibraryFileName_( Name, Path );

	Handler = dlopen( Path, RTLD_LAZY );
ERRErr
ERREnd
ERREpilog
	return Handler;
}
# endif

bso::bool__ dlbrry::dynamic_library___::_LoadLibrary( const ntvstr::string___ &Name )
{
	if ( _LibraryHandler != NULL )
		ERRFwk();

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
		ERRFwk();

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
#ifdef CPE_WIN
# ifdef CPE_MINGW
#  define FCAST	(void *)
# else
#  define FCAST
# endif
#endif

void * dlbrry::dynamic_library___::GetFunction( const char *FunctionName )
{
	void *Function = NULL;

	if ( !IsInitialized() )
		ERRFwk();

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




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dlbrrypersonnalization
: public dlbrrytutor
{
public:
	dlbrrypersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dlbrrypersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static dlbrrypersonnalization Tutor;

ttr_tutor &DLBRRYTutor = Tutor;
