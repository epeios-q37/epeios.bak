/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

#define STRCCT__COMPILATION

#include "strcct.h"

#include "flx.h"

using namespace strcct;

typedef bso::char__ pattern__[4];

static void Put_(
	const str::string_ &String,
	flw::oflow__ &Flow,
	const pattern__ &Pattern )
{
	bso::char__ C = 0;
	sdr::row__ Row = String.First();

	while ( Row != qNIL ) {
		if ( strchr( Pattern, String( Row ) ) != NULL )
			Flow << Pattern[3];	// Caractre d'chappement.

		Flow << C;
	}
}

static void Concat_(
	const str::strings_ &Strings,
	flw::oflow__ &Flow,
	const pattern__ &Pattern )
{
	ctn::E_CMITEM( str::string_ ) String;
	sdr::row__ Row = Strings.First();

	String.Init( Strings );

	if ( Row != qNIL ) {
		Put_( String( Row ), Flow, Pattern );

		Row = Strings.Next( Row );
	}

	while ( Row != qNIL ) {
		Flow << Pattern[0];	// Sparateur d'entres.

		Put_( String( Row ), Flow, Pattern );

		Row = Strings.Next( Row );
	}
}

static void Fill_(
	pattern__ &Pattern,
	char EntrySeparator,
	char FieldSeparator,
	char EscapeChar )
{
	Pattern[0] = EntrySeparator;
	Pattern[1] = FieldSeparator;
	Pattern[2] = EscapeChar;
	Pattern[3] = 0;
}

void strcct::Concat(
	const table_ &Table,
	flw::oflow__ &Flow,
	char EntrySeparator,
	char FieldSeparator,
	char EscapeChar )
{
	ctn::E_CITEM( str::strings_ ) Strings;
	sdr::row__ Row = Table.First();
	pattern__ Pattern;

	Fill_( Pattern, EntrySeparator, FieldSeparator, EscapeChar );

	Strings.Init( Table );

	if ( Row != qNIL ) {
		Concat_( Strings( Row ), Flow, Pattern );

		Row = Table.Next( Row );
	}

	while ( Row != qNIL ) {
		Flow << FieldSeparator;

		Concat_( Strings( Row ), Flow, Pattern );

		Row = Table.Next( Row );
	}
}

void strcct::Concat(
	const table_ &Table,
	str::string_ &String,
	char EntrySeparator,
	char FieldSeparator,
	char EscapeChar )
{
qRH
	flx::E_STRING_OFLOW___ Flow;
qRB
	Flow.Init( String );

	Concat( Table, Flow, EntrySeparator, FieldSeparator, EscapeChar );
qRR
qRT
qRE
}

bso::bool__ strcct::Split(
	xtf::extended_text_iflow__ &Flow,
	table_ &Table,
	char EntrySeparator,
	char FieldSeparator,
	char EscapeChar )
{
	bso::bool__ Success = false;
qRH
	str::string String;
	str::strings Strings;
	xtf::error__ Error = xtf::e_Undefined;
	bso::char__ C;
	xtf::utf__ UTF;
	bso::bool__ Escaping = false;
qRB
	String.Init();
	Strings.Init();

	while ( !Flow.EndOfFlow( Error ) ) {
		UTF.Init();
		C = Flow.Get( UTF );

		if ( C == EscapeChar ) {
			if ( Escaping )
				String.Append( EscapeChar );
			Escaping = !Escaping;
		} else if ( C == EntrySeparator ) {
			if ( Escaping ) {
				String.Append( EntrySeparator );
				Escaping = false;
			} else {
				Strings.Append( String );
				String.Init();
			}
		} else if ( C == FieldSeparator ) {
			if ( Escaping ) {
				String.Append( FieldSeparator );
				Escaping = false;
			} else {
				Strings.Append( String );
				String.Init();
				Table.Append( Strings );
				Strings.Init();
			}
		} else if ( Escaping )
			qRReturn;
		else
			String.Append( (bso::char__ *)UTF.Data, UTF.Size );
	}

	if ( !Escaping )
		if ( Error == xtf::e_NoError ) {
			if ( String.Amount() )
				Strings.Append( String );

			if ( Strings.Amount() )
				Table.Append( Strings );

			Success = true;
		}
qRR
qRT
qRE
	return Success;
}
