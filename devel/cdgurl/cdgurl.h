/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

// CoDinG URL.
// Based on http://www.geekhideout.com/urlcode.shtml.

#ifndef CDGURL__INC
# define CDGURL__INC

# define CDGURL_NAME		"CDGURL"

# if defined( E_DEBUG ) && !defined( CDGURL_NODBG )
#  define CDGURL_DBG
# endif

# include "fdr.h"
# include "flw.h"
# include "err.h"
# include "tol.h"

namespace cdgurl {
	typedef fdr::oflow_driver___<> rOFlowDriver_;

	inline fdr::byte__ ToHex_( fdr::byte__ Byte) {
	  static fdr::byte__ Hex[] = "0123456789abcdef";
	  return Hex[Byte&15];
	}

	class rEncodeDriver
	: public rOFlowDriver_
	{
	private:
		qRMV( flw::oflow__, F_, Flow_ );
	protected:
		virtual fdr::size__ FDRWrite(
			const fdr::byte__ *Buffer,
			fdr::size__ Maximum ) override
		{
			fdr::byte__ Byte = 0;
			fdr::size__ Written = 0;

			while ( Written < Maximum ) {
				switch ( Byte = Buffer[Written++] ) {
				default:
					if ( !isalnum( Byte ) ) {
						fdr::byte__ Buffer[3] = { '%' };
						Buffer[1] = ToHex_( Byte >> 4 );
						Buffer[2] = ToHex_( Byte & 15 );
						F_().Write( Buffer, sizeof( Buffer ) );
						break;
					}
				case '-':
				case '_':
				case '.':
				case '~':
					F_().Put( Byte );
					break;
				case ' ':
					F_().Put( '+' );
					break;
				}
			}

			return Written;
		}
		virtual void FDRCommit( void ) override
		{
			F_().Commit();
		}
	public:
		void reset( bso::sBool P = true )
		{
			rOFlowDriver_::reset( P );
			Flow_ = NULL;
		}
		qCVDTOR( rEncodeDriver );
		void Init( flw::oflow__ &Flow )
		{
			rOFlowDriver_::Init( fdr::ts_Default );
			Flow_ = &Flow;
		}
	};

	typedef flw::standalone_oflow__<>	sOFlow_;

	class rEncodingOFlow
	: public sOFlow_
	{
	private:
		rEncodeDriver Driver_;
	public:
		void reset( bso::bool__ P = true )
		{
			sOFlow_::reset( P );
			Driver_.reset( P );
		}
		qCDTOR( rEncodingOFlow );
		void Init( flw::oflow__ &Flow )
		{
			Driver_.Init( Flow );
			sOFlow_::Init( Driver_  );
		}
	};

	const str::string_ &Encode(
		const str::string_ &Plain,
		str::string_ &Encoded );

	typedef fdr::iflow_driver___<> rIFlowDriver_;

	inline fdr::byte__ FromHex_( fdr::byte__ Byte )
	{
		if ( !isxdigit( Byte ) )
			qRFwk();

		return isdigit( Byte ) ? Byte - '0' : tolower( Byte ) - 'a' + 10;
	}

	class rDecodeDriver
	: public rIFlowDriver_
	{
	private:
		qRMV( flw::iflow__, F_, Flow_ );
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			fdr::size__ Red = 0;
			bso::sSize Available = 0;
			fdr::byte__ Byte = 0;
			bso::sBool Stop = F_().EndOfFlow() || ( Red >= Maximum );

			while ( !Stop ) {
				switch ( Byte = F_().View() ) {
				case '+':
					F_().Skip();
					Buffer[Red++] = ' ';
					break;
				case '%':
					if ( F_().IsCacheEmpty( &Available ) )
						qRFwk();

					if ( ( Available >= 3 ) || ( Red == 0 ) ) {
						F_().Skip();
						Buffer[Red++] = ( FromHex_( F_().Get() ) << 4 ) + FromHex_( F_().Get() );
					} else
						Stop = true;
					break;
				default:
					Buffer[Red++] = F_().Get();
					break;
				}

				Stop = Stop || ( Red >= Maximum ) || F_().IsCacheEmpty() || F_().EndOfFlow();
			}

			return Red;
		}
		virtual void FDRDismiss( void ) override
		{
			F_().Dismiss();
		}
	public:
		void reset( bso::sBool P = true )
		{
			rIFlowDriver_::reset( P );
			Flow_ = NULL;
		}
		qCVDTOR( rDecodeDriver );
		void Init( flw::iflow__ &Flow )
		{
			rIFlowDriver_::Init( fdr::ts_Default );
			Flow_ = &Flow;
		}
	};

	typedef flw::standalone_iflow__<> sIFlow_;

	class rDecodingIflow
	: public sIFlow_
	{
	private:
		rDecodeDriver Driver_;
	public:
		void reset( bso::bool__ P = true )
		{
			sIFlow_::reset( P );
			Driver_.reset( P );
		}
		qCDTOR( rDecodingIflow );
		void Init( flw::iflow__ &Flow )
		{
			Driver_.Init( Flow );
			sIFlow_::Init( Driver_  );
		}
	};

	const str::string_ &Decode(
		const str::string_ &Encoded,
		str::string_ &Plain );
}

#endif
