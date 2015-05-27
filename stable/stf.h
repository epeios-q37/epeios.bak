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

//	$Id: stf.h,v 1.14 2012/11/14 16:06:37 csimon Exp $

#ifndef STF__INC
#define STF__INC

#define STF_NAME		"STF"

#define	STF_VERSION	"$Revision: 1.14 $"

#define STF_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( STF_NODBG )
#define STF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.14 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:37 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D STream (or STandard) Flow 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#error "Obsolete ! Use 'CIO' library instead !".

#include <iostream>

#include "err.h"
#include "flw.h"

#ifndef STF_STREAM_FLOW_BUFFER_SIZE
//d Buffer size for a stream output flow. 100 by default.
#define STF_STREAM_FLOW_BUFFER_SIZE	100
#endif


namespace stf {
	using flw::oflow__;
	using flw::iflow__;
	
	//c A stream output flow driver.
	class stream_oflow__
	: public oflow__
	{
	private:
		std::ostream &Stream_;
		flw::datum__ Cache_[STF_STREAM_FLOW_BUFFER_SIZE];
	protected:
		virtual flw::amount__ FLWPut(
			const flw::datum__ *Tampon,
			flw::amount__ Nombre,
			flw::amount__,
			bool Synchronize )
		{
#ifdef STF_DBG
			if ( ( Tampon == NULL ) && Nombre )
				ERRu();
#endif
			if ( Stream_.write( (char *)Tampon, (long)Nombre ).fail() )
			{
				if ( Tampon == NULL )
					Stream_.clear();	// Some compiler (BC++ V5.5) doesn't like 'Tampon' = NULL, even if 'Nombre' = 0.
				else
					ERRd();
			}

			if ( Synchronize )
				Stream_.flush();

			return Nombre;
		}
	public:
		void reset( bool P = true )
		{
			oflow__::reset( P );
		}
		stream_oflow__( std::ostream &Stream )
		: Stream_( Stream ),
		  oflow__()
		{
			reset( false );
		}
		virtual ~stream_oflow__( void )
		{
			reset( true );
		}
		//f Initialization.
		void Init( void )
		{
			reset();
		
			oflow__::Init( Cache_, sizeof( Cache_ ) );
		}
	};


	namespace {			
		//c Internal use. Core of an input flow from an 'istream'.
		class stream_iflow_core__
		: public iflow__
		{
		private:
			flw::datum__ Cache_[STF_STREAM_FLOW_BUFFER_SIZE];
		protected:
			// Le stream en question.
			std::istream &Stream_;
			flw::amount__ HandleAmount_(
				flw::amount__ Minimum,
				flw::datum__ *Tampon,
				flw::amount__ Desire,
				flw::amount__ AmountRead )
			{
				if ( AmountRead < Minimum )
				{
					if ( !Stream_.eof() )
						ERRd();
					else
						AmountRead += iflow__::HandleEOFD( Tampon + AmountRead, Desire - AmountRead );

					if ( AmountRead < Minimum )
						ERRd();
				}

				return AmountRead;
			}
		public:
			void reset( bool = true )
			{
			}
			stream_iflow_core__( std::istream &Stream )
			: Stream_( Stream )
			{
				reset( false );
			}
			virtual ~stream_iflow_core__( void )
			{
				reset( true );
			}
			//f Initialisation.
			void Init( void )
			{
				iflow__::Init( Cache_, sizeof( Cache_ ) );
			}
		};
	}

	//c Internal use. File input flow from an 'istream'.
	class stream_iflow__
	: public stream_iflow_core__
	{
	protected:
		virtual flw::amount__ FLWGet(
			flw::amount__ Minimum,
			flw::datum__ *Tampon,
			flw::amount__ Desire )
		{
	#ifdef STF_DBG
			if( Tampon == NULL )
				ERRu();
	#endif
			flw::amount__ NombreLus = 0;

			if ( !Stream_.eof() )
			{
				if ( Stream_.read( (char *)Tampon, (long)Desire ).bad() )
					ERRd();

				NombreLus = (flw::amount__)Stream_.gcount();
			}

			return HandleAmount_( Minimum, Tampon, Desire, NombreLus );
		}
	public:
		void reset( bool P = true )
		{
			stream_iflow_core__::reset( P );
		}
		stream_iflow__( std::istream &Stream )
		: stream_iflow_core__( Stream )
		{
			reset( false );
		}
		virtual ~stream_iflow__( void )
		{
			reset( true );
		}
		//f Initialisation.
		void Init( void )
		{
			stream_iflow_core__::Init();
		}
	};



	//c Internal use. File input flow from an 'istream'.
	class stream_iflow_line__
	: public stream_iflow_core__
	{
	protected:
		virtual flw::amount__ FLWGet(
			flw::amount__ Minimum,
			flw::datum__ *Tampon,
			flw::amount__ Desire )
		{
			flw::amount__ NombreLus = 0;

			if ( !Stream_.eof() )
			{
				if ( Stream_.getline( (char *)Tampon, (long)Desire ).bad() )
					ERRd();

				NombreLus = (flw::amount__)Stream_.gcount();

				if ( NombreLus && !Tampon[NombreLus-1] )
					Tampon[NombreLus-1]='\n';
			}

			return HandleAmount_( Minimum, Tampon, Desire, NombreLus );
		}
	public:
		void reset( bool P = true )
		{
			stream_iflow_core__::reset( P );
		}
		stream_iflow_line__( std::istream &Stream )
		: stream_iflow_core__( Stream )
		{
			reset( false );
		}
		virtual ~stream_iflow_line__( void )
		{
			reset( true );
		}
		//f Initialisation.
		void Init( void )
		{
			stream_iflow_core__::Init();
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
