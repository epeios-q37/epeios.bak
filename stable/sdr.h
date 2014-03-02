/*
	Header for the 'sdr' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2013-2004 Claude SIMON.

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

//	$Id: sdr.h,v 1.10 2013/04/15 10:50:54 csimon Exp $

#ifndef SDR__INC
#define SDR__INC

#define SDR_NAME		"SDR"

#define	SDR_VERSION	"$Revision: 1.10 $"

#define SDR_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &SDRTutor;

#if defined( E_DEBUG ) && !defined( SDR_NODBG )
#define SDR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.10 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/15 10:50:54 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Storage DRiver 
/* End addendum to automatic documentation generation part. */

# include "err.h"
# include "bso.h"
# include "tol.h"
# include "strng.h"

# define SDR_INTERNAL_SDRIVER *(sdr::E_SDRIVER__ *)NULL

# define E_NIL ( (sdr::row_t__) -1 )

# define SDR__DSIZE_SIZE_MAX ( ( ( 8 * sizeof( sdr::size__ ) ) / 7 ) + 1 )

namespace sdr {

	//t The position in a storage.
	typedef bso::size__	row_t__;
	E_TMIMIC__( row_t__, row__ );

# if 0
	typedef bso::p_size__	p_row_t__;
	E_TMIMIC__( p_row_t__, p_row__ );
# endif
	#define SDR_ROW_T_MAX		( BSO_SIZE_MAX - 1 )	// 'BSO_SIZE_MAX' is 'NONE'.

	//t The size of a storage.
	using bso::size__;
	#define SDR_SIZE_MAX	BSO_SIZE_MAX

# if 0
	//t The portable version of a 'size__'.
	using bso::p_size__;
	#define SDR_P_SIZE_MAX	BSO_P_SIZE_MAX
# endif

	//t The type of the datum in a storage.
	typedef bso::raw__	datum__;

	//c Abstract storage driver. Use 'E_STORAGE_DRIVER__' instead directly this class.
	class storage_driver__
	{
	protected:
		// Alloue 'Size' octet.
		virtual void SDRAllocate( size__ Size )
		{
			ERRFwk();
			// For read-only storage.
		}
		/* Si la mémoire repose sur une mémoire persistante (un fichier, par exemple), retourne la taille de cette mémoire,
		ou, si non initialisée (fichier absent, par exemple), ou non persistente, retourne 0 */
		virtual size__ SDRSize( void ) const = 0;
		//v Recall 'Amount' at position 'Position' and put them in 'Buffer'.
		virtual void SDRRecall(
			row_t__ Position,
			size__ Amount,
			datum__ *Buffer ) = 0;
		//v Write 'Amount' bytes from 'Buffer' to storage at position 'Position'.
		virtual void SDRStore(
			const datum__ *Buffer,
			size__ Amount,
			row_t__ Position )
		{
			ERRFwk();
			// For read-only storage.
		}
	public:
		void reset( bool = true )
		{
			// Standardisation.
		}
		E_CVDTOR( storage_driver__ );
		//f Initialization.
		void Init( void )
		{
			// Standardisation.
		}
		//f Allocate 'Size' bytes in storage.
		void Allocate( size__ Size )
		{
			SDRAllocate( Size );
		}
		//f Recall 'Amount' at position 'Position' and put them into 'Buffer'. Return 'Buffer'.
		void Recall(
			row_t__ Position,
			size__ Amount,
			datum__ *Buffer )
		{
			SDRRecall( Position, Amount, Buffer );
		}
		//f Store 'Amount' bytes from 'Buffer' at position 'Position'.
		void Store(
			const datum__ *Buffer,
			size__ Amount,
			row_t__ Position )
		{
			SDRStore( Buffer, Amount, Position );
		}
		sdr::size__ Size( void ) const
		{
			return SDRSize();
		}
	};

	#define E_SDRIVER__	storage_driver__
}

/*$END$*/

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
