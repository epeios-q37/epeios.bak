/*
	'ndbbsc' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'ndbbsc' header file ('ndbbsc.h').
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

#define NDBBSC__COMPILATION

#include "ndbbsc.h"


using namespace ndbbsc;

void ndbbsc::DropFile(
	const str::string_ &RootFileName,
	const char *Extension )
{
ERRProlog
	str::string FileName;
	TOL_CBUFFER___ FileNameBuffer;
ERRBegin
	if ( RootFileName.Amount() != 0 ) {
		FileName.Init( RootFileName );
		FileName.Append( Extension );

		if ( fil::Exists( FileName.Convert( FileNameBuffer) ) )
			if ( remove( FileNameBuffer ) != 0 )
				ERRLbr();
	}
ERRErr
ERREnd
ERREpilog
}
