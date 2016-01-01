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

#define NDBIDX__COMPILATION

#include "ndbidx.h"

using namespace ndbidx;

#define TREE_FILE_NAME_EXTENSION	".edt"
#define QUEUE_FILE_NAME_EXTENSION	".edq"

#define MEMORY_REINDEXATION_LIMIT	10000000
/* Limite du nombre d'neregistrement au-del de laquelle on utilise 
directement l'index sur le disque et non pas une copie temporaire en mmoire
pour viter la mise en oeuvre de la mmoire virtuelle. */

#define RECORD_PANEL_SIZE		50000	// Nombre d'enregistrements par tranche.

#define RECORD_TEST_PANEL_SIZE		1000	// Nombre d'enregistrements pour la tranche de test.


bso::sign__ ndbidx::index_::_Seek(
	const datum_ &Datum,
	context__ Context,
	behavior__ EqualBehavior,
	rrow__ &Row,
	bso::u8__ &Round,
	ndbctt::cache_ &Cache ) const
{
	bso::sign__ Result = 0;
qRH
	datum DatumToCompare;
	idxbtq::E_ISEEKERt__( rrow__ ) Seeker;
qRB
	_CompleteInitialization();

	Round = 0;

	if ( S_.Root == qNIL ) {
		Row = qNIL;

		qRReturn;
	}

	Seeker.Init( _Index(), S_.Root );

	Row = Seeker.GetCurrent();

	while ( Row != qNIL ) {
		DatumToCompare.Init();

		if ( !_Retrieve( Row, DatumToCompare, Cache ) )
			qRFwk();

		switch ( Result = _SortPointer->Compare( Datum, DatumToCompare, Context ) ) {
		case 0:
			switch ( EqualBehavior ) {
			case bStop:
				Row = qNIL;	// Pour sortir de la boucle.
				break;
			case bGreater:
				Row = Seeker.SearchGreater();
				break;
			case bLesser:
				Row = Seeker.SearchLesser();
				break;
			case bStopIfOneChildMissing:
				if ( !Seeker.HasLesser() || !Seeker.HasGreater() )
					Row = qNIL;	// Popur sortir de la boucle.
				else if ( *Row & 1 )	// Petit gnrateur alatoire (sans doute inutile ?).
					Row = Seeker.SearchLesser();
				else
					Row = Seeker.SearchGreater();
				break;
			default:
				qRFwk();
			}
			break;
		case 1:
			Row = Seeker.SearchGreater();
			break;
		case -1:
			Row = Seeker.SearchLesser();
			break;
		default:
			qRFwk();
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

rrow__ ndbidx::index_::_SearchStrictGreater(
	rrow__ Row,
	context__ Context ) const
{
	_CompleteInitialization();	

	rrow__ Buffer = _Index().GetTreeGreater( Row );
	rrow__ Candidate = qNIL;

	while ( ( Buffer != qNIL ) && ( Compare( Buffer, Row, Context ) == 0 ) )
		Buffer = _Index().GetTreeGreater( Buffer );

	if ( Buffer != qNIL ) {
		Candidate = Buffer;

		Buffer = _Index().GetTreeLesser( Buffer );

		while ( ( Buffer != qNIL ) && ( Compare( Buffer, Row, Context ) != 0 ) ) {
			Candidate = Buffer;

			Buffer = _Index().GetTreeLesser( Buffer );
		}

		if ( Buffer != qNIL ) {
			Buffer = _SearchStrictGreater( Buffer, Context );

			if ( Buffer != qNIL )
				Candidate = Buffer;
		}
	} else {
		Buffer = _Index().GetTreeParent( Row );

		if ( Buffer != qNIL ) {

			if ( _Index().IsTreeGreater( Row ) ) {
				while ( ( Buffer != qNIL ) && _Index().IsTreeGreater( Buffer ) )
					Buffer = _Index().GetTreeParent( Buffer );
			} else
				Buffer = Row;

			if ( Buffer != qNIL ) {
				Buffer = _Index().GetTreeParent( Buffer );

				if ( Buffer != qNIL ) {
					if ( Compare( Row, Buffer, Context ) != 0 )
						Candidate = Buffer;
					else
						Candidate = _SearchStrictGreater( Buffer, Context );
				}
			}
		}
	}

	return Candidate;
}

rrow__ ndbidx::index_::_SearchStrictLesser(
	rrow__ Row,
	context__ Context ) const
{
	_CompleteInitialization();	

	rrow__ Buffer = _Index().GetTreeLesser( Row );
	rrow__ Candidate = qNIL;

	while ( ( Buffer != qNIL ) && ( Compare( Buffer, Row, Context ) == 0 ) )
		Buffer = _Index().GetTreeLesser( Buffer );

	if ( Buffer != qNIL ) {
		Candidate = Buffer;

		Buffer = _Index().GetTreeGreater( Buffer );

		while ( ( Buffer != qNIL ) && ( Compare( Buffer, Row, Context ) != 0 ) ) {
			Candidate = Buffer;

			Buffer = _Index().GetTreeGreater( Buffer );
		}

		if ( Buffer != qNIL ) {
			Buffer = _SearchStrictLesser( Buffer, Context );

			if ( Buffer != qNIL )
				Candidate = Buffer;
		}
	} else {
		Buffer = _Index().GetTreeParent( Row );

		if ( Buffer != qNIL ) {

			if ( _Index().IsTreeLesser( Row ) ) {
				while ( ( Buffer != qNIL ) && _Index().IsTreeLesser( Buffer ) )
					Buffer = _Index().GetTreeParent( Buffer );
			} else
				Buffer = Row;

			if ( Buffer != qNIL ) {
				Buffer = _Index().GetTreeParent( Buffer );

				if ( Buffer != qNIL ) {
					if ( Compare( Row, Buffer, Context ) != 0 )
						Candidate = Buffer;
					else
						Candidate = _SearchStrictLesser( Buffer, Context );
				}
			}
		}
	}

	return Candidate;
}


bso::u8__ ndbidx::index_::Index(
	rrow__ Row,
	extremities__ *Extremities,
	ndbctt::cache_ &Cache )
{
	bso::u8__ Round = 0;
qRH
	datum DatumToCompare;
	datum Datum;
	rrow__ TargetRow = qNIL;
	bso::sign__ Result;
//	tol::buffer__ Buffer;
//	cio::aware_cout___ cout;
qRB
	if ( _Bufferized )
		qRFwk();

	_CompleteInitialization();

	if ( _Content().Amount() > DIndex.Amount() )
		DIndex.Allocate( _Content().Amount(), aem::m_Default );

	if ( S_.Root == qNIL ) {
		S_.Root = Row;

		DIndex.BecomeRoot( Row );

		qRReturn;
	}

	Datum.Init();

	if ( !_Retrieve( Row, Datum, *(ndbctt::cache_ *)NULL ) )
		qRReturn;	// L'enregistrement n'existe pas, on ne va donc pas l'inclure dans l'index.
					// Ce cas ne devrait pas arriv, sauf lorsqu'il y a eu des problmes de corruption corrigs  la main.

	if ( Extremities != NULL ) {
		if ( Extremities->Smallest == qNIL )
			Extremities->Smallest = DIndex.First( S_.Root );

		TargetRow = Extremities->Smallest;

		DatumToCompare.Init();

		if ( !_Retrieve( TargetRow, DatumToCompare, Cache ) )
			qRFwk();

		switch ( Result = _SortPointer->Compare( Datum, DatumToCompare, cIndexation ) ) {
		case 0:
			Result = -1;	// Pour forcer son positionnement en tant que premier.
		case -1:
			break;
		case 1:
			TargetRow = qNIL;
			break;
		default:
			qRFwk();
			break;
		}
	}

	if ( ( TargetRow == qNIL ) && ( Extremities != NULL ) ) {
		if ( Extremities->Greatest == qNIL )
			Extremities->Greatest = DIndex.Last( S_.Root );

		TargetRow = Extremities->Greatest;

		DatumToCompare.Init();

		if ( !_Retrieve( TargetRow, DatumToCompare, Cache ) )
			qRFwk();

		switch ( Result = _SortPointer->Compare( Datum, DatumToCompare, cIndexation ) ) {
		case 0:
			Result = 1;	// Pour forcer son positionnement en tant que dernier.
		case 1:
			break;
		case -1:
			TargetRow = qNIL;
			break;
		default:
			qRFwk();
			break;
		}
	} 

	if ( TargetRow == qNIL )
		Result = _Seek( Datum, cIndexation, bStopIfOneChildMissing, TargetRow, Round, Cache );
	else
		Extremities->Used++;

//	cout << tol::DateAndTime( Buffer ) << txf::tab << &BaseIndex << txf::nl <<  txf::sync;
//	Display( BaseIndex, S_.Root, cout );

	switch ( Result ) {
	case -1:
		if ( ( Extremities != NULL ) && ( Extremities->Smallest == TargetRow ) )
			Extremities->Smallest = Row;
		S_.Root = DIndex.BecomeLesser( Row, TargetRow, S_.Root );
		break;
	case 0:	// Pas de problme avec la gestion des 'extremities', 
		if ( !DIndex.TreeHasLesser( TargetRow ) )
			S_.Root = DIndex.BecomeLesser( Row, TargetRow, S_.Root );
		else if ( !DIndex.TreeHasGreater( TargetRow ) )
			S_.Root = DIndex.BecomeGreater( Row, TargetRow, S_.Root );
		else
			qRFwk();
		break;
	case 1:
		if ( ( Extremities != NULL ) && ( Extremities->Greatest == TargetRow ) )
			Extremities->Greatest = Row;
		S_.Root = DIndex.BecomeGreater( Row, TargetRow, S_.Root );
		break;
	default:
		qRFwk();
		break;
	}

//	Display( BaseIndex, S_.Root, cout );

#ifdef NDBIDX_DBG
	if ( ( Round > 32 ) || ( ( 2UL << ( Round >> 1 ) ) > _Content().Amount() ) )
		S_.Root = S_.Root;	// Juste pour pouvoir placer un point d'arrt.
#endif

#ifdef NDBIDX_DBG
	if ( TargetRow == qNIL )
		qRFwk();
#endif

qRR
qRT
	_Touch( false );
qRE
	return Round;
}

rrow__ ndbidx::index_::LooseSeek( 
	const datum_ &Datum,
	behavior__ EqualBehavior,
	context__ Context,
	bso::sign__ &Sign ) const
{
	rrow__ Row = qNIL;
	bso::u8__ Round;

	_CompleteInitialization();

	if ( S_.Root == qNIL )
		return qNIL;

	Sign = _Seek( Datum, Context, EqualBehavior, Row, Round, *(ndbctt::cache_ *)NULL );

#ifdef NDBIDX_DBG
	if ( Row == qNIL )
		qRFwk();
#endif

	return Row;
}

rrow__ ndbidx::index_::Test( void ) const
{
	rrow__ Row = qNIL;
qRH
	datum Datum;
qRB
	_CompleteInitialization();

	if ( S_.Root == qNIL )
		qRReturn;

	Row = First();

	Datum.Init();

	_Content().Retrieve( Row, Datum );

	Row = Next( Row );

	while ( Row != qNIL ) {
		if ( Compare( Row, Datum, cIndexation ) == 1 )
			qRReturn;

		Datum.Init();

		_Content().Retrieve( Row, Datum );

		Row = Next( Row );
	}
qRR
qRT
qRE
	return Row;
}


bso::sign__ ndbidx::index_::Compare(
	rrow__ RecordRow,
	const datum_ &Pattern,
	context__ Context ) const
{
	bso::sign__ Result = 0;
qRH
	datum Datum;
qRB
	Datum.Init();

	_CompleteInitialization();

	if ( !_Retrieve( RecordRow, Datum, *(ndbctt::cache_ *)NULL ) )
		qRFwk();

	Result = _SortPointer->Compare( Datum, Pattern, Context  );
qRR
qRT
qRE
	return Result;
}

bso::sign__ ndbidx::index_::Compare(
	rrow__ RecordRow1,
	rrow__ RecordRow2,
	context__ Context ) const
{
	bso::sign__ Result = 0;
qRH
	datum Pattern;
qRB
	Pattern.Init();

	_CompleteInitialization();

	if ( !_Retrieve( RecordRow2, Pattern, *(ndbctt::cache_ *)NULL ) )
		qRFwk();

	Result = Compare( RecordRow1, Pattern, Context );
qRR
qRT
qRE
	return Result;
}

static inline void Reindex_(
	rrows_ &Rows,
	index_ &Index,
	observer_functions__ &Observer,
	ndbctt::cache_  &Cache,
	tol::timer__ &Timer,
	bso::size__ &HandledRecordAmount,
	bso::uint__ &BalancingCount,
	tol::E_DPOINTER___( extremities__ ) &Extremities,
	bso::bool__ Randomly )
{
	sdr::row__ Row = qNIL;
	bso::u8__ Round = 0;

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

		if ( ( &Observer != NULL ) && Timer.IsElapsed() ) {
			Observer.Notify( HandledRecordAmount, Index.Content().Amount(), BalancingCount );

			Timer.Launch();
		}
	}
}

void ndbidx::index_::Reindex( observer_functions__ &Observer )
{
qRH
	const ndbctt::content__ &Content = _Content();
	sdr::size__ HandledRecordAmount = 0;
	tol::timer__ Timer;
	ndbidx::index IndexInMemory;
	ndbidx::index_ *UsedIndex = NULL;
	ndbbsc::cache  Cache;
	tol::E_DPOINTER___( extremities__ ) Extremities;
	bso::uint__ BalancingCount = 0;
	bch::E_BUNCH( rrow__ ) Rows;
	rrow__ Row = qNIL;
	bso::uint__ PanelRecordCounter;
	bso::uint__ PanelRecordSize;
	bso::bool__ Randomly = false;
qRB
	_CompleteInitialization();

	Reset();

	if ( Content.Amount() == 0 )
		qRReturn;

	if ( Content.Extent() < MEMORY_REINDEXATION_LIMIT ) {
		IndexInMemory.Init( *_ContentPointer, SortFunction() );

		IndexInMemory.Allocate( Content.Extent(), aem::m_Default );

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
		Timer.Init( Observer._Delay );
		Timer.Launch();
	}

	while ( Row != qNIL ) {
		Rows.Append( Row );

		if ( PanelRecordCounter-- == 0 ) {
			Reindex_( Rows, *UsedIndex, Observer, Cache, Timer, HandledRecordAmount, BalancingCount, Extremities, Randomly );

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

	Reindex_( Rows, *UsedIndex, Observer, Cache, Timer, HandledRecordAmount, BalancingCount, Extremities, Randomly );

	if ( ( &Observer != NULL ) && ( Content.Amount() != 0 ) )
		Observer.Notify( HandledRecordAmount, Content.Amount(), BalancingCount );

	UsedIndex->Balance();

	if ( UsedIndex != this )
		this->operator =( *UsedIndex );
qRR
qRT
qRE
}


void ndbidx::files_hook___::Init(
	const str::string_ &BaseFileName,
	bso::bool__ Erase,
	fil::mode__ Mode,
	fls::id__ ID )
{
qRH
	str::string TreeFileName;
	TOL_CBUFFER___ TreeFileNameBuffer;
	str::string QueueFileName;
	TOL_CBUFFER___ QueueFileNameBuffer;
qRB
	reset();

	TreeFileName.Init( BaseFileName );
	TreeFileName.Append( TREE_FILE_NAME_EXTENSION );

	QueueFileName.Init( BaseFileName );
	QueueFileName.Append( QUEUE_FILE_NAME_EXTENSION );

	_Hook.Init( TreeFileName.Convert( TreeFileNameBuffer ), QueueFileName.Convert( QueueFileNameBuffer ), Mode, true, ID );

	_BaseFileName.Init( BaseFileName );

	_Mode = Mode;

	if ( Erase )
		_ErasePhysically();
qRR
qRT
qRE
}

