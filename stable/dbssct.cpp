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

#define DBSSCT_COMPILATION_

#include "dbssct.h"

#include "dtfptb.h"

using namespace dbssct;

#include "flf.h"

#define LIST_FILE_NAME_EXTENSION	".edl"
#define CONTENT_FILE_NAME_EXTENSION		".edc"

static void Save_(
	const _list_ &List,
	const str::string_ &RootFileName,
	const char *Extension,
	time_t UnderlyingFilesLastModificationTime )
{
qRH
	str::string FileName;
	STR_BUFFER___ FileNameBuffer;
qRB
	FileName.Init( RootFileName );
	FileName.Append( Extension );
	lst::WriteToFile( List, FileName.Convert( FileNameBuffer ), UnderlyingFilesLastModificationTime );

/*
	while ( UnderlyingFilesLastModificationTime >= tol::GetFileLastModificationTime( FileNameBuffer ) ) {
		tol::Clock( true );
		tol::Touch( FileNameBuffer );
	}
*/
qRR
qRT
qRE
}

void dbssct::exploded_static_content_::_SaveLocations( void ) const
{
	Save_( *this, RootFileName, LIST_FILE_NAME_EXTENSION, _GetUnderlyingFilesLastModificationTime() );
}

void dbssct::exploded_static_content_::Init(
	epeios::size__ Size,
	const str::string_ &RootFileName,
	mdr::mode__ Mode,
	bso::bool__ Partial,
	flm::id__ ID )
{
qRH
	str::string ContentFileName;
	tol::E_FPOINTER___( bso::char__ ) ContentFileNameBuffer;
qRB
	reset();

	this->RootFileName.Init( RootFileName );
	S_.Mode = Mode;

	static_content_::Init( Size, Partial );

	ContentFileName.Init( RootFileName );
	ContentFileName.Append( CONTENT_FILE_NAME_EXTENSION );

	S_.FileManager.Init( ContentFileName.Convert( ContentFileNameBuffer ), Mode, true, ID );
qRR
qRT
qRE
}

// Permet de stocker les donnes entirement en mmoire. NON UTILISABLE_EN_EXPOITATION !
//#define IN_MEMORY

bso::bool__ dbssct::exploded_static_content_::_ConnectToFiles( void )
{
	bso::bool__ Exists = false;
qRH
	str::string ContentFileName;
	STR_BUFFER___ ContentFileNameBuffer;
	str::string ListFileName;
	STR_BUFFER___ ListFileNameBuffer;
#ifdef IN_MEMORY
	tym::E_MEMORY( atom__) Storage;
#endif
qRB
	ContentFileName.Init( RootFileName );
	ContentFileName.Append( CONTENT_FILE_NAME_EXTENSION );
#ifdef IN_MEMORY
	Storage.Init();
	Exists = Set_( S_.MemoryDriver.Storage, ContentFileNameBuffer, S_.Mode, Storage );

	static_content_::Storage.Allocate( tol::GetFileSize( ContentFileNameBuffer ) );
	static_content_::Storage.Store( Storage, tol::GetFileSize( ContentFileNameBuffer ) );
#else
//	Exists = Set_( S_.MemoryDriver.Storage, ContentFileNameBuffer, S_.Mode, static_content_::Storage );
	Exists = tym::Connect( this->Storage, S_.FileManager );
#endif

	if ( Exists ) {
		ListFileName.Init( RootFileName );
		ListFileName.Append( LIST_FILE_NAME_EXTENSION );

		if ( !lst::ReadFromFile( ListFileName.Convert( ListFileNameBuffer ), fil::GetFileSize( ContentFileName.Convert( ContentFileNameBuffer ) ) / S_.Size, *this, _GetUnderlyingFilesLastModificationTime() ) )
			RebuildLocations();
	}
qRR
qRT
qRE
	return Exists;
}

void dbssct::exploded_static_content_::_ErasePhysically( void )
{
qRH
qRB
	S_.FileManager.Drop();

	dbsbsc::DropFile( RootFileName, LIST_FILE_NAME_EXTENSION );
qRR
qRT
qRE
}
