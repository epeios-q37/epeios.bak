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

// DIRectory

#ifndef DIR__INC
# define DIR__INC

# define DIR_NAME		"DIR"

# if defined( E_DEBUG ) && !defined( DIR_NODBG )
#  define DIR_DBG
# endif

# include "err.h"
# include "flw.h"
# include "cpe.h"
# include "tol.h"
# include "fnm.h"
# include "str.h"

# include <limits.h>

# if defined( CPE_S_POSIX )
#  define DIR__POSIX
# elif defined( CPE_S_WIN )
#  define DIR__WIN
# else
#  error
# endif


# ifdef DIR__WIN
#  include <direct.h>
#  include <windows.h>
#  define DIR_PATH_MAX_SIZE	MAX_PATH
# elif defined( DIR__POSIX )
#  ifdef CPE_S_DARWIN
#   include <mach-o/dyld.h>
#   include <sys/param.h>
#  endif
#	include <unistd.h>
#	include <sys/stat.h>
#	include <dirent.h>
#  define DIR_PATH_MAX_SIZE	PATH_MAX
# else
#  error
# endif

# include <errno.h>

/***************/
/***** OLD *****/
/***************/

namespace dir {
	enum state__ {
		sOK,
		sExists,		// Le r�pertoire existe d�j�.
		sBadPath,		// Le chemin fournit n'est pas correct.
		sInadequatePath,	// Un �lment du chemin n'existe pas ou n'est pas un r�pertoire adapt� (fichier, mauvais droits, ...).
		sNotEmpty,		// Dans le cas d'une effacement, ke r�pertoire a effac� n'estr pas vide.
		s_amount,
		s_Undefined
	};

	typedef tol::E_BUFFER___( bso::char__) buffer___;
#	define DIR_BUFFER___ dir::buffer___

	state__ HandleError( void );

# ifdef DIR__WIN 
#  define DIR__MAX_PATH	(MAX_PATH+1)
# elif defined( DIR__POSIX )
#  ifdef CPE_S_DARWIN
#   define DIR__MAX_PATH	(MAXPATHLEN+1)
#  else	// Ne fonctionne peur-�tre pas sur tous les syt�mes POSIX, mais du moins avec 'GNU/Linux' et 'Cygwin'.
#   define DIR__MAX_PATH	(PATH_MAX+1)
#  endif
# else
#  error
# endif

	typedef fnm::nchar__ _filename__[DIR__MAX_PATH] ;

	inline const ntvstr::char__ *GetSelfPath_( _filename__ &FileName )
	{
# ifdef DIR__WIN 
		DWORD Size = GetModuleFileNameW( NULL, FileName, DIR__MAX_PATH );

		if ( Size == DIR__MAX_PATH )
			qRLmt();
		else if ( Size == 0 )
			qRSys();
# elif defined( DIR__POSIX )
#  ifdef CPE_S_DARWIN
		uint32_t Size = DIR__MAX_PATH;
		switch ( _NSGetExecutablePath( FileName, &Size ) ) {
		case -1 :	// La taille de 'Path' est insuffisante.
			qRLmt();
			break;
		case 0:	// Succ�s.
			break;
		default:
			qRSys();
			break;
		}
#  else	// Ne fonctionne peur-�tre pas sur tous les syt�mes POSIX, mais du moins avec 'GNU/Linux' et 'Cygwin'.
		int Size = readlink( "/proc/self/exe", FileName, DIR__MAX_PATH );

		if ( Size <= 0 )
			qRSys();

		if ( Size == DIR__MAX_PATH )
			qRLmt();

		FileName[Size] = 0;	//'readlink(...) ne rajoute pas le '\0' final.
#  endif
# else
#  error
# endif
		return FileName;
	}

	inline const fnm::name___ &GetSelfPath( fnm::name___ &Path )
	{
		_filename__ FileName;

		Path.Init( GetSelfPath_( FileName ) );

		return Path;
	}


	inline const fnm::name___ &GetSelfDir( fnm::name___ &Dir )
	{
		_filename__ FileName;

		return fnm::GetLocation( GetSelfPath_( FileName ), Dir );
	}

# ifdef DIR__WIN
	const fnm::rName &WinGetCommonAppDataPath( fnm::rName &Path );

	const fnm::rName &WinGetUserAppDataPath( fnm::rName &Path );
# endif

	const fnm::rName &GetAppDataPath( fnm::rName &Path );

	inline state__ CreateDir(
		const fnm::name___ &Dir,
		qRPD )
	{
# ifdef DIR__WIN
		switch ( _wmkdir( Dir.Internal() ) ) {
# elif defined( DIR__POSIX )
		switch ( mkdir( Dir.Internal(), 0777 ) ) {
# else
#  error
# endif
		case 0:
			return sOK;
			break;
		case -1:
			if ( qRPT )
				qRFwk();
			else
				return HandleError();
			break;
		default:
			qRSys();
			break;
		}

		return s_Undefined;	// Pour �viter un 'warning'.
	}

	inline state__ DropDir( const fnm::name___ &Dir )
	{
# ifdef DIR__WIN
		switch ( _wrmdir( Dir.Internal() ) ) {
# elif defined( DIR__POSIX )
		switch ( rmdir( Dir.Internal() ) ) {
# else
#  error
# endif
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

		return s_Undefined;	// Pour �viter un 'warning'.
	}

	inline state__ ChangeDir( const fnm::name___ &Dir )
	{
# ifdef DIR__WIN
		switch ( _wchdir( Dir.Internal() ) ) {
# elif defined( DIR__POSIX )
		switch ( chdir( Dir.Internal() ) ) {
# else
#	error
# endif
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

		return s_Undefined;	// Pour �viter un 'warning'.
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

# elif defined( DIR__POSIX )
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
# else
#  error
# endif
	// Si retourne cha�ne vide, plus de fichier; si retourne NULL, erreur.
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
			switch ( GetLastError() ) {
			case ERROR_NO_MORE_FILES:
			case ERROR_ACCESS_DENIED:
			case ERROR_NOT_READY:	// Empty DVD drive, for example.
				Handle.Path.Init( "" );	// Pour mettre la taille � 0 (ce qui signale l'absence de fichier, par opposition � 'Handle.Path' == 'NULL', qui signale une erreur).
				break;
			default:
				qRFwk();
				break;
		} else {
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
				Handle.Path.Init( "" );	// Pour mettre la taille � 0 (ce qui signale l'absence de fichier, par opposition � 'Handle.Name' == 'NULL', qui signale une erreur).
			else
				qRFwk();
		else
			Handle.Path.Init( ent->d_name );
# else
#  error
# endif
		return Handle.Path;
	}

	// Si retourne cha�ne vide, plus de fichier; si retourne NULL, erreur.
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
				Handle.Path.Init( "" );	// Pour mettre la taille � 0 (ce qui signale l'absence de fichier, par opposition � 'Handle.Name' == 'NULL', qui signale une erreur).
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
				Handle.Path.Init( "" );	// Pour mettre la taille � 0 (ce qui signale l'absence de fichier, par opposition � 'Handle.Name' == 'NULL', qui signale une erreur).
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
		if ( Handle.hSearch != INVALID_HANDLE_VALUE )
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

	void FileSystem( str::dStrings &Paths );
}

/***************/
/***** NEW *****/
/***************/

namespace dir {
	class rFileBrowser {
	private:
		handle___ Handle_;
		fnm::rName CurrentFile_, NextFile_;
	public:
		void reset( bso::sBool P = true )
		{
			if ( P )
				Close( Handle_ );

			tol::reset( P, Handle_, CurrentFile_ );
		}
		qCDTOR( rFileBrowser );
		void Init( const fnm::rName &Path )
		{
			Handle_.Init();
			CurrentFile_.Init();
			NextFile_.Init( GetFirstFile( Path, Handle_ ) );
		}
		const fnm::rName &GetCurrentFile( void )
		{
			return CurrentFile_;
		}
		const fnm::rName &GetNextFile( void )
		{
			CurrentFile_ = NextFile_;

			NextFile_ = dir::GetNextFile( Handle_ );

			return CurrentFile_;
		}
	};

	typedef state__ eState;

	eState CreateDirWithParents(
		const fnm::rName &Name,
		qRPD );

	inline bso::sBool GetDiskFreeSpace(
		const fnm::rName &Name,
		bso::u64__ &Size,
		err::handling__ Handling = err::h_Default )
	{
# ifdef DIR__WIN
		ULARGE_INTEGER RawSize;

		if ( GetDiskFreeSpaceExW( Name.Internal(), &RawSize, NULL, NULL) == 0 ) {
			if ( Handling == err::hThrowException )
				qRFwk();
			else
				return false;
		}

		Size = RawSize.QuadPart;

		return true;
# else
		qRVct();
		return false;
# endif
	}

	inline bso::sBool GetDiskTotalSpace(
		const fnm::rName &Name,
		bso::u64__ &Size,
		err::handling__ Handling = err::h_Default )
	{
# ifdef DIR__WIN
		ULARGE_INTEGER RawSize;

		if ( GetDiskFreeSpaceExW( Name.Internal(), NULL, &RawSize, NULL ) == 0 ) {
			if ( Handling == err::hThrowException )
				qRFwk();
			else
				return false;
		}

		Size = RawSize.QuadPart;

		return true;
# else
		qRVct();
		return false;
# endif
	}

	qENUM( BusType ){
		btUSB,
		bt_amount,
		bt_Undefined
	};

	eBusType GetDiskBusType( const fnm::rName &Disk );
}



#endif
