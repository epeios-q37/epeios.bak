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

// eXtended Text Flow. Text flow with extended features.

#ifndef XTF_INC_
# define XTF_INC_

# define XTF_NAME		"XTF"

# if defined( E_DEBUG ) && !defined( XTF_NODBG )
#  define XTF_DBG
# endif

# include "err.h"
# include "bso.h"
# include "str.h"
# include "bomhdl.h"
# include "utf.h"

//d The default cell separator.
#define XTF_DEFAULT_CELL_SEPARATOR	'\t'

// Predeclaration.
namespace lcl {
	class meaning_;
};

/**************/
/**** OLD *****/
/**************/

namespace xtf {
	//t type of position in a text (line or column).
	typedef bso::uint__ location__;

	enum error__ {
		eUnsupportedEncoding,
		eUnexpectedEncoding,
		eEncodingDiscrepancy,
		e_amount,
		e_Undefined,
		e_NoError = e_Undefined
	};

	inline const char *GetLabel( error__ Error )
	{
		switch ( Error ) {
		case eUnsupportedEncoding:
			return XTF_NAME "_UnsupportedEncoding";
			break;
		case eUnexpectedEncoding:
			return XTF_NAME "_UnexpectedEncoding";
			break;
		case eEncodingDiscrepancy:
			return XTF_NAME "_EncodingDiscrepancy";
			break;
		default:
			qRFwk();
			break;
		}

		return NULL;	// Pour viter un 'Warning'.
	}

	struct utf__ {
		fdr::byte__ Data[5];
		bso::u8__ Size;
		bso::sBool Eaten;
		void reset( bso::bool__ = true )
		{
			Size = 0;
			Eaten = false;
		}
		E_CDTOR( utf__ )
		void Init( void )
		{
			Eaten = true;
			Size = 0;
		}
	};

	// Position dans le flux.
	struct pos__ {
		location__ Line;
		location__ Column;
		void reset( bso::bool__ = false )
		{
			Line = Column = 0;
		}
		pos__( void )
		{
			reset( true );
		}
		pos__(
			location__ Line,
			location__ Column )
		{
			reset( true );

			this->Line = Line;
			this->Column = Column;
		}
		void Init( pos__ Position = pos__() )
		{
			reset();

			*this = Position;
		}
	};

	typedef bso::u8__ _amount__;

	struct feeder__ {
	private:
		flw::iflow__ *_Flow;
		bso::size__ _Amount;
		bso::size__ _Length;
		fdr::byte__ _Data[10];
		flw::iflow__ &_F( void )
		{
			if ( _Flow == NULL )
				qRFwk();

			return *_Flow;
		}
		bso::bool__ _FillData( void )
		{
			bso::size__ NewLength = _Amount + 1;

			if ( NewLength > ( sizeof( _Data ) / sizeof( _Data[0] ) ) )
				qRLmt();

			if ( _Length < NewLength ) {
				if ( ( _Length = _F().View( NewLength, _Data ) ) < NewLength )
					return false;
				else
					return true;
			} else
				return true;
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Flow = NULL;
			_Amount = _Length = 0;
		}
		E_CDTOR( feeder__ )
		void Init( flw::iflow__ &Flow )
		{
			_Flow = &Flow;
			_Amount = _Length = 0;
		}
		void Reset( void )
		{
			_Amount = _Length = 0;
		}
		bso::bool__ IsEmpty( void )
		{
			if ( _Length == 0 )
				return _F().EndOfFlow();
			else
				return !_FillData();
		}
		fdr::byte__ Get( void )
		{
			if ( !_FillData() )
				qRFwk();

			return _Data[_Amount++];
		}
	};


	//c To handle a text flow, with counting lines and columns.
	class extended_text_iflow__
	{
	private:
		// L'entree de base.
		flw::iflow__ *_Flow;
		bso::sBool NLWasRegular_;   // Means that a dual NL wa encountered and that the first one was handled as a regular character (no line incrementation).
		// Position du prochain caractre.
		pos__ _Position;
		feeder__ _Feeder;
		utf::utf__<feeder__> _UTFHandler;
		utf__ _UTF;
		error__ _Error;
		flw::iflow__ &_F( void ) const
		{
			if ( _Flow == NULL )
				qRFwk();

			return *_Flow;
		}
		void _NewCharAdjust( void )
		{
			_Position.Column++;
		}
		// Adjust counters.after reading a new line character.
		void _NewLineAdjust( void )
		{
			_Position.Line++;
			_Position.Column = 0;
		}
		bomhdl::byte_order_marker__ _GetBOM( void )
		{
# if 0
			fdr::byte__ BOMBuffer[BOM_SIZE_MAX];
			fdr::size__ Size = _F().View( sizeof( BOMBuffer ), BOMBuffer );
			bomhdl::byte_order_marker__ BOM = bomhdl::DetectBOM( BOMBuffer, Size );	// Si != 'bomhdl::bom_UnknownOrNone', 'Size' contient au retour la taille du 'BOM'.

			if ( BOM != bomhdl::bom_UnknownOrNone )
				_F().Skip( Size );

			return BOM;
# else
			fdr::size__ Size = 0;
			bomhdl::byte_order_marker__ BOM = bomhdl::DetectBOM( _Feeder, Size );	// Si != 'bomhdl::bom_UnknownOrNone', 'Size' contient au retour la taille du 'BOM'.

			if ( BOM != bomhdl::bom_UnknownOrNone )
				_F().Skip( Size );

			return BOM;
# endif
		}
		utf::format__ _HandleFormat(
			utf::format__ ExpectedFormat,
			bomhdl::byte_order_marker__ BOM )
		{
			switch ( ExpectedFormat ) {
			case utf::f_Guess:
				if ( BOM == bomhdl::bomUTF_8 )
					ExpectedFormat = utf::fUTF_8;
				else if ( BOM != bomhdl::bom_UnknownOrNone ) {
					_Error = eUnexpectedEncoding;
					ExpectedFormat = utf::f_Undefined;
				}
				break;
			case utf::fANSI:
				if ( BOM != bomhdl::bom_UnknownOrNone ) {
					_Error = eUnexpectedEncoding;
					ExpectedFormat = utf::f_Undefined;
				}
				break;
			case utf::fUTF_8:
				if ( ( BOM != bomhdl::bom_UnknownOrNone ) && ( BOM != bomhdl::bomUTF_8 ) ) {
					_Error = eUnexpectedEncoding;
					ExpectedFormat = utf::f_Undefined;
				}
				break;
			case utf::fUTF_16_BE:
				if ( BOM != bomhdl::bomUTF_16_BE ) {
					_Error = eUnexpectedEncoding;
					ExpectedFormat = utf::f_Undefined;
				}
				break;
			case utf::fUTF_16_LE:
				if ( BOM != bomhdl::bomUTF_16_LE ) {
					_Error = eUnexpectedEncoding;
					ExpectedFormat = utf::f_Undefined;
				}
				break;
			case utf::fUTF_32_BE:
				if ( BOM != bomhdl::bomUTF_32_BE ) {
					_Error = eUnexpectedEncoding;
					ExpectedFormat = utf::f_Undefined;
				}
				break;
			case utf::fUTF_32_LE:
				if ( BOM != bomhdl::bomUTF_32_LE ) {
					_Error = eUnexpectedEncoding;
					ExpectedFormat = utf::f_Undefined;
				}
				break;
			default:
				qRFwk();	// Les autres formats ne sont pas accpts et filtrs en amont.
				break;
			}

			return ExpectedFormat;
		}
		void _SetMeaning( lcl::meaning_ &Meaning );
		bso::bool__ _PrefetchUTF( bso::sBool Eat )
		{
			if ( _UTF.Size == 0 ) {
				/*				bso::size__ Size = _F().View( sizeof( _UTF.Data ), _UTF.Data );

								if ( Size == 0  ) {
									_UTF.Size = 0;
									return false;
								}
				*/
				_Feeder.Reset();

				_UTF.Size = _UTFHandler.Handle( _Feeder );

				if ( _UTF.Size != 0 ) {
					if ( _F().View( _UTF.Size, _UTF.Data ) != _UTF.Size )
						qRFwk();
					_UTF.Eaten = false;
				} else
					return false;
			}

			if ( Eat && !_UTF.Eaten ) {
				_F().Skip( _UTF.Size );
				_UTF.Eaten = true;
			}

			return true;
		}
		bso::bool__ _GetCell(
			str::string_ *Cell,
			flw::byte__ Separator );
		bso::bool__ _GetLine( str::string_ *Line )
		{
			return _GetCell( Line, 0 );
		}
	public:
		void reset( bool P = true )
		{
			NLWasRegular_ = false;
			_Position.reset( P );
			_Position.Line = _Position.Column = 1;
			_Flow = NULL;
			_Error = e_Undefined;
		}
		E_CVDTOR( extended_text_iflow__ );
		extended_text_iflow__(
			flw::iflow__ &IFlow,
			utf::format__ Format,
			pos__ Position = pos__( 1, 0 ) )
		{
			reset( false );

			Init( IFlow, Format, Position );
		}
		//f Initialization with 'Flow'..
		bomhdl::byte_order_marker__ Init(
			flw::iflow__ &IFlow,
			utf::format__ Format,
			pos__ Position = pos__( 1, 0 ) )
		{
			NLWasRegular_ = false;
			_Position.Init( Position );
			_Flow = NULL;
			_Flow = &IFlow;
			_Error = e_NoError;

			_Feeder.Init( IFlow );
			_UTF.Init();

			bomhdl::byte_order_marker__ BOM = _GetBOM();

			if ( ( Format = _HandleFormat( Format, BOM ) ) != utf::f_Undefined )
				if ( !_UTFHandler.Init( Format ) )
					_Error = eUnsupportedEncoding;

			return BOM;
		}
		//f Extract and return next character in flow.
		flw::byte__ Get(
			utf__ *UTF = NULL,
			bso::sBool KeepDualNL = true )
		{
			if ( !_PrefetchUTF( true ) )
				qRFwk();

			if ( UTF != NULL )
				*UTF = _UTF;

			_UTF.Init();

			flw::byte__ C = _UTF.Data[0];

			if (C == '\n') {
				if ( NLWasRegular_ ) {
					_NewLineAdjust();
					NLWasRegular_ = false;
				} else if ( !_F().EndOfFlow() && (_F().View() == '\r') ) {
					if ( KeepDualNL ) {
						NLWasRegular_ = true;
						_NewCharAdjust();
					} else {
						_F().Skip();
						_NewLineAdjust();
					}
				} else
					_NewLineAdjust();
			} else if (C == '\r') {
				if ( NLWasRegular_ ) {
					_NewLineAdjust();
					NLWasRegular_ = false;
				} else if ( !_F().EndOfFlow() && (_F().View() == '\n') ) {
					if ( KeepDualNL ) {
						NLWasRegular_ = true;
						_NewCharAdjust();
					} else {
						_F().Skip();
						_NewLineAdjust();
					}
				} else
					_NewLineAdjust();
			} else
				_NewCharAdjust();

			return C;
		}
		flw::byte__ Get(
			utf__ &UTF,
			bso::sBool KeepDualNL = true )
		{
			return Get( &UTF, KeepDualNL );
		}
		//f NOTA : if '.Line' == 0; a '\n' or a '\r' was unget()'.
		const pos__ &Position( void ) const
		{
			return _Position;
		}
		/*f Put the rest of the current cell in 'Cell'. Return true if the cell is delimited by 'Separator',
		false otherwise (cell delimited by a new line, for example). */
		bso::bool__ GetCell(
			str::string_ &Cell,
			flw::byte__ Separator = XTF_DEFAULT_CELL_SEPARATOR )
		{
			return _GetCell( &Cell, Separator );
		}
		//f Put rest of the current line in 'Line'. Return true if line ended by a new line, false when EOF.
		bso::bool__ GetLine( str::string_ &Line )
		{
			return _GetLine( &Line );
		}
		//f Skip the current line.
		void SkipLine( void )
		{
			_GetLine( NULL );
		}
		//f Return the next character in the flow, but let it in the flow.
		flw::byte__ View( utf__ *UTF = NULL )
		{
			if ( !_PrefetchUTF( false ) )
				qRFwk();

			flw::byte__ C = _UTF.Data[0];

			if ( UTF != NULL )
				*UTF = _UTF;

			return C;
		}
		flw::byte__ View( utf__ &UTF )
		{
			return View( &UTF );
		}
		bso::bool__ EndOfFlow( error__ *Error = NULL )	// If returning 'true' and there is an error and 'Error' != NULL,
		{
			// 'Error' contains then the error.s
			if ( _Error == e_NoError ) {
				if ( _UTF.Size != 0 )
					return false;

				if ( _F().EndOfFlow() )
					return true;

				if ( !_PrefetchUTF( false ) ) {
					_Error = eEncodingDiscrepancy;

					if ( Error != NULL )
						*Error = _Error;

					return true;
				}

				return false;
			} else {
				if ( Error != NULL )
					*Error = _Error;

				return true;
			}
		}
		bso::bool__ EndOfFlow( error__ &Error )
		{
			return EndOfFlow( &Error );
		}
		void Dismiss( void )
		{
			_F().Dismiss();
		}
		//f Return the amount of data red.
		flw::size__ AmountRed( void ) const
		{
			return _F().AmountRed();
		}
		void Set( pos__ Position )
		{
			_Position = Position;
		}
		flw::iflow__ &UndelyingFlow( void )
		{
			_UTF.Eaten = true;
			_UTF.Size = 0;

			return _F();
		}
		bso::bool__ SetFormat( utf::format__ Format )
		{
			return _UTFHandler.SetFormat( Format );
		}
		utf::format__ Format( void ) const
		{
			return _UTFHandler.Format();
		}

	};
}

/**************/
/**** NEW *****/
/**************/

namespace xtf {
	typedef extended_text_iflow__ sRFlow;
	typedef pos__ sPos;
	typedef utf__ sUTF;
}


#endif
