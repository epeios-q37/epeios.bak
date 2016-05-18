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

#include "frdmisc.h"

#include "frdplgn.h"

#include "sclmisc.h"

#include "flf.h"
#include "fnm.h"

using namespace frdmisc;

namespace {
	frdplgn::rXMLRetrievers XML_;
}

frdcllbck::cXML &frdmisc::Get( const str::dString &Label )
{
	plgn::sRow Row = qNIL;
qRH
	str::wString Id;
qRB
	Row = XML_.First();

	Id.Init();
	while ( (Row != qNIL) && ( Label != XML_.Plugin( Row ).GetBackendPluginId( Id ) ) ) {
		Row = XML_.Next( Row );

		Id.Init();
	}

	if ( Row == qNIL )
		qRGnr();
qRR
qRT
qRE
	return XML_.Plugin( Row );
}


void frdmisc::LoadPugins( void )
{
	XML_.Init();
	sclmisc::Plug( frdplgn::XMLTarget, NULL, XML_ );
}


