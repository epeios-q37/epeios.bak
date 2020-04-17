/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XDHWebQ.

    XDHWebQ is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XDHWebQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XDHWebQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "xdwmain.h"

#include "websck.h"

#include "lstcrt.h"

using namespace xdwmain;

void xdwmain::rUpstream::XDHCMNProcess(
	const str::string_ &Script,
	str::dString *ReturnedValue )
{
qRH
    qCBUFFERh Buffer;
qRB
    Flow_.Write( Script.Convert(Buffer), Script.Amount());
    Flow_.Commit();

    if ( ReturnedValue != NULL)
        websck::GetMessage(Flow_, *ReturnedValue);

    Flow_.Dismiss();
qRR
qRT
qRE
}

namespace {
    typedef lstbch::qLBUNCHd(fdr::rWDriver *, sRow) dDrivers_;
    qW(Drivers_);

    lstcrt::qLXCRATEwl(dDrivers_) TokenizedDrivers_;
    wDrivers_ UntokenizedDrivers_;

    str::wStrings Tokens_;

    dDrivers_ *GetDrivers_(const str::dString &Token)
    {
        if ( Token.Amount() == 0 )
            return &UntokenizedDrivers_;
        else {
            sdr::sRow Row = str::Search(Token, Tokens_);

            if ( Row == qNIL ) {
                Row = Tokens_.Append(Token);

                if ( TokenizedDrivers_.New() != Row )
                    qRGnr();
            }

            if ( Row != qNIL )
                return &TokenizedDrivers_(Row);
        }

        return NULL;
    }
}

sRow xdwmain::Add(
        fdr::rWDriver &Driver,
        const str::dString &Token)
{
    dDrivers_ *Drivers = GetDrivers_(Token);

    if ( Drivers != NULL )
        return Drivers->Add(&Driver);
    else
        qRGnr();

    return qNIL;
}

void xdwmain::Remove(
    sRow Row,
    const str::dString &Token)
{
    dDrivers_ *Drivers = GetDrivers_(Token);

    if ( Drivers != NULL )
        Drivers->Delete(Row);
}

void xdwmain::rUpstream::XDHCMNBroadcast(const str::dString &Id)
{
qRH
    wDrivers_ *Drivers;
qRB
    Drivers.Init();


qRR
qRT
qRE
}



qGCTOR(xdwmain)
{

}
