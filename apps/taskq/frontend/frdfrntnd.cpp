/*
	Copyright (C) 2021 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'TaskQ' software.

    'TaskQ' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'TaskQ' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'TaskQ'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "frdfrntnd.h"
#include "flf.h"
#include "flx.h"
#include "lcl.h"
#include "tskfbc.h"

using namespace fbltyp;

using namespace frdfrntnd;

dString &frdfrntnd::rMyObject::ToUpper( dString &String )
{
qRH
	wString Result;
qRB
	Result.Init();

	Object_.ToUC_1( String, Result );

	String = Result;
qRR
qRT
qRE
	return String;
}

