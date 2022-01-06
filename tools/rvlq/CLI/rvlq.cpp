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

//Type of substitution
qENUM(Type) {
  tText,
  tFile,
  tCommand,
  t_amount,
  t_Undefined
};

typedef bch::qBUNCHdl(eType) dTypes;
qW(Types);

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

		if ( !tagsbs::SubstituteLongTags(Input, Tags, Values, Output, sclm::MGetChar(registry::parameter::TagDelimiter)) )
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

		Table.Purge(sclm::MGetChar(registry::parameter::CommentMarker));
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

	void FillWithCommandResult_(
		const str::dString &Command,
		str::dString &Value)
	{
	qRH;
		flx::rExecRDriver RDriver;
		flx::rStringWDriver WDriver;
	qRB;
		if ( Command.Amount() == 0 )
			qRGnr();

		RDriver.Init(Command);
		WDriver.Init(Value);

		fdr::Copy(RDriver, WDriver);
	qRR;
	qRT;
	qRE;
	}

	eType GetTagDefinition_(
		const str::dString &RawValue,
		bso::sChar TextMarker,
		bso::sChar FileMarker,
		bso::sChar CommandMarker,
		str::dString &Definition)
	{
	  eType Type = t_Undefined;
	qRH;
		str::wString Buffer;
	qRB;
		if ( RawValue.Amount() != 0 ) {
			char Marker = RawValue(RawValue.First());

			if ( ( Marker == TextMarker ) || ( Marker == FileMarker ) || ( Marker == CommandMarker ) ) {
				Buffer.Init();

				if ( RawValue.Amount() > 1 )
					Buffer.Append(RawValue, RawValue.Next(RawValue.First() ), RawValue.Last());

				if ( Marker == TextMarker )
					Type = tText;
				else if ( Marker == FileMarker )
					Type = tFile;
				else if ( Marker == CommandMarker )
					Type = tCommand;
				else
					qRGnr();

        Definition.Append(Buffer);
			} else {
			  Type = tText;
				Definition.Append(RawValue);
			}
		}
	qRR;
	qRT;
	qRE;
    return Type;
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
		dTypes &Types,
		str::dStrings &Definitions)
	{
	qRH;
		sdr::sRow Row = qNIL;
		str::wString Tag,Definition;
	qRB;
		if( Line.Amount() != 2 )
			qRGnr();

		Row = Line.First();

		Tag.Init();
		Tag = Line(Row);

		Row = Line.Next(Row);

		if ( !TestTag_( Tag ) )
			qRGnr();

		Definition.Init();
		Types.Append( GetTagDefinition_(Line(Row), sclm::MGetChar(registry::parameter::TextMarker), sclm::MGetChar(registry::parameter::FileMarker),  sclm::MGetChar(registry::parameter::CommandMarker), Definition));

		if ( Tags.Append(Tag) != Definitions.Append(Definition) )
			qRGnr();
	qRR;
	qRT;
	qRE;
	}

	void GetTagsAndDefinitions_(
		txmtbl::dTable &Table,
		str::dStrings &Tags,
		dTypes &Types,
		str::dStrings &Definitions)
	{
		sdr::sRow Row = qNIL;

		Row = Table.First();

		while ( Row != qNIL ) {
			Add_(Table(Row), Tags, Types, Definitions);

			Row = Table.Next(Row);
		}
	}

	void GetTagsAndDefinitions_(
		fdr::rRDriver &TagsDriver,
		str::dStrings &Tags,
		dTypes &Types,
		str::dStrings &Definitions)
	{
	qRH;
		txmtbl::wTable Table;
	qRB;
		Table.Init();

		GetTagsTable_(TagsDriver, Table);

		GetTagsAndDefinitions_(Table, Tags, Types, Definitions);
	qRR;
	qRT;
	qRE;
	}

	void GetTagsAndDefinitions_(
		const str::dString &TagsFilename,
		str::dStrings &Tags,
		dTypes &Types,
		str::dStrings &Values)
	{
	qRH;
		sclm::rRDriverRack Input;
	qRB;
		GetTagsAndDefinitions_(Input.Init(TagsFilename), Tags, Types, Values);
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
		str::wStrings Tags, Definitions;
		wTypes Types;
	qRB
		tol::Init(Tags, Types, Definitions);

		GetTagsAndDefinitions_(TagsFilename, Tags, Types, Definitions);

		if ( Tags.Amount() != Types.Amount() )
      qRGnr();

    if ( Tags.Amount() != Definitions.Amount() )
      qRGnr();

		Reveal_(Tags, Types, Definitions, InputFilename, OutputFilename);
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
