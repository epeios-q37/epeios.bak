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

#define STSFSM__COMPILATION

#include "stsfsm.h"

#include "flx.h"

using namespace stsfsm;

id__ stsfsm::Add(
	const str::string_ &Tag,
	id__ Id,
	automat_ &Automat )
{
	sdr::row__ Row = Tag.First();
	crow__ Current = Automat.First(), Next = qNIL;

	if ( Current == qNIL ) {
		Current = Automat.New();
		Automat( Current ).Init();
	}


	while ( Row != qNIL ) {

		Next = Automat( Current ).Get( Tag( Row ) );

		Automat.Flush();

		if ( Next == qNIL ) {
			Next = Automat.New();
			Automat( Next ).Init();
			Automat( Current ).Set( Tag( Row ), Next );
			Automat.Flush();
		}

		Current = Next;

		Row = Tag.Next( Row );
	}

	if ( Current == qNIL )
		qRFwk();

	if ( Automat( Current ).GetId() != UndefinedId ) {
		Id = Automat( Current ).GetId();

		Automat.Flush();

		return Id;
	}

	Automat( Current ).SetId( Id );

	Automat.Flush();

	return UndefinedId;
}

status__ stsfsm::parser__::Handle( bso::u8__ C )
{
	status__ Status = s_Undefined;
	crow__ Next = qNIL;
	sCard_ Card;

	if ( Current_ == qNIL )
		Current_ = A_().First();

	if ( Current_ == qNIL ) {
		Status = sLost;	// L'automate est vide.
	} else {
		Card.Init( A_() );
		Next = Card(Current_).Get( C );

		if ( Next == qNIL ) {
			Status = sLost;
		} else {
			Current_ = Next;

			if ( Card(Current_).GetId() == UndefinedId )
				Status = sPending;
			else
				Status = sMatch;
		}
	}

	return Status;
}

status__ stsfsm::parser__::_Handle(
	const str::string_ &Pattern,
	sdr::row__ *LostPosition )
{
	status__ Status = s_Undefined;
	sdr::row__ Row = Pattern.First();

	while ( ( Row != qNIL ) && ( ( Status = Handle( Pattern( Row ) ) ) != sLost ) )
		Row = Pattern.Next( Row );

	if ( Row != qNIL ) {
		if ( Status != sLost )
			qRFwk();
		else if ( LostPosition != NULL )
			*LostPosition = Row;
	} else if ( Status == sLost )
		qRFwk();

	return Status;
}

id__ stsfsm::GetId(
	flw::iflow__ &Flow,
	const automat_ &Automat,
	eUnmatchingBehavior UnmatchingBehavior)
{
	id__ Id = UndefinedId;
qRH
	parser__ Parser;
	bso::bool__ Match = false;
	fdr::sByte Byte = 0;
	bso::sBool Continue = true;
qRB
	Parser.Init( Automat );

	Continue = !Flow.EndOfFlow() && ( ( Byte = Flow.Get() ) != 0 );

	while ( Continue ) {
		switch ( Parser.Handle( Byte ) ) {
		case sLost:
			Continue = false;
			Match = false;
			break;
		case sMatch:
			Match = true;
			break;
		case sPending:
			Match = false;
			break;
		default:
			qRFwk();
			break;
		}

		Continue = Continue && !Flow.EndOfFlow() && ( ( Byte = Flow.Get() ) != 0 );
	}

	if ( Match )
		Id = Parser.GetId();
	else if ( (Byte != 0 ) && ( UnmatchingBehavior == ubPurge ) )
		while ( !Flow.EndOfFlow() && (Flow.Get() != 0 ) );
qRR
qRT
qRE
	return Id;
}

id__ stsfsm::GetId(
	const str::dString &Pattern,
	const automat_ &Automat )
{
	flx::E_STRING_IFLOW__ Flow;

	Flow.Init( Pattern );

	return GetId( Flow, Automat, ubKeep );
}

