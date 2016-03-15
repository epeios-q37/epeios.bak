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

#include "ogzdtb.h"

using namespace ogzdtb;

namespace {
	E_CDEF(char *, RecordTypeLabel_, "Record");
	E_CDEF(char *, TextTypeLabel_, "Text");
}

void ogzdtb::rDatabase::FetchMandatoryTypes_( const ogztyp::dTypes &Types )
{
	ogztyp::sRow Row = Types.First();
	ogztyp::sType Type;

	while ( Row != qNIL ) {
		Types.Recall( Row, Type );

		if ( !strcmp( Type.GetLabel(), RecordTypeLabel_ ) )
			RecordType_ = Row;
		else if ( !strcmp( Type.GetLabel(), TextTypeLabel_ ) )
			TextType_ = Row;

		Type.reset( false );

		Row = Types.Next( Row );
	}

	if ( RecordType_ == qNIL )
		qRGnr();

	if ( TextType_ == qNIL )
		qRGnr();
}