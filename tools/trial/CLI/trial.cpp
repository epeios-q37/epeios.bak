/*
	Copyright (C) 2019 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'Trial' tool.

    'Trial' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'Trial' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'Trial'.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "trial.h"

#include "registry.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "csdcmn.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( trial, NAME_LC, NAME_MC );

const scli::sInfo &scltool::SCLTOOLInfo( void )
{
	return trial::Info;
}

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}

	namespace t1 {
        typedef bso::sS16 sValue;

        void Write(
            sValue Value,
            str::dString &Buffer)
        {
        qRH
            flx::rStringWFlow Flow;
        qRB
            Flow.Init(Buffer);
            csdcmn::Put(Value, Flow);
        qRR
        qRT
        qRE
        }

        sValue Read(const str::dString &Buffer)
        {
            sValue Value = 0;

            flx::sStringRFlow Flow;

            Flow.Init( Buffer);

            return csdcmn::Get(Flow, Value);
        }
	}

	void T1_( void )
	{
	qRH
        str::wString Buffer;
	qRB
        Buffer.Init();

        t1::Write(sclmisc::MGetS16(registry::parameter::t1::Value), Buffer);

        cio::COut << t1::Read(Buffer) << txf::nl << txf::commit;
    qRR
    qRT
    qRE
	}
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

int scltool::SCLTOOLMain(
	const str::dString &Command,
	const scltool::fOddities &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH;
qRB;
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( T1 );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR;
qRT;
qRE;
	return ExitValue;
}
