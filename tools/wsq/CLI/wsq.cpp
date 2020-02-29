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
#include "cdgb64.h"
#include "cio.h"
#include "csdbns.h"
#include "epsmsc.h"
#include "sha1.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "xtf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( wsq, NAME_LC, NAME_MC );

const scli::sInfo &scltool::SCLTOOLInfo( void )
{
	return wsq::Info;
}

namespace {
	void ComputeResponseKey_(
        const str::dString &Key,
        str::dString &Response )
	{
    qRH
        str::wString Message, Hash;
    qRB
        Message.Init(Key);
        Message.Append("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");

        Hash.Init();
        sha1::Compute(Message, Hash);

        cdgb64::Encode(Hash, cdgb64::fOriginal, Response);

    qRR
    qRT
    qRE
	}
}

#define NL	"\r\n"

#define RESPONSE\
    "HTTP/1.1 101 Switching Protocols" NL\
    "Upgrade: websocket" NL\
    "Connection: Upgrade" NL\
    "Sec-WebSocket-Accept: "

void SendResponse(
    const str::dString &Key,
    flw::rWFlow &Flow )
{
qRH
    str::wString ResponseKey;
    qCBUFFERr Buffer;
qRB
    ResponseKey.Init();
    ComputeResponseKey_(Key, ResponseKey);
    Flow.Write(RESPONSE,sizeof(RESPONSE)-1);
    Flow.Write(ResponseKey.Convert(Buffer),ResponseKey.Amount());
    Flow.Write(NL NL, 2*(sizeof(NL)-1));
    Flow.Commit();
qRR
qRT
qRE
}


namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}

	typedef csdbns::cProcessing cProcessing_;

	class dField {
	public:
        struct s {
            str::dString::s
                Label,
                Value;
        };
        str::dString
            Label,
            Value;
        dField( s &S )
        : Label( S.Label),
          Value( S.Value )
        {}
        void reset( bso::sBool P = true ) {
            tol::reset( P, Label, Value );
        }
        dField &operator =( const dField &Field )
        {
            Label = Field.Label;
            Value = Field.Value;

            return *this;
        }
		void plug( qASd *AS )
		{
            Label.plug( AS );
			Value.plug( AS );
		}
        qDTOR( dField );
		void Init( void ) {
            tol::Init( Label, Value );
        }
	};

	qW( Field );

	typedef crt::qCRATEdl( dField ) dFields;
	qW( Fields );

	void Extract(
        flw::rRFlow &Flow,
        dField &Field,
        str::dString &Key)
	{
        bso::sChar C = 0;

        while ( !Flow.EndOfFlow() && ( ( C = Flow.Get() ) != ':' ) )
            Field.Label.Append( C );

        if ( C != ':' )
            qRGnr();

        if ( Flow.Get() != ' ' )
            qRGnr();

        while ( !Flow.EndOfFlow() )
            Field.Value.Append( Flow.Get());

        if ( Field.Label == "Sec-WebSocket-Key" )
            Key = Field.Value;
	}

	void Extract(
        const str::dString &Line,
        dField &Field,
        str::dString &Key )
	{
    qRH
        flx::sStringRFlow Flow;
    qRB
        Flow.Init(Line);

        Extract(Flow, Field, Key);
    qRR
    qRT
    qRE
	}

	void Fill(
        xtf::sRFlow &Flow,
        dFields &Fields,
        str::dString &Key )
    {
    qRH
        str::wString Line;
        wField Field;
    qRB
        Flow.SkipLine();

        while (true) {
            Line.Init();
            Flow.GetLine(Line);

            if ( !Line.Amount() )
                break;

            Field.Init();
            Extract(Line, Field, Key);
            Fields.Append(Field);
        }
    qRR
    qRT
    qRE
    }

    void Display( const dField &Field )
    {
        cio::COut <<  Field.Label << " => '" << Field.Value << '\'' << txf::nl;
    }

    void Display( const dFields &Fields )
    {
        sdr::sRow Row = Fields.First();

        while ( Row != qNIL ) {
            Display( Fields( Row ) );

            Row = Fields.Next( Row );
        }

        cio::COut.Commit();
    }

    void Handshake(fdr::rRWDriver &RWDriver)
    {
    qRH
        flw::rDressedRWFlow<> Flow;
        xtf::sRFlow TFlow;
        wFields Fields;
        str::wString Key;
    qRB
        Flow.Init(RWDriver );
        TFlow.Init(Flow, utf::f_Guess);

        tol::Init(Fields, Key);
        Fill(TFlow, Fields, Key);
        Display(Fields);
        cio::COut << "KEY: '" << Key << '\'' << txf::nl << txf::commit;

        SendResponse(Key,Flow);
    qRR
    qRT
    qRE
    }

    bso::sSize GetLength( flw::rRFlow &Flow ) {
        bso::sSize Length = ( Flow.Get() & 0x7f );

        switch ( Length ) {
        case 127:
            Length = 0;
            Length = Length << 8 | Flow.Get();
            Length = Length << 8 | Flow.Get();
            Length = Length << 8 | Flow.Get();
            Length = Length << 8 | Flow.Get();
            Length = Length << 8 | Flow.Get();
            Length = Length << 8 | Flow.Get();
            Length = Length << 8 | Flow.Get();
            Length = Length << 8 | Flow.Get();
            break;
        case 126:
            Length = 0;
            Length = Length << 8 | Flow.Get();
            Length = Length << 8 | Flow.Get();
            break;
        default:
            break;
        }

        return Length;
    }

	class sCallback
	: public cProcessing_
	{
    protected:
        void *CSDSCBPreProcess(
            fdr::rRWDriver *RWDriver,
            const ntvstr::char__ *Origin ) override
        {
            cio::COut << "New connection !!!" << txf::nl << txf::commit;

            Handshake(*RWDriver);

            return NULL;
        }
        csdscb::action__ CSDSCBProcess(
            fdr::rRWDriver *RWDriver,
            void *UP ) override
        {
        qRH
            flw::rDressedRWFlow<> Flow;
            bso::sSize Length= 0;
            bso::sU32 Mask = 0;
            bso::sU8 Counter = 0;
            str::wString Data;
        qRB
            Flow.Init(*RWDriver );

            while( true ) {
            Flow.Get();

            Length = GetLength( Flow );
            cio::COut << "Length: " << Length << txf::nl << txf::commit;

            Flow.Read(sizeof(Mask), &Mask);

            Data.Init();

            Counter = 0;

            while ( Length-- ) {
                Data.Append(Flow.Get() ^ ((bso::sByte *)&Mask)[Counter++%4]);
            }

            cio::COut << Data << txf::nl << txf::commit;
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
        sCallback Callback;
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

