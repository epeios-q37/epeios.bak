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

#include "frdplgn.h"

#include "flf.h"
#include "flx.h"
#include "lcl.h"
#include "ogzfbc.h"

using namespace fbltyp;

using namespace frdfrntnd;

namespace {
	void Fill_(
		const fbltyp::dIds &Ids,
		const fbltyp::dStrings &Labels,
		frdmisc::dXTypes &Types )
	{
	qRH
		frdmisc::wXType Type;
		sdr::sRow Row = qNIL;
	qRB
		if ( Ids.Amount() != Labels.Amount() )
			qRGnr();

		Row = Ids.First();

		while ( Row != qNIL ) {
			Type.Init( Ids(Row), Labels(Row), frdmisc::Get(Labels( Row ) ) );

			Row = Ids.Next( Row );
		}
	qRR
	qRT
	qRE
	}
}

void frdfrntnd::rFrontend::GetTypes_( void )
{
qRH
	fbltyp::wIds Ids;
	fbltyp::strings Labels;
qRB
	Ids.Init();
	Labels.Init();

	Statics.OGZGetTypes( Ids, Labels );

	Fill_( Ids, Labels, Types_ );
qRR
qRT
qRE
}

void  frdfrntnd::rFrontend::DumpTypes_( xml::writer_ &Writer )
{
	sclfrntnd::Dump<sType>( Types_, "Types", "Type", Writer );
}

void frdfrntnd::rFrontend::GetNumbers_( void )
{
qRH
	fbltyp::id8s Ids;
	fbltyp::strings Labels, Wordings;
qRB
	Ids.Init();
	Labels.Init();
	Wordings.Init();

	Statics.OGZGetNumbers( Ids, Labels, Wordings );

	sclfrntnd::Fill<sNumber, dNumbers>( Ids, Labels, Wordings, Numbers_ );
qRR
qRT
qRE
}

void  frdfrntnd::rFrontend::DumpNumbers_( xml::writer_ &Writer )
{
	sclfrntnd::Dump<sNumber>( Numbers_, "Numbers", "Number", Writer );
}

