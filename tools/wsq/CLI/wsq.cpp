/*
	Copyright (C) 2019 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'WSq' tool.

    'WSq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'WSq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'WSq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "wsq.h"

#include "registry.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "csdbns.h"
#include "epsmsc.h"
#include "websck.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( wsq, NAME_LC, NAME_MC );

const scli::sInfo &scltool::SCLTOOLInfo( void )
{
	return wsq::Info;
}

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}
}

namespace {
	typedef csdbns::cProcessing cProcessing_;

	class sCallback_
	: public cProcessing_
	{
    protected:
        void *CSDSCBPreProcess(
            fdr::rRWDriver *Driver,
            const ntvstr::char__ *Origin ) override
        {
            cio::COut << "New connection !!!" << txf::nl << txf::commit;

            websck::Handshake(*Driver);

            return NULL;
        }
        csdscb::action__ CSDSCBProcess(
            fdr::rRWDriver *Driver,
            void *UP ) override
        {
        qRH
            websck::rRFlow Flow;
        qRB
            Flow.Init(*Driver );

            while( true ) {
                cio::COut << Flow.Get() << txf::commit;
            }

        qRR
        qRT
        qRE
            return csdscb::aContinue;
        }
        virtual bso::sBool CSDSCBPostProcess( void *UP ) override
        {
            return true;
        }
     public:
        void Init( void ) {}
	};

	void Test_( void )
	{
	qRH;
        csdbns::rServer Server;
        sCallback_ Callback;
	qRB;
        Callback.Init();
        Server.Init(58000, Callback );
        Server.Process( NULL );
	qRR;
	qRT;
	qRE;
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
	C( Test );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR;
qRT;
qRE;
	return ExitValue;
}

