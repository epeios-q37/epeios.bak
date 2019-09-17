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

#define TOL_COMPILATION_

#include "tol.h"

#include "fil.h"
#include "str.h"
#include "tht.h"

using namespace tol;

/*
	NOTA : Compil avec VC++ 2010, lorsque lanc dans une console DOS, 'localtime()' retourne le bon rsultat,
	MAIS, lorsque lanc dans une console 'Cygwin' (1.17.10), le rsultat est incorrect (diffrence d'une heure, peut-tre li  l'heure d'hiver/d't).
*/

#ifdef TOL__DARWIN
	uint64_t tol::_Numer = 0;
	uint32_t tol::_Denom = 0;
#endif

static xbool__ SystemCommandIsAvailable_ = xb_Undefined;

/*
int system( const char * )
{
	qRFbd();	// Ppour forcer l'utilisation de 'tol::System(...)'.

	return 0;	// Pour viter un 'warning'.
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
		qRFwk();
		break;
	default:
		qRFwk();
		break;
	}

	return false;	// Pour viter un 'warnong'.
}

#undef system

int tol::System( const ntvstr::string___ &Command )
{
	if ( Command.IsEmpty() )
		return system( NULL );

	if ( !IsSystemCommandAvailable() )
		qRSys();

	int Result = EXIT_FAILURE;

#ifdef TOL__WIN
qRH
	str::string ModifiedCommand;
	TOL_CBUFFER___ Buffer;
qRB
	// '_wsystem()' lance en fait "cmd /c ...". Or; lorsque cette commande reoit un paramtre commenant par '"' (hors espaces),
	// et avec plus d'un jeu de '"', elle en enlve certains (void "cmd /?"). Placer 'echo >NUL && ' en tte de paramtre rsoud ce problme...
	ModifiedCommand.Init("echo >NUL && ");
	ModifiedCommand.Append( Command.UTF8( Buffer ) );

	Result = _wsystem( ntvstr::string___( ModifiedCommand ).Internal() );
qRR
qRT
qRE
#elif defined( TOL__POSIX ) || defined( TOL__DARWIN)
	Result = system( Command.Internal() );
#else
# error
#endif

	if ( Result == -1 )
		qRSys();

	return Result;
}

#if defined ( CPE_S_LINUX )
# define LAUNCH_COMMAND	"xdg-open"
#elif defined( CPE_S_DARWIN )
# define LAUNCH_COMMAND	"open"
#elif defined( CPE_S_WIN ) || defined( CPE_S_CYGWIN )
# define LAUNCH_COMMAND	"start \"\"" // Les "" dans la commande sont ncessit par 'start', car si l'argument est pass entre "", 'start' considre cela comme le titre de la fentre.
#endif

void tol::Launch( const ntvstr::string___ &Document )
{
qRH
	str::string Command, Buffer;
qRB
#ifndef LAUNCH_COMMAND
	qRFwk();
#else
	Command.Init( LAUNCH_COMMAND " \"" );
	
	Buffer.Init();
	Command.Append( Document.UTF8( Buffer ) );

	Command.Append( '"' );

	System( Command );
#endif
qRR
qRT
qRE
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
	qRLbr();

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

#ifdef CPE_C_MSC
#	undef CreateFile
#endif

static inline void SetSystemCommandAvailabitity_( void )
{
	if ( system( NULL ) != 0 )
		SystemCommandIsAvailable_ = xbTrue;
	else
		SystemCommandIsAvailable_ = xbFalse;
}

const char *tol::UUIDGen( bUUID &UUID )
{
	InitializeRandomGenerator();

	// http://stackoverflow.com/questions/2174768/generating-random-uuids-in-linux
	// Modified, because, with the original, the size of the UUID can vary.

	sprintf(UUID, "%04x%04x-%04x-%04x-%04x-%04x%04x%04x", 
		rand()&0xffff, rand()&0xffff,
		rand()&0xffff,
		((rand() & 0x0fff) | 0x4000),
		rand() % 0x3fff + 0x8000,
		rand()&0xffff, rand()&0xffff, rand()&0xffff);

	return UUID;
}

namespace {
	void Crash_( str::dString )
	{
		Crash();
	}
}

// Crash the program by stack overflow.
void tol::Crash( void )
{
qRH
	str::wString Dummy;
qRB
	Crash_( Dummy );
qRR
qRT
qRE
}

namespace {
	tht::rLocker EnvLocker_;

	bso::sBool GetEnv_(
		const ntvstr::sChar *Name,
		ntvstr::rString &Value )
	{
		ntvstr::sChar *Result = NULL;

		EnvLocker_.Lock();

#ifdef CPE_S_WIN 
		Result = _wgetenv( Name );
#elif defined( CPE_S_POSIX )
		Result = getenv( Name );
#else
#  error
#endif

		if ( Result != NULL )
			Value.Init( Result );

		EnvLocker_.Unlock();

		return Result != NULL;
	}
}

bso::sBool tol::GetEnv(
	const str::dString &Name,
	str::dString &Value )
{
	bso::sBool Found = false;
qRH
	ntvstr::rString NativeName, NativeValue;
qRB
	NativeName.Init( Name );
	NativeValue.Init();

	if ( GetEnv_( NativeName.Internal(), NativeValue ) ) {
		NativeValue.UTF8( Value );
		Found = true;
	}
qRR
qRT
qRE
	return Found;
}

Q37_GCTOR( tol )
{
	SetSystemCommandAvailabitity_();
	EnvLocker_.Init();
#ifdef TOL__WIN		
	if ( QueryPerformanceFrequency( &tol::_TickFrequence ) == 0 )
		qRSys();
#elif defined( TOL__DARWIN)
	mach_timebase_info_data_t    TimebaseInfo;

	mach_timebase_info(&TimebaseInfo);
#if 0
	// Test toujours faux, d au type de 'TimebaseInfo.numer'
	if ( ( BSO_NUINT_MAX / 1000000 ) < TimebaseInfo.numer )
		qRLmt();
#endif
	tol::_Numer = TimebaseInfo.numer;
	tol::_Denom = TimebaseInfo.denom;
#endif
}
