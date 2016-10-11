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

#define DWTCPR__COMPILATION

#include "dwtcpr.h"

using namespace dwtcpr;

static stsfsm::automat VersionAutomat_;
static stsfsm::automat StatusAutomatV1_;

const char *dwtcpr::GetLabel( version__ Version )
{
	switch ( Version ) {
	case v0_1:
		return "0.1";
		break;
	default:
		qRGnr();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

version__ dwtcpr::GetVersion( const str::string_ &Pattern )
{
	return stsfsm::GetId( Pattern, VersionAutomat_, v_Undefined, v_amount );
}

static void FillVersionAutomat_( void )
{
	VersionAutomat_.Init();
	stsfsm::Fill<version__>( VersionAutomat_, v_amount, GetLabel );
}

#define S( name )	case s##name : return #name; break

const char *GetLabel_(
	status__ Status,
	version__ Version = v_Current )
{
	if ( Version != v0_1 )
		qRChk();

	switch ( Status ) {
	S( Steady );
	S( Created );
	S( Removed );
	S( Modified );
	default:
		qRGnr();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

static status__ GetStatus_(
	version__ Version,
	const str::string_ &Pattern )
{
	stsfsm::automat_ *Automat = NULL;

	switch ( Version ) {
	case v0_1:
		Automat = &StatusAutomatV1_;
		break;
	default:
		qRGnr();
		break;
	}
	
	return stsfsm::GetId( Pattern, *Automat, s_Undefined, s_amount );
}

static void FillStatusAutomats_( void )
{
	StatusAutomatV1_.Init();
	stsfsm::Fill<status__,version__>( v0_1, StatusAutomatV1_, s_amount, GetLabel_ );
}

class fill_link__ {
public:
	drow__ Parent;
	dwtftr::drow__ Source;
	dwtftr::drow__ Target;
	void reset( bso::bool__ = true )
	{
		Parent = qNIL;
		Source = Target = qNIL;
	}
	E_CDTOR( fill_link__ );
	void Init( void )
	{
		Parent = qNIL;
		Source = Target = qNIL;
	}
};

typedef stkbch::qBSTACKdl( fill_link__ ) dFillLinks;
qW( FillLinks );


static void Complete_(
	sdr::row__ Row,
	const dwtftr::drows_ &Rows,
	const dwtftr::directories_ &Directories,
	const dwtftr::names_ &Names,
	status__ Status,
	drows_ &RRows,
	directories_ &RDirectories,
	names_ &RNames,
	dFillLinks &Links )
{
qRH
	directory Dir;
	fill_link__ Link;
	drow__ NewRow = qNIL;
qRB
	while ( Row != qNIL ) {
		if ( Directories( Rows( Row ) )().Exclusion == dwtbsc::xNo ) {
			Dir.Init();
			Dir().Name = RNames.Append( Names( Directories( Rows( Row ) )().Name ) );
			Dir().Status = Status;

			RRows.Append( NewRow = RDirectories.Append( Dir ) );

			if ( Status == sCreated ) {
				Link.Init();
				Link.Parent = NewRow;
				Link.Source = Rows( Row );
				// Link.Target = TRows( TRow );	// Répertoire n'existant pas dans la cible, 'Target' est laissé à 'qNIL' pour signaler le fait qu'il faut mettre tous les sous-répertoires en 'Created'.
				Links.Push( Link );
			}
		}

		Row = Rows.Next( Row );
	}
qRR
qRT
qRE
}

static inline bso::sign__ Compare_(
	const str::string_ &Source,
	const str::string_ &Target )
{
	return str::Compare( Source, Target );
}

static bso::bool__ Compare_(
	const dwtftr::drows_ &SRows,
	const dwtftr::directories_ &SDirectories,
	const dwtftr::names_ &SNames,
	const dwtftr::drows_ &TRows,
	const dwtftr::directories_ &TDirectories,
	const dwtftr::names_ &TNames,
	drows_ &Rows,
	directories_ &Directories,
	names_ &Names,
	dFillLinks &Links )
{
	bso::bool__ Modified = false;
qRH
	sdr::row__ SRow = qNIL, TRow = qNIL;
	ctn::qCMITEMs( name_, nrow__ ) SName, TName;
	ctn::qCITEMs( dwtbsc::directory_, dwtbsc::drow__) SDir, TDir;
	directory Dir;
	fill_link__ Link;
	drow__ Row = qNIL;
	dwtbsc::exclusion__ Exclusion = dwtbsc::x_Undefined;
qRB
	SName.Init( SNames );
	TName.Init( TNames );

	SDir.Init( SDirectories );
	TDir.Init( TDirectories );

	SRow = SRows.First();
	TRow = TRows.First();

	while ( ( SRow != qNIL ) && ( TRow != qNIL ) ) {
		Exclusion = SDir( SRows( SRow ) )().Exclusion;
		Dir.Init();
		switch ( Compare_( SName( SDir( SRows( SRow ) )().Name ), TName( TDir( TRows( TRow ) )().Name ) ) ) {
		case -1:
			if ( Exclusion == dwtbsc::xNo ) {
				Rows.Append( Row = Directories.New() );

				Dir().Name = Names.Append( SName() );	// Déjà positionné.
				Dir().Status = sCreated;
				Modified = true;
				Directories.Store( Dir, Row );

				Link.Init();
				Link.Parent = Row;
				Link.Source = SRows( SRow );
				// Link.Target = TRows( TRow );	// Répertoire n'existant pas dans la cible, 'Target' est laissé à 'qNIL' pour signaler le fait qu'il faut mettre tous les sous-répertoires en 'Created'.
				Links.Push( Link );
}
			SRow = SRows.Next( SRow );
			break;
		case 0:
			if ( Exclusion == dwtbsc::xNo ) {
				Rows.Append( Row = Directories.New() );

				Dir().Name = Names.Append( SName() );	// Déjà positionné.
				Dir().Status = sSteady;
				Directories.Store( Dir, Row );

				Link.Init();
				Link.Parent = Row;
				Link.Source = SRows( SRow );
				Link.Target = TRows( TRow );
				Links.Push( Link );
			}

			SRow = SRows.Next( SRow );
			TRow = TRows.Next( TRow );
			break;
		case 1:
			Rows.Append( Row = Directories.New() );

			Dir().Name = Names.Append( TName() );	// Déjà positionné.
			Dir().Status = sRemoved;
			TRow = TRows.Next( TRow );
			Modified = true;
			Directories.Store( Dir, Row );
			break;
		default:
			qRGnr();
			break;
		}
	}

	if ( SRow != qNIL ) {
		Complete_( SRow, SRows, SDirectories, SNames, sCreated, Rows, Directories, Names, Links );
		Modified = true;
	}

	if ( TRow != qNIL ) {
		Complete_( TRow, TRows, TDirectories, TNames, sRemoved, Rows, Directories, Names, Links );
		Modified = true;
	}
qRR
qRT
qRE
	return Modified;
}

static inline bso::bool__ Compare_(
	dwtftr::drow__ SRow,
	const dwtftr::dKernel &Source,
	dwtftr::drow__ TRow,
	const dwtftr::dKernel &Target,
	drows_ &Rows,
	directories_ &Directories,
	names_ &Names,
	dFillLinks &Links )
{

	ctn::qCITEMs( dwtftr::directory_, dwtftr::drow__) SDir, TDir;

	SDir.Init( Source.Directories );
	TDir.Init( Target.Directories );

	return Compare_( SDir( SRow ).Dirs, Source.Directories, Source.Names, TDir( TRow ).Dirs, Target.Directories, Target.Names, Rows, Directories, Names, Links );
}

template<typename row, typename tree, typename rows> void SetTree_(
	row Parent,
	tree &Tree,
	const rows &Rows )
{
	sdr::row__ Row = Rows.First();

	while ( Row != qNIL ) {
		Tree.BecomeLastChild( Rows( Row ), Parent );

		Row = Rows.Next( Row );
	}
}

static void Complete_(
	sdr::row__ Row,
	const dwtftr::frows_ &Rows,
	const dwtftr::files_ &Files,
	const dwtftr::names_ &Names,
	status__ Status,
	frows_ &RRows,
	files_ &RFiles,
	names_ &RNames )
{
qRH
	file__ File;
qRB
	while ( Row != qNIL ) {
		if ( Files( Rows( Row ) ).Exclusion == dwtbsc::xNo ) {
			File.Init();
			File.Name = RNames.Append( Names( Files( Rows( Row ) ).Name ) );
			File.Status = Status;

			RRows.Append( RFiles.Append( File ) );
		}

		Row = Rows.Next( Row );
	}
qRR
qRT
qRE
}

namespace {
	time_t AbsDiff_(
		time_t Op1,
		time_t Op2 )
	{
		return ( Op1 > Op2 ? Op1 - Op2 : Op2 - Op1 );
	}
}

static bso::bool__ IsDifferent_(
	const dwtftr::file__ &SFile,
	const dwtftr::file__ &TFile )
{
	return ( AbsDiff_( SFile.Timestamp, TFile.Timestamp ) > 2 ) || ( SFile.Size != TFile.Size ); // Timestamps within 3 seconds are considered as equal (due to Windows timestamp imprecision).
}

static bso::bool__ Compare_(
	const dwtftr::frows_ &SRows,
	const dwtftr::files_ &SFiles,
	const dwtftr::names_ &SNames,
	const dwtftr::frows_ &TRows,
	const dwtftr::files_ &TFiles,
	const dwtftr::names_ &TNames,
	frows_ &Rows,
	files_ &Files,
	names_ &Names )
{
	bso::bool__ Modified = false;
qRH
	sdr::row__ SRow = qNIL, TRow = qNIL;
	ctn::qCMITEMs( name_, nrow__ ) SName, TName;
	file__ File;
	dwtbsc::exclusion__ Exclusion = dwtbsc::x_Undefined;
qRB
	SName.Init( SNames );
	TName.Init( TNames );

	SRow = SRows.First();
	TRow = TRows.First();

	while ( ( SRow != qNIL ) && ( TRow != qNIL ) ) {
		Exclusion = SFiles( SRows( SRow ) ).Exclusion;
		switch ( Compare_( SName( SFiles( SRows( SRow ) ).Name ), TName( TFiles( TRows( TRow ) ).Name ) ) ) {
		case -1:
			if ( Exclusion == dwtbsc::xNo ) {
				File.Init();
				File.Name = Names.Append( SName() );	// Déjà positionné.
				File.Status = sCreated;
				Rows.Append( Files.Append( File ) );
				Modified = true;
			}
			SRow = SRows.Next( SRow );
			break;
		case 0:
			if ( ( Exclusion == dwtbsc::xNo ) && IsDifferent_( SFiles( SRows( SRow ) ), TFiles( TRows( TRow ) ) ) ) {
				File.Init();
				File.Name = Names.Append( SName() );	// Déjà positionné.
				File.Status = sModified;
				Rows.Append( Files.Append( File ) );
				Modified = true;
			}

			SRow = SRows.Next( SRow );
			TRow = TRows.Next( TRow );
			break;
		case 1:
			File.Init();
			File.Name = Names.Append( TName() );	// Déjà positionné.
			File.Status = sRemoved;
			Rows.Append( Files.Append( File ) );
			TRow = TRows.Next( TRow );
			Modified = true;
			break;
		default:
			qRGnr();
			break;
		}
	}

	if ( SRow != qNIL ) {
		Complete_( SRow, SRows, SFiles, SNames, sCreated, Rows, Files, Names );
		Modified = true;
	}

	if ( TRow != qNIL ) {
		Complete_( TRow, TRows, TFiles, TNames, sRemoved, Rows, Files, Names );
		Modified = true;
	}
qRR
qRT
qRE
	return Modified;
}

static bso::bool__ inline Compare_(
	dwtftr::drow__ SRow,
	const dwtftr::dKernel &Source,
	dwtftr::drow__ TRow,
	const dwtftr::dKernel &Target,
	frows_ &Rows,
	files_ &Files,
	names_ &Names )
{

	ctn::qCITEMs( dwtftr::directory_, dwtftr::drow__ ) SDir, TDir;

	SDir.Init( Source.Directories );
	TDir.Init( Target.Directories );

	return Compare_( SDir( SRow ).Files, Source.Files, Source.Names, TDir( TRow ).Files, Target.Files, Target.Names, Rows, Files, Names );
}

static void MarkAsModified_(
	scene_ &Scene,
	drow__ Row )
{
	while ( ( Row != qNIL ) && ( Scene.Directories( Row )().Status == sSteady ) ) {
		Scene.Directories( Row )().Status = sModified;

		Row = Scene.Parent( Row );
	}

	Scene.Directories.Flush();
}

// Ajoute tous les sous-répertoires d'un nouveau répertoire.
static void Put_(
	drow__ Parent,
	const dwtftr::drows_ &Rows,
	const dwtftr::directories_ &Directories,
	const dwtftr::names_ &Names,
	drows_ &RRows,
	directories_ &RDirectories,
	names_ &RNames,
	dFillLinks &Links )
{
qRH
	directory Dir;
	sdr::row__ Row = qNIL;
	fill_link__ Link;
qRB
	Row = Rows.First();

	while ( Row != qNIL ) {
		if ( Directories( Rows( Row ) )().Exclusion == dwtbsc::xNo ) {
			Dir.Init();
			Dir().Name = RNames.Append( Names( Directories( Rows( Row ) )().Name ) );
			Dir().Status = sCreated;

			Link.Init();
			Link.Parent = RDirectories.Append( Dir );
			Link.Source = Rows( Row );
			// Link.Target = TRows( TRow );	// Répertoire n'existant pas dans la cible, 'Target' est laissé à 'qNIL' pour signaler le fait qu'il faut mettre tous les sous-répertoires en 'Created'.
			Links.Push( Link );

			RRows.Append( Link.Parent );
		}

		Row = Rows.Next( Row );
	}
qRR
qRT
qRE
}

static void Put_(
	drow__ Parent,
	dwtftr::drow__ Row,
	const dwtftr::dKernel &Kernel,
	frows_ &FRows,
	files_ &Files,
	drows_ &DRows,
	directories_ &Directories,
	names_ &Names,
	dFillLinks &Links )
{

	ctn::qCITEMs( dwtftr::directory_, dwtftr::drow__) Dir;

	Dir.Init( Kernel.Directories );

	Put_( Parent, Dir( Row ).Dirs, Kernel.Directories, Kernel.Names, DRows, Directories, Names, Links );
	Complete_( Dir( Row ).Files.First(), Dir( Row ).Files, Kernel.Files, Kernel.Names, sCreated, FRows, Files, Names );
}


drow__ dwtcpr::Compare(
	const dwtftr::dKernel &Source,
	const dwtftr::dKernel &Target,
	scene_ &Scene,
	comparison_observer__ &Observer )
{
	drow__ Root = qNIL;
qRH
	directory Dir;
	wFillLinks Links;
	fill_link__ Link;
	bso::bool__ Modified = false;
	bso::int__ Handled = 1;
	tol::timer__ Timer;
qRB
	Links.Init();

	Dir.Init();
	Dir().Status = sSteady;
	Dir().Name = Scene.Names.Append( str::string() );

	Root = Scene.Directories.New();

	Observer.Report( 0, Handled );

	Modified = Compare_( Source.Directories.First(), Source, Target.Directories.First(), Target, Dir.Dirs, Scene.Directories, Scene.Names, Links );
	Modified |= Compare_( Source.Directories.First(), Source, Target.Directories.First(), Target, Dir.Files, Scene.Files, Scene.Names );

	Scene.Directories.Store( Dir, Root );
	Scene.Allocate( Scene.Directories.Amount() );
	SetTree_( Root, Scene, Dir.Dirs );

	if ( Modified  )
		MarkAsModified_( Scene, Root );

	while ( Links.Amount() ) {
		Handled++;

		if ( Timer.IsElapsed() )
			Observer.Report( Handled, Handled + Links.Amount() );

		Modified = false;

		Link.Init();
		Links.Pop( Link );

		Dir.Init();
		Scene.Directories.Recall( Link.Parent, Dir );

		if ( Link.Target != qNIL ){
			Modified = Compare_( Link.Source, Source, Link.Target, Target, Dir.Dirs, Scene.Directories, Scene.Names, Links );
			Modified |= Compare_( Link.Source, Source, Link.Target, Target, Dir.Files, Scene.Files, Scene.Names );
		}
		else // On est dans le cas d'un répertoire nouvellement crée, et on veut donc ajouter tout son contenu à l'arbre des modifications.
			Put_( Link.Parent, Link.Source, Source, Dir.Files, Scene.Files, Dir.Dirs, Scene.Directories, Scene.Names, Links );

		Scene.Directories.Store( Dir, Link.Parent );

		Scene.Allocate( Scene.Directories.Amount() );

		SetTree_( Link.Parent, Scene, Dir.Dirs );

		if ( Modified  )
			MarkAsModified_( Scene, Link.Parent );
	}

	Observer.Report( Handled, Handled );
qRR
qRT
qRE
	return Root;
}

enum tag__
{
	tDirs,
	tDir,
	tFiles,
	tFile,
	t_amount,
	t_Undefined
};

#define T( name )	case t##name : return #name; break

static inline const char *GetLabel_(
	tag__ Tag,
	version__ Version = v_Current )
{
	if ( Version != v0_1 )
		qRChk();

	switch ( Tag ) {
	T( Dirs );
	T( Dir );
	T( Files );
	T( File );
	default:
		qRGnr();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

enum attribute__
{
	aName,
	aStatus,
	aAmount,
	a_amount,
	a_Undefined
};

#define A( name )	case a##name : return #name; break

static inline const char *GetLabel_(
	attribute__ Attribute,
	version__ Version = v_Current )
{
	if ( Version != v0_1 )
		qRChk();

	switch ( Attribute ) {
	A( Name );
	A( Status );
	A( Amount );
	default:
		qRGnr();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

static inline void DumpCore_(
	bso::bool__ IsFile,
	const core__ &Core,
	const names_ &Names,
	xml::writer_ &Writer )
{
	Writer.PutAttribute( GetLabel_( aName ), Names( Core.Name ) );

	switch ( Core.Status ) {
	case sSteady:
		break;
	case sCreated:
		Writer.PutAttribute( GetLabel_( aStatus ), GetLabel_( sCreated ) );
		break;
	case sRemoved:
		Writer.PutAttribute( GetLabel_( aStatus ), GetLabel_( sRemoved ) );
		break;
	case sModified:
		if ( IsFile )
			Writer.PutAttribute( GetLabel_( aStatus ), GetLabel_( sModified ) );
		break;
	default:
		qRGnr();
		break;
	}
}

static inline void Dump_( 
	const frows_ &Rows,
	const files_ &Files,
	const names_ &Names,
	xml::writer_ &Writer )
{
	if ( Rows.Amount() != 0 ) {
		sdr::row__ Row = Rows.First();

		Writer.PushTag( GetLabel_( tFiles ) );
		xml::PutAttribute( GetLabel_( aAmount ), Rows.Amount(), Writer );

		while ( Row != qNIL ) {
			Writer.PushTag( GetLabel_( tFile ) );

			DumpCore_( true, Files( Rows( Row ) ), Names, Writer );

			Writer.PopTag();

			Row = Rows.Next( Row );
		}

		Writer.PopTag();
	}
}

static inline void PushDirectory_(
	const directory_ &Directory,
	const names_ &Names,
	xml::writer_ &Writer )
{
	Writer.PushTag( GetLabel_( tDir ) );

	DumpCore_( false, Directory(), Names, Writer );
}

void dwtcpr::Dump( 
	drow__ Root,
	const scene_ &Scene,
	xml::writer_ &Writer )
{
	dtr::browser__<drow__> Browser;
//	drow__ ChildNotHandled = qNIL;
	bso::bool__ LastPop = false;

	PushDirectory_( Scene.Directories( Root ), Scene.Names, Writer );
	Dump_( Scene.Directories( Root ).Files, Scene.Files, Scene.Names, Writer );

	Browser.Init( Root );

	while ( Scene.Browse( Browser ) != qNIL ) {
		switch ( Browser.GetKinship() ) {
		case dtr::kChild:
			Writer.PushTag( GetLabel_( tDirs ) );
			xml::PutAttribute( GetLabel_( aAmount ), Scene.Directories( Scene.Parent( Browser.Position() ) ).Dirs.Amount(), Writer );
			PushDirectory_( Scene.Directories( Browser.GetPosition() ), Scene.Names, Writer );
			Dump_( Scene.Directories( Browser.GetPosition() ).Files, Scene.Files, Scene.Names, Writer );
			break;
		case dtr::kSibling:
			Writer.PopTag();	// Dir
			PushDirectory_( Scene.Directories( Browser.GetPosition() ), Scene.Names, Writer );
			Dump_( Scene.Directories( Browser.GetPosition() ).Files, Scene.Files, Scene.Names, Writer );
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

#define UNDEFINED_ID	DWTBSC_UNDEFINED_ID

inline static stsfsm::id__ GetItem_(
	const str::string_ &Name,
	const stsfsm::automat_ &Automat )
{
	return stsfsm::GetId( Name, Automat );
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
enum kind__ {
	kDir,
	kDirs,
	kFile,
	kFiles,
	k_amount,
	k_Undefined
};

typedef bso::uint__ depth__;

#define DEPTH_MAX	BSO_UINT_MAX

drow__ dwtcpr::Load(
	xml::parser___ &Parser,
	version__ Version,
	scene_ &Scene,
	load_observer__ &Observer )
{
	drow__ Root = qNIL;
qRH
	stsfsm::automat TagAutomat, AttributeAutomat;
	file__ File;
	directory Dir;
	kind__ Kind = kDir;
	drow__ DRow = qNIL;
	bso::bool__ Continue = true;
	depth__ Depth = 0;
	drow__ Current = qNIL, New = qNIL;
	sdr::row__ Error = qNIL;
	bso::uint__ Handled = 0, Total = 1;	// Pour le 'root'.
qRB
	TagAutomat.Init();
	FillTagAutomat_(Version,  TagAutomat );

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

				if ( Dir().Status == s_Undefined )
					Dir().Status = sSteady;

				Scene.Directories( Current ) = Dir;
				Scene.Directories.Flush();

				break;
			case tFiles:
				Kind = kFiles;
				break;
			case tDir:
				Kind = kDir;

				Scene.Directories.Flush();
				New = Scene.Directories.New();

				Dir.Init();

				Scene.dtree_::Allocate( Scene.Directories.Amount() );

				if ( Current != qNIL ) {
					Scene.Directories( Current ).Dirs.Append( New );
					Scene.BecomeLastChild( New, Current );
				} else
					Root = New;

				Current = New;
				break;
			case tFile:
				Kind = kFile;
				File.Init();
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
			case aName:
				switch ( Kind ) {
				case kDir:
					Dir().Name = Scene.Names.Append( Parser.Value() );
					break;
				case kFile:
					File.Name = Scene.Names.Append( Parser.Value() );
					break;
				default:
					qRGnr();
					break;
				}
				break;
			case aStatus:
				switch ( Kind ) {
				case kFile:
					File.Status = GetStatus_( Version, Parser.Value() );
					break;
				case kDir:
					Dir().Status = GetStatus_( Version, Parser.Value() );
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
				Dir = Scene.Directories( Current );
				Scene.Directories.Flush();
				break;
			case tDir:
				Kind = k_Undefined;

				if ( Dir().Status == s_Undefined )
					Dir().Status = sSteady;

				Scene.Directories( Current ) = Dir;
				Handled++;
				Scene.Directories.Flush();

				if ( Current == Root )
					Continue = false;
				else
					Current = Scene.Parent( Current );

				break;
			case tFiles:
				break;
			case tFile:
				Kind = k_Undefined;

				if ( File.Status == s_Undefined )
					File.Status = sSteady;

				Dir.Files.Append( Scene.Files.Append( File ) );

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
qRR
qRT
qRE
	return Root;
}

class clean_link__ {
public:
	drow__ Parent;
	drow__ Row;
	void reset( bso::bool__ = true )
	{
		Parent = qNIL;
		Row = qNIL;
	}
	E_CDTOR( clean_link__ );
	void Init( void )
	{
		Parent = qNIL;
		Row = qNIL;
	}
};

typedef stkbch::qBSTACKdl( clean_link__ ) dCleanLinks;
qW( CleanLinks );


static void Fill_(
	const drows_ &Rows,
	const directories_ &Directories,
	const names_ &Names,
	drows_ &NewRows,
	directories_ &NewDirectories,
	names_ &NewNames,
	dCleanLinks &Links )
{
qRH
	sdr::row__ Row = qNIL;
	directory NewDir;
	clean_link__ Link;
qRB
	Row = Rows.First();

	while ( Row != qNIL) {
		if ( Directories( Rows( Row ) )().Status != sSteady ) {
			NewDir.Init();
			NewDir().Name = NewNames.Append( Names( Directories( Rows( Row ) )().Name ) );
			NewDir().Status = Directories( Rows( Row ) )().Status;

			Link.Init();
			NewRows.Append( Link.Parent = NewDirectories.Append( NewDir ) );
			Link.Row = Rows( Row );
			Links.Push( Link );

		}

		Row = Rows.Next( Row );
	}
qRR
qRT
qRE
}

static inline void Fill_(
	drow__ Row,
	const scene_ &Scene,
	drows_ &Rows,
	directories_ &Directories,
	names_ &Names,
	dCleanLinks &Links )
{
	ctn::qCITEMs( directory_, drow__) Dir;

	Dir.Init( Scene.Directories );

	Fill_( Dir( Row ).Dirs, Scene.Directories, Scene.Names, Rows, Directories, Names, Links );
}

static void Fill_(
	const frows_ &Rows,
	const files_ &Files,
	const names_ &Names,
	frows_ &NewRows,
	files_ &NewFiles,
	names_ &NewNames )
{
qRH
	sdr::row__ Row = qNIL;
	file__ File;
qRB
	Row = Rows.First();

	while ( Row != qNIL ) {
		if ( Files( Rows( Row ) ).Status != sSteady ) {
			File.Init();
			File.Name = NewNames.Append( Names( Files( Rows( Row ) ).Name ) );
			File.Status = Files( Rows( Row ) ).Status;
			NewRows.Append( NewFiles.Append( File ) );
			Row = Rows.Next( Row );
		}
	}
qRR
qRT
qRE
}

static void inline Fill_(
	drow__ Row,
	const scene_ &Scene,
	frows_ &Rows,
	files_ &Files,
	names_ &Names )
{
	Fill_( Scene.Directories( Row ).Files, Scene.Files, Scene.Names, Rows, Files, Names );
}

drow__ dwtcpr::Clean(
	const scene_ &Scene,
	drow__ Root,
	scene_ &NewScene )
{
	drow__ NewRoot = qNIL;
qRH
	directory Dir;
	wCleanLinks Links;
	clean_link__ Link;
qRB
	Links.Init();

	Dir.Init();
	Dir().Status = Scene.Directories( Root )().Status;
	Dir().Name = NewScene.Names.Append( str::string() );

	NewRoot = NewScene.Directories.New();

	Fill_( Root, Scene, Dir.Dirs, NewScene.Directories, NewScene.Names, Links );
	Fill_( Root, Scene, Dir.Files, NewScene.Files, NewScene.Names );

	NewScene.Directories.Store( Dir, Root );

	NewScene.Allocate( NewScene.Directories.Amount() );
	SetTree_( Root, NewScene, Dir.Dirs );

	while ( Links.Amount() ) {
		Link.Init();
		Links.Pop( Link );

		Dir.Init();
		NewScene.Directories.Recall( Link.Parent, Dir );

		Fill_( Link.Row, Scene, Dir.Dirs, NewScene.Directories, NewScene.Names, Links );
		Fill_( Link.Row, Scene, Dir.Files, NewScene.Files, NewScene.Names );

		NewScene.Directories.Store( Dir, Link.Parent );

		NewScene.Allocate( NewScene.Directories.Amount() );
		SetTree_( Link.Parent, NewScene, Dir.Dirs );
	}
qRR
qRT
qRE
	return Root;
}

drow__ dwtcpr::Clean(
	scene_ &Scene,
	drow__ Root )
{
qRH
	scene NewScene;
qRB
	NewScene.Init();

	Root = Clean( Scene, Root, NewScene );

	Scene = NewScene;
qRR
qRT
qRE
	return Root;
}

Q37_GCTOR( dwtcpr )
{
	FillVersionAutomat_();
	FillStatusAutomats_();
}
