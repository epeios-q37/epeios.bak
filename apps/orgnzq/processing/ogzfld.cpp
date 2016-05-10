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

#include "ogzfld.h"

using namespace ogzfld;

sRow ogzfld::sXFields::Create_( ogzclm::sRow Column )
{
	sRow Row = qNIL;
qRH
	wField Field;
qRB
	Field.Init( Column );

	Row = Core_.New();

	Core_.Store( Field, Row );
qRR
qRT
qRE
	return Row;
}

namespace {
	void GetEntries_(
		const wField &Field,
		ogztyp::sRow Type,
		const ogzdta::sData &Data,
		str::dStrings &Entries )
	{
	qRH
		sdr::sRow Row = qNIL;
		str::wString Entry;
	qRB
		Row = Field.First();

		while ( Row != qNIL ) {
			Entry.Init();
			Data.Recall( Field( Row ), Type, Entry );

			Entries.Append( Entry );

			Row = Field.Next( Row );
		}
	qRR
	qRT
	qRE
	}
}

void ogzfld::sXFields::GetFeatures(
	sRow Row,
	ogzclm::sRow &Column,
	str::dStrings &Entries,
	ogztyp::sRow &Type ) const
{
qRH
	wField Field;
qRB
	Field.Init();
	Core_.Recall( Row, Field  );

	Column = Field.Column();

	Type = Columns_.GetType( Column );

	GetEntries_( Field, Type, D_(), Entries );
qRR
qRT
qRE
}

