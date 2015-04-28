/*
	'ndbsct' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'ndbsct' header file ('ndbsct.h').
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

#define NDBSCT__COMPILATION

#include "ndbsct.h"

using namespace ndbsct;

#define LIST_FILE_NAME_EXTENSION	".edl"
#define CONTENT_FILE_NAME_EXTENSION		".edc"

void ndbsct::files_hook___::Init(
	const str::string_ &BaseFileName,
	fil::mode__ Mode,
	fls::id__ ID )
{
ERRProlog
	str::string FileName;
	TOL_CBUFFER___ FileNameBuffer;
ERRBegin
	reset();

	_Mode = Mode;
	_BaseFileName.Init( BaseFileName );

	FileName.Init( BaseFileName );
	FileName.Append( CONTENT_FILE_NAME_EXTENSION );

	_Memory.Init( FileName.Convert( FileNameBuffer ), Mode, true, ID );


	FileName.Init( BaseFileName );
	FileName.Append( LIST_FILE_NAME_EXTENSION );

	_List.Init( FileName.Convert( FileNameBuffer ), Mode, true );
ERRErr
ERREnd
ERREpilog
}
