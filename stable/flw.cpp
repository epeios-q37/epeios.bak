/*
	'flw' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'flw' header file ('flw.h').
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



//	$Id: flw.cpp,v 1.93 2013/05/02 08:12:41 csimon Exp $

#define FLW__COMPILATION

#include "flw.h"

class flwtutor
: public ttr_tutor
{
public:
	flwtutor( void )
	: ttr_tutor( FLW_NAME )
	{
#ifdef FLW_DBG
		Version = FLW_VERSION "\b\bD $";
#else
		Version = FLW_VERSION;
#endif
		Owner = FLW_OWNER;
		Date = "$Date: 2013/05/02 08:12:41 $";
	}
	virtual ~flwtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#ifdef FLW__IGNORE_SIGPIPE
#	include <signal.h>
#endif

using namespace flw;

void flw::oflow__::_Write(
	const datum__ *Buffer,
	size__ Amount )
{
	size__ AmountWritten = _WriteUpTo( Buffer, Amount );

	while( AmountWritten < Amount )
		AmountWritten += _WriteUpTo( Buffer + AmountWritten, Amount - AmountWritten );
}

bool flw::GetString(
	iflow__ &Flot,
	char *Tampon,
	size__ NombreMax )
{
	size__ Position = 0;

	while( ( Position < NombreMax ) && ( ( Tampon[Position] = (char)Flot.Get() ) != 0 ) )
		Position++;

	return Position < NombreMax;
}

# if 0
size__ flw::iflow__::_RawRead(
	size__ Minimum,
	datum__ *Buffer,
	size__ Wanted,
	bso::bool__ Adjust,
	bso::bool__ &CacheIsEmpty )
{
	size__ Amount = 0;
ERRProlog
ERRBegin

#ifdef FLW_DBG
	if ( Wanted < Minimum )
		ERRPrm();

	if ( Minimum < 1 )
		ERRPrm();
#endif

	Amount = _LoopingRawRead( Minimum, Buffer, Wanted, Adjust, CacheIsEmpty );

	if ( Adjust )
		_Red += Amount;

	if ( _Red > _AmountMax )
		ERRFwk();
ERRErr
ERREnd
ERREpilog
	return Amount;
}
# endif

size__ flw::oflow__::_DirectWrite(
	const datum__ *Buffer,
	size__ Wanted,
	size__ Minimum )
{
	size__ Amount = 0;
ERRProlog
ERRBegin
	if ( ( _Size == 0 ) && ( _Cache != NULL ) )	// There was an error before. See below, in 'ERRErr'.
		ERRFwk();

#ifdef FLW_DBG
	if ( Wanted < 1 )
		ERRPrm();

	if ( Wanted < Minimum )
		ERRPrm();
#endif

	Amount = _LoopingWrite( Buffer, Wanted, Minimum );

	if ( Amount == 0 )
		ERRFwk();

#ifdef FLW_DBG
		if ( Amount > Wanted )
			ERRFwk();

		if ( Amount < Minimum )
			ERRFwk();
#endif

	_Written += Amount;

	if ( _Written >= _AmountMax )
		ERRFwk();
ERRErr
	_Size = _Free = 0;	// Pour éviter toute nouvelle écriture dans le cache. La prochaine tentative génèrera une erreur.
	Commit();	// N'écrit rien (à priori) ; juste pour déverouiiler.
ERREnd
ERREpilog
	return Amount;
}

#if 0
void flw::Copy(
	iflow__ &IFlow,
	size__ Amount,
	oflow__ &OFlow )
{
	bso::size__ Size = 4096;
	datum__ Buffer[4096];

	while ( Amount ) {
		if ( Amount < Size )
			Size = Amount;

		IFlow.Read( Size, Buffer );
		OFlow.Write( Buffer, Size );

		Amount -= Size;
	}
}
#endif


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class flwpersonnalization
: public flwtutor
{
public:
	flwpersonnalization( void )
	{
#ifdef FLW__IGNORE_SIGPIPE
//		signal( SIGPIPE, SIG_IGN );	// Witout this, an 'broken pipe' would terminate the program.
#endif	
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~flwpersonnalization( void )
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

static flwpersonnalization Tutor;

ttr_tutor &FLWTutor = Tutor;
