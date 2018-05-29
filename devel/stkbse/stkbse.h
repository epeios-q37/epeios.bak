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

// STacK BaSE

#ifndef STKBSE__INC
# define STKBSE__INC

# define STKBSE_NAME		"STKBSE"

# if defined( E_DEBUG ) && !defined( STKBSE_NODBG )
#  define STKBSE_DBG
# endif

# include "ags.h"
# include "aem.h"
# include "tol.h"

namespace stkbse {
	template <typename set, typename item, typename row> class dStack
	: public set
	{
	public:
		struct s
		: public set::s
		{};
		dStack( s &S )
		: set( S )
		{}
		void reset( bool P = true )
		{
			set::reset( P );
		}
		dStack &operator =( const dStack &S )
		{
			set::operator =( S );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			set::Init();
		}
		//f Place 'Object' at the top of the stack. Return the position where this object is put.
		row Push( const item &Object )
		{
			return set::Append( Object );
		}
		//f Remove the object at row 'R'. If 'Adjust' at 'true', than adjust the size of the stack.
		void Remove(
			row Row,
			aem::mode__ Mode = aem::m_Default )
		{
			set::Remove( Row );
		}
		row Pop( void )
		{
			row Row = set::Last();

			Remove( Row );

			return Row;
		}
		row Pop( item &Item )
		{
			set::Recall( set::Last(), Item );

			return Pop();
		}
		//f Return 'true' if 'Object' exists in the stack, false otherwise.
		bso::bool__ Exists( const item &Object ) const
		{
			return set::Search( Object, 0, set::Amount() ) != qNIL;
		}
		//f Return true if an entry exists for row 'Row'.
		bso::bool__ Exists( row Row ) const
		{
			return set::Exists( Row );
		}
		//f Return the value stord on top of the stack.
		void Top( item &Item ) const
		{
# ifdef STK_DBG
			if ( set::Amount() == 0 )
				qRFwk();
# endif
			set::Recall( set::Last(), Item );
		}
	};

	qW3( Stack )
}

#endif
