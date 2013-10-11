/*
	Header for the 'aem' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2001-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

//	$Id: aem.h,v 1.35 2013/04/26 14:43:40 csimon Exp $

#ifndef AEM__INC
#define AEM__INC

#define AEM_NAME		"AEM"

#define	AEM_VERSION	"$Revision: 1.35 $"

#define AEM_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &AEMTutor;

#if defined( E_DEBUG ) && !defined( AEM_NODBG )
#define AEM_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.35 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/26 14:43:40 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Amount/Extent managing 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "sdr.h"

//d The coefficient between step value and step size.
# define AEM_STEP_COEFFICIENT	256

namespace aem {

	//e Mode of the allocation.
	enum mode__ {
		//i Extent is handled using step value.
		mStepped,
		//i Extent fits with amount.
		mFit,
		m_amount,
		m_Undefined,
		m_Default = mStepped,
	};

	//t Type of a step value.
	typedef bso::u8__ step_value__;

	//t Type of a step size.
	typedef bso::u16__ step_size__;

	//c Core amount/extent manager. Internal use.
	template <typename row, typename size> class core_amount_extent_manager_
	{
	private:
		bso::bool__ OnlyGrowing_( void ) const
		{
			return ( S_.Misc & 0x80 ) != 0;
		}
		step_value__ StepValue_( void ) const
		{
			return (step_value__)( S_.Misc & 0x7f );
		}
		step_size__ StepSize_( void ) const
		{
			return (step_size__)( StepValue_() * AEM_STEP_COEFFICIENT );
		}
		sdr::size__ _GuessExtent( sdr::size__ Size )
		{
			step_size__ Step = StepSize_();

			return ( Size ? ( ( ( ( Size - 1 ) / Step ) + 1 ) * Step ) : 0 );
		}
		sdr::size__ Adjust_( sdr::size__ Size )
		{
			S_.Amount = Size;
			S_.Misc = _GuessExtent( Size ) | ( S_.Misc & 0xff );

			return S_.Misc & 0xffffff00;
		}
		sdr::size__ Extent_( void ) const
		{
			if ( S_.Misc <= 0xff )
				return S_.Amount;
			else
				return S_.Misc & 0xffffff00;
		}
		bso::bool__ Decrease_( sdr::size__ &Size )
		{
			if ( Extent_() >= ( StepSize_() + Size ) ) {
				Size = Adjust_( Size );
				return true;
			} else {
				S_.Amount = Size;
				return false;
			}
		}
		bso::bool__ Increase_( sdr::size__ &Size )
		{
			Size = Adjust_( Size );

			return true;
		}
	protected:
		/*f Return true if a allocation is needed for size 'Size'. 'Size' then contains
		the real size to allocate. If 'Mode' = 'mFit', 'Extent' is forced to be equal
		to 'Size'. */
		bso::bool__ AmountToAllocate(
			sdr::size__ &Size,
			mode__ Mode )
		{
			if ( Size == S_.Amount )
				return false;
			else if ( ( Mode == mFit ) || ( StepValue_() == 0 ) )
				return Force( Size );
			else if ( Size < S_.Amount ) {
				if ( !OnlyGrowing_() )
					return Decrease_( Size ) ;
				else {
					S_.Amount = Size;
					return false;
				}
			}
			else
				if ( Extent_() < Size )
					return Increase_( Size );
				else {
					S_.Amount = Size;
					return false;
				}
		}
		bso::bool__ Preallocate( sdr::size__ &Size )
		{
			sdr::size__ Extent = Extent_();

			Size = _GuessExtent( Size );

			if ( Size > Extent ) {
				S_.Misc = Size | ( S_.Misc & 0xff );
				return true;
			} else if ( Size < Extent_() ) {
				if ( S_.Amount > Size )
					ERRFwk();

				if ( !OnlyGrowing_() ) {
					S_.Misc = Size | ( S_.Misc & 0xff );
					return true;
				}
			}

			return false;
		}
		/*f Force the amount and extent to exactly 'Size'.
		Return true if the amount or the extent wasn't equal to 'Size'. */
		bso::bool__ Force( sdr::size__ Size )
		{
			bso::bool__ NotEqual = ( Extent() != Size ) || ( S_.Amount != Size );

			S_.Amount = Size;
			S_.Misc = S_.Misc & 0xff;	// Avoid the '&=' notation due to possible portable type of 'S_.Misc'.

			return NotEqual;
		}
	public:
		struct s {
		// First 3 bytes are the extent / 256.
		// Next first bit is at 1 if the we don't have to reallocate when size decrease.
		// Next seven bytes is the allocation step / 256.
		// If the allocation step at 0, then the extent fits with the amount
		// even in no decreasin state. */
			size Misc;
			size Amount;
		} &S_;
		core_amount_extent_manager_( s &S )
		: S_( S )
		{}
		void reset( bso::bool__ = true )
		{
			S_.Amount = 0;
			S_.Misc = 1;
		}
		core_amount_extent_manager_ &operator =( const core_amount_extent_manager_ &AEM )
		{
			// Amount normally handled by calling function.

			if ( AEM.StepValue_() != 0 )	// 'AEM' is a stand alone object.
				S_.Misc = 0;	
			else	// '*this' is a standalone object.
				S_.Misc = S_.Misc & 0xff;	// Avoid the '&=' notation due to possible portable type of 'S_.Misc'.

			return *this;
		}
		//f Initialization.
		void Init( size Amount = 0 )
		{
			S_.Amount = 0;
			Force( Amount );
		}
		//f Return the extent.
		sdr::size__ Extent( void ) const
		{
			return Extent_();
		}
		//f Return the amount.
		sdr::size__ Amount( void ) const
		{
			return S_.Amount;
		}
		//f Set the step to 'StepValue' (< 128).
		void SetStepValue( step_value__ StepValue )
		{
			if ( StepValue >= 128 )
				ERRPrm();

			S_.Misc = ( ( S_.Misc & 0xffffff40 ) | StepValue );

		}
		//f Return the step value.
		step_value__ GetStepValue( void ) const
		{
			return StepValue_();
		}
		//f Return the step size.
		step_size__ GetStepSize( void ) const
		{
			return StepSize_();
		}
		//f Return true if it grows only.
		bso::bool__ GetNoDecreasingState( void )
		{
			return OnlyGrowing_();
		}
		//f If 'State' at true, never decrease.
		void SetNoDecreasingState( bso::bool__ State )
		{
			S_.Misc |= ( S_.Misc & 0xffffff3f ) | ( State ? 0x80 : 0 );
		}
		//f Return position of the last object of the set.
		row Last( void ) const
		{
			if ( S_.Amount )
				return S_.Amount - 1;
			else
				return E_NIL;
		}
		row Last( sdr::size__ Offset ) const
		{
			row Row = Last();

			if ( Row != E_NIL )
				Row = Previous( Row, Offset  );

			return Row;
		}
		//f Return position of the first object of the set.
		row First( void ) const
		{
			if ( S_.Amount )
				return 0;
			else
				return E_NIL;
		}
		row First( sdr::size__ Offset ) const
		{
			row Row = First();

			if ( Row != E_NIL )
				Row =  Next( Row, Offset  );

			return Row;
		}
		//f Return the position of 'Offset' next to 'Current'.
		row Next(
			row Current,
			sdr::size__ Offset ) const
		{
			if ( ( *Current += Offset ) < S_.Amount )
				return Current;
			else
				return E_NIL;
		}
		//f Return the position of the object after 'Current' (to the top).
		row Next( row Current ) const
		{
			return Next( Current, 1 );
		}
		//f Return the position of 'Offset' previous to 'Current'.
		row Previous(
			row Current,
			sdr::size__ Offset ) const
		{
			if ( *Current >= Offset )
				return *Current - Offset;
			else
				return E_NIL;
		}
		//f Return the position of the object before 'Current' (to the bottom).
		row Previous( row Current ) const
		{
			return Previous( Current, 1 );
		}
		//f Return true if empty, false otherwise.
		bso::bool__ IsEmpty( void ) const
		{
			return S_.Amount == 0;
		}
		//f Return true if 'Row' exists, false otherwise.
		bso::bool__ Exists( row Row ) const
		{
			return *Row < S_.Amount;
		}
	};


	//c A amount/extent manager.
	template <typename row> class amount_extent_manager_
	: public core_amount_extent_manager_<row, sdr::size__>
	{
	public:
		struct s
		: core_amount_extent_manager_<row, sdr::size__>::s
		{};
		amount_extent_manager_( s &S )
		: core_amount_extent_manager_<row, sdr::size__>( S )
		{}
	};

# if 0
	//c A portable amount/extent manager.
	template <typename row> class p_amount_extent_manager_
	: public core_amount_extent_manager_<row, sdr::p_size__>
	{
	public:
		struct s
		: core_amount_extent_manager_<row, sdr::p_size__>::s
		{};
		p_amount_extent_manager_( s &S )
		: core_amount_extent_manager_<row, sdr::p_size__>( S )
		{}
	};
# endif

	//c Core amount/extent manager for fixed size set of object. Internal use only.
	template <int extent, typename row, typename size> class core_amount_extent_manager__
	{
	private:
		size Amount_;
	protected:
		/*f Return true if a allocation is needed for size 'Size'. 'Size' then contains
		the real size to allocate. */
		bso::bool__ AmountToAllocate(
			sdr::size__ &Size,
			mode__ Mode)
		{
			Amount_ = Size;
			return false;
		}
	public:
		struct s {};	// to simplify use in library 'BCH'.
		core_amount_extent_manager__( s &S = *(s *)NULL )
		{
			reset( false );
		}
		// Simplifies the 'SET' library.
		void reset( bso::bool__ = true )
		{
			Amount_ = 0;
		}
		core_amount_extent_manager__ &operator =( const core_amount_extent_manager__ &AEM )
		{
			// Amount normally already handled by calling function.

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Amount_ = 0;
		}
		//f Return the extent.
		sdr::size__ Extent( void ) const
		{
			return extent;
		}
		//f Return the amount.
		sdr::size__ Amount( void ) const
		{
			return Amount_;
		}
		//f Return position of the last object of the set.
		row Last( void ) const
		{
			if ( Amount_ )
				return Amount_ - 1;
			else
				return E_NIL;
		}
		row Last( sdr::size__ Offset ) const
		{
			row Row = Last();

			if ( Row != E_NIL )
				Row =  Previous( Row, Offset  );

			return Row;
		}
		//f Return position of the first object of the set.
		row First( void ) const
		{
			if ( Amount_ )
				return 0;
			else
				return E_NIL;
		}
		row First( sdr::size__ Offset ) const
		{
			row Row = First();

			if ( Row != E_NIL )
				Row = Next( Row, Offset  );

			return Row;
		}
		//f Return the position of 'Offset' next to 'Current'.
		row Next(
			row Current,
			sdr::size__ Offset ) const
		{
			if ( ( *Current += Offset ) < Amount_ )
				return Current;
			else
				return E_NIL;
		}
		//f Return the position of the object after 'Current' (to the top).
		row Next( row Current ) const
		{
			return Next( Current, 1 );
		}
		//f Return the position of 'Offset' previous to 'Current'.
		row Previous(
			row Current,
			sdr::size__ Offset ) const
		{
			if ( *Current >= Offset )
				return *Current - Offset;
			else
				return E_NIL;
		}
		//f Return the position of the object before 'Current' (to the bottom).
		row Previous( row Current ) const
		{
			return Previous( Current, 1 );
		}
		//f Return true if empty, false otherwise.
		bso::bool__ IsEmpty( void ) const
		{
			return Amount_ == 0;
		}
		//f Return true if 'Row' exists, false otherwise.
		bso::bool__ Exists( row Row ) const
		{
			return *Row < Amount_;
		}
	};

	//c Core amount/extent manager for fixed size set of object.
	template <int extent, typename row> class amount_extent_manager__
	: public core_amount_extent_manager__<extent, row, sdr::size__>
	{
	public:
		struct s
		: public core_amount_extent_manager__<extent, row, sdr::size__>::s
		{};
		amount_extent_manager__( s &S = *(s *)NULL )
		: core_amount_extent_manager__<extent, row, sdr::size__>( S )
		{}
	};

# if 0
	//c Core amount/extent manager for fixed size set of object.
	template <int extent, typename row> class p_amount_extent_manager__
	: public core_amount_extent_manager__<extent, row, sdr::p_size__>
	{
	public:
		struct s
		: public core_amount_extent_manager__<extent, row, sdr::p_size__>::s
		{};
		p_amount_extent_manager__( s &S = *(s *)NULL )
		: core_amount_extent_manager__<extent, row, sdr::p_size__>( S )
		{}
	};
# endif

	//c A amount/extent manager.
	template <typename row> class amount_extent_manager___
	: public amount_extent_manager_<row>
	{
	public:
		struct s
		: amount_extent_manager_<row>::s
		{};
		amount_extent_manager___( s &S )
		: amount_extent_manager_<row>( S )
		{}
	};

# if 0
	//c A portable amount/extent manager.
	template <typename row> class p_amount_extent_manager___
	: public p_amount_extent_manager_<row>
	{
	public:
		struct s
		: p_amount_extent_manager_<row>::s
		{};
		p_amount_extent_manager___( s &S )
		: p_amount_extent_manager_<row>( S )
		{}
	};
# endif

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
