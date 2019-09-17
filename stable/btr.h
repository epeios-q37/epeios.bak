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

// Binary Tree 

#ifndef BTR_INC_
# define BTR_INC_

# define BTR_NAME		"BTR"

# if defined( E_DEBUG ) && !defined( BTR_NODBG )
#  define BTR_DBG
# endif

# include "err.h"
# include "flw.h"
# include "bch.h"

# if 1

namespace btr {
	typedef bso::u8__ level__;

	//c Node. Internal use.
	class _node__
	{
	public:
		sdr::row_t__
			// Parent of the node.
			Parent,
			// Left children.
			Left,
			// Right children.
			Right;
		void reset( bso::bool__ = true )
		{
			Parent = Left = Right = qNIL;
		}
		_node__( void )
		{
			reset( false );
		}
	};

	//t Nodes. Internal use.
	typedef bch::E_BUNCH_( _node__ ) _nodes_;

	class _nodes_manager_
	: public _nodes_
	{
	private:
		void _Swap(
			sdr::row_t__ &R1,
			sdr::row_t__ &R2 )
		{
			sdr::row_t__ R = R1;
			R1 = R2;
			R2 = R;
		}
		void _ChangeLeft(
			sdr::row_t__ Node,
			sdr::row_t__ NewLeft )
		{
			_node__ N = Get( Node );

			N.Left = NewLeft;

			Store( N, Node );
		}
		void _ChangeRight(
			sdr::row_t__ Node,
			sdr::row_t__ NewRight )
		{
			_node__ N = Get( Node );

			N.Right = NewRight;

			Store( N, Node );
		}
		// Fait pointer le fils du parent de 'OldChild', s'il existe, sur 'NewChild'.
		void _ChangeParentChild(
			sdr::row_t__ &OldChild,
			sdr::row_t__ &NewChild )
		{
			if ( IsLeft( OldChild ) )
				_ChangeLeft( Parent( OldChild ), NewChild );
			else if ( IsRight( OldChild ) )
				_ChangeRight( Parent( OldChild ), NewChild );
		}
		void _ChangeParent(
			sdr::row_t__ Node,
			sdr::row_t__ NewParent )
		{
			_node__ N = Get( Node );

			N.Parent = NewParent;

			Store( N, Node );
		}
		void _ChangeChildrenParent(
			const _node__ &Node,
			sdr::row_t__ NewParent )
		{
			if ( Node.Left != qNIL )
				_ChangeParent( Node.Left, NewParent );

			if ( Node.Right != qNIL )
				_ChangeParent( Node.Right, NewParent );
		}
		sdr::row_t__ SearchMostLeftNode_(
			sdr::row_t__ Node,
			level__ *Level ) const;
		sdr::row_t__ SearchMostRightNode_(
			sdr::row_t__ Node,
			level__ *Level ) const;
	public:
		struct s
		: public _nodes_::s
		{};
		_nodes_manager_( s &S )
		: _nodes_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			_nodes_::reset( P );
		}
		//f Initialization.
		void Init( void )
		{
			_nodes_::Init();
		}
		void Prepare(
			sdr::row_t__ Start,
			sdr::row_t__ End );
		void Release(
			sdr::row_t__ Start,
			sdr::row_t__ End );
		void EraseParent( sdr::row_t__ Node )
		{
			_node__ Buffer = _nodes_::Get( Node );

			Buffer.Parent = qNIL;
			_nodes_::Store( Buffer, Node );
		}
		void EraseLeft( sdr::row_t__ Node )
		{
			_node__ Buffer = _nodes_::Get( Node );

			Buffer.Left = qNIL;
			_nodes_::Store( Buffer, Node );
		}
		void EraseRight( sdr::row_t__ Node )
		{
			_node__ Buffer = _nodes_::Get( Node );

			Buffer.Right = qNIL;
			_nodes_::Store( Buffer, Node );
		}
		//f Return true if 'Node' has a father.
		bso::bool__ HasParent( sdr::row_t__ Node ) const
		{
			return _nodes_::Get( Node ).Parent != qNIL;
		}
		//f Return true if 'Node' has left chid.
		bso::bool__ HasLeft( sdr::row_t__ Node ) const
		{
			return _nodes_::Get( Node ).Left != qNIL;
		}
		//f Return true if 'Node' has right chid.
		bso::bool__ HasRight( sdr::row_t__ Node ) const
		{
			return _nodes_::Get( Node ).Right != qNIL;
		}
		//f Return father of 'Node', or 'qNIL' if nonde.
		sdr::row_t__ Parent( sdr::row_t__ Node ) const
		{
			return _nodes_::Get( Node ).Parent;
		}
		//f Return left child of 'Node', or 'qNIL' if nonde.
		sdr::row_t__ Left( sdr::row_t__ Node ) const
		{
			return _nodes_::Get( Node ).Left;
		}
		//f Return right child of 'Node', or 'qNIL' if nonde.
		sdr::row_t__ Right( sdr::row_t__ Node ) const
		{
			return _nodes_::Get( Node ).Right;
		}
		void BecomeOverridingLeft(
			sdr::row_t__ Left,
			sdr::row_t__ Parent )	// Force l'action sans tenir compte de l'affectation des noeuds affects.
		{
			_node__ GParent = _nodes_::Get( Parent ), GLeft = _nodes_::Get( Left );

			GParent.Left = Left;
			GLeft.Parent = Parent;

			_nodes_::Store( GParent, Parent );
			_nodes_::Store( GLeft, Left );
		}
		//f 'Right' becomes left child of 'Parent'.
		void BecomeOverridingRight(
			sdr::row_t__ Right,
			sdr::row_t__ Parent )	// Force l'action sans tenir compte de l'affectation des noeuds affects.
		{
			_node__ GParent = _nodes_::Get( Parent ), GRight = _nodes_::Get( Right );

			GParent.Right = Right;
			GRight.Parent = Parent;

			_nodes_::Store( GParent,Parent );
			_nodes_::Store( GRight, Right );
		}

		//f 'Left' becomes left child of 'Parent'.
		void BecomeLeft(
			sdr::row_t__ Left,
			sdr::row_t__ Parent )
		{
			_node__ GParent = _nodes_::Get( Parent ), GLeft = _nodes_::Get( Left );
#  ifdef BTR_DBG
			if ( GParent.Left != qNIL )
				qRFwk();

			if ( ( GLeft.Parent != qNIL ) && ( GLeft.Parent != Parent ) )	// Un noeud peut avoir pour fils gauche et droit le mme noeud.
				qRFwk();
#  endif

			GParent.Left = Left;
			GLeft.Parent = Parent;

			_nodes_::Store( GParent, Parent );
			_nodes_::Store( GLeft, Left );
		}
		//f 'Right' becomes left child of 'Parent'.
		void BecomeRight(
			sdr::row_t__ Right,
			sdr::row_t__ Parent )
		{
			_node__ GParent = _nodes_::Get( Parent ), GRight = _nodes_::Get( Right );

#  ifdef BTR_DBG
			if ( GParent.Right != qNIL )
				qRFwk();

			if ( ( GRight.Parent != qNIL ) && ( GRight.Parent != Parent ) ) // Un noeud peut avoir pour fils gauche et droit le mme noeud.
				qRFwk();
#  endif
			GParent.Right = Right;
			GRight.Parent = Parent;

			_nodes_::Store( GParent,Parent );
			_nodes_::Store( GRight, Right );
		}
/*		row_t__ TrouverAieulMaleAvecRight(
			row_t__ Depart,
			row_t__ Racine ) const;
*/		sdr::row_t__ ForceParent(
			sdr::row_t__ Node,
			sdr::row_t__  Parent )
		{
			_node__ Lien = _nodes_::Get( Node );
			sdr::row_t__ AncienParent = Lien.Parent;

			Lien.Parent = Parent;

			_nodes_::Store( Lien, Node );

			return AncienParent;
		}
		//f Return true if 'Node' is a left child.
		bso::bool__ IsLeft( sdr::row_t__ Node ) const
		{
			return HasParent( Node ) && Left( Parent( Node ) ) == Node;
		}
		//f Return true if 'Node' is a right.
		bso::bool__ IsRight( sdr::row_t__ Node ) const
		{
			return HasParent( Node ) && Right( Parent( Node ) ) == Node;
		}
		//f Return true if 'Node' is child.
		bso::bool__ IsChild( sdr::row_t__ Node ) const
		{
			return HasParent( Node );
		}
		//f Return true if 'Node' is parent.
		bso::bool__ IsParent( sdr::row_t__ Node ) const
		{
			return HasLeft( Node ) || HasRight( Node );
		}
		// Retourne le premier noeud sans fils gauche  partir de 'Node' en descendant par les fils gauche.
		sdr::row_t__ SearchMostLeftNode(
			sdr::row_t__ Node,
			level__ &Level ) const
		{
			return SearchMostLeftNode_( Node, &Level );
		}
		sdr::row_t__ SearchMostLeftNode(
			sdr::row_t__ Node,
			level__ *Level = NULL ) const
		{
			return SearchMostLeftNode_( Node, Level );
		}
		sdr::row_t__ SearchMostRightNode(
			sdr::row_t__ Node,
			level__ &Level ) const
		{
			return SearchMostRightNode_( Node, &Level );
		}
		sdr::row_t__ SearchMostRightNode(
			sdr::row_t__ Node,
			level__ *Level = NULL) const
		{
			return SearchMostRightNode_( Node, Level );
		}
		void PrintStructure(
			sdr::row_t__ Racine,
			txf::text_oflow__ &Flot ) const;
		//f Return true if 'Node' has a child.
		bso::bool__ HasChildren( sdr::row_t__ Node ) const
		{
			return HasRight( Node ) || HasLeft( Node );
		}
		bso::bool__ HasBothChildren( sdr::row_t__ Node ) const
		{
			return HasRight( Node ) && HasLeft( Node );
		}
		// Echange 'Node1' avec 'Node2', c'est--dire qu'il echangent leur parent, et fils gauche et droit respectif.
		void SwapNodes(
			sdr::row_t__ RN1,
			sdr::row_t__ RN2 )
		{
			_node__ N1 = Get( RN1 ), N2 = Get( RN2 );

			_ChangeParentChild( RN1, RN2 );
			_ChangeParentChild( RN2, RN1 );

			_ChangeChildrenParent( N1, RN2 );
			_ChangeChildrenParent( N2, RN1 );

			Store( N1, RN2 );
			Store( N2, RN1 );
		}
		// Echange l'abre de racine 'Node1' avec l'abre de racine 'Node2'.
		void SwapTrees(
			sdr::row_t__ RN1,
			sdr::row_t__ RN2 )
		{
			_node__ N1 = Get( RN1 ), N2 = Get( RN2 );

			_ChangeParentChild( RN1, RN2 );
			_ChangeParentChild( RN2, RN1 );

			_Swap( N1.Parent, N2.Parent );

			Store( N1, RN1 );
			Store( N2, RN2 );
		}
		// Retourne le parent du premier noeud qui est fils en remontant.
		sdr::row_t__ ParentOfFirstLeftNode( sdr::row_t__ Node ) const;
		// Retourne le pre du premier noeud qui est fille en remontant.
		sdr::row_t__ ParentOfFirstRightNode( sdr::row_t__ Node ) const;
	};

	using bch::sHook;

	//c Binary tree.
	template <typename r> class binary_tree_
	{
	private:
		void Prepare_(
			sdr::row_t__ Start,
			sdr::row_t__ End )
		{
			Nodes.Prepare( Start, End );
		}
		void Release_(
			sdr::row_t__ Start,
			sdr::row_t__ End )
		{
			Nodes.Release( Start, End );
		}
		void EraseParent_( sdr::row_t__ Node )
		{
			Nodes.EraseParent( Node );
		}
		void EraseLeft_( sdr::row_t__ Node )
		{
			Nodes.EraseLeft( Node );
		}
		void EraseRight_( sdr::row_t__ Node )
		{
			Nodes.EraseRight( Node );
		}
		bso::bool__ HasParent_( sdr::row_t__ Node ) const
		{
			return Nodes.HasParent( Node );
		}
		bso::bool__ HasLeft_( sdr::row_t__ Node ) const
		{
			return Nodes.HasLeft( Node );
		}
		bso::bool__ HasRight_( sdr::row_t__ Node ) const
		{
			return Nodes.HasRight( Node );
		}
		sdr::row_t__ Parent_( sdr::row_t__ Node ) const
		{
			return Nodes.Parent( Node );
		}
		sdr::row_t__ Left_( sdr::row_t__ Node ) const
		{
			return Nodes.Left( Node );
		}
		sdr::row_t__ Right_( sdr::row_t__ Node ) const
		{
			return Nodes.Right( Node );
		}
		void BecomeLeft_(
			sdr::row_t__ Left,
			sdr::row_t__ Parent )
		{
			Nodes.BecomeLeft( Left, Parent );
		}
		void BecomeRight_(
			sdr::row_t__ Right,
			sdr::row_t__ Parent )
		{
			Nodes.BecomeRight( Right, Parent );
		}
		void BecomeOverridingLeft_(
			sdr::row_t__ Left,
			sdr::row_t__ Parent )
		{
			Nodes.BecomeOverridingLeft( Left, Parent );
		}
		void BecomeOverridingRight_(
			sdr::row_t__ Right,
			sdr::row_t__ Parent )
		{
			Nodes.BecomeOverridingRight( Right, Parent );
		}
/*
		sdr::row_t__ TrouverAieulMaleAvecRight_(
			sdr::row_t__ Depart,
			sdr::row_t__ Racine ) const
		{
			return Nodes.TrouverAieulMaleAvecRight( Depart, Racine );
		}
*/		sdr::row_t__ ForceParent_(
			sdr::row_t__ Node,
			sdr::row_t__  Parent )
		{
			return Nodes.ForceParent( Node, Parent );
		}
	public:
		struct s
		{
			_nodes_manager_::s Nodes;
		};
		// La table des liens.
		_nodes_manager_ Nodes;
		binary_tree_( s &S )
		: Nodes( S.Nodes )
		{}
		void reset( bool P = true )
		{
			Nodes.reset( P );
		}
		void plug( qASd *AS )
		{
			Nodes.plug( AS );
		}
		void plug( sHook &Hook )
		{
			Nodes.plug( Hook );
		}
		// Operateur d'affectation.
		binary_tree_ &operator =( const binary_tree_ &O )
		{
			Nodes = O.Nodes;

			return *this;
		}
	/*	void ecrire( flo_sortie_ &F ) const
		{
			Nodes.ecrire( F );
		}
		void lire( flo_entree_ &F )
		{
			Nodes.lire( F );
		}
	*/	//f Initialization.
		void Init( void )
		{
			Nodes.Init();
		}
		r GetRoot( r Row ) const
		{
			r Candidate = Row;

			while ( ( Candidate = Parent( Row ) ) != qNIL )
				Row = Candidate;

			return Row;
		}
		//f Extent of the tree.
		sdr::size__ Extent( void ) const
		{
			return Nodes.Extent();
		}
		//f Amount of node in the tree.
		sdr::size__ Amount( void ) const
		{
			return Nodes.Amount();
		}
		//f Return parent of 'Node'.
		r Parent( r Node ) const
		{
			return Parent_( *Node );
		}
		//f Return left of 'Node'..
		r Left( r Node ) const
		{
			return Left_( *Node ) ;
		}
		//f Return right of 'Node'..
		r Right( r Node ) const
		{
			return Right_( *Node );
		}
		// Retourne le grand-parent de 'Node', ou 'qNIL' si inexistant. Sotcke dans 'Parent' le parent de 'Node', si existant.
		r GrandParent(
			r Node,
			r &Parent ) const
		{
			if ( ( Parent = this->Parent( Node ) ) != qNIL )
				return this->Parent( Parent );
			else
				return qNIL;
		}
		// Retourne le grand-parent de 'Node', ou 'qNIL' si inexistant
		r GrandParent( r Node ) const
		{
			r Parent;

			return GrandParent( Node, Parent );
		}
		/* Retourne l'oncle de 'Node', ou 'qNIL' si inexistant. Stocke dans 'Parent' et 'GrandParent' respectivement 
		le parent et le grand-parent de 'Node', s'ils existent. ATTENTION : 'Parent' et 'GrandParent' ne doivent PAS pointer
		sur la mme variable. */
		r Uncle(
			r Node,
			r &Parent,
			r &GrandParent ) const
		{
			if ( ( GrandParent = this->GrandParent( Node, Parent ) ) == qNIL )
				return qNIL;

			if ( IsLeft( Parent ) )
				return Right( GrandParent );
			else
				return Left( GrandParent );
		}
		/* Retourne l'oncle de 'Node', ou 'qNIL' si inexistant. Stocke dans 'Parent' et 'GrandParent' respectivement 
		le parent et le grand-parent de 'Node', d'ils existent. */
		r Uncle( r Node ) const
		{
			r Parent, GrandParent;

			return Uncle( Node, Parent, GrandParent );
		}
		// Retourne le frre, 's'il existe, de 'Node'. Stocke, s'il existe, le parent de 'Node' dans 'PArent'
		r Sibling(
			r Node,
			r &Parent ) const
		{
			Parent = this->Parent( Node );

			if ( IsLeft( Node ) )
				return Right( Parent );
			else if ( IsRight( Node ) )
				return Left ( Parent );
			else
				return qNIL;
		}
		// Echange 'Node1' avec 'Node2', c'est--dire qu'il echangent leur parent, et enfants respectifs.
		void SwapNodes(
			r Node1,
			r Node2 )
		{
			Nodes.SwapNodes( *Node1, *Node2 );
		}
		// Echange l'arbre de racine 'Node1' avec l'arbre de racine 'Node2'.
		void SwapTrees(
			r Node1,
			r Node2 )
		{
			Nodes.SwapTrees( *Node1, *Node2 );
		}
		// Rotation  droite avec 'Node' come pivot, qui DOIT avoir un fils gauche. Retourne le remplaant de 'Node'.
		r RotateRight( r Node )
		{
			r B, D, &E = Node, P;
			bso::bool__ IsLeftFlag = false;

			P = Parent( E );

			if ( P != qNIL ) {
				IsLeftFlag = IsLeft( E );
				Cut( E );
			}

			B = Left( E );

#ifdef BTR_DBG
			if ( B == qNIL )
				qRFwk();
#endif
			Cut( B );

			D = Right( B );

			if ( D != qNIL )
				Cut( D );

			if ( P != qNIL ) {
				if ( IsLeftFlag )
					BecomeLeft( B, P );
				else
					BecomeRight( B, P );
			}

			BecomeRight( E, B );

			if ( D != qNIL )
				BecomeLeft( D, E );

			return B;
		}
		// Rotation  gauche avec 'Node' come pivot, qui DOIT avoir un fils droit.. Retourne le remplaant de 'Node'.
		r RotateLeft( r Node )
		{
			r &B = Node, D, E, P;
			bso::bool__ IsLeftFlag = false;

			P = Parent( B );

			if ( P != qNIL ) {
				IsLeftFlag = IsLeft( B );
				Cut( B );
			}

			E = Right( B );

#  ifdef BTR_DBG
			if ( E == qNIL )
				qRFwk();
#  endif
			Cut( E );

			D = Left( E );

			if ( D != qNIL )
				Cut( D );

			if ( P != qNIL ) {
				if ( IsLeftFlag )
					BecomeLeft( E, P );
				else
					BecomeRight( E, P );
			}

			BecomeLeft( B, E );

			if ( D != qNIL )
				BecomeRight( D, B );

			return E;
		}
		/* Elague 'Node'; 'Node' devient la racine de l'arbre
		et perd donc son pre. */
		//f Cut 'Node'. 'Node' becomes a root.
		void Cut( r Node )
		{
			sdr::row_t__ Parent = Parent_( *Node );

			if ( HasLeft_( Parent ) && ( Left_( Parent ) == *Node ) )
				EraseLeft_( Parent );
			
			if ( HasRight_( Parent ) && ( Right_( Parent ) == *Node ) )
				EraseRight_( Parent );

			EraseParent_( *Node );
		}
		//f Return true if 'Child' is left of 'Parent'.
		bso::bool__ IsLeft(
			r Child,
			r Parent ) const
		{
			return Left_( *Parent ) == Child;
		}
		//f Return true if 'Child' is right of 'Parent'.
		bso::bool__ IsRight(
			r Child,
			r Parent ) const
		{
			return Right_( *Parent ) == Child;
		}
		//f Return true if 'Child' is child of 'Parent'.
		bso::bool__ IsChild(
			r Child,
			r Parent ) const
		{
			return ( Left_( *Parent ) == Child ) || ( Right_( *Parent ) == Child );
		}
		//f Return true if 'Parent' is parent of 'Child'.
		bso::bool__ IsParent(
			r Parent,
			r Child ) const
		{
			return Parent_( *Child ) == Parent;
		}
		//f Return true if 'Node' is a child.
		bso::bool__ IsLeft( r Node ) const
		{
			return HasParent_( *Node ) && Left_( Parent_( *Node ) ) == *Node;
		}
		//f Return true if 'Node' is a right.
		bso::bool__ IsRight( r Node ) const
		{
			return HasParent_( *Node ) && Right_( Parent_( *Node ) ) == *Node;
		}
		//f Return true if 'Node' is child.
		bso::bool__ IsChild( r Node ) const
		{
			return HasParent_( *Node );
		}
		//f Return true if 'Node' is parent.
		bso::bool__ IsParent( r Node ) const
		{
			return HasLeft_( *Node ) || HasRight_( *Node );
		}
		//f Return true if 'Node' has a child.
		bso::bool__ HasChildren( r Node ) const
		{
			return Nodes.HasChildren( *Node );
		}
		bso::bool__ HasBothChildren( r Node ) const
		{
			return Nodes.HasBothChildren( *Node );
		}
		//f 'Parent' take 'Child' as left.
		void TakeLeft(
			r Parent,
			r Child )
		{
			BecomeLeft_( *Child, *Parent );
		}
		//f 'Parent' take 'Child' as right.
		void TakeRight(
			r Parent,
			r Child )
		{
			BecomeRight_( *Child, *Parent );
		}
		//f 'Child' becomes left of 'Parent'.
		void BecomeLeft(
			r Child,
			r Parent )
		{
			BecomeLeft_( *Child, *Parent );
		}
		//f 'Child' becomes right of 'Parent'.
		void BecomeRight(
			r Child,
			r Parent )
		{
			BecomeRight_( *Child, *Parent );
		}
		//f 'Child' becomes left of 'Parent'.
		void BecomeOverridingLeft(
			r Child,
			r Parent )
		{
			BecomeOverridingLeft_( *Child, *Parent );
		}
		//f 'Child' becomes right of 'Parent'.
		void BecomeOverridingRight(
			r Child,
			r Parent )
		{
			BecomeOverridingRight_( *Child, *Parent );
		}
		//f Allocate enough room to handle 'Size' node.
		void Allocate(
			sdr::size__ Size,
			aem::mode__ Mode = aem::m_Default )
		{
			if ( Size > Nodes.Amount() )
			{
				sdr::size__ PrecCapacite = Nodes.Amount();

				Nodes.Allocate( Size, Mode );
				Prepare_( PrecCapacite, Size - 1 );
			}
			else if ( Size < Nodes.Amount() )
			{
				Release_( Size, Nodes.Amount() - 1 );
				Nodes.Allocate( Size, Mode );
			}
		}
		//f Return true if 'Node' has right.
		bso::bool__ HasRight( r Node ) const
		{
			return HasRight_( *Node );
		}
		//f Return true if 'Node' has left.
		bso::bool__ HasLeft( r Node ) const
		{
			return HasLeft_( *Node );
		}
		//f Return true if 'Node' has a child.
		bso::bool__ HasChild( r Node ) const
		{
			return HasChild_( *Node ) || ALeft_( *Node );
		}
		//f Return true if 'Node' has a parent.
		bso::bool__ HasParent( r Node ) const
		{
			return HasParent_( *Node );
		}

		//f Force the parent from 'Node' to 'Parent'. Return the previous parent.
		r ForceParent(
			r Node,
			r Parent )
		{
			return ForceParent_( *Node, *Parent );
		}
		r SearchMostLeftNode(
			r Node,
			level__ &Level ) const
		{
			return Nodes.SearchMostLeftNode( *Node, Level );
		}
		r SearchMostLeftNode(
			r Node,
			level__ *Level ) const
		{
			return Nodes.SearchMostLeftNode( *Node, Level );
		}
		r SearchMostRightNode(
			r Node,
			level__ &Level ) const
		{
			return Nodes.SearchMostRightNode( *Node, Level );
		}
		r SearchMostRightNode(
			r Node,
			level__ *Level ) const
		{
			return Nodes.SearchMostRightNode( *Node, Level );
		}
		// Retourne le pre du premier noeud qui est fils en remontant.
		r ParentOfFirstLeftNode( r Node ) const
		{
			return Nodes.ParentOfFirstLeftNode( *Node );
		}
		// Retourne le pre du premier noeud qui est fille en remontant.
		r ParentOfFirstRightNode( r Node ) const
		{
			return Nodes.ParentOfFirstRightNode( *Node );
		}
		//f Print to 'OFfow' the structure of the tree whith root 'Root'.
		void PrintStructure(
			r Root,
			txf::text_oflow__ &OFlow ) const
		{
			Nodes.PrintStructure( *Root, OFlow );
		}
		// Sert  parcourir l'arbre de racine 'Racine'. Retourne le noeud aprs 'Position'.
	/*	r Suivant(
			r Courant,
			r Racine ) const
		{
			row_t__ &Temp = Racine;

			if ( ALeft( Courant ) )
				Courant = Left( Courant );
			else if ( ARight( Courant ) )
				Courant = Right( Courant );
			else if ( ( Courant = TrouverAieulMaleAvecRight_( Courant, Racine ) ) != BTR_INEXISTANT )
					Courant = Right( Courant );

			return Courant;
		}
	*/
	};

	E_AUTO1( binary_tree )

	using bch::rRH;

	using bch::rHF;
	using bch::rFH;

#  define E_BTREEt_( r )	binary_tree_< r >
#  define E_BTREEt( r )	binary_tree< r >

#  define E_BTREE_	E_BTREEt_( sdr::row__ )
#  define E_BTREE		E_BTREEt( sdr::row__ )
}


# else	//Version portable ; pb avec gcc.
namespace btr {
	//c Node. Internal use.
	template <typename r_t> class _node__
	{
	public:
		r_t
			// Parent of the node.
			Parent,
			// Left children.
			Left,
			// Right children.
			Right;
		void reset( bso::bool__ = true )
		{
			Parent = Left = Right = qNIL;
		}
		_node__( void )
		{
			reset( false );
		}
	};

	// Prdclaration.
	template <typename r_t, typename nodes> class _nodes_manager_;

	typedef bch::E_BUNCH_( btr::_node__<sdr::row_t__> ) _nodes_;

	void Release(
		_nodes_manager_<sdr::row_t__, _nodes_> &Nodes,
		sdr::row_t__ Start,
		sdr::row_t__ End );

	void Prepare(
		_nodes_manager_<sdr::row_t__, _nodes_> &Nodes,
		sdr::row_t__ Start,
		sdr::row_t__ End );


	template <typename r_t, typename nodes> class _nodes_manager_
	: public nodes
	{
	public:
		struct s
		: public nodes::s
		{};
		_nodes_manager_( s &S )
		: nodes( S )
		{}
		void reset( bso::bool__ P = true )
		{
			nodes::reset( P );
		}
		//f Initialization.
		void Init( void )
		{
			nodes::Init();
		}
		void Prepare(
			r_t Start,
			r_t End )
		{
			btr::Prepare( *this, Start, End );
		}
		void Release(
			r_t Start,
			r_t End )
		{
			btr::Release( *this, Start, End );
		}
		//f Release father of 'Node'.
		void ReleaseParent( r_t Node )
		{
			_node__<r_t> Buffer = nodes::Get( Node );

			Buffer.Parent = qNIL;
			nodes::Store( Buffer, Node );
		}
		//f Release left child of 'Node'.
		void ReleaseLeft( r_t Node )
		{
			_node__<r_t> Buffer = nodes::Get( Node );

			Buffer.Left = qNIL;
			nodes::Store( Buffer, Node );
		}
		//f Release right child of 'Node'.
		void ReleaseRight( r_t Node )
		{
			_node__<r_t> Buffer = nodes::Get( Node );

			Buffer.Right = qNIL;
			nodes::Store( Buffer, Node );
		}
		//f Return true if 'Node' has a father.
		bso::bool__ HasParent( r_t Node ) const
		{
			return nodes::Get( Node ).Parent != qNIL;
		}
		//f Return true if 'Node' has left chid.
		bso::bool__ HasLeft( r_t Node ) const
		{
			return nodes::Get( Node ).Left != qNIL;
		}
		//f Return true if 'Node' has right chid.
		bso::bool__ HasRight( r_t Node ) const
		{
			return nodes::Get( Node ).Right != qNIL;
		}
		//f Return father of 'Node', or 'qNIL' if nonde.
		r_t Parent( r_t Node ) const
		{
			return nodes::Get( Node ).Parent;
		}
		//f Return left child of 'Node', or 'qNIL' if nonde.
		r_t Left( r_t Node ) const
		{
			return nodes::Get( Node ).Left;
		}
		//f Return right child of 'Node', or 'qNIL' if nonde.
		r_t Right( r_t Node ) const
		{
			return nodes::Get( Node ).Right;
		}
		//f 'Left' becomes left child of 'Parent'.
		void BecomeLeft(
			r_t Left,
			r_t Parent )
		{
			_node__<r_t> GParent = nodes::Get( Parent ), GLeft = nodes::Get( Left );

			GParent.Left = Left;
			GLeft.Parent = Parent;

			nodes::Store( GParent, Parent );
			nodes::Store( GLeft, Left );
		}
		//f 'Right' becomes left child of 'Parent'.
		void BecomeRight(
			r_t Right,
			r_t Parent )
		{
			_node__<r_t> GParent = nodes::Get( Parent ), GRight = nodes::Get( Right );

			GParent.Right = Right;
			GRight.Parent = Parent;

			nodes::Store( GParent,Parent );
			nodes::Store( GRight, Right );
		}
/*		row_t__ TrouverAieulMaleAvecRight(
			row_t__ Depart,
			row_t__ Racine ) const;
*/		r_t ForceParent(
			r_t Node,
			r_t  Parent )
		{
			_node__<r_t> Lien = nodes::Get( Node );
			r_t AncienParent = Lien.Parent;

			Lien.Parent = Parent;

			nodes::Store( Lien, Node );

			return AncienParent;
		}
		//f Return true if 'Node' is a left child.
		bso::bool__ IsLeft( r_t Node ) const
		{
			return HasParent( Node ) && Left( Parent( Node ) ) == Node;
		}
		//f Return true if 'Node' is a right.
		bso::bool__ IsRight( r_t Node ) const
		{
			return HasParent( Node ) && Right( Parent( Node ) ) == Node;
		}
		//f Return true if 'Node' is child.
		bso::bool__ IsChild( r_t Node ) const
		{
			return HasParent( Node );
		}
		//f Return true if 'Node' is parent.
		bso::bool__ IsParent( r_t Node ) const
		{
			return HasLeft( Node ) || HasRight( Node );
		}
		void PrintStructure(
			r_t Racine,
			txf::text_oflow__ &Flot ) const;
		//f Return true if 'Node' has a child.
		bso::bool__ HasChild( r_t Node ) const
		{
			return HasRight( Node ) || HasLeft( Node );
		}
	};

	/**********************************************/
	/* CLASSE DE GESTION D'UN CONTENAIRE D'ARBRES */
	/**********************************************/

	//c Binary tree.
	template <typename r, typename r_t> class binary_tree_
	{
	private:
		void Prepare_(
			r_t Start,
			r_t End )
		{
			Nodes.Prepare( Start, End );
		}
		void Release_(
			r_t Start,
			r_t End )
		{
			Nodes.Release( Start, End );
		}
		void ReleaseParent_( r_t Node )
		{
			Nodes.ReleaseParent( Node );
		}
		void ReleaseLeft_( r_t Node )
		{
			Nodes.ReleaseLeft( Node );
		}
		void ReleaseRight_( r_t Node )
		{
			Nodes.ReleaseRight( Node );
		}
		bso::bool__ HasParent_( r_t Node ) const
		{
			return Nodes.HasParent( Node );
		}
		bso::bool__ HasLeft_( r_t Node ) const
		{
			return Nodes.HasLeft( Node );
		}
		bso::bool__ HasRight_( r_t Node ) const
		{
			return Nodes.HasRight( Node );
		}
		r_t Parent_( r_t Node ) const
		{
			return Nodes.Parent( Node );
		}
		r_t Left_( r_t Node ) const
		{
			return Nodes.Left( Node );
		}
		r_t Right_( r_t Node ) const
		{
			return Nodes.Right( Node );
		}
		void BecomeOverrindingLeft_(
			r_t Left,
			r_t Parent )
		{
			Nodes.BecomeOverridingLeft( Left, Parent );
		}
		void BecomeOverridingRight_(
			r_t Right,
			r_t Parent )
		{
			Nodes.BecomeOverridingRight( Right, Parent );
		}
		void BecomeLeft_(
			r_t Left,
			r_t Parent )
		{
			Nodes.BecomeLeft( Left, Parent );
		}
		void BecomeRight_(
			r_t Right,
			r_t Parent )
		{
			Nodes.BecomeRight( Right, Parent );
		}
/*
		r_t TrouverAieulMaleAvecRight_(
			r_t Depart,
			r_t Racine ) const
		{
			return Nodes.TrouverAieulMaleAvecRight( Depart, Racine );
		}
*/		r_t ForceParent_(
			r_t Node,
			r_t  Parent )
		{
			return Nodes.ForceParent( Node, Parent );
		}
	public:
		struct s
		{
			_nodes_manager_<r_t,bch::E_BUNCH_( _node__<r_t> )>::s Nodes;
		};
		// La table des liens.
		_nodes_manager_<r_t,bch::E_BUNCH_( _node__<r_t> )> Nodes;
		binary_tree_( s &S )
		: Nodes( S.Nodes )
		{}
		void reset( bool P = true )
		{
			Nodes.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Nodes.plug( M );
		}
		void plug( sdr::E_MEMORY_DRIVER__ &M )
		{
			Nodes.plug( M );
		}
		// Operateur d'affectation.
		binary_tree_ &operator =( const binary_tree_ &O )
		{
			Nodes = O.Nodes;

			return *this;
		}
	/*	void ecrire( flo_sortie_ &F ) const
		{
			Nodes.ecrire( F );
		}
		void lire( flo_entree_ &F )
		{
			Nodes.lire( F );
		}
	*/	//f Initialization.
		void Init( void )
		{
			Nodes.Init();
		}
		//f Extent of the tree.
		sdr::size__ Extent( void ) const
		{
			return Nodes.Extent();
		}
		//f Amount of node in the tree.
		sdr::size__ Amount( void ) const
		{
			return Nodes.Amount();
		}
		//f Return parent of 'Node'.
		r Parent( r Node ) const
		{
			return Parent_( *Node );
		}
		//f Return left of 'Node'..
		r Left( r Node ) const
		{
			return Left_( *Node ) ;
		}
		//f Return right of 'Node'..
		r Right( r Node ) const
		{
			return Right_( *Node );
		}
		// Retourne le grand-parent de 'Node', ou 'qNIL' si inexistant. Sotcke dans 'Parent' le parent de 'Node', si existant.
		r GrandParent(
			r Node,
			r &Parent ) const
		{
			if ( ( Parent = Parent_( *Node ) ) != qNIL )
				return Parent_( Parent );
			else
				return qNIL;
		}
		// Retourne le grand-parent de 'Node', ou 'qNIL' si inexistant
		r GrandParent( r Node ) const
		{
			r Parent;

			return GranParent( Node, Parent );
		}
		/* Retourne l'oncle de 'Node', ou 'qNIL' si inexistant. Stocke dans 'Parent' et 'GrandParent' respectivement 
		le parent et le grand-parent de 'Node', s'ils existent. ATTENTION : 'Parent' et 'GrandParent' ne doivent PAS pointer
		sur la mme variable. */
		r Uncle(
			r Node,
			r &Parent,
			r &GrandParent ) const
		{
			if ( ( GrandParent = GrandParent( Node, Parent ) ) == qNIL )
				return qNIL;

			if ( IsLeft( Parent, GrandParent ) )
				return Right( GrandParent );
			else
				return Left( GrandParent );
		}
		/* Retourne l'oncle de 'Node', ou 'qNIL' si inexistant. Stocke dans 'Parent' et 'GrandParent' respectivement 
		le parent et le grand-parent de 'Node', d'ils existent. */
		r Uncle( r Node ) const
		{
			r Parent, GrandParent;

			return Uncle( Node, Parent, GrandParent );
		}
		/* Elague 'Node'; 'Node' devient la racine de l'arbre
		et perd donc son pre. */
		//f Cut 'Node'. 'Node' becomes a root.
		void Cut( r Node )
		{
			r_t Parent = Parent_( *Node );

			if ( HasLeft_( Parent ) && ( Left_( Parent ) == *Node ) )
				ReleaseLeft_( Parent );
			
			if ( HasRight_( Parent ) && ( Right_( Parent ) == *Node ) )
				ReleaseRight_( Parent );

			ReleaseParent_( *Node );
		}
		//f Return true if 'Child' is left of 'Parent'.
		bso::bool__ IsLeft(
			r Child,
			r Parent ) const
		{
			return Left_( *Parent ) == Child;
		}
		//f Return true if 'Child' is right of 'Parent'.
		bso::bool__ IsRight(
			r Child,
			r Parent ) const
		{
			return Right_( *Parent ) == Child;
		}
		//f Return true if 'Child' is child of 'Parent'.
		bso::bool__ IsChild(
			r Child,
			r Parent ) const
		{
			return ( Left_( *Parent ) == Child ) || ( Right_( *Parent ) == Child );
		}
		//f Return true if 'Parent' is parent of 'Child'.
		bso::bool__ IsParent(
			r Parent,
			r Child ) const
		{
			return Parent_( *Child ) == Parent;
		}
		//f Return true if 'Node' is a child.
		bso::bool__ IsLeft( r Node ) const
		{
			return HasParent_( *Node ) && Left_( Parent_( *Node ) ) == *Node;
		}
		//f Return true if 'Node' is a right.
		bso::bool__ IsRight( r Node ) const
		{
			return HasParent_( *Node ) && Right_( Parent_( *Node ) ) == *Node;
		}
		//f Return true if 'Node' is child.
		bso::bool__ IsChild( r Node ) const
		{
			return HasParent_( *Node );
		}
		//f Return true if 'Node' is parent.
		bso::bool__ IsParent( r Node ) const
		{
			return HasLeft_( *Node ) || HasRight_( *Node );
		}
		//f 'Parent' take 'Child' as left.
		void TakeLeft(
			r Parent,
			r Child )
		{
			BecomeLeft_( *Child, *Parent );
		}
		//f 'Parent' take 'Child' as right.
		void TakeRight(
			r Parent,
			r Child )
		{
			BecomeRight_( *Child, *Parent );
		}
		//f 'Child' becomes left of 'Parent'.
		void BecomeLeft(
			r Child,
			r Parent )
		{
			BecomeLeft_( *Child, *Parent );
		}
		//f 'Child' becomes right of 'Parent'.
		void BecomeRight(
			r Child,
			r Parent )
		{
			BecomeRight_( *Child, *Parent );
		}
		//f 'Child' becomes left of 'Parent'.
		void BecomeOverridingLeft(
			r Child,
			r Parent )
		{
			BecomeOverridingLeft_( *Child, *Parent );
		}
		//f 'Child' becomes right of 'Parent'.
		void BecomeOverridingRight(
			r Child,
			r Parent )
		{
			BecomeOverridingRight_( *Child, *Parent );
		}
		//f Allocate enough room to handle 'Size' node.
		void Allocate(
			sdr::size__ Size,
			aem::mode Mode = aem::m_Default )
		{
			if ( Size > Nodes.Amount() )
			{
				sdr::size__ PrecCapacite = Nodes.Amount();

				Nodes.Allocate( Size, Mode );
				Prepare_( PrecCapacite, Size - 1 );
			}
			else if ( Size < Nodes.Amount() )
			{
				Release_( Size, Nodes.Amount() - 1 );
				Nodes.Allocate( Size, Mode );
			}
		}
		//f Return true if 'Node' has right.
		bso::bool__ HasRight( r Node ) const
		{
			return HasRight_( *Node );
		}
		//f Return true if 'Node' has left.
		bso::bool__ HasLeft( r Node ) const
		{
			return HasLeft_( *Node );
		}
		//f Return true if 'Node' has a child.
		bso::bool__ HasChild( r Node ) const
		{
			return HasChild_( *Node ) || ALeft_( *Node );
		}
		//f Return true if 'Node' has a parent.
		bso::bool__ HasParent( r Node ) const
		{
			return HasParent_( *Node );
		}
		// Rotation  droite avec 'Node' come pivot, qui DOIT avoir un fils gauche.
		void RotateRight( r Node )
		{
			r A, B, C, P, D = &Node;
			bso::bool__ IsLeft = false;

			Parent = Parent( D );

			if ( Parent != qNIL ) {
				IsLeft = IsLeft( D );
				Cut( D );
			}

			B = Left( D );

#  ifdef BTR_DBG
			if ( B == qNIL )
				qRFwk();
#  endif

			A = Left( B );
			C = Right( B );

			Cut( B );

			if ( A != qNIL )
				Cut( A );

			if ( C != qNIL )
				Cut( C );

			if ( Parent != qNIL )
				if ( IsLeft )
					BecomeLeft( B, Parent );
				else
					BecomRight( B, Parent );

			BecomeRight( D, B );

			if ( A != qNIL )
				BecomeLeft( A, B );

			if ( C != qNIL )
				BecomeLeft( C, D );
		}
		// Rotation  droite avec 'Node' come pivot, qui DOIT avoir un fils droit.
		void RotateRight( r Node )
		{
			r E, D, C, P, B = &Node;
			bso::bool__ IsLeft = false;

			P = P( B );

			if ( P != qNIL ) {
				IsLeft = IsLeft( B );
				Cut( B );
			}

			D = Right( B );

#  ifdef BTR_DBG
			if ( D == qNIL )
				qRFwk();
#  endif

			E = Right( D );
			C = Left( B );

			Cut( D );

			if ( E != qNIL )
				Cut( E );

			if ( C != qNIL )
				Cut( C );

			if ( P != qNIL )
				if ( IsLeft )
					BecomeLeft( D, Parent );
				else
					BecomRight( D, Parent );

			BecomeLeft( B, D );

			if ( E != qNIL )
				BecomeRight( E, D );

			if ( C != qNIL )
				BecomeRight( C, B );
		}
		//f Force the parent from 'Node' to 'Parent'. Return the previous parent.
		r ForceParent(
			r Node,
			r Parent )
		{
			return ForceParent_( *Node, *Parent );
		}

	/*	// Ecrit dans 'Flot' l'arbre de racine l'lment  'Position'.
		void EcrireDansFlot(
			flo_sortie_portable_ &Flot,
			r Position ) const;
		/* Lecture de l'abre contenu dans 'Flot'. La valeur retourne correspond 
		la position de la racine. */
		//	r LireDeFlot( flo_entree_portable_ &Flot );
		//f Print to 'OFfow' the structure of the tree whith root 'Root'.
		void PrintStructure(
			r Root,
			txf::text_oflow__ &OFlow ) const
		{
			Nodes.PrintStructure( *Root, OFlow );
		}
		// Sert  parcourir l'arbre de racine 'Racine'. Retourne le noeud aprs 'Position'.
	/*	r Suivant(
			r Courant,
			r Racine ) const
		{
			row_t__ &Temp = Racine;

			if ( ALeft( Courant ) )
				Courant = Left( Courant );
			else if ( ARight( Courant ) )
				Courant = Right( Courant );
			else if ( ( Courant = TrouverAieulMaleAvecRight_( Courant, Racine ) ) != BTR_INEXISTANT )
					Courant = Right( Courant );

			return Courant;
		}
	*/
	};

	E_AUTO2( binary_tree )

#  define E_BTREEt_( r )	binary_tree_<r,sdr::row_t__>
#  define E_BTREEt( r )	binary_tree<r,sdr::row_t__>

#  define E_BTREE_	E_BTREEt_( sdr::row__ )
#  define E_BTREE		E_BTREEt( sdr::row__ )

#  define E_PBTREEt_( r )	binary_tree_<r,sdr::p_row_t__>
#  define E_PBTREEt( r )	binary_tree<r,sdr::p_row_t__>

#  define E_PBTREE_		E_PBTREEt_( sdr::p_row__ )
#  define E_PBTREE		E_PBTREEt( sdr::p_row__ )
}
# endif

#endif
