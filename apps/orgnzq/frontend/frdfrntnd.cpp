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
		const fbltyp::dStrings &PluginIds,
		frdmisc::dXTypes &Types )
	{
	qRH
		frdmisc::wXType Type;
		sdr::sRow Row = qNIL;
	qRB
		if ( Ids.Amount() != Labels.Amount() )
			qRGnr();

		if ( Ids.Amount() != PluginIds.Amount() )
			qRGnr();

		Row = Ids.First();

		while ( Row != qNIL ) {
			Type.Init( Ids(Row), Labels(Row), frdmisc::Get( PluginIds( Row ) ) );

			Types.Append( Type );

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
	fbltyp::strings Labels, PluginIds;
qRB
	Ids.Init();
	Labels.Init();
	PluginIds.Init();

	Statics.OGZGetTypes( Ids, Labels, PluginIds );

	Fill_( Ids, Labels, PluginIds, Types_ );
qRR
qRT
qRE
}

namespace {
	void Dump_(
		const frdmisc::dXType &Type,
		xml::dWriter &Writer )
	{
		Writer.PushTag( "Type" );
		Writer.PutAttribute( "id", **Type.Id() );
		Writer.PutAttribute( "label", Type.Label );
		Writer.PopTag();
	}

	void Dump_(
		const frdmisc::dXTypes &Types,
		xml::dWriter &Writer )
	{
		sdr::sRow Row = Types.First();

		Writer.PushTag( "Types" );
		Writer.PutAttribute("Amount", Types.Amount() );

		while ( Row != qNIL ) {
			Dump_( Types( Row ), Writer );

			Row = Types.Next( Row );
		}

		Writer.PopTag();
	}
}

void  frdfrntnd::rFrontend::DumpTypes_( xml::writer_ &Writer )
{
	Dump_( Types_, Writer );
}

void frdfrntnd::rFrontend::GetNumbers_( void )
{
	Numbers_.Init();

	Statics.OGZGetNumbers( Numbers_.Ids, Numbers_.Strings1, Numbers_.Strings2 );
}

void  frdfrntnd::rFrontend::DumpNumbers_( xml::writer_ &Writer )
{
	sclfrntnd::DumpAsLabelWording( Numbers_, "Numbers", "Number", Writer );
}

