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

#include "misc.h"

#include "registry.h"

#include "cio.h"
#include "flf.h"

using namespace misc;

dwtbsc::exclusion_handling__ misc::GetExclusionHandling( const sclrgstry::registry_ &Registry )
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


void misc::Append(
	const char *Tag,
	const str::string_ &Value,
	str::strings_ &Tags,
	str::strings_ &Values )
{
	if ( Tags.Append( str::string( Tag ) ) != Values.Append( Value ) )
		qRGnr();
}

void misc::Append(
	const char *Tag,
	const rgstry::entry___ &Entry,
	const sclrgstry::registry_ &Registry,
	str::strings_ &Tags,
	str::strings_ &Values )
{
qRH
	str::string Value;
qRB
	Value.Init();
	sclrgstry::MGetValue( Registry, Entry, Value );

	Append( Tag, Value, Tags, Values );
qRR
qRT
qRE
}

void misc::Dump(
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

void misc::Dump(
	const dwtmov::movings_ &Movings,
	dwtcpr::drow__ SceneRoot,
	const dwtcpr::scene_ &Scene,
	const fnm::name___ &Filename )
{
qRH
	sclmisc::text_oflow_rack___ Rack;
	xml::writer Writer;
qRB
	Writer.Init( Rack.Init( Filename ), xml::oIndent, xml::e_Default );

	Writer.PushTag( MISC_NAME_MC );

	Writer.PushTag( "DirMovings" );

	dwtmov::Dump( Movings, Writer );

	Writer.PopTag();

	Writer.PushTag( "Differences" );

	dwtcpr::Dump( SceneRoot, Scene, Writer );

	Writer.PopTag();

	Writer.PopTag();
qRR
	Rack.HandleError();
qRT
qRE
}


