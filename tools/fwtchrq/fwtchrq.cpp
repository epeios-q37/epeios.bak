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

# define NAME_MC			"fwtchrq"
# define NAME_LC			"fwtchrq"
# define NAME_UC			"FWTCHRQ"
# define WEBSITE_URL		"http://q37.info"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	

E_CDEF( char *, ExplorationMessage_, "ExplorationMessage" );
E_CDEF( char *, ProcessingMessage_, "ProcessingMessage" );
E_CDEF( char *, UpdateMessage_, "UpdateMessage" );

E_CDEF( tol::delay__, Delay_, 700 );

static void PrintHeader_( void )
{
	COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
	COut << "Copyright (C) " COPYRIGHT << txf::nl;
	COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

static void Browse_(
	const str::string_ &Path,
	const str::string_ &OutputFileName )
{
qRH
	str::string
		Generator,
		ExplorationMessage,
		ProcessingMessage;
	fwtdct::basic_exploration_observer___ ExplorationObserver;
	fwtftr::basic_processing_observer___ ProcessingObserver;
qRB
	Generator.Init( NAME_MC " V" VERSION " (" );
	Generator.Append( cpe::GetDescription() );
	Generator.Append( ')' );

	ExplorationMessage.Init(); 
	sclmisc::GetBaseTranslation( ExplorationMessage_, ExplorationMessage );

	ProcessingMessage.Init(); 
	sclmisc::GetBaseTranslation( ProcessingMessage_, ProcessingMessage );

	ExplorationObserver.Init( ExplorationMessage, cio::COut, Delay_ );
	ProcessingObserver.Init( ProcessingMessage, cio::COut, Delay_ );

	browse::Browse( sclmisc::GetRegistry(), Path, Generator, OutputFileName, ExplorationObserver, ProcessingObserver );
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
	fwtdct::content Content;
	fwtxcl::excluder Excluder;
	fwtbsc::limitations__ Limitations;
	fwtbsc::ghosts_oddities GO;
	fwtdct::basic_exploration_observer___ ExplorationObserver;
	fwtdct::basic_ghosts_setting_observer___ GhostsSettingObserver;
	str::string ExplorationMessage, UpdateMessage;
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
	ExplorationObserver.Init( ExplorationMessage, cio::COut, Delay_ );
	fwtdct::Explore( Path, ThreadAmountMax, Excluder, Limitations, GO, fwtbsc::ehKeepGhostLike, Content, ExplorationObserver );

	cio::COut << txf::nl;

	UpdateMessage.Init();
	sclmisc::GetBaseTranslation( UpdateMessage_, UpdateMessage );

	GhostsSettingObserver.Init( UpdateMessage, cio::COut, Delay_ );
	fwtdct::SetGhosts( Path, Content, GO, GhostsSettingObserver );
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
#pragma message ("à enlever !")
	cio::COut << "----->" << sizeof(time_t) << txf::tab << sizeof( bso::s32__ ) <<  "<-----" << txf::nl << txf::commit;

	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( Browse );
	C( Update );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;

