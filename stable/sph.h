/*
  Header for the 'sph' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: sph.h,v 1.4 2012/11/14 16:06:37 csimon Exp $

#ifndef SPH__INC
#define SPH__INC

#define SPH_NAME		"SPH"

#define	SPH_VERSION	"$Revision: 1.4 $"	

#define SPH_OWNER		"the Epeios project (http://zeusw.org/epeios/) "

#if defined( E_DEBUG ) && !defined( SPH_NODBG )
#define SPH_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (http://zeusw.org/intl/contact.html) 
//R $Date: 2012/11/14 16:06:37 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#error "Library 'sph' is obsolete ; use library 'mtx' instead."

#include "err.h"
#include "cpe.h"

#if defined( CPE__UNIX )
#	define SPH__UNIX_LIKE
#elif !defined( CPE__BEOS )
#	define SPH__CUSTOM
#endif

#ifdef SPH__CUSTOM
#	include "shm.h"
#	include "bso.h"
typedef shm__desc sph__desc;
typedef bso__ubyte sph__counter;
//d An invalid semaphore descriptor.
#	define SPH_INVALID	SHM_INVALID
#elif defined( SPH__UNIX_LIKE )
#	include <sys/types.h>
#	include <sys/ipc.h>
#	include <sys/sem.h>
#	include <errno.h>
typedef int sph__desc;
#	define SPH_INVALID	-1

// From 'man semctl'.
// Begin
#	if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/* union semun is defined by including <sys/sem.h> */
#	else
/* according to X/OPEN we have to define it ourselves */
union semun {
	int val;                    /* value for SETVAL */
	struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
	unsigned short int *array;  /* array for GETALL, SETALL */
	struct seminfo *__buf;      /* buffer for IPC_INFO */
};
#	endif
// End
#elif defined( CPE__BEOS )
#	include <be/kernel/OS.h>
typedef sem_id sph__desc;
#	define SPH_INVALID	B_NO_MORE_SEMS
#else
#	error "Unknow compilation enviroment"
#endif

//f Return a new semaphore.
inline sph__desc SPHCreate( void )
{
	sph__desc Desc;
#ifdef SPH__CUSTOM
	sph__counter *Counter;

	Desc = SHMAllocate( sizeof( sph__counter ) );

	Counter = (sph__counter *)SHMMap( Desc );

	*Counter = 0;

	SHMUnmap( Counter );
#elif defined( SPH__UNIX_LIKE )
	union semun Un;

	Un.val = 1;

	if( ( Desc = semget( IPC_PRIVATE, 1, 0x1ff ) ) == -1 )
		ERRs();

	if ( semctl( Desc, 0, SETVAL, Un ) == -1 )
		ERRs();
#elif defined( CPE__BEOS )
	Desc = create_sem( 1, NULL );
	
	if ( Desc == B_BAD_VALUE )
		ERRu();
	else if ( Desc == B_NO_MEMORY )
		ERRa();
	else if ( Desc == B_NO_MORE_SEMS )
	    ERRs();
#else
#	error "Unknow compilation enviroment"
#endif
	return Desc;
}

//f Delete the semaphore of descriptor 'Desc'.
inline void SPHDelete( sph__desc Desc )
{
#ifdef SPH__CUSTOM
	SHMFree( Desc );
#elif defined( SPH__UNIX_LIKE )
	struct semid_ds DS;
	union semun Un;

	Un.buf = &DS;

	if ( semctl( Desc, 0, IPC_STAT, Un ) == -1 )
		ERRs();

	if ( semctl( Desc, 0, IPC_RMID, Un ) == -1 )
		ERRs();
#elif defined( CPE__BEOS )
	status_t Status = delete_sem( Desc );
	
	if ( Status != B_NO_ERROR )
		ERRu();
#else
#	error "Unknow compilation enviroment"
#endif
}

//c A semaphore. Can only be allocated once.
class sph__semaphore_
{
private:
#ifdef SPH__CUSTOM
	// Compteur du nombre de demande.
	sph__counter *Compteur_;
	// Retourne vrai si le smaphore nous a t attribu, faux sinon.
	bool AllouerSemaphore_( void )
	{
		if ( *Compteur_ != 0 )
			return false;

		(*Compteur_)++;

		if ( *Compteur_ > 1 )
		{
			(*Compteur_)--;
			return false;
		}
		else
			return true;
	}
	// Boucle jusqu' ce que le smaphore indique la disponibilit.
	void AttendreDisponibilite_( void );
#elif defined( SPH__UNIX_LIKE ) || defined( CPE__BEOS )
	sph__desc Descripteur_;
#else
#	error "Unknow compilation enviroment"
#endif
public:
	void reset( bool P = true )
	{
#ifdef SPH__CUSTOM
	if ( P )
		if ( Compteur_ )
			SHMUnmap( Compteur_);

	Compteur_ = NULL;
#elif defined( SPH__UNIX_LIKE ) || defined( CPE__BEOS )
	Descripteur_ = SPH_INVALID;
#else
#	error "Unknow compilation enviroment"
#endif
	}
	sph__semaphore_( void )
	{
		reset( false );
	}
	~sph__semaphore_( void )
	{
		reset( true );
	}
	//f Initialization.with semaphore descriptor 'Semaphore'.
	void Init( sph__desc Desc )
	{
		reset();
#ifdef SPH__CUSTOM
		Compteur_ = (sph__counter *)SHMMap( Desc );
#elif defined( SPH__UNIX_LIKE ) || defined( CPE__BEOS )
		Descripteur_ = Desc;
#else
#	error "Unknow compilation enviroment"
#endif
	}
	//f Lock the semaphore. BLOCKING FUNCTION.
	void Lock( void )
	{
#ifdef SPH__CUSTOM
#	ifdef SPH_DBG
		if ( Compteur_ == NULL )
			ERRu();
#	endif
		if ( !AllouerSemaphore_() )
			AttendreDisponibilite_();
#else
#	ifdef SPH_DBG
		if ( Descripteur_ == SPH_INVALID )
			ERRu();
#	endif
#	if defined( SPH__UNIX_LIKE )
		sembuf Buf = { 0, -1, 0 };

		if ( semop( Descripteur_, &Buf, 1 ) == -1 )
			ERRs();
#	elif defined( CPE__BEOS )
	status_t Status = acquire_sem( Descripteur_ );
	
	if ( Status != B_NO_ERROR )
		ERRu();
#	else
#		error "Unknow compilation enviroment"
#	endif
#endif
	}
	//f Unlock the semaphore.
	void Unlock( void )
	{
#ifdef SPH__CUSTOM
#	ifdef SPH_DBG
		if ( Compteur_ == NULL )
			ERRu();
#	endif
		(*Compteur_)--;
#else
#	ifdef SPH_DBG
		if ( Descripteur_ == SPH_INVALID )
			ERRu();
#	endif
#	if defined( SPH__UNIX_LIKE )
		sembuf Buf = { 0, 1, 0 };

		if ( semop( Descripteur_, &Buf, 1 ) == -1 )
			ERRs();
#	elif defined( CPE__BEOS )
	status_t Status = release_sem( Descripteur_ );
	
	if ( Status != B_NO_ERROR )
		ERRu();
#	else
#		error "Unknow compilation enviroment"
#	endif
#endif
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
