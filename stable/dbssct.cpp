/*
	'dbssct' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'dbssct' header file ('dbssct.h').
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: dbssct.cpp,v 1.32 2009/11/15 11:30:58 csimon Exp $

#define DBSSCT__COMPILATION

#include "dbssct.h"

class dbsscttutor
: public ttr_tutor
{
public:
	dbsscttutor( void )
	: ttr_tutor( DBSSCT_NAME )
	{
#ifdef DBSSCT_DBG
		Version = DBSSCT_VERSION "\b\bD $";
#else
		Version = DBSSCT_VERSION;
#endif
		Owner = DBSSCT_OWNER;
		Date = "$Date: 2009/11/15 11:30:58 $";
	}
	virtual ~dbsscttutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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
ERRProlog
	str::string FileName;
	STR_BUFFER___ FileNameBuffer;
ERRBegin
	FileName.Init( RootFileName );
	FileName.Append( Extension );
	lst::WriteToFile( List, FileName.Convert( FileNameBuffer ), UnderlyingFilesLastModificationTime );

/*
	while ( UnderlyingFilesLastModificationTime >= tol::GetFileLastModificationTime( FileNameBuffer ) ) {
		tol::Clock( true );
		tol::Touch( FileNameBuffer );
	}
*/
ERRErr
ERREnd
ERREpilog
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
ERRProlog
	str::string ContentFileName;
	tol::E_FPOINTER___( bso::char__ ) ContentFileNameBuffer;
ERRBegin
	reset();

	this->RootFileName.Init( RootFileName );
	S_.Mode = Mode;

	static_content_::Init( Size, Partial );

	ContentFileName.Init( RootFileName );
	ContentFileName.Append( CONTENT_FILE_NAME_EXTENSION );

	S_.FileManager.Init( ContentFileName.Convert( ContentFileNameBuffer ), Mode, true, ID );
ERRErr
ERREnd
ERREpilog
}

// Permet de stocker les données entièrement en mémoire. NON UTILISABLE_EN_EXPOITATION !
//#define IN_MEMORY

bso::bool__ dbssct::exploded_static_content_::_ConnectToFiles( void )
{
	bso::bool__ Exists = false;
ERRProlog
	str::string ContentFileName;
	STR_BUFFER___ ContentFileNameBuffer;
	str::string ListFileName;
	STR_BUFFER___ ListFileNameBuffer;
#ifdef IN_MEMORY
	tym::E_MEMORY( atom__) Storage;
#endif
ERRBegin
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
ERRErr
ERREnd
ERREpilog
	return Exists;
}

void dbssct::exploded_static_content_::_ErasePhysically( void )
{
ERRProlog
ERRBegin
	S_.FileManager.Drop();

	dbsbsc::DropFile( RootFileName, LIST_FILE_NAME_EXTENSION );
ERRErr
ERREnd
ERREpilog
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dbssctpersonnalization
: public dbsscttutor
{
public:
	dbssctpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dbssctpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static dbssctpersonnalization Tutor;

ttr_tutor &DBSSCTTutor = Tutor;
