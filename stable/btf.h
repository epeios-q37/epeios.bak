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

//	$Id: btf.h,v 1.18 2013/04/15 10:50:47 csimon Exp $

#ifndef BTF__INC
#define BTF__INC

#define BTF_NAME		"BTF"

#define	BTF_VERSION	"$Revision: 1.18 $"

#define BTF_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( BTF_NODBG )
#define BTF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.18 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/15 10:50:47 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Binary Tree Filler 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "btr.h"
#include "stk.h"

namespace btf {

	using stk::bstack_;

	qROW( row__ );

	//c Binary tree_filler.
	template <typename r> class binary_tree_filler_
	: public E_BSTACKt_( r, row__ )
	{
	private:
		btr::E_BTREEt_( r ) *Tree_;
	public:
		struct s
		: E_BSTACKt_( r, row__ )::s
		{};
		binary_tree_filler_( s &S )
		: E_BSTACKt_( r, row__ )( S )
		{}
		void reset( bso::bool__ P = true )
		{
			Tree_ = NULL;
			E_BSTACKt_( r, row__ )::reset( P );
		}
		void plug( qSD__ &SD )
		{
			E_BSTACKt_( r, row__ )::plug( SD );
		}
		void plug( qAS_ &AS )
		{
			E_BSTACKt_( r, row__ )::plug( AS );
		}
		binary_tree_filler_ &operator =( const binary_tree_filler_ &BTF )
		{
			E_BSTACKt_( r, row__ )::operator =( BTF );

			return *this;
		}
		//f Initialization with binary tree 'Tree' of root (if not empty) 'Root'.
		void Init(
			btr::E_BTREEt_( r ) &Tree,
			r Root )
		{
			Tree_ = &Tree;
			E_BSTACKt_( r, row__ )::Init();

			if ( Root != qNIL )
				E_BSTACKt_( r, row__ )::Push( Root );
		}
		//f Join the 2 nodes at bottom of stack with 'Node', which is pushed in stack.
		void Join( r Node )
		{
#ifdef BTF_DBG
			if ( E_BSTACKt_( r, row__ )::Amount() < 2 )
				qRFwk();
#endif
			Tree_->BecomeRight( E_BSTACKt_( r, row__ )::Pop(), Node );
			Tree_->BecomeLeft( E_BSTACKt_( r, row__ )::Pop(), Node );

			E_BSTACKt_( r, row__ )::Push( Node );
		}
		//f Return true if the tree is complete.
		bso::bool__ IsComplete( void ) const
		{
			return E_BSTACKt_( r, row__ )::Amount() <= 1;
		}
		//f Return the root of the tree. Significant only if 'IsComplete()' return true.
		r GetRoot( void ) const
		{
			row__ Row = E_BSTACKt_( r, row__ )::First();
#ifdef BTF_DBG
			if ( !IsComplete() )
				qRFwk();
#endif
			if ( Row != qNIL )
				return E_BSTACKt_( r, row__ )::Get( Row );
			else
				return qNIL;
		}
		//f Return true if a call to 'Join' is possible, false otherwise.
		bso::bool__ IsJoinable( void ) const
		{
			return E_BSTACKt_( r, row__ )::Amount() >= 2;
		}
		//f Only for the 'NAVt()' macro from 'TOL' library. Don't use.
		bso::bool__ Exists( row__ ) const
		{
			qRFwk();

			return false;	// Only to avoid a warning.
		}
	};

	E_AUTO1( binary_tree_filler )

	#define E_BTREE_FILLERt_( t )	binary_tree_filler_< t >
	#define E_BTREE_FILLERt( t )	binary_tree_filler< t >

	#define E_BTREE_FILLER_	E_BTREE_FILLERt_( epeios::row__ )
	#define E_BTREE_FILLER	E_BTREE_FILLERt( epeios::row__ )

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
