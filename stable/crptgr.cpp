/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#define CRPTGR__COMPILATION

#include "crptgr.h"

#include "flx.h"

using namespace crptgr;

const str::string_ &crptgr::Encrypt(
	const str::string_ &Plain,
	const str::string_ &Key,
	str::string_ &Crypted )
{
ERRProlog
	flx::E_STRING_OFLOW___ SFlow;
	encrypt_oflow___ EFlow;
	sdr::row__ Row = E_NIL;
ERRBegin
	SFlow.Init( Crypted );
	EFlow.Init( SFlow, Key );

	Row = Plain.First();

	while( Row != E_NIL ) {
		EFlow.Put( Plain( Row ) );

		Row = Plain.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
	return Crypted;
}

const str::string_ &crptgr::Decrypt(
	const str::string_ &Crypted,
	const str::string_ &Key,
	str::string_ &Plain )
{
ERRProlog
	flx::E_STRING_IFLOW__ SFlow;
	decrypt_iflow___ DFlow;
ERRBegin
	SFlow.Init( Crypted );
	DFlow.Init( SFlow, Key );

	while ( DFlow.AmountRed() != Crypted.Amount() )
		Plain.Append( DFlow.Get() );
ERRErr
ERREnd
ERREpilog
	return Plain;
}
