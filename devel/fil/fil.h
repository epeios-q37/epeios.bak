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

// Prdclaration, car l'incusion des fichiers d'entte correspondants posent des problmes d  l'inclusion circulaire.

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
		//i Ajoute  la fin du fichier.
		mAppend,
		//i Ouvre le fichier ne lecture/criture.
		mReadWrite,
		//i Ouvre le fichier en lecture seule.
		mReadOnly,
		//i Amount of mode.
		m_amount,
		m_Undefined,
	};

	iop::descriptor__ Open(
		const fnm::name___ &Path,
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
# ifdef CPE_MINGW	// 'MinGW' ne connait pas 'ELOOP'.
		case WSAELOOP:
# else
		case ELOOP:
# endif
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
				Creation = Modification = Access = 0;
			}
			E_CDTOR( time__ );
			void Init( void )
			{
				Creation = Modification = Access = 0;
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
		const fnm::name___ &Path,
		struct FIL__STATS &Stat )
	{
		errno = 0;

		if ( FIL__STATF( Path.Internal(), &Stat ) != 0 )
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
		const t &Path,
		info__ &Info )
	{
		struct FIL__STATS Stat;

		error__ Error = _Stat( Path, Stat );

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
			ERRSys();	// Normalement, cette erreur ne peut arriver, compte tenu de la fonction utilise.
			break;
		case eNoEnt:
			break;
		case eNotDir:
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
		const t &Path,
		info__ &Info )
	{
		if ( GetInfo( Path, Info ) != eNone )
			ERRFwk();

		return Info;
	}

	template <typename t> inline time_t GetLastModificationTime( const t &Path )
	{
		info__ Info;

		Info.Init();

		return _GetInfo( Path, Info ).Time.Modification;
	}

	template <typename t> inline size__ GetSize( const t &Path )
	{
		info__ Info;

		Info.Init();

		return _GetInfo( Path, Info ).Size;
	}

	template <typename t> inline size__ GetType( const t &Path )
	{
		info__ Info;

		Info.Init();

		return _GetInfo( Path, Info ).Type;
	}

	// Modifie la date de modification d'un fichier  la date courante.
	inline bso::bool__ Touch( const fnm::name___ &Filename )
	{
		bso::bool__ Success = false;
	ERRProlog
# ifdef FIL__WIN
		FILETIME ft;
		SYSTEMTIME st;
		HANDLE Handle = INVALID_HANDLE_VALUE;
	ERRBegin
		Handle = CreateFileW( Filename.Internal(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

		if ( Handle == INVALID_HANDLE_VALUE )
			ERRReturn;
		
		GetSystemTime( &st );

		if ( !SystemTimeToFileTime( &st, &ft ) )
			ERRReturn;

		if ( !SetFileTime( Handle, (LPFILETIME) NULL, (LPFILETIME) NULL, &ft ) )
			ERRReturn;

		Success = true;
	ERRErr
	ERREnd
		if ( Handle != INVALID_HANDLE_VALUE )
			CloseHandle( Handle );
# elif defined( FIL__POSIX )
		TOL_CBUFFER___ Buffer;
	ERRBegin
		/*
		NOTA : Le code ci-dessous fonctionne AUSSI sous Windows, mais SEULEMENT lorsque lanc  partir d'une console DOS,
		Lorsque lanc  partir d'une console 'Cygwin', il y a un dcalage d'une heure (dpendant de l'heure d'hiver/t ?).
		*/

		if ( utime( Filename.UTF8( Buffer ), NULL ) != 0 )
			ERRLbr();

		Success = true;
	ERRErr
	ERREnd
# else
#  error
# endif
	ERREpilog
		return Success;
	}

	inline bso::bool__ Remove( const fnm::name___ &Filename )
	{
# ifdef FIL__WIN
		return _wremove( Filename.Internal() ) == 0;
# elif defined ( FIL__POSIX )
		return remove( Filename.Internal() ) == 0;
# else
#  error
# endif
	}

	inline bso::bool__ Rename(
		const fnm::name___ &Filename,
		const fnm::name___ &NewFilename )
	{
# ifdef FIL__WIN
		return _wrename( Filename.Internal(), NewFilename.Internal() ) == 0;
# elif defined ( FIL__POSIX )
		return rename( Filename.Internal(), NewFilename.Internal() ) == 0;
# else
#  error
# endif
	}

	inline bso::bool__ Move(
		const fnm::name___ &Existing,
		const fnm::name___ &New )
	{
# ifdef FIL__WIN
		return MoveFileW( Existing.Internal(), New.Internal() ) != 0;
# elif defined ( FIL__POSIX )
		return rename( Existing.Internal(), New.Internal() ) == 0;
# else
#  error
# endif
	}

	inline bso::bool__ AssignSameTimes_(
		const fnm::name___ &SourceFilename,
		const fnm::name___ &TargetFilename )
	{
		bso::bool__ Success = false;
	ERRProlog
# ifdef FIL__WIN
		HANDLE
			Source = INVALID_HANDLE_VALUE,
			Target = INVALID_HANDLE_VALUE;
		FILETIME Creation, Access, Write;
	ERRBegin
		Source = CreateFileW( SourceFilename.Internal(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

		if ( Source == INVALID_HANDLE_VALUE )
			ERRReturn;

		Target = CreateFileW( TargetFilename.Internal(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

		if ( Target == INVALID_HANDLE_VALUE )
			ERRReturn;

		if ( !GetFileTime( Source, &Creation, &Access, &Write ) )
			ERRReturn;

		if ( !SetFileTime( Target, &Creation, &Access, &Write ) )
			ERRReturn;

		Success = true;
	ERRErr
	ERREnd
		if ( Source != INVALID_HANDLE_VALUE )
			CloseHandle( Source );

		if ( Target != INVALID_HANDLE_VALUE )
			CloseHandle( Target );
# elif defined( FIL__POSIX )
		utimbuf TimeBuf;
		info__ Info;
		TOL_CBUFFER___ Buffer;
	ERRBegin
		Info.Init();
	
		if ( GetInfo( SourceFilename, Info ) != eNone )
			ERRReturn;

		TimeBuf.actime = Info.Time.Access;
		TimeBuf.modtime = Info.Time.Modification;

		if ( utime( TargetFilename.UTF8( Buffer ), &TimeBuf ) != 0 )
			ERRReturn;

		Success = true;
	ERRErr
	ERREnd
# else
#  error
# endif
	ERREpilog
		return Success;

	}

	inline bso::bool__ Copy(
		const fnm::name___ &SourceFilename,
		const fnm::name___ &TargetFilename )
	{
# ifdef FIL__WIN
		// NOTA : le fichier copi avec la fonction ci-dessous possde les mmes horodatages, on s'arrange donc pour avoir le mme rsultat sous POSIX.
		return CopyFileW( SourceFilename.Internal(), TargetFilename.Internal(), false ) != 0;
# elif defined( FIL__POSIX )
		bso::bool__ Success = false;
	ERRProlog
		int Source = FIL_UNDEFINED_DESCRIPTOR;
		int Target = FIL_UNDEFINED_DESCRIPTOR;
		char Buffer[BUFSIZ];
		size_t Size = 0;
	ERRBegin
		Source = Open( SourceFilename, mReadOnly );

		if ( Source == FIL_UNDEFINED_DESCRIPTOR )
			ERRReturn;

		Target = Open( TargetFilename, mRemove);

		if ( Target == FIL_UNDEFINED_DESCRIPTOR )
			ERRReturn;

		while ( ( Size = read( Source, Buffer, sizeof( Buffer ) ) ) > 0 )  {
			if ( write( Target, Buffer, Size ) == -1 )
				ERRReturn;
		}

		if ( Size == -1 )
			ERRReturn;

		// NOTA : le fichier copi avec la version Windows de cette fonction possde les mmes horodatages, on s'arrange donc pour avoir le mme rsultat sous POSIX.
		Success = AssignSameTimes_( SourceFilename, TargetFilename );
	ERRErr
	ERREnd
		if ( Source != FIL_UNDEFINED_DESCRIPTOR )
			close( Source );

		if ( Target != FIL_UNDEFINED_DESCRIPTOR )
			close( Target );		
	ERREpilog
		return Success;
# else
#  error
# endif
	}


	bso::bool__ Create(
		const fnm::name___ &Filename,
		err::handling__ ErrorHandling = err::h_Default );	// Cre un fichier de nom 'FileName'.

# ifdef FIL__WIN
	inline bso::bool__ MakeNormal(
		const fnm::name___ &Filename,
		err::handling__ ErrorHandling = err::h_Default )	// Pour Windows, rend un fichier/rpertoire normal.
	{
		if ( SetFileAttributesW(Filename.Internal(), FILE_ATTRIBUTE_NORMAL) == 0 ) {
			if ( ErrorHandling == err::hThrowException )
				ERRFwk();
			else
				return false;
		}

		return true;
	}

	inline bso::bool__ MakeSystem(
		const fnm::name___ &Filename,
		err::handling__ ErrorHandling = err::h_Default )	// Pour Windows, rend un fichier/rpertoire system.
															// NOTA : pour que le fichier soit cach lorsque l'option correspondande est active,
															// il semblerait qu'il faut en plus mettre l'attribute 'HIDDEN' et 'ARCHIVE'.
	{
		if ( SetFileAttributesW(Filename.Internal(), FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_ARCHIVE) == 0 ) {
			if ( ErrorHandling == err::hThrowException )
				ERRFwk();
			else
				return false;
		}

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
		const fnm::name___ &Filename,
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

	const fnm::name___ &GetBackupFilename(
		const fnm::name___ &FileName,
		fnm::name___ &Name );

	backup_status__ CreateBackupFile(
		const fnm::name___ &Filename,
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
		const fnm::name___ &Filename,
		lcl::meaning_ &Meaning );

	recover_status__ RecoverBackupFile(
		const fnm::name___ &Filename,
		err::handling__ = err::h_Default  );
}
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
