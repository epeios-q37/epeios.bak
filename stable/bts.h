/*
	Header for the 'bts' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
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

//	$Id: bts.h,v 1.11 2013/04/15 10:50:48 csimon Exp $

#ifndef BTS__INC
#define BTS__INC

#define BTS_NAME		"BTS"

#define	BTS_VERSION	"$Revision: 1.11 $"

#define BTS_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &BTSTutor;

#if defined( E_DEBUG ) && !defined( BTS_NODBG )
#define BTS_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.11 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/15 10:50:48 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Binary Tree Seeker 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "btr.h"

namespace bts {
	//e Node types.
	enum type__ {
		//i Unknow type.
		tUnknow,
		//i Root node.
		tRoot,
		//i Parent node when going forward.
		tForward,
		//i Parent node when returning.
		tBack,
		//i Left node.
		tLeft,
		//i Right node.
		tRight,
		//i Amount node types.
		t_amount
	};

	typedef sdr::row_t__	_row__;

	//c Base binary tree seeker. Internal use.
	class _binary_tree_seeker__
	{
	private:
		const btr::E_BTREE_ *Tree_;
		_row__ Root_, Current_;
		type__ Type_;
	public:
		void reset( bso::bool__ P = true )
		{
			Tree_ = NULL;

			Root_, Current_ = E_NIL;

			Type_ = tUnknow;
		}
		~_binary_tree_seeker__( void )
		{
			reset( false );
		}
		//f Initialization with tree 'Tree' with root node 'Root' using browse method 'Method'.
		void Init(
			const btr::E_BTREE_ &Tree,
			_row__ Root )
		{
			Tree_ = &Tree;
			Root_ = Current_ = Root;
			Type_ = tRoot;
		}
		//f Return first node.
		_row__ First( void )
		{
			Current_ = Root_;

			if ( Current_ != E_NIL )
				if ( Tree_->IsParent( Current_ ) )
					Type_ = tForward;
				else
					Type_ = tRight;

			return Current_;
		}
		//f Return last node.
		_row__ Last( void )
		{
			Current_ = Root_;

			if ( Current_ != E_NIL )
				if ( Tree_->IsParent( Current_ ) )
					Type_ = tBack;
				else
					Type_ = tRight;

			return Current_;
		}
		//f Return next node, or 'E_NIL' if none.
		_row__ Next( void )
		{
			_row__ Candidate = E_NIL;

			switch ( Type_ ) {
			case tRoot:
				Candidate = First();	// Type set by 'First()'.
				break;
			case tBack:
				if ( !Tree_->HasParent( Current_ ) ) {
					Candidate = E_NIL;
					break;
				} else if ( Tree_->IsRight( Current_ ) ) {
					Candidate = *Tree_->Parent( Current_ );
					Type_ = tBack;
					break;
				}
			case tLeft:
				Candidate = *Tree_->Parent( Current_ );

				if ( Tree_->HasRight( Candidate ) ) {
					Candidate = *Tree_->Right( Candidate );

					if ( Tree_->IsParent( Candidate ) )
						Type_ = tForward;
					else
						Type_ = tRight;
				} else
					Type_ = tBack;
				break;
			case tRight:
				Candidate = *Tree_->Parent( Current_ );
				Type_ = tBack;
				break;
			case tForward:
				if ( Tree_->HasLeft( Current_ ) ) {
					Type_ = tLeft;
					Candidate = *Tree_->Left( Current_ );
				} else if ( Tree_->HasRight( Current_ ) ) {
					Type_ = tRight;
					Candidate = *Tree_->Right( Current_ );
				}

				if ( Tree_->IsParent( Candidate ) )
					Type_ = tForward;

				break;
			default:
				ERRFwk();
				break;
			}

			Current_ = Candidate;

			return Current_;
		}
		//f Return the current node, which is what is returned by the las 'First()' od 'Next()' function called.
		_row__ GetCurrent( void )
		{
			return Current_;
		}
		//f Return the type of the node. Significatif only if 'GetCurrent()' != E_NIL.
		type__ GetType( void )
		{
			return Type_;
		}
	};

	//c Binary tree seeker.
	template <typename r> class binary_tree_seeker__
	: private _binary_tree_seeker__
	{
	public:
		//f Initialisation with tree 'Tree' with root node 'Root'.
		void Init(
			const btr::E_BTREEt_( r ) &Tree,
			r Row )
		{
			_binary_tree_seeker__::Init( *(const btr::E_BTREE_ *)&Tree, *Row );
		}
		//f Return the first node.
		r First( void )
		{
			return _binary_tree_seeker__::First();
		}
		//f Return the next node.
		r Next( void )
		{
			return _binary_tree_seeker__::Next();
		}
		//f Return the current node, which is what is returned by the las 'First()' od 'Next()' function called.
		r GetCurrent( void )
		{
			return _binary_tree_seeker__::GetCurrent();
		}
		//f Return the type of the node. Significatif only if 'GetCurrent()' != E_NIL.
		type__ GetType( void )
		{
			return _binary_tree_seeker__::GetType();
		}
	};

	#define E_BTREE_SEEKERt__( r )	binary_tree_seeker__< r >
	#define E_BTREE_SEEKER__		E_BTREE_SEEKERt__( sdr::row__ )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
