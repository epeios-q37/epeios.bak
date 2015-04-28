/*
	'mthfrc' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'mthfrc' header file ('mthfrc.h').
	Copyright (C) 2004 Claude SIMON.

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

#define MTHFRC__COMPILATION

#include "mthfrc.h"

using namespace mthfrc;

void mthfrc::Simplify( fraction_ &Fraction )
{
ERRProlog
	integer PGCD;
ERRBegin
	PGCD.Init();

	mthitg::PGCD( Fraction.N, Fraction.D, PGCD );

	Fraction.N /= PGCD;
	Fraction.D /= PGCD;
ERRErr
ERREnd
ERREpilog
}
