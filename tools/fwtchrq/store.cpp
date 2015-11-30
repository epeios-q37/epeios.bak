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


#include "store.h"

#include "sclmisc.h"

#include "cio.h"
#include "flf.h"

using namespace store;

namespace {
	E_CDEF(char *, RootTagName_, "FileTree" );
}

static stsfsm::automat
	VersionAutomat_,
	ContentAutomat_;

static void FillVersionAutomat_( void )
{
	VersionAutomat_.Init();
	stsfsm::Fill<version__>( VersionAutomat_, v_amount, GetLabel );
}

static void FillContentAutomat_( void )
{
	ContentAutomat_.Init();
	stsfsm::Fill<content__>( ContentAutomat_, c_amount, GetLabel );
}

static void FillAutomats_( void )
{
	FillVersionAutomat_();
	FillContentAutomat_();
}

version__ store::GetVersion( const str::string_ &Pattern )
{
	return stsfsm::GetId( Pattern, VersionAutomat_, v_Undefined, v_amount );
}

#define C( name )	case c##name : return #name; break

const char *store::GetLabel( content__ Content )
{
	switch ( Content ) {
	C( TreeAndMovings );
	C( Modifications );
	default:
		qRGnr();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

content__ store::GetContent( const str::string_ &Pattern )
{
	return stsfsm::GetId( Pattern, ContentAutomat_, c_Undefined, c_amount );
}

void store::SaveTreeAndMovings(
	dwtftr::drow__ Row,
	const dwtftr::file_tree_ &Tree,
	const fwtmov::movings_ &Movings,
	xml::writer_ &Writer )
{
	Writer.PushTag( GetLabel( cTreeAndMovings ) );
	Writer.PushTag( "Content" );

	Tree.Dump( Row, Writer, store::GetFWTFTRVersion( v_Current ) );

	Writer.PopTag();

	Writer.PushTag( "DirectoryModifications" );

	fwtmov::Dump( Movings, Writer, store::GetFWTMOVVersion( v_Current ) );

	Writer.PopTag();
	Writer.PopTag();
}

dwtftr::drow__ store::LoadTreeAndMovings(
	xml::parser___ &Parser,
	dwtftr::load_observer__ &Observer,
	dwtftr::version__ TreeVersion,
	dwtftr::file_tree_ &Tree,
	fwtmov::version__ MovingsVersion,
	fwtmov::movings_ &Movings )
{
	dwtftr::drow__ Root = qNIL;
qRH
	bso::bool__ Continue = true;
qRB
	if ( Parser.TagName() != GetLabel( cTreeAndMovings ) )
		qRVct();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTag ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == "Content" ) {
				Root = dwtftr::Load( Parser, TreeVersion, Tree, Observer );
			} else if ( Parser.TagName() == "DirectoryModifications" ) {
				fwtmov::Load( Parser, MovingsVersion, Movings );
			} else
				qRGnr();
			break;
		case xml::t_Processed:
			Continue = false;
			break;
		case xml::t_Error:
			qRGnr();
			break;
		}
	}
qRR
qRT
qRE
	return Root;
}

void store::SaveModifications(
	dwtcpr::drow__ Root,
	const dwtcpr::scene_ &Scene,
	const fwtmov::movings_ &Movings,
	xml::writer_ &Writer )
{
	Writer.PushTag( GetLabel( cModifications ) );
	Writer.PushTag( "Directories" );

	fwtmov::Dump( Movings, Writer, GetFWTMOVVersion( v_Current ) );

	Writer.PopTag();

	Writer.PushTag( "Files" );

	Dump( Root, Scene, Writer, GetFWTCPRVersion( v_Current ) );

	Writer.PopTag();
	Writer.PopTag();
}

dwtcpr::drow__ store::LoadModifications(
	xml::parser___ &Parser,
	dwtcpr::load_observer__ &Observer,
	dwtcpr::version__ SceneVersion,
	dwtcpr::scene_ &Scene,
	fwtmov::version__ MovingsVersion,
	fwtmov::movings_ &Movings )
{
	dwtcpr::drow__ Root = qNIL;
qRH
	bso::bool__ Continue = true;
qRB
	if ( Parser.TagName() != GetLabel( cModifications ) )
		qRVct();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTag ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == "Files" ) {
				Root = dwtcpr::Load( Parser, SceneVersion, Scene, Observer );
			} else if ( Parser.TagName() == "Directories" ) {
				fwtmov::Load( Parser, MovingsVersion, Movings );
			} else
				qRGnr();
			break;
		case xml::t_Processed:
			Continue = false;
			break;
		case xml::t_Error:
			qRGnr();
			break;
		}
	}
qRR
qRT
qRE
	return Root;
}


void store::Save(
	const store__ &Store,
	const str::string_ &Generator,
	xml::writer_ &Writer )
{
	tol::buffer__ Buffer;

	Writer.PushTag( RootTagName_ );

	Writer.PutAttribute( "FormatVersion", GetLabel( v_Current ) );

	Writer.PutAttribute( "Generator", Generator );

	Writer.PutAttribute( "TimeStamp", tol::DateAndTime( Buffer ) );

	switch ( Store.Content() ) {
	case cTreeAndMovings:
		Writer.PutAttribute( "ContentType", GetLabel( cTreeAndMovings ) );
		SaveTreeAndMovings( Store.TreeRoot, *Store.Tree, *Store.Movings, Writer );
		break;
	case cModifications:
		Writer.PutAttribute( "ContentType", GetLabel( cModifications ) );
		SaveModifications( Store.SceneRoot, *Store.Scene, *Store.Movings, Writer );
		break;
	default:
		qRGnr();
		break;
	}

	Writer.PopTag();
}

void store::Save(
	const store__ &Store,
	const str::string_ &Generator,
	txf::text_oflow__ &Flow )
{
qRH
	xml::writer Writer;
qRB
	Writer.Init( Flow, xml::oIndent, xml::e_Default );

	Save( Store, Generator, Writer );
qRR
qRT
qRE
}

void store::Save(
	const store__ &Store,
	const str::string_ &Generator,
	const str::string_ &FileName )
{
qRH
	sclmisc::text_oflow_rack___ Rack;
qRB
	Save( Store, Generator, Rack.Init( FileName ) );
qRR
	Rack.HandleError();
qRT
qRE
}

bso::bool__ store::Load(
	xml::parser___ &Parser,
	content__ ExpectedContent,
	const observers__ &Observers,
	store___ &Store )
{
	bso::bool__ Success = false;
qRH
	version__ Version = v_Undefined;
	content__ Content = c_Undefined;
	bso::bool__ Continue = true;
qRB
	if ( Parser.Parse( xml::tfStartTag ) != xml::tStartTag )
		qRVct();

	if ( Parser.TagName() != RootTagName_ )
		qRVct();

	do { 
		if ( Parser.Parse( xml::tfAttribute | xml::tfStartTagClosed ) != xml::tAttribute )
			qRGnr();
	} while ( Parser.AttributeName() != "FormatVersion" );

	Version = GetVersion( Parser.Value() );

	do { 
		if ( Parser.Parse( xml::tfAttribute | xml::tfStartTagClosed ) != xml::tAttribute )
			qRGnr();
	} while ( Parser.AttributeName() != "ContentType" );

	Content = GetContent( Parser.Value() );

	while ( Parser.Parse( xml::tfStartTagClosed ) != xml::tStartTagClosed )
		qRVct();

	Store.Init( Content );

	if ( Parser.Parse( xml::tfStartTag ) != xml::tStartTag )
		qRVct();

	if ( ExpectedContent != c_Undefined )
		if ( Content != ExpectedContent )
			qRReturn;


	switch ( Content ) {
	case cTreeAndMovings:
		Store.TreeRoot = LoadTreeAndMovings( Parser, Observers.TreeObserver(), GetFWTFTRVersion( Version ), Store.Tree, GetFWTMOVVersion( Version ), Store.Movings );
		break;
	case cModifications:
		Store.SceneRoot = LoadModifications( Parser, Observers.SceneObserver(), GetFWTCPRVersion( Version ), Store.Scene, GetFWTMOVVersion( Version ), Store.Movings );
		break;
	default:
		qRGnr();
		break;
	}

	Success = true;
qRR
qRT
qRE
	return Success;
}

bso::bool__ store::Load(
	flw::iflow__ &Flow,
	content__ ExpectedContent,
	const observers__ &Observers,
	store___ &Store )
{
	bso::bool__ Success = false;
qRH
	xtf::extended_text_iflow__ XFlow;
	xml::parser___ Parser;
qRB
	XFlow.Init( Flow, utf::f_Default );

	Parser.Init( XFlow, xml::eh_Default );

	Success = Load( Parser, ExpectedContent, Observers, Store );

	Flow.Dismiss();
qRR
qRT
qRE
	return Success;
}

bso::bool__ store::Load(
	const fnm::name___ &FileName,
	content__ ExpectedContent,
	const observers__ &Observers,
	store___ &Store )
{
	bso::bool__ Success = false;
qRH
	flf::file_iflow___ Flow;
qRB
	if ( Flow.Init( FileName, err::hUserDefined ) != tol::rSuccess )
		sclmisc::ReportFileOpeningErrorAndAbort( FileName );

	Success = Load( Flow, ExpectedContent, Observers, Store );
qRR
qRT
qRE
	return Success;
}

Q37_GCTOR( store )
{
		FillAutomats_();
}




