/*
	Copyright (C) 2010-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of dpkq.

    dpkq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    dpkq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with dpkq.  If not, see <http://www.gnu.org/licenses/>
*/

#include "dpkals.h"

using namespace dpkals;

trow__ dpkals::FindTable(
	const str::string_ &Label,
	const table_aliases_ &Aliases )
{
	ctn::qCMITEMsl( table_alias_ ) Alias;
	sdr::row__ Row = Aliases.First();

	Alias.Init( Aliases );

	if ( ( Row != qNIL ) && ( Alias( Row ).Label != Label ) )
		Row = Aliases.Next( Row );

	if ( Row != qNIL )
		return Alias( Row ).TableRow();
	else
		return qNIL;
}
