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


#include "x2mid.h"

#include "mscmdx.h"

#include "sclm.h"

#include "xml.h"
#include "xtf.h"

namespace {
    void Convert_(
        xtf::sRFlow &RFlow,
        flw::rWFlow &WFlow )
    {
        xml::status__ Status = xml::s_Undefined;

        if ( ( Status = mscmdx::XMIDToMIDI( RFlow, mscmdm::xTicks, WFlow ) ) != xml::sOK )
            sclm::ReportAndAbort("XMIDError", xml::GetLabel( Status ), RFlow.Position().Line, RFlow.Position().Column );
    }

    void Convert_(
        xtf::sRFlow &RFlow,
        const fnm::rName &Target )
    {
    qRH
        sclm::rWFlowRack Rack;
    qRB
        Convert_( RFlow, Rack.Init(Target) );
    qRR
        Rack.HandleError();
    qRT
    qRE
    }

}

void x2mid::Convert(
	const fnm::rName &Source,
	const fnm::rName &Target )
{
qRH;
	flf::rRFlow RFlow;
	xtf::sRFlow TFlow;
qRB;
	if ( RFlow.Init( Source, err::hUserDefined ) != tol::rSuccess ) {
        sclm::ReportFileOpeningErrorAndAbort(Source);
	}

	TFlow.Init( RFlow, utf::f_Guess );

	Convert_( TFlow, Target );
qRR;
qRT;
qRE;
}


