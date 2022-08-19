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


#include "mid2x.h"

#include "mscmdm.h"
#include "mscmdx.h"

#include "flw.h"
#include "txf.h"
#include "xml.h"

#include "sclm.h"

namespace {
    void Convert_(
        flw::rRFlow &RFlow,
        txf::sWFlow &WFlow )
    {
    qRH
        xml::rWriter Writer;
    qRB
        Writer.Init( WFlow, xml::oIndent, xml::e_Default );

        mscmdx::MIDIToXMID( RFlow, mscmdm::xTicks, Writer );
    qRR
    qRT
    qRE
    }

    void Convert_(
        flw::rRFlow &RFlow,
        const fnm::rName &Target )
    {
    qRH
        sclm::rTextWFlowRack Rack;
    qRB
        Convert_( RFlow, Rack.Init(Target) );
    qRR
        Rack.HandleError();
    qRT
    qRE
    }
}

void mid2x::Convert(
	const fnm::rName &Source,
	const fnm::rName &Target )
{
qRH
	flf::file_iflow___ IFlow;
qRB
	if ( IFlow.Init( Source, err::hUserDefined ) != tol::rSuccess ) {
        sclm::ReportFileOpeningErrorAndAbort(Source);
	}

	Convert_( IFlow, Target );
qRR
qRT
qRE
}
