/*
	'cgidat' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'cgidat' header file ('cgidat.h').
	Copyright (C) 2002-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

	while( ( Row != E_NIL ) && ( Content( Relationships( Row ).Name ) != Name ) )
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

	while( ( Row != E_NIL )
		   && ( ( Content( Relationships( Row ).Name ) != Name )
			  || ( Content( Relationships( Row ).Suffix ) != Suffix ) ) )
		Row = Relationships.Next( Row );

	return Row;
}

void cgidat::Fill(
	const cgiarg::arguments_ &Args,
	data_ &Data )
{
ERRProlog
	cgiarg::row__ Row = E_NIL;
	str::string Name, Suffix, Value;
ERRBegin
	Row = Args.First();

	while ( Row != E_NIL ) {
		Name.Init();
		Suffix.Init();
		Value.Init();

		Data.Set( Args.GetName( Row, Name ), Args.GetSuffix( Row, Suffix ), Args.GetValue( Row, Value ) );

		Row = Args.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}
