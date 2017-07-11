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
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

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
		TaggedValues.Append("RI", bso::Convert( RecordId, IBuffer ) );
		TaggedValues.Append("RL", RecordLabel );
		TaggedValues.Append("TI", bso::Convert( TableId, IBuffer ) );
		TaggedValues.Append("TL", TableLabel);
		TaggedValues.Append("Data",DataFilename );
		TaggedValues.Append("XSL",XSLFilename );
		TaggedValues.Append("Output",OutputFilename );
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
	str::string ContextFilename;
	bso::uint__ SessionMaxDuration = 0;
	bso::bool__ Error = false;
	str::string Label, TableLabel;
	sId Id = 0;
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

	ContextFilename.Init();
	if ( !sclmisc::BGetValue( registry::Context, ContextFilename ) )
		sclmisc::ReportAndAbort( "ContextFileNotSpecifiedError" );

	Context.Init();
	context::Retrieve( ContextFilename, Context );

	Data.Init();
	data::Retrieve( DataFilename, Data );

	SessionMaxDuration = sclmisc::OGetUInt( registry::SessionMaxDuration, 0 );

	Label.Init();
	TableLabel.Init();
	Id = data::Display( Id, Data, XSLFilename, SessionMaxDuration, Label, TableLabel, Context, OutputFilename );

	context::Dump( Context, ContextFilename);

	LaunchViewer_( Id, *Data.Last() + 1, Label, TableLabel, DataFilename, OutputFilename, XSLFilename );
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
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;


