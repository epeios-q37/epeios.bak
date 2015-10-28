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

//	$Id: sma.h,v 1.4 2012/11/14 16:06:36 csimon Exp $

#ifndef SMA__INC
#define SMA__INC

#define SMA_NAME		"SMA"

#define	SMA_VERSION	"$Revision: 1.4 $"	

#define SMA_OWNER		"the Epeios project (http://zeusw.org/epeios/)"

#if defined( E_DEBUG ) && !defined( SMA_NODBG )
#define SMA_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:36 $

/* End of automatic documentation generation part. */
/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D Simplified Memory Allocation.
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "epeios.h"

namespace sma {

	typedef mdr::bsize__ size__;

	//c A memory heap.
	class memory_heap___
	{
	private:
		void *Buffer_;
		size__ Occupied_;
		size__ Size_;
	public:
		void reset( bso::bool__ P = true )
		{
			if( P ) {
				if ( Buffer_ != NULL )
					free( Buffer_ );
			}

			Buffer_ = NULL;
			Occupied_ = Size_ = 0;
		}
		memory_heap___( void )
		{
			reset( false );
		}
		~memory_heap___( void )
		{
			reset( true );
		}
		//f Initialization with a size of 'Size'.
		void Init( size__ Size )
		{
			reset();

			if ( ( Buffer_ = malloc( Size ) ) == NULL )
				ERRa();

			Size_ = Size;

			Occupied_ = 0;
		}
		//f Return a pointer to a memory of size 'Size'.
		void *Allocate( size__ Size )
		{
			void *Pointer = (char *)Buffer_ + Occupied_;

			if ( ( Size_ - Occupied_ ) < Size )
				ERRl();

			Occupied_ += Size;

			return Pointer;
		}
		//f Freeing memory corresponding to 'Pointer'.
		void Free( void *Pointer )
		{}
	};
}

/*$END$*/
#endif
