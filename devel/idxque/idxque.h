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

// InDeX QUEue

#ifndef IDXQUE__INC
# define IDXQUE__INC

# define IDXQUE_NAME		"IDXQUE"

# if defined( E_DEBUG ) && !defined( IDXQUE_NODBG )
#  define IDXQUE_DBG
# endif

# include "err.h"
# include "txf.h"
# include "que.h"

//d The end of the queue.
# define IDXQUE_END	qNIL

namespace idxque {
	using namespace que;
	using que::queue_;	// VC++ has problem without this, despite line above.

	using que::sHook;

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
		void plug( qASd *AS )
		{
			E_QUEUEt_( r )::plug( AS );
		}
		void plug( sHook &Hook )
		{
			E_QUEUEt_( r )::plug( Hook );
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
			stkbch::qBSTACKdl( r ) &Stack,
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

	E_AUTO1( queue_index );

	using que::rRH;

	using que::rHF;
	using que::rFH;

	#define E_IQUEUEt_( r )	queue_index_<r>
	#define E_IQUEUEt( r )	queue_index<r>
	
	#define E_IQUEUE_	E_IQUEUEt_( epeios::row__ )
	#define E_IQUEUE	E_IQUEUEt( epeios::row__ )
}

#endif
