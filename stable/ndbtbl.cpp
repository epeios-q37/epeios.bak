/*
	'ndbtbl' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'ndbtbl' header file ('ndbtbl.h').
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



//	$Id: ndbtbl.cpp,v 1.20 2013/04/15 10:50:53 csimon Exp $

#define NDBTBL__COMPILATION

#include "ndbtbl.h"

class ndbtbltutor
: public ttr_tutor
{
public:
	ndbtbltutor( void )
	: ttr_tutor( NDBTBL_NAME )
	{
#ifdef NDBTBL_DBG
		Version = NDBTBL_VERSION "\b\bD $";
#else
		Version = NDBTBL_VERSION;
#endif
		Owner = NDBTBL_OWNER;
		Date = "$Date: 2013/04/15 10:50:53 $";
	}
	virtual ~ndbtbltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "fnm.h"

using namespace ndbtbl;

void ndbtbl::table_::_InsertInIndexes( rrow__ Row )
{
	sdr::row__ IRow = _Indexes.First();

	while ( IRow != E_NIL ) {
		_Indexes( IRow )->Index( Row, NULL );

		IRow = _Indexes.Next( IRow );
	}
}

void ndbtbl::table_::_DeleteFromIndexes( rrow__ Row )
{
	sdr::row__ IRow = _Indexes.First();

	while ( IRow != E_NIL ) {
/*		if ( !Indexes( IRow )->InitializationCompleted() )
			Indexes( IRow )->CompleteInitialization();
*/
		_Indexes( IRow )->Delete( Row );

		IRow = _Indexes.Next( IRow );
	}
}

void ndbtbl::table_::_ReindexAll( observer_functions__ &Observer )
{
	_Test( mReadWrite );

	sdr::row__ Row = _Indexes.First();

	if ( &Observer != NULL ) {
		Observer.Set( _Indexes.Amount() );
	}

	while ( Row != E_NIL ) {
		_Indexes( Row )->Reindex( Observer );

		if ( &Observer )
			Observer.IncrementHandledIndexAmount();

		Row = _Indexes.Next( Row );
	}
}

void ndbtbl::table_::Retrieve(
	const rrows_ &Rows,
	data_ &Data ) const
{
ERRProlog
	datum Datum;
	sdr::row__ Row = E_NIL;
ERRBegin
	_Test( mReadOnly );

	Row = Rows.First();

	while ( Row != E_NIL ) {
		Datum.Init();

		Content().Retrieve( Rows( Row ), Datum );

		Data.Append( Datum );

		Row = Rows.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

void ndbtbl::table_::Insert(
	const data_ &Data,
	rrows_ &RecordRows )
{
	_Test( mReadWrite );

	ctn::E_CMITEM( datum_ ) Datum;
	sdr::row__ Row = Data.First();

	Datum.Init( Data );

	while ( Row != E_NIL ) {
		RecordRows.Append( Insert( Datum( Row ) ) );

		Row = Data.Next( Row );
	}
}

void ndbtbl::table_::Update(
	const data_ &Data,
	const rrows_ &RecordRows )
{
	if ( Data.Amount() != RecordRows.Amount() )
		ERRPrm();

	_Test( mReadWrite );

	ctn::E_CMITEM( datum_ ) Datum;
	sdr::row__ Row = Data.First();

	Datum.Init( Data );

	while ( Row != E_NIL ) {
		Update( Datum( Row ), RecordRows( Row ) );

		Row = Data.Next( Row );
	}
}

void ndbtbl::table_::Delete( const rrows_ &RecordRows )
{
	_Test( mReadWrite );

	sdr::row__ Row = RecordRows.First();

	while ( Row != E_NIL ) {
		Delete( RecordRows( Row ) );

		Row = RecordRows.Next( Row );
	}
}


void ndbtbl::table_::_ResetAllIndexes( void )
{
	sdr::row__ Row = _Indexes.First();

	while ( Row != E_NIL ) {
		_Indexes( Row )->Reset();

		Row = _Indexes.Next( Row );
	}
}

void ndbtbl::table_::TestRecordsExistence(
	const rrows_ &RecordRows,
	rows_ &Rows ) const
{
	_Test( mReadOnly );

	sdr::row__ Row = RecordRows.First();

	while ( Row != E_NIL ) {
		if ( !RecordExists( RecordRows( Row ) ) )
			Rows.Append( Row );

		Row = RecordRows.Next( Row );
	}

}

bso::bool__ ndbtbl::table_::AreAllIndexesSynchronized( void ) const
{
	_Test( mReadOnly );

	sdr::row__ Row = _Indexes.First();

	while ( Row != E_NIL ) {
		if ( !_Indexes( Row )->IsSynchronized() )
			return false;

		Row = _Indexes.Next( Row );
	}

	return true;
}
/*
static const str::string_ &GetFileName_(
	const str::string_ &Path,
	const str::string_ &RootFileName,
	str::string_ &FileName )
{
#ifdef DBIEDB_DBG
	if ( RootFileName.Amount() == 0 )
		ERRc();
#endif

	FileName.Init( Path );

	if ( FileName.Amount() != 0 )
		FileName.Append( FNM_DIRECTORY_SEPARATOR_STRING );

	FileName.Append( RootFileName );

	return FileName;
}
*/

void ndbtbl::files_hook___::_InitStatic(
	const str::string_ &BaseFileName,
	mode__ Mode,
	fls::id__ ID )
{
ERRProlog
	str::string FileName;
ERRBegin
	FileName.Init();

//	GetFileName_( Path, BaseFileName, FileName );

	_Static.Init( BaseFileName, Convert( Mode ), ID );
ERRErr
ERREnd
ERREpilog
}

void ndbtbl::files_hook___::_InitDynamic(
	const str::string_ &BaseFileName,
	mode__ Mode,
	fls::id__ ID )
{
ERRProlog
	str::string FileName;
ERRBegin
	FileName.Init();

//	GetFileName_( Path, RootFileName, FileName );

	_Dynamic.Init( BaseFileName, Convert( Mode ), ID );
ERRErr
ERREnd
ERREpilog
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class ndbtblpersonnalization
: public ndbtbltutor
{
public:
	ndbtblpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~ndbtblpersonnalization( void )
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

static ndbtblpersonnalization Tutor;

ttr_tutor &NDBTBLTutor = Tutor;
