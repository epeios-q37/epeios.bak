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

#define C( name )\
	case m##name:\
		return #name;\
		break


const char *common::GetLabel( eMessage Message )
{
	switch ( Message ){
	C( BadEntryValue );
	C( FieldLabelCanNotBeEmpty );
	C( FieldNameAlreadyUsed );
	C( NoSuchEntry );
	C( NoSuchField );
	C( NoSuchRecord );
	C( TestMessage );
	C( UnknownEntry );
	C( UnknownFieldNumber );
	C( UnknowFieldType );
	default:
		qRGnr();
		break;
	}

	return NULL;
}

#undef C


namespace {
	rTypes Types_;
	ogztyp::sRow
		RecordType = qNIL,
		TextType_ = qNIL;
		class rRack {
	public:
		rDatabase Database;
		rAuthentication Authentication;
	} Rack_;

	bso::bool__ IsInitialized_ = false;
}

ogzdtb::mDatabase &common::Database( void )
{
	return Rack_.Database;
}

ogzusr::mAuthentication &common::Authentication( void )
{
	return Rack_.Authentication;
}

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
			Type.Init( Retrievers.Plugin( Row ), Retrievers.Identifier( Row ) );
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

	mDatabase_::Init( Types, P_().GetMetaCallback(), P_().GetColumnCallback(), P_().GetEntryCallback(), P_().GetFieldCallback(), P_().GetRecordCallback(), P_().GetUserCallback() );
}

void common::rAuthentication::Init( const char *Identifier )
{
	Retriever_.Init();
	sclmisc::Plug( ogzplg::AuthenticationPluginTarget, Identifier, Retriever_ );

	mAuthentication_::Init( P_() );
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

	IsInitialized_ = true;
qRR
qRT
qRE
}

qGCTOR( common )
{
}
