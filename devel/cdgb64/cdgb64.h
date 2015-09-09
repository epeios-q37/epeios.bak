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

#ifndef CDGB64__INC
#define CDGB64__INC

#define CDGB64_NAME		"CDGB64"

#define	CDGB64_VERSION	"$Revision: 1.12 $"

#define CDGB64_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( CDGB64_NODBG )
#define CDGB64_DBG
#endif


/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

//D CoDinG Base 64 

#include "err.h"
#include "flw.h"
#include "bso.h"
#include "fdr.h"

namespace cdgb64 {

	extern bso::byte__ *Encode;

	inline bso::byte__ Encode_( flw::byte__ Value )
	{
#ifdef CDGB64_DBG
		if ( Value > 63 )
			qRFwk();
#endif
		return Encode[Value];
	}

	inline void Encode_(
		const flw::byte__ *Data,
		flw::byte__ *Result )
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
		fdr::byte__ _Cache[3];
		bso::u8__ _Amount;
		flw::oflow__ *_Flow;	
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum )
		{
			flw::byte__ Result[4];
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
					qRFwk();
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
					qRFwk();
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
					qRFwk();

				if ( _Amount != 0 )
					qRFwk();
#endif
				memcpy( _Cache, Buffer, Maximum );

				_Amount = (bso::u8__)Maximum;
				Amount += Maximum;
			}

			return Amount;
		}
		virtual void FDRCommit( void )
		{
			flw::byte__ Result[4];

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
				qRFwk();
#endif
		}
	};

	extern bso::byte__ *Decode;

	inline bso::byte__ Decode_( bso::byte__ Value )
	{
#ifdef CDGB64_DBG
		if ( Decode[Value] == '@' )
			qRFwk();
#endif
		return Decode[Value];
	}

	inline void Decode_(
		const fdr::byte__ *Source,	// Squence complte de 4 octets en b64, sans '='.
		fdr::byte__ *Target )	// Doit avoir la place suffisante pour recevoir la squence de 3 octets rsultante.
	{
		Target[0] = Decode_( Source[0] ) | ( Decode_( Source[1] ) << 6 );
		Target[1] = ( Decode_( Source[1] ) >> 2 ) | ( Decode_( Source[2] ) << 4 );
		Target[2] = ( Decode_( Source[2] ) >> 4 ) | ( Decode_( Source[3] ) << 2 );
	}

	inline void Decode_(
		fdr::byte__ *Data,
		fdr::size__ Amount )
	{
		const fdr::byte__ *Source = Data;
		fdr::byte__ *Target = Data;
#ifdef CDGB64_DBG
		if ( Amount & 3 )	// Si pas un multiple de 4.
			qRFwk();
#endif
		while ( Amount >= 4 ) {
			Decode_( Source, Target );
			Amount -= 4;
			Source += 4;
			Target += 3;
		}
	}

	inline void DecodePartial_(
		const flw::byte__ *Source,
		flw::size__ Amount,
		fdr::byte__ *Target )
	{
		memmove( Target, Source, 4 );

		switch ( Amount ) {
		case 0:
			qRFwk();
			break;
		case 1:
			qRFwk();
			break;
		case 2:
			Target[2] = 'A';
		case 3:
			Target[3] = 'A';
			break;
		default:
			qRFwk();
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
		fdr::byte__ _Cache[4];
		bso::u8__ _Size;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer )
		{
			bso::size__ Amount = 0;
			flw::byte__ Datum;
			bso::bool__ CacheIsEmpty = false;

#ifdef CDGB64_DBG
			if ( Maximum < 4 )
				qRFwk();
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
				Buffer[Amount++] = (flw::byte__)Datum;
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
				qRFwk();
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

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
