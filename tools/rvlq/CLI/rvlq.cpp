/*
	Copyright (C) 2000 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'RVLq' tool.

    'RVLq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'RVLq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'RVLq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "rvlq.h"

#include "registry.h"

#include "sclm.h"
#include "sclt.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "tagsbs.h"
#include "txmtbl.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( rvlq, NAME_LC, NAME_MC );

const scli::sInfo &sclt::SCLTInfo( void )
{
	return rvlq::Info;
}

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}

	void Reveal_(
		const str::dStrings &Tags,
		const str::dStrings &Values,
		fdr::rRDriver &InputDriver,
		fdr::rWDriver &OutputDriver)
	{
	qRH;
		flw::rDressedRFlow<> Input;
		flw::rDressedWFlow<> Output;
	qRB;
		Input.Init(InputDriver);
		Output.Init(OutputDriver);

		if ( !tagsbs::SubstituteLongTags(Input, Tags, Values, Output, '@') )
			qRGnr();
	qRR;
	qRT;
	qRE;
	}

	void Reveal_(
		const str::dStrings &Tags,
		const str::dStrings &Values,
		const str::dString &InputFilename,
		const str::dString &OutputFilename)
	{
	qRH;
		sclm::rRDriverRack Input;
		sclm::rWDriverRack Output;
	qRB;
		Reveal_(Tags, Values, Input.Init(InputFilename), Output.Init(OutputFilename) );
	qRR;
		Input.HandleError();
		Output.HandleError();
	qRT;
	qRE;
	}

	void GetTagsTable_(
		fdr::rRDriver &TagsDriver,
		txmtbl::dTable &Table )
	{
	qRH;
		flw::rDressedRFlow<> Flow;
		xtf::sRFlow TFlow;
	qRB;
		Flow.Init(TagsDriver);
		TFlow.Init(Flow,utf::f_Default);

		txmtbl::GetTable(TFlow, Table);

		Table.Purge('#');
	qRR;
	qRT;
	qRE;
	}

	void FillWithFileContent_(
		const str::dString &Filename,
		str::dString &Value)
	{
	qRH;
		flf::rRDriver FDriver;
		flx::rStringWDriver SDriver;
	qRB;
		if ( Filename.Amount() == 0 )
			qRGnr();

		FDriver.Init(Filename);
		SDriver.Init(Value);

		fdr::Copy(FDriver, SDriver);
	qRR;
	qRT;
	qRE;
	}

	void GetValue_(
		const str::dString &RawValue,
		bso::sChar StringDelimiter,
		bso::sChar FileDelimiter,
		str::dString &Value)
	{
	qRH;
		str::wString Buffer;
	qRB;
		if ( RawValue.Amount() != 0 ) {
			char Marker = RawValue(RawValue.First());

			if ( ( Marker == StringDelimiter ) || ( Marker == FileDelimiter ) ) {
				Buffer.Init();

				if ( RawValue.Amount() > 1 )
					Buffer.Append(RawValue, RawValue.Next(RawValue.First() ), RawValue.Last());

				if ( Marker == StringDelimiter )
					Value = Buffer;
				else if ( Marker == FileDelimiter )
					FillWithFileContent_(Buffer, Value);
				else
					qRGnr();
			} else
				Value.Append(RawValue);
		}
	qRR;
	qRT;
	qRE;
	}

	bso::bool__ TestTag_( const str::dString &Tag )
	{
		sdr::sRow Row = Tag.First();
		bso::sChar C;

		if ( Tag.Amount() != 0 ) {
			while( Row != qNIL ) {
				C = Tag( Row );
				if ( !( isalnum( C ) || ( C == '_' ) ) )
					return false;

				Row = Tag.Next( Row );
			}

			return true;
		}
		else
			return false;
	}


	void Add_(
		const txmtbl::dLine &Line,
		str::dStrings &Tags,
		str::dStrings &Values)
	{
	qRH;
		sdr::sRow Row = qNIL;
		str::wString Tag, Value;
	qRB;
		cio::COut	<< Line << txf::nl << txf::commit;
		if( Line.Amount() != 2 )
			qRGnr();

		Row = Line.First();

		Tag.Init();
		Tag = Line(Row);

		Row = Line.Next(Row);

		if ( !TestTag_( Tag ) )
			qRGnr();

		Value.Init();
		GetValue_(Line(Row), '&', '%', Value);

		if ( Tags.Append(Tag) != Values.Append(Value) )
			qRGnr();
	qRR;
	qRT;
	qRE;
	}

	void GetTagsAndValues_(
		txmtbl::dTable &Table,
		str::dStrings &Tags,
		str::dStrings &Values)
	{
		sdr::sRow Row = qNIL;

		Row = Table.First();

		while ( Row != qNIL ) {
			Add_(Table(Row), Tags, Values);

			Row = Table.Next(Row);
		}
	}

	void GetTagsAndValues_(
		fdr::rRDriver &TagsDriver,
		str::dStrings &Tags,
		str::dStrings &Values)
	{
	qRH;
		txmtbl::wTable Table;
	qRB;
		Table.Init();

		GetTagsTable_(TagsDriver, Table);

		cio::COut << Table << txf::nl << txf::nl << txf::commit;

		GetTagsAndValues_(Table, Tags, Values);
	qRR;
	qRT;
	qRE;
	}
	void GetTagsAndValues_(
		const str::dString &TagsFilename,
		str::dStrings &Tags,
		str::dStrings &Values)
	{
	qRH;
		sclm::rRDriverRack Input;
	qRB;
		GetTagsAndValues_(Input.Init(TagsFilename), Tags, Values);
	qRR;
		Input.HandleError();
	qRT;
	qRE;
	}

	void Reveal_(
		const str::dString &TagsFilename,
		const str::dString &InputFilename,
		const str::dString &OutputFilename)
	{
	qRH
		str::wStrings Tags, Values;
	qRB
		tol::Init(Tags, Values);

		GetTagsAndValues_(TagsFilename, Tags, Values);

		Reveal_(Tags, Values, InputFilename, OutputFilename);
	qRR
	qRT
	qRE
	}

	void Reveal_( void )
	{
	qRH;
		str::wString Tags, Input, Output;
	qRB;
		tol::Init(Tags, Input, Output);

		sclm::MGetValue(registry::parameter::Tags, Tags);
		sclm::OGetValue(registry::parameter::Input, Input);
		sclm::OGetValue(registry::parameter::Output, Output);

		Reveal_(Tags, Input, Output);

	qRR;
	qRT;
	qRE;
	}
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

int sclt::SCLTMain(
	const str::dString &Command,
	const sclt::fOddities &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH;
qRB;
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( Reveal );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR;
qRT;
qRE;
	return ExitValue;
}
