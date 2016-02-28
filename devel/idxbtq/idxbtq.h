/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

// InDeX Best of Tree and Queue

#ifndef IDXBTQ__INC
# define IDXBTQ__INC

#define IDXBTQ_NAME		"IDXBTQ"

#if defined( E_DEBUG ) && !defined( IDXBTQ_NODBG )
# define IDXBTQ_DBG
#endif

# include "err.h"
# include "cpe.h"
# include "idxque.h"
# include "idxbtr.h"

namespace idxbtq {
	using idxbtr::tree_index_;
	using idxque::queue_index_;

	//c Index using a tree-based index and a queue-based index. Fast browsing and sorting.
	template <typename r> class tree_queue_index_
	: public E_IBTREEt_( r ),
	  public E_IQUEUEt_( r )
	{
	public:
		struct s
		: public E_IBTREEt_( r )::s,
		  public E_IQUEUEt_( r )::s
		{};
		tree_queue_index_( s &S )
		: E_IBTREEt_( r )( S ),
		  E_IQUEUEt_( r )( S )
		{}
		void reset( bool P = true )
		{
			E_IBTREEt_( r )::reset( P );
			E_IQUEUEt_( r )::reset( P );
		}
		void plug( qASv &AS )
		{
			E_IBTREEt_( r )::plug( AS );
			E_IQUEUEt_( r )::plug( AS );
		}
		tree_queue_index_ &operator =( const tree_queue_index_ &I )
		{
			E_IBTREEt_( r )::operator =( I );
			E_IQUEUEt_( r )::operator =( I );

			return *this;
		}
	/*	void ecrire( flo_sortie_ &Flot ) const
		{
			tree_index_::ecrire( Flot );
			queue_index_::ecrire( Flot );
		}
		void lire( flo_entree_ &Flot )
		{
			tree_index_::lire( Flot );
			queue_index_::lire( Flot );
		}
	*/	//f Initializtion.
		void Init( void )
		{
			E_IBTREEt_( r )::Init();
			E_IQUEUEt_( r )::Init();
		}
		//f Allocate enough room to contain 'Size' items.
		void Allocate(
			sdr::size__ Size,
			aem::mode__ Mode = aem::m_Default )
		{
			E_IBTREEt_( r )::Allocate( Size, Mode );
			E_IQUEUEt_( r )::Allocate( Size, Mode );
		}
		void BecomeNext( void )	//Obsolete.
		{
			qRChk();	// 
		}
		void BecomePrevious( void )	//Obsolete.
		{
			qRChk();	// 
		}
		//f Put 'Row' as greater then 'Current'. 'Current' must be the result as a search with 'seeker_'.
		r BecomeGreater(
			r Row,
			r Current,
			r Root )
		{
			E_IQUEUEt_( r )::BecomeGreater( Row, Current );
			return E_IBTREEt_( r )::BecomeGreater( Row, Current, Root );
		}
		//f Put 'Row' as greater then 'Current'. 'Current' must be the result as a search with 'seeker_'.
		r BecomeLesser(
			r Row,
			r Current,
			r Root )
		{
			E_IQUEUEt_( r )::BecomeLesser( Row, Current );
			return E_IBTREEt_( r )::BecomeLesser( Row, Current, Root );
		}
		r Delete(
			r Item,
			r Root )
		{
			E_IQUEUEt_( r )::Delete( Item );
			return E_IBTREEt_( r )::Delete( Item, Root );
		}
		//f Balances the tree of the index.
		r Balance(
			r Root,
			uys::cHook *Hook )
		{
			return E_IBTREEt_( r )::Fill( *this, First( Root ), Hook );
		}
		r Balance( r Root )
		{
			r Row = qNIL;
		qRH
			uys::rRH Hook;
		qRB
			Hook.Init();

		Row = Balance( Root, &Hook );
		qRR
		qRT
		qRE
		}
		r Compare( r Root ) const
		{
			return E_IBTREEt_( r )::Compare( *this, First( Root ) );
		}
		r First(
			r Root,
			btr::level__ &Level = *(btr::level__ *)NULL ) const
		{
			return E_IBTREEt_( r )::First( Root, Level );
		}
		r Last(
			r Root,
			btr::level__ &Level = *(btr::level__ *)NULL ) const
		{
			return E_IBTREEt_( r )::Last( Root, Level );
		}
		r Next( r Current ) const
		{
			return E_IQUEUEt_( r )::Next( Current );
		}
		r Previous( r Current ) const
		{
			return E_IQUEUEt_( r )::Previous( Current );
		}
		sdr::size__ Amount( void ) const
		{
			return E_IBTREEt_( r )::Amount();
		}
		E_IBTREEt_( r ) &Tree( void )
		{
			return *this;
		}
		const E_IBTREEt_( r ) &Tree( void ) const
		{
			return *this;
		}
		E_IQUEUEt_( r ) &Queue( void )
		{
			return *this;
		}
		const E_IQUEUEt_( r ) &Queue( void ) const
		{
			return *this;
		}
		r GetTreeParent( r Node ) const
		{
			return E_IBTREEt_( r )::GetParent( Node );
		}
		r GetTreeLesser( r Node ) const
		{
			return E_IBTREEt_( r )::GetLesser( Node );
		}
		bso::bool__ IsTreeLesser( r Node ) const
		{
			r Parent = GetTreeParent( Node );

			if ( Parent != qNIL )
				return GetTreeLesser( Parent ) == Node;
			else
				return false;
		}
		r GetTreeGreater( r Node ) const
		{
			return E_IBTREEt_( r )::GetGreater( Node );
		}
		bso::bool__ IsTreeGreater( r Node ) const
		{
			r Parent = GetTreeParent( Node );

			if ( Parent != qNIL )
				return GetTreeGreater( Parent ) == Node;
			else
				return false;
		}
		bso::bool__ TreeHasLesser( r Node ) const
		{
			return E_IBTREEt_( r )::HasLesser( Node );
		}
		bso::bool__ TreeHasGreater( r Node ) const
		{
			return E_IBTREEt_( r )::HasGreater( Node );
		}
		void HasGreater( void )	// Obsolete.
		{
			qRChk();	
		}
		void HasLesser( void )	// Obsolete.
		{
			qRChk();	
		}
	};

	E_AUTO1( tree_queue_index )

	template <typename index, typename tree, typename queue> class rH_
	{
	protected:
		tree Tree_;
		queue Queue_;
	public:
		void reset( bso::fBool P = true )
		{
			Tree_.reset( P );
			Queue_.reset( P );
		}
		qCVDTOR( rH_ );
		bso::fBool Plug( index &Index )
		{
			bso::fBool Exists = Tree_.Plug( Index.GetTree() );

			return Queue_.Plug( Index.GetQueue() ) || Exists;
		}
	};

	template <class index> class rRH
	: public rH_<index, idxbtr::rRH, idxque::rRH>
	{
	public:
		void Init( void )
		{
			Tree_.Init();
			Index_.Init();
		}
	};

	struct rHF
	{
	public:
		idxbtr::rHF Tree;
		idxque::rHF Queue;
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

	template <class index> class rFH
	: public rH_<index, idxbtr::rFH, idxque::rFH>
	{
		uys::eState Init( 
			const rHF &Filenames,
			uys::mode__ Mode,
			uys::behavior__ Behavior,
			flsq::id__ ID )
		{
			uys::eState State = _Tree.Init( Filenames.Tree, Core.GetTree().GetTree().GetBunch(), Mode, Behavior, ID );

			if ( !State.IsError() ) {
				if ( _Queue.Init( Filenames.Queue, Core.GetQueue().GetQueue().GetBunch(), Mode, Behavior, ID ) != State )
					State = uys::sInconsistent;
			}

			fHook::Init();

			return State;
		}
		/*
		uys::state__ State( void ) const
		{
			uys::state__ State = _Tree.State();

			if ( !State.IsError() )
				if ( State != _Queue.State() )
					State = uys::sInconsistent;

			return State;
		}
		uys::state__ Settle( void )
		{
			uys::state__ State = _Tree.Settle();

			if ( State.IsError() )
				return State;

			if ( _Queue.Settle() != State )
				State = uys::sInconsistent;

			return State;

		}
		void ReleaseFiles( void )
		{
			_Tree.ReleaseFile();
			_Queue.ReleaseFile();
		}
		void Mode( uys::mode__ Mode )
		{
			_Tree.Mode( Mode );
			_Queue.Mode( Mode );
		}
		bso::bool__ IsPersistent( void ) const
		{
			if ( ( _Tree.IsPersistent() ) != _Queue.IsPersistent() )
				qRFwk();

			return _Tree.IsPersistent();
		}
		bso::bool__ Exists( void ) const
		{
			bso::bool__ Exists = _Tree.Exists();

			if ( Exists != _Queue.Exists() )
				qRFwk();

			return Exists;
		}
		bso::bool__ CreateFiles( err::handling__ ErrorHandling = err::h_Default )
		{
			bso::bool__ Success = _Tree.CreateFiles( ErrorHandling );

			if ( !Success )
				return false;

			Success = _Queue.CreateFiles( ErrorHandling );

			return Success;
		}
		void Drop( void )
		{
			_Tree.Drop();
			_Queue.Drop();
		}
		idxbtr::fh___ &TreeFilesHook( void )
		{
			return _Tree;
		}
		idxque::fh___ &QueueFilesHook( void )
		{
			return _Queue;
		}
		time_t TimeStamp( void ) const
		{
			time_t TreeTimeStamp, QueueTimeStamp;

			TreeTimeStamp = _Tree.TimeStamp();
			QueueTimeStamp = _Queue.TimeStamp();

			return ( TreeTimeStamp > QueueTimeStamp ? TreeTimeStamp : QueueTimeStamp );
		}
		*/
	};

	template <typename r> E_TTCLONE__( idxbtr::E_TSEEKERt__( r ), index_seeker__ );
}

//d An index.
# define E_INDEXt_( r )	tree_queue_index_<r>
# define E_INDEXt( r )	tree_queue_index<r>

# define E_INDEX_	E_INDEXt_( sdr::row__ )
# define E_INDEX		E_INDEXt( sdr::row__ )

# define E_ISEEKERt__( r )	index_seeker__<r>
# define E_ISEEKER__			index_seeker__<sdr::row__>

#endif
