/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

//	$Id: mscvkt.h,v 1.1 2010/07/16 08:25:54 csimon Exp $

#ifndef MSCVKT__INC
#define MSCVKT__INC

#define MSCVKT_NAME		"MSCVKT"

#define	MSCVKT_VERSION	"$Revision: 1.1 $"

#define MSCVKT_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( MSCVKT_NODBG )
#define MSCVKT_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.1 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2010/07/16 08:25:54 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D MuSiC VK (from Roland VK combo organ) Tree 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "dtr.h"
#include "mscvkp.h"

namespace mscvkt {
	using namespace mscvkp;

	struct node__ {
	public:
		description__ Description;
		address__ Address;
		datum__ Datum;	// Only valid if 'Address' != MSCVKT_UNDEFINED.
		void reset( bso::bool__ P = true )
		{
			Description.reset( P );
			Address = MSCVKP_UNDEFINED_ADDRESS;
			Datum = 0;
		}
		node__( void )
		{
			reset( false );
		}
		node__(
			const description__ &Description,
			address__ Address,
			datum__ Datum = (char)0xff )
		{
			this->Address = Address;
			this->Description = Description;
			this->Datum = Datum;
		}
	};

	E_ROW( row__ );

	typedef bch::E_BUNCH__( row__, MSCVKP_LEVEL_MAX ) rows__;

	typedef bch::E_BUNCHt_( node__, row__ )	nodes_;
	E_AUTO( nodes )

	typedef dtr::E_DTREEt_( row__ )	_dtree_;

	class tree_
	: public nodes_,
	  public _dtree_
	{
	private:
		row__ _GetNode(
			const description__ &Descritpion,
			row__ Row );
		row__ _GetOrCreateNode(
			const description__ &Description,
			address__ Address,
			row__ Row,
			row__ Candidate );
		row__ _CreateNode(
			const descriptions__ &Descriptions,
			address__ Address,
			rows__ &Rows );
		row__ _Add( 
			address__ Adress,
			datum__ Datum,
			rows__ &Rows );
	public:
		struct s
		: public nodes_::s,
		  public _dtree_::s
		{
			row__ Root;
		} &S_;
		tree_( s &S )
		: S_( S ),
		  nodes_( S ),
		  _dtree_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			nodes_::reset( P );
			_dtree_::reset( P );
			S_.Root = NONE;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			nodes_::plug( MM );
			_dtree_::plug( MM );
		}
		tree_ &operator=( const tree_ &T )
		{
			S_.Root = T.S_.Root;
			nodes_::operator= ( T );
			_dtree_::operator =( T );

			return *this;
		}
		void Init( void )
		{
			nodes_::Init();
			_dtree_::Init();

			S_.Root = 0;

			_dtree_::Allocate( 1 );
			nodes_::Allocate( 1 );

			nodes_::Store( node__( "VK77", 0xffffffff ), S_.Root );
		}
		void Add( const mscvkp::data_set_ &DataSet );
		void Add( const mscvkp::data_sets_ &DataSets );
		row__ GetRoot( void ) const
		{
			return S_.Root;
		}
	};

	E_AUTO( tree )

}

txf::text_oflow__ operator <<(
	txf::text_oflow__ &OFlow,
	const mscvkt::tree_ &Tree );

/*$END$*/
#endif
