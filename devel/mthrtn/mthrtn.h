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

// MaTH RaTional Numbers

#ifndef MTHRTN_INC_
# define MTHRTN_INC_

# define MTHRTN_NAME		"MTHRTN"

# if defined( E_DEBUG ) && !defined( MTHRTN_NODBG )
#  define MTHRTN_DBG
# endif

# include "err.h"

#include "mthitg.h"

namespace mthrtn {
	using namespace mthitg;

	template <typename t, typename t_> class dRational_ {
	public:
		struct s {
			typename t_::s D, N;
		};
		t_ D, N;
		dRational_( s &S )
			: D( S.D ),
			N( S.N )
		{}
		void reset( bso::bool__ P = true )
		{
			tol::reset( P, D, N );
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, D, N );
		}
		dRational_ &operator =( const dRational_ &R )
		{
			D = R.D;
			N = R.N;

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
		void Init( const dRational_ &R )
		{
			Init();

			operator =( R );
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
		integer ToInteger( void ) const
		{
			return N / D;
		}
		bso::lfloat__ ToLongFloat( void ) const
		{
			return N.GetLongFloat() / D.GetLongFloat();
		}
		const dRational_ &Simplify( void );	// Defined later.
	};

	qW2( Rational_ )

		template <typename t, typename t_> inline void Mul(
			const dRational_<t, t_> &Op1,
			const dRational_<t, t_> &Op2,
			dRational_<t, t_> &Result )
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
		const dRational_<t, t_> &Op1,
		const dRational_<t, t_> &Op2,
		dRational_<t, t_> &Result )
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
		const dRational_<t, t_> &Op1,
		const dRational_<t, t_> &Op2,
		dRational_<t, t_> &Result )
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
		const dRational_<t, t_> &Op1,
		const dRational_<t, t_> &Op2,
		dRational_<t, t_> &Result )
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

	typedef dRational_<integer, integer_> dRational;

	class wRational
		: public dRational {
	private:
		s S_;
	public:
		wRational( void )
			: dRational( S_ )
		{
			reset( false );
		}
		wRational( const dRational &Rational )
			: dRational( S_ )
		{
			reset( false );

			Init( Rational.N, Rational.D );
		}
		wRational( const wRational &Rational )
			: dRational( S_ )
		{
			reset( false );

			Init( Rational.N, Rational.D );
		}
		wRational(
			const integer_ &Numerator,
			const integer_ &Denominator = integer( 1 ) )
			: dRational( S_ )
		{
			reset( false );

			Init( Numerator, Denominator );
		}
		wRational(
			bso::s32__ Numerator,
			bso::u32__ Denominator = 1 )
			: dRational( S_ )
		{
			reset( false );

			Init( Numerator, Denominator );
		}
	};

	void Simplify( dRational &dRational );

	template <typename t, typename t_> const dRational_<t, t_> &dRational_<t, t_>::Simplify( void )
	{
		mthrtn::Simplify( *this );

		return *this;
	}

	inline void Mul(
		const dRational &Rational,
		const integer_ &Integer,
		dRational &Result )
	{
		Mul( Rational, wRational( Integer ), Result );
	}

	inline void Mul(
		const integer_ &Integer,
		const dRational &Rational,
		dRational &Result )
	{
		Mul( Rational, Integer, Result );
	}

	inline void Add(
		const dRational &Rational,
		const integer_ &Integer,
		dRational &Result )
	{
		Add( Rational, wRational( Integer ), Result );
	}

	inline void Add(
		const integer_ &Integer,
		const dRational &Rational,
		dRational &Result )
	{
		Add( Rational, Integer, Result );
	}
}

inline mthrtn::wRational operator +(
	const mthrtn::dRational &Op1,
	const mthrtn::dRational &Op2 )
{
	mthrtn::wRational Result;

	Result.Init();

	mthrtn::Add( Op1, Op2, Result );

	return Result;
}

inline mthrtn::wRational operator -(
	const mthrtn::dRational &Op1,
	const mthrtn::dRational &Op2 )
{
	mthrtn::wRational Result;

	Result.Init();

	mthrtn::Sub( Op1, Op2, Result );

	return Result;
}

inline mthrtn::wRational operator *(
	const mthrtn::dRational &Op1,
	const mthrtn::dRational &Op2 )
{
	mthrtn::wRational Result;

	Result.Init();

	mthrtn::Mul( Op1, Op2, Result );

	return Result;
}

inline mthrtn::wRational operator /(
	const mthrtn::dRational &Op1,
	const mthrtn::dRational &Op2 )
{
	mthrtn::wRational Result;

	Result.Init();

	mthrtn::Div( Op1, Op2, Result );

	return Result;
}

inline txf::text_oflow__ &operator <<(
	txf::text_oflow__ &TOFlow,
	const mthrtn::dRational &Rational )
{
	return TOFlow << Rational.N << '/' << Rational.D;
}

#endif
