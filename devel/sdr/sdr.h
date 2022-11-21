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

// Storage DRiver

#ifndef SDR_INC_
# define SDR_INC_

# define SDR_NAME		"SDR"

# if defined( E_DEBUG ) && !defined( SDR_NODBG )
#  define SDR_DBG
# endif

# include "err.h"
# include "bso.h"
# include "tol.h"
# include "strng.h"

/*************************/
/****** New version ******/
/*************************/

namespace sdr {
	using bso::sByte;
	using bso::sSize;

	typedef bso::sSize tRow;

	typedef tRow	row_t__;

	E_TMIMIC__( tRow, sRow );

	class storage_driver__;

	typedef storage_driver__ sStorageDriver;
}

# define qSDs	sdr::storage_driver__


/*************************/
/****** Old version ******/
/*************************/

# define SDR_INTERNAL_SDRIVER *(qSD__ *)NULL

# define qNIL ( (sdr::row_t__)-1 )

# define SDR__DSIZE_SIZE_MAX ( ( ( 8 * sizeof( sdr::size__ ) ) / 7 ) + 1 )

namespace sdr {
	typedef sRow row__;

	E_CDEF( sdr::row_t__, NIL, qNIL );

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

	// Unit of digital information, without any intrinsec signification.
	using bso::byte__;

	qENUM( Type )	{
		tVolatile,
		tPersistent,
		t_amount,
		t_Undefined
	};

	//c Abstract storage driver. Use 'E_STORAGE_DRIVER__' instead directly this class.
	class storage_driver__
	{
	protected:
		// Alloue 'Size' octet.
		virtual void SDRAllocate( size__ Size )
		{
			qRFwk();
			// For read-only storage.
		}
		/* Si la mmoire repose sur une mmoire persistante (un fichier, par exemple), retourne la taille de cette mmoire,
		ou, si non initialise (fichier absent, par exemple), ou non persistente, retourne 0 */
		virtual size__ SDRSize( void ) const = 0;
		//v Recall 'Amount' at position 'Position' and put them in 'Buffer'.
		virtual sSize SDRFetch(
			row_t__ Position,
			size__ Amount,
			byte__ *Buffer,
			qRPN) = 0;
		//v Write 'Amount' bytes from 'Buffer' to storage at position 'Position'.
		virtual void SDRStore(
			const byte__ *Buffer,
			size__ Amount,
			row_t__ Position )
		{
			qRFwk();
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
			return SDRAllocate( Size );
		}
		//f Recall 'Amount' at position 'Position' and put them into 'Buffer'. Return 'Buffer'.
		sSize Fetch(
			row_t__ Position,
			size__ Amount,
			byte__ *Buffer,
			qRPN)
		{
			return SDRFetch(Position, Amount, Buffer, qRP);
		}
		//f Store 'Amount' bytes from 'Buffer' at position 'Position'.
		void Store(
			const byte__ *Buffer,
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

//	#define qSD__	sdr::storage_driver__
}

namespace tol {
	inline void reset(
		bso::sBool,
		sdr::tRow &Row )
	{
		Row = qNIL;
	}
}

#endif
