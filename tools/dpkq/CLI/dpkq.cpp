/*
	Copyright (C) 2010-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of dpkq.

    dpkq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    dpkq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with dpkq.  If not, see <http://www.gnu.org/licenses/>
*/

#include "dpkq.h"

#include "context.h"
#include "data.h"
#include "registry.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( dpkq, NAME_LC, NAME_MC );

static void PrintHeader_( void )
{
	COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
	COut << "Copyright (C) " COPYRIGHT << txf::nl;
	COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
}


using data::sId;

using data::dData;
using data::wData;

void LaunchViewer_(
	sId RecordId,
	sId TableId,
	dpkctx::sBRow LastBox,
	dpkctx::sBRow ContainingBox,
	const str::string_ &RecordLabel,
	const str::string_ &TableLabel,
	const str::string &DataFilename,
	const str::string &OutputFilename,
	const str::string_ &XSLFilename )
{
qRH
	str::string Viewer;
	TOL_CBUFFER___ SBuffer;
	bso::integer_buffer__ IBuffer;
	tagsbs::tvalues TaggedValues;
qRB
	Viewer.Init();
	sclmisc::OGetValue( registry::Viewer, Viewer );

	if ( ( Viewer.Amount() != 0 ) && ( OutputFilename.Amount() != 0 ) ) {
		TaggedValues.Init();
		TaggedValues.Append( "RI", bso::Convert( RecordId, IBuffer ) );
		TaggedValues.Append( "RL", RecordLabel );
		TaggedValues.Append( "TI", bso::Convert( TableId, IBuffer ) );
		TaggedValues.Append( "TL", TableLabel);
		TaggedValues.Append( "LB", bso::Convert( LastBox == qNIL ? 0 : *LastBox + 1, IBuffer ) );
		TaggedValues.Append( "CB", bso::Convert( ContainingBox == qNIL ? 0 : *ContainingBox + 1, IBuffer ) );
		TaggedValues.Append( "Data", DataFilename );
		TaggedValues.Append( "XSL", XSLFilename );
		TaggedValues.Append( "Output",OutputFilename );
		tagsbs::SubstituteLongTags( Viewer, TaggedValues, '$' );
		tol::System( Viewer.Convert( SBuffer ) );
	}
qRR
qRT
qRE
}

void Process_( void )
{
qRH
	str::wString Namespace;
	str::string DataFilename;
	wData Data;
	dpkctx::context Context;
	str::string OutputFilename;
	str::string XSLFilename;
	bso::uint__ SessionMaxDuration = 0;
	str::string Label, TableLabel;
	sId Id = 0;
	dpkctx::sBRow BoxRow = qNIL;
qRB
	data::Initialize();

	Id = sclmisc::OGetUInt( registry::Id, data::All );

	switch ( Id ) {
	case data::Undefined:
		Id = 0;
		break;
	case 0:
		Id = data::All;
		break;
	default:
		break;
	}

	DataFilename.Init();
	if ( !sclmisc::BGetValue( registry::Data, DataFilename ) )
		sclmisc::ReportAndAbort( "DataFileNotSpecifiedError" );

	OutputFilename.Init();
	if ( !sclmisc::BGetValue( registry::Output, OutputFilename ) )
		sclmisc::ReportAndAbort( "OutputFileNotSpecifiedError" );

	XSLFilename.Init();
	sclmisc::OGetValue( registry::XSL, XSLFilename );

	Context.Init();
	context::Retrieve( Context);
	Context.AdjustBoxesAmount( sclmisc::OGetS8( registry::BoxesAmount, 0 ) );

	Data.Init();
	data::Retrieve( DataFilename, Data );

	SessionMaxDuration = sclmisc::OGetUInt( registry::SessionMaxDuration, 0 );

	Label.Init();
	TableLabel.Init();
	Id = data::Display( Id, Data, XSLFilename, SessionMaxDuration, Label, TableLabel, BoxRow, Context, OutputFilename );

	context::Dump( Context );

	LaunchViewer_( Id, *Data.Last() + 1, Context.Boxes.S_.Last, BoxRow, Label, TableLabel, DataFilename, OutputFilename, XSLFilename );
qRR
qRT
qRE
}

void Demote_( void )
{
qRH
	dpkctx::context Context;
	sId RecordId = data::Undefined;
qRB
	Context.Init();
	context::Retrieve( Context );

	RecordId = sclmisc::OGetUInt( registry::Id, 0 );

	Context.Demote( RecordId == 0 ? qNIL : RecordId - 1 );

	context::Dump( Context );
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
		epsmsc::PrintLicense( NAME_MC );
	C( Process );
	C( Demote );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}

const scli::sInfo &scltool::SCLTOOLInfo( void )
{
	return dpkq::Info;
}
