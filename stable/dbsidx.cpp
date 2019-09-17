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

#define DBSIDX_COMPILATION_

#include "dbsidx.h"

#include "flf.h"

using namespace dbsidx;

#define TREE_FILE_NAME_EXTENSION	".edt"
#define QUEUE_FILE_NAME_EXTENSION	".edq"
// #define ROOT_FILE_NAME_EXTENSION	".edr"

#define MEMORY_REINDEXATION_LIMIT	10000000
/* Limite du nombre d'neregistrement au-del de laquelle on utilise 
directement l'index sur le disque et non pas une copie temporaire en mmoire
pour viter la mise en oeuvre de la mmoire virtuelle. */

#define RECORD_PANEL_SIZE		50000	// Nombre d'enregistrements par tranche.

#define RECORD_TEST_PANEL_SIZE		1000	// Nombre d'enregistrements pour la tranche de test.


/*
#include "cio.h"

void DisplayTree(
	const idxbtr::E_IBTREEt_( rrow__ ) &Index,
	rrow__ Root,
	txf::text_oflow__ &cout )
{
	rrow__ Row = Index.First( Root );

	while ( Row != NONE ) {
		cout << *Row << txf::tab << txf::sync;

		Row = Index.Next( Row );
	}

	cout << txf::nl;
}

void DisplayQueue(
	const idxque::E_IQUEUEt_( rrow__) &Index,
	rrow__ Root,
	txf::text_oflow__ &cout )
{
	rrow__ Row = Root;

	while ( Root != NONE ) {
		Row = Root;

		Root = Index.Previous( Root );
	}

	while ( Row != NONE ) {
		cout << *Row << txf::tab << txf::sync;

		Row = Index.Next( Row );
	}

	cout << txf::nl;
}

void Display(
	const idxbtq::E_INDEXt_( rrow__) &Index,
	rrow__ Root,
	txf::text_oflow__ &cout )
{
	cout <<  "Q :" << txf::tab << txf::sync;
	DisplayQueue( Index, Root, cout );
	cout << "T :" << txf::tab << txf::sync;
	DisplayTree( Index, Root, cout );
}
*/

bso::sign__ dbsidx::index_::_Seek(
	const datum_ &Datum,
	skip_level__ SkipLevel,
	behavior__ EqualBehavior,
	rrow__ &Row,
	bso::ubyte__ &Round,
	dbsctt::_cache_ &Cache ) const
{
	bso::sign__ Result = 0;
qRH
	datum DatumToCompare;
	idxbtq::E_ISEEKERt__( rrow__ ) Seeker;
qRB
	Round = 0;

	if ( S_.Root == NONE ) {
		Row = NONE;

		qRReturn;
	}

	Seeker.Init( BaseIndex, S_.Root );

	Row = Seeker.GetCurrent();

	while ( Row != NONE ) {
		DatumToCompare.Init();

		_Retrieve( Row, DatumToCompare, Cache );

		switch ( Result = S_.Sort->Compare( Datum, DatumToCompare, SkipLevel ) ) {
		case 0:
			switch ( EqualBehavior ) {
			case bStop:
				Row = NONE;	// Pour sortir de la boucle.
				break;
			case bGreater:
				Row = Seeker.SearchGreater();
				break;
			case bLesser:
				Row = Seeker.SearchLesser();
				break;
			case bStopIfOneChildMissing:
				if ( !Seeker.HasLesser() || !Seeker.HasGreater() )
					Row = NONE;	// Popur sortir de la boucle.
				else if ( *Row & 1 )	// Petit gnrateur alatoire (sans doute inutile ?).
					Row = Seeker.SearchLesser();
				else
					Row = Seeker.SearchGreater();
				break;
			default:
				ERRu();
			}
			break;
		case 1:
			Row = Seeker.SearchGreater();
			break;
		case -1:
			Row = Seeker.SearchLesser();
			break;
		default:
			ERRc();
			break;
		}

		Round++;
	}

	Row = Seeker.GetCurrent();
qRR
qRT
qRE
	return Result;
}

rrow__ dbsidx::index_::_SearchStrictGreater(
	rrow__ Row,
	skip_level__ SkipLevel ) const
{
	rrow__ Buffer = BaseIndex.GetTreeGreater( Row );
	rrow__ Candidate = NONE;

	while ( ( Buffer != NONE ) && ( Compare( Buffer, Row, SkipLevel ) == 0 ) )
		Buffer = BaseIndex.GetTreeGreater( Buffer );

	if ( Buffer != NONE ) {
		Candidate = Buffer;

		Buffer = BaseIndex.GetTreeLesser( Buffer );

		while ( ( Buffer != NONE ) && ( Compare( Buffer, Row, SkipLevel ) != 0 ) ) {
			Candidate = Buffer;

			Buffer = BaseIndex.GetTreeLesser( Buffer );
		}

		if ( Buffer != NONE ) {
			Buffer = _SearchStrictGreater( Buffer, SkipLevel );

			if ( Buffer != NONE )
				Candidate = Buffer;
		}
	} else {
		Buffer = BaseIndex.GetTreeParent( Row );

		if ( Buffer != NONE ) {

			if ( BaseIndex.IsTreeGreater( Row ) ) {
				while ( ( Buffer != NONE ) && BaseIndex.IsTreeGreater( Buffer ) )
					Buffer = BaseIndex.GetTreeParent( Buffer );
			} else
				Buffer = Row;

			if ( Buffer != NONE ) {
				Buffer = BaseIndex.GetTreeParent( Buffer );

				if ( Buffer != NONE ) {
					if ( Compare( Row, Buffer, SkipLevel ) != 0 )
						Candidate = Buffer;
					else
						Candidate = _SearchStrictGreater( Buffer, SkipLevel );
				}
			}
		}
	}

	return Candidate;
}


bso::ubyte__ dbsidx::index_::Index(
	rrow__ Row,
	extremities__ *Extremities,
	dbsctt::_cache_ &Cache )
{
	bso::ubyte__ Round = 0;
qRH
	datum DatumToCompare;
	datum Datum;
	rrow__ TargetRow = NONE;
	bso::sign__ Result;
//	tol::buffer__ Buffer;
//	cio::aware_cout___ cout;
qRB

	if ( _Content( true ).Extent() > BaseIndex.Extent() )
		BaseIndex.Allocate( _Content( true ).Extent(), aem::mDefault );

	if ( S_.Root == NONE ) {
		S_.Root = Row;

		BaseIndex.BecomeRoot( Row );

		qRReturn;
	}

	Datum.Init();

	_Retrieve( Row, Datum, *(dbsctt::_cache_ *)NULL );


	if ( Extremities != NULL ) {
		if ( Extremities->Smallest == NONE )
			Extremities->Smallest = BaseIndex.First( S_.Root );

		TargetRow = Extremities->Smallest;

		DatumToCompare.Init();

		_Retrieve( TargetRow, DatumToCompare, Cache );

		switch ( Result = S_.Sort->Compare( Datum, DatumToCompare, DBSIDX_NO_SKIP ) ) {
		case 0:
			Result = -1;	// Pour forcer son positionnement en tant que premier.
		case -1:
			break;
		case 1:
			TargetRow = NONE;
			break;
		default:
			ERRc();
			break;
		}
	}

	if ( ( TargetRow == NONE ) && ( Extremities != NULL ) ) {
		if ( Extremities->Greatest == NONE )
			Extremities->Greatest = BaseIndex.Last( S_.Root );

		TargetRow = Extremities->Greatest;

		DatumToCompare.Init();

		_Retrieve( TargetRow, DatumToCompare, Cache );

		switch ( Result = S_.Sort->Compare( Datum, DatumToCompare, DBSIDX_NO_SKIP ) ) {
		case 0:
			Result = 1;	// Pour forcer son positionnement en tant que dernier.
		case 1:
			break;
		case -1:
			TargetRow = NONE;
			break;
		default:
			ERRc();
			break;
		}
	} 

	if ( TargetRow == NONE )
		Result = _Seek( Datum, DBSIDX_NO_SKIP, bStopIfOneChildMissing, TargetRow, Round, Cache );
	else
		Extremities->Used++;

//	cout << tol::DateAndTime( Buffer ) << txf::tab << &BaseIndex << txf::nl <<  txf::sync;
//	Display( BaseIndex, S_.Root, cout );

	switch ( Result ) {
	case -1:
		if ( ( Extremities != NULL ) && ( Extremities->Smallest == TargetRow ) )
			Extremities->Smallest = Row;
		S_.Root = BaseIndex.BecomeLesser( Row, TargetRow, S_.Root );
		break;
	case 0:	// Pas de problme avec la gestion des 'extremities', 
		if ( !BaseIndex.TreeHasLesser( TargetRow ) )
			S_.Root = BaseIndex.BecomeLesser( Row, TargetRow, S_.Root );
		else if ( !BaseIndex.TreeHasGreater( TargetRow ) )
			S_.Root = BaseIndex.BecomeGreater( Row, TargetRow, S_.Root );
		else
			ERRc();
		break;
	case 1:
		if ( ( Extremities != NULL ) && ( Extremities->Greatest == TargetRow ) )
			Extremities->Greatest = Row;
		S_.Root = BaseIndex.BecomeGreater( Row, TargetRow, S_.Root );
		break;
	default:
		ERRc();
		break;
	}

//	Display( BaseIndex, S_.Root, cout );

#ifdef DBSIDX_DBG
	if ( ( Round > 32 ) || ( ( 2UL << ( Round >> 1 ) ) > _Content( true ).Amount() ) )
		S_.Root = S_.Root;	// Juste pour pouvoir placer un point d'arrt.
#endif

#ifdef DBSIDX_DBG
	if ( TargetRow == NONE )
		ERRc();
#endif

qRR
qRT
	_Touch( false );
qRE
	return Round;
}

rrow__ dbsidx::index_::LooseSeek( 
	const datum_ &Datum,
	behavior__ EqualBehavior,
	skip_level__ SkipLevel,
	bso::sign__ &Sign ) const
{
	rrow__ Row = NONE;
	bso::ubyte__ Round;

	if ( S_.Root == NONE )
		return NONE;

	Sign = _Seek( Datum, SkipLevel, EqualBehavior, Row, Round, *(dbsctt::_cache_ *)NULL );

#ifdef DBSIDX_DBG
	if ( Row == NONE )
		ERRc();
#endif

	return Row;
}

rrow__ dbsidx::index_::Test( void ) const
{
	rrow__ Row = NONE;
qRH
	datum Datum;
qRB
	if ( S_.Root == NONE )
		qRReturn;

	Row = First();

	Datum.Init();

	_Content( true ).Retrieve( Row, Datum );

	Row = Next( Row );

	while ( Row != NONE ) {
		if ( Compare( Row, Datum, DBSIDX_NO_SKIP ) == 1 )
			qRReturn;

		Datum.Init();

		_Content( true ).Retrieve( Row, Datum );

		Row = Next( Row );
	}
qRR
qRT
qRE
	return Row;
}


bso::sign__ dbsidx::index_::Compare(
	rrow__ RecordRow,
	const datum_ &Pattern,
	skip_level__ SkipLevel ) const
{
	bso::sign__ Result = 0;
qRH
	datum Datum;
qRB
	Datum.Init();

	_Content( true ).Retrieve( RecordRow, Datum, *(dbsctt::_cache_ *)NULL );

	Result = S_.Sort->Compare( Datum, Pattern, SkipLevel  );
qRR
qRT
qRE
	return Result;
}

bso::sign__ dbsidx::index_::Compare(
	rrow__ RecordRow1,
	rrow__ RecordRow2,
	skip_level__ SkipLevel ) const
{
	bso::sign__ Result = 0;
qRH
	datum Pattern;
qRB
	Pattern.Init();

	_Content( true ).Retrieve( RecordRow2, Pattern, *(dbsctt::_cache_ *)NULL );

	Result = Compare( RecordRow1, Pattern, SkipLevel );
qRR
qRT
qRE
	return Result;
}

static inline void Save_(
	rrow__ Row,
	flw::oflow__ &Flow )
{
	dtfptb::PutULong( *Row, Flow );
}

static void Save_(
	rrow__ Row,
	const char *RootFileName )
{
qRH
	flf::file_oflow___ Flow;
qRB
	Flow.Init( RootFileName );

	Save_( Row, Flow );
qRR
qRT
qRE
}

static void Save_(
	rrow__ Row,
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

	Save_( Row, FileName.Convert( FileNameBuffer ) );

	while ( UnderlyingFilesLastModificationTime >= fil::GetFileLastModificationTime( FileNameBuffer ) ) {
		tol::Clock( true );
		fil::TouchFile( FileNameBuffer );
	}

qRR
qRT
qRE
}

/*
void dbsidx::file_index_::_SaveRoot( void ) const
{
	Save_( index_::S_.Root, RootFileName, ROOT_FILE_NAME_EXTENSION, _GetUnderlyingFilesLastModificationTime() );
}
*/

static inline void Load_(
	flw::iflow__ &Flow,
	rrow__ &Row )
{
	Row = dtfptb::GetULong( Flow );
}

static bso::bool__ Load_(
	const char *RootFileName,
	rrow__ &Row,
	time_t TimeStamp )
{
	bso::bool__ Success = false;
qRH
	flf::file_iflow___ Flow;
qRB
	if ( Flow.Init( RootFileName, err::hSkip ) == fil::sSuccess ) {
		if ( fil::GetFileLastModificationTime( RootFileName ) < TimeStamp )
			qRReturn;

		Load_( Flow, Row );

		Success = true;
	}
qRR
qRT
qRE
	return Success;
}

static bso::bool__ Load_(
	const str::string_ &RootFileName,
	rrow__ &Row,
	const char *Extension,
	time_t TimeStamp )
{
	bso::bool__ Success = false;
qRH
	str::string FileName;
	STR_BUFFER___ FileNameBuffer;
qRB
	FileName.Init( RootFileName );
	FileName.Append( Extension );
	Success = Load_( FileName.Convert( FileNameBuffer ), Row, TimeStamp );
qRR
qRT
qRE
	return Success;
}


void dbsidx::exploded_index_::Init(
	const str::string_ &RootFileName,
	dbsctt::content__ &Content,
	sort_function__ &Sort,
	mdr::mode__ Mode,
	bso::bool__ Erase,
	bso::bool__ Partial,
	flm::id__ ID )
{
qRH
	str::string TreeFileName;
	STR_BUFFER___ TreeFileNameBuffer;
	str::string QueueFileName;
	STR_BUFFER___ QueueFileNameBuffer;
qRB
	reset();

	TreeFileName.Init( RootFileName );
	TreeFileName.Append( TREE_FILE_NAME_EXTENSION );

	QueueFileName.Init( RootFileName );
	QueueFileName.Append( QUEUE_FILE_NAME_EXTENSION );

	S_.FileManager.Init( TreeFileName.Convert( TreeFileNameBuffer ), QueueFileName.Convert( QueueFileNameBuffer ), Mode, true, ID );

	this->RootFileName.Init( RootFileName );

	index_::Init( Content, Sort, Partial );

	S_.Mode = Mode;
	S_.Erase = Erase;
qRR
qRT
qRE
}

static inline void Reindex_(
	rrows_ &Rows,
	index_ &Index,
	observer_functions__ &Observer,
	dbsctt::_cache_  &Cache,
	tol::chrono__ &Chrono,
	bso::ulong__ &HandledRecordAmount,
	bso::ulong__ &BalancingCount,
	tol::E_DPOINTER___( extremities__ ) &Extremities,
	bso::bool__ Randomly )
{
	epeios::row__ Row = NONE;
	bso::ubyte__ Round = 0;

	while ( Rows.Amount() ) {
		if ( Randomly )
			Row = Rows.Amount() - ( rand() % Rows.Amount() ) - 1;
		else
			Row = Rows.First();

		Round = Index.Index( Rows( Row ), Extremities, Cache );

		Rows.Remove( Row );

		if ( ( 1UL << ( Round >> 3 ) ) > HandledRecordAmount ) {
			Index.Balance();
			BalancingCount++;
			if ( ( Extremities == NULL ) && ( BalancingCount > 1 ) )
				Extremities = new extremities__;
		}

		HandledRecordAmount++;

		if ( ( &Observer != NULL ) && Chrono.IsElapsed() ) {
			Observer.Notify( HandledRecordAmount, Index.Content( true ).Amount(), BalancingCount );

			Chrono.Launch();
		}
	}
}

void dbsidx::index_::Reindex( observer_functions__ &Observer )
{
qRH
	dbsctt::content__ &Content = *S_.Content;
	mdr::size__ HandledRecordAmount = 0;
	tol::chrono__ Chrono;
	dbsidx::index IndexInMemory;
	dbsidx::index_ *UsedIndex = NULL;
	dbsbsc::_cache  Cache;
	tol::E_DPOINTER___( extremities__ ) Extremities;
	bso::ulong__ BalancingCount = 0;
	bch::E_BUNCH( rrow__ ) Rows;
	rrow__ Row = NONE;
	bso::ulong__ PanelRecordCounter;
	bso::ulong__ PanelRecordSize;
	bso::bool__ Randomly = false;
qRB
	Reset();

	if ( Content.Amount() == 0 )
		qRReturn;

	if ( Content.Extent() < MEMORY_REINDEXATION_LIMIT ) {
		IndexInMemory.Init( Content, SortFunction() );

		IndexInMemory.Allocate( Content.Extent(), aem::mDefault );

		UsedIndex = &IndexInMemory;
	} else
		UsedIndex = this;

	Cache.Init( Content.Extent() );

	Rows.Init();

	Row = Content.First();

	PanelRecordSize = RECORD_TEST_PANEL_SIZE;

	PanelRecordCounter = PanelRecordSize;

	if ( ( &Observer != NULL ) && ( Content.Amount() != 0 ) ) {
		Observer.Notify( 0, Content.Amount(), BalancingCount );
		Chrono.Init( Observer._Delay );
		Chrono.Launch();
	}

	while ( Row != NONE ) {
		Rows.Append( Row );

		if ( PanelRecordCounter-- == 0 ) {
			Reindex_( Rows, *UsedIndex, Observer, Cache, Chrono, HandledRecordAmount, BalancingCount, Extremities, Randomly );

			if ( Randomly == false )
				if ( ( Extremities == NULL ) || ( Extremities->Used < ( ( 2 * PanelRecordSize ) / 3 ) ) )
					Randomly = true;
				else
					Extremities->Used = 0;


			PanelRecordSize = RECORD_PANEL_SIZE;

			PanelRecordCounter = PanelRecordSize;
		}

		Row = Content.Next( Row );
	}

	Reindex_( Rows, *UsedIndex, Observer, Cache, Chrono, HandledRecordAmount, BalancingCount, Extremities, Randomly );

	if ( ( &Observer != NULL ) && ( Content.Amount() != 0 ) )
		Observer.Notify( HandledRecordAmount, Content.Amount(), BalancingCount );

	UsedIndex->Balance();

	if ( UsedIndex != this )
		this->operator =( *UsedIndex );
qRR
qRT
qRE
}

