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

#ifndef CDGB64__INC
#define CDGB64__INC

#define CDGB64_NAME		"CDGB64"

#define	CDGB64_VERSION	"$Revision: 1.12 $"

#define CDGB64_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( CDGB64_NODBG )
#define CDGB64_DBG
#endif


//D CoDinG Base 64 

#include "err.h"
#include "flw.h"
#include "bso.h"
#include "fdr.h"

#include <ctype.h>

namespace cdgb64 {

	extern bso::sByte *CommonEncoding_;
	extern bso::sByte *OriginalEncoding_;
	extern bso::sByte *URLEncoding_;

	qENUM( Flavor ) {
		fOriginal, // Using '+' and '/'.
		fURL, // Using '-' and '_'.
		f_amount,
		f_Undefined
	};

	inline bso::sByte Encode_(
		bso::sByte Value,
		eFlavor Flavor )
	{
		if ( Value <= 61 )
			return CommonEncoding_[ Value ];
		else if ( Value <= 63 ) {
			Value -= 62;

			switch ( Flavor ) {
			case fOriginal:
				return OriginalEncoding_[Value];
				break;
			case fURL:
				return URLEncoding_[Value];
				break;
			default:
				qRFwk();
				break;
			}
		} else
			qRFwk();

		return 0;	// To avoid a warning.
	}

	inline void Encode_(
		const flw::byte__ *Data,
		eFlavor Flavor,
		flw::byte__ *Result )
	{
		Result[0] = Encode_( Data[0] >> 2, Flavor );
		Result[1] = Encode_( ( ( Data[0] << 4 ) | ( Data[1] >> 4 ) ) & 0x3f, Flavor );
		Result[2] = Encode_( ( ( Data[1] << 2 ) | ( Data[2] >> 6 ) ) & 0x3f, Flavor );
		Result[3] = Encode_( Data[2] & 0x3f, Flavor );
	}

#	define CDGB64__PROCESSED	BSO_U8_MAX

	typedef fdr::oflow_driver___<> _oflow_driver___;

	class encoding_oflow_driver___
	: public _oflow_driver___
	{
	private:
		eFlavor Flavor_;
		fdr::byte__ _Cache[3];
		bso::u8__ _Amount;
		flw::oflow__ *_Flow;	
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum ) override
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
					Encode_( _Cache, Flavor_, Result );
					memset(_Cache, 0, sizeof( _Cache ) );
					_Flow->Write( Result, 4 );
					_Amount = 0;
					break;
				default:
					qRFwk();
					break;
				}
			}

			while ( Maximum >= 3 ) {
				Encode_( Buffer, Flavor_, Result );
				memset(_Cache, 0, sizeof( _Cache ) );

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
		virtual bso::sBool FDRCommit(
			bso::sBool Unlock,
			qRPN ) override
		{
			flw::byte__ Result[4];

			if ( _Flow == NULL )
				return true;

			if ( _Amount == CDGB64__PROCESSED )
				return true;

			Encode_( _Cache, Flavor_, Result );
			memset(_Cache, 0, sizeof( _Cache ) );

			switch ( _Amount ) {
			case 0:
				break;
			case 1:
				_Flow->Write( &Result, 2 );
				_Flow->Write( "==", 2 );
				break;
			case 2:
				_Flow->Write( &Result, 3 );
				_Flow->Put( '=' );
				break;
			default:
				break;
			}

			_Amount = CDGB64__PROCESSED;

			return _Flow->Commit( Unlock, ErrHandling );
		}
		virtual fdr::sTID FDRWTake( fdr::sTID Owner ) override
		{
			return _Flow->ODriver().WTake( Owner );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				Commit( true, err::hUserDefined );	// Errors are ignored.

			_oflow_driver___::reset( P );
			_Amount = CDGB64__PROCESSED;
			_Flow = NULL;
			Flavor_ = f_Undefined;
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
			eFlavor Flavor,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			Commit( true, err::hUserDefined );	// Errors are ignored.

			_Amount = CDGB64__PROCESSED;
			_Flow = &Flow;
			memset(_Cache, 0, sizeof( _Cache ) );

			_oflow_driver___::Init( ThreadSafety );

			Flavor_ = Flavor;
		}
	};

	typedef flw::standalone_oflow__<>	_oflow__;	// NOTA : Cache size MUST be greater as 3.

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
		qCDTOR( encoding_oflow___ );
		void Init(
			flw::oflow__ &Flow,
			eFlavor Flavor )
		{
			_Driver.Init( Flow, Flavor, fdr::tsDisabled );
			_oflow__::Init( _Driver  );

#ifdef CDGB64_DBG
			if ( GetCacheSize() < 3 )
				qRFwk();
#endif
		}
	};

	const str::string_ &Encode(
		const str::string_ &Plain,
		eFlavor Flavor,
		str::string_ &Encoded );

	typedef fdr::rRFlow rIFlowDriver_;

	class rSkippingIFlowDriver_
	: public rIFlowDriver_
	{
	private:
		qRMV( flw::rRFlow, F_, Flow_ );
	protected:
		virtual fdr::sSize FDRRead(
			fdr::sSize Maximum,
			fdr::sByte *Buffer ) override
		{
			fdr::sByte C = 0;
			fdr::sSize Amount = 0;

			while ( !F_().EndOfFlow() && ( Amount < Maximum ) ) {
				switch ( C = F_().Get() ) {
				case '-':
				case '+':
				case '/':
				case '_':
					Buffer[Amount++] = C;
					break;
				default:
					if ( isalnum(C) )
						Buffer[Amount++] = C;
					break;
				}
			}

			return Amount;
		}
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override
		{
			return F_().Dismiss( Unlock, ErrHandling );
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return F_().IDriver().RTake( Owner );
		}
	public:
		void reset( bso::sBool P = true )
		{
			rIFlowDriver_::reset( P );
			tol::reset( P, Flow_ );
		}
		qCVDTOR( rSkippingIFlowDriver_ );
		void Init(
			flw::rRFlow &Flow, 
			fdr::eThreadSafety ThreadSafety )
		{
			rIFlowDriver_::Init( ThreadSafety );
			Flow_ = &Flow;
		}
	};

	extern bso::byte__ *Decoding_;

	inline bso::byte__ Decode_( bso::byte__ Value )
	{
#ifdef CDGB64_DBG
		if ( Decoding_[Value] == '@' )
			qRFwk();
#endif
		return Decoding_[Value];
	}

	inline void Decode_(
		const fdr::byte__ *Source,	// Squence complte de 4 octets en b64, sans '='.
		fdr::byte__ *Target )	// Doit avoir la place suffisante pour recevoir la squence de 3 octets rsultante.
	{
		Target[0] = ( Decode_( Source[0] ) << 2 ) | ( Decode_( Source[1] ) >> 4 );
		Target[1] = ( Decode_( Source[1] ) << 4 ) | ( Decode_( Source[2] ) >> 2 );
		Target[2] = ( Decode_( Source[2] ) << 6 ) | Decode_( Source[3] );
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

	typedef fdr::iflow_driver___<1023> _iflow_driver___;

	class decoding_iflow_driver___
	: public _iflow_driver___
	{
	private:
		rSkippingIFlowDriver_ SkippingIFlowDriver_;
		flw::rDressedRFlow<> SkippingIFlow_;
		fdr::byte__ _Cache[4];
		bso::u8__ _Size;
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			bso::size__ Amount = 0;	// Is the amount of handled imput data, not the amount of resulting data, which is calculated on the end of this method.

			if ( !SkippingIFlow_.EndOfFlow() ) {
# ifdef CDGB64_DBG
				if ( Maximum < 4 )
					qRFwk();
# endif
				Maximum &= ~3UL;	// On veut un multiple de 4.

				if ( _Size != 0 ) {
					memcpy( Buffer, _Cache, _Size );
					Maximum -= _Size;
					Amount = _Size;
					_Size = 0;
				}

				while ( ( !SkippingIFlow_.IsCacheEmpty() || Amount < 4 ) && Maximum-- )
					Buffer[Amount++] = SkippingIFlow_.Get();

				if ( Amount != 0 ) {
					if ( Amount >= 4 )
						Decode_( Buffer, Amount & ~3UL );

					if ( Amount & 3 ) {
						_Size = Amount & 3;
						memcpy( _Cache, Buffer + ( Amount & ~3UL ), _Size );
						Amount &= ~3UL;
					}

				}
			} else if ( _Size != 0 ) {
				DecodePartial_( _Cache, _Size, Buffer );
				Amount = _Size;
				_Size = 0;
			}

			return 3 * ( Amount >> 2 ) + ( ( Amount & 3 ) > 1 ? ( Amount & 3 ) - 1 : 0 ); 
		}
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override
		{
			if ( _Size != 0 )
				qRFwk();

			return SkippingIFlow_.Dismiss( Unlock, ErrHandling );
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return SkippingIFlowDriver_.RTake( Owner );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_iflow_driver___::reset( P );
			tol::reset( P, SkippingIFlow_, SkippingIFlowDriver_ );

			_Size = 0;
		}
		qCVDTOR( decoding_iflow_driver___ );
		void Init(
			flw::iflow__ &Flow,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			_iflow_driver___::Init( ThreadSafety );
			_Size = 0;
			SkippingIFlowDriver_.Init( Flow, ThreadSafety );
			SkippingIFlow_.Init( SkippingIFlowDriver_ );
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
		qCDTOR( decoding_iflow___ );
		void Init( flw::iflow__ &Flow )
		{
			_Driver.Init( Flow );
			_iflow___::Init( _Driver  );
		}
	};

	const str::string_ &Decode(
		const str::string_ &Encoded,
		str::string_ &Plain );
}

/*************/
/**** NEW ****/
/*************/

namespace cdgb64 {
	typedef cdgb64::encoding_oflow___ rEncodingOFlow;
	typedef cdgb64::decoding_iflow___ rDecodingIFlow;

	class rEncodingODriver
	: public encoding_oflow_driver___
	{
	private:
		flw::rDressedWFlow<> Flow_;
	public:
		void reset( bso::sBool P = true )
		{
			encoding_oflow_driver___::reset( P );
			tol::reset( P, Flow_ );
		}
		qCVDTOR( rEncodingODriver );
		void Init(
			fdr::rWDriver &Driver,
			eFlavor Flavor,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			Flow_.Init( Driver );
			encoding_oflow_driver___::Init( Flow_, Flavor, ThreadSafety );
		}
	};

	class rDecodingIDriver
	: public decoding_iflow_driver___
	{
	private:
		flw::rDressedRFlow<> Flow_;
	public:
		void reset( bso::sBool P = true )
		{
			decoding_iflow_driver___::reset( P );
			tol::reset( P, Flow_ );
		}
		qCVDTOR( rDecodingIDriver );
		void Init(
			fdr::rRDriver &Driver,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			Flow_.Init( Driver );
			decoding_iflow_driver___::Init( Flow_, ThreadSafety );
		}
	};
}

#endif
