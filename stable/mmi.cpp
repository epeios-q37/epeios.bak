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

#define MMI__COMPILATION

#include "mmi.h"


#define capacite				mdr::size__
#define multimemoire_indexee	indexed_multimemory_

namespace mmi {
	void multimemoire_indexee::AllouerMoins_(
		capacite CapaciteCourante,
		capacite CapaciteDemandee,
		aem::mode__ Mode )
	{
		while ( CapaciteCourante-- > CapaciteDemandee )
			Multimemoire.Free( Descripteurs.Get( CapaciteCourante ).Descripteur );

		Descripteurs.Allocate( CapaciteDemandee, Mode );
	}

	void multimemoire_indexee::RemoveWithoutReallocating(
		mdr::row__ Position,
		mdr::size__ ActualCapacity,
		mdr::size__ Amount )
	{
		mdr::size__ Counter = Amount;

		while( Counter-- )
			Multimemoire.Free( Descripteurs.Get( *Position + Counter ).Descripteur );

		Descripteurs.Store( Descripteurs, ActualCapacity - *Position - Amount, *Position, *Position + Amount );

		Initialize_( ActualCapacity - Amount, ActualCapacity );
	}

}

