/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ogzusr.h"

using namespace ogzusr;

sAmount ogzusr::rRegularCallback::GetSet_(
	const dRecords &Records,
	sIndice Indice,
	sAmount Amount,
	dSet &Set ) const
{
	sAmount RealAmount = 0;

	if ( Amount == 0 )
		Amount = Records.Amount() - 1;

	sRRow Row = Records.First( Indice );

	while ( ( Row != qNIL) && Amount-- ) {
		Set.Append( Row );

		Row = Records.Next( Row );
		RealAmount++;
	}

	return RealAmount;
}


