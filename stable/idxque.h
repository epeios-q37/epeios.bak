/*
	Header for the 'idxque' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: idxque.h,v 1.24 2013/04/11 19:36:49 csimon Exp $

#ifndef IDXQUE__INC
#define IDXQUE__INC

#define IDXQUE_NAME		"IDXQUE"

#define	IDXQUE_VERSION	"$Revision: 1.24 $"

#define IDXQUE_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &IDXQUETutor;

#if defined( E_DEBUG ) && !defined( IDXQUE_NODBG )
#define IDXQUE_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.24 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/11 19:36:49 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D InDeX QUEue 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D InDeX QUEue
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "txf.h"
#include "que.h"
#include "stk.h"

//d The end of the queue.
#define IDXQUE_END	NONE

namespace idxque {
	using namespace que;
	using que::queue_;	// VC++ has problem without this, despite line above.

	//c Queue-based index, fast browsing, but slow sorting.
	template <typename r> class queue_index_
	: public E_QUEUEt_( r )
	{
	public:
		struct s
		: public E_QUEUEt_( r )::s
		{};
	// fonctions
		queue_index_( s &S )
		: E_QUEUEt_( r )( S )
		{}
		void reset( bool P = true )
		{
			E_QUEUEt_( r )::reset( P );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			E_QUEUEt_( r )::plug( AS );
		}
		void plug( sdr::E_SDRIVER__ &SD )
		{
			E_QUEUEt_( r )::plug( SD );
		}
		queue_index_ &operator =( const queue_index_ &T )
		{
			E_QUEUEt_( r )::operator =( T );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			E_QUEUEt_( r )::Init();
		}
		/*f Dump to the stack 'Stack' the queue beginning at 'Begin' and in the 'Direction ' direction.. */
		void Dump(
			stk::E_BSTACK_( r ) &Stack,
			r Begin,
			idxque::direction Direction )
		{
			E_QUEUEt_( r )::Dump( Stack, Begin, Direction );
		}
		//f Item is marked as greater then 'Node'.
		void BecomeGreater(
			r Item,
			r Node )
		{
			E_QUEUEt_( r )::BecomeNext( Item, Node );
		}
		//f Item is marked as lesser then 'Node'.
		void BecomeLesser(
			r Item,
			r Node )
		{
			E_QUEUEt_( r )::BecomePrevious( Item, Node );
		}
	};

	E_AUTO1( queue_index )

	using que::files_hook___;

	template <typename index> uys::state__ Plug(
		index &Index,
		files_hook___ &Hook )
	{
		return que::Plug( Index, Hook );
	}

	
	#define E_IQUEUEt_( r )	queue_index_<r>
	#define E_IQUEUEt( r )	queue_index<r>
	
	#define E_IQUEUE_	E_IQUEUEt_( epeios::row__ )
	#define E_IQUEUE	E_IQUEUEt( epeios::row__ )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
