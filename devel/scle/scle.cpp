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

#define SCLE_COMPILATION_

#include "scle.h"

#include "scll.h"

using namespace scle;

static inline void Lock_( void )
{
	mtx::Lock( SCLEError->Mutex );
}

static inline void Unlock_( void )
{
	mtx::Unlock( SCLEError->Mutex );
}

static inline row__  Search_( void )
{
	row__ Row = qNIL;

	Lock_();

	Row = SCLEError->TIds.Search( tht::GetTID() );

	Unlock_();

	return Row;
}

static inline row__  SearchOrCreate_( void )
{
	row__ Row = Search_();

	if ( Row == qNIL ) {
		Lock_();

		Row = SCLEError->TIds.Add( tht::GetTID() );

		SCLEError->Meanings.Allocate( SCLEError->TIds.Extent() );
		SCLEError->Meanings( Row ).Init();
		SCLEError->Meanings.Flush();

		Unlock_();
	}

	return Row;
}

static inline bso::bool__ IsMeaningEmpty_( row__ Row )
{
	bso::bool__ IsEmpty = false;

	Lock_();

	IsEmpty = SCLEError->Meanings(Row).IsEmpty();

	Unlock_();

	return IsEmpty;
}

bso::bool__ scle::IsErrorPending( void )
{
	row__ Row = Search_();

	if ( Row == qNIL )
		return false;
	else
		return !IsMeaningEmpty_( Row );
}

const lcl::meaning_ &scle::GetMeaning( lcl::meaning_ &Meaning )
{
	if ( !IsErrorPending() )
		qRFwk();

	row__ Row = Search_();

	Lock_();

	Meaning = SCLEError->Meanings( Row );

	Unlock_();

	return Meaning;
}

void scle::ResetPendingError( void )
{
	row__ Row = SearchOrCreate_();

	Lock_();

	SCLEError->Meanings( Row ).Init();
	SCLEError->Meanings.Flush();

	SCLEError->TIds.Delete( Row );

	Unlock_();
}

void scle::SetMeaning( const lcl::meaning_ &Meaning )
{
	if ( IsErrorPending() )
		qRFwk();

	row__ Row = SearchOrCreate_();

	if ( !IsMeaningEmpty_( Row ) )
		qRFwk();

	Lock_();

	SCLEError->Meanings( Row ) = Meaning;

	SCLEError->Meanings.Flush();

	Unlock_();
}

bso::bool__ scle::GetPendingErrorTranslation(
	const char *Language,
	str::string_ &Translation,
	err::handling__ ErrHandling )
{
	bso::bool__ PendingErrorAvailable = false;
qRH
	lcl::meaning Meaning;
qRB
	if ( IsErrorPending() )
		PendingErrorAvailable = true;

	if ( PendingErrorAvailable ) {
		Meaning.Init();
		Translation.Init();
		scll::GetTranslation( GetMeaning( Meaning ), Language, Translation );
	} else if ( ErrHandling != err::hUserDefined )
		qRFwk();
qRR
qRT
qRE
	return PendingErrorAvailable;
}
