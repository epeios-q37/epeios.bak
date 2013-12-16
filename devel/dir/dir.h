/*
	'dir.h' by Claude SIMON (http://zeusw.org/).

	'dir' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DIR__INC
#define DIR__INC

#define DIR_NAME		"DIR"

#include "ttr.h"

extern class ttr_tutor &DIRTutor;

#if defined( E_DEBUG ) && !defined( DIR_NODBG )
#define DIR_DBG
#endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// DIRectory

#include "err.h"
#include "flw.h"
#include "cpe.h"
#include "tol.h"
#include "fnm.h"

#include <limits.h>

#if defined( CPE_POSIX )
#	define DIR__POSIX
#elif defined( CPE_WIN )
#	define DIR__WIN
#else
#	error "Unknown target !"
#endif


#ifdef DIR__WIN
#	include <direct.h>
#	include <windows.h>
# define DIR_PATH_MAX_SIZE	MAX_PATH
#elif defined( DIR__POSIX )
# ifdef CPE__MAC
#  include <mach-o/dyld.h>
#  include <sys/param.h>
# endif
#	include <unistd.h>
#	include <sys/stat.h>
#	include <dirent.h>
# define DIR_PATH_MAX_SIZE	PATH_MAX
#else
#	error
#endif

#include <errno.h>

namespace dir {
	enum state__ {
		sOK,
		sExists,		// Le répertoire existe déjà.
		sBadPath,		// Le chemin fournit n'est pas correct.
		sInadequatePath,	// Un élment du chemin n'existe pas ou n'est pas un répertoire adapté (fichier, mauvais droits, ...).
		s_amount,
		s_Undefined
	};

	typedef tol::E_BUFFER___( bso::char__) buffer___;
#	define DIR_BUFFER___ dir::buffer___

	state__ HandleError( void );

	inline const fnm::name___ &GetSelfPath( fnm::name___ &Path )
	{
#ifdef DIR__WIN 
		fnm::base__ FileName[MAX_PATH];
		DWORD Size = GetModuleFileNameW( NULL, FileName, sizeof( FileName ) );
		return fnm::GetLocation( FileName, Path );
#endif
#ifdef DIR__POSIX
# ifdef CPE__MAC
		char FileName[MAXPATHLEN];
		uint32_t Size = sizeof( FileName );
		switch ( _NSGetExecutablePath( FileName, &Size ) ) {
		case -1 :	// La taille de 'Path' est insuffisante.
			ERRLmt();
			break;
		case 0:	// Succés.
			break;
		default:
			ERRSys();
			break;
		}

		return fnm::GetLocation( fnm::name___( FileName ), Path );
# else	// Ne fonctionne peur-être pas sur tous les sytèmes POSIX, mais du moins avec 'GNU/Linux' et 'Cygwin'.
		char FileName[PATH_MAX];
		int Size = readlink( "/proc/self/exe", FileName, sizeof( FileName ) );

		// Valeur d"erreur retournée par 'GetModuleFileName(..)'.
		// Valeur d'erreur retrounée par 'readlink(...)', mais '0' est normalement une impossibilité.
		if ( Size <= 0 )
			ERRSys();

		if ( Size == sizeof( FileName ) )
			ERRLmt();

		FileName[Size] = 0;	//'readlink(...) ne rajoute pas le '\0' final.

		return fnm::GetLocation( fnm::name___( FileName ), Path );
# endif
#endif
	}

	inline state__ CreateDir( const fnm::name___ &Path )
	{
#ifdef DIR__WIN
		switch ( _wmkdir( Path.Core() ) ) {
#elif defined( DIR__POSIX )
		switch ( mkdir( Path.Core(), 0777 ) ) {
#else
#	error
#endif
		case 0:
			return sOK;
			break;
		case -1:
			return HandleError();
			break;
		default:
			ERRSys();
			break;
		}

		return s_Undefined;	// Pour éviter un 'warning'.
	}

	inline state__ DropDir( const fnm::name___ &Path )
	{
#ifdef DIR__WIN
		switch ( _wrmdir( Path.Core() ) ) {
#elif defined( DIR__POSIX )
		switch ( rmdir( Path.Core() ) ) {
#else
#	error
#endif
		case 0:
			return sOK;
			break;
		case -1:
			return HandleError();
			break;
		default:
			ERRSys();
			break;
		}

		return s_Undefined;	// Pour éviter un 'warning'.
	}

	inline state__ ChangeDir( const fnm::name___ &Path )
	{
#ifdef DIR__WIN
		switch ( _wchdir( Path.Core() ) ) {
#elif defined( DIR__POSIX )
		switch ( chdir( Path.Core() ) ) {
#else
#	error
#endif
		case 0:
			return sOK;
			break;
		case -1:
			return HandleError();
			break;
		default:
			ERRSys();
			break;
		}

		return s_Undefined;	// Pour éviter un 'warning'.
	}


# ifdef DIR__WIN
#  define DIR__WBUFFER___	tol::E_BUFFER___( wchar_t )
#  define DIR__BUFFER___	tol::E_BUFFER___( bso::char__ )
	struct handle___ {
		WIN32_FIND_DATAW File;
		HANDLE hSearch;
		fnm::name___ Name;
	};

#elif defined( DIR__POSIX )
	typedef DIR	*handle___;
#else
#	error
#endif
	// Si retourne chaîne vide, plus de fichier; si retourne NULL, erreur.
	inline const fnm::name___ &GetFirstFile(
		const fnm::name___ &Path,
		handle___ &Handle )
	{
# ifdef DIR__WIN
		WIN32_FIND_DATAW &File = Handle.File;
		HANDLE &hSearch = Handle.hSearch;
		fnm::core___ &Buffer = Handle.Name.Core();

		Handle.Name.Init();

		Buffer.Malloc( wcslen( Path.Core() ) + 1 );
		wcscpy( Buffer, Path.Core() );

		if ( *Buffer ) {
			Buffer.Realloc( wcslen( Buffer ) + 2 );
			wcscat( Buffer, L"\\" );
		}

		Buffer.Realloc( wcslen( Buffer ) + 2 );
		wcscat( Buffer, L"*" );

	    hSearch = FindFirstFileW( Buffer, &File );

		if ( hSearch == INVALID_HANDLE_VALUE )
			if ( GetLastError() == ERROR_NO_MORE_FILES )
				Handle.Name.Init();
			else
				ERRFwk();
		else {
			Buffer.Malloc( wcslen( File.cFileName ) + 1 );
			wcscpy( Buffer, File.cFileName );
		}

		return Handle.Name;
# elif defined( DIR__POSIX )
	struct dirent * ent;
    DIR *&rep = Handle;
    
    rep = opendir( Directory );

	if( rep == NULL )
		return NULL;

	errno = 0;
    
    if ( ( ent = readdir(rep) ) == NULL )
		if ( errno != 0 )
			return NULL;
		else
			return "";
	else
		return ent->d_name;
    
    return 0;
# else
#  error
# endif
	}

	// Si retourne chaîne vide, plus de fichier; si retourne NULL, erreur.
	inline const fnm::name___ &GetNextFile( handle___ &Handle )
	{
# ifdef DIR__WIN
#  ifdef DIR_DBG
		if ( Handle.hSearch == INVALID_HANDLE_VALUE )
			ERRFwk();
#  endif
		WIN32_FIND_DATAW &File = Handle.File;
		HANDLE &hSearch = Handle.hSearch;
		DIR__WBUFFER___ &Buffer = Handle.Name.Core();

		if ( !FindNextFileW( hSearch, &File ) )
			if ( GetLastError() == ERROR_NO_MORE_FILES )
				Handle.Name.Init();
			else
				ERRFwk();
		else {
			Buffer.Malloc( wcslen( File.cFileName ) + 1 );
			wcscpy( Buffer, File.cFileName );
		}

		return Handle.Name;
# endif
# ifdef DIR__POSIX
	struct dirent * ent;
    DIR *&rep = Handle;
    
	errno = 0;
    
    if ( ( ent = readdir(rep) ) == NULL )
		if ( errno != 0 )
			return NULL;
		else
			return "";
	else
		return ent->d_name;
    
    return 0;
# endif
	}

	inline void Close( handle___ &Handle )
	{
# ifdef DIR__WIN
#  ifdef DIR_DBG
		if ( Handle.hSearch == INVALID_HANDLE_VALUE )
			ERRFwk();
#  endif
		if ( !FindClose( Handle.hSearch ) )
			ERRLbr();
# endif
		
# ifdef DIR__POSIX
    DIR *&rep = Handle;
    
    if ( closedir(rep) )
		ERRLbr();

	Handle = NULL;
# endif
	}
}


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
