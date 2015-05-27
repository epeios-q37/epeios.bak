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

//	$Id: idxbtq.h,v 1.49 2013/05/17 15:03:50 csimon Exp $

#ifndef IDXBTQ__INC
#define IDXBTQ__INC

#define IDXBTQ_NAME		"IDXBTQ"

#define	IDXBTQ_VERSION	"$Revision: 1.49 $"

#define IDXBTQ_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"


#if defined( E_DEBUG ) && !defined( IDXBTQ_NODBG )
#define IDXBTQ_DBG
#endif

//D InDeX Best of Tree and Queue

#include "err.h"
#include "cpe.h"
#include "idxque.h"
#include "idxbtr.h"

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
		void plug( ags::E_ASTORAGE_ &AS )
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
			ERRChk();	// 
		}
		void BecomePrevious( void )	//Obsolete.
		{
			ERRChk();	// 
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
			sdr::E_SDRIVER__ &SD = SDR_INTERNAL_SDRIVER )
		{
			return E_IBTREEt_( r )::Fill( *this, First( Root ), SD );
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

			if ( Parent != E_NIL )
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

			if ( Parent != E_NIL )
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
			ERRChk();	
		}
		void HasLesser( void )	// Obsolete.
		{
			ERRChk();	
		}
	};

	E_AUTO1( tree_queue_index )

	struct hook_filenames___
	{
	public:
		idxbtr::hook_filenames___ Tree;
		idxque::hook_filenames___ Queue;
		void reset( bso::bool__ P = true )
		{
			Tree.reset( P );
			Queue.reset( P );
		}
		E_CDTOR( hook_filenames___ );
		void Init(
			const fnm::name___ &Path,
			const fnm::name___ &Basename );
	};


	class files_hook___
	{
	private:
		idxbtr::files_hook___ _Tree;
		idxque::files_hook___ _Queue;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				Settle();
			}

			_Tree.reset( P );
			_Queue.reset( P );
		}
		files_hook___( void ) 
		{
			reset( false );
		}
		~files_hook___( void ) 
		{
			reset();
		}
		void Init( 
			const hook_filenames___ &Filenames,
			uys::mode__ Mode,
			uys::behavior__ Behavior,
			flsq::id__ ID )
		{
			reset();

			_Tree.Init( Filenames.Tree, Mode, Behavior, ID );

			_Queue.Init( Filenames.Queue, Mode, Behavior, ID );
		}
		uys::state__ State( void ) const
		{
			uys::state__ State = _Tree.State();

			if ( !uys::IsError( State ) )
				if ( State != _Queue.State() )
					State = uys::sInconsistent;

			return State;
		}
		uys::state__ Bind( void )
		{
			uys::state__ State = _Tree.Bind();

			if ( uys::IsError( State ) )
				return State;

			if ( _Queue.Bind() != State )
				State = uys::sInconsistent;

			return State;

		}
		uys::state__ Settle( void )
		{
			uys::state__ State = _Tree.Settle();

			if ( uys::IsError( State ) )
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
				ERRFwk();

			return _Tree.IsPersistent();
		}
		bso::bool__ Exists( void ) const
		{
			bso::bool__ Exists = _Tree.Exists();

			if ( Exists != _Queue.Exists() )
				ERRFwk();

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
		idxbtr::files_hook___ &TreeFilesHook( void )
		{
			return _Tree;
		}
		idxque::files_hook___ &QueueFilesHook( void )
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
	};


	template <typename index> uys::state__ Plug(
		index &Index,
		files_hook___ &Hook )
	{
		uys::state__ State = idxbtr::Plug( Index, Hook.TreeFilesHook() );

		if ( uys::IsError( State ) ) {
			Hook.reset();
			return State;
		}

		if ( State != idxque::Plug( Index, Hook.QueueFilesHook() ) ) {
			Hook.reset();
			State = uys::sInconsistent;
		}

		return State;
	}

	template <typename r> E_TTCLONE__( idxbtr::E_TSEEKERt__( r ), index_seeker__ );
}

//d An index.
#define E_INDEXt_( r )	tree_queue_index_<r>
#define E_INDEXt( r )	tree_queue_index<r>

#define E_INDEX_	E_INDEXt_( sdr::row__ )
#define E_INDEX		E_INDEXt( sdr::row__ )

#define E_ISEEKERt__( r )	index_seeker__<r>
#define E_ISEEKER__			index_seeker__<sdr::row__>

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
