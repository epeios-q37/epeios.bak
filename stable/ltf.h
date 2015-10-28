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

//	$Id: ltf.h,v 1.25 2013/04/11 19:36:50 csimon Exp $

#ifndef LTF__INC
#define LTF__INC

#define LTF_NAME		"LTF"

#define	LTF_VERSION	"$Revision: 1.25 $"

#define LTF_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"


#if defined( E_DEBUG ) && !defined( LTF_NODBG )
#define LTF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.25 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2013/04/11 19:36:50 $

/* End of automatic documentation generation part. */

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
		fdr::datum__ *_Data;
		bso::u8__ _Size;
		bso::u8__ _Amount;
		txf::text_oflow__ *_TFlow;
		bso::u8__ _FreezePosition;
		txf::text_oflow__ &_TF( void )
		{
			if ( _TFlow == NULL )
				qRFwk();

			return *_TFlow;
		}
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			if ( ( _Amount + Maximum ) > _Size ) {
				if ( Maximum >= (fdr::size__)( _Size - _FreezePosition ) ) {
					memcpy( _Data + _FreezePosition, Buffer + ( _Size - _FreezePosition - Maximum ), _Size - _FreezePosition );
				} else {
					memcpy( _Data + _FreezePosition, _Data + _FreezePosition + Maximum, _Size - Maximum - _FreezePosition );
					memcpy( _Data + _Size - Maximum, Buffer, Maximum );
				}

				_Amount = _Size + 1;
			} else {
				memcpy( _Data + _Amount, Buffer, Maximum );
				_Amount += (bso::u8__)Maximum;
			}

			if ( _Amount < _Size ) {
				_TF().Put( Buffer, Maximum );
			} else if ( _Amount > _Size )
				_Data[_FreezePosition] = '<';

			return Maximum;
		}
		virtual void FDRCommit( void )
		{
			_TF() << txf::commit;
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
				qRLmt();

			_TFlow = &TFlow;

			_oflow_driver___::Init( ThreadSafety );
			
			_Data = Data;

			_Size = (bso::u8__)Size;

			memset( _Data, ' ', _Size );
			_Data[_Size] = 0;
			_Amount = 0;
			_FreezePosition = 0;

			Clear();
		}
		txf::text_oflow__ &TFlow( void )
		{
			return _TF();
		}
		void Clear( void )
		{
			memset( _Data + _FreezePosition, ' ', _Size - _FreezePosition);
			_TF() << txf::rfl;
			_TF().Put( _Data, _Size );
			CR();
		}
		void ClearAll( void )
		{
			_FreezePosition = 0;

			Clear();
		}
		void CR( void )
		{
			_TF() << txf::rfl;
			_TF().Put( _Data, _FreezePosition );
			_Amount = _FreezePosition;
		}
		void Freeze( void )
		{
			_FreezePosition = _Amount;
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
		txf::text_oflow__ &TFlow( void )
		{
			return _Driver.TFlow();
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
		txf::text_oflow__ &TFlow( void )
		{
			return Flow_.TFlow();
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
#endif
