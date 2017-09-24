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

#define MTHITG__COMPILATION

#include "mthitg.h"

#include "flx.h"

#include <math.h>

using namespace mthitg;

// 'Result' is shifted before adding the result of the operation.
namespace {
	integer_ &Divide_(
		const integer_ &Num,
		const integer_ &Den,
		integer_ &Result,
		integer_ &Remainder )
	{
	qRH
		bso::u32__ Inf = 0UL, Sup = 0x10000UL, Rep = 0x10000UL >> 1;
		integer Inter, Comp;
	qRB
		Inter.Init();
		Comp.Init();

		Inter = 0;
		Comp = 0;

		for ( ;;) {
			Inter.Init();
			Mul_( Den, integer( Rep ), Inter );
			Sub_( Num, Inter, Inter );
			if ( Inter < Comp ) {
				Sup = Rep;
				Rep = ( Sup + Inf ) >> 1;
			} else if ( Inter >= Den ) {
				Inf = Rep;
				Rep = Inf + ( ( Sup - Inf ) >> 1 );
				// '( Sup + Inf ) >> 1' doesn't work : overflow.
				// nor '( Sup >> 1 ) + ( Inf >> 1 )' : loss of precision.
			} else {
				Mul_( Result, integer( 0x10000 ), Result );
				Add_( Result, integer( Rep ), Result );
				Remainder = Inter;
				qRReturn;
			}
		}
	qRR
	qRT
	qRE
		return Result;
	}
}

bso::sign__ mthitg::Comp(
	const integer_ &Op1,
	const integer_ &Op2 )
{
	int Res;

	if ( !Op1.GetSignFlag() && Op2.GetSignFlag() )
		return 1;
	else if ( !Op2.GetSignFlag() && Op1.GetSignFlag() )
		return -1;

	size__ Op1Size = Op1.GetSize();
	size__ Op2Size = Op2.GetSize();

	if ( Op1Size < Op2Size )
		Res = -1;
	else if ( Op1Size > Op2Size )
		Res = 1;
	else if ( Op1Size == 0 )	// If true 'Op2Size' also equal to 0.
		Res = 0;
	else
	{
		size__ Indice = Op2Size;

		do Indice--;
		while( ( Op1.Core( Indice )
				 == Op2.Core( Indice ) )
				&& Indice );

		if ( Op1.Core( Indice ) != Op2.Core( Indice ) )
			if ( Op1.Core( Indice ) > Op2.Core( Indice ) )
				Res = 1;
			else
				Res = -1;
		else return 0;

	}

	return (int)( Op1.GetSignFlag() ? -Res : Res );
}		

integer_ &mthitg::Add_(
	const integer_ &Op1,
	const integer_ &Op2,
	integer_ &Result )
{
qRH
	integer ResTemp;
	const integer_ *Max = &Op1, *Min = &Op2;
	bso::u16__ Indice = 0;
	bso::u32__ Retenue = 0;
	bso::u16__ Limite;
qRB
	ResTemp.Init();

	size__ MaxSize, MinSize;

	if ( ( MaxSize = Max->GetSize() ) < ( MinSize = Min->GetSize() ) )
	{
		size__ Buffer = MaxSize;
		MaxSize = MinSize;
		MinSize = Buffer;

		Max = &Op2;
		Min = &Op1;
	}

	ResTemp.Core.Allocate( MaxSize + 1 );

	Limite = MinSize;

	while( Indice < Limite )
	{
		Retenue = ( Retenue >> 16 )
			  + Min->Core( Indice )
			  + Max->Core( Indice);

		ResTemp.Core.Store( (base__)( Retenue & 0xffff ), Indice++ );
	}

	Limite = MaxSize;

	while( Indice < Limite )
	{
		Retenue = ( Retenue >> 16 )
			  + Max->Core( Indice );

		ResTemp.Core.Store( (base__)( Retenue & 0xffff ), Indice++ );
	}

	if ( Indice >= MTHITG_MAX_SIZE )
		qRLmt();

	ResTemp.Core.Store( (base__)( Retenue >> 16 ), Indice++ );
	ResTemp.PutSize_( Indice );
	ResTemp.PutSignFlag_( false );
	ResTemp.Adjust_();

	Result = ResTemp;
qRR
qRT
qRE
	return Result;
}
	
integer_ &mthitg::Sub_(
	const integer_ &Op1,
	const integer_ &Op2,
	integer_ &Result )
{
qRH
	integer Res;
	unsigned Indice = 0;
	size__ Limite;
	bso::u32__ Inter = 0;
	const integer_ *Max = &Op1, *Min = &Op2;
qRB
	Res.Init( 0 );

	if ( *Max < *Min )
	{
		Res.PutSignFlag_( true );
		Max = &Op2;
		Min = &Op1;
	}
	else
		Res.PutSignFlag_( false );

	size__ MaxSize = Max->GetSize(), MinSize = Min->GetSize();

	if ( MaxSize == 0 ) {
		Result = Res;
		qRReturn;
	}

	Res.Core.Allocate( MaxSize );

	Res.PutSize_( MaxSize );

	Limite = MinSize;

	while ( Indice < Limite )
	{
		if ( Max->Core( Indice ) < ( Inter += Min->Core( Indice ) ) )
		{
			Res.Core.Store(
				( (base__)( ( Max->Core( Indice ) | ( 1 << 16 ) )
					- ( Inter & 0xffff ) ) ),
				Indice );

			Inter = 1;
		}
		else
		{
			Res.Core.Store( (base__)( Max->Core(Indice) - ( Inter & 0xffff ) ), Indice );

			Inter = 0;
		}

		Indice++;
	}

	Limite = MaxSize;

	while( Indice < Limite )
	{
		if ( Max->Core( Indice ) < Inter )

		{
			Res.Core.Store(
				( (base__)( ( Max->Core( Indice ) | ( 1 << 16 ) )
					- ( Inter & 0xffff ) ) ),
					Indice );

			Inter = 1;
		}
		else
		{
			Res.Core.Store( (base__)( Max->Core( Indice ) - ( Inter & 0xffff ) ), Indice );

			Inter = 0;
		}

		Indice++;
	}

	Res.Adjust_();

	Result = Res;
qRR
qRE
qRT
	return Result;
}


integer_ &mthitg::Mul_(
	const integer_ &Op1,
	const integer_ &Op2,
	integer_ &Result )
{
qRH
	integer Inter, Res;
	unsigned long Retenue;
	size__ LimiteS, LimiteP, IndiceS = 0, IndiceP = 0;
	const integer_ *Max = &Op1, *Min = &Op2;
qRB
	Inter.Init();
	Res.Init( 0 );

	if ( *Max < *Min ) {
		Max = &Op2;
		Min = &Op1;
	}

	size__ MaxSize = Max->GetSize(), MinSize = Min->GetSize();

	if ( ( MaxSize + MinSize ) > MTHITG_MAX_SIZE )
		qRLmt();

	Inter.PutSignFlag_( false );

	Inter.Core.Allocate( MaxSize + MinSize );

	LimiteS = MinSize;
	LimiteP = MaxSize;

	while ( IndiceS < LimiteS )
	{
		IndiceP = 0;
		Retenue = 0;

		while ( IndiceP < LimiteP )
		{
			Retenue = ( Retenue >> 16 )
				  + (unsigned long)Max->Core(IndiceP)
					* (unsigned long)Min->Core(IndiceS);
			Inter.Core.Store( (base__)( Retenue & 0xffff ), IndiceP+IndiceS );
			IndiceP++;
		}

		Inter.Core.Store( (base__)( Retenue >> 16 ), IndiceP+IndiceS );
		Inter.PutSize_( IndiceP + IndiceS + 1 );

		Add_( Inter, Res, Res );;
		IndiceS++;

		for( size__ C = 0; C < LimiteS + LimiteP; C++ )
			Inter.Core.Store( 0, C );

	}

	Res.Adjust_();

	Result = Res;
qRR
qRT
qRE
	return Result;
}


integer_ &mthitg::Div_(
	const integer_ &Num,
	const integer_ &Den,
	integer_ &Quotient,
	integer_ *RemainderP )
{
qRH
	integer Result;
	unsigned long I;
	integer Remainder, TenThousendHex;
qRB
	Result.Init( 0 );
	Remainder.Init();
	TenThousendHex.Init( 0x10000 );

	if ( Den > Num )
	{
		Remainder = Num;
	} else {
		size__ DenSize = Den.GetSize(), NumSize = Num.GetSize();

		Remainder.Core.Allocate( DenSize );

		Remainder.PutSize_( DenSize );
		Remainder.PutSignFlag_( false );

		Remainder.Core.Store( Num.Core, DenSize, 0, I = NumSize - DenSize );

		while ( I-- ) {
			Divide_( Remainder, Den, Result, Remainder );
			Mul_( Remainder, TenThousendHex, Remainder );
			Add_( Remainder, integer( Num.Core( I ) ), Remainder );
		}

		Divide_( Remainder, Den, Result, Remainder );
	}

	Quotient = Result;

	if ( RemainderP != NULL )
		*RemainderP = Remainder;
qRR
qRT
qRE
	return Quotient;
}


void mthitg::integer_::Init( flw::sIFlow &Flow )
{
	if ( Flow.EndOfFlow() )
		return;
qRH
	integer Ten, Inter;
	bso::sBool IsNegative = false;
qRB
	Ten.Init( 10 );

	switch ( Flow.View() ) {
	case '-':
		IsNegative = true;
	case '+':
		Flow.Skip();
	default:
		break;
	}

	Inter.Init( 0 );

	while ( !Flow.EndOfFlow() && isdigit( Flow.View() ) ) {
		Mul( Ten, Inter, Inter );
		Add( wInteger( Flow.Get() - '0' ), Inter, Inter );
	}

	*this = Inter;

	if ( IsNegative )
		Inter.Negate();
qRR
qRT
qRE
}

void mthitg::integer_::Init( const str::dString &Integer )
{
qRH
	flx::sStringIFlow Flow;
qRB
	S_.Size = 0;
	Core.Init();

	Flow.Init( Integer );
	Init( Flow );
qRR
qRT
qRE
}

namespace {
	integer_ &Negate_( integer_ &Integer )
	{
		Integer.Negate();

		return Integer;
	}
}

integer_ &mthitg::Add(
	const integer_ &Op1,
	const integer_ &Op2,
	integer_ &Result )
{
	if ( Op1.GetSignFlag() && Op2.GetSignFlag() )
		return Negate_( Add_( Op1, Op2, Result ) );
	else if ( Op1.GetSignFlag() )
		return Sub_( Op2, Op1, Result );
	else if ( Op2.GetSignFlag() )
		return Sub_( Op1, Op2, Result );
	else
		return Add_( Op1, Op2, Result );
}

integer_ &mthitg::Sub(
	const integer_ &Op1,
	const integer_ &Op2,
	integer_ &Result )

{
	if ( Op1.GetSignFlag() && Op2.GetSignFlag() )
		return Sub_( Op2, Op1, Result );
	else if ( Op1.GetSignFlag() )
		return Negate_( Add_( Op1, Op2, Result ) );
	 else if ( Op2.GetSignFlag() )
		return Add_( Op1, Op2, Result );
	else
		return Sub_( Op1, Op2, Result );
}

integer_ &mthitg::Mul(
	const integer_ &Op1,
	const integer_ &Op2,
	integer_ &Result )
{
	if ( Op1.GetSignFlag() == Op2.GetSignFlag() )
		return Mul_( Op1, Op2, Result );
	else 
		return Negate_( Mul_( Op1, Op2, Result ) );
}

integer_ &mthitg::Div(
	const integer_ &Num,
	const integer_ &Den,
	integer_ &Quotient,
	integer_ *Remainder )
{
	if ( Num.GetSignFlag() != Den.GetSignFlag() )
		return Negate_( Div_( Num, Den, Quotient, Remainder ) );
	else
		return Div_( Num, Den, Quotient, Remainder );
}

integer_ &mthitg::Mod(
	const integer_ &Num,
	const integer_ &Den,
	integer_ &Result )
{
qRH
	integer Quotient, Remainder;
qRB
	Quotient.Init();
	Remainder.Init();
	Div( Num, Den, Quotient, &Remainder );

	if ( Num.GetSignFlag() != Den.GetSignFlag() )
		Sub( Den, Remainder, Remainder );

	if ( Num.GetSignFlag() )
		Remainder.Negate();

	Result = Remainder;
qRR
qRE
qRT
	return Result;
}

void mthitg::PGCD(
	const integer_ &Op1,
	const integer_ &Op2,
	integer_ &Result )
{
qRH
	integer A, B;
qRB
	A.Init( Op1 );
	A.Abs();

	B.Init( Op2 );
	B.Abs();

	if ( !B ) {
		Result = A;
		qRReturn;
	} else if ( !A  ) {
		Result = B;
		qRReturn;
	}

	while ( A != B )  {
		if ( A > B )  {
			Mod( A, B, A );

			if ( !A )
				A = B;
		}
		else {
			Mod( B, A, B );

			if ( !B )
				B = A;
		}
	 }

	Result = A;
qRR
qRT
qRE
}


	
void integer_::Adjust_( aem::mode__ Mode )
{
	size__ Size = GetSize();

	if ( Size < _GetRawSize() )
	{
		if ( Size == 0 )
			Size++;

		Core.Allocate( Size, Mode );

		PutSize_( Size );
	}
}

bso::lfloat__ integer_::GetLongFloat( qRPN ) const
{
	bso::lfloat__ Resultat = 0;
	size__ Compteur = GetSize();

	while( Compteur-- && ( Resultat < ( BSO_LFLOAT_MAX / 10 ) ) )
		Resultat += Core(Compteur) * pow( (double)0x10000UL, Compteur );

	if ( Compteur != (size__)-1 )
		if ( qRP == err::hThrowException )
			qRFwk();

	if ( GetSignFlag() )
		return -Resultat;
	else
		return Resultat;
}

integer Exp(
	const integer_ &X,
	int Y )
{
	integer Res;

	Res.Init();
	Res = 1;
	
	while( Y-- )
		Mul( Res, X, Res );
		
	return Res;
}

# if 0
txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const integer_ &Integer )
{
qRH
	int Compteur, Chiffre;
	integer I;
	bso::lfloat__ Float;
qRB
	I.Init();

	I = Integer;

	if ( !I ) {
		Flow << 0UL;
		qRReturn;
	} else if ( I.GetSign() == -1 )	{
		Flow << '-';
		I.Negate();
	}

	Float = I.GetLongFloat( err::hUserDefined );

	if ( Float == HUGE_VALL )
		Flow << Float;
	else {
		Compteur = (int)log10( Float );

		while ( Compteur ) {
			Chiffre = (int)( I.GetLongFloat() / pow( 10.0, Compteur ) );

			Flow << (unsigned long)Chiffre;

			I -= Exp( integer( 10 ), Compteur ) * integer( Chiffre );
			Compteur--;
		}

		Flow << I.GetLongFloat();
	}
qRR
qRT
qRE
	return Flow;

}
#else
txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const integer_ &Integer )
{
qRH
	integer I, R, Ten;
	str::wString String;
qRB
	I.Init( Integer );

	Ten.Init( 10 );

	if ( !I ) {
		Flow << 0UL;
		qRReturn;
	} else if ( I.GetSign() == -1 )	{
		Flow << '-';
		I.Negate();
	}

	String.Init();

	while ( I.GetSign() != 0 ) {
		R.Init();
		Div( I, Ten, I, R );
		String.InsertAt( R.GetU32() + '0', 0 );
	}

	Flow << String;
qRR
qRT
qRE
	return Flow;
}
#endif
