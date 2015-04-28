/*
	'ias' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'ias' header file ('ias.h').
	Copyright (C) 2004 Claude SIMON.

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

#define IAS__COMPILATION

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

void ias::hook_filenames___::Init(
	const fnm::name___ &Path,
	const fnm::name___ &Basename )
{
ERRProlog
	fnm::name___ Descriptors, Storage;
ERRBegin
	Descriptors.Init();
	fnm::BuildPath( Path, Basename, "d", Descriptors );

	Storage.Init();
	fnm::BuildPath( Path, Basename, "s", Storage );

	this->Descriptors.Init( NULL, Descriptors );
	this->Storage.Init( NULL, Storage );

ERRErr
ERREnd
ERREpilog
}

