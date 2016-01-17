/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#define PLGN__COMPILATION

#include "plgn.h"

using namespace plgn;

#include "sclmisc.h"

// 'qR...' to be sure that the recalled and removed retriever is deleted if an error occurs.
void plgn::Delete_( iRetrievers &Retrievers )
{
qRH
	fRow Row = qNIL;
	rLooseRetriever Retriever;
qRB
	fRow Row = Retrievers.Last();

	while ( Row != qNIL ) {
		Retrievers.Recall( Row, Retriever );

		Retriever.reset();

		Row = Retrievers.Previous( Row );

		Retrievers.Remove( Row );
	}
qRR
qRT
qRE
}

