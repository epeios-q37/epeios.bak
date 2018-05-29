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

#define XMLBSC__COMPILATION

#include "xmlbsc.h"

#include "fnm.h"

static void Init_(
	fls::file_storage_driver___ &Driver,
	char *Buffer,
	const char *Discriminator,
	const char *Name,
	const char *Directory,
	const char *Suffix,
	fls::id__ ID )
{
qRH
	fnm::name___ FileName;
	TOL_CBUFFER___ FileNameBuffer;
qRB
	sprintf( Buffer, "%s%s", Name, Discriminator );

	Driver.Init( ID, fnm::BuildFileName( Directory, Buffer, Suffix, FileName ).UTF8( FileNameBuffer ) );
qRR
qRT
qRE
}


void xmlbsc::file_memory_drivers::Init(
	const char *Name,
	fls::id__ ID,
	const char *Directory,
	const char *Suffix )
{
qRH
	char *Buffer = NULL;
qRB
	if ( ( Buffer = (char *)malloc( strlen( Name ) + 2 ) ) == NULL )
		qRAlc();

	Init_( DTree.Tree, Buffer, "T", Name, Directory, Suffix, ID );
	Init_( DTree.Queue, Buffer, "Q", Name, Directory, Suffix, ID );
	Init_( List, Buffer, "L", Name, Directory, Suffix, ID );
	Init_( Container.Statics, Buffer, "S", Name, Directory, Suffix, ID );
	Init_( Container.Dynamics.Descriptors, Buffer, "D", Name, Directory, Suffix, ID );
	Init_( Container.Dynamics.Multimemory, Buffer, "M", Name, Directory, Suffix, ID );

qRR
qRT
	if ( Buffer != NULL )
		free( Buffer );
qRE
}
