/*
	'epsmsc' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'epsmsc' header file ('epsmsc.h').
	Copyright (C) 2000, 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#define EPSMSC__COMPILATION

#include "epsmsc.h"

using namespace epsmsc;
using namespace txf;

void epsmsc::PrintLicense( txf::text_oflow__ &Flow )
{
	Flow 
	<< "    This program is free software: you can redistribute it and/or modify" << nl
	<< "    it under the terms of the GNU General Public License as published by" << nl
	<< "    the Free Software Foundation, either version 3 of the License, or" << nl
	<< "    (at your option) any later version." << nl
	<< nl
	<< "    This program is distributed in the hope that it will be useful," << nl
	<< "    but WITHOUT ANY WARRANTY; without even the implied warranty of" << nl
	<< "    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << nl
	<< "    GNU General Public License for more details." << nl
	<< nl
	<< "    You should have received a copy of the GNU General Public License" << nl
	<< "    along with this program.  If not, see <http://www.gnu.org/licenses/>." << nl;

}

