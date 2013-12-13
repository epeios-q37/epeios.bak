/*
	Header for the 'fil' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000-2001, 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

//	$Id: fil.h,v 1.59 2013/07/18 19:46:24 csimon Exp $

#ifndef FIL__INC
#define FIL__INC

#define FIL_NAME		"FIL"

#define	FIL_VERSION	"$Revision: 1.59 $"

#define FIL_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &FILTutor;

#if defined( E_DEBUG ) && !defined( FIL_NODBG )
#define FIL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.59 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/07/18 19:46:24 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D FILe 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

//D FILe. File management.

#include <sys/stat.h>
#include <errno.h>

#include "iop.h"
#include "flw.h"
#include "txf.h"
#include "tol.h"
#include "cpe.h"

// Prédéclaration, car l'incusion des fichiers d'entête correspondants posent des problèmes dû à l'inclusion circulaire.

namespace str {
	class string_;
}

namespace lcl {
	class meaning_;
}

# if defined( CPE_POSIX )
#  define FIL__POSIX
# elif defined( CPE_WIN )
#  define FIL__WIN
# else
#  error "Undefined platform !"
# endif

# ifdef FIL__WIN
#    define FIL__STATS	_stat
#    define FIL__STATF	_stat
#    define FIL__FSTAT	_fstat
# endif

# ifdef FIL__POSIX
#  include <utime.h>
#  define FIL__STATS	stat
#  define FIL__STATF	stat
#  define FIL__FSTAT	fstat
# endif



#ifdef FIL_BACKUP_FILE_EXTENSION
#	define FIL__BACKUP_FILE_EXTENSION	FIL_BACKUP_FILE_EXTENSION
#else
#	define FIL__BACKUP_FILE_EXTENSION	".bak"
#endif

#define FIL_UNDEFINED_DESCRIPTOR	IOP_UNDEFINED_DESCRIPTOR

#ifdef FIL_FLOW_BUFFER_SIZE
#	define FIL__FLOW_BUFFER_SIZE FIL_FLOW_BUFFER_SIZE
#else
#	define FIL__FLOW_BUFFER_SIZE	100
#endif

namespace fil {
	using namespace iop;

	enum mode__ {
		//i Ecrase le contenu du fichier.
		mRemove,
		//i Ajoute à la fin du fichier.
		mAppend,
		//i Ouvre le fichier ne lecture/écriture.
		mReadWrite,
		//i Ouvre le fichier en lecture seule.
		mReadOnly,
		//i Amount of mode.
		m_amount,
		m_Undefined,
	};
 	//e Status.
	enum status__ {
		//i Failure.
		sFailure = 0,
		//i Success.
		sSuccess,
		//i Amount of status.
		s_amount,
		//i Unknow,
		s_Undefined,
	};

	iop::descriptor__ Open(
		const char *Nom,
		mode__ Mode );

	void Close( iop::descriptor__ D );

	//f Return true if the file 'Name' exists, false otherwise.
	inline bool FileExists( const char *FileName )
	{
		struct FIL__STATS Stat;

		if ( FIL__STATF( FileName, &Stat ) == 0 )
			return true;

		switch ( errno ) {
		case EBADF:
			ERRSys();	// Normalement, cette erreur ne peut arriver, compte tenu de la fonction utilisée.
			break;
		case ENOENT:
			break;
		case ENOTDIR:
			break;
#if defined( FIL__POSIX )
		case ELOOP:
			break;
#endif
		case EFAULT:
			ERRLbr();
			break;
		case EACCES:
			break;
		case ENOMEM:
			ERRSys();
			break;
		case ENAMETOOLONG:
			ERRSys();
			break;
		default:
			ERRSys();
			break;
		}

		return false;
	}

	inline time_t GetFileLastModificationTime( const char *FileName )
	{
		struct FIL__STATS Stat;

		if ( FIL__STATF( FileName, &Stat ) != 0 )
			ERRLbr();

		return Stat.st_mtime;
	}

	inline bso::size__ GetFileSize( iop::descriptor__ Descriptor )
	{
		struct FIL__STATS Stat;

		if ( FIL__FSTAT( Descriptor, &Stat ) != 0 )
			ERRLbr();

		return Stat.st_size;
	}

	inline bso::size__ GetFileSize( const char *FileName )
	{
		struct FIL__STATS Stat;

		if ( FIL__STATF( FileName, &Stat ) != 0 )
			ERRLbr();

		return Stat.st_size;
	}

	inline bso::size__ IsDirectory( const char *FileName )
	{
		struct FIL__STATS Stat;

		if ( FIL__STATF( FileName, &Stat ) != 0 )
			ERRLbr();

		return Stat.st_mode & S_IFDIR;
	}

	inline bso::size__ IsFile( const char *FileName )
	{
		struct FIL__STATS Stat;

		if ( FIL__STATF( FileName, &Stat ) != 0 )
			ERRLbr();

		return Stat.st_mode & S_IFREG;
	}

	// Modifie la date de modification d'un fichier à la date courante.
	inline void TouchFile( const char *FileName )
	{
# ifdef FIL__WIN
		FILETIME ft;
		SYSTEMTIME st;
		HANDLE Handle = CreateFileA( FileName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

		if ( Handle ==  INVALID_HANDLE_VALUE )
			ERRLbr();
		
		GetSystemTime( &st );

		if ( !SystemTimeToFileTime( &st, &ft ) )
			ERRSys();

		if ( !SetFileTime( Handle, (LPFILETIME) NULL, (LPFILETIME) NULL, &ft ) )
			ERRLbr();
# else
		/*
		NOTA : Le code ci-dessous fonctionne AUSSI sous Windows, mais SEULEMENT lorsque lancé à partir d'une console DOS,
		Lorsque lancé à partir d'une console 'Cygwin', il y a un décalage d'une heure 'dépendant de l'heure d'hiver/été ?).
		*/

		if ( utime( FileName, NULL ) != 0 )
			ERRLbr();
# endif
	}

	inline void RemoveFile( const char *FileName )
	{
		remove( FileName );
	}

#ifdef CPE__VC
#	undef CreateFile
#endif

	bso::bool__ CreateFile(
		const char *FileName,
		err::handling__ ErrorHandling = err::h_Default );	// Crée un fichier de nom 'FileName'.

	enum backup_status__
	{
		bsOK,
		bsUnableToRename,
		bsUnableToDuplicate,
		bsUnableToSuppress,
		bs_amount,
		bs_Undefined
	};

	const char *GetLabel( backup_status__ Status );

	void GetMeaning(
		backup_status__ Status,
		const char *FileName,
		lcl::meaning_ &Meaning );

	//e How handle the backuped file.
	enum backup_mode__
	{
		bmRename,
		bmDuplicate,
		bm_amount,
		bm_Undefined,
		bm_Default = bmRename,
	};

# define FIL_BUFFER___	tol::E_BUFFER___( char )

	const char* GetBackupFileName(
		const char *FileName,
		FIL_BUFFER___ &Buffer );

	backup_status__ CreateBackupFile(
		const char *FileName,
		backup_mode__ Mode,
		err::handling__ = err::h_Default  );

	enum recover_status__
	{
		rsOK,
		rsUnableToRename,
		rsUnableToSuppress,
		rs_amount,
		rs_Undefined
	};

	const char *GetLabel( recover_status__ Status );

	void GetMeaning(
		recover_status__ Status,
		const char *FileName,
		lcl::meaning_ &Meaning );

	//f Recover the backup file 'Name' with 'Extension' as extension.
	recover_status__ RecoverBackupFile(
		const char *FileName,
		err::handling__ = err::h_Default  );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
