/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHWebQ.

	XDHWebQ is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHWebQ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHWebQ. If not, see <http://www.gnu.org/licenses/>.
*/

#include "query.h"

using namespace query;

prow__ query::dPairs::Add(
	const item_ &Field,
	const item_ &Value )
{
	pair__ Pair;

	Pair.Init();

	Pair.Field = Items.Add( Field );
	Pair.Value = Items.Add( Value );

	return Pairs.Add( Pair );
}

void query::dPairs::Add(
	const str::dStrings &Keys,
	const str::dStrings &Values )
{
	sdr::sRow Row = Keys.First();

	if ( Keys.Amount() != Values.Amount() )
		qRGnr();

	while ( Row != qNIL ) {
		Add( Keys( Row ), Values( Row ) );

		Row = Keys.Next( Row );
	}
}

#ifdef CPE_C_CLANG
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wparentheses"
#endif

static void Read_(
	flw::iflow__ &Flow,
	item_ &Item )
{
	bso::char__ C = 0;

	while ( C = Flow.Get() )
		Item.Append( C );
}

#ifdef CPE_C_CLANG
# pragma clang diagnostic pop
#endif

void query::dPairs::FillWith( flw::iflow__ &Flow )
{
qRH
	str::string Field, Value;
qRB
	Field.Init();
	Read_( Flow, Field );

	while ( Field.Amount() != 0 ) {
		Value.Init();
		Read_( Flow, Value );

		Add( Field, Value);

		Field.Init();
		Read_( Flow, Field );
	}
qRR
qRT
qRE
}

prow__ query::dPairs::Find( const item_ &Field ) const
{
	prow__ Row = Pairs.First();

	while ( (Row != qNIL) && (Items( Pairs( Row ).Field ) != Field) )
		Row = Pairs.Next( Row );

	return Row;
}

bso::bool__ query::dPairs::GetValue(
	const item_ &Field,
	item_ &Value ) const
{
	prow__ Row = Find( Field );

	if (Row == qNIL )
		return false;

	Items.Recall(Pairs( Row ).Value, Value);

	return false;
}

void query::dPairs::Dump( xml::writer_ &Writer ) const
{
	prow__ Row = Pairs.First();

	Writer.PushTag("Pairs");
	xml::PutAttribute("Amount", Pairs.Amount(), Writer );

	while ( Row != qNIL )
	{
		Writer.PushTag("Pair");

		Writer.PutValue( Items( Pairs( Row ).Field ), "Field" );
		Writer.PutValue( Items( Pairs( Row ).Value ), "Value" );

		Writer.PopTag();

		Row = Pairs.Next( Row );
	}

	Writer.PopTag();
}



