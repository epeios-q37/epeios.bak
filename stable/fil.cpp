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

#define FIL__COMPILATION

#include "fil.h"

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
		qRFwk();
		break;
	}

	strcat( Flags, "b" );

	return fopen( Nom, Flags );
}

static void Close_( iop::descriptor__ D )
{
	if ( fclose( D ) != 0 )
		qRLbr();
}

#elif defined( IOP__USE_LOWLEVEL_IO )
# if defined( FIL__WIN )

static inline iop::descriptor__ Open_(
	const fnm::name___ &Filename,
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
		qRFwk();
		break;
	}

	return _wopen( Filename.Internal(), Flags, PMode );
}

static void Close_( iop::descriptor__ D )
{
	if ( _close( D ) != 0 )
		qRLbr();
}

#	elif defined( FIL__POSIX )
static inline iop::descriptor__ Open_(
	const fnm::name___ &Filename,
	mode__ Mode )
{
#ifdef CPE_S_CYGWIN
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
		qRFwk();
		break;
	}

	return open( Filename.Internal(), Flags, 0666 );	/* rw-rw-rw- */
}

static void Close_( iop::descriptor__ D )
{
	if ( close( D ) != 0 )
		qRLbr();
}

#	else
#		error "Unknow complation enviroment !"
#	endif
#else
#	error "Unknow IO enviroment !"
#endif

iop::descriptor__ fil::Open(
	const fnm::name___ &Filename,
	mode__ Mode )
{
	return ::Open_( Filename, Mode );
}

void fil::Close( iop::descriptor__ D )
{
	::Close_( D );
}

#include "cio.h"

bso::bool__ fil::Create(
	const fnm::name___ &Filename,
	err::handling__ ErrorHandling )
{
	bso::bool__ Success = false;
qRH
	iop::descriptor__ Descriptor = IOP_UNDEFINED_DESCRIPTOR;
qRB
	Descriptor = fil::Open( Filename, fil::mReadWrite );

	Success = ( Descriptor != IOP_UNDEFINED_DESCRIPTOR );

	if ( !Success && (ErrorHandling == err::hThrowException) ) {
// If removed, remove above "#include...".
		cio::CErr << txf::nl << Filename << txf::nl << txf::commit;
		qRLbr();
	}

qRR
qRT
	if ( Descriptor != IOP_UNDEFINED_DESCRIPTOR )
		fil::Close( Descriptor );
qRE
	return Success;
}

namespace {
	void Copy_(
		const fnm::name___ &SourceFilename,
		const fnm::name___ &TargetFilename,
		size__ Size )
	{
		qRH
			flf::file_iflow___ Source;
			flf::file_oflow___ Target;
		qRB
			Source.Init( SourceFilename );
			Target.Init( TargetFilename );

			while ( Size-- ) {
				Target.Put( Source.Get() );
			}
		qRR
		qRT
		qRE
	}
}

void fil::Shrink(
	const fnm::name___ &Filename,
	size__ Size )
{
qRH
	str::wString TempName;
qRB
	TempName.Init();
	Filename.UTF8( TempName );
	
	TempName.Append( '~' );

	Copy_( Filename, TempName, Size );

	Remove( Filename );
	Rename( TempName, Filename );
qRR
	Remove( TempName );
qRT
qRE
}


const fnm::name___ &fil::GetBackupFilename(
	const fnm::name___ &Filename,
	fnm::name___ &Buffer )
{
	return fnm::BuildPath( "", Filename, FIL__BACKUP_FILE_EXTENSION, Buffer );
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
		qRFwk();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}

void fil::GetMeaning(
	backup_status__ Status,
	const fnm::name___ &Filename,
	lcl::meaning_ &Meaning )
{
qRH
	TOL_CBUFFER___ Buffer;
	fnm::name___ BackupFilename;
qRB
	Meaning.SetValue( GetLabel( Status ) );

	switch ( Status ) {
	case bsUnableToRename:
	case bsUnableToDuplicate:
		Meaning.AddTag( Filename.UTF8( Buffer ) );
		break;
	case bsUnableToSuppress:
		BackupFilename.Init();
		Meaning.AddTag( GetBackupFilename( Filename, BackupFilename ).UTF8( Buffer ) );
		break;
	default:
		qRFwk();
		break;
	}
qRR
qRT
qRE
}

backup_status__ fil::CreateBackupFile(
	const fnm::name___ &Filename,
	backup_mode__ Mode,
	err::handling__ ErrorHandling )
{
	backup_status__ Status = bsOK;
qRH
	fnm::name___ BackupFilename;
qRB
	if ( Exists( Filename ) ) {
		GetBackupFilename( Filename, BackupFilename );

		if ( Exists( BackupFilename ) )
			if ( !Remove( BackupFilename ) )
				Status = bsUnableToSuppress;

		if ( Status == bsOK ) {
			if ( Mode == bmDuplicate ) {
#if 0	// Old		
				FILE *In, *Out;

				/* Ouverture de l'ancien fichier en lecture */
				In = fopen( NomFichier, "rb" );

				if ( In == NULL ) {
					Status = bsUnableToDuplicate;
					qRReturn;
				}

				Out = fopen( NomFichierSecurite, "w" );
				if ( Out == NULL ) {
					fclose( In );
					Status = bsUnableToDuplicate;
					qRReturn;
				}

				while ( !feof( In ) ) {
					if ( fputc( fgetc( In ), Out ) == EOF ) {
						Status = bsUnableToDuplicate;
						fclose( In );
						fclose( Out );
						remove( NomFichierSecurite );
						qRReturn;
					}
				}

				fclose( Out );
				fclose( In );
#endif
				qRVct();
			} else if ( Mode == bmRename ) {
				if ( !Rename( Filename, BackupFilename ) )
					Status = bsUnableToRename;
			} else
				qRFwk();
		}
	}
qRR
qRT
	if ( Status != bsOK )
		if ( ErrorHandling == err::hThrowException )
			qRFwk();
qRE
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
		qRFwk();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}

void fil::GetMeaning(
	recover_status__ Status,
	const fnm::name___ &Filename,
	lcl::meaning_ &Meaning )
{
qRH
	fnm::name___ BackupFilename;
	TOL_CBUFFER___ Buffer;
qRB
	Meaning.SetValue( GetLabel( Status ) );

	switch ( Status ) {
	case rsUnableToRename:
		BackupFilename.Init();
		Meaning.AddTag( GetBackupFilename( Filename, BackupFilename ).UTF8( Buffer ) );
		break;
	case rsUnableToSuppress:
		Meaning.AddTag(  Filename.UTF8( Buffer ) );
		break;
	default:
		qRFwk();
		break;
	}
qRR
qRT
qRE
}

recover_status__ fil::RecoverBackupFile(
	const fnm::name___ &Filename,
	err::handling__ ErrorHandling )
{
	recover_status__ Status = rsOK;
qRH
	fnm::name___ BackupFilename;
	TOL_CBUFFER___ Buffer;
qRB
	if ( Exists( Filename ) )
		if ( !Remove( Filename ) )
			Status = rsUnableToSuppress;

	if ( Status == rsOK ) {
		BackupFilename.Init();
		GetBackupFilename( Filename, BackupFilename );

		if ( Exists( BackupFilename ) )
			if ( !Rename( BackupFilename, Filename ) )
				Status = rsUnableToRename;
	}
qRR
qRT
	if ( Status != rsOK )
		if ( ErrorHandling == err::hThrowException )
			qRFwk();
qRE
	return Status;
}
