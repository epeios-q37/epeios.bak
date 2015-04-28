/*
	'xtf' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'xtf' header file ('xtf.h').
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#define XTF__COMPILATION

#include "xtf.h"

# include "lcl.h"

using namespace xtf;

void xtf::extended_text_iflow__::_SetMeaning( lcl::meaning_ &Meaning )
{
	Meaning.SetValue( GetLabel( _Error ) );
}


bso::bool__  xtf::extended_text_iflow__::GetCell(
	str::string_ &Line,
	flw::datum__ Separator )
{
	bso::bool__ Cont = true;
	flw::datum__ C;
	error__ Error = e_NoError;
	utf__ UTF;

	if ( !EndOfFlow( Error ) ) {
		UTF.Init();
		C = Get( UTF );

		if ( C == Separator )
			Cont = false;
		else if ( ( C == '\r' ) || ( C == '\n' ) )
			if ( _EOL != 0 )
				Cont = false;
			else if ( !EndOfFlow( Error ) ) {
				UTF.Init();
				C = Get( UTF );
			} else
				Cont = false;

		while( Cont ) {

			if ( C == Separator )
				Cont = false;
			else if ( _Position.Column == 0 )
				Cont = false;
			else if ( &Line != NULL )
				Line.Append( (const bso::char__ *)UTF.Data, UTF.Size );

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
