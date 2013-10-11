/*
	'dbsdct' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'dbsdct' header file ('dbsdct.h').
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



//	$Id: dbsdct.cpp,v 1.29 2009/11/15 11:30:57 csimon Exp $

#define DBSDCT__COMPILATION

#include "dbsdct.h"

class dbsdcttutor
: public ttr_tutor
{
public:
	dbsdcttutor( void )
	: ttr_tutor( DBSDCT_NAME )
	{
#ifdef DBSDCT_DBG
		Version = DBSDCT_VERSION "\b\bD $";
#else
		Version = DBSDCT_VERSION;
#endif
		Owner = DBSDCT_OWNER;
		Date = "$Date: 2009/11/15 11:30:57 $";
	}
	virtual ~dbsdcttutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace dbsdct;

#include "flf.h"

#define LIST_FILE_NAME_EXTENSION	".edl"
#define AVAILABLES_FILE_NAME_EXTENSION	".eda"
#define CONTENT_FILE_NAME_EXTENSION		".edc"
#define ENTRIES_FILE_NAME_EXTENSION		".ede"

static inline void Save_(
	epeios::row__ Row,
	flw::oflow__ &Flow )
{
	dtfptb::PutULong( *Row, Flow );
}

static inline void Save_(
	const available__ &Available,
	flw::oflow__ &Flow )
{
	dtfptb::PutULong( *Available.Row, Flow );
	dtfptb::PutULong( Available.RawSize, Flow );
}

template <typename item> static void Save_(
	const stk::E_BSTACK_( item ) &Bunch,
	flw::oflow__ &Flow )
{
	stk::row__ Row = Bunch.First();

	while ( Row != NONE ) {
		Save_( Bunch( Row ), Flow );

		Row = Bunch.Next( Row );
	}
}

template <typename item> static void Save_(
	const stk::E_BSTACK_( item ) &Bunch,
	const char *RootFileName )
{
ERRProlog
	flf::file_oflow___ Flow;
ERRBegin
	Flow.Init( RootFileName );

	Save_( Bunch, Flow );
ERRErr
ERREnd
ERREpilog
}

template <typename item> static void Save_(
	const stk::E_BSTACK_( item ) &Bunch,
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
	Save_( Bunch, FileName.Convert( FileNameBuffer ) );

	while ( UnderlyingFilesLastModificationTime >= fil::GetFileLastModificationTime( FileNameBuffer ) ) {
		tol::Clock( true );
		fil::TouchFile( FileNameBuffer );
	}
ERRErr
ERREnd
ERREpilog
}

void dbsdct::exploded_dynamic_content_::_SaveLocationsAndAvailables( void ) const
{
	Save_( Availables, RootFileName, AVAILABLES_FILE_NAME_EXTENSION, _GetUnderlyingFilesLastModificationTime() );
}

static inline void Load_(
	flw::iflow__ &Flow,
	epeios::row__ &Row )
{
	Row = dtfptb::GetULong( Flow );
}
	
static inline void Load_(
	flw::iflow__ &Flow,
	available__ &Available )
{
	Available.Row = dtfptb::GetULong( Flow );
	Available.RawSize = dtfptb::GetULong( Flow );
}
	

template <typename item> static void Load_(
	flw::iflow__ &Flow,
	stk::E_BSTACK_( item ) &Bunch,
	item TestValue )
{
	item Item;

	Load_( Flow, Item );

	while ( Item != TestValue ) {
		Bunch.Append( Item );
		Load_( Flow, Item );
	}
}

template <typename item> static bso::bool__ Load_(
	const char *RootFileName,
	stk::E_BSTACK_( item ) &Bunch,
	item TestValue,
	time_t TimeStamp )
{
	bso::bool__ Success = false;
ERRProlog
	flf::file_iflow___ Flow;
	static flw::datum__ Buffer[sizeof( item )];
ERRBegin
	if ( Flow.Init( RootFileName, err::hSkip ) == fil::sSuccess ) {
		if ( fil::GetFileLastModificationTime( RootFileName ) < TimeStamp )
			ERRReturn;

		memcpy( Buffer, &TestValue, sizeof( item ) );

		Flow.EOFD( (void *)Buffer, sizeof( item ) );

		Load_( Flow, Bunch, TestValue );

		Flow.reset();

		Success = true;
	}
ERRErr
ERREnd
ERREpilog
	return Success;
}

template <typename item> static bso::bool__ Load_(
	const str::string_ &RootFileName,
	stk::E_BSTACK_( item ) &Bunch,
	item TestValue,
	const char *Extension,
	time_t TimeStamp )
{
	bso::bool__ Success = false;
ERRProlog
	str::string FileName;
	STR_BUFFER___ FileNameBuffer;
ERRBegin
	FileName.Init( RootFileName );
	FileName.Append( Extension );
	Success = Load_( FileName.Convert( FileNameBuffer ), Bunch, TestValue, TimeStamp );
ERRErr
ERREnd
ERREpilog
	return Success;
}

void dbsdct::exploded_dynamic_content_::Init(
	const str::string_ &RootFileName,
	mdr::mode__ Mode,
	bso::bool__ Partial,
	flm::id__ ID )
{
ERRProlog
	str::string ContentFileName;
	STR_BUFFER___ ContentFileNameBuffer;
	str::string EntriesBunchFileName;
	STR_BUFFER___ EntriesBunchFileNameBuffer;
	str::string EntriesListFileName;
	STR_BUFFER___ EntriesListFileNameBuffer;
ERRBegin
	reset();

	ContentFileName.Init( RootFileName );
	ContentFileName.Append( CONTENT_FILE_NAME_EXTENSION );
	S_.StorageFileManager.Init( ContentFileName.Convert( ContentFileNameBuffer ), Mode, true, ID );

	EntriesBunchFileName.Init( RootFileName );
	EntriesBunchFileName.Append( ENTRIES_FILE_NAME_EXTENSION );

	EntriesListFileName.Init( RootFileName );
	EntriesListFileName.Append( LIST_FILE_NAME_EXTENSION );

	S_.EntriesFileManager.Init( EntriesBunchFileName.Convert( EntriesBunchFileNameBuffer) , EntriesListFileName.Convert( EntriesListFileNameBuffer ), Mode, true, ID );

	this->RootFileName.Init( RootFileName );
	S_.Mode = Mode;

	dynamic_content_::Init( Partial );
ERRErr
ERREnd
ERREpilog
}


bso::bool__ dbsdct::exploded_dynamic_content_::_ConnectToFiles( void )
{
	bso::bool__ Exists = false;
ERRProlog
	available__ TestAvailable;
ERRBegin
	Exists = tym::Connect( Storage.Memory, S_.StorageFileManager );

	if ( lstbch::Connect( Entries, S_.EntriesFileManager ) != Exists )
		ERRu();

	if ( Exists )
		dynamic_content_::S_.Unallocated = S_.StorageFileManager.FileSize();
	else
		dynamic_content_::S_.Unallocated = 0;

	if ( Exists ) {
		if ( !Load_<available__>( RootFileName, Availables, TestAvailable, AVAILABLES_FILE_NAME_EXTENSION, _GetUnderlyingFilesLastModificationTime() ) )
			RebuildAvailables();
	}
ERRErr
ERREnd
ERREpilog
	return Exists;
}

void dbsdct::exploded_dynamic_content_::_ErasePhysically( void )
{
ERRProlog
ERRBegin
	S_.StorageFileManager.Drop();
	S_.EntriesFileManager.Drop();

	dbsbsc::DropFile( RootFileName, AVAILABLES_FILE_NAME_EXTENSION );
ERRErr
ERREnd
ERREpilog
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dbsdctpersonnalization
: public dbsdcttutor
{
public:
	dbsdctpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dbsdctpersonnalization( void )
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

static dbsdctpersonnalization Tutor;

ttr_tutor &DBSDCTTutor = Tutor;
