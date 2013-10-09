/*
	'tol' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'tol' header file ('tol.h').
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: tol.cpp,v 1.50 2013/04/06 14:50:42 csimon Exp $

#define TOL__COMPILATION

#include "tol.h"

class toltutor
: public ttr_tutor
{
public:
	toltutor( void )
	: ttr_tutor( TOL_NAME )
	{
#ifdef TOL_DBG
		Version = TOL_VERSION "\b\bD $";
#else
		Version = TOL_VERSION;
#endif
		Owner = TOL_OWNER;
		Date = "$Date: 2013/04/06 14:50:42 $";
	}
	virtual ~toltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/


#include "fil.h"

using namespace tol;

/*
	NOTA : Compilé avec VC++ 2010, lorsque lancé dans une console DOS, 'localtime()' retourne le bon résultat,
	MAIS, lorsque lancé dans une console 'Cygwin' (1.17.10), le résultat est incorrect (différence d'une heure, peut-être lié à l'heure d'hiver/d'été).
*/

#ifdef TOL__MAC
	uint64_t tol::_Numer = 0;
	uint32_t tol::_Denom = 0;
#endif

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

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class tolpersonnalization
: public toltutor
{
public:
	tolpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
		ExitOnSignal_();
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


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static tolpersonnalization Tutor;

ttr_tutor &TOLTutor = Tutor;
