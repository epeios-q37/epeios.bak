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

//	$Id: utf.h,v 1.4 2013/07/18 19:46:23 csimon Exp $

#ifndef UTF__INC
#define UTF__INC

#define UTF_NAME		"UTF"

#define	UTF_VERSION	"$Revision: 1.4 $"

#define UTF_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( UTF_NODBG )
#define UTF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.4 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/07/18 19:46:23 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D UCS transformation format 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "tol.h"
# include "bomhdl.h"

namespace utf {

	template <typename feeder> inline bso::bool__ ControlUTF8TailingBytes_(
		bso::u8__ Amount,
		feeder &Feeder )
	{
		while ( !Feeder.IsEmpty() && Amount ) {
			if ( ( Feeder.Get() & 0xC0 ) != 0x80 )
				return false;

			Amount--;
		}

		if ( Amount != 0 )
			return false;

		return true;
	}

	inline bso::u8__ FindFirst0_( fdr::byte__ C )
	{
		bso::u8__ Pos = 8;

		while ( Pos-- && ( ( C & ( 1 << Pos ) ) != 0 ) );

		return Pos + 1;
	}

	template <typename feeder> inline bso::u8__ HandleUTF8( feeder &Feeder )
	{
		bso::u8__ LeadingByteLast0Position = 0;

		if ( Feeder.IsEmpty() )
			qRFwk();	// Erreur.

		LeadingByteLast0Position = FindFirst0_( Feeder.Get() );

		switch ( LeadingByteLast0Position ) {
		case 8:
			return 1;
			break;
		case 7:
			return 0;
			break;
		case 6:
		case 5:
		case 4:
		{
			bso::u8__ Amount = 8 - LeadingByteLast0Position;

			if ( !ControlUTF8TailingBytes_( Amount - 1, Feeder ) )
				return 0;

			return Amount;
		}
		break;
		case 3:
		case 2:
		case 1:
		case 0:
			return 0;
			break;
		default:
			qRFwk();
			break;
		}

		return 0;	// Pour viter un 'warning'.
	}

	template <typename feeder> inline bso::u8__ HandleANSI( feeder &Feeder )
	{
		if ( Feeder.IsEmpty() )
			qRFwk();	// Erreur.

		if ( HandleUTF8( Feeder ) > 1 )
			return 0;
		else
			return 1;
	}

	enum format__ {
		fANSI,
		fUTF_8,
		fUTF_16_LE,
		fUTF_16_BE,
		fUTF_32_LE,
		fUTF_32_BE,
		f_amount,
		f_Undefined,
		f_Guess,	// Tenter de deviner si c'est de l'ANSI ou de l'UTF-8.
		f_Default = f_Guess
	};

	template <typename feeder> class utf__ {
	private:
		format__ _Format;
	public:
		void reset( bso::bool__ P = true )
		{
			_Format = f_Undefined;
		}
		E_CDTOR( utf__ );
		bso::bool__ Init( format__ Format )	// Retourne 'false' si format non support, 'true' sinon.
		{
			_Format = Format;

			switch( _Format ) {
			case f_Guess:
			case fANSI:
			case fUTF_8:
				break;
			case fUTF_16_LE:
			case fUTF_16_BE:
			case fUTF_32_LE:
			case fUTF_32_BE:
				return false;
				break;
			default:
				qRFwk();
				break;
			}

			return true;
		}
		bso::u8__ Handle( feeder &Feeder )
		{
			if ( Feeder.IsEmpty() )
				qRFwk();

			switch( _Format ) {
			case f_Guess:
			{
				bso::u8__ UTFSize = HandleUTF8( Feeder );

				if ( UTFSize > 1 ) {
					if ( !Feeder.IsEmpty() ) {

						fdr::byte__ Datum = Feeder.Get();

						switch ( Datum >> 6 ) {
						case 0:
						case 1:
							break;
						case 2:
							UTFSize = 0;
							break;
						case 3:
							if ( FindFirst0_( Datum ) < 4 )
								UTFSize = 0;
							break;
						default:
							qRFwk();
							break;
						}
					}
				}

				if ( UTFSize  == 0 ) {
					_Format = fANSI;
					UTFSize = HandleANSI( Feeder );
				} else if ( UTFSize > 1 )
					_Format = fUTF_8;

				return UTFSize;
				break;
			}
			case fANSI:
				return HandleANSI( Feeder );
				break;
			case fUTF_8:
				return HandleUTF8( Feeder );
				break;
			default:
				qRFwk();
				break;
			}

			return 0;	// Pour viter iun 'warning'.
		}
		bso::bool__ SetFormat(
			format__ Format,
			err::handling__ ERRHandling = err::h_Default )
		{
			if ( Format == f_Guess )
				qRFwk();

			if ( _Format == f_Guess ) {
				if ( (Format != fUTF_8) && (Format != fANSI) ) {
					if ( ERRHandling != err::hUserDefined )
						qRFwk();
					else
						return false;
				}

				_Format = Format;
			} else if ( _Format != Format ) {
				if ( ERRHandling != err::hUserDefined )
					qRFwk();
				else
					return false;
			}

			return true;
		}
		E_RODISCLOSE__( format__, Format );

	};
}

/*$END$*/
#endif
