/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#define NDBTBL__COMPILATION

#include "ndbtbl.h"

#include "fnm.h"

using namespace ndbtbl;

void ndbtbl::table_::_InsertInIndexes( rrow__ Row )
{
	sdr::row__ IRow = _Indexes.First();

	while ( IRow != qNIL ) {
		_Indexes( IRow )->Index( Row, NULL );

		IRow = _Indexes.Next( IRow );
	}
}

void ndbtbl::table_::_DeleteFromIndexes( rrow__ Row )
{
	sdr::row__ IRow = _Indexes.First();

	while ( IRow != qNIL ) {
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

	while ( Row != qNIL ) {
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
qRH
	datum Datum;
	sdr::row__ Row = qNIL;
qRB
	_Test( mReadOnly );

	Row = Rows.First();

	while ( Row != qNIL ) {
		Datum.Init();

		Content().Retrieve( Rows( Row ), Datum );

		Data.Append( Datum );

		Row = Rows.Next( Row );
	}
qRR
qRT
qRE
}

void ndbtbl::table_::Insert(
	const data_ &Data,
	rrows_ &RecordRows )
{
	_Test( mReadWrite );

	ctn::E_CMITEM( datum_ ) Datum;
	sdr::row__ Row = Data.First();

	Datum.Init( Data );

	while ( Row != qNIL ) {
		RecordRows.Append( Insert( Datum( Row ) ) );

		Row = Data.Next( Row );
	}
}

void ndbtbl::table_::Update(
	const data_ &Data,
	const rrows_ &RecordRows )
{
	if ( Data.Amount() != RecordRows.Amount() )
		qRFwk();

	_Test( mReadWrite );

	ctn::E_CMITEM( datum_ ) Datum;
	sdr::row__ Row = Data.First();

	Datum.Init( Data );

	while ( Row != qNIL ) {
		Update( Datum( Row ), RecordRows( Row ) );

		Row = Data.Next( Row );
	}
}

void ndbtbl::table_::Delete( const rrows_ &RecordRows )
{
	_Test( mReadWrite );

	sdr::row__ Row = RecordRows.First();

	while ( Row != qNIL ) {
		Delete( RecordRows( Row ) );

		Row = RecordRows.Next( Row );
	}
}


void ndbtbl::table_::_ResetAllIndexes( void )
{
	sdr::row__ Row = _Indexes.First();

	while ( Row != qNIL ) {
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

	while ( Row != qNIL ) {
		if ( !RecordExists( RecordRows( Row ) ) )
			Rows.Append( Row );

		Row = RecordRows.Next( Row );
	}

}

bso::bool__ ndbtbl::table_::AreAllIndexesSynchronized( void ) const
{
	_Test( mReadOnly );

	sdr::row__ Row = _Indexes.First();

	while ( Row != qNIL ) {
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
qRH
	str::string FileName;
qRB
	FileName.Init();

//	GetFileName_( Path, BaseFileName, FileName );

	_Static.Init( BaseFileName, Convert( Mode ), ID );
qRR
qRT
qRE
}

void ndbtbl::files_hook___::_InitDynamic(
	const str::string_ &BaseFileName,
	mode__ Mode,
	fls::id__ ID )
{
qRH
	str::string FileName;
qRB
	FileName.Init();

//	GetFileName_( Path, RootFileName, FileName );

	_Dynamic.Init( BaseFileName, Convert( Mode ), ID );
qRR
qRT
qRE
}
