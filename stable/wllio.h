/*
	Header for the 'wllio' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: wllio.h,v 1.22 2013/04/05 16:33:45 csimon Exp $

#ifndef WLLIO__INC
#define WLLIO__INC

#define WLLIO_NAME		"WLLIO"

#define	WLLIO_VERSION	"$Revision: 1.22 $"

#define WLLIO_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &WLLIOTutor;

#if defined( E_DEBUG ) && !defined( WLLIO_NODBG )
#define WLLIO_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.22 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/05 16:33:45 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Windows Low-Level Input/Output 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "cpe.h"
#include "err.h"
#include "bso.h"
#ifdef CPE__CYGWIN
#	include <mingw/io.h>
#else
#	include <io.h>
#endif
#include <fcntl.h>

namespace wllio {

	typedef bso::u64__	size__;	// LA taille d'un fichier est toujours cod� en 64 bits, m�me pour une architectue 32 bits.

	typedef int descriptor__;

#define WLLIO_UNDEFINED_DESCRIPTOR	-1

	class io_core__
	{
	protected:
		descriptor__ _D;
		void _Test( void ) const
		{
#ifdef WLLIO_DBG
			if ( _D == WLLIO_UNDEFINED_DESCRIPTOR )
				ERRFwk();
#endif
		}
		unsigned int _Patch( size__ Amount )
		{
			return (unsigned int)( Amount > INT_MAX ? INT_MAX : Amount );
			// Comme les fonctions ('_reae', '_write_'...) retournent un 'int' pour indiquer le nombre d'octets effectivement trair�s,
			// Je suppose que l'on ne peut faire trait� que 'INT_MAX' octets � la fois.
		}
	public:
		void reset( bso::bool__ = true )
		{
			_D = WLLIO_UNDEFINED_DESCRIPTOR;
		}
		io_core__( void )
		{
			reset( false );
		}
		virtual ~io_core__( void )
		{
			reset();
		}
		void Seek( size__ Offset )
		{
			_Test();

			if ( _lseeki64( _D, Offset, SEEK_SET ) != Offset )
				ERRLbr();
		}
/*		long Size( void )
		{
			long Size;

			_Test();

			if ( ( Size = _lseek( _D, 0, SEEK_END ) ) == -1 )
				ERRd();

			return Size;
		}
*/		void Init( descriptor__ D )
		{
			_D = D;
		}
		bso::bool__ IsSTDIN( void ) const
		{
			return _fileno( stdin ) == _D;
		}
		bso::bool__ IsSTDOUT( void ) const
		{
			return _fileno( stdout ) == _D;
		}
		bso::bool__ IsSTDERR( void ) const
		{
			return _fileno( stderr ) == _D;
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
		bso::size__ Read(
			bso::size__ Amount,
			void *Buffer )
		{
			int Result;

			if ( ( Result = _read( _D, Buffer, _Patch( Amount ) ) ) == -1 )
				ERRLbr();
			else if ( Result < 0 )
				ERRSys();

			return Result;
		}
		bso::bool__ OnEOF( void )
		{
			_Test();

			if ( IsSTDIN() ) {
				return feof( stdin ) != 0;
			} else 
				switch( _eof( _D ) ) {
				case 1:
					return true;
					break;
				case 0:
					return false;
					break;
				default:
	#ifdef WLLIO_DBG
					int Error = errno;	// Pour le d�bogage.
	#endif
					if ( _D == _fileno( stdin ) )	// Si 'stdin'.
						return false;
					else
						ERRSys();
					return true;	// To avoid a warning.
					break;
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
		size__ Write(
			const void *Buffer,
			size__ Amount )
		{
			int Result;

			if ( ( Result = _write( _D, Buffer, _Patch( Amount ) ) ) == -1 )
				ERRLbr();
			else if ( Result < 0 )
				ERRSys();

			return Result;
		}
		void Flush( void )
		{
			/*if ( _D != WLLIO_UNDEFINED_DESCRIPTOR )
				if ( !IsSTDOUT() && !IsSTDERR() )	// '_commit' n'est pas utilisable sur 'stdout' et 'stderr' (et accessoirement sur 'stdin').
					if ( _commit( _D ) != 0 )
						ERRLbr();
						*/
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
