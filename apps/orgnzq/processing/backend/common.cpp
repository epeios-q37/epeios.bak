/*
	'common.cpp' by Claude SIMON (http://q37.info/contact/).

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

#include "common.h"

#include "sclmisc.h"

using namespace common;

naked_rack___ Rack_;
rack___ common::Rack;
bso::bool__ IsInitialized_ = false;

bso::bool__ common::IsInitialized( void )
{
	return IsInitialized_;
}

namespace {
	void Fill_(
		const plgn::rRetrievers<ogzplg::fTypePlugin> &Retrievers,
		ogztyp::vTypes &Types )
	{
	qRH
		plgn::fRow Row = qNIL;
		ogztyp::fType Type;
	qRB
		Row = Retrievers.First();

		while ( Row != qNIL )
		{
			Type.Init( Retrievers.Plugin( Row ) );
			Types.Add( Type );

			Row = Retrievers.Next( Row );
		}
	qRR
	qRT
	qRE
	}
}

void common::rTypes::Init( void )
{
	Retrievers_.Init();

	sclmisc::Plug( ogzplg::TypePluginTarget, Retrievers_ );
}


template <typename retriever> static void Set_(
	const char *Target,
	retriever &PluginRetriever )
{
	PluginRetriever.Init();
	sclmisc::Plug( Target, PluginRetriever );
	PluginRetriever.Plugin().Initialize();
}

void common::rDatabase::Init( const ogztyp::vTypes &Types )
{
	Retriever_.Init();
	sclmisc::Plug( ogzplg::DatabasePluginTarget, Retriever_ );
//	P_().Initialize();

	Core.Init( Types, P_().GetDTA(), P_().GetCLM(), P_().GetFLD(), P_().GetRCD(), P_().GetUSR() );
}


void common::Initialize( void )
{
qRH
qRB
	if ( IsInitialized() )
		qRGnr();

	Rack_.Types.Init();
//	Rack_.Database.Init( Rack_.Types.Core );

	Rack.Init( Rack_ );

	IsInitialized_ = true;
qRR
qRT
qRE
}
