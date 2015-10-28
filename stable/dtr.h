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

//	$Id: dtr.h,v 1.27 2013/07/25 15:59:14 csimon Exp $

#ifndef DTR__INC
#define DTR__INC

#define DTR_NAME		"DTR"

#define	DTR_VERSION	"$Revision: 1.27 $"

#define DTR_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( DTR_NODBG )
#define DTR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.27 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/07/25 15:59:14 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D Dynamic TRee 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "que.h"
#include "btr.h"


namespace dtr {
	//e Browse status;
	enum kinship__ {
		kChild,
		kSibling,
		kParent,
		k_amount,
		k_Undefined,
	};
	
	template <typename r> class dynamic_tree_;	// Predeclaration.


	//c Browse structure.
	template <typename r> struct browser__
	{
	private:
		kinship__ _Kinship;
		r _Position;
		r _Root;
		bso::bool__ _GotoParent;	// Indique qu'il faut directement sauter au parent.
	public:
		browser__( void )
		{
			_Kinship = k_Undefined;
			_Position = qNIL;
			_Root = qNIL;
			_GotoParent = false;
		}
		//f Initialization wih 'Root' as root.
		void Init( r Root )
		{
			_Kinship = k_Undefined;
			_Position = _Root = Root;
			_GotoParent = false;
		}
		void GotoParent( void )
		{
			_GotoParent = true;
		}
		E_RODISCLOSE__( kinship__, Kinship )
		E_RODISCLOSE__( r, Position )
		E_RODISCLOSE__( r, Root )
		friend class dynamic_tree_<r>;
	};

# define E_BROWSER__( t )	browser__<t>
	//c A dynamic tree.
	template <typename r> class dynamic_tree_
	{
	public:
		//r A binary tree.
		btr::E_BTREEt_( r ) Tree;
		//r A queue
		que::E_QUEUEt_( r ) Queue;
		struct s 
		{
			typename btr::E_BTREEt_( r )::s Tree;
			typename que::E_QUEUEt_( r )::s Queue;
		};
		dynamic_tree_( s &S )
		: Tree( S.Tree ),
		  Queue( S.Queue )
		{}
		void reset( bso::bool__ P = true )
		{
			Tree.reset( P );
			Queue.reset( P );
		}
		void plug( qAS_ &AS )
		{
			Tree.plug( AS );
			Queue.plug( AS );
		}
		dynamic_tree_ &operator =( const dynamic_tree_ &T )
		{
			Tree = T.Tree;
			Queue = T.Queue;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Tree.Init();
			Queue.Init();
		}
		//f Allocate place nedeed for 'Size' nodes.
		void Allocate(
			sdr::size__ Size,
			aem::mode__ Mode = aem::m_Default )
		{
			Tree.Allocate( Size, Mode );
			Queue.Allocate( Size, Mode );
		}
		//f 'First' becomes first of 'Node'.
		void BecomeFirstChild( 
			r First,
			r Node )
		{
			if ( Tree.Left( Node ) == qNIL )
				Tree.BecomeOverridingRight( First, Node );
			else
				Queue.BecomePrevious( First, Tree.Left( Node ) );

			Tree.BecomeOverridingLeft( First, Node );
		}
		//f 'Last' becomes last of 'Node'.
		void BecomeLastChild( 
			r Last,
			r Node )
		{
			if ( Tree.Right( Node ) == qNIL )
				Tree.BecomeOverridingLeft( Last, Node );
			else
				Queue.BecomeNext( Last, Tree.Right( Node ) );

			Tree.BecomeOverridingRight( Last, Node );
		}
		//f 'Previous' becomes node previous to 'Node'.
		void BecomePreviousSibling(
			r Previous,
			r Node )
		{
			r Parent;

			Queue.BecomePrevious( Previous, Node );

			if ( ( Parent = Tree.Parent( Node ) ) != qNIL ) {
				if ( Tree.Left( Parent ) == Node )
					Tree.BecomeLeft( Previous, Parent );
				Tree.ForceParent( Previous, Parent );
			}
		}
		//f 'Next' becomes node next to 'Node'.
		void BecomeNextSibling(
			r Next,
			r Node )
		{
			r Parent;

			Queue.BecomeNext( Next, Node );

			if ( ( Parent = Tree.Parent( Node ) ) != qNIL ) {
				if ( Tree.Right( Parent ) == Node )
					Tree.BecomeRight( Next, Parent );
				Tree.ForceParent( Next, Parent );
			}
		}
	#if 0
		// Swap 'Node1' and 'Node2'. Both nodes must be from the same queue.
		void Swap(
			r Node1,
			r Node2 )
		{
			r Parent;

			Queue.Swap( Node1, Node2 );

			if ( ( Parent = Tree.Parent( Node1 ) ) != qNIL ) {
				if ( Tree.Left( Parent ) == Node1 )
					Tree.BecomeLeft( Node2, Parent );
				else if ( Tree.Left( Parent ) == Node2 )
					Tree.BecomeLeft( Node1, Parent );

				if ( Tree.Right( Parent ) == Node1 )
					Tree.BecomeRight( Node2, Parent );
				else if ( Tree.Right( Parent ) == Node2 )
					Tree.BecomeRight( Node1, Parent );
			}
		}
	#endif
		//f Return first node of 'Node'.
		r FirstChild( r Node ) const
		{
			return Tree.Left( Node );
		}
		//f Return last node of 'Node'.
		r LastChild( r Node ) const
		{
			return Tree.Right( Node );
		}
		bso::bool__ HasChild( r Node ) const
		{
			return FirstChild( Node ) != qNIL;
		}
		//f Return node previous to 'Node'.
		r PreviousSibling( r Node ) const
		{
			return Queue.Previous( Node );
		}
		bso::bool__ HasPreviousSibling( r Node ) const
		{
			return PreviousSibling( Node ) != qNIL;
		}
		//f Return node next to 'Node'.
		r NextSibling( r Node ) const
		{
			return Queue.Next( Node );
		}
		bso::bool__ HasNextSibling( r Node ) const
		{
			return NextSibling( Node ) != qNIL;
		}
		//f Return parent of 'Node'.
		r Parent( r Node ) const
		{
			return Tree.Parent( Node );
		}
		r Cut( r Node )
		{
			r P = Tree.Parent( Node );
			r L = Queue.Previous( Node ), R = Queue.Next( Node );

			Tree.Cut( Node );

			if ( P != qNIL ) {
				if ( L == qNIL ) {
					if ( R != qNIL )
						Tree.BecomeLeft( R, P );
				}

				if ( R == qNIL ) {
					if ( L != qNIL )
						Tree.BecomeRight( L, P );
				}
			}

			Queue.Delete( Node );

			return Node;
		}
		bso::bool__ HasParent( r Node ) const
		{
			return Parent( Node ) != qNIL;
		}
		//f Return amount of nodes.
		sdr::size__ Amount( void ) const
		{
			return Tree.Amount();
		}
		/*f Return the first, then next node, or 'qNIL' if no more,
		using 'BrowseStruct'. */
		r Browse( browser__<r> &Browser ) const
		{
			kinship__ &Kinship = Browser._Kinship;
			r &Position = Browser._Position, &Root = Browser._Root;

			switch ( Kinship ) {
			case k_Undefined:
				if ( Root == qNIL )
					qRFwk();

				if ( HasChild( Position ) ) {
					Position = FirstChild( Position );
					Kinship = kChild;
				} else if ( Position == Root )
					Position = qNIL;
				break;
			case kChild:
			case kSibling:
				if ( !Browser._GotoParent && HasChild( Position ) ) {
					Position = FirstChild( Position );
					Kinship = kChild;
				} else if ( !Browser._GotoParent && HasNextSibling( Position ) ) {
					Position = NextSibling( Position );
					Kinship = kSibling;
#ifdef DTR_DBG
				} else if ( !HasParent( Position ) ) {
					qRFwk();
#endif
				} else {
					Position = Parent( Position );
					Kinship = kParent;
				}
				Browser._GotoParent = false;
				break;
			case kParent:
				if ( Position == Root ) {
					Position = qNIL;
					Kinship = k_Undefined;
				} else if ( HasNextSibling( Position ) ) {
					Position = NextSibling( Position );
					Kinship = kSibling;
#ifdef DTR_DBG
				} else if ( !HasParent( Position ) ) {
					qRFwk();
#endif
				} else {
					Position = Parent( Position );
					Kinship = kParent;
				}
				break;
			default:
				qRFwk();
				break;
			}

			return Position;
		}
		bso::uint__ GetLevel( r Row ) const
		{
			bso::uint__ Level = 0;

			while ( HasParent( Row ) ) {
				if ( Level == BSO_UINT_MAX )
					qRLmt();

				Level++;
				Row = Parent( Row );
			}

			return Level;
		}
	};

	E_AUTO1( dynamic_tree )

	#define E_DTREEt( r )	dynamic_tree< r >
	#define E_DTREEt_( r )	dynamic_tree_< r >

	#define E_DTREE		E_DTREEt( epeios::row__ )
	#define E_DTREE_	E_DTREEt_( epeios::row__ )
	
}

/*$END$*/
#endif
