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

#ifndef DIR__INC
#define DIR__INC

#define DIR_NAME		"DIR"

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
# ifdef CPE_XCODE
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
		sExists,		// Le rpertoire existe dj.
		sBadPath,		// Le chemin fournit n'est pas correct.
		sInadequatePath,	// Un lment du chemin n'existe pas ou n'est pas un rpertoire adapt (fichier, mauvais droits, ...).
		sNotEmpty,		// Dans le cas d'une effacement, ke rpertoire a effac n'estr pas vide.
		s_amount,
		s_Undefined
	};

	typedef tol::E_BUFFER___( bso::char__) buffer___;
#	define DIR_BUFFER___ dir::buffer___

	state__ HandleError( void );

	inline const fnm::name___ &GetSelfDir( fnm::name___ &Dir )
	{
#ifdef DIR__WIN 
		fnm::nchar__ FileName[MAX_PATH];
		DWORD Size = GetModuleFileNameW( NULL, FileName, sizeof( FileName ) );
		return fnm::GetLocation( FileName, Dir );
#endif
#ifdef DIR__POSIX
# ifdef CPE_XCODE
		char Filename[MAXPATHLEN];
		uint32_t Size = sizeof( Filename );
		switch ( _NSGetExecutablePath( Filename, &Size ) ) {
		case -1 :	// La taille de 'Path' est insuffisante.
			qRLmt();
			break;
		case 0:	// Succs.
			break;
		default:
			qRSys();
			break;
		}

		return fnm::GetLocation( fnm::name___( Filename ), Dir );
# else	// Ne fonctionne peur-tre pas sur tous les sytmes POSIX, mais du moins avec 'GNU/Linux' et 'Cygwin'.
		char Filename[PATH_MAX];
		int Size = readlink( "/proc/self/exe", Filename, sizeof( Filename ) );

		// Valeur d"erreur retourne par 'GetModuleFileName(..)'.
		// Valeur d'erreur retroune par 'readlink(...)', mais '0' est normalement une impossibilit.
		if ( Size <= 0 )
			qRSys();

		if ( Size == sizeof( Filename ) )
			qRLmt();

		Filename[Size] = 0;	//'readlink(...) ne rajoute pas le '\0' final.

		return fnm::GetLocation( fnm::name___( Filename ), Dir );
# endif
#endif
	}

	inline state__ CreateDir( const fnm::name___ &Dir )
	{
#ifdef DIR__WIN
		switch ( _wmkdir( Dir.Internal() ) ) {
#elif defined( DIR__POSIX )
		switch ( mkdir( Dir.Internal(), 0777 ) ) {
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
			qRSys();
			break;
		}

		return s_Undefined;	// Pour viter un 'warning'.
	}

	inline state__ DropDir( const fnm::name___ &Dir )
	{
#ifdef DIR__WIN
		switch ( _wrmdir( Dir.Internal() ) ) {
#elif defined( DIR__POSIX )
		switch ( rmdir( Dir.Internal() ) ) {
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
			qRSys();
			break;
		}

		return s_Undefined;	// Pour viter un 'warning'.
	}

	inline state__ ChangeDir( const fnm::name___ &Dir )
	{
#ifdef DIR__WIN
		switch ( _wchdir( Dir.Internal() ) ) {
#elif defined( DIR__POSIX )
		switch ( chdir( Dir.Internal() ) ) {
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
			qRSys();
			break;
		}

		return s_Undefined;	// Pour viter un 'warning'.
	}


# ifdef DIR__WIN
	struct handle___ {
		WIN32_FIND_DATAW File;
		HANDLE hSearch;
		fnm::name___ Path;
		void reset( bso::bool__ P = true )
		{
			hSearch = NULL;
			Path.reset( P );
		}
		E_CDTOR( handle___ );
		void Init( void )
		{
			hSearch = NULL;
			Path.Init();
		}
	};

#elif defined( DIR__POSIX )
	struct handle___ {
		DIR	*Dir;
		fnm::name___ Path;
		TOL_CBUFFER___ Buffer;
		void reset( bso::bool__ P = true )
		{
			Dir = NULL;
			Path.reset( P );
		}
		E_CDTOR( handle___ );
		void Init( void )
		{
			Dir = NULL;
			Path.Init();
		}
	};
#else
#	error
#endif
	// Si retourne chane vide, plus de fichier; si retourne NULL, erreur.
	inline const fnm::name___ &GetFirstFile(
		const fnm::name___ &Path,
		handle___ &Handle )
	{
# ifdef DIR__WIN
		WIN32_FIND_DATAW &File = Handle.File;
		HANDLE &hSearch = Handle.hSearch;
		fnm::ncore___ &Buffer = Handle.Path.ExposedInternal();

		Handle.Path.Init();

		Buffer.Malloc( wcslen( Path.Internal() ) + 1 );
		wcscpy( Buffer, Path.Internal() );

		if ( *Buffer ) {
			Buffer.Realloc( wcslen( Buffer ) + 2 );
			wcscat( Buffer, L"\\" );
		}

		Buffer.Realloc( wcslen( Buffer ) + 2 );
		wcscat( Buffer, L"*" );

	    hSearch = FindFirstFileW( Buffer, &File );

		if ( hSearch == INVALID_HANDLE_VALUE )
			if ( GetLastError() == ERROR_NO_MORE_FILES )
				Handle.Path.Init( "" );	// Pour mettre la taille  0 (ce qui signale l'absence de fichier, par opposition  'Handle.Name' == 'NULL', qui signale une erreur).
			else
				qRFwk();
		else {
			Buffer.Malloc( wcslen( File.cFileName ) + 1 );
			wcscpy( Buffer, File.cFileName );
		}
# elif defined( DIR__POSIX )
		struct dirent * ent;
		DIR *&rep = Handle.Dir;

		errno = 0;
    
		rep = opendir( Path.UTF8( Handle.Buffer ) );

		if( rep == NULL )
			qRFwk();

		if ( ( ent = readdir(rep) ) == NULL )
			if ( errno == 0 )
				Handle.Path.Init( "" );	// Pour mettre la taille  0 (ce qui signale l'absence de fichier, par opposition  'Handle.Name' == 'NULL', qui signale une erreur).
			else
				qRFwk();
		else
			Handle.Path.Init( ent->d_name );
# else
#  error
# endif
		return Handle.Path;
	}

	// Si retourne chane vide, plus de fichier; si retourne NULL, erreur.
	inline const fnm::name___ &GetNextFile( handle___ &Handle )
	{
# ifdef DIR__WIN
#  ifdef DIR_DBG
		if ( Handle.hSearch == INVALID_HANDLE_VALUE )
			qRFwk();
#  endif
		WIN32_FIND_DATAW &File = Handle.File;
		HANDLE &hSearch = Handle.hSearch;
		fnm::ncore___ &Buffer = Handle.Path.ExposedInternal();

		if ( !FindNextFileW( hSearch, &File ) )
			if ( GetLastError() == ERROR_NO_MORE_FILES )
				Handle.Path.Init( "" );	// Pour mettre la taille  0 (ce qui signale l'absence de fichier, par opposition  'Handle.Name' == 'NULL', qui signale une erreur).
			else
				qRFwk();
		else {
			Buffer.Malloc( wcslen( File.cFileName ) + 1 );
			wcscpy( Buffer, File.cFileName );
		}
# endif
# ifdef DIR__POSIX
		struct dirent * ent;
		DIR *&rep = Handle.Dir;
    
		errno = 0;
    
		if ( ( ent = readdir(rep) ) == NULL )
			if ( errno == 0 )
				Handle.Path.Init( "" );	// Pour mettre la taille  0 (ce qui signale l'absence de fichier, par opposition  'Handle.Name' == 'NULL', qui signale une erreur).
			else
				qRFwk();
		else
			Handle.Path.Init( ent->d_name );
    
# endif
		return Handle.Path;
	}

	inline void Close( handle___ &Handle )
	{
# ifdef DIR__WIN
#  ifdef DIR_DBG
		if ( Handle.hSearch == INVALID_HANDLE_VALUE )
			qRFwk();
#  endif
		if ( !FindClose( Handle.hSearch ) )
			qRLbr();
# endif
		
# ifdef DIR__POSIX
		DIR *&rep = Handle.Dir;
    
		if ( closedir(rep) )
			qRLbr();

		rep = NULL;
# endif
	}
}


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
