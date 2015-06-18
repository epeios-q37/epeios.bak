/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#define DBSTBL__COMPILATION

#include "dbstbl.h"

#include "fnm.h"

using namespace dbstbl;

void dbstbl::table_::_InsertInIndexes( rrow__ Row )
{
	epeios::row__ IRow = Indexes.First();

	while ( IRow != NONE ) {
		Indexes( IRow )->Index( Row, false );

		IRow = Indexes.Next( IRow );
	}
}

void dbstbl::table_::_DeleteFromIndexes( rrow__ Row )
{
	epeios::row__ IRow = Indexes.First();

	while ( IRow != NONE ) {
		if ( !Indexes( IRow )->InitializationCompleted() )
			Indexes( IRow )->CompleteInitialization();

		Indexes( IRow )->Delete( Row );

		IRow = Indexes.Next( IRow );
	}
}

void dbstbl::table_::_ReindexAll( observer_functions__ &Observer )
{
	_Test( mReadWrite );

	epeios::row__ Row = Indexes.First();

	if ( &Observer != NULL ) {
		Observer.Set( Indexes.Amount() );
	}

	while ( Row != NONE ) {
		Indexes( Row )->Reindex( Observer );

		if ( &Observer )
			Observer.IncrementHandledIndexAmount();

		Row = Indexes.Next( Row );
	}
}

void dbstbl::table_::Retrieve(
	const rrows_ &Rows,
	data_ &Data ) const
{
qRH
	datum Datum;
	epeios::row__ Row = NONE;
qRB
	_Test( mReadOnly );

	const dbsctt::content__ &Content = _C();

	Row = Rows.First();

	while ( Row != NONE ) {
		Datum.Init();

		Content.Retrieve( Rows( Row ), Datum );

		Data.Append( Datum );

		Row = Rows.Next( Row );
	}
qRR
qRT
qRE
}

void dbstbl::table_::Insert(
	const data_ &Data,
	rrows_ &RecordRows )
{
	_Test( mReadWrite );

	ctn::E_CMITEM( datum_ ) Datum;
	epeios::row__ Row = Data.First();

	Datum.Init( Data );

	while ( Row != NONE ) {
		RecordRows.Append( Insert( Datum( Row ) ) );

		Row = Data.Next( Row );
	}
}

void dbstbl::table_::Update(
	const data_ &Data,
	const rrows_ &RecordRows )
{
	if ( Data.Amount() != RecordRows.Amount() )
		ERRu();

	_Test( mReadWrite );

	ctn::E_CMITEM( datum_ ) Datum;
	epeios::row__ Row = Data.First();

	Datum.Init( Data );

	while ( Row != NONE ) {
		Update( Datum( Row ), RecordRows( Row ) );

		Row = Data.Next( Row );
	}
}

void dbstbl::table_::Delete( const rrows_ &RecordRows )
{
	_Test( mReadWrite );

	epeios::row__ Row = RecordRows.First();

	while ( Row != NONE ) {
		Delete( RecordRows( Row ) );

		Row = RecordRows.Next( Row );
	}
}


void dbstbl::table_::_ResetAllIndexes( void )
{
	epeios::row__ Row = Indexes.First();

	while ( Row != NONE ) {
		Indexes( Row )->Reset();

		Row = Indexes.Next( Row );
	}
}

void dbstbl::table_::TestRecordsExistence(
	const rrows_ &RecordRows,
	rows_ &Rows ) const
{
	_Test( mReadOnly );

	epeios::row__ Row = RecordRows.First();

	while ( Row != NONE ) {
		if ( !RecordExists( RecordRows( Row ) ) )
			Rows.Append( Row );

		Row = RecordRows.Next( Row );
	}

}


bso::bool__ dbstbl::table_::AreAllIndexesSynchronized( void ) const
{
	_Test( mReadOnly );

	epeios::row__ Row = Indexes.First();

	while ( Row != NONE ) {
		if ( !Indexes( Row )->IsSynchronized() )
			return false;

		Row = Indexes.Next( Row );
	}

	return true;
}


#ifdef DBSTBL__THREAD_SAFE

#define RO	const table_ &T = _Lock();
#define RW	table_ &T = _Lock();

#define RRO	_Release();
#define RRW	_Release();

void dbstbl::thread_safe_table_::DBSTBLErasePhysically( void )
{
qRH
qRB
	RW

	T.ErasePhysically();
qRR
qRT
	RRW
qRE
}

void dbstbl::thread_safe_table_::AddIndex( index_ &Index )
{
qRH
qRB
	RW

	T.AddIndex( Index );
qRR
qRT
	RRW
qRE
}

rrow__ dbstbl::thread_safe_table_::Insert( const datum_ &Datum )
{
	rrow__ Row = NONE;
qRH
qRB
	RW

	Row = T.Insert( Datum );
qRR
qRT
	RRW
qRE
	return Row;
}

void dbstbl::thread_safe_table_::Insert(
	const data_ &Data,
	rrows_ &RecordRows )
{
qRH
	ctn::E_CMITEM( datum_ ) Datum;
	epeios::row__ Row = NONE;
qRB
	RW

	epeios::row__ Row = Data.First();

	Datum.Init( Data );

	while ( Row != NONE ) {
		RecordRows.Append( T.Insert( Datum( Row ) ) );

		Row = Data.Next( Row );
	}
qRR
qRT
	RRW
qRE
}

void dbstbl::thread_safe_table_::Update(
	const datum_ &Datum,
	rrow__ Row )
{
qRH
qRB
	RW

	T.Update( Datum, Row );
qRR
qRT
	RRW
qRE
}

void dbstbl::thread_safe_table_::Update(
	const data_ &Data,
	const rrows_ &RecordRows )
{
qRH
	ctn::E_CMITEM( datum_ ) Datum;
	epeios::row__ Row = NONE;
qRB
	RW

	if ( Data.Amount() != RecordRows.Amount() )
		ERRu();

	Row = Data.First();

	Datum.Init( Data );

	while ( Row != NONE ) {
		T.Update( Datum( Row ), RecordRows( Row ) );

		Row = Data.Next( Row );
	}
qRR
qRT
	RRW
qRE
}

void dbstbl::thread_safe_table_::Retrieve(
	rrow__ Row,
	datum_ &Datum )
{
qRH
qRB
	RO

	T.Retrieve( Row, Datum );
qRR
qRT
	RRO
qRE
}

void dbstbl::thread_safe_table_::Retrieve(
	const rrows_ &Rows,
	data_ &Data )
{
qRH
	datum Datum;
	epeios::row__ Row = NONE;
qRB
	RO

	Row = Rows.First();

	while ( Row != NONE ) {
		Datum.Init();

		T.Retrieve( Rows( Row ), Datum );

		Data.Append( Datum );

		Row = Rows.Next( Row );
	}
qRR
qRT
	RRO
qRE
}

void dbstbl::thread_safe_table_::Delete( rrow__ Row )
{
qRH
qRB
	RW

	T.Delete( Row );
qRR
qRT
	RRW
qRE
}

void dbstbl::thread_safe_table_::Delete( const rrows_ &RecordRows )
{
qRH
	epeios::row__ Row = NONE;
qRB
	RW

	Row = RecordRows.First();

	while ( Row != NONE ) {
		T.Delete( RecordRows( Row ) );

		Row = RecordRows.Next( Row );
	}
qRR
qRT
	RRW
qRE
}

#if 0
rrow__ dbstbl::thread_safe_table_::LooseSeek(
	const datum_ &Datum,
	irow__ IRow,
	behavior__ EqualBehavior,
	skip_level__ SkipLevel,
	bso::sign__ &Sign )
{
	rrow__ Row = NONE;
qRH
qRB
	RO

	Row = T.LooseSeek( Datum, IRow, EqualBehavior, SkipLevel, Sign );
qRR
qRT
	RRO
qRE
	return Row;
}

rrow__ dbstbl::thread_safe_table_::StrictSeek(
	const datum_ &Datum,
	irow__ IRow,
	behavior__ EqualBehavior,
	skip_level__ SkipLevel )
{
	rrow__ Row = NONE;
qRH
qRB
	RO

	Row = T.StrictSeek( Datum, IRow, EqualBehavior, SkipLevel );
qRR
qRT
	RRO
qRE
	return Row;
}

rrow__ dbstbl::thread_safe_table_::First( irow__ IRow )
{
	rrow__ Row = NONE;
qRH
qRB
	RO

	Row = T.First( IRow );
qRR
qRT
	RRO
qRE
	return Row;
}

rrow__ dbstbl::thread_safe_table_::Last( irow__ IRow )
{
	rrow__ Row = NONE;
qRH
qRB
	RO

	Row = T.Last( IRow );
qRR
qRT
	RRO
qRE
	return Row;
}

rrow__ dbstbl::thread_safe_table_::Next( 
	irow__ IRow,
	rrow__ Row )
{
qRH
qRB
	RO

	Row = T.Next( IRow, Row );
qRR
qRT
	RRO
qRE
	return Row;
}

rrow__ dbstbl::thread_safe_table_::GetStrictGreater( 
	irow__ IRow,
	rrow__ Row,
	skip_level__ SkipLevel )
{
qRH
qRB
	RO

	Row = T.GetStrictGreater( IRow, Row, SkipLevel );
qRR
qRT
	RRO
qRE
	return Row;
}

rrow__ dbstbl::thread_safe_table_::Previous( 
	irow__ IRow,
	rrow__ Row )
{
qRH
qRB
	RO

	Row = T.Previous( IRow, Row );
qRR
qRT
	RRO
qRE
	return Row;
}

mdr::size__ dbstbl::thread_safe_table_::Amount( void )
{
	mdr::size__ Amount;
qRH
qRB
	RO

	Amount = T.Amount();
qRR
qRT
	RRO
qRE
	return Amount;
}
#endif

mdr::size__ dbstbl::thread_safe_table_::Extent( void )
{
	mdr::size__ Extent;
qRH
qRB
	RO

	Extent = T.Extent();
qRR
qRT
	RRO
qRE
	return Extent;
}

dbstbl::mode__ dbstbl::thread_safe_table_::SwitchMode( dbstbl::mode__ Mode )
{
qRH
qRB
	RW

	Mode = T.SwitchMode( Mode );
qRR
qRT
	RRW
qRE
	return Mode;
}

dbstbl::mode__ dbstbl::thread_safe_table_::Mode( void )
{
	mode__ Mode = m_Undefined;
qRH
qRB
	RO

	Mode = T.Mode();
qRR
qRT
	RRO
qRE
	return Mode;
}

bso::bool__ dbstbl::thread_safe_table_::RecordExists( rrow__ RecordRow )
{
	bso::bool__ Exists = false;
qRH
qRB
	RO

	Exists = T.RecordExists( RecordRow );
qRR
qRT
	RRO
qRE
	return Exists;
}

void dbstbl::thread_safe_table_::TestRecordsExistence(
	const rrows_ &RecordRows,
	rows_ &Rows )
{
qRH
	epeios::row__ Row = NONE;
qRB
	RO

	Row = RecordRows.First();

	while ( Row != NONE ) {
		if ( !T.RecordExists( RecordRows( Row ) ) )
			Rows.Append( Row );

		Row = RecordRows.Next( Row );
	}

qRR
qRT
	RRO
qRE
}

#if 0
bso::sign__ dbstbl::thread_safe_table_::Compare(
	rrow__ RecordRow,
	const datum_&Pattern,
	irow__ IndexRow,
	skip_level__ SkipLevel )
{
	bso::sign__ Result = 0;
qRH
qRB
	RO

	Result = T.Compare( RecordRow, Pattern, IndexRow, SkipLevel );
qRR
qRT
	RRO
qRE
	return Result;
}

bso::bool__ dbstbl::thread_safe_table_::IsIndexSynchronized( irow__ IndexRow )
{
	bso::bool__ Result = false;
qRH
qRB
	RO

	Result = T.IsIndexSynchronized( IndexRow );
qRR
qRT
	RRO
qRE
	return Result;
}
#endif

bso::bool__ dbstbl::thread_safe_table_::AreAllIndexesSynchronized( void )
{
	bso::bool__ Result = false;
qRH
qRB
	RO

	Result = T.AreAllIndexesSynchronized();
qRR
qRT
	RRO
qRE
	return Result;
}

#if 0
void dbstbl::thread_safe_table_::Reindex(
	irow__ IndexRow,
	observer_functions__ &Observer )
{
qRH
qRB
	RW

	T.Reindex( IndexRow, Observer );
qRR
qRT
	RRW
qRE
}
#endif

void dbstbl::thread_safe_table_::ReindexAll( observer_functions__ &Observer )
{
qRH
qRB
	RW

	T.ReindexAll( Observer );
qRR
qRT
	RRW
qRE
}

#endif

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

void dbstbl::exploded_table::InitStatic(
	epeios::size__ Size,									
	const str::string_ &Path,
	const str::string_ &RootFileName,
	mode__ Mode,
	flm::id__ ID )
{
qRH
	str::string FileName;
qRB
	FileName.Init();

	GetFileName_( Path, RootFileName, FileName );

	if ( Mode == dbstbl::mReadOnly )
		_Static.Init( Size, FileName, mdr::mReadOnly, true, ID );
	else
		_Static.Init( Size, FileName, mdr::mReadWrite, true, ID );

	Content.Init( _Static );

	table::Init( RootFileName, Content, Mode );
qRR
qRT
qRE
}

void dbstbl::exploded_table::InitDynamic(
	const str::string_ &Path,
	const str::string_ &RootFileName,
	mode__ Mode,
	flm::id__ ID )
{
qRH
	str::string FileName;
qRB
	FileName.Init();

	GetFileName_( Path, RootFileName, FileName );

	if ( Mode == dbstbl::mReadOnly )
		_Dynamic.Init( FileName, mdr::mReadOnly, true, ID );
	else
		_Dynamic.Init( FileName, mdr::mReadWrite, true, ID );

	Content.Init( _Dynamic );

	table::Init( RootFileName, Content, Mode );
qRR
qRT
qRE
}
