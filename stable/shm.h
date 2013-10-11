/*
  Header for the 'shm' library by Claude SIMON (http://zeusw.org/intl/contact.html)
  Copyright (C) 2000 Claude L. Simon 

  This file is part of the Epeios project (http://zeusw.org/epeios/).
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 3
  of the License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: shm.h,v 1.4 2012/11/14 16:06:36 csimon Exp $

#ifndef SHM__INC
#define SHM__INC

#define SHM_NAME		"SHM"

#define	SHM_VERSION	"$Revision: 1.4 $"	

#define SHM_OWNER		"the Epeios project (http://zeusw.org/epeios/) "

#include "ttr.h"

extern class ttr_tutor &SHMTutor;

#if defined( E_DEBUG ) && !defined( SHM_NODBG )
#define SHM_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (http://zeusw.org/intl/contact.html) 
//R $Date: 2012/11/14 16:06:36 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#ifndef SHM_FORCE_USE
#error "The 'shm' library is no longer needed. To override this error, define 'SHM_FORCE_USE'"
#endif

#include "err.h"
#include "cpe.h"

#if defined( CPE__UNIX )
#	define SHM__UNIX_LIKE
#endif

#ifdef CPE__MS
#	include <windows.h>
//d	An invalid shared memory descriptor.
#	define SHM_INVALID	NULL
	typedef void * shm__desc;
#elif defined( CPE__BEOS )
#	include <be/kernel/OS.h>
	typedef area_id shm__desc;
#	define SHM_INVALID	B_ERROR
#elif defined( SHM__UNIX_LIKE )
#	include <sys/shm.h>
	typedef int shm__desc;
#	define SHM_INVALID	-1
#else
#	error "Unknow compilation enviroment"
#endif

//f Return a descriptor to a portion of shared memory of size 'Size'.
inline shm__desc SHMAllocate( size_t Size )
{
	shm__desc Desc;
#ifdef CPE__MS
	if ( ( Desc = CreateFileMapping( (HANDLE)0xffffffff, NULL, PAGE_READWRITE, 0, Size, NULL ) ) == SHM_INVALID )
		ERRs();
#elif defined( CPE__BEOS )
	void *Dummy;
	if ( ( Desc = create_area( "", &Dummy, B_ANY_ADDRESS, ( ( Size % B_PAGE_SIZE ) + 1 ) * B_PAGE_SIZE, B_FULL_LOCK, B_WRITE_AREA ) ) < 0 )
		ERRs();
#elif defined( SHM__UNIX_LIKE )
	if ( ( Desc = shmget( IPC_PRIVATE, Size, 0x1ff | IPC_CREAT ) ) == SHM_INVALID)
		ERRs();
#else
#	error "Unknow compilation enviroment"
#endif

	return Desc;
}

//f Return a pointer to the memory mapped with the shared memory of descriptor 'Desc'.
inline void *SHMMap( shm__desc Desc )
{
#ifdef SHM_DBG
	if ( Desc == SHM_INVALID )
		ERRu();
#endif
	void *Pointeur;
#ifdef CPE__MS
	if ( ( Pointeur = MapViewOfFile( Desc, FILE_MAP_ALL_ACCESS, 0, 0, 0 ) ) == NULL )
		ERRs();
#elif defined( CPE__BEOS )
	if ( clone_area( "", &Pointeur, B_ANY_ADDRESS, B_WRITE_AREA, Desc ) < 0 )
		ERRs();
#elif defined( SHM__UNIX_LIKE )
	if ( ( Pointeur = shmat( Desc, 0, 0 ) ) == (void *)-1 )
		ERRs();
#else
#	error "Unknow compilation enviroment"
#endif
	return Pointeur;
}

//f Unmap the mapped memory corresponding to 'Mem'.
inline void SHMUnmap( void *Mem )
{
#ifdef SHM_DBG
	if ( Mem == NULL )
		ERRu();
#endif
#ifdef CPE__MS
	if ( !UnmapViewOfFile( Mem ) )
		ERRs();
#elif defined( CPE__BEOS )
	area_id Area;
	
	if ( ( Area = area_for( Mem ) ) == B_OK )
		ERRu();
		
	if ( delete_area( Area ) != B_OK )
		ERRs(); 
#elif defined( SHM__UNIX_LIKE )
	if ( shmdt( (char *)Mem ) )
		ERRs();
#else
#	error "Unknow compilation enviroment"
#endif
}

//f Free the shared memory of descriptor 'Desc'. 
inline void SHMFree( shm__desc Desc )
{
#ifdef SHM_DBG
	if ( Desc == SHM_INVALID )
		ERRu();
#endif
#ifdef CPE__MS
	if ( !CloseHandle( Desc ) )
		ERRs();
#elif defined( CPE__BEOS )
	if ( delete_area( Desc ) != B_OK )
		ERRs();
#elif defined( SHM__UNIX_LIKE )
	shmid_ds DS;

	shmctl( Desc, IPC_STAT, &DS );
	shmctl( Desc, IPC_RMID, &DS );
#else
#	error "Unknow compilation enviroment"
#endif
}



/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
