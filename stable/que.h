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

// QUEue 

#ifndef QUE__INC
# define QUE__INC

# define QUE_NAME		"QUE"

# define	QUE_VERSION	"$Revision: 1.44 $"

# define QUE_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

# if defined( E_DEBUG ) && !defined( QUE_NODBG )
#  define QUE_DBG
# endif

# include "err.h"
# include "flw.h"
# include "tys.h"
# include "bch.h"
# include "stkbch.h"

namespace que {
	//e dump direction.	
	enum direction
	{
		//i Unknow direction.
		dUnknow,
		//i Ascending.
		dAscending,
		//i Descending.
		dDescending,
		//i amount of directyon item.
		d_amount
	};
	
	// The link between queue nodes.
	struct link__
	{
	public:
		// Previous node.
		sdr::row_t__ Previous;
		// Next node.
		sdr::row_t__ Next;
		link__( void )
		{
			Previous = Next = qNIL;
		}
		// Return 'true' if node has next node, false otherwise.
		bso::bool__ HasNext( void ) const
		{
			return Next != qNIL;
		}
		// Return 'true' if node has previous node, false otherwise.
		bso::bool__ HasPrevious( void ) const
		{
			return Previous != qNIL;
		}
		// To help swapping. Replace all reference to 'Node1' by 'Node2'.
		void Replace(
			sdr::row_t__ Node1,
			sdr::row_t__ Node2 )
		{
			if ( Previous == Node1 )
				Previous = Node2;
			else if ( Next == Node1 )
				Next = Node2;
		}
	};

	using namespace aem;

	typedef bch::E_BUNCHt_( que::link__, sdr::row__ ) _lbunch_;

	//c A set lof links.
	class links_
	: public _lbunch_
	{
	public:
		struct s
		: _lbunch_::s
		{};
		links_( s &S )
		: _lbunch_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			_lbunch_::reset( P );
		}
		links_ &operator =( const links_ &QL )
		{
			_lbunch_::operator =( QL );
			return *this;
		}
		//f Previous of 'Item' is set to 'Value'. Next remains unchanged.
		void SetPrevious(
			sdr::row_t__ Item,
			sdr::row_t__ Value )
		{
			que::link__ L = Get( Item );

			L.Previous = Value;

			Store( L, Item );
		}
		//f Next of 'Item' is set to 'Value'. Previous remains unchanged.
		void SetNext(
			sdr::row_t__ Item,
			sdr::row_t__ Value )
		{
			que::link__ L = Get( Item );

			L.Next = Value;

			Store( L, Item );
		}
		//f Initialization.
		void Init( void )
		{
			_lbunch_::Init();
		}
		void Initialize(
			sdr::row_t__ Begin,
			sdr::row_t__ End );
	};
	
	E_AUTO( links )

/*
	E_TYPEDEF( sdr::row_t__, _stack_item_row__ );

	// simulates :
	// 	template<typename r> typedef stk::stack_<r, _stack_item_row__> _stack_;
	template <typename r> class _stack_
	: public stk::E_BSTACK_( r )
	{
	public:
		struct s
		stk::E_BSTACK_( r )::s
		{};
		_stack_( s &S )
		: stk::E_BSTACK_( r )( S )
		{}
	};

	E_AUTO1( _stack )
*/
# define qQSTACKd( r ) stkbch::qBSTACKdl( r )
# define qQSTACKw( r ) stkbch::qBSTACKwl( r )

	using bch::sHook;

	//c A queue. Use 'QUEUE_' rather than directly this.
	template <typename r> class queue_
	{
	private:
		void HandleNeighboursForSwap_(
			que::link__ Link,
			sdr::row_t__ Node )
		{
			link__ L;

			if ( Link.HasPrevious() ) {
				L = Links( Link.Previous );
				L.Next = Node;
				Links.Store( L, Link.Previous );
			}

			if ( Link.HasNext() ) {
				L = Links( Link.Next );
				L.Previous = Node;
				Links.Store( L, Link.Next );
			}
		}
# ifdef QUE_DBG
		void TestIfItem_( sdr::row_t__ Item )
		{
			link__ L = Links( Item );

			if ( ( L.Previous != qNIL ) || ( L.Next != qNIL ) )
				qRFwk();
		}
# endif
	public:
		//r Links between nodes.
		que::links_ Links;
		struct s
		{
			que::links_::s Links;
		};
		queue_( s &S )
		: Links( S.Links )
		{}
		void reset( bso::bool__ P = true )
		{
			Links.reset( P );
		}
		void plug( sHook &Hook )
		{
			Links.plug( Hook );
		}
		void plug( qASd *AS )
		{
			Links.plug( AS );
		}
		queue_ &operator =( const queue_ &Q )
		{
			Links = Q.Links;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Links.Init();
		}
		//f Allocate enough room to contains 'Size' nodes.
		void Allocate(
			sdr::size__ Size,
			aem::mode__ Mode = aem::m_Default )
		{
			sdr::size__ Before = Links.Amount();
			Links.Allocate( Size, Mode );

			if ( Before < Size )
				Links.Initialize( Before, Size - 1 );
		}
		//f Return node next to 'Node'.
		r Next( r Node ) const
		{
			return Links( *Node ).Next;
		}
		//f Return node previous to 'Node'.
		r Previous( r Node ) const
		{
			return Links( *Node ).Previous;
		}
		//f Return true if 'Node' has next node.
		bso::bool__ HasNext( r Node ) const
		{
			return Links( *Node ).HasNext();
		}
		//f Return true if 'Node' has previous node.
		bso::bool__ HasPrevious( r Node ) const
		{
			return Links( *Node ).HasPrevious();
		}
		//f Insert 'Item' after 'Node'.
		void BecomeNext(
			r Item,
			r Node )
		{
# ifdef QUE_DBG
			TestIfItem_( *Item );
# endif
			link__ LNode = Links( *Node );
			link__ LItem = Links( *Item );

			if ( LNode.HasNext() )
				Links.SetPrevious( LNode.Next, *Item );

			LItem.Next = LNode.Next;
			LItem.Previous = *Node;
			LNode.Next = *Item;

			Links.Store( LNode, *Node );
			Links.Store( LItem, *Item );
		}
		//f Insert 'Item' before 'Node'.
		void BecomePrevious(
			r Item,
			r Node )
		{
# ifdef QUE_DBG
			TestIfItem_( *Item );
# endif
			link__ LNode = Links( *Node );
			link__ LItem = Links( *Item );

			if ( LNode.HasPrevious() )
				Links.SetNext( LNode.Previous, *Item );

			LItem.Previous = LNode.Previous;
			LItem.Next = *Node;
			LNode.Previous = *Item;

			Links.Store( LNode, *Node );
			Links.Store( LItem, *Item );
		}
		//f Delete node 'Node'.
		void Delete( r Node )
		{
			link__ LNode = Links( *Node );

			if ( LNode.HasNext() )
				Links.SetPrevious( LNode.Next, LNode.Previous );

			if ( LNode.HasPrevious() )
				Links.SetNext( LNode.Previous, LNode.Next );

			LNode.Next = LNode.Previous = qNIL;

			Links.Store( LNode, *Node );
		}
		//f Return the extent of the queue.
		sdr::size__ Extent( void ) const
		{
			return Links.Extent();
		}
		//f Return the amount of node in the queue.
		sdr::size__ Amount( void ) const
		{
			return Links.Amount();
		}
		//f Swap 'Node1' with 'Node2'.
		void Swap(
			r Node1,
			r Node2 )
		{
			link__ LNode1 = Links.Get( *Node1 );
			link__ LNode2 = Links.Get( *Node2 );

			HandleNeighboursForSwap_( LNode1, *Node2 );
			HandleNeighboursForSwap_( LNode2, *Node1 );

			LNode1.Replace( *Node2, *Node1 );
			LNode2.Replace( *Node1, *Node2 );

			Links.Store( LNode1, *Node2 );
			Links.Store( LNode2, *Node1 );
		}
		//f Return true if 'Node' exists, false oterwise.
		bso::bool__ Exists( r Node ) const
		{
			return Links.Exists( *Node );
		}
		void Dump(
			qQSTACKd( r ) &Stack,
			r Begin,
			que::direction Direction ) const;
		r Cut( r Node )
		{
			r P = Previous( Node );

			link__ L = Links.Get( *Node );

			L.Previous = qNIL;

			Links.Store( L, *Node );

			if ( P != qNIL ) {
				L = Links.Get( *P );

				L.Next = qNIL;

				Links.Store( L, *P );
			}

			return Node;
		}
	};

	E_AUTO1( queue );

# ifndef FLS__COMPILATION
	using bch::rRH;

	using bch::rHF;
	using bch::rFH;
# endif

	//d A queue.
# define E_QUEUEt_( r )		queue_< r >
# define E_QUEUEt(r )		queue< r >

# define E_QUEUE_	E_QUEUEt_( sdr::row__ )
# define E_QUEUE		E_QUEUEt( sdr::row__ )

	void Dump_(
		const E_QUEUE_ &Queue,
		qQSTACKd( sdr::row__ ) &Stack,
		sdr::row__ Begin,
		direction Direction );

	template <typename r> inline void queue_<r>::Dump(
		qQSTACKd( r ) &Stack,
		r Begin,
		que::direction Direction ) const
	{
		Dump_( *(E_QUEUE_ *)this, *(qQSTACKd( sdr::row__)*)&Stack, Begin, Direction );
	}


	//c A queue manager. The managed queue must be provided at each call ('Queue' parameter).
	template <typename r> class queue_manager__
	{
	private:
		r Head_;
		r Tail_;
		sdr::size__ Amount_;
# ifdef QUE_DBG
		void Test_( void ) const
		{
			if ( ( Tail_ == qNIL ) ||( Head_ == qNIL ) )
				qRFwk();
		}
# endif
	public:
		void reset( bso::bool__ = true )
		{
			Head_ = Tail_ = qNIL;
			Amount_ = 0;
		}
		queue_manager__( void )
		{
			reset( false );
		}
		queue_manager__ &operator =( const queue_manager__ &Q )
		{
			Head_ = Q.Head_;
			Tail_ = Q.Tail_;
			Amount_ = Q.Amount_;

			return *this;
		}
		//f Initialization with queue 'Queue'.
		void Init( que::E_QUEUEt_( r ) & )
		{
			Head_ = Tail_ = qNIL;
			Amount_ = 0;
		}
		//f Create the queue with item 'Item'.
		void Create(
			r Item,
			que::E_QUEUEt_(r) & )
		{
# ifdef QUE_DBG
			if ( ( Head_ != qNIL ) || ( Tail_ != qNIL ) )
				qRFwk();
# endif
			Head_ = Tail_ = Item;

			Amount_ = 1;
		}
		//f Insert 'Item' after 'Node'.
		void BecomeNext(
			r Item,
			r Node,
			que::E_QUEUEt_(r) &Queue )
		{
# ifdef QUE_DBG
			Test_();
# endif
			Queue.BecomeNext( Item, Node );

			if ( Node == Tail_ )
				Tail_ = Item;

			Amount_++;
		}
		//f Insert 'Item' Before 'Node'.
		void BecomePrevious(
			r Item,
			r Node ,
			que::E_QUEUEt_(r) &Queue )
		{
	#ifdef QUE_DBG
			Test_();
	#endif
			Queue.BecomePrevious( Item, Node );

			if ( Node == Head_ )
				Head_ = Item;

			Amount_++;
		}
		//f Delete 'Node'.
		void Delete(
			r Node,
			que::E_QUEUEt_(r) &Queue  )
		{
	#ifdef QUE_DBG
			Test_();
	#endif
			if ( Tail_ == Node )
				Tail_ = Queue.Previous( Node );

			if ( Head_ == Node )
				Head_ = Queue.Next( Node );

			Queue.Delete( Node );

			Amount_--;
		}
		//f Return amount of node in the queue.
		sdr::size__ Amount( const que::E_QUEUEt_(r) & ) const
		{
			return Amount_;
		}
		//f Return 'true' if 'Node' exists, false otherwise.
		bso::bool__ Exists(
			r Node,
			const que::E_QUEUEt_(r) &Queue ) const
		{
			return Queue.Exists( Node );
		}
		bso::bool__ IsMember(
			r Node,
			const que::E_QUEUEt_(r) &Queue ) const
		{
			return Queue.HasPrevious( Node ) || Queue.HasNext( Node ) || ( Head_ == Node );
		}
		//f return 'true' if empty, false otherwise.
		bso::bool__ IsEmpty( void ) const
		{
			return !Amount_;
		}
		//f Return the head.
		r Head( const que::E_QUEUEt_(r) & ) const
		{
			return Head_;
		}
		//f Return first node.
		r First( const que::E_QUEUEt_(r) & ) const
		{
			return Head_;
		}
		//f Return the tail.
		r Tail( const que::E_QUEUEt_(r) & ) const
		{
			return Tail_;
		}
		//f Return last node.
		r Last( const que::E_QUEUEt_(r) & ) const
		{
			return Tail_;
		}
		//f Return node next to 'Node'.
		r Next(
			r Node,
			const que::E_QUEUEt_(r) &Queue )
		{
	#ifdef QUE_DBG
			Test_();

			if ( Node == qNIL )
				qRFwk();
	#endif
			return Queue.Next( Node );
		}
		//f Return node previous to 'Node'.
		r Previous(
			r Node,
			const que::E_QUEUEt_(r) &Queue ) const
		{
	#ifdef QUE_DBG
			Test_();

			if ( Node == qNIL )
				qRFwk();
	#endif
			return Queue.Previous( Node );
		}
		//f Swap 'Node1' and 'Node2'.
		void Swap(
			r Node1,
			r Node2,
			que::E_QUEUEt_(r) &Queue  )
		{
	#ifdef QUE_DBG
			Test_();
	#endif
			if ( Tail_ == Node1 )
				Tail_ = Node2;
			else if ( Tail_ == Node2 )
				Tail_ = Node1;

			if ( Head_ == Node1 )
				Head_ = Node2;
			else if ( Head_ == Node2 )
				Head_ = Node1;

			Queue.Swap( Node1, Node2 );
		}
	};
			
	//c A managed queue (with head and tail). Use 'MQUEUE_' rather then directly this class.
	template <typename r> class managed_queue_
	{
	public:
		E_QUEUEt_( r ) Queue;
		struct s {
			typename E_QUEUEt_( r )::s Queue;
			queue_manager__<r> QueueManager;
		} &S_;
		void reset( bso::bool__ P = true )
		{
			Queue.reset( P );
			S_.QueueManager.reset( P );
		}
		managed_queue_( s &S )
		: Queue( S.Queue ),
		  S_( S )
		{}
		void plug( sHook &Hook )
		{
			return Queue.plug( Hook );
		}
		void plug( qASd *AS )
		{
			return Queue.plug( AS );
		}
		managed_queue_ &operator =( const managed_queue_ &Q )
		{
			Queue = Q.Queue;
			S_.QueueManager = Q.S_.QueueManager;

			return *this;
		}
		//f Initialization with queue 'Queue'.
		void Init( void )
		{
			Queue.Init();
			S_.QueueManager.Init( Queue );
		}
		//f Allocate enough room to contains 'Size' nodes.
		void Allocate(
			sdr::size__ Size,
			aem::mode__ Mode = aem::m_Default )
		{
			Queue.Allocate( Size, Mode );
		}
		//f Create the queue with item 'Item'.
		void Create( r Item )
		{
			S_.QueueManager.Create( Item, Queue );
		}
		//f Insert 'Item' after 'Node'.
		void BecomeNext(
			r Item,
			r Node )
		{
			S_.QueueManager.BecomeNext( Item, Node, Queue );
		}
		//f Insert 'Item' Before 'Node'.
		void BecomePrevious(
			r Item,
			r Node )
		{
			S_.QueueManager.BecomePrevious( Item, Node, Queue );
		}
		//f Delete 'Node'.
		void Delete( r Node )
		{
			S_.QueueManager.Delete( Node, Queue );
		}
		//f Return amount of node in the queue.
		sdr::size__ Amount( void ) const
		{
			return S_.QueueManager.Amount( Queue );
		}
		//f Return extent of node in the queue.
		sdr::size__ Extent( void ) const
		{
			return Queue.Extent();
		}
		bso::bool__ IsMember( r Node ) const
		{
			return S_.QueueManager.IsMember( Node, Queue );
		}
		//f Return 'true' if 'Node' exists, false otherwise.
		bso::bool__ Exists( r Node ) const
		{
			return S_.QueueManager.Exists( Node, Queue );
		}
		//f Return 'true' if empty, false otherwise.
		bso::bool__ IsEmpty( void ) const
		{
			return S_.QueueManager.IsEmpty();
		}
		//f Return the head.
		r Head( void ) const
		{
			return S_.QueueManager.Head( Queue );
		}
		//f Return first node.
		r First( void ) const
		{
			return S_.QueueManager.First( Queue );
		}
		//f Return the tail.
		r Tail( void ) const
		{
			return S_.QueueManager.Tail( Queue );
		}
		//f Return last node.
		r Last( void ) const
		{
			return S_.QueueManager.Last( Queue );;
		}
		//f Return node next to 'Node'.
		r Next( r Node ) const
		{
			return S_.QueueManager.Next( Node, Queue );
		}
		//f Return node previous to 'Node'.
		r Previous( r Node ) const
		{
			return S_.QueueManager.Previous( Node, Queue );
		}
		//f Swap 'Node1' and 'Node2'.
		void Swap(
			r Node1,
			r Node2 )
		{
			S_.QueueManager.Swap( Node1, Node2, Queue );
		}
		void Dump(
			qQSTACKd( r ) &Stack,
			r Begin,
			que::direction Direction ) const
		{
			Queue.Dump( Stack, Begin, Direction );
		}
	};

	E_AUTO1( managed_queue )

	//d A managed queue.
	#define E_MQUEUEt_( r )		managed_queue_< r >
	#define E_MQUEUEt( r )		managed_queue<r>
	
	#define E_MQUEUE_	E_MQUEUEt_( sdr::row__ )
	#define E_MQUEUE	E_MQUEUEt( sdr::row__ )
}

#endif
