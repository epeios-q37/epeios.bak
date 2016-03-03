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
			Type.Init( Retrievers.Plugin( Row ), NULL );
			Types.Add( Type );

			Row = Retrievers.Next( Row );
		}
	qRR
	qRT
	qRE
	}
}

void common::rTypes::Init( const char *Identifier )
{
	Retrievers_.Init();

	sclmisc::Plug( ogzplg::TypePluginTarget, Identifier, Retrievers_ );

	Core.Init();
	Fill_( Retrievers_, Core );
}


template <typename retriever> static void Set_(
	const char *Target,
	retriever &PluginRetriever )
{
	PluginRetriever.Init();
	sclmisc::Plug( Target, PluginRetriever );
	PluginRetriever.Plugin().Initialize();
}

void common::rDatabase::Init(
	const ogztyp::vTypes &Types,
	const char *Identifier )
{
	Retriever_.Init();
	sclmisc::Plug( ogzplg::DatabasePluginTarget, Identifier, Retriever_ );
//	P_().Initialize();

	Core.Init( Types, P_().GetDataCallback(), P_().GetColumnCallback(), P_().GetFieldCallback(), P_().GetRecordCallback(), P_().GetUserCallback() );
}

void common::rAuthentication::Init( const char *Identifier )
{
	Retriever_.Init();
	sclmisc::Plug( ogzplg::AuthenticationPluginTarget, Identifier, Retriever_ );
}

void common::Initialize( void )
{
qRH
qRB
	if ( IsInitialized() )
		qRGnr();

	Rack_.Types.Init( NULL );
	Rack_.Database.Init( Rack_.Types.Core, NULL );
	Rack_.Authentication.Init( NULL );
	Rack_.Record.Init();

	Rack.Init( Rack_ );

	IsInitialized_ = true;
qRR
qRT
qRE
}
