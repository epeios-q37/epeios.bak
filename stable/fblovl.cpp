/*
	'fblovl' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fblovl' header file ('fblovl.h').
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
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

#define FBLOVL__COMPILATION

#include "fblovl.h"

#include "err.h"

using namespace fblovl;

#define CASE( m )\
	case r##m:\
	return FBLOVL_NAME "_" #m;\
	break

const char *fblovl::GetLabel( reply__ Reply )
{
	switch ( Reply ) {
	CASE( OK );
	CASE( SoftwareError );
	CASE( RequestError );
	default:
		ERRPrm();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}

