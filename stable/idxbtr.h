/*
	Header for the 'idxbtr' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: idxbtr.h,v 1.57 2013/04/15 10:50:52 csimon Exp $

#ifndef IDXBTR__INC
#define IDXBTR__INC

#define IDXBTR_NAME		"IDXBTR"

#define	IDXBTR_VERSION	"$Revision: 1.57 $"

#define IDXBTR_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"


#if defined( E_DEBUG ) && !defined( IDXBTR_NODBG )
#define IDXBTR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.57 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/15 10:50:52 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D InDeX Binary TRee 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D InDeX Binary TRee
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "btr.h"
#include "que.h"
#include "stk.h"
#include "bitbch.h"

namespace idxbtr {

	using btr::level__;

	//c Tree-based index, fast sorting, but slow browsing.
	template <typename r> class tree_index_
	{
	private:
		/* Equilibre l'arbre, sachant que l'ordre des lments est donne par
		la file 'File' de tte 'Tete' et que l'on doit utiliser la 'Pilote' si != NULL.
		Retourne la nouvelle racine de l'arbre. */
		r Equilibrer_(
			const que::E_QUEUEt_( r ) &Index,
			r Premier,
			sdr::E_SDRIVER__ &Pilote );
		r _Compare(
			const que::E_QUEUEt_( r ) &Queue,
			r First ) const;
		r _SearchMostLeftNode(
			r Node,
			level__ &Level ) const
		{
			return BaseTree.SearchMostLeftNode( Node, Level );
		}
		r _SearchMostRightNode(
			r Node,
			level__ &Level ) const
		{
			return BaseTree.SearchMostRightNode( Node, Level );
		}
	public:
		btr::E_BTREEt_( r ) BaseTree;
		struct s
		{
			typename btr::E_BTREEt_( r )::s BaseTree;
		};
		tree_index_( s &S )
		: BaseTree( S.BaseTree )
		{}
		void reset( bool P = true )
		{
			BaseTree.reset( P );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			BaseTree.plug( AS );
		}
/*
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			E_BTREEt_( r )::plug( MD );
		}
*/		tree_index_ &operator =( const tree_index_ &I )
		{
			BaseTree = I.BaseTree;

			return *this;
		}
	/*	void ecrire( flo_sortie_ &Flot ) const
		{
			FLOEcrire( S_.Racine, Flot );
			bts_binary_tree_::ecrire( Flot );
		}
		void lire( flo_entree_ &Flot )
		{
			FLOLire( Flot, S_.Racine );
			bts_binary_tree_::lire( Flot );
		}
	*/	//f Initialization.
		void Init( void )
		{
			BaseTree.Init();
		}
		r GetRoot( r Row ) const
		{
			return BaseTree.GetRoot( Row );
		}
		void Allocate(
			sdr::size__ Size,
			aem::mode__ Mode = aem::m_Default )
		{
			BaseTree.Allocate( Size, Mode );
		}
		//f Return the first item of the index.
		r First(
			r Root,
			level__ &Level = *(level__ *)NULL) const
		{
			return _SearchMostLeftNode( Root, Level );
		}
		//f Return the last item of the index.
		r Last(
			r Root,
			level__ &Level = *(level__ *)NULL) const
		{
			return _SearchMostRightNode( Root, Level );
		}
		//f Return the item next to 'Item'.
		r Next( r Item ) const
		{
			if ( BaseTree.HasRight( Item ) )
				return _SearchMostLeftNode( BaseTree.Right( Item ), *(btr::level__ *)NULL );
			else
				if ( BaseTree.IsLeft( Item ) )
					return BaseTree.Parent( Item );
				else if ( BaseTree.IsRight( Item ) )
					return BaseTree.ParentOfFirstLeftNode( Item );
				else
					return E_NIL;
		}
		//f Return the item previous to 'Item'.
		r Previous( r Position ) const
		{
			if ( BaseTree.HasLeft( Position ) )
				return NoeudSansFille_( BaseTree.Left( Position ) );
			else
				if ( BaseTree.IsRight( Position ) )
					return BaseTree.Parent( Position );
				else if ( BaseTree.IsLeft( Position ) )
					return BaseTree.SearchFirstRightFather( Position );
				else
					return E_NIL;
		}
		sdr::size__ Amount( void ) const
		{
			return BaseTree.Amount();
		}
#if 0
		r Delete(
			r Node,
			r Root )
		{
			if ( BaseTree.HasBothChildren( Node ) ) {
				r TargetNode = E_NIL;

				if ( *Node & 1 )	// Petit gnrateur alatoire.
					TargetNode = _SearchMostLeftNode( BaseTree.Right( Node ) );
				else
					TargetNode = _SearchMostRightNode( BaseTree.Left( Node ) );

				if ( Root == Node )
					Root = TargetNode;

				BaseTree.SwapNodes( Node, TargetNode );
			}

			if ( !BaseTree.HasChildren( Node ) ) {
				if ( Node != Root )
					BaseTree.Cut( Node );
			} else {
				r Child = BaseTree.HasLeft( Node ) ? BaseTree.Left( Node ) : BaseTree.Right( Node );

				if ( Root == Node )
					Root = Child;

				BaseTree.Cut( Child );

				BaseTree.SwapTrees( Child, Node );	// Sans que les couleurs suivent.
			}

			if ( Root == Node )
				Root = E_NIL;

			return Root;
		}
#else
		//f Remove 'Item'. Return the new root.
		r Delete(
			r Item,
			r Root )
		{
			r Fils = BaseTree.Left( Item );
			r Fille = BaseTree.Right( Item );
			r Pere = BaseTree.Parent( Item );

			if ( Fils != E_NIL )
				BaseTree.Cut( Fils );

			if ( Fille != E_NIL )
				BaseTree.Cut( Fille );

			if ( BaseTree.IsLeft( Item ) )
			{
				BaseTree.Cut( Item );

				if ( Fils != E_NIL )
				{
					BaseTree.BecomeLeft( Fils, Pere );

					if ( Fille != E_NIL )
						BaseTree.BecomeRight( Fille, _SearchMostRightNode( Fils, *(btr::level__ *)NULL ) );
				}
				else if ( Fille != E_NIL )
					BaseTree.BecomeLeft( Fille, Pere );
			}
			else if ( BaseTree.IsRight( Item ) )
			{
				BaseTree.Cut( Item );

				if ( Fille != E_NIL )
				{
					BaseTree.BecomeRight( Fille, Pere );

					if ( Fils != E_NIL )
						BaseTree.BecomeLeft( Fils, _SearchMostLeftNode( Fille, *(btr::level__ *)NULL ) );
				}
				else if ( Fils != E_NIL )
					BaseTree.BecomeRight( Fils, Pere );
			}
			else
			{
				if ( Fils != E_NIL )
				{
					if ( Fille != E_NIL )
						BaseTree.BecomeRight( Fille, _SearchMostRightNode( Fils, *(btr::level__ *)NULL ) );

					return Fils;
				}
				else
					return Fille;
			}

			return Root;
		}
#endif
		//f Mark 'Row' as greater then 'Current'. 'Current' must be the result as a search with 'seeker_'.
		r BecomeRight(
			r Row,
			r Current,
			r Root )
		{
			BaseTree.BecomeRight( Row, Current );

			return Root;
		}
		//f Mark 'Row' as lesser then 'Current'. 'Current' must be the result as a search with 'seeker_'.
		r BecomeLeft(
			r Row,
			r Current,
			r Root )
		{
			BaseTree.BecomeLeft( Row, Current );

			return Root;
		}
		void BecomeRoot( r Node )
		{
#ifdef IDXBTR_DBG
			if ( BaseTree.HasParent( Node ) )
				ERRFwk();
#endif
		}
		r GetParent( r Node ) const
		{
			return BaseTree.Parent( Node );
		}
		r GetLesser( r Node ) const
		{
			return BaseTree.Left( Node );
		}
		bso::bool__ HasLesser( r Node ) const
		{
			return GetLesser( Node ) != E_NIL;
		}
		r GetGreater( r Node ) const
		{
			return BaseTree.Right( Node );
		}
		bso::bool__ HasGreater( r Node ) const
		{
			return GetGreater( Node ) != E_NIL;
		}
		r BecomeGreater(
			r Row,
			r Current,
			r Root )
		{
			return BecomeRight( Row, Current, Root );
		}
		//f Mark 'Row' as lesser then 'Current'. 'Current' must be the result as a search with 'seeker_'.
		r BecomeLesser(
			r Row,
			r Current,
			r Root )
		{
			return BecomeLeft( Row, Current, Root );
		}
		//f Balances the tree which underlies the index. Return the new root.
		r Balance( r Root );
		//f Fill the index with the items in 'Queue' beginning at 'Head', using 'MD' as memory driver if != 'NULL'. Return the new root.
		r Fill(
			const que::E_QUEUEt_( r ) &Queue,
			r Head,
			sdr::E_SDRIVER__ &SD = SDR_INTERNAL_SDRIVER )
		{
			Init();

			Allocate( Queue.Amount() );

			return Equilibrer_( Queue, Head, SD );
		}
		r Compare(
			const que::E_QUEUEt_( r ) &Queue,
			r First ) const
		{
			return _Compare( Queue, First );
		}
		//f Print the tree structure of the index.
		void PrintStructure(
			txf::text_oflow__ &OStream,
			r Root ) const
		{
			BaseTree.PrintStructure( Root, OStream );
		}
		friend sdr::row_t__ Equilibrer_(
			tree_index_<sdr::row__> &Tree,
			const que::E_QUEUE_ &File,
			sdr::row_t__ Premier,
			sdr::E_SDRIVER__ &Pilote );
	};

	E_AUTO1( tree_index )

	using btr::files_hook___;

	template <typename index> uys::state__ Plug(
		index &Index,
		files_hook___ &Hook )
	{
		return btr::Plug( Index.BaseTree, Hook );
	}


	#define E_IBTREEt( r )	tree_index< r >
	#define E_IBTREEt_( r )	tree_index_< r >

	#define E_IBTREE	E_IBTREEt( sdr::row__ )
	#define E_IBTREE_	E_IBTREEt_( sdr::row__ )

	sdr::row_t__ Balance_(
		E_IBTREE_ &Tree,
		sdr::row_t__ Root );

	sdr::row_t__ Equilibrer_(
		E_IBTREE_ &Tree,
		const que::E_QUEUE_ &File,
		sdr::row_t__ Premier,
		sdr::E_SDRIVER__ &Pilote );

	sdr::row_t__ Compare_(
		const E_IBTREE_ &Tree,
		const que::E_QUEUE_ &File,
		sdr::row_t__ First );	// Compare 'Tree' avec 'Queue'.

	template <typename r> inline r tree_index_<r>::Equilibrer_(
		const que::E_QUEUEt_( r ) &Index,
		r Premier,
		sdr::E_SDRIVER__ &Pilote )
	{
		return idxbtr::Equilibrer_( *(E_IBTREE_ *)this, *(const que::E_QUEUE_ *)&Index, *Premier, Pilote );
	}

	template <typename r> inline r tree_index_<r>::Balance( r Root )
	{
		return idxbtr::Balance_( *(E_IBTREE_ *)this, *Root );
	}

	template <typename r> inline r tree_index_<r>::_Compare(
		const que::E_QUEUEt_( r ) &Index,
		r First ) const
	{
		return idxbtr::Compare_( *(E_IBTREE_ *)this, *(que::E_QUEUE_ *)&Index, *First );
	}

	//c To seek in a tree index.
	template <typename r> class tree_seeker__
	{
	private:
		r _Current;
		const tree_index_<r> *Index_;
#ifdef IDXBTR_DBG
		void _Test( void ) const
		{
			if ( Index_== NULL )
				ERRFwk();

			if ( _Current == E_NIL )
				ERRFwk();
		}
#endif
		r _Handle( r Row )
		{
			if ( Row != E_NIL )
				_Current = Row;

			return Row;
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Current = E_NIL;
			Index_ = NULL;
		}
		tree_seeker__( void )
		{
			reset( false );
		}
		~tree_seeker__( void )
		{
			reset( true );
		}
		//f Initialisation with index 'Index'. 'Index' must not be empty.
		void Init(
			const tree_index_<r> &Index,
			r Root )
		{
			reset( true );

			Index_ = &Index;

			_Current = Root;
		}
		r GetGreater( void ) const
		{
			return Index_->GetGreater( _Current );
		}
		//f Try to find an element greater then the current.
		r SearchGreater( void )
		{
#ifdef IDXBTR_DBG
			_Test();
#endif
			return _Handle( GetGreater() );
		}
		bso::bool__ HasGreater( void ) const
		{
			return GetGreater() != E_NIL;
		}
		//f Try to find an element lesser then the current.
		r GetLesser( void ) const
		{
			return Index_->GetLesser( _Current );
		}
		r SearchLesser( void )
		{
#ifdef IDXBTR_DBG
			_Test();
#endif
			return _Handle( GetLesser() );
		}
		bso::bool__ HasLesser( void ) const
		{
			return GetLesser() != E_NIL;
		}

		E_RODISCLOSE__( r, Current )
	};

	//d A seeker.
	#define E_TSEEKERt__( r )	tree_seeker__<r>
	#define E_TSEEKER__			tree_seeker__<mdr::row__>


}


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
