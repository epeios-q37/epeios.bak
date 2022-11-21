/*
	Copyright (C) 2021 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'tasq' tool.

    'tasq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'tasq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'tasq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "tasq.h"

#include "registry.h"

#include "tasqtasks.h"
#include "tasqxml.h"

#include "sclm.h"
#include "sclt.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( tasq, NAME_LC, NAME_MC );

const scli::sInfo &sclt::SCLTInfo( void )
{
	return tasq::Info;
}

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build: " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}

	namespace _ {
	  void GetFilenames(
      str::dString &DBFilename,
      str::dString &XMLFilename)
      {
        sclm::MGetValue(registry::parameter::DBFilename, DBFilename);
        sclm::MGetValue(registry::parameter::XMLFilename, XMLFilename);
      }
	}

	void Export_( void )
	{
	qRH;
    str::wString DBFilename, XMLFilename;
    sclm::rTWFlowRack Rack;
    xml::rWriter Writer;
    tasqtasks::hGuard Guard;
    bso::sBool Initialized = false;
	qRB;
    tol::Init(DBFilename, XMLFilename);
    _::GetFilenames(DBFilename, XMLFilename);

    tasqtasks::Initialize(DBFilename);

    Initialized = true;


    Writer.Init(Rack.Init(XMLFilename), xml::lIndent);

    Writer.PushTag("TasQ");

    tasqxml::Write(tasqtasks::Get(Guard), tasqxml::ffExport, Writer);

    Writer.PopTag();
	qRR;
    Rack.HandleError();
	qRT;
    if ( Initialized )
      tasqtasks::Immortalize();
	qRE;
	}

	void Import_( void )
	{
	qRH;
    str::wString DBFilename, XMLFilename;
    sclm::rXRFlowRack Rack;
    xml::rParser Parser;
    tasqtasks::hGuard Guard;
    bso::sBool Initialized = false;
	qRB;
    tol::Init(DBFilename, XMLFilename);
    _::GetFilenames(DBFilename, XMLFilename);

    tasqtasks::Initialize(DBFilename);

    Initialized = true;

    Parser.Init(Rack.Init(XMLFilename), xml::eh_Default);

    if ( ( Parser.Parse(xml::tfStartTagClosed) != xml::tStartTagClosed ) || ( Parser.TagName() != "TasQ") )
      qRFwk();

    tasqxml::Parse(Parser, tasqtasks::Get(Guard));
	qRR;
    Rack.HandleError();
	qRT;
    if ( Initialized )
      tasqtasks::Immortalize();
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
	C( Export );
  C( Import );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR;
qRT;
qRE;
	return ExitValue;
}
