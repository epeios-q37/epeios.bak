/*
	Header for the 'tht' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: tht.h,v 1.10 2013/04/06 14:50:47 csimon Exp $

#ifndef THT__INC
#define THT__INC

#define THT_NAME		"THT"

#define	THT_VERSION	"$Revision: 1.10 $"

#define THT_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &THTTutor;

#if defined( E_DEBUG ) && !defined( THT_NODBG )
#define THT_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.10 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/06 14:50:47 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D THread Tools 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "cpe.h"

#if defined( CPE_POSIX )
#	define THT__POSIX
#elif defined ( CPE_WIN )
#	define THT__WIN
#else
#	error "Unknown target !"
#endif

#ifdef THT__WIN
#	include <process.h>
#	include <windows.h>
#elif defined( THT__POSIX )
#	include <pthread.h>
#	include <unistd.h>
#	include <stdlib.h>
#	include <signal.h>
#else
#	error 
#endif

#define THT_UNDEFINED_THREAD_ID	0	// Totalement arbitraire, à priori, correspond au thread système, donc ne peut être renvoyé par la focntion 'GetTID()'.


namespace tht {
#ifdef THT__WIN
	typedef DWORD	thread_id__;
#elif defined( THT__POSIX )
	typedef pthread_t	thread_id__;
#endif

	//f Return an unique ID for the current thread.
	inline thread_id__ GetTID( void )
	{
#ifdef THT__WIN
		return GetCurrentThreadId();
#elif defined( THT__POSIX )
		return pthread_self();
#else
#	error
#endif
	}


	//f Suspend the current thred for 'Delay' milliseconds.
	void Suspend( unsigned long Delay );

	//f Wait 'Seconds' seconds.
	inline void Wait( unsigned long Seconds )
	{
		Suspend( Seconds * 1000 );
	}

	//f Tell the remainder to give hand to the next thread.
	void Defer( void );

	//f Tell the remainder to give hand to the next thread and wait 'Delay' millisecond.
	inline void Defer( unsigned long Delay )
	{
		Defer();

		Suspend( Delay );
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
