/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of fwtchrq.

    fwtchrq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    fwtchrq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with fwtchrq.  If not, see <http://www.gnu.org/licenses/>
*/

#include "registry.h"
// #include "browse.h"
#include "misc.h"
#include "exclusion.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

E_CDEF( char *, ExplorationMessage_, "ExplorationMessage" );
E_CDEF( char *, ProcessingMessage_, "ProcessingMessage" );
E_CDEF( char *, UpdateMessage_, "UpdateMessage" );
E_CDEF( char *, GhostsSettingMessage_, "GhostsSettingMessage" );
E_CDEF( char *, ComparisonMessage_, "ComparisonMessage" );


E_CDEF( tol::delay__, Delay_, 700 );

static void PrintHeader_( void )
{
	COut << MISC_NAME_MC " V" VERSION << " (" MISC_WEBSITE_URL ")" << txf::nl;
	COut << "Copyright (C) " MISC_COPYRIGHT << txf::nl;
	COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
}

static void Browse_(
	const rgstry::multi_level_registry_ &Registry,
	const str::string_ &Path,
	const str::string_ &Generator,
	const fnm::name___ &OutputFilename,
	dwtdct::exploration_observer__ &ExplorationObserver,
	dwtftr::processing_observer__ &ProcessingObserver	)
{
qRH
	bso::uint__ ThreadAmountMax = 0;
	dwtftr::file_tree Tree;
	dwtmov::movings Movings;
	dwtftr::drow__ Root = qNIL;
	dwtbsc::exclusions_handling__ ExclusionsHandling = dwtbsc::eh_Undefined;
	dwtxcl::excluder Excluder;
	dwtbsc::limitations__ Limitations;
	dwtbsc::ghosts_oddities GO;
	dwtdct::content Content;
qRB
	ThreadAmountMax = sclrgstry::OGetUInt( Registry, registry::ThreadAmountMax, 0 );

	ExclusionsHandling = misc::GetExclusionsHandling_( Registry );

	GO.Init();

	exclusion::Get( Registry, GO );

	Excluder.Init( GO );
	Limitations.Init();
	exclusion::Fill( ExclusionsHandling, Registry, Excluder, Limitations );

	Content.Init();
	dwtdct::Explore( Path, ThreadAmountMax, Excluder, Limitations, GO, ExclusionsHandling, Content, ExplorationObserver );

	Tree.Init();
	Root = dwtftr::Process( Content, Tree, ProcessingObserver );

	misc::Dump( Root, Tree, OutputFilename );
#if 0

	Movings.Init();
	fwtmov::Explore( Path, Content, GO, Movings );

	Store.InitAsTreeAndMovings( Root, Tree, Movings );

	store::Save( Store, Generator, Output );

//	Dump_( Generator, Row, Tree, Movings, tol::EpochTime( false ) - Start, ThreadAmountMax, Output );
#endif
qRR
qRT
qRE
}

static void Browse_(
	const str::string_ &Path,
	const str::string_ &OutputFilename )
{
qRH
	str::string
		Generator,
		ExplorationMessage,
		ProcessingMessage;
	dwtdct::basic_exploration_observer___ ExplorationObserver;
	dwtftr::basic_processing_observer___ ProcessingObserver;
qRB
	Generator.Init( MISC_NAME_MC " V" VERSION " (" );
	Generator.Append( cpe::GetDescription() );
	Generator.Append( ')' );

	ExplorationMessage.Init(); 
	sclmisc::GetBaseTranslation( ExplorationMessage_, ExplorationMessage );

	ProcessingMessage.Init(); 
	sclmisc::GetBaseTranslation( ProcessingMessage_, ProcessingMessage );

	ExplorationObserver.Init( OutputFilename.Size() == 0 ? 0 : Delay_, ExplorationMessage, cio::COut );
	ProcessingObserver.Init( OutputFilename.Size() == 0 ? 0 : Delay_,ProcessingMessage, cio::COut );

	Browse_( sclmisc::GetRegistry(), Path, Generator, OutputFilename, ExplorationObserver, ProcessingObserver );
qRR
qRT
qRE
}

static void Browse_( void )
{
qRH
	str::string Path, Output;
qRB
	Path.Init();
	sclmisc::MGetValue( registry::Path, Path );

	misc::NormalizeAndTestPath( Path );

	Output.Init();
	sclmisc::OGetValue( registry::Output, Output );

	Browse_( Path, Output );
qRR
qRT
qRE
}

static void Update_( const str::string_ &Path )
{
qRH
	bso::uint__ ThreadAmountMax = 0;
	dwtdct::content Content;
	dwtxcl::excluder Excluder;
	dwtbsc::limitations__ Limitations;
	dwtbsc::ghosts_oddities GO;
	dwtdct::basic_exploration_observer___ ExplorationObserver;
	dwtdct::rBasicGhostsSettingObserver GhostsSettingObserver;
	str::string ExplorationMessage, UpdateMessage;
	dwtdct::ghost2files_rack___ G2FRack;
	dwtftr::basic_processing_observer___ ProcessingObserver;
	str::string ProcessingMessage;
	dwtdct::i2g I2G;
qRB
	ThreadAmountMax = sclmisc::OGetUInt( registry::ThreadAmountMax, 0 );

	GO.Init();
	exclusion::Get( sclmisc::GetRegistry(), GO );

	Excluder.Init( GO );
	Limitations.Init();
	exclusion::Fill( sclmisc::GetRegistry(), Excluder, Limitations );

	ExplorationMessage.Init();
	sclmisc::GetBaseTranslation( ExplorationMessage_, ExplorationMessage );

	Content.Init();
	ExplorationObserver.Init( Delay_, ExplorationMessage, cio::COut );
	dwtdct::Explore( Path, ThreadAmountMax, Excluder, Limitations, GO, dwtbsc::ehKeepGhostLike, Content, ExplorationObserver );

	cio::COut << txf::nl;

	UpdateMessage.Init();
	sclmisc::GetBaseTranslation( UpdateMessage_, UpdateMessage );

	GhostsSettingObserver.Init( UpdateMessage, cio::COut, Delay_ );
	I2G.Init();
	dwtdct::SetGhosts( Path, Content, GO, GhostsSettingObserver, I2G );

	G2FRack.Init();

	dwtdct::ghost2files_ &G2F = dwtdct::GetRWG2F( Path, GO, G2FRack );

	G2F.Init();

	G2F.Append( Content, I2G );
qRR
qRT
qRE
}

static void Update_( void )
{
qRH
	str::string Path;
qRB
	Path.Init();
	sclmisc::MGetValue( registry::Path, Path );

	misc::NormalizeAndTestPath( Path );

	Update_( Path );
qRR
qRT
qRE
}

static void TestGhosts_( void )
{
qRH
	str::string Path, Output;
	bso::uint__ ThreadAmountMax = 0;
	dwtdct::content Content;
	dwtxcl::excluder Excluder;
	dwtbsc::limitations__ Limitations;
	dwtbsc::ghosts_oddities GO;
	dwtdct::basic_exploration_observer___ ExplorationObserver;
	dwtdct::rBasicGhostsSettingObserver GhostsSettingObserver;
	str::string NoGhostMessage, GhostIgnoredMessage;
	str::string GhostsSettingMessage, ExplorationMessage;
	sclmisc::text_oflow_rack___ TFRack;
qRB
	Path.Init();
	sclmisc::MGetValue( registry::Path, Path );

	misc::NormalizeAndTestPath( Path );

	ThreadAmountMax = sclmisc::OGetUInt( registry::ThreadAmountMax, 0 );

	GO.Init();
	exclusion::Get( sclmisc::GetRegistry(), GO );

	Excluder.Init( GO );
	Limitations.Init();
	exclusion::Fill( sclmisc::GetRegistry(), Excluder, Limitations );

	ExplorationMessage.Init(); 
	sclmisc::GetBaseTranslation( ExplorationMessage_, ExplorationMessage );

	Content.Init();
	ExplorationObserver.Init( Delay_, ExplorationMessage, cio::COut );
	dwtdct::Explore( Path, ThreadAmountMax, Excluder, Limitations, GO, dwtbsc::eh_Default, Content, ExplorationObserver );

	cio::COut << txf::nl;

	NoGhostMessage.Init();
	sclmisc::GetBaseTranslation( "NoGhost", NoGhostMessage );

	GhostIgnoredMessage.Init();
	sclmisc::GetBaseTranslation( "GhostIgnored", GhostIgnoredMessage );

	Output.Init();
	sclmisc::OGetValue( registry::Output, Output );

	GhostsSettingMessage.Init();
	sclmisc::GetBaseTranslation( GhostsSettingMessage_, GhostsSettingMessage );

	GhostsSettingObserver.Init( GhostsSettingMessage, cio::COut, Delay_ );
	dwtdct::TestGhosts( Path, Content, GO, NoGhostMessage, GhostIgnoredMessage, TFRack.Init( Output ) );
qRR
	TFRack.HandleError();
qRT
qRE
}

static void Compare_(
	const rgstry::multi_level_registry_ &Registry,
	const str::string_ &Path,
	const str::string_ &Generator,
	const fnm::name___ &OutputFilename,
	dwtdct::exploration_observer__ &ExplorationObserver,
	dwtftr::processing_observer__ &ProcessingObserver	)
{
qRH
	bso::uint__ ThreadAmountMax = 0;
	dwtftr::file_tree New;
	dwtmov::movings Movings;
	dwtbsc::exclusions_handling__ ExclusionsHandling = dwtbsc::eh_Undefined;
	dwtxcl::excluder Excluder;
	dwtbsc::limitations__ Limitations;
	dwtbsc::ghosts_oddities GO;
	dwtdct::content Content;
	dwtdct::wKernel Old;
	dwtdct::ghost2files_rack___ G2FRack;
	dwtcpr::scene Scene;
	dwtcpr::comparison_basic_observer___ ComparisonObserver;
	str::string ComparisonMessage;
	dwtcpr::drow__ SceneRoot = qNIL;
qRB
	ThreadAmountMax = sclrgstry::OGetUInt( Registry, registry::ThreadAmountMax, 0 );

	ExclusionsHandling = misc::GetExclusionsHandling_( Registry );

	GO.Init();

	exclusion::Get( Registry, GO );

	Excluder.Init( GO );
	Limitations.Init();
	exclusion::Fill( ExclusionsHandling, Registry, Excluder, Limitations );
	
	Content.Init();
	dwtdct::Explore( Path, ThreadAmountMax, Excluder, Limitations, GO, ExclusionsHandling, Content, ExplorationObserver );

	New.Init();
	dwtftr::Process( Content, New, ProcessingObserver );

	dwtftr::Sort( New, dwtbsc::st_Default );

	Movings.Init();
	dwtmov::Explore( Path, Content, GO, Movings );

	G2FRack.Init();
	Old.Init();
	dwtdct::Fill( Content, dwtdct::GetROG2F( Path, GO, G2FRack ), Old );

	dwtftr::Sort( Old, dwtbsc::st_Default );

	Scene.Init();
	ComparisonMessage.Init();
	sclmisc::GetBaseTranslation( ComparisonMessage_, ComparisonMessage );
	ComparisonObserver.Init( ComparisonMessage, COut, 750 );
	SceneRoot = dwtcpr::Compare( New, Old, Scene, ComparisonObserver );

	SceneRoot = dwtcpr::Clean( Scene, SceneRoot );

	misc::Dump( Movings, SceneRoot, Scene, OutputFilename);
qRR
qRT
qRE
}

static void Compare_(
	const str::string_ &Path,
	const str::string_ &OutputFilename )
{
qRH
	str::string
		Generator,
		ExplorationMessage,
		ProcessingMessage;
	dwtdct::basic_exploration_observer___ ExplorationObserver;
	dwtftr::basic_processing_observer___ ProcessingObserver;
qRB
	Generator.Init( MISC_NAME_MC " V" VERSION " (" );
	Generator.Append( cpe::GetDescription() );
	Generator.Append( ')' );

	ExplorationMessage.Init(); 
	sclmisc::GetBaseTranslation( ExplorationMessage_, ExplorationMessage );

	ProcessingMessage.Init(); 
	sclmisc::GetBaseTranslation( ProcessingMessage_, ProcessingMessage );

	ExplorationObserver.Init( OutputFilename.Size() == 0 ? 0 : Delay_, ExplorationMessage, cio::COut );
	ProcessingObserver.Init( OutputFilename.Size() == 0 ? 0 : Delay_,ProcessingMessage, cio::COut );

	Compare_( sclmisc::GetRegistry(), Path, Generator, OutputFilename, ExplorationObserver, ProcessingObserver );
qRR
qRT
qRE
}

static void Compare_( void )
{
qRH
	str::string Path, Output;
qRB
	Path.Init();
	sclmisc::MGetValue( registry::Path, Path );

	misc::NormalizeAndTestPath( Path );

	Output.Init();
	sclmisc::OGetValue( registry::Output, Output );

	Compare_( Path, Output );
qRR
qRT
qRE
}

static void Clean_( void )
{
qRH
	str::string Path;
	bso::uint__ ThreadAmountMax = 0;
	dwtdct::content Content;
	dwtxcl::excluder Excluder;
	dwtbsc::limitations__ Limitations;
	dwtbsc::ghosts_oddities GO;
	dwtdct::basic_exploration_observer___ ExplorationObserver;
	str::string ExplorationMessage, GhostsSettingMessage;
qRB
	Path.Init();
	sclmisc::MGetValue( registry::Path, Path );

	misc::NormalizeAndTestPath( Path );

	ThreadAmountMax = sclmisc::OGetUInt( registry::ThreadAmountMax, 0 );

	GO.Init();
	exclusion::Get( sclmisc::GetRegistry(), GO );

	Excluder.Init( GO );
	Limitations.Init();
	exclusion::Fill( sclmisc::GetRegistry(), Excluder, Limitations );

	ExplorationMessage.Init();
	sclmisc::GetBaseTranslation( ExplorationMessage_, ExplorationMessage );

	Content.Init();
	ExplorationObserver.Init( Delay_, ExplorationMessage, cio::COut );
	dwtdct::Explore( Path, ThreadAmountMax, Excluder, Limitations, GO, dwtbsc::ehKeepGhostLike, Content, ExplorationObserver );

	cio::COut << txf::nl;

	GhostsSettingMessage.Init();
	sclmisc::GetBaseTranslation( GhostsSettingMessage_, GhostsSettingMessage );

	ExplorationObserver.Init( Delay_, ExplorationMessage, cio::COut );
	dwtdct::DelGhosts( Path, Content, GO, ExplorationObserver );
qRR
qRT
qRE
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

int scltool::SCLTOOLMain(
	const str::string_ &Command,
	const scltool::oddities__ &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH
qRB
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( MISC_NAME_MC );
	C( Browse );
	C( Update );
	C( TestGhosts );
	C( Compare );
	C( Clean );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}

const char *sclmisc::SCLMISCTargetName = MISC_NAME_LC;

