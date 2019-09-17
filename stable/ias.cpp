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

#define IAS_COMPILATION_

#include "ias.h"

using namespace ias;

void indexed_aggregated_storage_::RemoveWithoutReallocating(
	sdr::row__ Position,
	sdr::size__ ActualCapacity,
	sdr::size__ Amount )
{
	sdr::size__ Counter = Amount;

	while( Counter-- )
		AStorage.Free( Descriptors.Get( *Position + Counter ) );

	Descriptors.Store( Descriptors, ActualCapacity - *Position - Amount, *Position, *Position + Amount );

	_Initialize( ActualCapacity - Amount, ActualCapacity );
}

void ias::rHF::Init(
	const fnm::name___ &Path,
	const fnm::name___ &Basename )
{
qRH
	fnm::name___ Descriptors, Storage;
qRB
	Descriptors.Init();
	fnm::BuildPath( Path, Basename, "d", Descriptors );

	Storage.Init();
	fnm::BuildPath( Path, Basename, "s", Storage );

	this->Descriptors.Init( "", Descriptors );
	this->Storage.Init( "", Storage );

qRR
qRT
qRE
}

