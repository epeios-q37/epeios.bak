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

#include "dwtftr.h"
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

namespace {
	void Dump_(
		dwtftr::drow__ Root,
		const dwtftr::file_tree_ &Tree,
		const fnm::name___ &Filename )
	{
	qRH
		sclmisc::text_oflow_rack___ Rack;
		xml::writer Writer;
	qRB
		Writer.Init( Rack.Init( Filename ), xml::oIndent, xml::e_Default );

		Writer.PushTag( MISC_NAME_MC );

		Writer.PushTag( "FileTree" );

		Tree.Dump( Root, Writer );

		Writer.PopTag();

		Writer.PopTag();
	qRR
		Rack.HandleError();
	qRT
	qRE
	}
}

void browse::Browse(
	const rgstry::multi_level_registry_ &Registry,
	const str::string_ &Path,
	const str::string_ &Generator,
	const fnm::name___ &OutputFilename,
	dwtdct::exploration_observer__ &ExplorationObserver,
	dwtftr::processing_observer___ &ProcessingObserver	)
{
qRH
	bso::uint__ ThreadAmountMax = 0;
	dwtftr::file_tree Tree;
	fwtmov::movings Movings;
	dwtftr::drow__ Root = qNIL;
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
	Root = dwtftr::Process( Content, Tree, ProcessingObserver );

	Dump_( Root, Tree, OutputFilename );

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

