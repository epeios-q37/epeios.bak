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

#define BKDDSC__COMPILATION

#include "bkddsc.h"

using namespace bkddsc;

void bkddsc::description_::Init(
	const char *Name,
	const cast *Casts )
{
	int i = 0;
	cast Cast = c_Unknow;

	this->Name.Init();
	this->Casts.Init();

	this->Name = Name;

	do
	{
		Cast = Casts[i++];
		this->Casts.Append( Cast );
	} while ( Cast != cEnd );

	while ( ( Cast = Casts[i++] ) != cEnd ) {
		this->Casts.Append( Cast );
	}

#ifdef RQM_DBG
	if ( Array != 0 )
		ERRc();
#endif

}


epeios::row__ bkddsc::descriptions_::Position( const description_ &Description ) const
{
	epeios::row__ Position = E_CONTAINER_( description_ )::First();
ERRProlog
	E_CITEM( description_ ) D;
ERRBegin

	D.Init( *this );

	while( ( Position != NONE )
		    && ( ( D( Position ).Name != Description.Name )
		         || ( D( Position ).Casts != Description.Casts ) ) )
		Position = E_CONTAINER_( description_ )::Next( Position );
ERRErr
ERREnd
ERREpilog
	return Position;
}
