/*
	Header for the 'pllio' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: pllio.h,v 1.19 2013/04/06 14:50:46 csimon Exp $

#ifndef PLLIO__INC
#define PLLIO__INC

#define PLLIO_NAME		"PLLIO"

#define	PLLIO_VERSION	"$Revision: 1.19 $"

#define PLLIO_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &PLLIOTutor;

#if defined( E_DEBUG ) && !defined( PLLIO_NODBG )
#define PLLIO_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.19 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/06 14:50:46 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D POSIX Low-Level Input/Output 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

namespace pllio {
	using bso::size__;

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
				ERRFwk();
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
				ERRLbr();
		}
		long Size( void )
		{
			long Size;

			_Test();

			if ( ( Size = lseek( _D, 0, SEEK_END ) ) == -1 )
				ERRLbr();

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

			if ( Amount > SSIZE_MAX )
				Amount = SSIZE_MAX;

			if ( ( Amount = read( _D, Buffer, Amount ) ) == -1 )
				ERRLbr();

			return Amount;
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
					ERRLbr();

				if ( ( Position = lseek( _D, 0, SEEK_CUR ) ) == -1 )
					ERRLbr();

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

			if ( Amount > SSIZE_MAX )
				Amount = SSIZE_MAX;

			if ( ( Amount = write( _D, Buffer, Amount ) ) == -1 )
				ERRLbr();

			return Amount;
		}
		void Flush( void )
		{
			if ( _D != PLLIO_UNDEFINED_DESCRIPTOR ) {
				fsync( _D );

//				fdatasync( _D );	// N'existe que sous Linux. Alternative à 'fsync()', sauf que les meta-données (herure de modification, p. ex.) ne sont pas stockées, économisant une écriture disque.
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
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
