/*
  Header for the 'sma' library by Claude SIMON (http://zeusw.org/intl/contact.html)
  Copyright (C) 2000,2001 Claude SIMON (http://zeusw.org/intl/contact.html) 

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

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
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
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
