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

// InDeX Binary TRee 

#ifndef IDXBTR__INC
# define IDXBTR__INC

# define IDXBTR_NAME		"IDXBTR"

# if defined( E_DEBUG ) && !defined( IDXBTR_NODBG )
#  define IDXBTR_DBG
# endif

# include "err.h"
# include "btr.h"
# include "que.h"
# include "bitbch.h"

namespace idxbtr {
	using btr::level__;

	using btr::sHook;

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
			uys::sHook *Pilote );
		r _Compare(
			const que::E_QUEUEt_( r ) &Queue,
			r First ) const;
		r _SearchMostLeftNode(
			r Node,
			level__ *Level ) const
		{
			return BaseTree.SearchMostLeftNode( Node, Level );
		}
		r _SearchMostRightNode(
			r Node,
			level__ *Level ) const
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
		void plug( sHook &Hook )
		{
			return BaseTree.plug( Hook );
		}
		void plug( qASd *AS )
		{
			return BaseTree.plug( AS );
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
			level__ *Level = NULL) const
		{
			return _SearchMostLeftNode( Root, Level );
		}
		r First(
			r Root,
			level__ &Level ) const
		{
			return First( Root, &Level );
		}
		//f Return the last item of the index.
		r Last(
			r Root,
			level__ *Level = NULL) const
		{
			return _SearchMostRightNode( Root, Level );
		}
		r LAst(
			r Root,
			level__ &Level ) const
		{
			return LAst( Root, &Level );
		}
		//f Return the item next to 'Item'.
		r Next( r Item ) const
		{
			if ( BaseTree.HasRight( Item ) )
				return _SearchMostLeftNode( BaseTree.Right( Item ), NULL );
			else
				if ( BaseTree.IsLeft( Item ) )
					return BaseTree.Parent( Item );
				else if ( BaseTree.IsRight( Item ) )
					return BaseTree.ParentOfFirstLeftNode( Item );
				else
					return qNIL;
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
					return qNIL;
		}
		sdr::size__ Amount( void ) const
		{
			return BaseTree.Amount();
		}
# if 0
		r Delete(
			r Node,
			r Root )
		{
			if ( BaseTree.HasBothChildren( Node ) ) {
				r TargetNode = qNIL;

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
				Root = qNIL;

			return Root;
		}
# else
		//f Remove 'Item'. Return the new root.
		r Delete(
			r Item,
			r Root )
		{
			r Fils = BaseTree.Left( Item );
			r Fille = BaseTree.Right( Item );
			r Pere = BaseTree.Parent( Item );

			if ( Fils != qNIL )
				BaseTree.Cut( Fils );

			if ( Fille != qNIL )
				BaseTree.Cut( Fille );

			if ( BaseTree.IsLeft( Item ) )
			{
				BaseTree.Cut( Item );

				if ( Fils != qNIL )
				{
					BaseTree.BecomeLeft( Fils, Pere );

					if ( Fille != qNIL )
						BaseTree.BecomeRight( Fille, _SearchMostRightNode( Fils, NULL ) );
				}
				else if ( Fille != qNIL )
					BaseTree.BecomeLeft( Fille, Pere );
			}
			else if ( BaseTree.IsRight( Item ) )
			{
				BaseTree.Cut( Item );

				if ( Fille != qNIL )
				{
					BaseTree.BecomeRight( Fille, Pere );

					if ( Fils != qNIL )
						BaseTree.BecomeLeft( Fils, _SearchMostLeftNode( Fille, NULL ) );
				}
				else if ( Fils != qNIL )
					BaseTree.BecomeRight( Fils, Pere );
			}
			else
			{
				if ( Fils != qNIL )
				{
					if ( Fille != qNIL )
						BaseTree.BecomeRight( Fille, _SearchMostRightNode( Fils, NULL ) );

					return Fils;
				}
				else
					return Fille;
			}

			return Root;
		}
# endif
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
# ifdef IDXBTR_DBG
			if ( BaseTree.HasParent( Node ) )
				qRFwk();
# endif
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
			return GetLesser( Node ) != qNIL;
		}
		r GetGreater( r Node ) const
		{
			return BaseTree.Right( Node );
		}
		bso::bool__ HasGreater( r Node ) const
		{
			return GetGreater( Node ) != qNIL;
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
		r Fill(
			const que::E_QUEUEt_( r ) &Queue,
			r Head,
			uys::sHook *Hook  )
		{
			Init();

			Allocate( Queue.Amount() );

			return Equilibrer_( Queue, Head, Hook );
		}
		r Fill(
			const que::E_QUEUEt_( r ) &Queue,
			r Head )
		{
			Init();

			Allocate( Queue.Amount() );

			return Equilibrer_( Queue, Head, NULL );
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
			uys::sHook *Pilote );
	};

	E_AUTO1( tree_index );

	using btr::rRH;

	using btr::rHF;
	using btr::rFH;


# define E_IBTREEt( r )	tree_index< r >
# define E_IBTREEt_( r )	tree_index_< r >

# define E_IBTREE	E_IBTREEt( sdr::row__ )
# define E_IBTREE_	E_IBTREEt_( sdr::row__ )

	sdr::row_t__ Balance_(
		E_IBTREE_ &Tree,
		sdr::row_t__ Root );

	sdr::row_t__ Equilibrer_(
		E_IBTREE_ &Tree,
		const que::E_QUEUE_ &File,
		sdr::row_t__ Premier,
		uys::sHook *Pilote );

	sdr::row_t__ Compare_(
		const E_IBTREE_ &Tree,
		const que::E_QUEUE_ &File,
		sdr::row_t__ First );	// Compare 'Tree' avec 'Queue'.

	template <typename r> inline r tree_index_<r>::Equilibrer_(
		const que::E_QUEUEt_( r ) &Index,
		r Premier,
		uys::sHook *Pilote )
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
# ifdef IDXBTR_DBG
		void _Test( void ) const
		{
			if ( Index_== NULL )
				qRFwk();

			if ( _Current == qNIL )
				qRFwk();
		}
# endif
		r _Handle( r Row )
		{
			if ( Row != qNIL )
				_Current = Row;

			return Row;
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Current = qNIL;
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
# endif
			return _Handle( GetGreater() );
		}
		bso::bool__ HasGreater( void ) const
		{
			return GetGreater() != qNIL;
		}
		//f Try to find an element lesser then the current.
		r GetLesser( void ) const
		{
			return Index_->GetLesser( _Current );
		}
		r SearchLesser( void )
		{
# ifdef IDXBTR_DBG
			_Test();
# endif
			return _Handle( GetLesser() );
		}
		bso::bool__ HasLesser( void ) const
		{
			return GetLesser() != qNIL;
		}

		E_RODISCLOSE__( r, Current )
	};

	//d A seeker.
# define E_TSEEKERt__( r )	tree_seeker__<r>
# define E_TSEEKER__			tree_seeker__<mdr::row__>


}

#endif
