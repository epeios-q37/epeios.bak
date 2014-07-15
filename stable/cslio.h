/*
	Header for the 'cslio' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: cslio.h,v 1.11 2012/11/14 16:06:26 csimon Exp $

#ifndef CSLIO__INC
#define CSLIO__INC

#define CSLIO_NAME		"CSLIO"

#define	CSLIO_VERSION	"$Revision: 1.11 $"

#define CSLIO_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &CSLIOTutor;

#if defined( E_DEBUG ) && !defined( CSLIO_NODBG )
#define CSLIO_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.11 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:26 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D C Standard Library Input/Output 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "bso.h"
#include <stdio.h>
#include "tol.h"

namespace cslio {
	typedef size_t	amount__;

	typedef FILE *descriptor__;

#define CSLIO_UNDEFINED_DESCRIPTOR	NULL
	E_CDEF( descriptor__, UndefinedDescriptor, NULL );

	class io_core__ {
	protected:
		descriptor__ _D;
		void _Test( void ) const
		{
#ifdef CSLIO_DBG
			if ( _D == CSLIO_UNDEFINED_DESCRIPTOR )
				ERRPrm();
#endif
		}
	public:
		void reset( bso::bool__ = true )
		{
			_D = CSLIO_UNDEFINED_DESCRIPTOR;
		}
		io_core__( void )
		{
			reset( false );
		}
		virtual ~io_core__( void )
		{
			reset();
		}
		void Init( descriptor__ D )
		{
			reset();

			_D = D;
		}
		void Seek( long Offset )
		{
			_Test();

			if ( fseek( _D, Offset, SEEK_SET ) != 0 )
				ERRFwk();
		}
		long Size( void )
		{
			_Test();

			if ( fseek( _D, 0, SEEK_END ) != 0 )
				ERRFwk();

			return ftell( _D );
		}
	};


	class standard_input__
	: public virtual io_core__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			io_core__::reset( P );
		}
		standard_input__( void )
		{
			reset( false );
		}
		virtual ~standard_input__( void )
		{
			reset();
		}
		void Init( descriptor__ D )
		{
			io_core__::Init( D );
		}
		size_t Read(
			amount__ Amount,
			void *Buffer )
		{
			_Test();

			if ( _D == stdin ) {
				*(char *)Buffer = getc( _D );	// To avoid the buffering of '\n'.
				return 1;
			} else
				return fread( Buffer, 1, Amount, _D );
		}
		bso::bool__ OnEOF( void )
		{
			_Test();

			return feof( _D ) != 0;
		}
	};

	class standard_output__
	: public virtual io_core__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			io_core__::reset( P );
		}
		standard_output__( void )
		{
			reset( false );
		}
		virtual ~standard_output__( void )
		{
			reset();
		}
		void Init( descriptor__ D )
		{
			io_core__:: Init( D );
		}
		size_t Write(
			const void *Buffer,
			amount__ Amount )
		{
			_Test();

			return fwrite( Buffer, 1, Amount, _D );
		}
		void Flush( void )
		{
			_Test();

			fflush( _D );
		}
	};

	class standard_io__
	: public standard_output__,
	  public standard_input__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			standard_output__::reset( P );
			standard_input__::reset( P );
		}
		standard_io__( void )
		{
			reset( false );
		}
		virtual ~standard_io__( void )
		{
			reset();
		}
		void Init( descriptor__ D )
		{
			standard_output__::Init( D );
			standard_input__::Init( D );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
