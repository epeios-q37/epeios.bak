/*
	Header for the 'ltf' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

//	$Id: ltf.h,v 1.25 2013/04/11 19:36:50 csimon Exp $

#ifndef LTF__INC
#define LTF__INC

#define LTF_NAME		"LTF"

#define	LTF_VERSION	"$Revision: 1.25 $"

#define LTF_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &LTFTutor;

#if defined( E_DEBUG ) && !defined( LTF_NODBG )
#define LTF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.25 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/11 19:36:50 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Line Text Flow. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "fdr.h"
#include "flw.h"
#include "txf.h"

namespace ltf {
	#define LTF__SIZE_MAX	BSO_U8_MAX

	typedef fdr::oflow_driver___<> _oflow_driver___;

	class _line_text_oflow_driver___
	: public _oflow_driver___
	{
	private:
		fdr::datum__ *Data_;
		bso::u8__ Size_;
		bso::u8__ Amount_;
		txf::text_oflow__ *TFlow_;
		bso::u8__ _FreezePosition;
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			if ( ( Amount_ + Maximum ) > Size_ ) {
				if ( Maximum >= (fdr::size__)( Size_ - _FreezePosition ) ) {
					memcpy( Data_ + _FreezePosition, Buffer + ( Size_ - _FreezePosition - Maximum ), Size_ - _FreezePosition );
				} else {
					memcpy( Data_ + _FreezePosition, Data_ + _FreezePosition + Maximum, Size_ - Maximum - _FreezePosition );
					memcpy( Data_ + Size_ - Maximum, Buffer, Maximum );
				}

				Amount_ = Size_ + 1;
			} else {
				memcpy( Data_ + Amount_, Buffer, Maximum );
				Amount_ += (bso::u8__)Maximum;
			}

			if ( Amount_ < Size_ ) {
				TFlow_->Put( Buffer, Maximum );
			} else if ( Amount_ > Size_ )
				Data_[_FreezePosition] = '<';

			return Maximum;
		}
		virtual void FDRCommit( void )
		{
			*TFlow_ << txf::commit;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_oflow_driver___::reset( P );
		}
		_line_text_oflow_driver___( void )
		{
			reset( false );
		}
		virtual ~_line_text_oflow_driver___( void )
		{
			reset();
		}
		void Init(
			txf::text_oflow__ &TFlow,
			fdr::datum__ *Data,
			fdr::size__ Size,
			fdr::thread_safety__ ThreadSafety )
		{
			if ( Size > LTF__SIZE_MAX )
				ERRLmt();

			TFlow_ = &TFlow;

			_oflow_driver___::Init( ThreadSafety );
			
			Data_ = Data;

			Size_ = (bso::u8__)Size;

			memset( Data_, ' ', Size_ );
			Data_[Size_] = 0;
			Amount_ = 0;
			_FreezePosition = 0;

			Clear();
		}
		void Clear( void )
		{
			memset( Data_ + _FreezePosition, ' ', Size_ - _FreezePosition);
			*TFlow_ << txf::rfl;
			TFlow_->Put( Data_, Size_ );
			CR();
		}
		void ClearAll( void )
		{
			_FreezePosition = 0;

			Clear();
		}
		void CR( void )
		{
			*TFlow_ << txf::rfl;
			TFlow_->Put( Data_, _FreezePosition );
			Amount_ = _FreezePosition;
		}
		void Freeze( void )
		{
			_FreezePosition = Amount_;
		}
	};




	class _line_text_oflow___
	: public flw::oflow__
	{
	private:
		_line_text_oflow_driver___ _Driver;
	public:
		void reset( bso::bool__ P = true )
		{
			flw::oflow__::reset( P );
			_Driver.reset( P );
		}
		_line_text_oflow___(  )
		{
			reset( false );
		}
		virtual ~_line_text_oflow___( void )
		{
			reset();
		}
		void Init(
			txf::text_oflow__ &TFlow,
			flw::datum__ *Data,
			flw::size__ Size )
		{
			_Driver.Init( TFlow, Data, Size, fdr::tsDisabled );
			oflow__::Init( _Driver, NULL, 0, FLW_AMOUNT_MAX );
		}
		void Clear( void )
		{
			_Driver.Clear();
		}
		void ClearAll( void )
		{
			_Driver.ClearAll();
		}
		void CR( void )
		{
			_Driver.CR();
		}
		void Freeze( void )
		{
			_Driver.Freeze();
		}
	};


	template <int size = 79> class line_text_oflow___
	: public txf::text_oflow__
	{
	private:
		flw::datum__ Data_[size];
	protected:
		_line_text_oflow___ Flow_;
	public:
		void reset( bso::bool__ P = true )
		{
			text_oflow__::reset( P );
			Flow_.reset( P );
		}
		line_text_oflow___( void )
		{
			reset( false );
		}
		virtual ~line_text_oflow___( void )
		{
			reset();
		}
		void Init( txf::text_oflow__ &TFlow )
		{
			Flow_.Init( TFlow, Data_, sizeof( Data_ ) );
			txf::text_oflow__::Init( Flow_ );
		}
		void CR( void )
		{
			Flow_.CR();
		}
		void Clear( void )
		{
			Flow_.Clear();
		}
		void ClearAll( void )
		{
			Flow_.ClearAll();
		}
		void Freeze( void )
		{
			Flow_.Freeze();
		}
	};

/*
	template <int size> inline line_console__<size> & CR( line_console__<size> &Flow )
	{
		Flow.Line_.CR();

		return *this;
	}
*/

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
