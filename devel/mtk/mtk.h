/*
	'mtk.h' by Claude SIMON (http://zeusw.org/).

	'mtk' is part of the Epeios framework.

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

#ifndef MTK__INC
#define MTK__INC

#define MTK_NAME		"MTK"

#if defined( E_DEBUG ) && !defined( MTK_NODBG )
#define MTK_DBG
#endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// MultiTasKing

# include "err.h"
# include "cpe.h"
# include "errno.h"
# include "tht.h"

# if defined( CPE_POSIX )
#  define MTK__POSIX
# elif defined( CPE_WIN )
#  define MTK__WIN
# else
#  error "Unknown enviroment !"
# endif

 #ifdef MTK_THREADS_REMAIN
#  define MTK__THREADS_REMAIN
# endif


# ifdef MTK__WIN
#  include <process.h>
#  include <windows.h>
# elif defined( MTK__POSIX )
#  include <pthread.h>
#  include <unistd.h>
#  include <stdlib.h>
#  include <signal.h>
# else
#  error
# endif

# ifndef CPE_MT
#  error "Multitasking required, but compilation options don't allow this."
# endif

typedef void (* mtk__routine)(void *);

# ifdef E_KILL_THREADS
#  ifdef E_KEEP_THREADS
#   error "Both 'E_KILL_THREADS' and 'E_KEEP_THREADS' cannot be defined together."
#	else
#    define MTK__KILL
#  endif
# elif defined( E_KEEP_THREADS )
#  define MTK__KEEP
# else
#  ifdef CPE_LINUX
#   define MTK__KEEP
#  else
#   define MTK__KILL
#  endif
# endif

namespace mtk {
	typedef void (* routine__)(void *);
	
	/*f Launch a new thread executing 'Routine', with 'UP' as user pointer.
	Thread is killed when returning from 'Routine'. */
	void LaunchAndKill(
		routine__ Routine,
		void *UP );
		
	/*f Launch a new thread executing 'Routine', with 'UP' as user pointer.
	Thread is NOI killed when returning from 'Routine', and reused if available
	at next call of this function. */
	void LaunchAndKeep(
		routine__ Routine,
		void *UP );


	//f Launch a new thread executing 'Routine', with 'UP' as user pointer.
	inline void Launch(
		routine__ Routine,
		void *UP )
	{
#ifdef MTK__KILL
		LaunchAndKill( Routine, UP );
#elif defined( MTK__KEEP )
		LaunchAndKeep( Routine, UP );
#else
#	error "None of 'MTK_KEEP' or 'MTK_KILL' are defined."
#endif
	}

	/*f Force the program to exit after 'Seconds' second.
	Usefull to force a server to exit to obtain the profiling file. */
	void ForceExit( unsigned long Seconds );

	using tht::thread_id__;
	using tht::GetTID;

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
