/*
	'fil.cpp' by Claude SIMON (http://zeusw.org/).

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

#define FIL__COMPILATION

#include "fil.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "cpe.h"

#include <stdlib.h>

#include "str.h"
#include "lcl.h"

using namespace fil;

#ifdef IOP__USE_STANDARD_IO
static inline iop::descriptor__ Open_(
	const char *Nom,
	mode__ Mode )
{
	char Flags[4]="";

	switch ( Mode ) {
	case mRemove:
		strcat( Flags, "w+" );
		break;
	case mAppend:
		strcat( Flags, "a+" );
		break;
	case mReadWrite:
		if ( FileExists( Nom ) )
			strcat( Flags, "r+" );
		else
			strcat( Flags, "w+" );
		break;
	case mReadOnly:
		strcat( Flags, "r" );
		break;
	default:
		ERRPrm();
		break;
	}

	strcat( Flags, "b" );

	return fopen( Nom, Flags );
}

static void Close_( iop::descriptor__ D )
{
	if ( fclose( D ) != 0 )
		ERRLbr();
}

#elif defined( IOP__USE_LOWLEVEL_IO )
# if defined( FIL__WIN )

static inline iop::descriptor__ Open_(
	const fnm::name___ &Name,
	mode__ Mode )
{
	int Flags = _O_BINARY;
	int PMode = 0;

	switch ( Mode ) {
	case mRemove:
		Flags |= _O_TRUNC | _O_CREAT |_O_WRONLY;
		PMode |= _S_IWRITE;
		break;
	case mAppend:
		Flags |= _O_CREAT | _O_APPEND | _O_WRONLY;
		PMode |= _S_IWRITE /*| _S_IREAD*/;
		break;
	case mReadWrite:
		Flags |= _O_CREAT | _O_RDWR;
		PMode |= _S_IWRITE | _S_IREAD;
		break;
	case mReadOnly:
		Flags |= _O_RDONLY;
		break;
	default:
		ERRPrm();
		break;
	}

	return _wopen( Name.Core(), Flags, PMode );
}

static void Close_( iop::descriptor__ D )
{
	if ( _close( D ) != 0 )
		ERRLbr();
}

#	elif defined( FIL__POSIX )
static inline iop::descriptor__ Open_(
	const fnm::name___ &Name,
	mode__ Mode )
{
#ifdef CPE__CYGWIN
	int Flags = O_BINARY;
#else
	int Flags = 0;
#endif

	switch ( Mode ) {
	case mRemove:
		Flags |= O_TRUNC | O_CREAT | O_WRONLY;
		break;
	case mAppend:
		Flags |= O_APPEND | O_WRONLY;
		break;
	case mReadWrite:
		Flags |= O_CREAT | O_RDWR;
		break;
	case mReadOnly:
		Flags |= O_RDONLY;
		break;
	default:
		ERRPrm();
		break;
	}

	return open( Name.Core(), Flags, 0666 );	/* rw-rw-rw- */
}

static void Close_( iop::descriptor__ D )
{
	if ( close( D ) != 0 )
		ERRLbr();
}

#	else
#		error "Unknow complation enviroment !"
#	endif
#else
#	error "Unknow IO enviroment !"
#endif

iop::descriptor__ fil::Open(
	const fnm::name___ &Nom,
	mode__ Mode )
{
	return ::Open_( Nom, Mode );
}

void fil::Close( iop::descriptor__ D )
{
	::Close_( D );
}

bso::bool__ fil::Create(
	const fnm::name___ &FileName,
	err::handling__ ErrorHandling )
{
	bso::bool__ Success = false;
ERRProlog
	iop::descriptor__ Descriptor = IOP_UNDEFINED_DESCRIPTOR;
ERRBegin
	Descriptor = fil::Open( FileName, fil::mReadWrite );

	Success = ( Descriptor != IOP_UNDEFINED_DESCRIPTOR );

	if ( !Success && ( ErrorHandling == err::hThrowException ) )
		ERRLbr();
ERRErr
ERREnd
	if ( Descriptor != IOP_UNDEFINED_DESCRIPTOR )
		fil::Close( Descriptor );
ERREpilog
	return Success;
}

const fnm::name___ &fil::GetBackupFileName(
	const fnm::name___ &FileName,
	fnm::name___ &Buffer )
{
	return fnm::BuildFileName( NULL, FileName, FIL__BACKUP_FILE_EXTENSION, Buffer );
}

#define CASE( m )\
	case bs##m:\
	return FIL_NAME "_" #m;\
	break

const char *fil::GetLabel( backup_status__ Status )
{
	switch ( Status ) {
	CASE( UnableToRename );
	CASE( UnableToDuplicate );
	CASE( UnableToSuppress );
	default:
		ERRPrm();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

void fil::GetMeaning(
	backup_status__ Status,
	const fnm::name___ &FileName,
	lcl::meaning_ &Meaning )
{
ERRProlog
	TOL_CBUFFER___ Buffer;
	fnm::name___ BackupFileName;
ERRBegin
	Meaning.SetValue( GetLabel( Status ) );

	switch ( Status ) {
	case bsUnableToRename:
	case bsUnableToDuplicate:
		Meaning.AddTag( FileName.UTF8( Buffer ) );
		break;
	case bsUnableToSuppress:
		BackupFileName.Init();
		Meaning.AddTag( GetBackupFileName( FileName, BackupFileName ).UTF8( Buffer ) );
		break;
	default:
		ERRPrm();
		break;
	}
ERRErr
ERREnd
ERREpilog
}

backup_status__ fil::CreateBackupFile(
	const fnm::name___ &FileName,
	backup_mode__ Mode,
	err::handling__ ErrorHandling )
{
	backup_status__ Status = bs_Undefined;
ERRProlog
	fnm::name___ BackupFileName;
ERRBegin
	if ( Exists( FileName ) )
	{
		GetBackupFileName( FileName, BackupFileName );

		if ( Exists( BackupFileName ) )
			if ( !Remove( BackupFileName ) ) {
				Status = bsUnableToSuppress;
				ERRReturn;
			}

		if ( Mode == bmDuplicate )
		{
#if 0	// Old		
			FILE *In, *Out;

			/* Ouverture de l'ancien fichier en lecture */
			In = fopen( NomFichier, "rb");

			if ( In == NULL )
			{
				Status = bsUnableToDuplicate;
				ERRReturn;
			}

			Out = fopen(NomFichierSecurite, "w");
			if ( Out == NULL )
			{
				fclose ( In );
				Status = bsUnableToDuplicate;
				ERRReturn;
			}

			while (! feof( In ) )
			{
				if ( fputc( fgetc( In ), Out ) == EOF )
				{
					Status = bsUnableToDuplicate;
					fclose( In );
					fclose( Out );
					remove( NomFichierSecurite );
					ERRReturn;
				}
			}

			fclose(Out);
			fclose(In);
#endif
			ERRVct();
		}
		else if ( Mode == bmRename )
		{
			if ( !Rename( FileName, BackupFileName ) )
				Status = bsUnableToRename;
		}
		else
			ERRPrm();
	}

	Status = bsOK;
ERRErr
ERREnd
	if ( Status != bsOK )
		if ( ErrorHandling == err::hThrowException )
			ERRFwk();
ERREpilog
	return Status;
}

#undef CASE

#define CASE( m )\
	case rs##m:\
	return FIL_NAME "_" #m;\
	break


const char *fil::GetLabel( recover_status__ Status )
{
	switch ( Status ) {
	CASE( UnableToRename );
	CASE( UnableToSuppress );
	default:
		ERRPrm();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

void fil::GetMeaning(
	recover_status__ Status,
	const fnm::name___ &FileName,
	lcl::meaning_ &Meaning )
{
ERRProlog
	fnm::name___ BackupFileName;
	TOL_CBUFFER___ Buffer;
ERRBegin
	Meaning.SetValue( GetLabel( Status ) );

	switch ( Status ) {
	case rsUnableToRename:
		BackupFileName.Init();
		Meaning.AddTag( GetBackupFileName( FileName, BackupFileName ).UTF8( Buffer ) );
		break;
	case rsUnableToSuppress:
		Meaning.AddTag(  FileName.UTF8( Buffer ) );
		break;
	default:
		ERRPrm();
		break;
	}
ERRErr
ERREnd
ERREpilog
}

recover_status__ fil::RecoverBackupFile(
	const fnm::name___ &FileName,
	err::handling__ ErrorHandling )
{
	recover_status__ Status = rs_Undefined;
ERRProlog
	fnm::name___ BackupFileName;
	TOL_CBUFFER___ Buffer;
ERRBegin
	if ( Exists( FileName ) )
		if ( !Remove( FileName ) ) {
			Status = rsUnableToSuppress;
			ERRReturn;
		}

	BackupFileName.Init();
	GetBackupFileName( FileName, BackupFileName );

	if ( Exists( BackupFileName ) )
		if ( !Rename( BackupFileName, FileName ) ) {
			Status = rsUnableToRename;
			ERRReturn;
		}

	Status = rsOK;

ERRErr
ERREnd
	if ( Status != rsOK )
		if ( ErrorHandling == err::hThrowException )
			ERRFwk();
ERREpilog
	return Status;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class filpersonnalization
{
public:
	filpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~filpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static filpersonnalization Tutor;
