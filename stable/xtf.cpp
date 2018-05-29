/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define XTF__COMPILATION

#include "xtf.h"

# include "lcl.h"

using namespace xtf;

void xtf::extended_text_iflow__::_SetMeaning( lcl::meaning_ &Meaning )
{
	Meaning.SetValue( GetLabel( _Error ) );
}


bso::bool__  xtf::extended_text_iflow__::_GetCell(
	str::string_ *Cell,
	flw::byte__ Separator )
{
	bso::bool__ Cont = true;
	flw::byte__ C = 0;
	error__ Error = e_NoError;
	utf__ UTF;

	if ( !EndOfFlow( Error ) ) {
		UTF.Init();
		C = Get( UTF );

		if ( C == Separator )
			Cont = false;
		else if ( (C == '\r') || (C == '\n') ) {
			if ( _EOL != 0 )
				Cont = false;
			else if ( !EndOfFlow( Error ) ) {
				UTF.Init();
				C = Get( UTF );
			} else
				Cont = false;
		}

		while( Cont ) {

			if ( C == Separator )
				Cont = false;
			else if ( _Position.Column == 0 )
				Cont = false;
			else if ( Cell != NULL )
				Cell->Append( (const bso::char__ *)UTF.Data, UTF.Size );

			if ( Cont && EndOfFlow( Error ) )
				Cont = false;

			if ( Cont == true )	 {
				UTF.Init();
				C = Get( UTF );
			}
		}
	}

	if ( EndOfFlow( Error ) )
		return false;
	else if ( Separator == 0 )
		return true;
	else
		return ( C == Separator );
}
