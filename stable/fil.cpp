/*
	'fil' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'fil' header file ('fil.h').
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



//	$Id: fil.cpp,v 1.60 2013/07/18 19:46:24 csimon Exp $

#define FIL__COMPILATION

#include "fil.h"

class filtutor
: public ttr_tutor
{
public:
	filtutor( void )
	: ttr_tutor( FIL_NAME )
	{
#ifdef FIL_DBG
		Version = FIL_VERSION "\b\bD $";
#else
		Version = FIL_VERSION;
#endif
		Owner = FIL_OWNER;
		Date = "$Date: 2013/07/18 19:46:24 $";
	}
	virtual ~filtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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
	const char *Nom,
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

	return _open( Nom, Flags, PMode );
}

static void Close_( iop::descriptor__ D )
{
	if ( _close( D ) != 0 )
		ERRLbr();
}

#	elif defined( FIL__POSIX )
static inline iop::descriptor__ Open_(
	const char *Nom,
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

	return open( Nom, Flags, 0666 );	/* rw-rw-rw- */
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
	const char *Nom,
	mode__ Mode )
{
	return ::Open_( Nom, Mode );
}

void fil::Close( iop::descriptor__ D )
{
	::Close_( D );
}

bso::bool__ fil::CreateFile(
	const char *FileName,
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

const char* fil::GetBackupFileName(
	const char *FileName,
	FIL_BUFFER___ &Buffer )
{

	Buffer.Malloc( strlen( FileName ) + sizeof( FIL__BACKUP_FILE_EXTENSION  ) + 1 );

	sprintf( Buffer, "%s%s", FileName, FIL__BACKUP_FILE_EXTENSION );

	return Buffer;
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
	const char *FileName,
	lcl::meaning_ &Meaning )
{
ERRProlog
	FIL_BUFFER___ Buffer;
ERRBegin
	Meaning.SetValue( GetLabel( Status ) );

	switch ( Status ) {
	case bsUnableToRename:
	case bsUnableToDuplicate:
		Meaning.AddTag( FileName );
		break;
	case bsUnableToSuppress:
		Meaning.AddTag( GetBackupFileName( FileName, Buffer ) );
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
	const char *NomFichier,
	backup_mode__ Mode,
	err::handling__ ErrorHandling )
{
	backup_status__ Status = bs_Undefined;
ERRProlog
	const char *NomFichierSecurite = NULL;
	FIL_BUFFER___ Buffer;
ERRBegin
	if ( FileExists( NomFichier ) )
	{
		NomFichierSecurite = GetBackupFileName( NomFichier, Buffer );

		if ( FileExists( NomFichierSecurite ) )
			if ( remove( NomFichierSecurite ) ) {
				Status = bsUnableToSuppress;
				ERRReturn;
			}

		if ( Mode == bmDuplicate )
		{
#if 0
			std::ofstream Out( NomFichierSecurite );
			std::ifstream In( NomFichier );
			int C;

			while( Out && In && ( ( C = In.get() ) != EOF ) )
				Out.put( (char)C );

			Out.close();
			In.close();
#endif
		
			FILE *In, *Out;

			/* Ouverture de l'ancien fichier en lecture */
			In = fopen(NomFichier, "rb");

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
		}
		else if ( Mode == bmRename )
		{
			if ( rename( NomFichier, NomFichierSecurite ) )
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
	const char *FileName,
	lcl::meaning_ &Meaning )
{
ERRProlog
	FIL_BUFFER___ Buffer;
ERRBegin
	Meaning.SetValue( GetLabel( Status ) );

	switch ( Status ) {
	case rsUnableToRename:
		Meaning.AddTag( GetBackupFileName( FileName, Buffer ) );
		break;
	case rsUnableToSuppress:
		Meaning.AddTag(  FileName );
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
	const char *NomFichier,
	err::handling__ ErrorHandling )
{
	recover_status__ Status = rs_Undefined;
ERRProlog
	const char *NomFichierSecurite = NULL;
	FIL_BUFFER___ Buffer;
ERRBegin
	if ( FileExists( NomFichier ) )
		if ( remove( NomFichier ) ) {
			Status = rsUnableToSuppress;
			ERRReturn;
		}

	NomFichierSecurite = GetBackupFileName( NomFichier, Buffer );

	if ( FileExists( NomFichierSecurite ) )
		if ( rename( NomFichierSecurite, NomFichier ) ) {
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
: public filtutor
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


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static filpersonnalization Tutor;

ttr_tutor &FILTutor = Tutor;
