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

#include "exclusion.h"

#include "registry.h"

#include "sclmisc.h"
#include "scllocale.h"

#include "txmtbl.h"

using namespace exclusion;

static void GetExclusionsTable_(
	const str::string_ &FileName,
	txmtbl::table_ &Table )
{
qRH
	flf::file_iflow___ Flow;
	xtf::extended_text_iflow__ XFlow;
qRB
	if ( Flow.Init( FileName, err::hUserDefined ) != tol::rSuccess )
		sclmisc::ReportFileOpeningErrorAndAbort( FileName );

	XFlow.Init( Flow, utf::f_Default );

	txmtbl::GetTable( XFlow, Table );
qRR
	qRT
	qRE
}


static void GetExclusionsTable_(
	const registry_ &Registry,
	txmtbl::table_ &Table )
{
qRH
	str::string FileName;
qRB
	FileName.Init();

	if ( sclrgstry::OGetValue(Registry, registry::ExclusionsListFileName, FileName) ) {
		GetExclusionsTable_( FileName, Table );
		Table.Purge( '#' );
	}
qRR
qRT
qRE
}

static void Fill_(
	dwtxcl::excluder_ &Excluder,
	const txmtbl::table_ &Table )
{
qRH
	txmtbl::cell Exclusion;
	sdr::row__ Row = qNIL;
	lcl::meaning Meaning;
	bso::integer_buffer__ Buffer;
qRB
	Row = Table.First();

	while ( Row != qNIL ) {
		Exclusion.Init();

		if ( !Table.GetUniqueCell( Row, Exclusion ) ) {
			Meaning.Init();
			Meaning.SetValue( "ErrorInExclusionListAtLine" );
			Meaning.AddTag( bso::Convert( Table.Location( Row ), Buffer ) );
			sclmisc::ReportAndAbort( Meaning );
		}

		Excluder.Add( Exclusion );

		Row = Table.Next( Row );
	}
qRR
qRT
qRE
}

void exclusion::Get(
	const registry_ &Registry,
	dwtbsc::ghosts_oddities_ &GO )
{
	sclrgstry::MGetValue( Registry, registry::GhostsPrefix, GO.Prefix );
	sclrgstry::MGetValue( Registry, registry::GhostsSuffix, GO.Suffix );

	if ( GO.Suffix.Amount() == 0 )
		sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( registry::GhostsSuffix );
}


void exclusion::Fill(
	dwtbsc::exclusions_handling__ ExclusionsHandling,
	const registry_ &Registry,
	dwtxcl::excluder_ &Excluder )
{
qRH
	txmtbl::table Table;
qRB
	if ( ExclusionsHandling != dwtbsc::ehNone )	{
		Table.Init();
		GetExclusionsTable_( Registry, Table );

		Fill_( Excluder, Table );
	}
qRR
qRT
qRE
}

void exclusion::Fill(
	dwtbsc::exclusions_handling__ ExclusionsHandling,
	const registry_ &Registry,
	dwtbsc::limitations__ &Limitations )
{
	if ( ExclusionsHandling != dwtbsc::ehNone )	{
		Limitations.FileSizeMax = sclrgstry::MGetUInt( Registry, registry::ExclusionsFileSizeMax );
		Limitations.NameLengthMax = sclrgstry::MGetUInt( Registry, registry::ExclusionsNameLengthMax );
	}
}

