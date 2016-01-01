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

#define CGIDAT__COMPILATION

#include "cgidat.h"

using namespace cgidat;

#ifndef CPE__MT
const content_ &data_::GetContent( content_row__ Row ) const
{
	static content Content;

	Content.Init();

	GetContent( Row, Content );

	return Content;
}
#endif

row__ data_::GetDataWithName( const content_ &Name ) const
{
	row__ Row = Relationships.First();
	ctn::E_CMITEMt( content_, content_row__ ) Content;

	Content.Init( Contents );

	while( ( Row != qNIL ) && ( Content( Relationships( Row ).Name ) != Name ) )
		Row = Relationships.Next( Row );

	return Row;
}

row__ data_::GetDataWithNameAndSuffix(
	const content_ &Name,
	const content_ &Suffix ) const
{
	row__ Row = Relationships.First();
	ctn::E_CMITEMt( content_, content_row__ ) Content;

	Content.Init( Contents );

	while( ( Row != qNIL )
		   && ( ( Content( Relationships( Row ).Name ) != Name )
			  || ( Content( Relationships( Row ).Suffix ) != Suffix ) ) )
		Row = Relationships.Next( Row );

	return Row;
}

void cgidat::Fill(
	const cgiarg::arguments_ &Args,
	data_ &Data )
{
qRH
	cgiarg::row__ Row = qNIL;
	str::string Name, Suffix, Value;
qRB
	Row = Args.First();

	while ( Row != qNIL ) {
		Name.Init();
		Suffix.Init();
		Value.Init();

		Data.Set( Args.GetName( Row, Name ), Args.GetSuffix( Row, Suffix ), Args.GetValue( Row, Value ) );

		Row = Args.Next( Row );
	}
qRR
qRT
qRE
}
