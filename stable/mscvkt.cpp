/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#define MSCVKT__COMPILATION

#include "mscvkt.h"

using namespace mscvkt;

row__ mscvkt::tree_::_GetNode(
	const description__ &Description,
	row__ Parent )
{
	row__ Row = _dtree_::FirstChild( Parent );

	while ( ( Row != NONE ) && ( Get( Row ).Description != Description ) )
		Row = _dtree_::NextSibling( Row );

	return Row;
}

row__ mscvkt::tree_::_GetOrCreateNode(
	const description__ &Description,
	address__ Address,
	row__ Parent,
	row__ Candidate )
{
	row__ &Row = Candidate;

	if ( ( Row == NONE ) || ( nodes_::Get( Row ).Description != Description ) ) {
		if ( ( Row = _GetNode( Description, Parent ) ) == NONE ) {
			Row = nodes_::Append( node__( Description, GetLevel( Parent ) <= 1 ? Address : 0xffffffff ) );
			_dtree_::Allocate( *Row + 1 );
			_dtree_::BecomeLastChild( Row, Parent );
		}
	}

	return Row;
}

row__ mscvkt::tree_::_CreateNode(
	const descriptions__ &Descriptions,
	address__ Address,
	rows__ &Rows )
{
	row__ NRow = S_.Root;
	epeios::row__ DRow = Descriptions.First();

	while ( DRow != NONE ) {
		if ( !Rows.Exists( DRow ) )
			Rows.Append( NONE );

		NRow = _GetOrCreateNode( Descriptions( DRow ), Address, NRow, Rows( DRow ) );

		if ( Rows( DRow ) != NRow ) {
			Rows.Store( NRow, DRow );

			if ( Rows.Last() != DRow )
				Rows.Truncate( Rows.Next( DRow ) );
		}

		DRow = Descriptions.Next( DRow );
	}

	return NRow;
}

row__ mscvkt::tree_::_Add( 
	address__ Address,
	datum__ Datum,
	rows__ &Rows )
{
	row__ Row;
	descriptions__ Descriptions;

	Descriptions.Init();

	mscvkp::GetDescription( Address, Descriptions );
#ifdef MSCVKT_DBG
	if ( Descriptions.Amount() < 1 )
		ERRc();
#endif

	Row = _CreateNode( Descriptions, Address, Rows );

	Store( node__( Descriptions( Descriptions.Last() ), Address, Datum ), Row );

	return Row;
}

void mscvkt::tree_::Add( const mscvkp::data_set_ &DataSet )
{
	rows__ Rows;
	address__ Address = DataSet.Address();
	epeios::row__ Row = DataSet.First();

	Rows.Init();

	while ( Row != NONE ) {
		_Add( Address++, DataSet( Row ), Rows );

		Row = DataSet.Next( Row );
	}
}

void mscvkt::tree_::Add( const mscvkp::data_sets_ &DataSets )
{
	epeios::row__ Row = DataSets.First();
	ctn::E_CMITEM( mscvkp::data_set_ ) DataSet;

	DataSet.Init( DataSets );

	while ( Row != NONE ) {
		Add( DataSet( Row ) );

		Row = DataSets.Next( Row );
	}
}

txf::text_oflow__ operator <<(
	txf::text_oflow__ &TFlow,
	const mscvkt::tree_ &Tree )
{
	dtr::browser__<row__>  Browser;
	row__ Row = NONE;
	mscvkp::descriptions__ Descriptions;

	Descriptions.Init();

	Descriptions.Append( "Root" );

	Browser.Init( Tree.GetRoot() );

	Row = Tree.Browse( Browser );

	Row = Browser.Position();

	while ( Row != NONE ) {
		switch ( Browser.Kinship() ) {
		case dtr::kChild:
			Descriptions.Append( Tree( Row ).Description );
			break;
		case dtr::kParent:
			Descriptions.Truncate();
			break;
		case dtr::kSibling:
			Descriptions.Truncate();
			Descriptions.Append( Tree( Row ).Description );
			break;
		default:
			ERRc();
			break;
		}

		if ( Tree._dtree_::FirstChild( Row ) == NONE )
			TFlow << "(" << mscvkp::AddressToString( Tree( Row ).Address ) << ") " << Descriptions << " : " << mscvkp::DatumToString( Tree( Row ).Datum, false ) << txf::nl;

		Row = Tree.Browse( Browser );
	}

	return TFlow;
}
