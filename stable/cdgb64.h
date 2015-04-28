/*
	Header for the 'cdgb64' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20112004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
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

//	$Id: cdgb64.h,v 1.12 2013/04/26 14:43:40 csimon Exp $

#ifndef CDGB64__INC
#define CDGB64__INC

#define CDGB64_NAME		"CDGB64"

#define	CDGB64_VERSION	"$Revision: 1.12 $"

#define CDGB64_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( CDGB64_NODBG )
#define CDGB64_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.12 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/26 14:43:40 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D CoDinG Base 64 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bso.h"
#include "fdr.h"

namespace cdgb64 {

	extern bso::raw__ *Encode;

	inline bso::raw__ Encode_( flw::datum__ Value )
	{
#ifdef CDGB64_DBG
		if ( Value > 63 )
			ERRPrm();
#endif
		return Encode[Value];
	}

	inline void Encode_(
		const flw::datum__ *Data,
		flw::datum__ *Result )
	{
		Result[0] = Encode_( Data[0]  & 0x3f );
		Result[1] = Encode_( ( ( Data[0] >> 6 ) | ( ( Data[1] << 2 ) & 0x3f ) ) );
		Result[2] = Encode_( ( ( Data[1] >> 4 ) | ( ( Data[2] << 4 ) & 0x3f ) ) );
		Result[3] = Encode_( Data[2] >> 2 );
	}

#	define CDGB64__PROCESSED	BSO_U8_MAX

	typedef fdr::oflow_driver___<> _oflow_driver___;

	class encoding_oflow_driver___
	: public _oflow_driver___
	{
	private:
		fdr::datum__ _Cache[3];
		bso::u8__ _Amount;
		flw::oflow__ *_Flow;	
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::datum__ *Buffer,
			fdr::size__ Maximum )
		{
			flw::datum__ Result[4];
			fdr::size__ Amount = 0;

			if ( _Amount == CDGB64__PROCESSED )
				_Amount = 0;

			if ( _Amount != 0 ) {
				if ( (fdr::size__)( 3 - _Amount ) < Maximum )
					Amount = 3 - _Amount;
				else
					Amount = Maximum;

				memcpy( _Cache + _Amount, Buffer, Amount );

				_Amount += (bso::u8__)Amount;
				Maximum -= Amount;
				Buffer += Amount;

				switch ( _Amount ) {
				case 0:
					ERRFwk();
					break;
				case 1:
				case 2:
					break;
				case 3:
					Encode_( _Cache, Result );
					_Flow->Write( Result, 4 );
					_Amount = 0;
					break;
				default:
					ERRFwk();
					break;
				}
			}

			while ( Maximum >= 3 ) {
				Encode_( Buffer, Result );

				_Flow->Write( Result, 4 );

				Amount += 3;
				Maximum -= 3;
				Buffer += 3;

			}

			if ( Maximum != 0 ) {
#ifdef CDGB64_DBG
				if ( Maximum > 3 )
					ERRPrm();

				if ( _Amount != 0 )
					ERRFwk();
#endif
				memcpy( _Cache, Buffer, Maximum );

				_Amount = (bso::u8__)Maximum;
				Amount += Maximum;
			}

			return Amount;
		}
		virtual void FDRCommit( void )
		{
			flw::datum__ Result[4];

			if ( _Flow == NULL )
				return;

			if ( _Amount == CDGB64__PROCESSED )
				return;

			Encode_( _Cache, Result );

			switch ( _Amount ) {
			case 0:
				break;
			case 1:
				_Flow->Write( &Result, 2 );
				break;
			case 2:
				_Flow->Write( &Result, 3 );
				break;
			default:
				break;
			}

			_Flow->Put( '=' );

			_Amount = CDGB64__PROCESSED;

			_Flow->Commit();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				Commit( _Flow );

			_oflow_driver___::reset( P );
			_Amount = CDGB64__PROCESSED;
			_Flow = NULL;
		}
		encoding_oflow_driver___( void )
		{
			reset( false );
		}
		~encoding_oflow_driver___( void )
		{
			reset();
		}
		void Init(
			flw::oflow__ &Flow,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			Commit( _Flow );

			_Amount = CDGB64__PROCESSED;
			_Flow = &Flow;

			_oflow_driver___::Init( ThreadSafety );
		}
	};

	typedef flw::standalone_oflow__<>	_oflow__;	// NOTA : la taille du cache doit tre suprieur ou gal  3.

	class encoding_oflow___
	: public _oflow__
	{
	private:
		encoding_oflow_driver___ _Driver;
	public:
		void reset( bso::bool__ P = true )
		{
			_oflow__::reset( P );
			_Driver.reset( P );
		}
		encoding_oflow___( void )
		{
			reset( false );
		}
		~encoding_oflow___( void )
		{
			reset();
		}
		void Init(
			flw::oflow__ &Flow,
			flw::size__ AmountMax = FLW_AMOUNT_MAX )
		{
			_Driver.Init( Flow, fdr::tsDisabled );
			_oflow__::Init( _Driver, AmountMax  );

#ifdef CDGB64_DBG
			if ( GetCacheSize() < 3 )
				ERRFwk();
#endif
		}
	};

	extern bso::raw__ *Decode;

	inline bso::raw__ Decode_( bso::raw__ Value )
	{
#ifdef CDGB64_DBG
		if ( Decode[Value] == '@' )
			ERRFwk();
#endif
		return Decode[Value];
	}

	inline void Decode_(
		const fdr::datum__ *Source,	// Squence complte de 4 octets en b64, sans '='.
		fdr::datum__ *Target )	// Doit avoir la place suffisante pour recevoir la squence de 3 octets rsultante.
	{
		Target[0] = Decode_( Source[0] ) | ( Decode_( Source[1] ) << 6 );
		Target[1] = ( Decode_( Source[1] ) >> 2 ) | ( Decode_( Source[2] ) << 4 );
		Target[2] = ( Decode_( Source[2] ) >> 4 ) | ( Decode_( Source[3] ) << 2 );
	}

	inline void Decode_(
		fdr::datum__ *Data,
		fdr::size__ Amount )
	{
		const fdr::datum__ *Source = Data;
		fdr::datum__ *Target = Data;
#ifdef CDGB64_DBG
		if ( Amount & 3 )	// Si pas un multiple de 4.
			ERRPrm();
#endif
		while ( Amount >= 4 ) {
			Decode_( Source, Target );
			Amount -= 4;
			Source += 4;
			Target += 3;
		}
	}

	inline void DecodePartial_(
		const flw::datum__ *Source,
		flw::size__ Amount,
		fdr::datum__ *Target )
	{
		memmove( Target, Source, 4 );

		switch ( Amount ) {
		case 0:
			ERRPrm();
			break;
		case 1:
			ERRPrm();
			break;
		case 2:
			Target[2] = 'A';
		case 3:
			Target[3] = 'A';
			break;
		default:
			ERRFwk();
			break;
		}

		Decode_( Target, 4 );
	}

	typedef fdr::iflow_driver___<1023> _iflow_driver____;

	class decoding_iflow_driver___
	: public _iflow_driver____
	{
	private:
		flw::iflow__ *_Flow;
		fdr::datum__ _Cache[4];
		bso::u8__ _Size;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			bso::size__ Amount = 0;
			flw::datum__ Datum;
			bso::bool__ CacheIsEmpty = false;

#ifdef CDGB64_DBG
			if ( Maximum < 4 )
				ERRPrm();
#endif
			Maximum &= ~3UL;	// On veut un multiple de 4.

			if ( _Size != 0 ) {
				memcpy( Buffer, _Cache, _Size );
				Maximum -= _Size;
				Amount = _Size;
				_Size = 0;
			}

			while ( ( !CacheIsEmpty || Amount < 4 ) && Maximum-- && ( ( Datum = _Flow->Get() ) != '=' ) ) {
				CacheIsEmpty = _Flow->IsCacheEmpty();
				Buffer[Amount++] = (flw::datum__)Datum;
			}

			if ( Amount != 0 ) {
				if ( Amount >= 4 )
					Decode_( Buffer, Amount & ~3UL );

				if ( Amount & 3 ) {
					if ( Datum == '=' )
						DecodePartial_( Buffer + ( Amount & ~3UL ), Amount & 3, Buffer + 3 * ( Amount >> 2 ) );
					else {
						_Size = Amount & 3;
						memcpy( _Cache, Buffer + ( Amount & ~3UL ), _Size );
						Amount &= ~3UL;
					}
				}

			}

			return 3 * ( Amount >> 2 ) + ( ( Amount & 3 ) > 1 ? ( Amount & 3 ) - 1 : 0 ); 
		}
		virtual void FDRDismiss( void )
		{
#ifdef CDGB64_DBG
			if ( _Flow == NULL )
				ERRFwk();
#endif
			_Flow->Dismiss();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_iflow_driver____::reset( P );
			_Flow = NULL;
			_Size = 0;
		}
		decoding_iflow_driver___( void )
		{
			reset( false );
		}
		~decoding_iflow_driver___( void )
		{
			reset();
		}
		void Init(
			flw::iflow__ &Flow,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			_Flow = &Flow;
			_iflow_driver____::Init( ThreadSafety );
			_Size = 0;
		}
	};

	typedef flw::standalone_iflow__<> _iflow___;

	class decoding_iflow___
	: public _iflow___
	{
	private:
		decoding_iflow_driver___ _Driver;
	public:
		void reset( bso::bool__ P = true )
		{
			_iflow___::reset( P );
			_Driver.reset( P );
		}
		void Init(
			flw::iflow__ &Flow,
			flw::size__ AmountMax = FLW_AMOUNT_MAX )
		{
			_Driver.Init( Flow );
			_iflow___::Init( _Driver, AmountMax  );
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
