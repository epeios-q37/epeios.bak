/*
	'bkddsc' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'bkddsc' header file ('bkddsc.h').
	Copyright (C) 2001, 2003-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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
