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
    void Display_(const websck::dFields &Fields) {
        sdr::sRow Row = Fields.First();

        while ( Row != qNIL ) {
            cio::COut << Fields(Row).Label << " -> " << Fields(Row).Value << txf::nl;

            Row = Fields.Next(Row);
        }

        cio::COut.Commit();
    }

    void Display_(const websck::dHeader &Header) {
        cio::COut << Header.FirstLine << txf::nl;

        Display_(Header.Fields);

        cio::COut.Commit();
    }
}
#define TEXT "My name is JohnLorem ipsum dolor sit amet, consectetur adipiscing elit. Sed non risus. Suspendisse lectus tortor, dignissim sit amet, adipiscing nec, ultricies sed, dolor. Cras elementum ultrices diam. Maecenas ligula massa, varius a, semper congue, euismod non, mi. Proin porttitor, orci nec nonummy molestie, enim est eleifend mi, non fermentum diam nisl sit amet erat. Duis semper. Duis arcu massa, scelerisque vitae, consequat in, pretium a, enim. Pellentesque congue. Ut in risus volutpat libero pharetra tempor. Cras vestibulum bibendum augue. Praesent egestas leo in pede. Praesent blandit odio eu enim. Pellentesque sed dui ut augue blandit sodales. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Aliquam nibh. Mauris ac mauris sed pede pellentesque fermentum. Maecenas adipiscing ante non diam sodales hendrerit"

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
        qRH
            websck::wHeader Header;
        qRB
            cio::COut << "New connection !!!" << txf::nl << txf::commit;

            Header.Init();
            websck::Handshake(*Driver, Header);

            Display_(Header);
        qRR
        qRT
        qRE
            return NULL;
        }
        csdscb::action__ CSDSCBProcess(
            fdr::rRWDriver *Driver,
            void *UP ) override
        {
        qRH
            websck::rRFlow RFlow;
            websck::rWFlow WFlow;
        qRB
            RFlow.Init(*Driver );
            WFlow.Init(*Driver);

            WFlow.Write("coucou", 6);
            WFlow.Write("youpou", 6);
            WFlow.Commit();

            WFlow.Write("youhou", 6);
            WFlow.Commit();

            WFlow.Write(TEXT, sizeof(TEXT)-1);
            WFlow.Commit();


            while( true ) {
                cio::COut << RFlow.Get() << txf::commit;
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

