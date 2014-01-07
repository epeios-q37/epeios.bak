/*
	'aem.h' by Claude SIMON (http://zeusw.org/).

	'aem' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AEM__INC
# define AEM__INC

# define AEM_NAME		"AEM"

# if defined( E_DEBUG ) && !defined( AEM_NODBG )
#  define AEM_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// Amount/Extent Manager

# include "err.h"
# include "flw.h"
# include "sdr.h"
# include "cpe.h"

namespace aem {
	enum mode__ {
		mFitted,	// Allocation de la taille exacte.
		mStepped,	// Allocation réelle ('extent') au palier supérieur le plus proche.
		mOnlyGrowing,	// Allocation au palier supérieur le plus proche, seulement si la taille augmente.
		m_amount,
		m_Default = mOnlyGrowing,
		m_Undefined
	};

# define AEM_DEFAULT_STEP	64

	using sdr::size__;

	inline bso::bool__ _IsFitted( size__ RawExtent )
	{
		return RawExtent == 0;
	}

	inline bso::bool__ _IsFixed( size__ RawExtent )
	{
		return ( RawExtent & (size__)1 ) == 1;
	}

	inline bso::bool__ _IsStepped( size__ RawExtent )
	{
		return !_IsFitted( RawExtent) && !_IsFixed( RawExtent );
	}

	inline bso::bool__ _IsUsable( size__ RawExtent )
	{
		return
			_IsFitted( RawExtent )
			|| _IsFixed( RawExtent )
			|| ( _IsStepped( RawExtent ) && ( ( RawExtent & 2 ) == 0 ) );
	}

	// La première allocation ne se fera que sur la moitié du palier (Ex. : pour un paliier de 4, le première allocation se fera sur 2, puis 6, puis 10...).
	inline size__ _GetStepCorrespondingExtentForUnusableState( size__ Step )
	{
		bso::uint__ Counter = 0;

		if ( Step <=1 )
			ERRFwk();

		if ( Step >= ( SDR_SIZE_MAX - 1 ) )
			ERRFwk();

		Step += 1;

		while ( Step >>= 1 ) 
			Counter++;

		Step = ~0;

		while ( Counter-- )
			Step <<= 1;

		Step = ~Step;

		return Step & ~((size__)1);
	}

	inline size__ _ConvertSteppedExtentFromUnusableToUsableState( size__ RawExtent )
	{
		if ( !_IsStepped( RawExtent ) )
			ERRFwk();

		return RawExtent + 2;
	}

	inline size__ _GetStepCorrespondingExtentForUsableState( size__ Step )
	{
		return _ConvertSteppedExtentFromUnusableToUsableState( _GetStepCorrespondingExtentForUnusableState( Step ) );
	}

	inline bso::uint__ _GetMostRightNonZeroBytePosition( size__ Value )
	{
		bso::uint__ Position = 0;

		while ( ( Position < sizeof( Value ) ) && ( ((bso::raw__ *)&Value)[Position] != 0 ) )
			Position++;

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

	inline bso::uint__ _GetMostRigthOnePosition( size__ Value )
	{
		bso::size__ BytePosition = _GetMostRightNonZeroBytePosition( Value );

		if ( BytePosition == 0 )
			return 0;

		BytePosition--;

		return ( BytePosition * 8 ) + _GetMostRightOnePosition( ((bso::raw__ *)&Value)[BytePosition] );
	}

	inline size__ _GetMask( bso::size__ OnesAmount )	// Retourne un masque dont les 'OnesAmount' bits les plus à droite sont à 1.
	{
		return ~( (~((size__)0) << OnesAmount ) );
	}

	// Si 'Extent'  == '00...0100', retourne '00...000011'.
	inline size__ _GetStepMask( size__ Extent )
	{
		if ( !_IsStepped( Extent ) )
			ERRFwk();

		if ( !_IsUsable( Extent ) )
			ERRFwk();

		size__ Position = _GetMostRightOnePosition( Extent );

		if ( Position == 0 )
			ERRFwk();

		return _GetMask( Position - 1 );
	}

	// Retourne 'true' si l''extent' correspondant à 'Size' peut être encodé, en fonction de 'StepMask',  en mode 'stepped'. Retourne faux sinon, auquel cas on bascule en mode 'fitted'.
	inline bso::bool__ _IsInStepLimit(
		size__ Amount,
		size__ StepMask )
	{
		StepMask = ~StepMask << 1;

		return ( Amount & StepMask ) != StepMask;
	}

	inline size__ _GetExtent(
		size__ Amount,
		size__ RawExtent )
	{
# ifdef AEM_DBG
		if ( !_IsUsable( RawExtent ) )
			ERRFwk();
# endif
		return ( _IsFitted( RawExtent ) ? Amount : RawExtent );
	}
	
	inline void _MarkAsFitted( size__ &RawExtent )
	{
		RawExtent = 0;
	}

	template <typename row> class _amount_extent_manager_
	{
	private:
		size__ _GetRawExtent( void ) const
		{
			return S_.Extent;
		}
		bso::bool__ _IsFitted( void ) const
		{
			return aem::_IsFitted( _GetRawExtent() );
		}
		void _MarkAsFitted( void )
		{
			aem::_MarkAsFitted( S_.Extent );
		}
		bso::bool__ _IsFixed( void ) const
		{
			return aem::_IsFixed( _GetRawExtent() );
		}
		bso::bool__ _IsStepped( void ) const
		{
			return aem::_IsStepped( _GetRawExtent() );
		}
		size__ _GetStepMask( void ) const
		{
			return aem::_GetStepMask( _GetRawExtent() );
		}
		size__ _GetAmount( void ) const
		{
			return S_.Amount;
		}
		bso::bool__ _IsUsable( void ) const
		{
			return aem::_IsUsable( _GetRawExtent() );
		}
		size__ _GetExtent( void ) const
		{
			return aem::_GetExtent( _GetAmount(), _GetRawExtent() );
		}
		bso::bool__ _IsBigEnough( sdr::size__ Amount ) const
		{
# ifdef AEM_DBG
			if ( !_IsUsable() )
				ERRFwk();
# endif
			// En n'utilisant pas '_GetExtent(...)', on gagne un peu de temps.
			return ( _GetRawExtent() >= Amount )
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
			if ( _IsFitted() || _IsFixed() )
				ERRFwk();

			size__ StepMask = _GetStepMask();

			S_.Amount = Amount;

			if ( _IsInStepLimit( Amount, StepMask ) ) {
				size__ NewExtent = ( ( Amount + StepMask ) & ~StepMask ) | ( StepMask + 1 );

				if ( NewExtent != _GetExtent() ) {
					Amount = S_.Extent = NewExtent;
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
				ERRFwk();

			S_.Extent = Amount |= 1;

			return true;
		}
		bso::bool__ SetStepped( size__ &Step )
		{
			if ( Step == 0 ) {
				_MarkAsFitted();
				return false;
			} else if ( _IsUsable() ) {
				size__ Extent = _GetStepCorrespondingExtentForUsableState( Step );

				if ( Extent < _GetExtent() ) {
					Step = S_.Extent = Extent;
					return false;
				} else {
					S_.Extent = Extent;
					return true;
				}
			} else {
				S_.Extent = _GetStepCorrespondingExtentForUnusableState( Step );
				return false;
			}
		}
		bso::bool__ Handle(
			size__ &Amount,	// Si valeur retournée != 'false', alors allouer 'Amount' (qui peut avoir été modifiée).
			mode__ Mode = m_Default )	// Si configuré en 'fitted' ou en 'fixed', cette valeur est ignorée.
		{
			if ( _IsFixed() ) {
				if ( Amount > _GetExtent() )
					ERRFwk();

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
					if ( !_IsBigEnough( Amount ) )
						return _SteppedHandle( Amount );
					else {
						S_.Amount = Amount;
						return false;
					}
					break;
				default:
					ERRFwk();
					break;
				}
			}

			return false;	// Pour éviter un 'warning'.
		}
		void AwareHandle(
			size__ Amount,
			mode__ Mode = m_Default )
		{
			if ( Handle( Amount, Mode ) )
				ERRFwk();
		}
		bso::bool__ Init( size__ &Size )	// Si valeur retournée == 'true', alors allouer 'Size'.
		{
			S_.Amount= 0;

			if ( !_IsUsable() ) {
				Size = S_.Extent = _ConvertSteppedExtentFromUnusableToUsableState( _GetRawExtent() );
				return true;
			} else {
				Size = _GetExtent();
				return false;
			}
		}
	public:
		struct s {
			size__
				Amount,
				Extent;
		} &S_;
		_amount_extent_manager_( s &S )
		: S_( S )
		{}
		void reset( bso::bool__ = true )
		{
			S_.Amount = 0;
			S_.Extent = _GetStepCorrespondingExtentForUnusableState( AEM_DEFAULT_STEP );
		}
		_amount_extent_manager_ &operator =( const _amount_extent_manager_ &AEM )
		{
			// 'S_.Amount' et 'S_.Extent' sont normallement traités en amont.

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
		row First( void ) const
		{
			if ( S_.Amount )
				return 0;
			else
				return E_NIL;
		}
		row First( size__ Offset ) const
		{
			row Row = First();

			if ( Row != E_NIL )
				Row =  Next( Row, Offset  );

			return Row;
		}
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
		row Next(
			row Current,
			size__ Offset ) const
		{
			if ( ( *Current += Offset ) < S_.Amount )
				return Current;
			else
				return E_NIL;
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
				return E_NIL;
		}
		row Previous( row Current ) const
		{
			return Previous( Current, 1 );
		}
		bso::bool__ Exists( row Row ) const
		{
			return *Row < S_.Amount;
		}
		bso::bool__ IsEmpty( void ) const
		{
			return _GetAmount() ==  0;
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
	public:
		struct s {};	// Simplifie son utilisation dans 'BCH'.
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

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
