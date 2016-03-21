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

namespace {
	rTypes Types_;
	ogztyp::sRow
		RecordType = qNIL,
		extType_ = qNIL;
	rNakedRack Rack_;
	bso::bool__ IsInitialized_ = false;
}

rRack common::Rack;

const ogztyp::dTypes &common::GetTypes( void )
{
	return Types_.Core();
}

ogztyp::sRow common::GetMandatoryRecordType( void )
{
	return Types_.GetMandatoryRecordType();
}

ogztyp::sRow common::GetMandatoryTextType( void )
{
	return Types_.GetMandatoryTextType();
}


bso::bool__ common::IsInitialized( void )
{
	return IsInitialized_;
}

namespace {
	E_CDEF(char *, RecordTypeLabel_, "Record");
	E_CDEF(char *, TextTypeLabel_, "Text");

	void Fill_(
		const plgn::rRetrievers<ogzplg::fTypePlugin> &Retrievers,
		ogztyp::dTypes &Types )
	{
	qRH
		plgn::sRow Row = qNIL;
		ogztyp::sType Type;
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

void common::rTypes::FetchMandatoryTypes_( void )
{
	ogztyp::sRow Row = Core_.First();
	ogztyp::sType Type;

	while ( Row != qNIL ) {
		Core_.Recall( Row, Type );

		if ( !strcmp( Type.GetLabel(), RecordTypeLabel_ ) )
			MandatoryRecordType_ = Row;
		else if ( !strcmp( Type.GetLabel(), TextTypeLabel_ ) )
			MandatoryTextType_ = Row;

		Type.reset( false );

		Row = Core_.Next( Row );
	}

	if ( MandatoryRecordType_ == qNIL )
		qRGnr();

	if ( MandatoryTextType_ == qNIL )
		qRGnr();
}

void common::rTypes::Init( const char *Identifier )
{
	Retrievers_.Init();

	sclmisc::Plug( ogzplg::TypePluginTarget, Identifier, Retrievers_ );

	Core_.Init();
	Fill_( Retrievers_, Core_ );
	FetchMandatoryTypes_();
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
	const ogztyp::dTypes &Types,
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

	Types_.Init( NULL );
	Rack_.Database.Init( GetTypes(), NULL );
	Rack_.Authentication.Init( NULL );

	Rack.Init( Rack_ );

	IsInitialized_ = true;
qRR
qRT
qRE
}
