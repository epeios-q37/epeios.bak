/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

//	$Id: dbt.h,v 1.23 2013/04/13 16:21:15 csimon Exp $

#ifndef DBT__INC
#define DBT__INC

#define DBT_NAME		"DBT"

#define	DBT_VERSION	"$Revision: 1.23 $"

#define DBT_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( DBT_NODBG )
#define DBT_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.23 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/13 16:21:15 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D Differentiated Binary Tree 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bch.h"
#include "lst.h"
#include "lstbch.h"
#include "btr.h"
#include "btf.h"

namespace dbt {

	using btf::row__;

	//t Row type for an internal node.
	qROW( internal_row__ );	// Node with childrens (always 2).

	//t Row type for an external node.
	qROW( external_row__ );	// Node without children.

	enum type__ {
		internal = 0,	// false
		external = 1	// true.
	};

	using btr::binary_tree_;
	using lst::list_;
	using btf::binary_tree_filler_;

	//c Differenciated binary tree.
	template <typename int__, typename ext__, typename irow__ > class differentiated_binary_tree_
	: public E_LISTt_( irow__ ),
	  public E_BTREEt_( irow__ )
	{
	protected:
		void LSTAllocate(
			sdr::size__ Size,
			aem::mode__ Mode )
		{
			Nodes.Allocate( Size, Mode );
			Types.Allocate( Size, Mode );
			E_BTREEt_( irow__ )::Allocate( Size, Mode );
			DBTAllocate( Size, Mode );
		}
		//v To synchronize size of this tree with other bunch/container. Do nothing by default.
		virtual void DBTAllocate(
			sdr::size__ Size,
			aem::mode__ Mode ) = 0;
	public:
		struct s
		: public E_LISTt_( irow__ )::s,
		  public E_BTREEt_( irow__ )::s
		{
			typename lstbch::E_LBUNCHt_( int__, internal_row__ )::s Internals;
			typename lstbch::E_LBUNCHt_( ext__, external_row__ )::s Externals;
			typename bch::E_BUNCHt_( sdr::row_t__, irow__ )::s Nodes; 
			typename bitbch::bit_bunch_< irow__ >::s Types;
		};
		lstbch::E_LBUNCHt_( int__, internal_row__ ) Internals;
		lstbch::E_LBUNCHt_( ext__, external_row__ ) Externals;
		bch::E_BUNCHt_( sdr::row_t__, irow__ ) Nodes;
		bitbch::bit_bunch_< irow__ > Types;
		differentiated_binary_tree_( s &S )
		: E_LISTt_( irow__ )( S ),
		  E_BTREEt_( irow__ )( S ),
		  Internals( S.Internals ),
		  Externals( S.Externals ),
		  Nodes( S.Nodes ),
		  Types( S.Types )
		{}
		void reset( bso::bool__ P = true )
		{
			E_LISTt_( irow__ )::reset( P );
			E_BTREEt_( irow__ )::reset( P );
			Internals.reset( P );
			Externals.reset( P );
			Nodes.reset( P );
			Types.reset( P );
		}
		void plug( qAS_ &AS )
		{
			E_LISTt_( irow__ )::plug( AS );
			E_BTREEt_( irow__ )::plug( AS );
			Internals.plug( AS );
			Externals.plug( AS );
			Nodes.plug( AS );
			Types.plug( AS );
		}
		const differentiated_binary_tree_ &operator=( const differentiated_binary_tree_ &T )
		{
			E_LISTt_( irow__ )::operator=( T );
			E_BTREEt_( irow__ )::operator =( T );
			Internals = T.Internals;
			Externals = T.Externals;
			Nodes = T.Nodes;
			Types = T.Types;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			E_LISTt_( irow__ )::Init();
			E_BTREEt_( irow__ )::Init();
			Internals.Init();
			Externals.Init();
			Nodes.Init();
			Types.Init();
		}
		//f Create internal node with value 'Internal'. Returns its row.
		irow__ CreateInternal( const int__ &Internal )
		{
			irow__ Row = E_LISTt_( irow__ )::New();

			Types.Store( internal, Row );

			Nodes.Store( *Internals.Add( Internal ), Row );

			return Row;
		}
		//f Create external node with value 'External'. Returns its row.
		irow__ CreateExternal( const ext__ &External )
		{
			irow__ Row = E_LISTt_( irow__ )::New();

			Types.Store( external, Row );

			Nodes.Store( *Externals.Add( External ), Row );

			return Row;
		}
		//f Return true if node 'Node' is an internal one.
		bso::bool__ IsInternal( irow__ Node ) const
		{
			return Types.Get( Node ) == internal;
		}
		//f Return true if node 'Node' is an external one.
		bso::bool__ IsExternal( irow__ Node ) const
		{
			return Types.Get( Node ) == external;
		}
		//f Return the internal row corresponding to 'Node'.
		internal_row__ GetInternalRow( irow__ Node ) const
		{
#ifdef DBT_DBG
			if ( !IsInternal( Node ) )
				qRFwk();
#endif
			return Nodes( Node );
		}
		//f Return the external row corresponding to 'Node'.
		external_row__ GetExternalRow( irow__ Node ) const
		{
#ifdef DBT_DBG
			if ( !IsExternal( Node ) )
				qRFwk();
#endif
			return Nodes( Node );
		}
		//f Return the object storing the internals.
		const lstbch::E_LBUNCHt_( int__, internal_row__ ) &GetInternals( void ) const
		{
			return Internals;
		}
		//f Return the object storing the internals.
		lstbch::E_LBUNCHt_( int__, internal_row__ ) &GetInternals( void )
		{
			return Internals;
		}
		//f Return the object storing the externals.
		const lstbch::E_LBUNCHt_( ext__, external_row__ ) &GetExternals( void ) const
		{
			return Externals;
		}
		//f Return the object storing the externals.
		lstbch::E_LBUNCHt_( ext__, external_row__ ) &GetExternals( void )
		{
			return Externals;
		}
		//f Delete node 'Node'.
		void Delete( irow__ Node )
		{
			if ( IsExternal( Node ) )
				Externals.Delete( GetExternalRow( Node ) );
			else if ( IsInternal( Node ) )
				Internals.Delete( GetInternalRow( Node ) );
			else
				qRFwk();

			E_LISTt_( irow__ )::Delete( Node );
		}
		//f Return the internal corresponding to 'Node'.
		int__ GetInternal( irow__ Node ) const
		{
			return Internals( GetInternalRow( Node ) );
		}
		//f Return the external corresponding to 'Node'.
		ext__ GetExternal( irow__ Node ) const
		{
			return Externals( GetExternalRow( Node ) );
		}
		E_NAVt( E_LISTt_( irow__ )::, irow__ )
	};

	E_AUTO3( differentiated_binary_tree )

	#define E_DBTREEt_( internal, external, row )	differentiated_binary_tree_<internal, external, row>
	#define E_DBTREEt( internal, external, row )	differentiated_binary_tree<internal, external, row>

	#define E_DBTREE_( internal, external )	E_DBTREEt_( internal, external, sdr::row__ )
	#define E_DBTREE( internal, external )	E_DBTREEt( internal, external, sdr::row__ )

	//c differentiated bianry tree filler.
	template <typename int__, typename ext__, typename irow__> class differentiated_binary_tree_filler_
	: public E_BTREE_FILLERt_( irow__ )
	{
	private:
		E_DBTREEt_( int__, ext__, irow__ ) *Tree_;
	public:
		struct s
		: public E_BTREE_FILLERt_( irow__ )::s
		{};
		differentiated_binary_tree_filler_( s &S )
		: E_BTREE_FILLERt_( irow__ )( S )
		{}
		void reset( bso::bool__ P = true )
		{
			Tree_ = NULL;
			E_BTREE_FILLERt_( irow__ )::reset( P );
		}
		void plug( qSD__ &SD )
		{
			E_BTREE_FILLERt_( irow__ )::plug( SD );
		}
		void plug( qAS_ &AS )
		{
			E_BTREE_FILLERt_( irow__ )::plug( AS );
		}
		differentiated_binary_tree_filler_ &operator =( const differentiated_binary_tree_filler_ &DBTF )
		{
			E_BTREE_FILLERt_( irow__ )::operator =( DBTF );

			return *this;
		}
		//f Initialization with tree 'Tree' of root 'Root' (if not empty).
		void Init(
			E_DBTREEt_( int__, ext__, irow__ ) &Tree,
			irow__ Root )
		{
			Tree_ = &Tree;
			E_BTREE_FILLERt_( irow__ )::Init( Tree, Root );
		}
		//f Push external node 'External'. Return row where the new element is added.
		irow__ Push( const ext__ &External )
		{
			irow__ Row = Tree_->CreateExternal( External );

			E_BTREE_FILLERt_( irow__ )::Push( Row );

			return Row;
		}
		//f Join the 2 last pending node with internal node 'Internal'. Return row where the new element is added.
		irow__ Join( const int__ &Internal )
		{
			irow__ Row = Tree_->CreateInternal( Internal );

			E_BTREE_FILLERt_( irow__ )::Join( Row );

			return Row;
		}
	};

	E_AUTO3( differentiated_binary_tree_filler )

	#define E_DBTREE_FILLERt_( internal, external, row )	differentiated_binary_tree_filler_<internal, external, row>
	#define E_DBTREE_FILLERt( internal, external, row )		differentiated_binary_tree_filler<internal, external, row>

	#define E_DBTREE_FILLER_( internal, external )	E_DBTREE_FILLERt_( internal, external, sdr::row__ )
	#define E_DBTREE_FILLER( internal, external )	E_DBTREE_FILLERt( internal, external, sdr::row__ )


}

/*$END$*/
#endif
