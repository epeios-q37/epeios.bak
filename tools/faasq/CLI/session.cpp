/*
	Copyright (C) 2019 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'FaaSq' tool.

    'FaaSq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'FaaSq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'FaaSq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "session.h"

#include "csdcmn.h"

using namespace session;

void session::sUpstream_::XDHCMNProcess(
    const str::string_ &Script,
    str::dString *ReturnedValue )
{
    csdcmn::Put(Id_, Proxy_);
    csdcmn::Put("Execute_1", Proxy_);

    csdcmn::Put((bso::sU8)(ReturnedValue == NULL ? 0 : 1), Proxy_);
    csdcmn::Put((bso::sU8)1,Proxy_);
    csdcmn::Put(Script, Proxy_);
    csdcmn::Put((bso::sU8)0, Proxy_);

    Proxy_.Commit();

    if ( ReturnedValue != NULL) {
        sId Id = 0;

        if ( csdcmn::Get(Proxy_, Id) != Id_ )
            qRGnr();

        csdcmn::Get(Proxy_, *ReturnedValue);
        Proxy_.Dismiss();
    }
}

void session::sSession::Launch(
    const str::dString &Id,
    const str::dString &Action )
{
qRH
    qCBUFFERr IdBuffer, ActionBuffer;
qRB
    Session_.Launch(Id.Convert(IdBuffer), Action.Convert(ActionBuffer));
qRR
qRT
qRE
}

sRow session::Search(
    sId Id,
    const dIds &Ids)
{
    sRow Row = Ids.First();

    while ( ( Row != qNIL ) && ( Ids(Row) != Id) )
        Row = Ids.Next(Row);

    return Row;
}
