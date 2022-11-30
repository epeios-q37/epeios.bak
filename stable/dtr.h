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

// Dynamic TRee

#ifndef DTR_INC_
# define DTR_INC_

# define DTR_NAME		"DTR"

# if defined( E_DEBUG ) && !defined( DTR_NODBG )
#  define DTR_DBG
# endif

# include "err.h"
# include "flw.h"
# include "que.h"
# include "btr.h"

/***************/
/***** OLD *****/
/***************/

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

	qHOOKS2( btr::sHook, Tree, que::sHook, Queue );

# define E_BROWSER__( t )	browser__<t>

	template <typename r> class dynamic_tree_
	{
  private:
		btr::E_BTREEt_( r ) Tree_;
		que::E_QUEUEt_( r ) Queue_;
  public:
		struct s
		{
			typename btr::E_BTREEt_( r )::s Tree;
			typename que::E_QUEUEt_( r )::s Queue;
		};
		dynamic_tree_( s &S )
		: Tree_( S.Tree ),
		  Queue_( S.Queue )
		{}
		void reset( bso::bool__ P = true )
		{
			Tree_.reset( P );
			Queue_.reset( P );
		}
		void plug( sHooks &Hooks )
		{
			Tree_.plug(Hooks.Tree_);
			Queue_.plug(Hooks.Queue_);
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, Tree_, Queue_ );
		}
		dynamic_tree_ &operator =( const dynamic_tree_ &T )
		{
			Tree_ = T.Tree_;
			Queue_ = T.Queue_;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Tree_.Init();
			Queue_.Init();
		}
		//f Allocate place nedeed for 'Size' nodes.
		void Allocate(
			sdr::size__ Size,
			aem::mode__ Mode = aem::m_Default )
		{
			Tree_.Allocate( Size, Mode );
			Queue_.Allocate( Size, Mode );
		}
		//f 'First' becomes first of 'Node'.
		void BecomeFirstChild(
			r First,
			r Node )
		{
			if ( Tree_.Left( Node ) == qNIL )
				Tree_.BecomeOverridingRight( First, Node );
			else
				Queue_.BecomePrevious( First, Tree_.Left( Node ) );

			Tree_.BecomeOverridingLeft( First, Node );
		}
		void BecomeFirst(
			r First,
			r Node )
		{
		  return BecomeFirstChild(First, Node);
		}
		//f 'Last' becomes last of 'Node'.
		void BecomeLastChild(
			r Last,
			r Node )
		{
			if ( Tree_.Right( Node ) == qNIL )
				Tree_.BecomeOverridingLeft( Last, Node );
			else
				Queue_.BecomeNext( Last, Tree_.Right( Node ) );

			Tree_.BecomeOverridingRight( Last, Node );
		}
		void BecomeLast(
			r Last,
			r Node )
		{
		  return BecomeLastChild(Last, Node);
		}
		//f 'Previous' becomes node previous to 'Node'.
		void BecomePreviousSibling(
			r Previous,
			r Node )
		{
			r Parent;

			Queue_.BecomePrevious( Previous, Node );

			if ( ( Parent = Tree_.Parent( Node ) ) != qNIL ) {
				if ( Tree_.Left( Parent ) == Node )
					Tree_.BecomeLeft( Previous, Parent );
				Tree_.ForceParent( Previous, Parent );
			}
		}
		void BecomePrevious(
			r Previous,
			r Node )
		{
		  return BecomePreviousSibling(Previous, Node);
		}
		//f 'Next' becomes node next to 'Node'.
		void BecomeNextSibling(
			r Next,
			r Node )
		{
			r Parent;

			Queue_.BecomeNext( Next, Node );

			if ( ( Parent = Tree_.Parent( Node ) ) != qNIL ) {
				if ( Tree_.Right( Parent ) == Node )
					Tree_.BecomeRight( Next, Parent );
				Tree_.ForceParent( Next, Parent );
			}
		}
		void BecomeNext(
			r Next,
			r Node )
		{
		  return BecomeNextSibling(Next, Node);
		}
# if 0
		// Swap 'Node1' and 'Node2'. Both nodes must be from the same queue.
		void Swap(
			r Node1,
			r Node2 )
		{
			r Parent;

			Queue_.Swap( Node1, Node2 );

			if ( ( Parent = Tree_.Parent( Node1 ) ) != qNIL ) {
				if ( Tree_.Left( Parent ) == Node1 )
					Tree_.BecomeLeft( Node2, Parent );
				else if ( Tree_.Left( Parent ) == Node2 )
					Tree_.BecomeLeft( Node1, Parent );

				if ( Tree_.Right( Parent ) == Node1 )
					Tree_.BecomeRight( Node2, Parent );
				else if ( Tree_.Right( Parent ) == Node2 )
					Tree_.BecomeRight( Node1, Parent );
			}
		}
# endif
		//f Return first node of 'Node'.
		r FirstChild( r Node ) const
		{
			return Tree_.Left( Node );
		}
		r First( r Node ) const
		{
			return FirstChild(Node);
		}
		//f Return last node of 'Node'.
		r LastChild( r Node ) const
		{
			return Tree_.Right( Node );
		}
		r Last( r Node ) const
		{
			return LastChild(Node);
		}
		bso::bool__ HasChild( r Node ) const
		{
			return FirstChild( Node ) != qNIL;
		}
		//f Return node previous to 'Node'.
		r PreviousSibling( r Node ) const
		{
			return Queue_.Previous( Node );
		}
		r Previous( r Node ) const
		{
			return PreviousSibling(Node);
		}
		bso::bool__ HasPreviousSibling( r Node ) const
		{
			return PreviousSibling( Node ) != qNIL;
		}
		//f Return node next to 'Node'.
		r NextSibling( r Node ) const
		{
			return Queue_.Next( Node );
		}
		r Next( r Node ) const
		{
			return NextSibling(Node);
		}
		bso::bool__ HasNextSibling( r Node ) const
		{
			return NextSibling( Node ) != qNIL;
		}
		//f Return parent of 'Node'.
		r Parent( r Node ) const
		{
			return Tree_.Parent( Node );
		}
		r Cut( r Node )
		{
			r P = Tree_.Parent( Node );
			r L = Queue_.Previous( Node ), R = Queue_.Next( Node );

			Tree_.Cut( Node );

			if ( P != qNIL ) {
				if ( L == qNIL ) {
					if ( R != qNIL )
						Tree_.BecomeLeft( R, P );
				}

				if ( R == qNIL ) {
					if ( L != qNIL )
						Tree_.BecomeRight( L, P );
				}
			}

			Queue_.Delete( Node );

			return Node;
		}
		bso::bool__ HasParent( r Node ) const
		{
			return Parent( Node ) != qNIL;
		}
		//f Return amount of nodes.
		sdr::size__ Amount( void ) const
		{
			return Tree_.Amount();
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
# ifdef DTR_DBG
				} else if ( !HasParent( Position ) ) {
					qRFwk();
# endif
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
# ifdef DTR_DBG
				} else if ( !HasParent( Position ) ) {
					qRFwk();
# endif
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

# define E_DTREEt( r )	dynamic_tree< r >
# define E_DTREEt_( r )	dynamic_tree_< r >

# define E_DTREE		E_DTREEt( epeios::row__ )
# define E_DTREE_	E_DTREEt_( epeios::row__ )

	template <typename tree, typename queue> class rH_
	: public sHooks
	{
	protected:
		tree Tree_;
		queue Queue_;
	public:
		rH_( void )
		: sHooks( Tree_, Queue_ )
		{}
	};

	class rRH
	: public rH_<btr::rRH, que::rRH>
	{
	public:
		void reset( bso::sBool P = true )
		{
			Tree_.reset( P );
			Queue_.reset( P );
		}
		qCVDTOR( rRH );
		void Init( void )
		{
			Tree_.Init();
			Queue_.Init();
		}
	};

	struct rHF
	{
	public:
		btr::rHF Tree;
		que::rHF Queue;
		void reset( bso::bool__ P = true )
		{
			Tree.reset( P );
			Queue.reset( P );
		}
		E_CDTOR( rHF );
		void Init(
			const fnm::name___ &Path,
			const fnm::name___ &Basename );
	};

	class rFH
	: public rH_<btr::rFH, que::rFH>
	{
	public:
		void reset( bso::sBool P = true )
		{
			Tree_.reset( P );
			Queue_.reset( P );
		}
		qCVDTOR( rFH );
		uys::eState Init(
			const rHF &Filenames,
			uys::mode__ Mode,
			sdr::eType Type,
			flsq::id__ ID )
		{
			reset();

			uys::eState State = Tree_.Init( Filenames.Tree, Mode,Type, ID );

			if ( !State.IsError() ) {
				if ( Queue_.Init( Filenames.Queue, Mode, Type, ID ) != State )
					State = uys::sInconsistent;
			}

			return State;
		}
		/*
		uys::state__ Settle( void )
		{
			uys::state__ State = Tree_.Settle();

			if ( State.IsError() )
				return State;

			if ( Queue_.Settle() != State )
				State = uys::sInconsistent;

			return State;
		}
		void Drop( void )
		{
			Tree_.Drop();
			Queue_.Drop();
		}
		uys::state__ State( void ) const
		{
			uys::state__ State = Tree_.State();

			if ( State != Queue_.State() )
				State = uys::sInconsistent;

			return State;
		}
# ifdef CPE_C_MSC
#   undef CreateFile
# endif
		bso::bool__ CreateFiles( err::handling__ ErrorHandling = err::h_Default )
		{
			bso::bool__ Success =Tree_.CreateFiles( ErrorHandling );

			if ( !Success )
				return false;

			if ( !Queue_.CreateFiles( ErrorHandling ) ) {
				Tree_.Drop();
				Success = false;
			}

			return Success;
		}
		time_t TimeStamp( void ) const
		{
			time_t TreeTimeStamp = Tree_.TimeStamp();
			time_t QueueTimeStamp = Queue_.TimeStamp();

			if ( TreeTimeStamp > QueueTimeStamp )
				return TreeTimeStamp;
			else
				return QueueTimeStamp;
		}
		void ReleaseFile( void )
		{
			Tree_.ReleaseFile();
			Queue_.ReleaseFile();
		}
		void Mode( uys::mode__ Mode )
		{
			Tree_.Mode( Mode );
			Queue_.Mode( Mode );
		}
		bso::bool__ IsPersistent( void ) const
		{
			bso::bool__ Is = Tree_.IsPersistent();

			if ( Is != Queue_.IsPersistent() )
				qRFwk();

			return Is;
		}
		template <typename dynamic_tree> friend uys::state__ Plug(
			dynamic_tree &DynamicTree,
			fh___ &Hook );
		*/
	};
}

/***************/
/***** NEW *****/
/***************/

# define qDTREEd( row ) E_DTREEt_( row )
# define qDTREEw( row ) E_DTREEt( row )

# define qDTREEdl E_DTREE_
# define qDTREEwl E_DTREEt

# define qBROWSERs( row ) E_BROWSER__( row )


#endif
