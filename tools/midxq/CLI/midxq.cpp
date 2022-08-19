/*
	Copyright (C) 2019 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'MIDXq' tool.

    'MIDXq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MIDXq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MIDXq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "midxq.h"

#include "mid2x.h"
#include "registry.h"
#include "x2mid.h"

#include "sclt.h"
#include "scle.h"
#include "sclm.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( midxq, NAME_LC, NAME_MC );

const scli::sInfo &sclt::SCLTInfo( void )
{
	return midxq::Info;
}

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}

	void MID2X_( void )
	{
	qRH;
        str::wString MIDISource, XMIDTarget;
	qRB;
        tol::Init(MIDISource, XMIDTarget);

        sclm::MGetValue(registry::parameter::MIDISource, MIDISource);
        sclm::OGetValue(registry::parameter::XMIDTarget, XMIDTarget);

        mid2x::Convert(MIDISource, XMIDTarget);
	qRR;
	qRT;
	qRE;
	}

	void X2MID_( void )
	{
	qRH;
        str::wString XMIDSource, MIDITarget;
	qRB;
        tol::Init(XMIDSource, MIDITarget);

        sclm::MGetValue(registry::parameter::XMIDSource, XMIDSource);
        sclm::MGetValue(registry::parameter::MIDITarget, MIDITarget);

        x2mid::Convert(XMIDSource, MIDITarget);
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
	C( MID2X );
	C( X2MID );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR;
qRT;
qRE;
	return ExitValue;
}
