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
#include "browse.h"
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

E_CDEF( tol::delay__, Delay_, 700 );

static void PrintHeader_( void )
{
	COut << MISC_NAME_MC " V" VERSION << " (" MISC_WEBSITE_URL ")" << txf::nl;
	COut << "Copyright (C) " MISC_COPYRIGHT << txf::nl;
	COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

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

	browse::Browse( sclmisc::GetRegistry(), Path, Generator, OutputFilename, ExplorationObserver, ProcessingObserver );
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

static void CompareDirs_(
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

	browse::Browse( sclmisc::GetRegistry(), Path, Generator, OutputFilename, ExplorationObserver, ProcessingObserver );
qRR
qRT
qRE
}

static void CompareDirs_( void )
{
qRH
	str::string Path, Output;
qRB
	Path.Init();
	sclmisc::MGetValue( registry::Path, Path );

	misc::NormalizeAndTestPath( Path );

	Output.Init();
	sclmisc::OGetValue( registry::Output, Output );

	CompareDirs_( Path, Output );
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
	dwtdct::basic_ghosts_setting_observer___ GhostsSettingObserver;
	str::string ExplorationMessage, UpdateMessage;
	dwtftr::file_tree_rack___ FileTreeRack;
	dwtftr::basic_processing_observer___ ProcessingObserver;
	str::string ProcessingMessage;
qRB
	ThreadAmountMax = sclmisc::OGetUInt( registry::ThreadAmountMax, 0 );

	GO.Init();
	exclusion::Get( sclmisc::GetRegistry(), GO );

	Excluder.Init( GO );
	exclusion::Fill( sclmisc::GetRegistry(), Excluder );

	Limitations.Init();
	exclusion::Fill( sclmisc::GetRegistry(), Limitations );

	ExplorationMessage.Init();
	sclmisc::GetBaseTranslation( ExplorationMessage_, ExplorationMessage );

	Content.Init();
	ExplorationObserver.Init( Delay_, ExplorationMessage, cio::COut );
	dwtdct::Explore( Path, ThreadAmountMax, Excluder, Limitations, GO, dwtbsc::ehKeepGhostLike, Content, ExplorationObserver );

	cio::COut << txf::nl;

	UpdateMessage.Init();
	sclmisc::GetBaseTranslation( UpdateMessage_, UpdateMessage );

	GhostsSettingObserver.Init( UpdateMessage, cio::COut, Delay_ );
	dwtdct::SetGhosts( Path, Content, GO, GhostsSettingObserver );

	FileTreeRack.Init();

	dwtftr::file_tree_ &FileTree = dwtftr::GetRWFileTree( Path, GO, FileTreeRack );

	FileTree.Init();

	ProcessingMessage.Init(); 
	sclmisc::GetBaseTranslation( ProcessingMessage_, ProcessingMessage );
	ProcessingObserver.Init( 750, ProcessingMessage, cio::COut );
	dwtftr::Process( Content, FileTree, ProcessingObserver );
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

int scltool::SCLTOOLMain(
	const str::string_ &Command,
	const scltool::oddities__ &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH
qRB
#pragma message ( __LOC__ "à enlever !")
//	cio::COut << "----->" << sizeof(time_t) << txf::tab << sizeof( bso::s32__ ) <<  "<-----" << txf::nl << txf::commit;

	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( MISC_NAME_MC );
	C( Browse );
	C( CompareDirs);
	C( Update );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}

const char *sclmisc::SCLMISCTargetName = MISC_NAME_LC;

