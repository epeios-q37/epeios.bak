/*
	Copyright (C) 2021 Claude SIMON (http://q37.info/contact/).

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

#include "tskinf.h"
#include "tsktasks.h"
#include "tskxml.h"

#include "sclm.h"
#include "sclt.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "flw.h"

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
		COut << txf::pad << "Build: " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}

	void Export_(void)
	{
	qRH;
	  sTRow Row = qNIL;
	  sclm::rTextWFlowRack Rack;
	  qCBUFFERh Buffer;
	qRB;
    Row = sclm::OGetU16(registry::parameter::Index, 0);

    Tasks.Init();

    tskxml::Export(Tasks, Row, Rack.Init(sclm::OGetValue(registry::parameter::Output, Buffer)), NAME_MC " V" VERSION);
  qRR;
    Rack.HandleError();
	qRT;
	qRE;
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
	qRR;
	qRT;
	qRE;
	}

	class sBrowse
	: public cBrowser
	{
  private:
    void Indent_(sLevel Level)
    {
      while ( --Level )
        cio::COut << ' ';
    }
  protected:
	  virtual void TSKTasks(sTRow Row) override
    {
    }
    virtual void TSKTask(
      eKinship Kinship,
      sLevel Level,
      sTRow Row,
      const str::dString &Label,
      const str::dString &Description) override
      {
        Indent_(Level);

        cio::COut << *Row << ": " << Label << txf::nl;
      }
    virtual void TSKParent(sLevel Level) override
    {
    }
  } Browse;

	void Display_(void)
	{
	  sTRow Row = qNIL;

    Row = sclm::OGetU16(registry::parameter::Index, 0);

    Tasks.Init();

    if ( !Tasks.Exists(Row))
      qRGnr();

    Tasks.Browse(Row, Browse);
	}

	void Import_(void)
	{
	qRH;
    sclm::rRDriverRack Rack;
    flw::rDressedRFlow<> Flow;
    xtf::sRFlow XFlow;
	  qCBUFFERh Buffer;
	qRB;
    Flow.Init(Rack.Init(sclm::OGetValue(registry::parameter::Input, Buffer)));
    XFlow.Init(Flow, utf::f_Default);

    Tasks.Init();

    tskxml::Import(XFlow, 0, Tasks);
	qRR;
    Rack.HandleError();
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
	C( Export );
	C( Create );
	C( Display );
	C( Import );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR;
qRT;
qRE;
	return ExitValue;
}
