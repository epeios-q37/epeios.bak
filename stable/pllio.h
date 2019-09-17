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

//	$Id: pllio.h,v 1.19 2013/04/06 14:50:46 csimon Exp $

#ifndef PLLIO_INC_
#define PLLIO_INC_

#define PLLIO_NAME		"PLLIO"

#define	PLLIO_VERSION	"$Revision: 1.19 $"

#define PLLIO_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"


#if defined( E_DEBUG ) && !defined( PLLIO_NODBG )
#define PLLIO_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.19 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/06 14:50:46 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D POSIX Low-Level Input/Output 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

namespace pllio {
	typedef bso::size__ size__;
	qCDEF( size__, SizeMax, BSO_SIZE_MAX );

	typedef int	descriptor__;

#define	PLLIO_UNDEFINED_DESCRIPTOR	-1

	class io_core__
	{
	protected:
		descriptor__ _D;
		void _Test( void ) const
		{
#ifdef PLLIO_DBG
			if ( _D == PLLIO_UNDEFINED_DESCRIPTOR )
				qRFwk();
#endif
		}
	public:
		void reset( bso::bool__ = true )
		{
			_D = PLLIO_UNDEFINED_DESCRIPTOR;
		}
		io_core__( void )
		{
			reset( false );
		}
		virtual ~io_core__( void )
		{
			reset();
		}
		void Seek( long Offset )
		{
			_Test();

			if ( lseek( _D, Offset, SEEK_SET ) != Offset )
				qRLbr();
		}
		long Size( void )
		{
			long Size;

			_Test();

			if ( ( Size = lseek( _D, 0, SEEK_END ) ) == -1 )
				qRLbr();

			return Size;
		}
		void Init( descriptor__ D )
		{
			_D = D;
		}
		bso::bool__ IsSTDIN( void ) const
		{
			return fileno( stdin ) == _D;
		}
		bso::bool__ IsSTDOUT( void ) const
		{
			return fileno( stdout ) == _D;
		}
		bso::bool__ IsSTDERR( void ) const
		{
			return fileno( stderr ) == _D;
		}
	};

	class lowlevel_input__
	: public virtual io_core__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			io_core__::reset( P );
		}
		lowlevel_input__( void )
		{
			reset( false );
		}
		~lowlevel_input__( void )
		{
			reset();
		}
		void Init( descriptor__ D )
		{
			io_core__::Init( D );
		}
		unsigned int Read(
			size__ Amount,
			void *Buffer )
		{
			_Test();

			ssize_t Result = 0;

			if ( Amount > SSIZE_MAX )
				Amount = SSIZE_MAX;

			if ( ( Result = read( _D, Buffer, Amount ) ) == -1 )
				qRLbr();

			if ( Result < 0 )
				qRLbr();

			return Result;
		}
		bso::bool__ OnEOF( void )
		{
			if ( IsSTDIN() ) {
				return feof( stdin ) != 0;
			} else {
				struct stat Stat;
				off_t Position;

				_Test();

				if ( fstat( _D, &Stat ) != 0 )
					qRLbr();

				if ( ( Position = lseek( _D, 0, SEEK_CUR ) ) == -1 )
					qRLbr();

				return Position >= Stat.st_size;
			}
		}
	};

	class lowlevel_output__
	: public virtual io_core__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			io_core__::reset( P );
		}
		lowlevel_output__( void )
		{
			reset( false );
		}
		~lowlevel_output__( void )
		{
			reset();
		}
		void Init( descriptor__ D )
		{
			io_core__::Init( D );
		}
		int Write(
			const void *Buffer,
			size__ Amount )
		{
			_Test();

			ssize_t Result = 0;

			if ( Amount > SSIZE_MAX )
				Amount = SSIZE_MAX;

			if ( ( Result = write( _D, Buffer, Amount ) ) == -1 )
				qRLbr();

			if ( Result < 0 )
				qRLbr();

			return Result;
		}
		void Flush( void )
		{
			if ( _D != PLLIO_UNDEFINED_DESCRIPTOR ) {
				fsync( _D );

//				fdatasync( _D );	// Doesn't exists in all POSIX systems. 'fsync()' does the same job, but meta-data (modification time, for ex.) is not updated, saving a write operation.
			}
		}
	};

	class lowlevel_io__
	: public lowlevel_output__,
	  public lowlevel_input__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			lowlevel_output__::reset( P );
			lowlevel_input__::reset( P );
		}
		lowlevel_io__( void )
		{
			reset( false );
		}
		~lowlevel_io__( void )
		{
			reset();
		}
		void Init( descriptor__ D )
		{
			lowlevel_output__::Init( D );
			lowlevel_input__::Init( D );
		}
	};
}

/*$END$*/
#endif
