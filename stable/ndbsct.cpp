/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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
qRH
	str::string FileName;
	TOL_CBUFFER___ FileNameBuffer;
qRB
	reset();

	_Mode = Mode;
	_BaseFileName.Init( BaseFileName );

	FileName.Init( BaseFileName );
	FileName.Append( CONTENT_FILE_NAME_EXTENSION );

	_Memory.Init( FileName.Convert( FileNameBuffer ), Mode, true, ID );


	FileName.Init( BaseFileName );
	FileName.Append( LIST_FILE_NAME_EXTENSION );

	_List.Init( FileName.Convert( FileNameBuffer ), Mode, true );
qRR
qRT
qRE
}
