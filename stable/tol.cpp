/*
	'tol.cpp' by Claude SIMON (http://zeusw.org/).

	'tol' is part of the Epeios framework.

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

#define TOL__COMPILATION

#include "tol.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "fil.h"

#include "str.h"

using namespace tol;

/*
	NOTA : Compilé avec VC++ 2010, lorsque lancé dans une console DOS, 'localtime()' retourne le bon résultat,
	MAIS, lorsque lancé dans une console 'Cygwin' (1.17.10), le résultat est incorrect (différence d'une heure, peut-être lié à l'heure d'hiver/d'été).
*/

#ifdef TOL__MAC
	uint64_t tol::_Numer = 0;
	uint32_t tol::_Denom = 0;
#endif

static xbool__ SystemCommandIsAvailable_ = xb_Undefined;

/*
int system( const char * )
{
	ERRFbd();	// Ppour forcer l'utilisation de 'tol::System(...)'.

	return 0;	// Pour éviter un 'warning'.
}
*/

inline bso::bool__ tol::IsSystemCommandAvailable( void )
{
	switch ( *SystemCommandIsAvailable_ ) {
	case xbTrue:
		return true;
		break;
	case xbFalse:
		return false;
		break;
	case xb_Undefined:
		ERRFwk();
		break;
	default:
		ERRFwk();
		break;
	}

	return false;	// Pour éviter un 'warnong'.
}

#undef system

int tol::System( const ntvstr::string___ &Command )
{
	if ( Command.IsEmpty() )
		return system( NULL );

	if ( !IsSystemCommandAvailable() )
		ERRSys();

	int Result = EXIT_FAILURE;

#ifdef TOL__WIN
ERRProlog
	str::string ModifiedCommand;
	TOL_CBUFFER___ Buffer;
ERRBegin
	// '_wsystem()' lance en fait "cmd /c ...". Or; lorsque cette commande reçoit un paramètre commençant par '"' (hors espaces),
	// et avec plus d'un jeu de '"', elle en enlève certains (void "cmd /?"). Placer 'echo >NUL && ' en tête de paramètre résoud ce problème...
	ModifiedCommand.Init("echo >NUL && ");
	ModifiedCommand.Append( Command.UTF8( Buffer ) );

	Result = _wsystem( ntvstr::string___( ModifiedCommand ).Internal() );
ERRErr
ERREnd
ERREpilog
#elif defined( TOL__POSIX ) || defined( TOL__MAC )
	Result = system( Command.Internal() );
#else
# error
#endif

	if ( Result == -1 )
		ERRSys();

	return Result;


}

#ifdef TOL__WIN		
LARGE_INTEGER	tol::_TickFrequence;
#else
static const char *PosixCoreDateAndTime_(
	const char *Format,
	buffer__ &Buffer )
{
   struct tm *time_now;
   time_t secs_now;

   time(&secs_now);
   time_now = localtime(&secs_now);

   if ( !strftime( Buffer, sizeof( Buffer ), Format, time_now) )
	ERRLbr();

   return Buffer;
}
#endif

const char *tol::Date( buffer__ &Buffer )
{
#ifdef TOL__WIN
	SYSTEMTIME LocalTime;

	GetLocalTime( &LocalTime );

	sprintf( Buffer, "%02d/%02d/%04d", LocalTime.wDay, LocalTime.wMonth, LocalTime.wYear );

	return Buffer;
#else
	return PosixCoreDateAndTime_( "%d/%m/%Y", Buffer );
#endif
}

const char *tol::Time( buffer__ &Buffer )
{
#ifdef TOL__WIN
	SYSTEMTIME LocalTime;

	GetLocalTime( &LocalTime );

	sprintf( Buffer, "%02d:%02d:%02d", LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond );

	return Buffer;
#else
	return PosixCoreDateAndTime_( "%H:%M:%S", Buffer );
#endif
}

const char *tol::DateAndTime( buffer__ &Buffer )
{
#ifdef TOL__WIN
	SYSTEMTIME LocalTime;

	GetLocalTime( &LocalTime );

	sprintf( Buffer, "%02d/%02d/%04d %02d:%02d:%02d", LocalTime.wDay, LocalTime.wMonth, LocalTime.wYear, LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond );

	return Buffer;
#else
	return PosixCoreDateAndTime_( "%d/%m/%Y %H:%M:%S", Buffer );
#endif
}

#ifdef TOL__WIN

#include <crtdbg.h>

bso::size__ tol::GetMemoryUsage( void )
{
	_CrtMemState s;
	_CrtMemCheckpoint( &s );

	return s.lSizes[1];
}

#endif

#ifdef CPE__VC
#	undef CreateFile
#endif

static inline void signal_( int s )
{
	exit( EXIT_SUCCESS );
}

static inline void ExitOnSignal_( void )
{
#if defined( TOL__MAC ) || defined( TOL__POSIX )
	signal( SIGHUP, signal_ );
#elif defined( TOL__WIN )
	signal( SIGBREAK, signal_ );
#else
#	error "Undefined target !"
#endif

	signal( SIGTERM, signal_ );
	signal( SIGABRT, signal_ );
	signal( SIGINT, signal_ );	// Documentations about this signal not very clear, but this handles Ctrl-C.
}

static inline void SetSystemCommandAvailabitity_( void )
{
	if ( system( NULL ) != 0 )
		SystemCommandIsAvailable_ = xbTrue;
	else
		SystemCommandIsAvailable_ = xbFalse;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class tolpersonnalization
{
public:
	tolpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
			ExitOnSignal_();
			SetSystemCommandAvailabitity_();
#ifdef TOL__WIN		
		if ( QueryPerformanceFrequency( &tol::_TickFrequence ) == 0 )
			ERRSys();
#elif defined( TOL__MAC )
		mach_timebase_info_data_t    TimebaseInfo;

		mach_timebase_info(&TimebaseInfo);

#if 0
		// Test toujours faux, dû au type de 'TimebaseInfo.numer'
		if ( ( BSO_NUINT_MAX / 1000000 ) < TimebaseInfo.numer )
			ERRLmt();
#endif
		tol::_Numer = TimebaseInfo.numer;
		tol::_Denom = TimebaseInfo.denom;
#endif
}
	~tolpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static tolpersonnalization Tutor;
