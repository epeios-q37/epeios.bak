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

#define SCLERROR__COMPILATION

#include "sclerror.h"

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
	row__ Row = qNIL;

	Lock_();

	Row = SCLERRORError->TIds.Search( tht::GetTID() );

	Unlock_();

	return Row;
}

static inline row__  SearchOrCreate_( void )
{
	row__ Row = Search_();

	if ( Row == qNIL ) {
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

	Lock_();

	IsEmpty = SCLERRORError->Meanings(Row).IsEmpty();

	Unlock_();

	return IsEmpty;
}

bso::bool__ sclerror::IsErrorPending( void )
{
	row__ Row = Search_();

	if ( Row == qNIL )
		return false;
	else
		return !IsMeaningEmpty_( Row );
}

const lcl::meaning_ &sclerror::GetMeaning( lcl::meaning_ &Meaning )
{
	if ( !IsErrorPending() )
		qRFwk();

	row__ Row = Search_();

	Lock_();

	Meaning = SCLERRORError->Meanings( Row );

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
		qRFwk();

	row__ Row = SearchOrCreate_();

	if ( !IsMeaningEmpty_( Row ) )
		qRFwk();

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
qRH
	lcl::meaning Meaning;
qRB
	if ( IsErrorPending() )
		PendingErrorAvailable = true;

	if ( PendingErrorAvailable ) {
		Meaning.Init();
		Translation.Init();
		scllocale::GetTranslation( GetMeaning( Meaning ), Language, Translation );
	} else if ( ErrHandling != err::hUserDefined )
		qRFwk();
qRR
qRT
qRE
	return PendingErrorAvailable;
}
