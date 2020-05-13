/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#define WEBSCK_COMPILATION_

#include "websck.h"

#include "cdgb64.h"
#include "flx.h"
#include "sha1.h"
#include "xtf.h"

using namespace websck;

namespace {
    namespace {
        bso::sBool GetField_(
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

            if ( Field.Label == "Sec-WebSocket-Key" ) {
                Key = Field.Value;
                return true;
            } else
                return false;
        }

        bso::sBool GetField_(
            const str::dString &Line,
            dField &Field,
            str::dString &Key )
        {
            bso::sBool ContainsKey = false;
        qRH
            flx::sStringRFlow Flow;
        qRB
            Flow.Init(Line);

            ContainsKey = GetField_(Flow, Field, Key);
        qRR
        qRT
        qRE
            return ContainsKey;
        }
    }

    bso::sBool GetHeader_(
        xtf::sRFlow &Flow,
        dHeader &Header,
        str::dString &Key )
    {
        bso::sBool ContainsKey = false;
    qRH
        str::wString Line;
        wField Field;
    qRB
        Flow.GetLine(Header.FirstLine);

        while (true) {
            Line.Init();
            Flow.GetLine(Line);

            if ( !Line.Amount() )
                break;

            Field.Init();
            ContainsKey |= GetField_(Line, Field, Key);
            Header.Fields.Append(Field);
        }
    qRR
    qRT
    qRE
        return ContainsKey;
    }
}

#define NL	"\r\n"

#define RESPONSE\
    "HTTP/1.1 101 Switching Protocols" NL\
    "Upgrade: websocket" NL\
    "Connection: Upgrade" NL\
    "Sec-WebSocket-Accept: "

namespace {
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

    void SendResponse_(
        const str::dString &Key,
        flw::rWFlow &Flow )
    {
    qRH
        str::wString ResponseKey;
        qCBUFFERh Buffer;
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
}

sdr::sRow websck::Search(
    const str::dString &Label,
    const dFields &Fields)
{
    sdr::sRow Row = Fields.First();

    while ( ( Row != qNIL ) && ( Fields(Row).Label != Label ) )
        Row = Fields.Next(Row);

    return Row;
}

bso::sBool websck::Handshake(
    fdr::rRWDriver &Driver,
    dHeader &Header)
{
    bso::sBool IsWebSocketHeader = false;
qRH
    flw::rDressedRWFlow<> Flow;
    xtf::sRFlow TFlow;
    str::wString Key;
qRB
    Flow.Init(Driver);
    TFlow.Init(Flow, utf::f_Guess);

    Key.Init();
    if ( ( IsWebSocketHeader = GetHeader_(TFlow, Header, Key) ) )
        SendResponse_(Key,Flow);
qRR
qRT
qRE
    return IsWebSocketHeader;
}

bso::sBool websck::Handshake(fdr::rRWDriver &Driver)
{
    bso::sBool IsWebSocketHeader = false;
qRH
    wHeader Header;
qRB
    Header.Init();
    IsWebSocketHeader = Handshake(Driver, Header);
qRR
qRT
qRE
    return IsWebSocketHeader;
}


bso::sSize websck::rRDriverBase_::GetLength( flw::rRFlow &Flow )
{
    bso::sSize Length = ( Flow.Get() & 0x7f );

    switch ( Length ) {
    case 127:
        Length = Flow.Get();
        Length = Length << 8 | Flow.Get();
        Length = Length << 8 | Flow.Get();
        Length = Length << 8 | Flow.Get();
        Length = Length << 8 | Flow.Get();
        Length = Length << 8 | Flow.Get();
        Length = Length << 8 | Flow.Get();
        Length = Length << 8 | Flow.Get();
        break;
    case 126:
        Length = Flow.Get();
        Length = Length << 8 | Flow.Get();
        break;
    default:
        break;
    }

    return Length;
}

fdr::size__ websck::rRDriverBase_::Read_(
    fdr::size__ Maximum,
    fdr::byte__ *Buffer )
{
    bso::sSize Amount = 0;
    bso::sBool EndOfConnection = false;

    if ( Length_ == 0 ) {
        if ( EOSPending_ ) {
            Buffer[Amount++] = EOSChar_;
            EOSPending_ = false;
        }

        EndOfConnection = Flow_.Get() & 0x8;
        Length_ = GetLength( Flow_ );
        Flow_.Read(sizeof(Mask_), &Mask_);

        if ( EndOfConnection ) {
            Flow_.Skip(Length_);
            return 0;
        } else
            MaskCounter_ = 0;
    }

    while ( Length_ && ( Amount < Maximum ) ) {
        Buffer[Amount++] = Flow_.Get() ^ ((bso::sByte *)&Mask_)[MaskCounter_++%4];
        Length_--;
    }

    if ( ( Length_ == 0 ) && ( Mode_ == mWithTerminator ) ) {
        if ( Amount < Maximum )
            Buffer[Amount++] = EOSChar_;
        else
            EOSPending_ = true;
    }

    return Amount;
}

bso::sBool websck::GetMessage(
    flw::rRFlow &Flow,
    flw::rWFlow &Message )
{
    bso::sChar C = 0;

    while ( !Flow.EndOfFlow() && ( ( C = Flow.Get() ) != EOSChar_ ) )
        Message.Put(C);

    return C == EOSChar_;
}

bso::sBool websck::GetMessage(
    flw::rRFlow &Flow,
    str::dString &Message )
{
    bso::sBool ReturnValue = false;
qRH
    flx::rStringWFlow MessageFlow;
qRB
    MessageFlow.Init(Message);

    ReturnValue = GetMessage(Flow, MessageFlow);
qRR
qRT
qRE
    return ReturnValue;
}

bso::sBool websck::SkipMessage(flw::rRFlow &Flow)
{
    return GetMessage(Flow, flx::VoidWFlow);
}

void websck::rWDriverBase_::SendSize_(
    bso::sU64 Size,
    bso::sU8 Pos )
{
    Pos *= 8;
    Flow_.Put(( Size >> Pos ) & 0xff);
}

void websck::rWDriverBase_::SendSize_(bso::sU64 Size)
{
    if ( Size < 126 )
        SendSize_(Size, 0);
    else if ( Size <= 655535 ) {
        Flow_.Put(126);
        SendSize_(Size, 1);
        SendSize_(Size, 0);
    } else {
        Flow_.Put(127);
        SendSize_(Size, 7);
        SendSize_(Size, 6);
        SendSize_(Size, 5);
        SendSize_(Size, 4);
        SendSize_(Size, 3);
        SendSize_(Size, 2);
        SendSize_(Size, 1);
        SendSize_(Size, 0);
    }
}

