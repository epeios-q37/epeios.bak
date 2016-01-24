/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "frdfrntnd.h"
#include "flf.h"
#include "flx.h"
#include "lcl.h"
#include "ogzfbc.h"

using namespace fbltyp;

using namespace frdfrntnd;

void frdfrntnd::rFrontend::GetTypes_( void )
{
qRH
	fbltyp::strings Labels;
	fbltyp::id8s Ids;
qRB
	Labels.Init();
	Ids.Init();

	Statics.OGZGetTypes( Labels, Ids );

	sclfrntnd::Fill<types_t_, type, types_>( Ids, Labels, Types_ );
qRR
qRT
qRE
}


void  frdfrntnd::rFrontend::DumpTypes_( xml::writer_ &Writer )
{
	sclfrntnd::Dump<type__>( Types_, "Fields", "Field", Writer );
}
