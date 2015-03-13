/*
	'sclerror' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'sclerror' header file ('sclerror.h').
	Copyright (C) 20112004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
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



//	$Id: sclerror.cpp,v 1.6 2013/07/18 19:46:25 csimon Exp $

#define SCLERROR__COMPILATION

#include "sclerror.h"

class sclerrortutor
: public ttr_tutor
{
public:
	sclerrortutor( void )
	: ttr_tutor( SCLERROR_NAME )
	{
#ifdef SCLERROR_DBG
		Version = SCLERROR_VERSION "\b\bD $";
#else
		Version = SCLERROR_VERSION;
#endif
		Owner = SCLERROR_OWNER;
		Date = "$Date: 2013/07/18 19:46:25 $";
	}
	virtual ~sclerrortutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace sclerror;


static inline void Lock_( void )
{
	mtx::Lock( SCLERRORError->Mutex );
}

static inline void Unlock_( void )
{
	mtx::Unlock( SCLERRORError->Mutex );
}

static inline row__  Search_( void )
{
	row__ Row = E_NIL;

	Lock_();

	Row = SCLERRORError->TIds.Search( tht::GetTID() );

	Unlock_();

	return Row;
}

static inline row__  SearchOrCreate_( void )
{
	row__ Row = Search_();

	if ( Row == E_NIL ) {
		Lock_();

		Row = SCLERRORError->TIds.Add( tht::GetTID() );

		SCLERRORError->Meanings.Allocate( SCLERRORError->TIds.Extent() );
		SCLERRORError->Meanings( Row ).Init();
		SCLERRORError->Meanings.Flush();

		Unlock_();
	}

	return Row;
}

static inline bso::bool__ IsMeaningEmpty_( row__ Row )
{
	bso::bool__ IsEmpty = false;
	ctn::E_CITEMt( lcl::meaning_, row__ ) Meaning;

	Lock_();

	Meaning.Init( SCLERRORError-> Meanings );

	IsEmpty = Meaning(Row).IsEmpty();

	Unlock_();

	return IsEmpty;
}

bso::bool__ sclerror::IsErrorPending( void )
{
	row__ Row = Search_();

	if ( Row == E_NIL )
		return false;
	else
		return !IsMeaningEmpty_( Row );
}

const lcl::meaning_ &sclerror::GetMeaning( lcl::meaning_ &Meaning )
{
	if ( !IsErrorPending() )
		ERRFwk();

	row__ Row = Search_();
	ctn::E_CITEMt( lcl::meaning_, row__ ) MeaningBuffer;

	Lock_();

	MeaningBuffer.Init( SCLERRORError->Meanings );

	Meaning = MeaningBuffer( Row );

	Unlock_();

	return Meaning;
}

void sclerror::ResetPendingError( void )
{
	row__ Row = SearchOrCreate_();

	Lock_();

	SCLERRORError->Meanings( Row ).Init();
	SCLERRORError->Meanings.Flush();

	SCLERRORError->TIds.Delete( Row );

	Unlock_();
}

void sclerror::SetMeaning( const lcl::meaning_ &Meaning )
{
	if ( IsErrorPending() )
		ERRFwk();

	row__ Row = SearchOrCreate_();

	if ( !IsMeaningEmpty_( Row ) )
		ERRFwk();

	Lock_();

	SCLERRORError->Meanings( Row ) = Meaning;

	SCLERRORError->Meanings.Flush();

	Unlock_();
}

bso::bool__ sclerror::GetPendingErrorTranslation(
	const char *Language,
	str::string_ &Translation,
	err::handling__ ErrHandling )
{
	bso::bool__ PendingErrorAvailable = false;
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	if ( IsErrorPending() )
		PendingErrorAvailable = true;

	if ( PendingErrorAvailable ) {
		Meaning.Init();
		Translation.Init();
		scllocale::GetTranslation( GetMeaning( Meaning ), Language, Translation );
	} else if ( ErrHandling != err::hUserDefined )
		ERRFwk();
ERRErr
ERREnd
ERREpilog
	return PendingErrorAvailable;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class sclerrorpersonnalization
: public sclerrortutor
{
public:
	sclerrorpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sclerrorpersonnalization( void )
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

static sclerrorpersonnalization Tutor;

ttr_tutor &SCLERRORTutor = Tutor;
