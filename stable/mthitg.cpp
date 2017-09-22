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

// Return the rest of the operation, and 'Result' is shifted before adding the result of the operation.
integer mthitg::Divide_(
	const integer_ &Num,
	const integer_ &Den,
	integer_ &Result )
{
	bso::u32__ Inf = 0UL, Sup = 0x10000UL, Rep = 0x10000UL >> 1;
	integer Inter, Comp;

	Inter.Init();
	Comp.Init();

	Inter = 0;
	Comp = 0;
	
	for (;;) {
		if ( ( Inter = Num - Den * integer( Rep ) ) < Comp )
		{
			Sup = Rep;
			Rep = ( Sup + Inf ) >> 1;
		}
		else if ( Inter >= Den )
		{
			Inf = Rep;
			Rep = Inf + ( ( Sup - Inf ) >> 1 );
				// ( Sup + Inf ) >> 1 ne marche pas (dpassement de capacit)
				// ni ( Sup >> 1 ) + ( Inf >> 1 ) (perte de prcision)
		}
		else
		{
			Result = Result * integer( 0x10000 ) + integer( Rep );
			return Inter;
		}
	}
}

int mthitg::Comp_(
	const integer_ &Op1,
	const integer_ &Op2 )
{
	int Res;

	if ( !Op1._GetSignFlag() && Op2._GetSignFlag() )
		return 1;
	else if ( !Op2._GetSignFlag() && Op1._GetSignFlag() )
		return -1;

	size__ Op1Size = Op1._GetRealSize();
	size__ Op2Size = Op2._GetRealSize();

	if ( Op1Size < Op2Size )
		Res = -1;
	else if ( Op1Size > Op2Size )
		Res = 1;
	else if ( Op1Size == 0 )	// If true 'Op2Size' also equal to 0.
		Res = 0;
	else
	{
		size__ Indice = Op2Size;

		do
		{
			Indice--;
		}
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

	return (int)( Op1._GetSignFlag() ? -Res : Res );
}		

integer mthitg::Add_(
	const integer_ &Op1,
	const integer_ &Op2 )
{
	integer ResTemp;
	const integer_ *Max = &Op1, *Min = &Op2;
	bso::u16__ Indice = 0;
	bso::u32__ Retenue = 0;
	bso::u16__ Limite;

	ResTemp.Init();

	size__ MaxSize, MinSize;

	if ( ( MaxSize = Max->_GetRealSize() ) < ( MinSize = Min->_GetRealSize() ) )
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

	return ResTemp;
}
	
integer mthitg::Sub_(
	const integer_ &Op1,
	const integer_ &Op2 )
{
	integer Res;
	unsigned Indice = 0;
	size__ Limite;
	bso::u32__ Inter = 0;
	const integer_ *Max = &Op1, *Min = &Op2;

	Res.Init( 0 );

	if ( *Max < *Min )
	{
		Res.PutSignFlag_( true );
		Max = &Op2;
		Min = &Op1;
	}
	else
		Res.PutSignFlag_( false );

	size__ MaxSize = Max->_GetRealSize(), MinSize = Min->_GetRealSize();

	if ( MaxSize == 0 )
		return Res;

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

	return Res;
}


integer mthitg::Mul_(
	const integer_ &Op1,
	const integer_ &Op2 )
{
	integer Inter, Res;
	unsigned long Retenue;
	size__ LimiteS, LimiteP, IndiceS = 0, IndiceP = 0;
	const integer_ *Max = &Op1, *Min = &Op2;

	Inter.Init();
	Res.Init();

	Res = 0;

	if ( *Max < *Min )
	{
		Max = &Op2;
		Min = &Op1;
	}

	size__ MaxSize = Max->_GetRealSize(), MinSize = Min->_GetRealSize();

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

		Res += Inter;
		IndiceS++;

		for( size__ C = 0; C < LimiteS + LimiteP; C++ )
			Inter.Core.Store( 0, C );

	}

	Res.Adjust_();

	return Res;
}


integer mthitg::Div_(
	const integer_ &Num,
	const integer_ &Den,
	integer_ &Reste )
{
	integer Result;
	unsigned long I;

	Result.Init();
	Result = 0;

	if ( Den > Num )
	{
		Reste = Num;
		return Result;
	}

	Reste.Init();

	size__ DenSize = Den._GetRealSize(), NumSize = Num._GetRealSize();
	
	Reste.Core.Allocate( DenSize );

	Reste.PutSize_( DenSize );
	Reste.PutSignFlag_( false );

	Reste.Core.Store( Num.Core, DenSize, 0, I = NumSize - DenSize );
	
	while( I-- )
	{
		Reste = Divide_( Reste, Den, Result ) * integer( 0x10000 ) + integer( Num.Core( I ) );

/*		Reste = Divide_( Reste, Den, Res );

		Reste.Core.Allocate( Reste.GetSize_() + 1 );
		Reste.Core.Write( Reste.Core, Reste.GetSize_(), 1 );
		Reste.PutSize_( Reste.GetSize_() + 1 );
		Reste.Core.Write( Num.Core( I ), 0 );
*/	}

	Reste = Divide_( Reste, Den, Result );

	return Result;
}


void mthitg::integer_::Init( flw::sIFlow &Flow )
{
	if ( Flow.EndOfFlow() )
		return;

	bso::sBool IsNegative = false;

	switch ( Flow.View() ) {
	case '-':
		IsNegative = true;
	case '+':
		Flow.Skip();
	default:
		break;
	}

	while ( !Flow.EndOfFlow() && isdigit( Flow.View() ) ) {
		*this =Mul( wInteger( 10 ) );
		*this =Add( wInteger( Flow.Get() - '0' ) );
	}
}

void mthitg::integer_::Init( const str::dString &Integer )
{
qRH
	flx::sStringIFlow Flow;
qRB
	Flow.Init( Integer );
	Init( Flow );
qRR
qRT
qRE
}

integer integer_::Add( const integer_ &Op2 ) const
{
	const integer_ &Op1 = *this;

	if ( Op1._GetSignFlag() && Op2._GetSignFlag() )
		return -Add_( -Op1, -Op2 );
	else if ( Op1._GetSignFlag() )
		return Sub_( Op2, -Op1 );
	else if ( Op2._GetSignFlag() )
		return Sub_( Op1, -Op2 );
	else
		return Add_( Op1, Op2 );
}

integer integer_::Sub( const integer_ &Op2 ) const
{
	const integer_ &Op1 = *this;

	if ( Op1._GetSignFlag() && Op2._GetSignFlag() )
		return Sub_( -Op2, -Op1 );
	else if ( Op1._GetSignFlag() )
		return -Add_( -Op1, Op2 );
	else if ( Op2._GetSignFlag() )
		return Add_( Op1, -Op2 );
	else
		return Sub_( Op1, Op2 );
}

integer integer_::Mul( const integer_ &Op2 ) const
{
	const integer_ &Op1 = *this;

	if ( Op1._GetSignFlag() && Op2._GetSignFlag() )
		return Mul_( -Op2, -Op1 );
	else if ( Op1._GetSignFlag() )
		return -Mul_( -Op1, Op2 );
	else if ( Op2._GetSignFlag() )
		return -Mul_( Op1, -Op2 );
	else
		return Mul_( Op1, Op2 );
}

integer integer_::Div( const integer_ &Den ) const
{
	const integer_ &Num = *this;
	integer Dummy;

	Dummy.Init();
	
	if ( Num._GetSignFlag() != Den._GetSignFlag() )
		return -Div_( ::Abs( Num ), ::Abs( Den ), Dummy );
	else
		return Div_( ::Abs( Num ), ::Abs( Den ), Dummy );
}

integer integer_::Mod( const integer_ &Den ) const
{
	const integer_ &Num = *this;
	integer Result;

	Result.Init();

	Div_( ::Abs( Num ), ::Abs( Den ), Result );

	if ( Num._GetSignFlag() != Den._GetSignFlag() )
		Result = Den - Result;

	if ( Num._GetSignFlag() )
		Result = -Result;

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
	A.Init();
	A = Op1.Abs();;

	B.Init();
	B = Op2.Abs();

	if ( !B ) {
		Result = A;
		qRReturn;
	} else if ( !A  ) {
		Result = B;
		qRReturn;
	}

	while ( A != B )  {
		if ( A > B )  {
			A = A % B;

			if ( !A )
				A = B;
		}
		else {
			B  = B % A;

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
	size__ Size = _GetRealSize();

	if ( Size < _GetRawSize() )
	{
		if ( Size == 0 )
			Size++;

		Core.Allocate( Size, Mode );

		PutSize_( Size );
	}
}

bso::lfloat__ integer_::GetLongFloat( void ) const
{
	bso::lfloat__ Resultat = 0;
	size__ Compteur = _GetRealSize();

	while( Compteur-- )
		Resultat += Core(Compteur) * pow( (double)0x10000UL, Compteur );

	if ( _GetSignFlag() )
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
		Res *= X;
		
	return Res;
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const integer_ &Integer )
{
	int Compteur, Chiffre;
	integer I;

	I.Init();

	I = Integer;

	if ( !I )
		return Flow << 0UL;
	else if ( I.GetSign() == -1 )
	{
		Flow << '-';
		I = -I;
	}

	Compteur = (int)log10( I.GetLongFloat() );

	while( Compteur )
	{
		Chiffre =  (int)( I.GetLongFloat() / pow( 10.0, Compteur ) );

		Flow << (unsigned long)Chiffre;

		I -= Exp( integer( 10 ), Compteur ) * integer( Chiffre );
		Compteur--;
	}

	return Flow << I.GetLongFloat();
}

