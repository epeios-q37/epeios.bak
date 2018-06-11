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

//	$Id: cslio.h,v 1.11 2012/11/14 16:06:26 csimon Exp $

#ifndef CSLIO__INC
#define CSLIO__INC

#define CSLIO_NAME		"CSLIO"

#define	CSLIO_VERSION	"$Revision: 1.11 $"

#define CSLIO_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( CSLIO_NODBG )
#define CSLIO_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.11 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:26 $

/* End of automatic documentation generation part. */

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
				qRFwk();
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
				qRFwk();
		}
		long Size( void )
		{
			_Test();

			if ( fseek( _D, 0, SEEK_END ) != 0 )
				qRFwk();

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
#endif
