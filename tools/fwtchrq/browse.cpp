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

#include "browse.h"
#include "exclusion.h"
#include "registry.h"
#include "store.h"
#include "misc.h"

#include "fwtftr.h"
#include "fwtmov.h"

using namespace browse;

dwtbsc::exclusion_handling__ GetExclusionHandling_( const sclrgstry::registry_ &Registry )
{
	dwtbsc::exclusion_handling__ ExclusionHandling = dwtbsc::eh_Undefined;
qRH
	str::string RawExclusionHandling;
qRB
	RawExclusionHandling.Init();
	
	sclrgstry::MGetValue( Registry, registry::ExclusionsHandling, RawExclusionHandling );

	if ( RawExclusionHandling == "Regular" )
		ExclusionHandling = dwtbsc::ehRegular;
	else if ( RawExclusionHandling == "Keep" ) 
		ExclusionHandling = dwtbsc::ehKeep;
	else if ( RawExclusionHandling == "Skip" ) 
		ExclusionHandling = dwtbsc::ehSkip;
	else
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( registry::ExclusionsHandling );
qRR
qRT
qRE
	return ExclusionHandling;
}

void browse::Browse(
	const rgstry::multi_level_registry_ &Registry,
	const str::string_ &Path,
	const str::string_ &Generator,
	const str::string_ &Output,
	dwtdct::exploration_observer__ &ExplorationObserver,
	fwtftr::processing_observer__ &ProcessingObserver	)
{
qRH
	bso::uint__ ThreadAmountMax = 0;
	fwtftr::file_tree Tree;
	fwtmov::movings Movings;
	fwtftr::drow__ Root = qNIL;
	dwtbsc::exclusion_handling__ ExclusionHandling = dwtbsc::eh_Undefined;
	dwtxcl::excluder Excluder;
	dwtbsc::limitations__ Limitations;
	dwtbsc::ghosts_oddities GO;
	dwtdct::content Content;
	store::store__ Store;
qRB
	ThreadAmountMax = sclrgstry::OGetUInt( Registry, registry::ThreadAmountMax, 0 );

	ExclusionHandling = GetExclusionHandling_( Registry );

	GO.Init();

	exclusion::Get( Registry, GO );

	Excluder.Init( GO );
	exclusion::Fill( Registry, Excluder );

	Limitations.Init();
	exclusion::Fill( Registry, Limitations );

	Content.Init();
	dwtdct::Explore( Path, ThreadAmountMax, Excluder, Limitations, GO, ExclusionHandling, Content, ExplorationObserver );

	Tree.Init();
	Root = fwtftr::Process( Content, Tree, ProcessingObserver );

	Movings.Init();
	fwtmov::Explore( Path, Content, GO, Movings );

	Store.InitAsTreeAndMovings( Root, Tree, Movings );

	store::Save( Store, Generator, Output );

//	Dump_( Generator, Row, Tree, Movings, tol::EpochTime( false ) - Start, ThreadAmountMax, Output );
qRR
qRT
qRE
}

