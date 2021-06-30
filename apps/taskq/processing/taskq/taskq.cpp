/*
	Copyright (C) 2019 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'TaskQ' tool.

    'TaskQ' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'TaskQ' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'TaskQ'.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "taskq.h"

#include "registry.h"

#include "tsktasks.h"

#include "sclm.h"
#include "sclt.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using namespace tsktasks;

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( taskq, NAME_LC, NAME_MC );

tsktasks::rTasks Tasks;

const scli::sInfo &sclt::SCLTInfo( void )
{
	return taskq::Info;
}

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}

	namespace {
    void Dump_(
      tsktasks::rTasks &Tasks,
      sTRow Row)
    {
    qRH;
      xml::rWriter Writer;
    qRB;
      Writer.Init(cio::COut, xml::lIndent, xml::fEncoding());

      Tasks.Export(Row, Writer, NAME_MC " V" VERSION);
    qRR;
    qRT;
    qRE;
    }
	}

	void Export_(void)
	{
	  sTRow Row = qNIL;

    Row = sclm::OGetU16(registry::parameter::Index, 0);

    Tasks.Init();

    Dump_(Tasks, Row);
	}

	void Create_(void)
	{
	qRH;
	  str::wString Label;
	  sTRow Row = qNIL;
	qRB;
    Label.Init();
    sclm::MGetValue(registry::parameter::Label, Label);

    Row = sclm::OGetU16(registry::parameter::Index, 0);

    Tasks.Init();

    if ( !Tasks.Exists(Row))
      qRGnr();

    Tasks.Append(Label, Row);

    Tasks.Display(Row, cio::COut);
	qRR;
	qRT;
	qRE;
	}

	void Display_(void)
	{
	  sTRow Row = qNIL;

    Row = sclm::OGetU16(registry::parameter::Index, 0);

    Tasks.Init();

    if ( !Tasks.Exists(Row))
      qRGnr();

    Tasks.Display(Row, cio::COut);
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
	C( Create );
	C( Display );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR;
qRT;
qRE;
	return ExitValue;
}
