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

#define FLW_COMPILATION_

#include "flw.h"

#ifdef FLW__IGNORE_SIGPIPE
#	include <signal.h>
#endif

using namespace flw;

bso::sBool flw::oflow__::_Write(
	const byte__ *Buffer,
	size__ Amount,
	qRPN )
{
	size__ PonctualAmountWritten = _WriteUpTo( Buffer, Amount );
	size__ AmountWritten = PonctualAmountWritten;

	while( ( PonctualAmountWritten != 0 ) && ( AmountWritten < Amount ) )
		AmountWritten += PonctualAmountWritten = _WriteUpTo( Buffer + AmountWritten, Amount - AmountWritten );

	if ( PonctualAmountWritten == 0 ) {
		if ( qRP == err::hThrowException )
			qRFwk();
		return false;
	} else
		return true;
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
	byte__ *Buffer,
	size__ Wanted,
	bso::bool__ Adjust,
	bso::bool__ &CacheIsEmpty )
{
	size__ Amount = 0;
qRH
qRB

#ifdef FLW_DBG
	if ( Wanted < Minimum )
		qRFwk();

	if ( Minimum < 1 )
		qRFwk();
#endif

	Amount = _LoopingRawRead( Minimum, Buffer, Wanted, Adjust, CacheIsEmpty );

	if ( Adjust )
		_Red += Amount;

	if ( _Red > _AmountMax )
		qRFwk();
qRR
qRT
qRE
	return Amount;
}
# endif

size__ flw::oflow__::_DirectWrite(
	const byte__ *Buffer,
	size__ Wanted,
	size__ Minimum )
{
	size__ Amount = 0;
qRH
qRB
	if ( ( _Size == 0 ) && ( _Cache != NULL ) )	// There was an error before. See below, in 'qRR'.
		qRFwk();

#ifdef FLW_DBG
	if ( Wanted < 1 )
		qRFwk();

	if ( Wanted < Minimum )
		qRFwk();
#endif

	Amount = _LoopingWrite( Buffer, Wanted, Minimum );

#ifdef FLW_DBG
	if ( Amount > Wanted )
		qRFwk();

	if ( ( Amount > 0 ) && ( Amount < Minimum ) )
		qRFwk();
#endif
qRR
	_Size = _Free = 0;	// Pour viter toute nouvelle criture dans le cache. La prochaine tentative gnrera une erreur.
	_Unlock( err::hUserDefined );		// Errors are ignored.
qRT
qRE
	return Amount;
}

#if 0
void flw::Copy(
	iflow__ &IFlow,
	size__ Amount,
	oflow__ &OFlow )
{
	bso::size__ Size = 4096;
	byte__ Buffer[4096];

	while ( Amount ) {
		if ( Amount < Size )
			Size = Amount;

		IFlow.Read( Size, Buffer );
		OFlow.Write( Buffer, Size );

		Amount -= Size;
	}
}
#endif

