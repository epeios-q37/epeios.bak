/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

//D MaTH InTeGer 

#ifndef MTHITG_INC_
# define MTHITG_INC_

# define MTHITG_NAME		"MTHITG"

# if defined( E_DEBUG ) && !defined( MTHITG_NODBG )
#  define MTHITG_DBG
# endif

# include "ias.h"
# include "err.h"
# include "flw.h"
# include "bso.h"
# include "bch.h"

# define MTHITG_MAX_SIZE	((base__)( 0xffffU >> 1 ))

# define MTHITG_SIGN_POS	((base__)15U)
# define MTHITG_SIGN_MASK	((base__)( 1U << MTHITG_SIGN_POS ))

/*************/
/**** OLD ****/
/*************/


namespace mthitg {
	typedef bso::u16__	base__;
	typedef bso::u16__	size__;
	
	class integer;
	class integer_;

	// Predclaration.

	int Comp_(
		const integer_ &Op1,
		const integer_ &Op2 );
	integer Divide_(
		const integer_ &Num,
		const integer_ &Den,
		integer_ &Rest );
	integer Div_(
		const integer_ &Num,
		const integer_ &Den,
		integer_ &Rest );
	integer Add_(
		const integer_ &Op1,
		const integer_ &Op2 );
	integer Sub_(
		const integer_ &Op1,
		const integer_ &Op2 );
	integer Mul_(
		const integer_ &Op1,
		const integer_ &Op2 );

	using bch::sHook;
	
	//c An integer.
	class integer_ {
	protected:
		size__ _GetRawSize( void ) const
		{
			return S_.Size & ~MTHITG_SIGN_MASK;
		}
		size__ _GetRealSize( void ) const
		{
			size__ Size = _GetRawSize();

			if ( Size == 0 )
				qRFwk();

			while ( ( Size-- ) && !Core( Size ) );

			return Size + 1;
		}
		bso::bool__ _GetSignFlag( void ) const
		{
			return ( ( S_.Size & MTHITG_SIGN_MASK ) >> MTHITG_SIGN_POS ) != 0;
		}
		size__ PutSize_( size__ Size )
		{
			S_.Size = ( ( S_.Size & MTHITG_SIGN_MASK ) | Size );

			return _GetRawSize();
		}
		size__ PutSignFlag_( bso::bool__ Sign )
		{
			S_.Size = ( ( S_.Size & ~MTHITG_SIGN_MASK ) | ( Sign << MTHITG_SIGN_POS ) );
			return _GetSignFlag();
		}
		void Adjust_( aem::mode__ Mode = aem::m_Default );
		void Dup_( const integer_ &Interger );
		friend int Comp_(
			const integer_ &Num,
			const integer_ &Den );
		friend integer Divide_(
			const integer_ &Num,
			const integer_ &Den,
			integer_ &Rest );
		friend integer Div_(
			const integer_ &Num,
			const integer_ &Den,
			integer_ &Rest );
		friend integer Add_(
			const integer_ &Op1,
			const integer_ &Op2 );
		friend integer Sub_(
			const integer_ &Op1,
			const integer_ &Op2 );
		friend integer Mul_(
			const integer_ &Op1,
			const integer_ &Op2 );
		bso::u32__ _GetAbsULong( void )	// Valeur absolue en 'ulong'.
		{
			Adjust_();

			if ( _GetRawSize() == 0 )
				return 0;
			else if ( _GetRawSize() == 1 )
				return Core( Core.First() );
			else if ( _GetRawSize() == 2 )
				return Core( Core.First() ) | ( Core( Core.First( 1 ) ) << 16UL );
			else
				qRFwk();

			return 0;	// To avoid a 'warning'.
		}
	public:
		//o The core.
		bch::E_BUNCH_( base__ ) Core;
		struct s {
			/* The most significant is the sign (0:+, 1:-) (this are not smileys).
			Other bits are the size, in word, of the integer. If this bits are all
			at 0, then the integer is invalid. */
			bch::E_BUNCH_( base__ )::s Core;
			size__ Size;
		} &S_;
		integer_( s &S )
		: S_( S ),
		  Core( S.Core )
		 {}
		void reset( bso::bool__ P = true )
		{
			Core.reset( P );
			S_.Size = 0;
		}
		void plug( sHook &Hook )
		{
			Core.plug( Hook );
		}
		void plug( qASd *AS )
		{
			Core.plug( AS );
		}
		integer_ &operator =( const integer_ &I )
		{
			S_.Size = I.S_.Size;
			Core = I.Core;
			
			return *this;
		}
		void Init( flw::sIFlow &Flow );
		void Init( const str::dString &Integer );
		void Init( const integer_ &Integer )
		{
			reset();

			Core.Init();

			this->operator =( Integer );
		}
		//f Initialization.
		void Init( bso::s32__ Seed = 0 )
		{
			S_.Size = 0;
			Core.Init();

			if ( Seed < 0 )
			{
				Seed = -Seed;
				PutSignFlag_( true );
			}
			else
				PutSignFlag_( false );

			if ( Seed < 0x10000UL )
			{
				Core.Allocate( 1 );

				Core.Store( (base__)Seed, 0 );
				PutSize_( 1 );
			}
			else
			{
				Core.Allocate( 2 );

				Core.Store( (base__)( Seed % 0x10000UL ), 0 );
				Core.Store( (base__)( Seed >> 16 ), 1 );
				PutSize_( 2 );
			}
		}
		void Init( bso::u32__ Seed )
		{
			S_.Size = 0;
			Core.Init();

			PutSignFlag_( false );

			if ( Seed < 0x10000UL )
			{
				Core.Allocate( 1 );

				Core.Store( (base__)Seed, 0 );
				PutSize_( 1 );
			}
			else
			{
				Core.Allocate( 2 );

				Core.Store( (base__)( Seed % 0x10000UL ), 0 );
				Core.Store( (base__)( Seed >> 16 ), 1 );
				PutSize_( 2 );
			}
		}
		integer Add( const integer_ &Op ) const;
		integer Sub( const integer_ &Op ) const;
		integer Mul( const integer_ &Op ) const;
		integer Div( const integer_ &Op ) const;
		integer Mod( const integer_ &Op ) const;
		integer Abs( void ) const;
		integer operator -( void ) const;
		int operator !( void ) const
		{
			return _GetRealSize() == 0;
		}
		//f Return true if valid, fasle otherwise.
		bso::bool__ IsValid( void ) const
		{
			return _GetRawSize() != 0;
		}
		bso::u32__ GetU32( void )
		{
			if ( _GetSignFlag() )
				qRFwk();

			return _GetAbsULong();
		}
		bso::s32__ GetS32()
		{
			bso::u32__ Value = _GetAbsULong();

			if ( Value > BSO_S32_MAX )
				qRFwk();

			if ( _GetSignFlag() )
				return -(bso::s32__)Value;
			else
				return Value;
		}
		bso::lfloat__ GetLongFloat( void ) const;
		bso::sign__ GetSign( void ) const
		{
			if ( _GetRealSize() == 0 )
				return 0;
			else if ( _GetSignFlag() )
				return -1;
			else
				return 1;
		}
	};			
		
	inline int Compare(
		const integer_ &Op1,
		const integer_ &Op2 )
	{
		return 	Comp_( Op1, Op2 );
	}
	
	class integer
	: public integer_
	{
	private:
		s S_;
	public:
		integer( void )
		: integer_( S_ )
		{
			reset( false );
		}
		~integer( void )
		{
			reset( true );
		}
		integer( const integer_ &I )
		: integer_( S_ )
		{
			reset( false );
			
			integer_::Init();
			
			integer_::operator =( I );
		}
		integer( const integer &I )
		: integer_( S_ )
		{
			reset( false );
			
			integer_::Init();
			
			integer_::operator =( I );
		}
		integer( bso::s32__ Seed )
		: integer_( S_ )
		{
			reset( false );

			integer_::Init( Seed );
		}
		integer( bso::u32__ Seed )
		: integer_( S_ )
		{
			reset( false );

			integer_::Init( Seed );
		}
	};		

	inline integer integer_::operator -( void ) const 
	{
		integer R = *this;

		if ( GetSign() )
			R.PutSignFlag_( !R._GetSignFlag() );

		return R;
	}

	inline integer integer_::Abs( void ) const
	{
		integer R = *this;
		R.PutSignFlag_( false );

		return R;
	}

	void PGCD(
		const integer_ &Op1,
		const integer_ &Op2,
		integer_ &Result );

	inline integer PGCD(
		const integer_ &Op1,
		const integer_ &Op2 )
	{
		integer Result;

		Result.Init();

		PGCD( Op1, Op2, Result );

		return Result;
	}

	inline void Add(
		const integer_ &Op1,
		const integer_ &Op2,
		integer_ &Result )
	{
		Result = Op1.Add( Op2 );
	}

	inline void Sub(
		const integer_ &Op1,
		const integer_ &Op2,
		integer_ &Result )
	{
		Result = Op1.Sub( Op2 );
	}

	inline void Mul(
		const integer_ &Op1,
		const integer_ &Op2,
		integer_ &Result )
	{
		Result = Op1.Mul( Op2 );
	}
}

inline mthitg::integer Abs( const mthitg::integer_ &I )
{
	return I.Abs();
}

inline mthitg::integer operator +(
	const mthitg::integer_ &Op1,
	const mthitg::integer_ &Op2 )
{
	return Op1.Add( Op2 );
}

inline mthitg::integer operator -(
	const mthitg::integer_ &Op1,
	const mthitg::integer_ &Op2 )
{
	return Op1.Sub( Op2 );
}

inline mthitg::integer operator *(
	const mthitg::integer_ &Op1,
	const mthitg::integer_ &Op2 )
{
	return Op1.Mul( Op2 );
}

inline mthitg::integer operator /(
	const mthitg::integer_ &Op1,
	const mthitg::integer_ &Op2 )
{
	return Op1.Div( Op2 );
}

inline mthitg::integer operator %(
	const mthitg::integer_ &Op1,
	const mthitg::integer_ &Op2 )
{
	return Op1.Mod( Op2 );
}

inline mthitg::integer operator +=(
	mthitg::integer_ &I1,
	 const mthitg::integer_ &I2 )
{
	return I1  = I1 + I2;
}

inline mthitg::integer operator -=(
	mthitg::integer_ &I1,
	 const mthitg::integer_ &I2 )
{
	return I1  = I1 - I2;
}

inline mthitg::integer operator *=(
	mthitg::integer_ &I1,
	 const mthitg::integer_ &I2 )
{
	return I1  = I1 * I2;
}

inline mthitg::integer operator /=(
	mthitg::integer_ &I1,
	 const mthitg::integer_ &I2 )
{
	return I1  = I1 / I2;
}

inline mthitg::integer operator %=(
	mthitg::integer_ &I1,
	 const mthitg::integer_ &I2 )
{
	return I1  = I1 % I2;
}

inline bso::bool__ operator ==(
	const mthitg::integer_ &Op1,
	const mthitg::integer_ &Op2 )
{
	return mthitg::Comp_( Op1, Op2 ) == 0;
}	
inline bso::bool__ operator !=(
	const mthitg::integer_ &Op1,
	const mthitg::integer_ &Op2 )
{
	return mthitg::Comp_( Op1, Op2 ) != 0;
}	
inline bso::bool__ operator >=(
	const mthitg::integer_ &Op1,
	const mthitg::integer_ &Op2 )
{
	return mthitg::Comp_( Op1, Op2 ) >= 0;
}	
inline bso::bool__ operator >(
	const mthitg::integer_ &Op1,
	const mthitg::integer_ &Op2 )
{
	return mthitg::Comp_( Op1, Op2 ) > 0;
}	
inline bso::bool__ operator <(
	const mthitg::integer_ &Op1,
	const mthitg::integer_ &Op2 )
{
	return mthitg::Comp_( Op1, Op2 ) < 0;
}	
inline bso::bool__ operator <=(
	const mthitg::integer_ &Op1,
	const mthitg::integer_ &Op2 )
{
	return mthitg::Comp_( Op1, Op2 ) <= 0;
}	


txf::text_oflow__ &operator <<(
	txf::text_oflow__ &OFlow,
	const mthitg::integer_ &Integer );

/*************/
/**** NEW ****/
/*************/

namespace mthitg {
	typedef integer_ dInteger;
	typedef integer wInteger;
}


#endif
