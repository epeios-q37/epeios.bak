/*
	'fil.h' by Claude SIMON (http://zeusw.org/).

	'fil' is part of the Epeios framework.

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

#ifndef FIL__INC
# define FIL__INC

# define FIL_NAME		"FIL"

# if defined( E_DEBUG ) && !defined( FIL_NODBG )
#  define FIL_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// FILe

# include <sys/stat.h>
# include <errno.h>

# include "iop.h"
# include "flw.h"
# include "txf.h"
# include "tol.h"
# include "cpe.h"
# include "fnm.h"

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
#    define FIL__STATS	_stat64
#    define FIL__STATF	_wstat64
#    define FIL__FSTAT	_fstat64
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

	iop::descriptor__ Open(
		const fnm::name___ &Name,
		mode__ Mode );

	void Close( iop::descriptor__ D );

	enum error__ {
		// No error,
		eNone,
		eBadF,
		eNoEnt,
		eNotDir,
		eLoop,
		eFault,
		eAcces,
		eNoMem,
		eNameTooLong,
		eUnknown,
		e_amount,
		e_Undefined
	};

	inline error__ _ConvertErrNo( int ErrNo )
	{
		switch ( ErrNo ) {
		case EBADF:
			return eBadF;
			break;
		case ENOENT:
			return eNoEnt;
			break;
		case ENOTDIR:
			return eNotDir;
			break;
		case ELOOP:
			return eLoop;
			break;
		case EFAULT:
			return eFault;
			break;
		case EACCES:
			return eAcces;
			break;
		case ENOMEM:
			return eNoMem;
			break;
		case ENAMETOOLONG:
			return eNameTooLong;
			break;
		default:
			return eUnknown;
			break;
		}

		return e_Undefined;
	}

	enum type__ {
		tFile,
		tDir,
		tUnknown,
		t_amount,
		t_Undefined
	};

	inline type__ _ConvertMode( int Mode )
	{
		if ( Mode &  S_IFREG )
			return tFile;
		else if ( Mode & S_IFDIR )
			return tDir;
		else
			return tUnknown;
	}

	struct info__ {
		type__ Type;
		size__ Size;
		struct time__ {
			time_t
				Creation,
				Modification,
				Access;
			void reset( bso::bool__ = true )
			{
				Creation, Modification, Access = 0;
			}
			E_CDTOR( time__ );
			void Init( void )
			{
				Creation, Modification, Access = 0;
			}
		} Time;
		void reset( bso::bool__ P = true )
		{
			Time.reset( P );

			Type = t_Undefined;
			Size = 0;
		}
		E_CDTOR( info__ );
		void Init( void )
		{
			Time.Init();

			Type = t_Undefined;
			Size = 0;
		}
	};

	inline error__ _Stat(
		const fnm::name___ &FileName,
		struct FIL__STATS &Stat )
	{
		errno = 0;

		if ( FIL__STATF( FileName.Core(), &Stat ) != 0 )
			return _ConvertErrNo( errno );

		return eNone;
	}

	inline error__ _Stat(
		iop::descriptor__ Descriptor,
		struct FIL__STATS &Stat )
	{
		errno = 0;

		if ( FIL__FSTAT( Descriptor, &Stat ) != 0 )
			return _ConvertErrNo( errno );

		return eNone;
	}

	template <typename t> inline error__ GetInfo(
		const t &FileSpecification,
		info__ &Info )
	{
		struct FIL__STATS Stat;

		error__ Error = _Stat( FileSpecification, Stat );

		if ( Error != eNone )
			return Error;

		Info.Size = Stat.st_size;
		Info.Type = _ConvertMode( Stat.st_mode );

		Info.Time.Access = Stat.st_atime;
		Info.Time.Creation = Stat.st_ctime;
		Info.Time.Modification = Stat.st_mtime;

		return Error;
	}


	//f Return true if the file 'Name' exists, false otherwise.
	inline bool Exists( const fnm::name___ &Name )
	{
		info__ Info;

		Info.Init();

		switch ( GetInfo( Name, Info ) ) {
		case eNone:
			return true;
			break;
		case eBadF:
			ERRSys();	// Normalement, cette erreur ne peut arriver, compte tenu de la fonction utilisée.
			break;
		case eNoEnt:
			break;
		case ENOTDIR:
			break;
		case eLoop:
			break;
		case eFault:
			ERRLbr();
			break;
		case eAcces:
			break;
		case eNoMem:
			ERRSys();
			break;
		case eNameTooLong:
			ERRSys();
			break;
		default:
			ERRFwk();
			break;
		}

		return false;
	}

	template <typename t> inline const info__ &_GetInfo(
		const t &FileSpecification,
		info__ &Info )
	{
		if ( GetInfo( FileSpecification, Info ) != eNone )
			ERRFwk();

		return Info;
	}

	template <typename t> inline time_t GetLastModificationTime( const t &FileSpecification )
	{
		info__ Info;

		Info.Init();

		return _GetInfo( FileSpecification, Info ).Time.Modification;
	}

	template <typename t> inline size__ GetSize( const t &FileSpecification )
	{
		info__ Info;

		Info.Init();

		return _GetInfo( FileSpecification, Info ).Size;
	}

	template <typename t> inline size__ GetType( const t &FileSpecification )
	{
		info__ Info;

		Info.Init();

		return _GetInfo( FileSpecification, Info ).Type;
	}

	// Modifie la date de modification d'un fichier à la date courante.
	inline void Touch( const fnm::name___ &FileName )
	{
# ifdef FIL__WIN
		FILETIME ft;
		SYSTEMTIME st;
		HANDLE Handle = CreateFileW( FileName.Core(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

		if ( Handle ==  INVALID_HANDLE_VALUE )
			ERRLbr();
		
		GetSystemTime( &st );

		if ( !SystemTimeToFileTime( &st, &ft ) )
			ERRSys();

		if ( !SetFileTime( Handle, (LPFILETIME) NULL, (LPFILETIME) NULL, &ft ) )
			ERRLbr();
# elif defined( FIL__POSIX )
		ERRProlog
			TOL_CBUFFER___ Buffer;
		ERRBegin
		/*
		NOTA : Le code ci-dessous fonctionne AUSSI sous Windows, mais SEULEMENT lorsque lancé à partir d'une console DOS,
		Lorsque lancé à partir d'une console 'Cygwin', il y a un décalage d'une heure (dépendant de l'heure d'hiver/été ?).
		*/

		if ( utime( FileName.UTF8( Buffer ), NULL ) != 0 )
			ERRLbr();
		ERRErr
		ERREnd
		ERREpilog
# else
#  error
# endif
	}

	inline bso::bool__ Remove( const fnm::name___ &FileName )
	{
# ifdef FIL__WIN
		return _wremove( FileName.Core() ) == 0;
# elif defined ( FIL__POSIX )
		return remove( FileName.Core() ) == 0;
# else
#  error
# endif
	}

	inline bso::bool__ Rename(
		const fnm::name___ &FileName,
		const fnm::name___ &NewFileName )
	{
# ifdef FIL__WIN
		return _wrename( FileName.Core(), NewFileName.Core() ) == 0;
# elif defined ( FIL__POSIX )
		return rename( FileName.Core(), NewFileName.Core() ) == 0;
# else
#  error
# endif
	}

	inline bso::bool__ Move(
		const fnm::name___ &Existing,
		const fnm::name___ &New )
	{
# ifdef FIL__WIN
		return MoveFileW( Existing.Core(), New.Core() ) != 0;
# elif defined ( FIL__POSIX )
		return rename( Existing.Core(), New.Core() ) == 0;
# else
#  error
# endif
	}

	bso::bool__ Create(
		const fnm::name___ &FileName,
		err::handling__ ErrorHandling = err::h_Default );	// Crée un fichier de nom 'FileName'.

# ifdef FIL__WIN
	inline bso::bool__ MakeNormal(
		const fnm::name___ &FileName,
		err::handling__ ErrorHandling = err::h_Default )	// Pour Windows, rend un fichier/répertoire normal.
	{
		if ( SetFileAttributesW( FileName.Core(), FILE_ATTRIBUTE_NORMAL ) == 0 )
			if ( ErrorHandling == err::hThrowException )
				ERRFwk();
			else
				return false;

		return true;
	}

	inline bso::bool__ MakeSystem(
		const fnm::name___ &FileName,
		err::handling__ ErrorHandling = err::h_Default )	// Pour Windows, rend un fichier/répertoire system.
															// NOTA : pour que le fichier soit caché lorsque l'option correspondande est activée,
															// il semblerait qu'il faut en plus mettre l'attribute 'HIDDEN' et 'ARCHIVE'.
	{
		if ( SetFileAttributesW( FileName.Core(), FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_ARCHIVE ) == 0 )
			if ( ErrorHandling == err::hThrowException )
				ERRFwk();
			else
				return false;

		return true;
	}
# endif

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
		const fnm::name___ &FileName,
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

	const fnm::name___ &GetBackupFileName(
		const fnm::name___ &FileName,
		fnm::name___ &Name );

	backup_status__ CreateBackupFile(
		const fnm::name___ &FileName,
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
		const fnm::name___ &FileName,
		lcl::meaning_ &Meaning );

	//f Recover the backup file 'Name' with 'Extension' as extension.
	recover_status__ RecoverBackupFile(
		const fnm::name___ &FileName,
		err::handling__ = err::h_Default  );
}
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
