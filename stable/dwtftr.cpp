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

#define DWTFTR__COMPILATION

#include "dwtftr.h"

using namespace dwtftr;

#include "dwtdct.h"

#include "dir.h"
#include "mtx.h"
#include "mtk.h"
#include "idxbtq.h"

using namespace dwtftr;

using namespace dwtbsc;
using namespace dwtdct;

static stsfsm::automat VersionAutomat_;

static stsfsm::automat ExclusionAutomatV1_;

const char *dwtftr::GetLabel( version__ Version )
{
	switch ( Version ) {
	case v0_1:
		return "0.1";
		break;
	case v0_2:
		return "0.2";
		break;
	case v0_3:
		return "0.3";
		break;
	default:
		qRGnr();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

version__ dwtftr::GetVersion( const str::string_ &Pattern )
{
	return GetId( Pattern, VersionAutomat_, v_Undefined, v_amount );
}

enum xversion__ {
	xv1,
	xv_amount,
	xv_Current = xv_amount - 1,
	xv_Undefined,
};

static inline xversion__ GetXVersion_( version__ Version )
{
	switch ( Version ) {
	case v0_1:
	case v0_2:
	case v0_3:
		return xv1;
		break;
	default:
		qRGnr();
		break;
	}

	return xv_Undefined;	// Pour éviter un 'xarning'.
}

#define E( name )	case x##name : return #name; break

const char *GetLabel_(
	exclusion__ Exclusion,
	xversion__ Version = xv_Current )
{
	switch ( Exclusion ) {
	E( No );
	E( MatchList );
	E( Ghost );
	E( FileTooBig );
	E( NameTooLong );
	default:
		qRGnr();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

static const stsfsm::automat_ &GetExclusionAutomat_( xversion__ Version )
{
	switch ( Version ) {
	case xv1:
		return ExclusionAutomatV1_;
		break;
	default:
		qRGnr();
		break;
	}

	return *(stsfsm::automat_ *)NULL;	// Pour éviter un 'warning'.
}

exclusion__ GetExclusion_(
	version__ Version,
	const str::string_ &Pattern )
{
	return GetId( Pattern, GetExclusionAutomat_( GetXVersion_( Version ) ), x_Undefined, x_amount );
}

static void FillVersionAutomat_( void )
{
	VersionAutomat_.Init();
	stsfsm::Fill<version__>( VersionAutomat_, v_amount, GetLabel );
}

static void FillExclusionAutomats_( void )
{
	ExclusionAutomatV1_.Init();
	stsfsm::Fill<exclusion__,xversion__>( xv1, ExclusionAutomatV1_, x_amount, GetLabel_ );
}

const str::string_ &dwtftr::file_tree_::GetPath(
	drow__ Row,
	str::string_ &Path ) const
{
	ctn::E_CITEMt( directory_, drow__ ) Directory;
	ctn::E_CMITEMt( name_, nrow__ ) Name;

	Directory.Init( Directories );
	Name.Init( Names );

	if ( Row != qNIL ) {
		Path.InsertAt( Name( Directory( Row )().Name ) );
		Row = Parent( Row );
	}

	while ( Row != qNIL ) {
		Path.InsertAt( '/' );
		Path.InsertAt( Name( Directory( Row )().Name ) );
		Row = Parent( Row );
	}

	return Path;
}

static void Fill_(
	const goofs_data_ &Data,
	vKernel &Kernel,
	dwtbsc::grows_ &Rows )
{
qRH
	dwtbsc::grow__ Row = qNIL;
	ctn::E_CMITEMt( str::string_, dwtbsc::grow__ ) Name;
	ctn::E_CMITEMt( str::string_, dwtbsc::grow__ ) Oddity;
	goof__ Goof;
qRB
	Name.Init( Data.Names );
	Oddity.Init( Data.Oddities );

	if ( Data.Names.Amount() != 0 ) {
		Row = Data.Names.First();

		while ( Row != qNIL ) {
			Goof.Init();

			Goof.Name = Kernel.Names.Append( Name( Row ) );

			if ( Oddity( Row ).Amount() != 0  )
				Goof.Oddity = Kernel.Oddities.Append( Oddity( Row ) );

			Rows.Append( Kernel.Goofs.Append( Goof ) );

			Row = Data.Names.Next( Row );
		}
	}
qRR
qRT
qRE
}

static void Fill_(
	const files_data_ &Data,
	vKernel &Kernel,
	frows_ &Rows )
{
qRH
	frow__ Row = qNIL;
	ctn::E_CMITEMt( str::string_, frow__ ) Name;
	file__ File;
qRB
	Name.Init( Data.Names );

	if ( Data.Names.Amount() != 0 ) {
		Row = Data.Names.First();

		while ( Row != qNIL ) {
			File.Init();

			File.Name = Kernel.Names.Append( Name( Row ) );

			File.Size = Data.Sizes( Row );

			File.Exclusion = Data.Exclusions( Row );
			File.Timestamp = Data.Timestamps( Row );

			Rows.Append( Kernel.Files.Append( File ) );

			Row = Data.Names.Next( Row );
		}
	}
qRR
qRT
qRE
}

typedef bch::E_BUNCHt_( drows_ *, irow__ ) drows_set_;
E_AUTO( drows_set );

static void Organize_(
	const items_ &Items,
	vKernel &Kernel,
	i2d_ &I2D,
	drows_set_ &Dirs,
	processing_observer__ &Observer )
{
qRH
	irow__ IRow = qNIL, ParentIRow = qNIL;
	dwtght::grow__ GRow = qNIL;
	directory Directory;
qRB
	I2D.Allocate( Items.Amount() );
	I2D.FillWith( qNIL );

	Dirs.Allocate( Items.Amount() );
	Dirs.FillWith( NULL );

	IRow = Items.First();

	Observer.Report( 0, Items.Amount() );

	while ( IRow != qNIL ) {
		const item_ &Item = *Items( IRow );

		Directory.Init();

		Fill_( Item.Files, Kernel, Directory.Files );
		Fill_( Item.Goofs, Kernel, Directory.Goofs );

		ParentIRow = Item.Parent();	// Peut-être modifié ci-dessous.

		Directory().Name = Kernel.Names.Append( Item.Dir.Name );

		ParentIRow = Item.Parent();

		Directory().Exclusion = Item.Dir.Exclusion();
		Directory().Timestamp = Item.Dir.Timestamp();

		I2D.Store( Kernel.Directories.Append( Directory ), IRow );

		if ( ParentIRow != qNIL ) {
			drows_ *DRows = Dirs( ParentIRow );

			if ( DRows == NULL ) {
				if ( ( DRows = new drows ) == NULL )
					qRAlc();

				DRows->Init();

				Dirs.Store( DRows, ParentIRow );
			}

			DRows->Append( I2D( IRow ) );
		}

		if ( Observer.IsElapsed() )
			Observer.Report( *IRow + 1, Items.Amount() );

		IRow = Items.Next( IRow );
	}

	Observer.Report( Items.Amount(), Items.Amount() );
qRR
qRT
qRE
}

static void Attach_(
	drow__ ParentRow,
	const drows_ &Rows,
	dtree_ &Tree )
{
	sdr::row__ Row = Rows.First();

	while ( Row != qNIL ) {
		Tree.BecomeLastChild( Rows( Row ), ParentRow );

		Row = Rows.Next( Row );
	}
}

static drow__ Attach_(
	drows_set_ &Dirs,	// No 'const', because we delete also the content.
	const i2d_ &I2D,
	file_tree_ &Tree )
{
	drow__ Root = qNIL;
qRH
	irow__ Row = qNIL;
	drows Rows;
qRB
	Row = Dirs.First();

	if ( Row != qNIL )
		Root = I2D( Row );

	while ( Row != qNIL ) {
		Rows.Init();
		if ( Dirs( Row ) != NULL ) {
			Rows = *Dirs( Row );

			delete Dirs( Row );
			Dirs.Store( NULL, Row );
		}

		Tree.Directories( I2D( Row ) ).Dirs = Rows;

		Attach_( I2D( Row ), Rows, Tree );

		Tree.Directories.Flush();

		Row = Dirs.Next( Row );
	}

qRR
qRT
qRE
	return Root;
}

static void Delete_( drows_set_ &Dirs )
{
	irow__ Row = Dirs.First();

	while ( Row != qNIL ) {
		if ( Dirs( Row ) != NULL ) {
			delete Dirs( Row );

			Dirs.Store( NULL, Row );
		}

		Row = Dirs.Next( Row );
	}
}

drow__ dwtftr::Process(
	const dwtdct::content_ &Content,
	file_tree_ &Tree,
	processing_observer__ &Observer )
{
	drow__ Root = qNIL;
qRH
	drows_set Dirs;
	i2d I2D;
qRB
	Tree.Allocate( Content.Amount() );

	I2D.Init();
	Dirs.Init();
	Organize_( Content, Tree, I2D, Dirs, Observer );

	Root =  Attach_( Dirs, I2D, Tree );
qRR
	Delete_( Dirs );
qRT
qRE
	return Root;
}

void dwtftr::Fill(
	const i2d_ &I2D,
	const dwtdct::content_ &Content,
	d2g_ &D2G )
{
	dwtdct::item_ *Item = NULL;
	drow__ DRow = qNIL;
	irow__ Row = I2D.First();

	D2G.Init();
	D2G.Allocate( Content.Amount() );
	D2G.FillWith( qNIL );

	while ( Row != qNIL ) {
		Item = Content( Row );

		if ( Item == NULL )
			qRFwk();

		DRow = I2D( Row );

		if ( DRow != qNIL )
			D2G.Store( Item->Dir.GetGhostRow(), DRow );

		Row = I2D.Next( Row );
	}
}

enum tag__
{
	tDirs,
	tDir,
	tFiles,
	tFile,
	tGoofs,
	tGoof,
	t_amount,
	t_Undefined
};

#define T( name )	case t##name : return #name; break

static inline const char *GetLabel_(
	tag__ Tag,
	version__ Version = v_Current )
{
	switch ( Version ) {

	case v0_2:
	case v0_3:
		switch ( Tag ) {
			T( Goofs );
			T( Goof );
			T( Dirs );
			T( Dir );
			T( Files );
			T( File );
		default:
			qRGnr();
			break;
		}
	case v0_1:
		switch ( Tag ) {
			T( Dirs );
			T( Dir );
			T( Files );
			T( File );
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
	aName,
	aExclusion,
	aOddity,
	aTimestamp,
	aAmount,
	aSize,
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
	case v0_2:
		switch ( Attribute ) {
		A( Name );
		A( Exclusion );
		A( Oddity );
		A( Timestamp );
		A( Amount );
		A( Size );
//		A( NewName );
		default:
			qRGnr();
			break;
		}
		break;
	case v0_3:
		switch ( Attribute ) {
		A( Name );
		A( Exclusion );
		A( Oddity );
		A( Timestamp );
		A( Amount );
		A( Size );
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

static inline void DumpCommon_(
	const common__ &Common,
	const names_ &Names,
	xml::writer_ &Writer )
{
	ctn::E_CMITEMt( name_, nrow__ ) Name;

	Name.Init( Names );

	Writer.PutAttribute( GetLabel_( aName ), Name( Common.Name ) );
}

static void Dump_( 
	const grows_ &Rows,
	const goofs_ &Goofs,
	const names_ &Names,
	const oddities_ &Oddities,
	xml::writer_ &Writer )
{
	if ( Rows.Amount() == 0 )
		return;

	sdr::row__ Row = Rows.First();

	ctn::E_CMITEMt( oddity_, orow__ ) Oddity;

	Oddity.Init( Oddities );

	Writer.PushTag( GetLabel_( tGoofs ) );
	xml::PutAttribute( GetLabel_( aAmount ), Rows.Amount(), Writer );

	while ( Row != qNIL ) {
		Writer.PushTag( GetLabel_( tGoof ) );

		DumpCommon_( Goofs( Rows( Row ) ), Names, Writer );

		if ( Goofs( Rows( Row ) ).Oddity != qNIL )
			Writer.PutAttribute( GetLabel_( aOddity ), Oddity( Goofs( Rows( Row ) ).Oddity ) );

		Writer.PopTag();

		Row = Rows.Next( Row );
	}

	Writer.PopTag();
}

static inline void DumpRegular_(
	const regular__ &Regular,
	const names_ &Names,
	xml::writer_ &Writer )
{
	DumpCommon_( Regular, Names, Writer );

	switch ( Regular.Exclusion ) {
	case xNo:
		break;
	case xMatchList:
		Writer.PutAttribute( GetLabel_( aExclusion ), GetLabel_( xMatchList ) );
		break;
	case xFileTooBig:
		Writer.PutAttribute( GetLabel_( aExclusion ), GetLabel_( xFileTooBig ) );
		break;
	case xNameTooLong:
		Writer.PutAttribute( GetLabel_( aExclusion ), GetLabel_( xNameTooLong ) );
		break;
	case xGhost:
		Writer.PutAttribute( GetLabel_( aExclusion ), GetLabel_( xGhost ) );
		break;
	default:
		qRGnr();
		break;
	}

	xml::PutAttribute( GetLabel_( aTimestamp ), Regular.Timestamp, Writer );
}

static void Dump_( 
	const frows_ &Rows,
	const files_ &Files,
	const names_ &Names,
	xml::writer_ &Writer )
{
	if ( Rows.Amount() == 0 )
		return;

	sdr::row__ Row = Rows.First();

	Writer.PushTag( GetLabel_( tFiles ) );
	xml::PutAttribute( GetLabel_( aAmount ), Rows.Amount(), Writer );

	while ( Row != qNIL ) {
		Writer.PushTag( GetLabel_( tFile ) );

		DumpRegular_( Files( Rows( Row ) ), Names, Writer );

		xml::PutAttribute( GetLabel_( aSize ), Files( Rows( Row ) ).Size, Writer );

		Writer.PopTag();

		Row = Rows.Next( Row );
	}

	Writer.PopTag();
}

static inline void PushDirectory_(
	const directory_ &Directory,
	const names_ &Names,
	xml::writer_ &Writer )
{
	ctn::E_CMITEMt( name_, nrow__ ) Name;

	Name.Init( Names );

	Writer.PushTag( GetLabel_( tDir ) );

	DumpRegular_( Directory(), Names, Writer );
}

void dwtftr::file_tree_::Dump( 
	drow__ Root,
	xml::writer_ &Writer ) const
{
	dtr::browser__<drow__> Browser;
	ctn::E_CITEMt( directory_, drow__ ) Directory;
//	drow__ ChildNotHandled = qNIL;

	Directory.Init( Directories );

	PushDirectory_( Directory( Root ), Names, Writer );
	Dump_( Directory( Root ).Files, Files, Names, Writer );
	Dump_( Directory( Root ).Goofs, Goofs, Names, Oddities, Writer );

	Browser.Init( Root );

	while ( Browse( Browser ) != qNIL ) {
		switch ( Browser.GetKinship() ) {
		case dtr::kChild:
			Writer.PushTag( GetLabel_( tDirs ) );
			xml::PutAttribute( GetLabel_( aAmount ), Directory( Parent( Browser.Position() ) ).Dirs.Amount(), Writer );
			PushDirectory_( Directory( Browser.GetPosition() ), Names,  Writer );
			Dump_( Directory( Browser.GetPosition() ).Files, Files, Names, Writer );
			Dump_( Directory( Browser.GetPosition() ).Goofs, Goofs, Names, Oddities, Writer );
			break;
		case dtr::kSibling:
			Writer.PopTag();	// 'Dir'.
			PushDirectory_( Directory( Browser.GetPosition() ), Names, Writer );
			Dump_( Directory( Browser.GetPosition() ).Files, Files, Names, Writer );
			Dump_( Directory( Browser.GetPosition() ).Goofs, Goofs, Names, Oddities, Writer );
			break;
		case dtr::kParent:
			Writer.PopTag();	// 'Dir'.
			Writer.PopTag();	// 'Dirs'.
			break;
		default:
			qRGnr();
			break;
		}
	}

	Writer.PopTag();
}

void dwtftr::file_tree_hf___::Init(
	const fnm::name___ &Path,
	const fnm::name___ &Basename )
{
qRH
	fnm::name___ Tree, Kernel;
qRB
	Tree.Init();
	fnm::BuildPath(Path, Basename, "t", Tree );
	this->Tree.Init( "", Tree );

	Kernel.Init();
	fnm::BuildPath( Path, Basename, "k", Kernel );
	this->Kernel.Init( "", Kernel );
qRR
qRT
qRE
}

namespace {
	void SetFileTreeFilesHook_(
		const fnm::name___ &Path,
		const file_tree_ &FileTree,
		uys::mode__ Mode,
		file_tree_fh___ &FilesHook )
	{
	qRH
		file_tree_hf___ Filenames;
	qRB
		Filenames.Init( Path, "tree" );

		FilesHook.Init( Filenames, FileTree, Mode, uys::bPersistent, flsq::GetId() );
	qRR
	qRT
	qRE
	}

	void GetFileTree_(
		fnm::name___ &DataDirName,
		uys::mode__ Mode,
		file_tree_rack___ &Rack )
	{
		SetFileTreeFilesHook_( DataDirName, Rack.Tree, Mode, Rack.Hook );

		if ( !Plug( Rack.Tree, Rack.Hook ) )
			Rack.Tree.Init();
	}
}

file_tree_ &dwtftr::GetRWFileTree(
	const str::string_ &Root,
	const dwtbsc::ghosts_oddities_ &GO,
	file_tree_rack___ &Rack )
{
qRH
	fnm::name___ Name;
qRB
	Name.Init();
	GetGhostsDataDirName( Root, GO, Name );

	GetFileTree_( Name, uys::mReadWrite, Rack );
qRR
qRT
qRE
	return Rack.Tree;
}

const file_tree_ &dwtftr::GetROFileTree(
	const str::string_ &Root,
	const dwtbsc::ghosts_oddities_ &GO,
	file_tree_rack___ &Rack )
{
qRH
	fnm::name___ Name;
qRB
	Name.Init();
	GetGhostsDataDirName( Root, GO, Name );

	if ( fil::Exists( Name ) )
		GetFileTree_( Name, uys::mReadOnly, Rack );
	else
		Rack.Tree.Init();
qRR
qRT
qRE
	return Rack.Tree;
}

static void FillTagAutomat_(
	version__ Version,
	stsfsm::automat_ &Automat )
{
	stsfsm::Fill<tag__,version__>( Version, Automat, t_amount, GetLabel_ );
}

static void FillAttributeAutomat_(
	version__ Version,
	stsfsm::automat_ &Automat )
{
	stsfsm::Fill<attribute__,version__>( Version, Automat, a_amount, GetLabel_ );
}

#define UNDEFINED_ID	FWTBSC_UNDEFINED_ID

inline static stsfsm::id__ GetItem_(
	const str::string_ &Name,
	const stsfsm::automat_ &Automat )
{
	return GetId( Name, Automat );
}

static tag__ GetTag_(
	const str::string_ &Name,
	const stsfsm::automat_ &Automat )
{
	stsfsm::id__ Id = GetItem_( Name, Automat );

	if ( Id == stsfsm::UndefinedId )
		return t_Undefined;
	else if ( Id >= t_amount )
		qRGnr();
	else
		return (tag__)Id;

	return t_Undefined;	// Pour éviter un 'warning'.
}

static attribute__ GetAttribute_(
	const str::string_ &Name,
	const stsfsm::automat_ &Automat )
{
	stsfsm::id__ Id = GetItem_( Name, Automat );

	if ( Id == stsfsm::UndefinedId )
		return a_Undefined;
	else if ( Id >= a_amount )
		qRGnr();
	else
		return (attribute__)Id;

	return a_Undefined;	// Pour éviter un 'warning'.
}

#define LEVEL_DELTA	5

template <typename row> static inline row BalanceIfNeeded_(
	btr::level__ Level,
	row Root,
	idxbtq::E_INDEXt_( row ) &Index )
{
	return Root;
	if ( ( Level > LEVEL_DELTA ) && ( ( 2UL << ( Level - LEVEL_DELTA ) ) > Index.Amount() ) )
		Root = Index.Balance( Root );

	return Root;
}

static inline nrow__ GetNameRow_(
	const directories_ &Directories,
	drow__ Row )
{
	ctn::E_CITEMt( directory_, drow__ ) Directory;

	Directory.Init( Directories );

	return Directory( Row )().Name;
}

static inline nrow__ GetNameRow_(
	const files_ &Files,
	frow__ Row )
{
	return Files( Row ).Name;
}

typedef idxbtq::E_INDEX_ _index_;
E_AUTO( _index );

inline static bso::sign__ Compare_(
	const str::string_ &O1,
	const str::string_ &O2,
	sort_type__ SortType )
{
	switch ( SortType ) {
	case stNone:
		qRGnr();
		break;
	case stRegular:
		return str::Compare( O1, O2 );
		break;
	case stReverse:
		return str::Compare( O2, O1 );
		break;
	default:
		qRGnr();
		break;
	}

	return 0;	// Pour éviter un 'warning'.
}

template <typename rows, typename items> static sdr::row__ Insert_(
	sdr::row__ Row,
	const rows &Rows,
	const str::string_ &Pattern,
	const items &Items,
	const names_ &Names,
	sdr::row__ Root,
	sort_type__ SortType,
	_index_ &Index )
{
	ctn::E_CMITEMt( str::string_, nrow__ ) Name;
	sdr::row__ IRow = qNIL;
	btr::level__ Level = 0;
	bso::bool__ Inserted = false;

	Name.Init( Names );

	IRow = Index.First( Root, Level );
	
	if ( Compare_( Pattern, Name( GetNameRow_( Items, Rows( IRow ) ) ), SortType ) == -1 ) {
		Index.BecomeLesser( Row, IRow, Root );

		Root = BalanceIfNeeded_( Level, Root, Index );

		return Root;
	}

	IRow = Index.Last( Root, Level );

	if ( Compare_( Pattern, Name( GetNameRow_( Items, Rows( IRow ) ) ), SortType ) == 1 ) {
		Index.BecomeGreater( Row, IRow, Root );

		Root = BalanceIfNeeded_( Level, Root, Index );

		return Root;
	}

	IRow = Root;


	while ( !Inserted ) {
		switch ( Compare_( Pattern, Name( GetNameRow_( Items, Rows( IRow ) ) ) , SortType ) ) {
		case 1:
			if ( Index.TreeHasGreater( IRow ) )
				IRow = Index.GetGreater( IRow );
			else {
				Index.BecomeGreater( Row, IRow, Root );
				Inserted = true;
			}
			break;
		case 0:
			qRGnr();
			break;
		case -1:
			if ( Index.TreeHasLesser( IRow ) )
				IRow = Index.GetLesser( IRow );
			else {
				Index.BecomeLesser( Row, IRow, Root );
				Inserted = true;
			}
			break;
		default:
			break;
		}
		Level++;
	}

	Root = BalanceIfNeeded_( Level, Root, Index );

	return Root;
}

template <typename rows, typename items> static sdr::row__ Sort_(
	const rows &Rows,
	const names_ &Names,
	const items &Items,
	sort_type__ SortType,
	_index_ &Index )
{
	sdr::row__ Root = qNIL;
qRH
	sdr::row__ Row = qNIL;
	name Name;
qRB
	Row = Rows.First();

	if ( Row != qNIL ) {
		Root = Row;
		Index.BecomeRoot( Root ) ;
		Row = Rows.Next( Row );
	}


	while ( Row != qNIL ) {
		Name.Init();
		Names.Recall( GetNameRow_( Items, Rows( Row ) ), Name );

		Root = Insert_( Row, Rows, Name, Items, Names, Root, SortType, Index );

		Row = Rows.Next( Row );
	}
qRR
qRT
qRE
	return Root;
}

template <typename rows> static void Fill_(
	const rows &UnsortedRows,
	const que::E_QUEUE_ &Queue,
	sdr::row__ Row,
	rows &SortedRows )
{
	if ( Queue.Previous( Row ) != qNIL )
		qRGnr();


	while ( Row != qNIL) {
		SortedRows.Append( UnsortedRows( Row ) );

		Row = Queue.Next( Row );
	}
}

template <typename rows> static void Fill_(
	const rows &UnsortedRows,
	const _index_ &Index,
	sdr::row__  Root,
	rows &Rows )
{
	if ( Root != qNIL )
		Fill_( UnsortedRows, Index.Queue(), Index.First( Root ), Rows );
}

template <typename r> static void Fill_(
	const bch::E_BUNCH_( r ) &UnsortedRows,
	const idxbtq::E_INDEX_ &Index,
	sdr::row__ Root,
	bch::E_BUNCH_( r ) &Rows )
{
	sdr::row__ Row = Index.First( Root );

	while ( Row != qNIL ) {
		Rows.Append( UnsortedRows( Row ) );

		Row = Index.Next( Row );
	}
}

template <typename row, typename items> static void Sort_(
	bch::E_BUNCH_( row ) &Rows,
	const names_ &Names,
	const items &Items,
	sort_type__ SortType )
{
qRH
	_index Index;
	sdr::row__ Root = qNIL;
	bch::E_BUNCH( row ) SortedRows;
qRB
	Index.Init();
	Index.Allocate( Rows.Amount(), aem::mFitted );

	Root = Sort_<bch::E_BUNCH_( row ), items>( Rows, Names, Items, SortType, Index );

	if ( Root != qNIL ) {
		SortedRows.Init();
		Fill_( Rows, Index, Root, SortedRows );
		Rows = SortedRows;
	}
qRR
qRT
qRE
}

static void Sort_(
	directory_ &Directory,
	const vKernel &Kernel,
	sort_type__ SortType )
{
	Sort_<drow__,directories_>( Directory.Dirs, Kernel.Names, Kernel.Directories, SortType );
	Sort_<frow__,files_>( Directory.Files, Kernel.Names, Kernel.Files, SortType );
}

enum kind__ {
	kDir,
	kDirs,
	kFile,
	kFiles,
	kGoof,
	kGoofs,
	k_amount,
	k_Undefined
};

static void Fill_(
	const drows_ &Rows,
	dtree_ &Tree,
	drow__ Parent )
{
	sdr::row__ Row = Rows.First();

	while ( Row != qNIL ) {
		Tree.BecomeLastChild( Rows( Row ), Parent );

		Row = Rows.Next( Row );
	}
}

static void Reorganize_(
	const file_tree_ &Tree,
	drow__ Root,
	dtree_ &NewTree )
{
	dtr::browser__<drow__> Browser;
	ctn::E_CITEMt( directory_, drow__ ) Directory;

	Directory.Init( Tree.Directories );
	NewTree.Allocate( Tree.Amount(), aem::mFitted );

	if ( Root != qNIL ) {
		Fill_( Directory( Root ).Dirs, NewTree, Root );
	}

	Browser.Init( Root );

	while ( Tree.Browse( Browser ) != qNIL ) {
		switch ( Browser.GetKinship() ) {
		case dtr::kChild:
		case dtr::kSibling:
			Fill_( Directory( Browser.Position() ).Dirs, NewTree, Browser.Position() );
			break;
		case dtr::kParent:
			break;
		default:
			qRGnr();
			break;
		}
	}
}

static void Reorganize_(
	file_tree_ &Tree,
	drow__ Root )
{
qRH
	dtree NewTree;
qRB
	NewTree.Init();
	Reorganize_( Tree, Root, NewTree);
	Tree.dtree_::operator =( NewTree );
qRR
qRT
qRE
}

static void Sort_(
	directories_ &Directories,
	const vKernel &Kernel,
	dwtbsc::sort_type__ SortType )
{
qRH
	directory Directory;
	drow__ Row = qNIL;
qRB
	Row = Directories.First();

	while ( Row != qNIL ) {
		Directory.Init();
		Directories.Recall( Row, Directory );

		Sort_( Directory, Kernel, SortType );

		Directories.Store( Directory, Row );

		Row = Directories.Next( Row );
	}
qRR
qRT
qRE
}

void dwtftr::Sort(
	vKernel &Kernel,
	dwtbsc::sort_type__ SortType )
{
	Sort_( Kernel.Directories, Kernel, SortType );
}

void dwtftr::Sort(
	drow__ Root,
	file_tree_ &Tree,
	dwtbsc::sort_type__ SortType )
{
	if ( Root != qNIL ) {
		Sort( Tree, SortType );

		Reorganize_( Tree, Root );
	}
}

typedef bso::uint__ depth__;

#define DEPTH_MAX	BSO_UINT_MAX

drow__ dwtftr::Load(
	xml::parser___ &Parser,
	version__ Version,
	vKernel &Kernel,
	dtree_ &Tree,
	load_observer__ &Observer )
{
	drow__ Root = qNIL;
qRH
	stsfsm::automat TagAutomat, AttributeAutomat;
	goof__ Goof;
	file__ File;
	directory Dir;
	kind__ Kind = kDir;
	drow__ DRow = qNIL;
	bso::bool__ Continue = true;
	dwtbsc::depth__ Depth = 0;
	drow__ Current = qNIL, New = qNIL;
	exclusion__ Exclusion = x_Undefined;
	sdr::row__ Error = qNIL;
	bso::uint__ Handled = 0, Total = 1;	// Pour le 'root'.
qRB
	TagAutomat.Init();
	FillTagAutomat_( Version,  TagAutomat );

	AttributeAutomat.Init();
	FillAttributeAutomat_( Version, AttributeAutomat );

	Observer.Report( 0, 0 );

	do {
		if ( Observer.IsElapsed() )
			Observer.Report( Handled, Total );

		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			if ( Depth >= DEPTH_MAX )
				qRLmt();
			Depth++;
			switch ( GetTag_( Parser.TagName(), TagAutomat ) ) {
			case tDirs:
				Kind = kDirs;
				Kernel.Directories( Current ) = Dir;
				Kernel.Directories.Flush();

				break;
			case tFiles:
				Kind = kFiles;
				break;
			case tGoofs:
				Kind = kGoofs;
				break;
			case tDir:
				Kind = kDir;

				Kernel.Directories.Flush();
				New = Kernel.Directories.New();

				Dir.Init();

				Tree.dtree_::Allocate( Kernel.Directories.Amount() );

				if ( Current != qNIL ) {
					Kernel.Directories( Current ).Dirs.Append( New );
					Tree.BecomeLastChild( New, Current );
				} else
					Root = New;

				Current = New;
				break;
			case tFile:
				Kind = kFile;
				File.Init();
				break;
			case tGoof:
				Kind = kGoof;
				Goof.Init();
				break;
			default:
				qRVct();
				break;
			}
			break;
		case xml::tAttribute:
			switch ( GetAttribute_( Parser.AttributeName(), AttributeAutomat ) ) {
			case aAmount:
				switch ( Kind ) {
				case kFile:
				case kFiles:
				case kGoof:
				case kGoofs:
				case kDir:
					break;
				case kDirs:
					Error = qNIL;
					Total += Parser.Value().ToUInt( &Error );

					if ( Error != qNIL )
						qRVct();

					break;
				default:
					qRGnr();
					break;
				}
				break;
			case aOddity:
				switch ( Kind ) {
				case kDirs:
				case kFiles:
				case kGoofs:
				case kDir:
				case kFile:
					qRGnr();
					break;
				case kGoof:
					Goof.Oddity = Kernel.Oddities.Append( Parser.Value() );
					break;
				default:
					qRGnr();
					break;
				}
				break;
			case aExclusion:
				Exclusion = GetExclusion_( Version, Parser.Value() );

				if ( Exclusion == x_Undefined )
					qRVct();

				switch ( Kind ) {
				case kDirs:
				case kFiles:
					break;
				case kDir:
					Dir().Exclusion = Exclusion;
					break;
				case kFile:
					File.Exclusion = Exclusion;
					break;
				default:
					qRGnr();
					break;
				}
				break;
			case aTimestamp:
				Error = qNIL;

				switch ( Kind ) {
				case kDirs:
				case kFiles:
					break;
				case kDir:
					Parser.Value().ToNumber( Dir().Timestamp, &Error );
					break;
				case kFile:
					Parser.Value().ToNumber( File.Timestamp, &Error );
					break;
				default:
					qRGnr();
					break;
				}

				if ( Error != qNIL )
					qRVct();

				break;
			case aSize:
				switch ( Kind ) {
				case kDirs:
				case kFiles:
					break;
				case kDir:
					qRVct();
					break;
				case kFile:
					Error = qNIL;
					Parser.Value().ToNumber( File.Size, &Error );
					if ( Error != qNIL )
						qRVct();
					break;
				default:
					qRGnr();
					break;
				}
				break;
			break; 
			case aName:
				switch ( Kind ) {
				case kDir:
					Dir().Name = Kernel.Names.Append( Parser.Value() );
					break;
				case kFile:
					File.Name = Kernel.Names.Append( Parser.Value() );
					break;
				case kGoof:
					Goof.Name =  Kernel.Names.Append( Parser.Value() );
					break;
				default:
					qRGnr();
					break;
				}
				break;
			default:
				qRVct();
				break;
			}
			break; 
		case xml::tValue:
			qRGnr();
			break;
		case xml::tEndTag:
			if ( Depth == 0 )
				qRGnr();

			Depth--;

			switch ( GetTag_( Parser.TagName(), TagAutomat ) ) {
			case tDirs:
				Dir = Kernel.Directories( Current );
				Kernel.Directories.Flush();
				break;
			case tDir:
				Kind = k_Undefined;

				if ( Dir().Exclusion == x_Undefined )
					Dir().Exclusion = xNo;
				/*
				if ( SortType != stNone )
					Sort_( Dir, Tree, SortType );
					*/
				Kernel.Directories( Current ) = Dir;
				Handled++;
				Kernel.Directories.Flush();

				if ( Current == Root )
					Continue = false;
				else
					Current = Tree.Parent( Current );

				break;
			case tFiles:
				break;
			case tFile:
				Kind = k_Undefined;

				if ( File.Exclusion == x_Undefined )
					File.Exclusion = xNo;

				Dir.Files.Append( Kernel.Files.Append( File ) );

				break;
			case tGoof:
				Kind = k_Undefined;

				Dir.Goofs.Append( Kernel.Goofs.Append( Goof ) );

				break;
			default:
				qRFwk();
				break;
			}
			break;
		case xml::t_Error:
			qRVct();
			break;
		default:
			qRGnr();
			break;
		}
	} while ( Depth != 0 );

	Observer.Report( Handled, Total );
	/*
	if ( SortType != stNone  )
		Reorganize_( Tree, Root );
		*/
qRR
qRT
qRE
	return Root;
}

Q37_GCTOR( fwtftr )
{
	FillVersionAutomat_();
	FillExclusionAutomats_();
}
