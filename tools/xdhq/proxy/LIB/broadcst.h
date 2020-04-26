/*
	Copyright (C) 2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

    'XDHq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BROADCST_INC_
# define BROADCST_INC_

# include "fdr.h"
# include "sdr.h"

namespace broadcst {
    qROW(Row);

    sRow Add(
        fdr::rWDriver &Driver,
        const str::dString &Token);

    void Broadcast(
        const str::dString &Message,
        const str::dString &Token);

    void Remove(sRow Row);
}

#endif
