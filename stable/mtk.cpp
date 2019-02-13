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

#define MTK__COMPILATION

# include "mtk.h"

# include "mtx.h"

using namespace mtk;

# ifdef MTK__POSIX

namespace {
	struct thread_struct__
	{
		mtk__routine R;
		void *UP;
		mtx::handler___ MH;
		thread_struct__( void )
		{
			R = NULL;
			UP = NULL;
			MH = MTX_INVALID_HANDLER;
		}
	};
};

#	if 0	// Was for BeOS. No more handled.

#	include <be/kernel/scheduler.h>

static int32 ThreadFunction_( void *D )
{
qRH
	mtk__thread_struct TS = *((mtk__thread_struct *)D);
qRB
	mtx::Unlock( TS.MH );
	TS.R( TS.UP );
qRR
qRT
qRE
	return 0;
}

static void LaunchThread_(
	mtk__routine Routine,
	void *UP )
{
qRH
	thread_id ID;
	mtk__thread_struct TS;
	sph__semaphore_ S;
qRB
	TS.R = Routine;
	TS.UP = UP;
	TS.SD = SPHCreate();

	S.Init( TS.SD );

	S.Lock();	// Unlocked by 'ThreadFunction'.
	
	if ( ( ID = spawn_thread( ThreadFunction_, NULL, suggest_thread_priority(), &TS ) ) < B_OK )
		qRSys();
		
	if ( resume_thread( ID ) != B_OK )
		qRSys();

	S.Lock();
	S.Unlock();

qRR
qRT
	if ( TS.SD != SPH_INVALID )
		SPHDelete( TS.SD );
qRE
}

#	else

static void *ThreadFunction_( void *D )
{
qRH
	thread_struct__ TS = *((thread_struct__ *)D);
qRB
	mtx::Unlock( TS.MH );
	TS.R( TS.UP );
#ifdef MTK__THREADS_REMAIN
	for(;;)
		TOLYield();
#endif
qRR
qRT
qRE
	return NULL;
}

static void LaunchThread_(
	mtk__routine Routine,
	void *UP )
{
qRH
	pthread_t TID;
	thread_struct__ TS;
qRB
	TS.R = Routine;
	TS.UP = UP;
	TS.MH = mtx::Create();
	
	mtx::Lock( TS.MH );	// Unlocked by 'ThreadFunction'.

	switch ( pthread_create( &TID, NULL, ThreadFunction_, &TS ) ) {
	case 0:
		break;
	case EAGAIN:
		qRSys();
		break;
	default:
		qRSys();
		break;
	}

	mtx::Lock( TS.MH );
	mtx::Unlock( TS.MH );

qRR
qRT
	if ( TS.MH != MTX_INVALID_HANDLER )
		mtx::Delete( TS.MH );
qRE
}
#	endif

#endif

void mtk::RawLaunchAndKill(
	mtk__routine Routine,
	void *UP )
{
qRH
qRB
#ifdef MTK__WIN
	if ( _beginthread( Routine, 0, UP ) == (unsigned long)-1 )
		qRSys();
#elif defined( MTK__POSIX )
	LaunchThread_( Routine, UP );
#else
#	error
#endif
qRR
qRT
qRE
}

namespace {
	struct common__ {
		// Lock to ensure that data was red before reusing them.
		mtx::handler___ Data;
		// Lock for new thread to block caller.
		mtx::handler___ Thread;
		// Lock for thread launcher to block new created thread.
		mtx::handler___ Store;
		// Lock to handle concurrent calls to 'LauncAndKeep()'.
		mtx::handler___ Exclusion;
		bso::bool__ Continue;
		void *UP;
		bso::uint__ Amount;
		routine__ Routine;
	} Common;
	
	void Create_( void )
	{
	qRH
	qRB
		Common.Thread = Common.Data = Common.Store = Common.Exclusion = MTX_INVALID_HANDLER;

		Common.Continue = true;
		Common.UP = NULL;
		Common.Amount = 0;
		Common.Routine = NULL;
		Common.Data = mtx::Create();
		Common.Thread = mtx::Create();
		Common.Store = mtx::Create();
		Common.Exclusion = mtx::Create();

		mtx::Lock( Common.Thread );

		mtx::Lock( Common.Store );
	qRR
		if ( Common.Data != MTX_INVALID_HANDLER )
			mtx::Delete( Common.Data );

		if ( Common.Thread != MTX_INVALID_HANDLER )
			mtx::Delete( Common.Thread );

		if ( Common.Store != MTX_INVALID_HANDLER )
			mtx::Delete( Common.Store );
			
		if ( Common.Exclusion != MTX_INVALID_HANDLER )
			mtx::Delete( Common.Exclusion );
	qRT
	qRE
	}

	void Close_( void )
	{
		Common.Continue = false;

		mtx::Lock( Common.Data );

		while ( Common.Amount != 0 ){

			mtx::Unlock( Common.Data );

			mtx::Unlock( Common.Thread );

			mtx::Lock( Common.Store );

			mtx::Lock( Common.Data );		
		}
		
		mtx::Delete( Common.Data, true );
		mtx::Delete( Common.Thread, true );
		mtx::Delete( Common.Store, true );
		mtx::Delete( Common.Exclusion, true );
	}
	
	// Les 'ERR...' macros, bien que fonctionnelemy inutiles, facilite le dbogage.
	// Les exceptions devraient avoir t traites en aval.
	void Launcher_( void * )
	{
	qRH
		void *RUP;
		mtk::routine__ Routine;
	qRB
		mtx::Lock( Common.Data );	// To ensure that data are available.
	
		do {
			RUP = Common.UP;
			Routine = Common.Routine;
			
			mtx::Unlock( Common.Data );	// I have what I need.
			
			mtx::Unlock( Common.Store );// Calling function can continue.
			
			Routine( RUP );	// Launching user function.
			
			mtx::Lock( Common.Data );	// To ensure that nobody else is accessing data.
			
			if ( Common.Amount == BSO_UINT_MAX )
				qRLmt();
				
			Common.Amount++;// One more thread available.
			
			mtx::Unlock( Common.Data );	// I'm finished with the data.
			
			mtx::Lock( Common.Thread );	// Waiting for new work.
			
			mtx::Lock( Common.Data );	// To ensure that nobody else is accessing data.
			
			Common.Amount--;// One less thread available.
			
		} while ( Common.Continue );
		
		mtx::Unlock( Common.Data );
		
		mtx::Unlock( Common.Store );
		
	qRR
	qRT
	qRE
	}
}

void mtk::RawLaunchAndKeep(
	routine__ Routine,
	void *UP )
{
	mtx::Lock( Common.Exclusion );

	mtx::Lock( Common.Data );
	
	Common.Routine = Routine;
	Common.UP = UP;
	
	if ( Common.Amount == 0 )
		mtk::RawLaunchAndKill( Launcher_, NULL );
	else
		mtx::Unlock( Common.Thread );
		
	mtx::Unlock( Common.Data );
	
	mtx::Lock( Common.Store );
	
	mtx::Unlock( Common.Exclusion );
}

namespace {
	struct sData_ {
		sXRoutine Routine;
		void *UP;
		tht::rBlocker *Blocker;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Routine, UP, Blocker );
		}
		qCDTOR( sData_ )
	};

	class gBlocker_
	: public gBlocker
	{
	public:
		void Init( tht::rBlocker &Blocker )
		{
			gBlocker::Init( Blocker );
		}
	};

	void Routine_( void *UP )
	{
		sData_ &Data = *(sData_ *)UP;
		gBlocker_ Blocker;

		Blocker.Init( *Data.Blocker );

		Data.Routine( Data.UP, Blocker );

		// 'Blocker.Blocker_' is no more valid here !!!
	}

	void Launch_(
		sXRoutine Routine,
		void(* Launch)( routine__, void *),
		void *UP )
	{
	qRH
		tht::rBlocker Blocker;
		sData_ Data;
	qRB
		Blocker.Init();
		Data.Blocker = &Blocker;
		Data.Routine = Routine;
		Data.UP = UP;

		Launch( Routine_, &Data );

		Blocker.Wait();
	qRR
	qRT
	qRE
	}

}

void mtk::LaunchAndKill(
	sXRoutine Routine,
	void *UP )
{
	Launch_( Routine, mtk::RawLaunchAndKill, UP );
}

void mtk::LaunchAndKeep(
	sXRoutine Routine,
	void *UP )
{
	Launch_( Routine, mtk::RawLaunchAndKeep, UP );
}

# if 0	// Obsolete  ?
#  ifdef CPE_C_MSC
typedef unsigned long	intptr_t;
# endif
#endif


static void WaitAndExit_( void *UP )
{
	tht::Wait( (unsigned long)(intptr_t)UP );
	exit( EXIT_SUCCESS );
}

void mtk::ForceExit( unsigned long Seconds )
{
	RawLaunch( WaitAndExit_, (void *)(intptr_t)Seconds );
}

Q37_GCTOR( mtk )
{
	Create_();
}

Q37_GDTOR( mtk )
{
	Close_();
}
