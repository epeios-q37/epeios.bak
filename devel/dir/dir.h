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

	typedef tol::E_FPOINTER___(bso::char__) buffer___;
#	define DIR_BUFFER___ dir::buffer___

	state__ HandleError( void );

	inline const char *GetSelfPath( DIR_BUFFER___ &Buffer )
	{
#ifdef DIR__WIN 
		char Path[MAX_PATH];
		DWORD Size = GetModuleFileNameA( NULL, Path, sizeof( Path ) );
#endif
#ifdef DIR__POSIX
# ifdef CPE__MAC
		char Path[MAXPATHLEN];
		uint32_t Size = sizeof( Path );
		switch ( _NSGetExecutablePath( Path, &Size ) ) {
		case -1 :	// La taille de 'Path' est insuffisante.
			ERRLmt();
			break;
		case 0:	// Succés.
			break;
		default:
			ERRSys();
			break;
		}

# else	// Ne fonctionne peur-être pas sur tous les sytèmes POSIX, mais du moins avec 'GNU/Linux' et 'Cygwin'.
		char Path[PATH_MAX];
		int Size = readlink( "/proc/self/exe", Path, sizeof( Path ) );

		// Valeur d"erreur retournée par 'GetModuleFileName(..)'.
		// Valeur d'erreur retrounée par 'readlink(...)', mais '0' est normalement une impossibilité.
		if ( Size <= 0 )
			ERRSys();

		if ( Size == sizeof( Path ) )
			ERRLmt();

		Path[Size] = 0;	//'readlink(...) ne rajoute pas le '\0' final.
# endif
#endif
		return fnm::GetLocation( Path, Buffer );
	}

	inline state__ CreateDir( const char *Path )
	{
#ifdef DIR__WIN
		switch ( _mkdir( Path ) ) {
#elif defined( DIR__POSIX )
		switch ( mkdir( Path, 0777 ) ) {
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

	inline state__ DropDir( const char *Path )
	{
#ifdef DIR__WIN
		switch ( _rmdir( Path ) ) {
#elif defined( DIR__POSIX )
		switch ( rmdir( Path ) ) {
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

	inline state__ ChangeDir( const char *Path )
	{
#ifdef DIR__WIN
		switch ( _chdir( Path ) ) {
#elif defined( DIR__POSIX )
		switch ( chdir( Path ) ) {
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

#ifdef DIR__WIN
	typedef struct _handle___ {
		WIN32_FIND_DATAA File;
		HANDLE hSearch;
	} *handle___;
#elif defined( DIR__POSIX )
	typedef DIR	*handle___;
#else
#	error
#endif

	// Si retourne chaîne vide, plus de fichier; si retourne NULL, erreur.
	inline const char *GetFirstFile(
		const char *Directory,
		handle___ &Handle )
	{
#ifdef DIR__WIN
		if ( ( Handle = new _handle___) ==  NULL )
			ERRAlc();

		WIN32_FIND_DATAA &File = Handle->File;
		HANDLE &hSearch = Handle->hSearch;

		char SearchString[MAX_PATH+1] = "";

		if ( ( strlen( Directory ) + 4 ) > MAX_PATH )
			ERRLmt();

		strcpy( SearchString, Directory );

		if ( *SearchString )
			strcat( SearchString, "\\" );

		strcat( SearchString, "*.*" );

	    hSearch = FindFirstFileA( SearchString, &File );

		if ( hSearch == INVALID_HANDLE_VALUE )
			if ( GetLastError() == ERROR_NO_MORE_FILES )
				return "";
			else
				return NULL;
		else
			return File.cFileName;
#elif defined( DIR__POSIX )
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
#endif
	}

	// Si retourne chaîne vide, plus de fichier; si retourne NULL, erreur.
	inline const char *GetNextFile( handle___ &Handle )
	{
		if ( Handle ==  NULL )
			ERRFwk();
# ifdef DIR__WIN
#  ifdef DIR_DBG
		if ( Handle->hSearch == INVALID_HANDLE_VALUE )
			ERRFwk();
#  endif
		WIN32_FIND_DATAA &File = Handle->File;
		HANDLE &hSearch = Handle->hSearch;

		if ( !FindNextFileA( hSearch, &File ) )
			if ( GetLastError() == ERROR_NO_MORE_FILES )
				return "";
			else
				return NULL;

		return File.cFileName;
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
		if ( Handle == NULL )
			ERRFwk();
# ifdef DIR__WIN
#  ifdef DIR_DBG
		if ( Handle->hSearch == INVALID_HANDLE_VALUE )
			ERRFwk();
#  endif
		if ( !FindClose( Handle->hSearch ) )
			ERRLbr();

		delete Handle;

		Handle = NULL;
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
