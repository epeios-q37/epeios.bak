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

#ifndef AEM_INC_
# define AEM_INC_

# define AEM_NAME		"AEM"

# if defined( E_DEBUG ) && !defined( AEM_NODBG )
#  define AEM_DBG
# endif

// Amount/Extent Manager

# include "err.h"
# include "flw.h"
# include "sdr.h"
# include "cpe.h"

namespace aem {
	enum mode__ {
		mFitted,	// Allocation de la taille exacte.
		mStepped,	// Allocation relle ('extent') au palier suprieur le plus proche.
		mOnlyGrowing,	// Allocation au palier suprieur le plus proche, seulement si la taille augmente.
		m_amount,
		m_Default = mOnlyGrowing,
		m_Undefined
	};

# define AEM_DEFAULT_STEP	64

	using sdr::size__;

	E_TMIMIC__( sdr::size__, shadow__ );	// Type permettant de grer l''extent' ainsi que les pas d'allocation.

	inline bso::bool__ _IsFitted( shadow__ Shadow )
	{
		return Shadow == 0;
	}

	inline bso::bool__ _IsFixed( shadow__ Shadow )
	{
		return ( *Shadow & (size__)1 ) == 1;
	}

	inline bso::bool__ _IsStepped( shadow__ Shadow )
	{
		return !_IsFitted( Shadow ) && !_IsFixed( Shadow );
	}

	inline bso::bool__ _IsUsable( shadow__ Shadow )
	{
		return
			_IsFitted( Shadow )
			|| _IsFixed( Shadow )
			|| ( _IsStepped( Shadow ) && ( ( *Shadow & 2 ) == 0 ) );
	}

	// La premire allocation ne se fera que sur la moiti du palier (Ex. : pour un paliier de 4, le premire allocation se fera sur 2, puis 6, puis 10...).
	inline shadow__ _GetStepCorrespondingShadowForUnusableState( size__ Step )
	{
		bso::uint__ Counter = 0;

		if ( Step <=1 )
			qRFwk();

		if ( Step >= ( SDR_SIZE_MAX - 1 ) )
			qRFwk();

		Step += 1;

		while ( Step >>= 1 ) 
			Counter++;

		Step = ~0;

		while ( Counter-- )
			Step <<= 1;

		Step = ~Step;

		return Step & ~((size__)1);
	}

	inline shadow__ _ConvertSteppedShadowFromUnusableToUsableState( shadow__ Shadow )
	{
		if ( !_IsStepped( Shadow ) )
			qRFwk();

		return *Shadow + 2;
	}

	inline shadow__ _GetStepCorrespondingShadowForUsableState( size__ Step )
	{
		return _ConvertSteppedShadowFromUnusableToUsableState( _GetStepCorrespondingShadowForUnusableState( Step ) );
	}

	inline bso::uint__ _GetMostRightNonZeroBytePosition( size__ Value )
	{
		bso::uint__ Position = 0;

		while ( ( Position < sizeof( Value ) ) && ( ( Value & 0xff ) == 0 ) ) {
			Position++;
			Value >>= 8;
		}

		if ( Position == sizeof( Value ) )
			Position = 0;
		else
			Position++;

		return Position;
	}

	inline bso::size__ _GetMostRightOnePosition( bso::u8__ Value )
	{
		bso::size__ Position = 0;

		while ( ( Position < ( 8 * sizeof( Value ) ) ) && ( ( Value & ( 1 << Position ) ) == 0 ) )
			Position++;

		if ( Position == ( 8 * sizeof( Value ) ) )
			Position = 0;
		else
			Position++;

		return Position;
	}

	inline bso::uint__ _GetMostRightOnePosition( size__ Value )
	{
		bso::size__ BytePosition = _GetMostRightNonZeroBytePosition( Value );

		if ( BytePosition == 0 )
			return 0;

		BytePosition--;

		return ( BytePosition * 8 ) + _GetMostRightOnePosition( ((bso::byte__ *)&Value)[BytePosition] );
	}

	inline size__ _GetMask( bso::size__ OnesAmount )	// Retourne un masque dont les 'OnesAmount' bits les plus  droite sont  1.
	{
		return ~( (~((size__)0) << OnesAmount ) );
	}

	// Si 'Shadow'  == '00...0100', retourne '00...000011'.
	inline size__ _GetStepMask( shadow__ Shadow )
	{
		if ( !_IsStepped( Shadow ) )
			qRFwk();

		if ( !_IsUsable( Shadow ) )
			qRFwk();

		size__ Position = _GetMostRightOnePosition( *Shadow );

		if ( Position == 0 )
			qRFwk();

		return _GetMask( Position - 1 );
	}

	// Retourne 'true' si l''extent' correspondant  'Size' peut tre encod, en fonction de 'StepMask',  en mode 'stepped'. Retourne faux sinon, auquel cas on bascule en mode 'fitted'.
	inline bso::bool__ _IsInStepLimit(
		size__ Amount,
		size__ StepMask )
	{
		StepMask = ~StepMask << 1;

		return ( Amount & StepMask ) != StepMask;
	}

	inline size__ _GetExtent(
		size__ Amount,
		shadow__ Shadow )
	{
		if ( !_IsUsable( Shadow ) )
			return 0;
		else
			return ( _IsFitted( Shadow ) ? Amount : *Shadow );
	}
	
	inline void _MarkAsFitted( shadow__ &Shadow )
	{
		Shadow = 0;
	}

	class vAmountExtentManagerCore
	{
	private:
		shadow__ _GetShadow( void ) const
		{
			return S_.Shadow;
		}
		size__ _GetStepMask( void ) const
		{
			return aem::_GetStepMask( _GetShadow() );
		}
		bso::bool__ _IsFitted( void ) const
		{
			return aem::_IsFitted( _GetShadow() );
		}
		void _MarkAsFitted( void )
		{
			aem::_MarkAsFitted( S_.Shadow );
		}
		bso::bool__ _IsFixed( void ) const
		{
			return aem::_IsFixed( _GetShadow() );
		}
		bso::bool__ _IsStepped( void ) const
		{
			return aem::_IsStepped( _GetShadow() );
		}
		size__ _GetAmount( void ) const
		{
			return S_.Amount;
		}
		bso::bool__ _IsUsable( void ) const
		{
			return aem::_IsUsable( _GetShadow() );
		}
		size__ _GetExtent( void ) const
		{
			return aem::_GetExtent( _GetAmount(), _GetShadow() );
		}
		bso::bool__ _IsBigEnough( sdr::size__ Amount ) const
		{
# ifdef AEM_DBG
			if ( !_IsUsable() )
				qRFwk();
# endif
			// En n'utilisant pas '_GetExtent(...)', on gagne un peu de temps.
			return ( *_GetShadow() >= Amount )
				   || ( _IsFitted() && ( _GetAmount() >= Amount ) );
		}
		bso::bool__ _FittedHandle( size__ Amount )
		{
			if ( _IsFitted() ) {
				if ( _GetAmount() == Amount )
					return false;
				else {
					S_.Amount = Amount;
					return true;
				}
			} else {
				S_.Amount = Amount;
				
				if ( !_IsUsable() || ( _GetExtent() != Amount ) ) {
					_MarkAsFitted();
					return true;
				} else {
					_MarkAsFitted();
					return false;
				}
			}
		}
		bso::bool__ _SteppedHandle( size__ &Amount )
		{
			bso::bool__ Force = false;

			if ( _IsFitted() || _IsFixed() )
				qRFwk();

			if ( !_IsUsable() )	{	// Nota : danse ce cas, 'S_.Amount' == 0;
				if ( Amount == 0 ) {
					return false;
				} else {
					S_.Shadow = _ConvertSteppedShadowFromUnusableToUsableState( _GetShadow() );
					Force = true;
				}
			}

			size__ StepMask = _GetStepMask();

			S_.Amount = Amount;

			if ( _IsInStepLimit( Amount, StepMask ) ) {
				shadow__ NewShadow = ( ( Amount + StepMask ) & ~StepMask ) | ( StepMask + 1 );

				if ( ( NewShadow != _GetShadow() ) || Force ) {
					Amount = *(S_.Shadow = NewShadow);
					return true;
				} else
					return false;
			} else
				return _FittedHandle( Amount );
		}
	protected:
		bso::bool__ SetFixed( size__ &Amount )
		{
			if ( _GetAmount() > Amount )
				qRFwk();

			S_.Shadow = Amount |= 1;

			return true;
		}
		bso::bool__ SetStepped( size__ &Step )
		{
			if ( Step == 0 ) {
				_MarkAsFitted();
				return false;
			} else if ( _IsUsable() ) {
				shadow__ Shadow = _GetStepCorrespondingShadowForUsableState( Step );

				if ( *Shadow < _GetExtent() ) {
					Step = *(S_.Shadow = Shadow);
					return false;
				} else {
					S_.Shadow = Shadow;
					return true;
				}
			} else {
				S_.Shadow = _GetStepCorrespondingShadowForUnusableState( Step );
				return false;
			}
		}
		bso::bool__ Handle(
			size__ &Amount,	// Si valeur retourne != 'false', alors allouer 'Amount' (qui peut avoir t modifie).
			mode__ Mode = m_Default )	// Si configur en 'fitted' ou en 'fixed', cette valeur est ignore.
		{
			if ( _IsFixed() ) {
				if ( Amount > _GetExtent() )
					qRFwk();

				S_.Amount = Amount;

				return false;
			} else if ( _IsFitted() )
				return _FittedHandle( Amount );
			else {
				switch ( Mode ) {
				case mFitted:
					return _FittedHandle( Amount );
					break;
				case mStepped:
					return _SteppedHandle( Amount );
					break;
				case mOnlyGrowing:
					if ( !_IsUsable() || !_IsBigEnough( Amount ) )
						return _SteppedHandle( Amount );
					else {
						S_.Amount = Amount;
						return false;
					}
					break;
				default:
					qRFwk();
					break;
				}
			}

			return false;	// Pour viter un 'warning'.
		}
/*		void AwareHandle(
			size__ Amount,
			mode__ Mode = m_Default )
		{
			if ( Handle( Amount, Mode ) )
				qRFwk();
		}
*/		void Init( void )
		{
			S_.Amount = 0;
		}
	public:
		struct s {
			size__ Amount;
			shadow__ Shadow;	// Contient de quoi dduire l''extent', ainsi que la gestion des pas d'allocation.
		} &S_;
		vAmountExtentManagerCore( s &S )
		: S_( S )
		{}
		void reset( bso::bool__ = true )
		{
			S_.Amount = 0;
			S_.Shadow = _GetStepCorrespondingShadowForUnusableState( AEM_DEFAULT_STEP );
		}
		vAmountExtentManagerCore &operator =( const vAmountExtentManagerCore &AEMC )
		{
			// 'S_.Amount' et 'S_.Extent' sont normallement traits en amont.

			return *this;
		}
		size__ Amount( void ) const
		{
			return _GetAmount();
		}
		size__ Extent( void ) const
		{
			return _GetExtent();
		}
		bso::bool__ IsEmpty( void ) const
		{
			return _GetAmount() ==  0;
		}
	};

	template <typename row> class _amount_extent_manager_
	: public vAmountExtentManagerCore
	{
	public:
		struct s
		: public vAmountExtentManagerCore::s {};
		void reset( bso::bool__ P = true )
		{
			vAmountExtentManagerCore::reset( P );
		}
		_amount_extent_manager_ &operator =( const _amount_extent_manager_ &AEM )
		{
			vAmountExtentManagerCore::operator =( AEM );

			return *this;
		}
		_amount_extent_manager_( s &S )
		: vAmountExtentManagerCore( S )
		{}
row First( void ) const
		{
			if ( S_.Amount )
				return 0;
			else
				return qNIL;
		}
		row First( size__ Offset ) const
		{
			row Row = First();

			if ( Row != qNIL )
				Row =  Next( Row, Offset  );

			return Row;
		}
		row Last( void ) const
		{
			if ( S_.Amount )
				return S_.Amount - 1;
			else
				return qNIL;
		}
		row Last( sdr::size__ Offset ) const
		{
			row Row = Last();

			if ( Row != qNIL )
				Row = Previous( Row, Offset  );

			return Row;
		}
		row Next(
			row Current,
			size__ Offset ) const
		{
			if ( ( *Current += Offset ) < S_.Amount )
				return Current;
			else
				return qNIL;
		}
		row Next( row Current ) const
		{
			return Next( Current, 1 );
		}
		row Previous(
			row Current,
			size__ Offset ) const
		{
			if ( *Current >= Offset )
				return *Current - Offset;
			else
				return qNIL;
		}
		row Previous( row Current ) const
		{
			return Previous( Current, 1 );
		}
		bso::bool__ Exists( row Row ) const
		{
			return *Row < S_.Amount;
		}
	};

	template <typename row> class amount_extent_manager_
	: public _amount_extent_manager_<row>
	{
	public:
		struct s
		: _amount_extent_manager_<row>::s
		{};
		amount_extent_manager_( s &S )
		: _amount_extent_manager_<row>( S )
		{}
	};


	template <int extent, typename row> class _amount_extent_manager__
	{
	private:
		size__ _Amount;
	protected:
		bso::bool__ Handle(
			sdr::size__ &Size,
			mode__ Mode)
		{
			_Amount = Size;

			return false;
		}
	public:
		struct s {};	// Simplifie son utilisation dans 'BCH'.
		_amount_extent_manager__( s &S = *(s *)NULL )
		{
			reset( false );
		}
		// Simplifies the 'SET' library.
		void reset( bso::bool__ = true )
		{
			_Amount = 0;
		}
		_amount_extent_manager__ &operator =( const _amount_extent_manager__ &AEM )
		{
			// 'Amount' normallement dj trait en amont.

			return *this;
		}
		void Init( void )
		{
			_Amount = 0;
		}
		sdr::size__ Extent( void ) const
		{
			return extent;
		}
		//f Return the amount.
		sdr::size__ Amount( void ) const
		{
			return _Amount;
		}
		//f Return position of the last object of the set.
		row Last( void ) const
		{
			if ( _Amount )
				return _Amount - 1;
			else
				return qNIL;
		}
		row Last( sdr::size__ Offset ) const
		{
			row Row = Last();

			if ( Row != qNIL )
				Row =  Previous( Row, Offset  );

			return Row;
		}
		//f Return position of the first object of the set.
		row First( void ) const
		{
			if ( _Amount )
				return 0;
			else
				return qNIL;
		}
		row First( sdr::size__ Offset ) const
		{
			row Row = First();

			if ( Row != qNIL )
				Row = Next( Row, Offset  );

			return Row;
		}
		//f Return the position of 'Offset' next to 'Current'.
		row Next(
			row Current,
			sdr::size__ Offset ) const
		{
			if ( ( *Current += Offset ) < _Amount )
				return Current;
			else
				return qNIL;
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
				return qNIL;
		}
		//f Return the position of the object before 'Current' (to the bottom).
		row Previous( row Current ) const
		{
			return Previous( Current, 1 );
		}
		//f Return true if empty, false otherwise.
		bso::bool__ IsEmpty( void ) const
		{
			return _Amount == 0;
		}
		//f Return true if 'Row' exists, false otherwise.
		bso::bool__ Exists( row Row ) const
		{
			return *Row < _Amount;
		}
};

	template <int extent, typename row> class amount_extent_manager__
	: public _amount_extent_manager__<extent, row>
	{
	public:
		struct s
		: public _amount_extent_manager__<extent, row>::s
		{};
		amount_extent_manager__( s &S = *(s *)NULL )
		: _amount_extent_manager__<extent, row>( S )
		{}
	};

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
}

#endif
