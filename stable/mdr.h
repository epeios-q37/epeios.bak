/*
	Header for the 'mdr' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: mdr.h,v 1.28 2013/03/14 11:09:19 csimon Exp $

#ifndef MDR__INC
#define MDR__INC

#define MDR_NAME		"MDR"

#define	MDR_VERSION	"$Revision: 1.28 $"

#define MDR_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &MDRTutor;

#if defined( E_DEBUG ) && !defined( MDR_NODBG )
#define MDR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.28 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/03/14 11:09:19 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Memory DRiver 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "Obsolete ! Use 'SDR' instead !"

//D Memory DRiver.

#include "err.h"
#include "flw.h"
#include "bso.h"
#include "tol.h"

#define MDR_INTERNAL_MEMORY_DRIVER *(mdr::E_MEMORY_DRIVER__ *)NULL

# define NONE ( (mdr::row_t__) -1 )

# define MDR__DSIZE_SIZE_MAX ( ( ( 8 * sizeof( mdr::size__ ) ) / 7 ) + 1 )

namespace mdr {

	//t The position in a memory.
	typedef bso::size__	row_t__;
	E_TMIMIC__( row_t__, row__ );

	typedef bso::p_size__	p_row_t__;
	E_TMIMIC__( p_row_t__, p_row__ );

	#define MDR_ROW_T_MAX		( BSO_SIZE_MAX - 1 )	// 'BSO_SIZE_MAX' is 'NONE'.

	//t The size of a memory.
	using bso::size__;
	#define MDR_SIZE_MAX	BSO_SIZE_MAX

	//t The portable version of a 'size__'.
	using bso::p_size__;
	#define MDR_P_SIZE_MAX	BSO_P_SIZE_MAX

	//t The type of the datum in a memory.
	typedef bso::raw__	datum__;

// Prdclarations.
	struct _xsize__;
	typedef _xsize__ xsize__;

	xsize__ Convert( size__ Size );

	// 'Dynamic size' : taile de taille variable.
	typedef mdr::datum__ dsize__[MDR__DSIZE_SIZE_MAX];

	typedef bso::ubyte__ length__;
# define MDRM__LENGTH_MAX BSO_UBYTE_MAX

	typedef struct _xsize__ {
	private:
		dsize__ _Size;
		length__ _Length;
	public:
		const mdr::datum__ *DSizeBuffer( void ) const
		{
			if ( _Length == 0 )
				ERRc();

			return _Size + MDR__DSIZE_SIZE_MAX - _Length;
		}
		length__ BufferSize( void ) const
		{
			return _Length;
		}
		void reset( bso::bool__ = true )
		{
			memset( _Size, 0, sizeof( _Size ) );
			_Length = 0;
		}
		E_CVDTOR( _xsize__ );
		friend xsize__ mdr::Convert( mdr::size__ Size );
	} xsize__;

	size__ Convert( const mdr::datum__ *DSize );


	//c Abstract memory driver. Use 'E_MEMORY_DRIVER__' instead directly this class.
	class memory_driver__
	{
	protected:
		// Alloue 'Size' octet.
		virtual void MDRAllocate( size__ Size )
		{
			ERRc();
			// For read-only memory.
		}
		/* Si la mmoire repose sur une mmoire persistante (un fichier, par exemple), retourne la taille de cette mmoire,
		ou, si non initialise (fichier absent, par exemple), ou non persistente, retourne 0 */
		virtual size__ MDRUnderlyingSize( void )
		{
			return 0;
		}
		//v Recall 'Amount' at position 'Position' and put them in 'Buffer'.
		virtual void MDRRecall(
			row_t__ Position,
			size__ Amount,
			datum__ *Buffer ) = 0;
		//v Write 'Amount' bytes from 'Buffer' to memory at position 'Position'.
		virtual void MDRStore(
			const datum__ *Buffer,
			size__ Amount,
			row_t__ Position )
		{
			ERRc();
			// For read-only memory.
		}
	public:
		memory_driver__( void )
		{
			reset( false );
		}
		virtual ~memory_driver__( void )	// to be sure that the destructor of dervaed classes are call.
		{
			reset( true );
		}
		void reset( bool = true )
		{
			// A des fins de standardisation.
		}
		//f Initialization.
		void Init( void )
		{
			reset();
		}
		//f Allocate 'Size' bytes in memory.
		void Allocate( size__ Size )
		{
			MDRAllocate( Size );
		}
		size__ UnderlyingSize( void )
		{
			return MDRUnderlyingSize();
		}
		//f Recall 'Amount' at position 'Position' and put them into 'Buffer'. Return 'Buffer'.
		void Recall(
			row_t__ Position,
			size__ Amount,
			datum__ *Buffer )
		{
			MDRRecall( Position, Amount, Buffer );
		}
		//f Store 'Amount' bytes from 'Buffer' at position 'Position'.
		void Store(
			const datum__ *Buffer,
			size__ Amount,
			row_t__ Position )
		{
			MDRStore( Buffer, Amount, Position );
		}
	};

	#define E_MEMORY_DRIVER__	memory_driver__
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
