/*
  Header for the 'fhd' library by Claude SIMON (http://zeusw.org/intl/contact.html)
  Copyright (C) 2000 Claude SIMON (http://zeusw.org/intl/contact.html) 

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
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: fhd.h,v 1.4 2012/11/14 16:06:27 csimon Exp $

#ifndef FHD__INC
#define FHD__INC

#define FHD_NAME		"FHD"

#define	FHD_VERSION	"$Revision: 1.4 $"	

#define FHD_OWNER		"the Epeios project (http://zeusw.org/epeios/)"

#include "ttr.h"

extern class ttr_tutor &FHDTutor;

#if defined( E_DEBUG ) && !defined( FHD_NODBG )
#define FHD_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2012/11/14 16:06:27 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D Flow HanDler. OBSOLETE ! Use library 'FLW' instead.
/* End addendum to automatic documentation generation part. */

#error "Obsolete. Use library 'FLW' instead."

#include "err.h"

namespace fhd {
	//t Amount of data red/written.
	typedef size_t			amount__;

	//t Size (of a cache, for example).
	typedef size_t			size__;

	//t Type of the data
	typedef char			data__;

	//c The base class for an input flow driver.
	class iflow_driver___
	{
	private:
		// The cache.
		data__ *Cache_;
		// Size of the cache.
		size__ Size_;
		// Amount of data available in the cache.
		amount__ Available_;
		// Position of the available data.
		size__ Position_;
		amount__ Get_(
			amount__ Minimum,
			data__ *Buffer,
			amount__ Wanted )
		{
			return FHDGet( Minimum, Buffer, Wanted );
		}
#ifdef FHD_DBG
		// Test if there is a cache available.
		void Test_( void )
		{
			if ( ( Cache_ == NULL ) )
				ERRu();
				
			if ( Size_ == 0 )
				ERRu();
		}				
#endif		
		// Fill the cache with a minimum of 'Minimum' bytes. The cache must be empty.
		void FillCache_( amount__ Minimum )
		{
#ifdef FHD_DBG
			Test_();

			if ( Available_ != 0 )				
				ERRc();
#endif
			Available_ = Get_( Minimum, Cache_, Size_ );
			Position_ = 0;
		}
		/* Read from cache up to 'Amount' bytes and place them to 'Buffer'.
		Return amount of data red. */
		amount__ ReadFromCache_(
			amount__ Amount,
			data__ *Buffer )
		{
#ifdef FHD_DBG
			Test_();
#endif
			if ( Available_ < Amount )
				Amount = Available_;
				
			if ( Amount != 0 ) {
				memcpy( Buffer, Cache_ + Position_, Amount );
				
				Position_ += Amount;
				Available_ -= Amount;
			}
			
			return Amount;
		}
		/* Put a minimum of 'Minimum' and up to 'Wanted' bytes in 'Buffer',
		directly or through the cache. return amount of byte red.
		The cache MUST be empty. */
		amount__ ReadFromCacheOrDirectly_(
			amount__ Minimum,
			data__ *Buffer,
			amount__ Wanted )
		{
#ifdef FHD_DBG
			Test_();

			if ( Available_ != 0 )				
				ERRc();
#endif
			if ( Size_ > Minimum ) {
				FillCache_( Minimum );
				return ReadFromCache_( Wanted, Buffer );
			} else
				return Get_( Minimum, Buffer, Wanted );
		}
	protected:
		/*v Place up to 'Wanted' bytes in 'Buffer', and not less than 'Minimum'.
		Return how many bytes red. If 'Minimum' == 0, then the function must return
		as fast as possible if there is no data to read. */
		virtual amount__ FHDGet(
			amount__ Minimum,
			data__ *Buffer,
			amount__ Wanted ) = 0;
	public:
		void reset( bool = true )
		{
			Cache_ = NULL;
			Available_ = Position_ = Size_ = 0;
		}
		iflow_driver___( void )
		{
			reset( false );
		}
		virtual ~iflow_driver___( void )
		{
			reset( true );
		}
		//f Initialization with 'Cache' of size 'Size' as cache.
		void Init(
			data__ *Cache,
			size__ Size )
		{
			reset();
			
			Cache_ = Cache;
			Size_ = Size;
		}
		/*f Place up to 'Number' bytes in 'Buffer' whith a minimum of 'Minimum'.
		Return number of bytes red. */
		amount__ GetUpTo(
			amount__ Amount,
			data__ *Buffer,
			amount__ Minimum = 0 )
		{
			amount__ Available = ReadFromCache_( Amount, Buffer );

			if ( ( Available < Minimum ) || ( Available == 0 ) )
				Available += ReadFromCacheOrDirectly_( Minimum - Available, Buffer + Available, Amount  - Available );

			return Available;
		}
		//f Place 'Amount' bytes in 'Buffer'.
		void Get(
			amount__ Amount,
			data__ *Buffer )
		{
			amount__ Available = ReadFromCache_( Amount, Buffer );

			if ( Amount > Available )
				Available += ReadFromCacheOrDirectly_( Amount - Available, Buffer + Available, Amount - Available );
		}

	};
	
	//t An input flow handler;
	typedef iflow_driver___ *iflow_handler__;

	//f Delete the input flow handler 'Handler'.
	inline void Delete( iflow_handler__ Handler )
	{
		delete Handler;
	}
	

	//c The base class for an output flow handler.
	class oflow_driver___
	{
	private:
		// The cache.
		data__ *Cache_;
		// The size of the cache.
		size__ Size_;
		// The amount of bytes yet free.
		amount__ Free_;
		amount__ Put_(
			const data__ *Buffer,
			amount__ Wanted,
			amount__ Minimum,
			bool Synchronization )
		{
			return FHDPut( Buffer, Wanted, Minimum, Synchronization );
		}
		/* Try to dump (vider) the cache. Return true if succeed,, false otherwise.
		'Synchornization is at true if a synchonization is requested. */
		bool DumpCache_( bool Synchronization )
		{
			amount__ Stayed;
			amount__ AmountWritten;
			
			if ( ( Stayed = Size_ - Free_ ) != 0 ) {
				AmountWritten = Put_( Cache_, Stayed, Stayed, Synchronization );
				
				if ( AmountWritten < Stayed )
					memmove( Cache_, Cache_ + AmountWritten, Stayed - AmountWritten );
					
				Free_ += AmountWritten;
			}
			
			return Free_ == Size_;
		}
		/* Force the dumping (vidage) of the cache. 'Synchronization' at true
		if synchronization required. */
		void ForceDumpingOfCache_( bool Synchronization );				
		/* Write up to 'Amount' bytes from 'Buffer' into the cxache.
		Return amount of bytes written. */
		amount__ WriteIntoCache_(
			const data__ *Buffer,
			amount__ Amount )
		{
			if ( Free_ < Amount )
				Amount = Free_;
				
			memcpy( Cache_ + Size_ - Free_, Buffer, Amount );
			
			Free_ -= Amount;
			
			return Amount;
		}
		/* Put up to 'Amount' bytes from 'Buffer' directly or through the cache.
		Return amount of bytes written. Cache MUST be EMPTY. */
		amount__ WriteIntoCacheOrDirectly_(
			const data__ *Buffer,
			amount__ Amount )
		{
#ifdef FHD_DBG
			if ( Size_ != Free_ )
				ERRc();
#endif
			if ( Amount > Size_ )
				return Put_( Buffer, Amount - 1, Amount - 1, false );
				// The '-1' is to be sure that at least 1 byte is in the cache when a 'Synchronize' occurs.
			else
				return WriteIntoCache_( Buffer, Amount );
		}
		// Force writing of 'Amount( bytes of 'Buffer'.
		void ForceWriting_(
			const data__ *Data,
			amount__ Amount );
		//fSynchronization.
		void Synchronize_( void )
		{
			if ( !DumpCache_( true ) )
				ForceDumpingOfCache_( true );
		}
	protected:
		/*v Put up to 'Wanted' and a minimum of 'Minimum' bytes from 'Buffer'.
		If 'Synchronization' is true, then this fonction is called due to a
		synchronization operation. Return the amount of data written.
		If 'Minimum' == 0, then the function must return as fast as
		possible if no data can be written. */
		virtual amount__ FHDPut(
			const data__ *Buffer,
			amount__ Wanted,
			amount__ Minimum,
			bool Synchronization ) = 0;
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( Size_ != Free_ )
					Synchronize_();
			}
			
			Cache_ = NULL;
			
			Free_ = Size_ = 0;
		}
		oflow_driver___( void )
		{
			reset( false );
		}
		virtual ~oflow_driver___( void )
		{
			reset( true );
		}
		//f Initialization with 'Cache' of size 'Size' as cache.
		void Init(
			data__ *Cache,
			size__ Size )
		{
			reset();
			
			Cache_ = Cache;
			Size_ = Free_ = Size;
		}
		//f Put up to 'Amount' bytes from 'Buffer'. Return number of bytes written.
		amount__ PutUpTo(
			const data__ *Buffer,
			amount__ Amount )
		{
			amount__ AmountWritten = WriteIntoCache_( Buffer, Amount );

			if ( ( AmountWritten == 0 )  && ( Amount != 0 ) )
			{
				if ( DumpCache_( false ) )
					AmountWritten = WriteIntoCacheOrDirectly_( Buffer, Amount );
				else
					AmountWritten = WriteIntoCache_( Buffer, Amount );
			}

			return AmountWritten;
		}
		//f Put 'Amount' data from 'Buffer'.
		void Put(
			const data__ *Buffer,
			amount__ Amount )
		{
			amount__ AmountWritten = PutUpTo( Buffer, Amount );

			if ( AmountWritten < Amount )
				ForceWriting_( Buffer + AmountWritten, Amount - AmountWritten );
		}
		//f Synchronization.
		void Synchronize( void )
		{
			Synchronize_();
		}

	};
	
	//t An output flow handler;
	typedef oflow_driver___ *oflow_handler__;

	//f Delete the output flow handler 'Handler'.
	inline void Delete( oflow_handler__ Handler )
	{
		delete Handler;
	}

	//c The base class for an input/output flow driver.
	class ioflow_driver___
	: public iflow_driver___,
	  public oflow_driver___
	{
	public:
		void reset( bool P = true )
		{
			iflow_driver___::reset( P );
			oflow_driver___::reset( P );
		}
		ioflow_driver___( void )
		{
			reset( false );
		}
		virtual ~ioflow_driver___( void )
		{
			reset( true );
		}
		/*f Initialization with 'ICache' of size 'ISize' and 'OCache' of size
		'OSize' as, respectively cache and size of this cache for the input flow
		and output flow. */
		void Init(
			data__ *ICache,
			size__ ISize,
			data__ *OCache,
			size__ OSize )
		{
			iflow_driver___::Init( ICache, ISize );
			oflow_driver___::Init( OCache, OSize );
		}
		//f Initialisation with cache 'Cache' of size 'Size'.
		void Init(
			data__ *Cache,
			size__ Size )
		{
			Init( Cache, Size / 2, Cache + Size / 2, Size / 2 );
		}

	};

	//t A flow handler;
	typedef ioflow_driver___ *ioflow_handler__;

	//f Delete the input/output flow handler 'Handler'.
	inline void Delete( ioflow_handler__ Handler )
	{
		delete Handler;
	}

	//d The value of an invalid input/output flow handler.
	#define FHD_INVALID_FLOW_HANDLER	NULL
};

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
