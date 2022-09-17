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

#ifndef MTK_INC_
# define MTK_INC_

# define MTK_NAME		"MTK"

# if defined( E_DEBUG ) && !defined( MTK_NODBG )
#  define MTK_DBG
# endif

// MultiTasKing

# include "err.h"
# include "cpe.h"
# include "errno.h"
# include "tht.h"

# if defined( CPE_S_POSIX )
#  define MTK__POSIX
# elif defined( CPE_S_WIN )
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

# ifndef CPE_F_MT
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
#  ifdef MTK__POSIX
#   define MTK__KEEP
#  else
#   define MTK__KILL
#  endif
# endif

namespace mtk {
    // Called on final error handling, before quitting the thread.
    // NOT called with 'Raw!launchAndKill' (hence the reminder).
  extern void (* MTKErrorHandling)(void); // To override by user.

	typedef void (* routine__)(void *);

	/*f Launch a new thread executing 'Routine', with 'UP' as user pointer.
	Thread is killed when returning from 'Routine'. */
	void RawLaunchAndKill(
		routine__ Routine,
		void *UP,
		bso::sBool Reminder );  // Useless parameter, only as reminder
                            // that there is no default final error handling
                            // for thread launched with this function.

	/*f Launch a new thread executing 'Routine', with 'UP' as user pointer.
	Thread is NOI killed when returning from 'Routine', and reused if available
	at next call of this function. */
	void RawLaunchAndKeep(
		routine__ Routine,
		void *UP,
		bso::sBool = true);    // To be like 'RawLaunchAndKill(…)'; makes some things easier.

	/* Launch in the same thread, does return only when 'Routine' returns.
	Useful for testing */
	inline void RawSyncLaunch(
		routine__ Routine,
		void *UP,
		bso::sBool = true)    // To be like 'RawLaunchAndKill(…)'; makes some things easier.
	{
		Routine(UP);
	}


	//f Launch a new thread executing 'Routine', with 'UP' as user pointer.
	inline void RawLaunch(
		routine__ Routine,
		void *UP,
		bso::sBool) // Same as for `RawLaunchAndKill(…)`.
	{
#ifdef MTK__KILL
		RawLaunchAndKill(Routine, UP, true);
#elif defined( MTK__KEEP )
		RawLaunchAndKeep(Routine, UP, true );
#else
#	error "None of 'MTK_KEEP' or 'MTK_KILL' are defined."
#endif
	}

	/*f Force the program to exit after 'Seconds' second.
	Useful to force a server to exit to obtain the profiling file. */
	void ForceExit( unsigned long Seconds );

	using tht::thread_id__;
	using tht::GetTID;

  struct rXBlocker_
  {
  public:
    tht::rBlocker Blocker;
    bso::sBool Error;
    void reset(bso::sBool P = true)
    {
      Blocker.reset(P);
      Error = false;
    }
    qCDTOR(rXBlocker_);
    void Init(void)
    {
      Blocker.Init();
      Error = false;
    }
    void Release(void)
    {
      Blocker.Unblock();
    }
    bso::sBool Wait(void)
    {
      Blocker.Wait(tht::bbDismiss);

      return !Error;
    }
  };


	class gBlocker {
	private:
		qRMV( rXBlocker_, B_, Blocker_ );
	protected:
		bso::sBool IsBlocked_;
		void Init(rXBlocker_ &Blocker)
		{
			Blocker_ = &Blocker;
			B_().Blocker.SetIsBlockedFlag(&IsBlocked_);
		}
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Blocker_ );
			IsBlocked_ = false;
		}
		qCDTOR( gBlocker );
		void Release( void )
		{
			B_().Release();
		}
		tht::rBlocker &Blocker(void) const
		{
		  return B_().Blocker;
		}
	};

	// 'Blocker' protects data in 'UP' from being deleted before it 'Release()' method will be called.
	typedef void (* sXRoutine)(void *UP, gBlocker &Blocker );

	void LaunchAndKill(
		sXRoutine Routine,
		void *UP );

	void LaunchAndKeep(
		sXRoutine Routine,
		void *UP );

	inline void Launch(
		sXRoutine Routine,
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

	/* Launch in the same thread, does return only when 'Routine' returns.
	Useful for testing */
	void SyncLaunch(
		sXRoutine Routine,
		void *UP );

	template <typename type> struct sData_
	{
	public:
		void( *Routine )(type &UP, gBlocker &Blocker);
		type *UP;
	};

	template <typename type> inline void sRoutine_(
		void *UP,
		gBlocker &Blocker)
	{
		sData_<type> &Data = *(sData_<type> *)UP;

		Data.Routine( *Data.UP, Blocker );
	}

	template <typename type> inline void LaunchAndKill(
		void( *Routine )(type &UP, gBlocker &Blocker),
		type &UserObject )
	{
		sData_<type> Data;

		Data.Routine = Routine;
		Data.UP = &UserObject;

		LaunchAndKill( sRoutine_<type>, &Data );
	}

	template <typename type> inline void LaunchAndKeep(
		void( *Routine )(type &UP, gBlocker &Blocker),
		type &UserObject )
	{
		sData_<type> Data;

		Data.Routine = Routine;
		Data.UP = &UserObject;

		LaunchAndKeep( sRoutine_<type>, &Data );
	}

	template <typename type> inline void Launch(
		void( *Routine )(type &UP, gBlocker &Blocker),
		type &UserObject )
	{
		sData_<type> Data;

		Data.Routine = Routine;
		Data.UP = &UserObject;

		Launch( sRoutine_<type>, &Data );
	}


}

#endif
