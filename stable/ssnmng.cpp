/*
	'ssnmng' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'ssnmng' header file ('ssnmng.h').
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

#define SSNMNG__COMPILATION

#include "ssnmng.h"

#define DIGITS "azertyuiopmlkjhgfdsqwxcvbnNBVCXWQSDFGHJKLMPOIUYTREZA9876543210"

using namespace ssnmng;

void ssnmng::session_id__::New( void )
{
	int i;

	for( i = 0; i < SSNMNG_SIZE; i++ )
		Raw_[i] = DIGITS[rand() % ( sizeof( DIGITS ) - 1 )];

	Raw_[i] = 0;
}

static inline bso::sign__ Test_(
	const char *S1,
	const char *S2 )
{
	return strcmp( S1, S2 );
}

static bso::sign__ Search_(
	const bch::E_BUNCHt_( session_id__, row__ ) &T,
	const char *S,
	idxbtq::E_ISEEKERt__( row__ ) &Seeker )
{
	bso::sign__ Test;
	row__ Row = Seeker.GetCurrent();

	while ( ( Row != E_NIL )
			&& ( ( Test = Test_( T( Row ).Value(), S ) ) != 0 ) ) {
		switch( Test ) {
		case 1:
			Row = Seeker.SearchGreater();
			break;
		case -1:
			Row = Seeker.SearchLesser();
			break;
		default:
			ERRFwk();
			break;
		}
	}

	return Test;
}

row__ ssnmng::sessions_manager_::New( void *UP )
{
	row__ Row = Pointers.Add( UP );
	session_id__ SessionID;
	chrono__ C;

	_AdjustSizes();

	do {
		SessionID.New();
	} while( Search( SessionID ) != E_NIL );

	Table.Store( SessionID, Row );

	if ( S_.Root == E_NIL ) {
		S_.Root = Row;
		Queue.Create( Row );
	} else {
		idxbtq::E_ISEEKERt__( row__ ) Seeker;

		Seeker.Init( Index, S_.Root );

		switch ( Search_( Table, SessionID.Value(), Seeker ) ) {
		case 1:
			S_.Root = Index.BecomeGreater( Row, Seeker.GetCurrent(), S_.Root );
			break;
		case -1:
			S_.Root = Index.BecomeLesser( Row, Seeker.GetCurrent(), S_.Root );
			break;
		default:
			ERRFwk();
			break;
		}

		Queue.BecomeNext( Row, Queue.Tail() );
	}


	if ( time( &C.Relative ) == -1 )
		ERRLbr();

	if ( time( &C.Absolute ) == -1 )
		ERRLbr();

	C.Immortal = false;

	Chronos.Store( C, Row );

	return Row;
}

row__ ssnmng::sessions_manager_::Search( const char *SessionID ) const
{
	if ( S_.Root != E_NIL )	{
		idxbtq::E_ISEEKERt__( row__ ) Seeker;

		Seeker.Init( Index, S_.Root );

		if ( Search_( Table, SessionID, Seeker ) == 0 )
			return Seeker.GetCurrent();
		else
			return E_NIL;
	}

	return E_NIL;
}

row__ ssnmng::sessions_manager_::Search( const str::string_ &SessionID ) const
{
	char Buffer[SSNMNG_SIZE+1];

	if ( SessionID.Amount() != SSNMNG_SIZE )
		return E_NIL;

	SessionID.Recall( 0, SSNMNG_SIZE, Buffer );

	Buffer[SSNMNG_SIZE] = 0;

	return Search( Buffer );
}

void ssnmng::sessions_manager_::GetExpired( rows_ &Expired ) const
{
	row__ Row = First();

	while ( Row != E_NIL ) {
		if ( IsExpired( Row ) )
			Expired.Append( Row );

		Row = Next( Row );
	}
}

void ssnmng::sessions_manager_::GetAll( rows_ &Rows ) const
{
	row__ Row = First();

	while ( Row != E_NIL ) {
		Rows.Append( Row );

		Row = Next( Row );
	}
}

void ssnmng::sessions_manager_::_Close( const rows_ &Rows )
{
	sdr::row__ Row = Rows.First();

	while ( Row != E_NIL ) {
		Close( Rows( Row ) );

		Row = Rows.Next( Row );
	}
}


void ssnmng::sessions_manager_::CloseAll( void )
{
ERRProlog
	rows Rows;
ERRBegin
	Rows.Init();

	GetAll( Rows );

	_Close( Rows );
ERRErr
ERREnd
ERREpilog
}

void ssnmng::sessions_manager_::CloseExpired( void )
{
ERRProlog
	rows Rows;
ERRBegin
	Rows.Init();

	GetExpired( Rows );

	_Close( Rows );
ERRErr
ERREnd
ERREpilog
}

