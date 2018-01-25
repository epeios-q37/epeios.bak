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

//	$Id: mthfrc.h,v 1.1 2010/07/15 10:58:18 csimon Exp $

#ifndef MTHFRC__INC
#define MTHFRC__INC

#define MTHFRC_NAME		"MTHFRC"

#define	MTHFRC_VERSION	"$Revision: 1.1 $"

#define MTHFRC_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( MTHFRC_NODBG )
#define MTHFRC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.1 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2010/07/15 10:58:18 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D MaTH FRaCtion 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# error "OBSOLETE !!! Use 'MTHRTN' instead."

#include "err.h"
#include "flw.h"
#include "tol.h"
#include "mthitg.h"

namespace mthfrc {
	using namespace mthitg;

	template <typename t, typename t_> class _fraction_
	{
	public:
		struct s {
			typename t_::s D, N;
		};
		t_ D, N;
		_fraction_( s &S )
		: D( S.D ),
		  N( S.N )
		{}
		void reset( bso::bool__ P = true )
		{
			D.reset( P );
			N.reset( P );
		}
		void plug( qAS_ &AS )
		{
			D.plug( AS );
			N.plug( AS );
		}
		_fraction_ &operator =( const _fraction_ &F )
		{
			D = F.D;
			N = F.N;

			return *this;
		}
		void Init(
			const t_ &Numerator,
			const t_ &Denominator = t( 1 ) )
		{
			reset();

			N.Init();
			D.Init();

			N = Numerator;
			D = Denominator;
		}
		void Init(
			bso::s32__ Numerator = 0,
			bso::u32__ Denominator = 1 )
		{
			Init( t( Numerator ), t( Denominator ) );
		}
		void Init( const _fraction_ &F )
		{
			Init();

			operator =( F );
		}
		int operator !( void ) const
		{
			return !N;
		}
		bso::sign__ GetSign( void ) const
		{
			switch ( N.GetSign() ) {
			case -1:
				return -D.GetSign();
				break;
			case 0:
				return 0;
					break;
			case 1:
				return D.GetSign();
				break;
			default:
				qRFwk();
				break;
			}

			return 0;	// To avoid a 'warning'.
		}
		integer GetInteger( void ) const
		{
			return N / D;
		}
		bso::lfloat__ GetLongFloat( void ) const
		{
			return N.GetLongFloat() / D.GetLongFloat();
		}
		const _fraction_ &Simplify( void );	// Dfini plus loin.
	};

	E_AUTO2( _fraction )

	template <typename t, typename t_> inline void Mul(
		const _fraction_<t,t_> &Op1,
		const _fraction_<t,t_> &Op2,
		_fraction_<t,t_> &Result )
	{
	qRH
		t N, D;
	qRB
		N.Init();
		Mul( Op1.N, Op2.N, N );

		D.Init();
		Mul( Op1.D, Op2.D, D );

		Result.Init( N, D );
	qRR
	qRT
	qRE
	}

	template <typename t, typename t_> inline void Div(
		const _fraction_<t,t_> &Op1,
		const _fraction_<t,t_> &Op2,
		_fraction_<t,t_> &Result )
	{
	qRH
		t N, D;
	qRB
		N.Init();
		Mul( Op1.N, Op2.D, N );

		D.Init();
		Mul( Op1.D, Op2.N, D );

		Result.Init( N, D );
	qRR
	qRT
	qRE
	}

	template <typename t, typename t_> inline void Add(
		const _fraction_<t,t_> &Op1,
		const _fraction_<t,t_> &Op2,
		_fraction_<t,t_> &Result )
	{
	qRH
		t N, D, T1, T2;
	qRB
		T1.Init();
		Mul( Op1.N, Op2.D, T1 );

		T2.Init();
		Mul( Op2.N, Op1.D, T2 );
		
		N.Init();
		Add( T1, T2, N );

		D.Init();
		Mul( Op1.D, Op2.D, D );

		Result.Init( N, D );
	qRR
	qRT
	qRE
	}

	template <typename t, typename t_> inline void Sub(
		const _fraction_<t,t_> &Op1,
		const _fraction_<t,t_> &Op2,
		_fraction_<t,t_> &Result )
	{
	qRH
		t N, D, T1, T2;
	qRB
		T1.Init();
		Mul( Op1.N, Op2.D, T1 );

		T2.Init();
		Mul( Op2.N, Op1.D, T2 );
		
		N.Init();
		Sub( T1, T2, N );

		D.Init();
		Mul( Op1.D, Op2.D, D );

		Result.Init( N, D );
	qRR
	qRT
	qRE
	}

	typedef _fraction_<integer, integer_> fraction_;

	 class fraction
	: public fraction_
	{
	private:
		s S_;
	public:
		fraction( void )
		: fraction_( S_ )
		{
			reset( false );
		}
		fraction( const fraction_ &Fraction )
		: fraction_( S_ )
		{
			reset( false );

			Init( Fraction.N, Fraction.D );
		}
		fraction( const fraction &Fraction )
		: fraction_( S_ )
		{
			reset( false );

			Init( Fraction.N, Fraction.D );
		}
		fraction(
			const integer_ &Numerator,
			const integer_ &Denominator = integer( 1 ) )
		: fraction_( S_ )
		{
			reset( false );

			Init( Numerator, Denominator );
		}
		fraction(
			bso::s32__ Numerator,
			bso::u32__ Denominator = 1 )
		: fraction_( S_ )
		{
			reset( false );

			Init( Numerator, Denominator );
		}
	};		

	void Simplify( fraction_ &Fraction );

	template <typename t, typename t_> const _fraction_<t, t_> &_fraction_<t, t_>::Simplify( void )
	{
		mthfrc::Simplify( *this );

		return *this;
	}

	inline void Mul(
		const fraction_ &Fraction,
		const integer_ &Integer,
		fraction_ &Result )
	{
		Mul( Fraction, fraction( Integer ), Result );
	}

	inline void Mul(
		const integer_ &Integer,
		const fraction_ &Fraction,
		fraction_ &Result )
	{
		Mul( Fraction, Integer, Result );
	}

	inline void Add(
		const fraction_ &Fraction,
		const integer_ &Integer,
		fraction_ &Result )
	{
		Add( Fraction, fraction( Integer ), Result );
	}

	inline void Add(
		const integer_ &Integer,
		const fraction_ &Fraction,
		fraction_ &Result )
	{
		Add( Fraction, Integer, Result );
	}

/*
	class long_fraction__
	: public core_fraction<bso::slong__>
*/
}

inline mthfrc::fraction operator +(
	const mthfrc::fraction_ &Op1,
	const mthfrc::fraction_ &Op2 )
{
	mthfrc::fraction Result;

	Result.Init();

	mthfrc::Add( Op1, Op2, Result );

	return Result;
}

inline mthfrc::fraction operator -(
	const mthfrc::fraction_ &Op1,
	const mthfrc::fraction_ &Op2 )
{
	mthfrc::fraction Result;

	Result.Init();

	mthfrc::Sub( Op1, Op2, Result );

	return Result;
}

inline mthfrc::fraction operator *(
	const mthfrc::fraction_ &Op1,
	const mthfrc::fraction_ &Op2 )
{
	mthfrc::fraction Result;

	Result.Init();

	mthfrc::Mul( Op1, Op2, Result );

	return Result;
}

inline mthfrc::fraction operator /(
	const mthfrc::fraction_ &Op1,
	const mthfrc::fraction_ &Op2 )
{
	mthfrc::fraction Result;

	Result.Init();

	mthfrc::Div( Op1, Op2, Result );

	return Result;
}

inline txf::text_oflow__ &operator <<(
	txf::text_oflow__ &TOFlow,
	const mthfrc::fraction_ &Fraction )
{
	return TOFlow << Fraction.N << '/' << Fraction.D;
}


/*$END$*/
#endif
