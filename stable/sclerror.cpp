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

#include "scllocale.h"

using namespace sclerror;

#ifdef CPE_MT
# define MT
#endif

#ifdef MT
# include "tht.h"
# include "mtx.h"

static  mtx::handler___ Mutex_ = mtx::UndefinedHandler;

typedef tht::thread_id__ tid__;

E_ROW( row__ );

static lstbch::E_LBUNCHt( tid__, row__ ) TIds_;
static ctn::E_CONTAINERt( lcl::meaning_, row__ ) Meanings_;

static inline void Lock_( void )
{
	mtx::Lock( Mutex_ );
}

static inline void Unlock_( void )
{
	mtx::Unlock( Mutex_ );
}

static inline row__  Search_( void )
{
	row__ Row = E_NIL;

	Lock_();

	Row = TIds_.Search( tht::GetTID() );

	Unlock_();

	return Row;
}

static inline row__  SearchOrCreate_( void )
{
	row__ Row = Search_();

	if ( Row == E_NIL ) {
		Lock_();

		Row = TIds_.Add( tht::GetTID() );

		Meanings_.Allocate( TIds_.Extent() );
		Meanings_( Row ).Init();
		Meanings_.Flush();

		Unlock_();
	}

	return Row;
}

static inline bso::bool__ IsMeaningEmpty_( row__ Row )
{
	bso::bool__ IsEmpty = false;
	ctn::E_CITEMt( lcl::meaning_, row__ ) Meaning;

	Lock_();

	Meaning.Init( Meanings_ );

	IsEmpty = Meaning(Row).IsEmpty();

	Unlock_();

	return IsEmpty;
}

#else
static lcl::meaning Meaning_;
#endif


bso::bool__ sclerror::IsErrorPending( void )
{
#ifdef MT
	row__ Row = Search_();

	if ( Row == E_NIL )
		return false;
	else
		return !IsMeaningEmpty_( Row );
#else
	return !Meaning_.IsEmpty();
#endif
}

const lcl::meaning_ &sclerror::GetMeaning( lcl::meaning_ &Meaning )
{
	if ( !IsErrorPending() )
		ERRFwk();
#ifdef MT 
	row__ Row = Search_();
	ctn::E_CITEMt( lcl::meaning_, row__ ) MeaningBuffer;

	Lock_();

	MeaningBuffer.Init( Meanings_ );

	Meaning = MeaningBuffer( Row );

	Unlock_();
#else
	Meaning = Meaning_;
#endif
	return Meaning;
}

void sclerror::ResetPendingError( void )
{
#ifdef MT
	row__ Row = SearchOrCreate_();

	Lock_();

	Meanings_( Row ).Init();
	Meanings_.Flush();

	TIds_.Delete( Row );

	Unlock_();
#else
	Meaning_.Init();
#endif
}

void sclerror::SetMeaning( const lcl::meaning_ &Meaning )
{
	if ( IsErrorPending() )
		ERRFwk();
# ifdef MT
	row__ Row = SearchOrCreate_();

	if ( !IsMeaningEmpty_( Row ) )
		ERRFwk();

	Lock_();

	Meanings_( Row ) = Meaning;

	Meanings_.Flush();

	Unlock_();
#else
	if ( Meaning.IsEmpty() )
		ERRFwk();

	Meaning_ = Meaning;
#endif
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
#ifdef MT
		Mutex_ = mtx::Create();

		TIds_.Init();
		Meanings_.Init();
#else
		Meaning_.Init();
#endif
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sclerrorpersonnalization( void )
	{
#ifdef MT
		if ( Mutex_ != mtx::UndefinedHandler )
			mtx::Delete( Mutex_ );
#else
#endif
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
