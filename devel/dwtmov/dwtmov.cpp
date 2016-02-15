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

#define DWTMOV__COMPILATION

#include "dwtmov.h"

#include "idxbtq.h"
#include "dir.h"

using namespace dwtmov;

static stsfsm::automat VersionAutomat_;

const char *dwtmov::GetLabel( version__ Version )
{
	switch ( Version ) {
	case v0_1:
		return "0.1";
		break;
	case v0_2:
		return "0.2";
		break;
	default:
		qRFwk();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

version__ dwtmov::GetVersion( const str::string_ &Pattern )
{
	return stsfsm::GetId( Pattern, VersionAutomat_, v_Undefined, v_amount );
}

static void FillVersionAutomat_( void )
{
	VersionAutomat_.Init();
	stsfsm::Fill<version__>( VersionAutomat_, v_amount, GetLabel );
}

E_ROW( drow__);

class core__ {
public:
	drow__ Parent;
	nrow__ Name;
	void reset( bso::bool__ = true )
	{
		Parent = qNIL;
		Name = qNIL;
	}
	E_CDTOR( core__ );
	void Init( void )
	{
		Parent = qNIL;
		Name = qNIL;
	}
};

class dir__ {
public:
	core__
		Old,
		New;
	void reset( bso::bool__ P = true )
	{
		Old.reset( P );
		New.reset( P );
	}
	E_CDTOR( dir__);
	void Init( void )
	{
		Old.Init();
		New.Init();
	}
};

typedef bch::E_BUNCHt_( dir__, drow__) dirs_;
E_AUTO( dirs );

class bundle_
{
public:
	struct s {
		dirs_::s Dirs;
		names_::s Names;
	};
	dirs_ Dirs;
	names_ Names;
	bundle_( s &S )
	: Dirs( S.Dirs ),
	  Names( S.Names )
	{}
	void reset( bso::bool__ P = true )
	{
		Dirs.reset( P );
		Names.reset( P );
	}
	void plug( qAS_ &AS )
	{
		Dirs.plug( AS );
		Names.plug( AS );
	}
	bundle_ &operator = (const bundle_ &B )
	{
		Dirs = B.Dirs;
		Names = B.Names;

		return *this;
	}
	void Init( void )
	{
		Dirs.Init();
		Names.Init();
	}
	E_NAVt( Dirs., drow__ );
};

E_AUTO( bundle );

typedef bch::E_BUNCHt_( dwtdct::irow__, dwtght::grow__ ) gtoi_;	// Garde la trace pour chaque 'ghost' de l'item l'utilisant.
																	// Pour éviter qu'un 'ghost' soit utilisé par deux items (duplication de répertoire).

E_AUTO( gtoi );

typedef bch::E_BUNCHt_( drow__, dwtdct::irow__ ) itod_;
E_AUTO( itod );

typedef bch::E_BUNCHt_( drow__, dwtght::grow__ ) gtod_;
E_AUTO( gtod );

struct link__ {
	dwtght::grow__ GRow;
	dwtdct::irow__ IRow;
	void reset( bso::bool__ = true )
	{
		GRow = qNIL;
		IRow = qNIL;
	}
	E_CDTOR( link__ );
	void Init( void )
	{
		GRow = qNIL;
		IRow = qNIL;
	}
};

typedef bch::E_BUNCHt_( link__, drow__ ) links_;
E_AUTO( links );

static void Complete_(
	const dwtght::ghosts_ &Ghosts,
	gtod_ &GToD,
	links_ &Links,
	bundle_ &Bundle )
{
	ctn::E_CMITEMt( dwtght::ghost_, dwtght::grow__ ) Ghost;
	dir__ Dir;
	link__ Link;
	drow__ DRow = qNIL;
	dwtght::grow__ GRow = Ghosts.First();

	if ( GRow != qNIL  )
		GRow = Ghosts.Next( GRow );	// On passe la racine.

	Ghost.Init( Ghosts );

	while ( GRow != qNIL ) {
		if ( GToD( GRow ) == qNIL )
		{
			Dir.Init();
			Link.Init();

			Dir.Old.Name = Bundle.Names.Append( Ghost( GRow ).Name );
			Link.GRow = GRow;

			DRow = Bundle.Dirs.New();

			if ( DRow != Links.New() )
				qRGnr();

			GToD.Store( DRow, GRow );

			Bundle.Dirs.Store( Dir, DRow );
			Links.Store( Link, DRow );
		}

		GRow = Ghosts.Next( GRow );
	}
}

static void Fill_(
	const dwtdct::content_ &Content,
	const dwtght::ghosts_ &Ghosts,
	itod_ &IToD,
	gtod_ &GToD,
	links_ &Links,
	bundle_ &Bundle )
{
qRH
	dwtdct::irow__ IRow = qNIL;
	dwtght::grow__ GRow = qNIL;
	ctn::E_CMITEMt( dwtght::ghost_, dwtght::grow__ ) Ghost;
	gtoi GToI;
	link__ Link;
	dir__ Dir;
	drow__ DRow = qNIL;
qRB
	IRow = Content.First();

	GToI.Init();
	GToI.Allocate( Ghosts.Extent(), aem::mFitted );
	GToI.FillWith( qNIL );

	Ghost.Init( Ghosts );

	if ( IRow != qNIL ) {
		IRow = Content.Next( IRow );	// On saute le répertoire racine, qui correspond à 'Root'.
	}

	while ( IRow != qNIL ) {
		dwtdct::item_ &Item = *Content( IRow );

		Dir.Init();
		Link.Init();

		Dir.New.Name = Bundle.Names.Append( Item.Dir.Name );
		Link.IRow = IRow;

		DRow = Bundle.Dirs.New();

		if ( DRow != Links.New() )
			qRGnr();

		GRow = Item.Dir.GetGhostRow();

		if ( ( GRow != qNIL)
			 && ( GToI.Exists( GRow ) )
			 && ( GToI( GRow ) == qNIL ) )
		{
			GToI.Store( IRow, GRow );

			Dir.Old.Name = Bundle.Names.Append( Ghost( GRow ).Name );

			Link.GRow = GRow;

			GToD.Store( DRow, GRow );
		} 


		IToD.Store( DRow, IRow );

		Bundle.Dirs.Store( Dir, DRow );
		Links.Store( Link, DRow );

		IRow = Content.Next( IRow );
	}
qRR
qRT
qRE
}

static void Complete_(
	const dwtdct::content_ &Content,
	const itod_ &IToD,
	const dwtght::ghosts_ &Ghosts,
	const gtod_ &GToD,
	const links_ &Links,
	bundle_ &Bundle )
{
	drow__ Row = Links.First();
	ctn::E_CMITEMt( dwtght::ghost_, dwtght::grow__ ) Ghost;
	dir__ Dir;
	dwtdct::irow__ ParentIRow = qNIL;
	dwtght::grow__ ParentGRow = qNIL;	


	Ghost.Init( Ghosts );

	if ( Links.Amount() != Bundle.Dirs.Amount() )
		qRGnr();

	while ( Row != qNIL ) {
		Dir.Init();
		Bundle.Dirs.Recall( Row, Dir );

		if ( Links( Row ).GRow != qNIL ) {
			ParentGRow = Ghost( Links( Row ).GRow ).S_.Parent;

			if ( ParentGRow != qNIL )
				Dir.Old.Parent = GToD( ParentGRow );
		}

		if ( Links( Row ).IRow != qNIL ) {
			ParentIRow = Content( Links( Row ).IRow )->Parent();

			if ( ParentIRow != qNIL )
				Dir.New.Parent = IToD( ParentIRow );
		}

		Bundle.Dirs.Store( Dir, Row );

		Row = Links.Next( Row );
	}
}

typedef bitbch::E_BIT_BUNCHt_( drow__ ) handled_;
E_AUTO( handled );

static const str::string_ &GetPath_(
	drow__ Row,
	const handled_ &Handled,
	const dirs_ &Dirs,
	const names_ &Names,
	str::string_ &Path )
{
	ctn::E_CMITEMt( name_, nrow__ ) Name;

	Name.Init( Names );

	if ( Row != qNIL) {
		if ( Handled( Row ) )
		{
			Path.Append( Name( Dirs( Row ).New.Name ) );
			Row = Dirs( Row ).New.Parent;
		} else {
			Path.Append( Name( Dirs( Row ).Old.Name ) );
			Row = Dirs( Row ).Old.Parent;
		}
	}

	while ( Row != qNIL ) {
		Path.InsertAt( '/' );
		if ( Handled( Row ) )
		{
			Path.InsertAt( Name( Dirs( Row ).New.Name ) );
			Row = Dirs( Row ).New.Parent;
		} else {
			Path.InsertAt( Name( Dirs( Row ).Old.Name ) );
			Row = Dirs( Row ).Old.Parent;
		}
	}

	return Path;
}

static const str::string_ &GetPath_(
	drow__ Row,
	const handled_ &Handled,
	const bundle_ &Bundle,
	str::string_ &Path )
{
	return GetPath_( Row, Handled, Bundle.Dirs, Bundle.Names, Path );
}


static void Fill_(
	const str::string_ &Root,
	const dwtdct::content_ &Content,
	const dwtbsc::ghosts_oddities_ &GO,
	bundle_ &Bundle )
{
qRH
	dwtght::rRack GhostsRack;
	itod IToD;
	gtod GToD;
	links Links;
qRB
	GhostsRack.Init();
	const dwtght::ghosts_ &Ghosts = GetROGhosts( Root, GO, GhostsRack );

	IToD.Init();
	IToD.Allocate( Content.Extent(), aem::mFitted );
	IToD.FillWith( qNIL );

	GToD.Init();
	GToD.Allocate( Ghosts.Extent(), aem::mFitted );
	GToD.FillWith( qNIL );

	Links.Init();

	Fill_( Content, Ghosts, IToD, GToD, Links, Bundle );

	Complete_( Ghosts, GToD, Links, Bundle );

	Complete_( Content, IToD, Ghosts, GToD, Links, Bundle );
qRR
qRT
qRE
}

static void Fill_(
	const bundle_ &Bundle,
	moves_ &Moves,
	names_ &Names )
{
qRH
	drow__ Row = qNIL;
	move__ Move;
	ctn::E_CMITEMt( name_, nrow__ ) OldName, NewName;
	handled Handled;
	dir__ Dir;
	bso::bool__ Old = false, New = false;
	str::string Path;
qRB
	Handled.Init();
	Handled.Allocate( Bundle.Extent() );
	Handled.Reset();

	OldName.Init( Bundle.Names );
	NewName.Init( Bundle.Names );

	Row = Bundle.First();

	while ( Row != qNIL ) {
		Dir.Init();
		Move.Init();

		Old = New = false;

		Bundle.Dirs.Recall( Row, Dir );

		if ( Dir.New.Name == qNIL )
			Old = true;
		else if ( Dir.Old.Name == qNIL  )
			New = true;
		else if ( OldName( Dir.Old.Name ) != NewName( Dir.New.Name ) )
			New = Old = true;

		if ( Dir.Old.Parent != Dir.New.Parent ) {
			if ( Dir.New.Name != qNIL )
				New = true;
			if ( Dir.Old.Name != qNIL )
				Old = true;
		}

		if ( Old ) {
			if ( ( Dir.New.Name == qNIL ) && ( Dir.Old.Parent != qNIL ) && ( Bundle.Dirs( Dir.Old.Parent ).New.Name == qNIL ) )
				Old = false;
			else {
				Path.Init();
				GetPath_( Row, Handled, Bundle, Path );
				Move.Old = Names.Append( Path );
			}
		}

		if ( Old || New )
			Handled.Store( true, Row );	// 'GetPath_()' utilisera alors le nouveau nom.

		if ( New ) {
			Path.Init();
			GetPath_( Row, Handled, Bundle, Path );
			Move.New = Names.Append( Path );
		}

		if ( New || Old )
			Moves.Append( Move );

		Row = Bundle.Next( Row );
	}
qRR
qRT
qRE
}

void dwtmov::Explore(
	const str::string_ &Root,
	const dwtdct::content_ &Content,
	const dwtbsc::ghosts_oddities_ &GO,
	movings_ &Movings )
{
qRH
	bundle Bundle;
qRB
	Bundle.Init();
	Fill_( Root, Content, GO, Bundle );

	Fill_( Bundle, Movings.Moves, Movings.Names );
qRR
qRT
qRE
}

inline bso::sign__ Compare_(
	bso::size__ S1,
	bso::size__ S2,
	bso::bool__ Creation,
	dwtbsc::sort_type__ SortType )
{
	bso::sign__ Result = bso::Compare( S1, S2 );

	switch ( SortType ) {
	case dwtbsc::stNone:
		qRGnr();
		break;
	case dwtbsc::stRegular:
		if ( Creation  )
			Result = -Result;
		break;
	case dwtbsc::stReverse:
		if ( !Creation  )
			Result = -Result;
		break;
	default:
		qRGnr();
		break;
	}

	return Result;	// Pour éviter un 'warning'.
}

inline bso::sign__ Compare_(
	const move__ &M1,
	const move__ &M2,
	const names_ &Names,
	dwtbsc::sort_type__ SortType )
{
	ctn::E_CMITEMt( str::string_, nrow__ ) Name;

	if ( M1.Old == qNIL )
		if ( M2.Old == qNIL ) {
			Name.Init( Names );

			return Compare_( Name( M1.New ).Amount(), Name( M2.New ).Amount(), true, SortType );
		} else
			return 1;
	else if ( M2.Old == qNIL )
		return -1;
	else {
		Name.Init( Names );

		return Compare_( Name( M1.Old ).Amount(), Name( M2.Old ).Amount(), false, SortType );
	}

}

typedef idxbtq::E_INDEXt_( mrow__ ) _index_;
E_AUTO( _index );

#define LEVEL_DELTA	5

static inline mrow__ BalanceIfNeeded_(
	btr::level__ Level,
	mrow__ Root,
	_index_ &Index )
{
	if ( ( Level > LEVEL_DELTA ) && ( ( 2UL << ( Level - LEVEL_DELTA ) ) > Index.Amount() ) )
		Root = Index.Balance( Root );

	return Root;
}

static mrow__ Insert_(
	mrow__ Row,
	const moves_ &Moves,
	const names_ &Names,
	mrow__ Root,
	dwtbsc::sort_type__ SortType,
	_index_ &Index )
{
	bso::bool__ Inserted = false;
	mrow__ Candidate = Root;
	btr::level__ Level = 0;
	ctn::E_CMITEM( str::string_ ) Name;

	while ( !Inserted ) {
		switch ( Compare_( Moves( Row ), Moves( Candidate ), Names, SortType ) ) {
		case -1:
		case 0:
			if ( Index.TreeHasGreater( Candidate ) )
				Candidate = Index.GetGreater( Candidate );
			else {
				Root = Index.BecomeGreater( Row, Candidate, Root );
				Inserted = true;
			}
			break;
		case 1:
			if ( Index.TreeHasLesser( Candidate ) )
				Candidate = Index.GetLesser( Candidate );
			else {
				Root = Index.BecomeLesser( Row, Candidate, Root );
				Inserted = true;
			}
			break;
		default:
			qRGnr();
			break;
		}

		Level++;
	}

	return BalanceIfNeeded_( Level, Root, Index );
}


static mrow__ Sort_(
	const moves_ &Unsorted,
	const names_ &Names,
	dwtbsc::sort_type__ SortType,
	_index_ &Index )
{
	mrow__ Root = qNIL;
	mrow__ Row = Unsorted.First();

	if ( Row != qNIL ) {
		Root = Row;
		Index.BecomeRoot( Row );

		Row = Unsorted.Next( Row );
	}

	while ( Row != qNIL ) {
		Root = Insert_( Row, Unsorted, Names, Root, SortType, Index );

		Row = Unsorted.Next( Row );
	}

	return Root;
}

static void Fill_(
	const moves_ &Unsorted,
	const que::E_QUEUEt_( mrow__ ) &Queue,
	mrow__ Row,
	moves_ &Sorted )
{
	if ( Queue.Previous( Row ) != qNIL )
		qRGnr();

	while ( Row != qNIL) {
		Sorted.Append( Unsorted( Row ) );

		Row = Queue.Next( Row );
	}
}

static void Fill_(
	const moves_ &Unsorted,
	const _index_ &Index,
	mrow__ Root,
	moves_ &Sorted )
{
	if ( Root != qNIL )
		Fill_( Unsorted, Index.Queue(), Index.First( Root ), Sorted );
}

static void Sort_(
	const moves_ &Unsorted,
	const names_ &Names,
	dwtbsc::sort_type__ SortType,
	moves_ &Sorted )
{
qRH
	_index Index;
	mrow__ Row = qNIL;
	mrow__ Root = qNIL;
qRB
	Index.Init();
	Index.Allocate( Unsorted.Extent() );

	Root = Sort_( Unsorted, Names, SortType, Index );

	Fill_( Unsorted, Index, Root, Sorted );
qRR
qRT
qRE
}

enum tag__
{
	tMoves,
	tMove,
	t_amount,
	t_Undefined
};

#define T( name )	case t##name : return #name; break

static inline const char *GetLabel_(
	tag__ Tag,
	version__ Version = v_Current )
{
	switch ( Version ) {
	case v0_1:
	case v0_2:
		switch ( Tag ) {
			T( Moves );
			T( Move );
		default:
			qRGnr();
			break;
		}
	default:
		qRGnr();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

enum attribute__
{
	aAmount,
	aOld,
	aNew,
	aDepth,	// Obsolete ; conservé pour des raisons de compatibilité.
	a_amount,
	a_Undefined
};

#define A( name )	case a##name : return #name; break

static inline const char *GetLabel_(
	attribute__ Attribute,
	version__ Version = v_Current )
{
	switch( Version ) {
	case v0_1:
		switch ( Attribute ) {
		A( Amount );
		case aOld:
			return "Current";
			break;
		A( New );
		A( Depth );
		default:
			qRGnr();
			break;
		}
		break;
	case v0_2:
		switch ( Attribute ) {
		A( Amount );
		A( Old );
		A( New );
		case aDepth:	// Obsolete.
			qRGnr();
			break;
		default:
			qRGnr();
			break;
		}
		break;
	default:
		qRGnr();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

static void Dump_(
	const moves_ &Moves,
	const names_ &Names,
	xml::writer_ &Writer )
{
	mrow__ Row = Moves.First();
	ctn::E_CMITEMt( dwtbsc::name_, nrow__ ) Name;

	Name.Init( Names );

	Writer.PushTag( GetLabel_( tMoves ) );
	xml::PutAttribute( GetLabel_( aAmount ) , Moves.Amount(), Writer );

	while ( Row != qNIL ) {
		Writer.PushTag( GetLabel_( tMove ) );

		if ( Moves( Row ).Old != qNIL )
			Writer.PutAttribute( GetLabel_( aOld ), Name( Moves( Row ).Old ) );

		if ( Moves( Row ).New != qNIL )
			Writer.PutAttribute( GetLabel_( aNew ), Name( Moves( Row ).New ) );

		Writer.PopTag();

		Row = Moves.Next( Row );
	}

	Writer.PopTag();
}

void dwtmov::Dump(
	const movings_ &Movings,
	xml::writer_ &Writer )
{
	Dump_( Movings.Moves, Movings.Names, Writer );
}

static void Display_(
	const movings_ &Movings,
	mrow__ Row,
	txf::text_oflow__ &Flow )
{
qRH
	str::string Old, New;
	depth__ Depth = 0;
qRB
	Old.Init();
	New.Init();

	Movings.GetMove( Row, Old, New );

	if ( Old.Amount() )
		Flow << '\'' << Old << '\'';
		
	Flow << "=>";
	
	if ( New.Amount() )
		Flow << '\'' << New << '\'';
		
	Flow << txf::nl;
qRR
qRT
qRE
}



void dwtmov::Display(
	const movings_ &Movings,
	txf::text_oflow__ &Flow )
{
	mrow__ Row = Movings.First();

	while ( Row != qNIL ) {
		Display_( Movings, Row, Flow );

		Row = Movings.Next( Row );
	}
}

typedef bso::uint__ depth__;

#define DEPTH_MAX	BSO_UINT_MAX

// '<Move ..../>
//        ^     ^
static void ParseMoveSection_(
	xml::parser___ &Parser,
	version__ Version,
	movings_ &Movings )
{
qRH
	bso::bool__ Continue = true;
	move__ Move;
qRB
	Move.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfAttribute | xml::tfEndTag ) ) {
		case xml::tAttribute:
			if ( Parser.AttributeName() == GetLabel_( aOld, Version ) )
				Move.Old = Movings.Names.Append( Parser.Value() );
			else if ( Parser.AttributeName() == GetLabel_( aNew, Version ) )
				Move.New = Movings.Names.Append( Parser.Value() );
			else if ( Parser.AttributeName() != GetLabel_( aDepth, Version ) )	// NOTA: L'attribute 'Depth' est obsolete.
				qRGnr();
			break;
		case xml::tEndTag:
			if ( Parser.TagName() != GetLabel_( tMove, Version ) )
				qRGnr();
			else
				Continue = false;
			break;
		default:
			qRGnr();
			break;
		}
	}

	Movings.Moves.Append( Move );
qRR
qRT
qRE
}

void dwtmov::Load(
	xml::parser___ &Parser,
	version__ Version,
	movings_ &Movings )
{
	bso::bool__ Continue = true;
	bso::uint__ Amount = 0;
	sdr::row__ Error = qNIL;

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTag | xml::tfAttribute | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == GetLabel_( tMove, Version ) )
				ParseMoveSection_( Parser, Version, Movings );
			else if ( Parser.TagName() != GetLabel_( tMoves, Version ) )
				qRGnr();
			break;
		case xml::tAttribute:
			Error = qNIL;
			if ( Parser.AttributeName() != GetLabel_( aAmount, Version ) )
				qRGnr();

			Parser.Value().ToNumber( Amount, &Error );

			if ( Error != qNIL )
				qRGnr();
			break;
		case xml::tEndTag:
			if ( Parser.TagName() == GetLabel_( tMoves, Version ) )
				Continue = false;
			else
				qRGnr();
			break;
		case xml::t_Processed:
			qRGnr();
			break;
		case xml::t_Error:
			qRGnr();
			break;
		default:
			qRFwk();
			break;
		}
	}
}

static inline const str::string_ &GetAbsolute_(
	const str::string_ &Path,
	const str::string_ &Name,
	str::string_ &Absolute )
{
	Absolute.Init( Path );

	if ( Path.Amount() != 0  )
		Absolute.Append( '/' );

	Absolute.Append( Name );

	return Absolute;
}

static bso::bool__ Move_(
	const str::string_ &Path,
	const str::string_ &Old,
	const str::string_ &New )
{
	bso::bool__ Success = false;
qRH	
	str::string AbsoluteOld, AbsoluteNew;
qRB
	AbsoluteOld.Init();
	AbsoluteNew.Init();

	Success = fil::Move( GetAbsolute_( Path, Old, AbsoluteOld ), GetAbsolute_( Path, New, AbsoluteNew ) );
qRR
qRT
qRE
	return Success;
}

static bso::bool__ Create_(
	const str::string_ &Path,
	const str::string_ &Name )
{
	bso::bool__ Success = false;
qRH	
	str::string Absolute;
qRB
	Absolute.Init( Path );

	Success = dir::CreateDir( GetAbsolute_( Path, Name, Absolute ) ) == dir::sOK;
qRR
qRT
qRE
	return Success;
}

static bso::bool__ Apply_(
	const str::string_ &Path,
	const move__ &Move,
	const names_ &Names,
	dwtbsc::tamount__ ThreadAmountMax )
{
	ctn::E_CMITEMt( name_, nrow__ ) Old, New;

	Old.Init( Names );
	New.Init( Names );

	if ( ( Move.Old != qNIL ) && ( Move.New != qNIL ) )
		return Move_( Path, Old( Move.Old ), New( Move.New ) );
	else if ( Move.New != qNIL )
		return Create_( Path, New( Move.New ) );
	else if ( Move.Old != qNIL  )
		return dwtbsc::Delete( Path, New( Move.Old ), ThreadAmountMax );
	else
		qRGnr();

	return false;	// Pour éviter un 'warning'.
}

bso::bool__ dwtmov::Apply(
	const str::string_ &Path,
	const movings_ &Movings,
	dwtbsc::tamount__ ThreadAmountMax,
	mrows_ &Failures )
{
	bso::bool__ Failure = false;
	mrow__ Row = Movings.First();
	ctn::E_CMITEMt( name_, nrow__ ) Current, New;

	Current.Init( Movings.Names );
	New.Init( Movings.Names );

	while ( Row != qNIL ) {

		if ( !Apply_( Path, Movings.Moves( Row ), Movings.Names, ThreadAmountMax ) ) {
			Failure = true;
			Failures.Append( Row );
		}

		Row = Movings.Next( Row );
	}

	return !Failure;
}

void dwtmov::Display(
	const movings_ &Movings,
	const mrows_ &Rows,
	txf::text_oflow__ &Flow )
{
	sdr::row__ Row = Rows.First();

	while ( Row != qNIL ) {
		Display_( Movings, Rows( Row ), Flow );

		Row = Rows.Next( Row );
	}
}

Q37_GCTOR( fwtmov )
{
	FillVersionAutomat_();
}
