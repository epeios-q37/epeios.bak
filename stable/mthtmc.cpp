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

#define MTHTMC__COMPILATION

#include "mthtmc.h"


#include "mthitg.h"

using namespace mthtmc;

using mthitg::integer_;
using mthitg::integer;

static inline base__ ExtractItem_(
	const str::string_ &String,
	sdr::row__ &Position )
{
	sdr::row__ PositionBuffer = Position;

	if ( Position == qNIL )
		return 0;

	Position = qNIL;	// Because if Position == 'qNil', 'ToU32(...)' assumes there was an error on a precedent call, and don't make any conversion.

	return String.ToU32( PositionBuffer, &Position, str::bAuto );
}

static inline bso::bool__ TestPosition_(
	const str::string_ &String,
	sdr::row__ &Position )
{
	if ( Position == qNIL )
		return false;

	bso::char__ C = String( Position );

	if ( ( C != '.' ) && ( C != ':' )  && ( C != ';' ) )
		return false;
	else {
		Position = String.Next( Position );
		return true;
	}
}

bso::bool__ dXFPS::Init(
	const str::string_ &String,
	sdr::row__ &Position )
{
	sdr::row__ Error = qNIL;
	base__ FPS = String.ToU32( Position, &Error, str::bAuto );

	this->FPS.reset();
	S_.Type = t_Undefined;

	if ( FPS < 1 )
		return false;

	if ( Error == qNIL )
		Init( FPS, tNDF );
	else if ( String( Error ) == 'd' ) {
		if ( ( FPS == 29 ) || ( FPS == 30 ) )
			Init( 30, tDF );
		else if ( ( FPS == 59 ) ||( FPS == 60 ) )
			Init( 60, tDF );
		else
			return false;
		Error = String.Next( Error );
	} else if ( String( Error ) == 'n' ) {
		if ( FPS == 23 )
			Init( 24, tNRT );
		else if ( FPS == 29 )
			Init( 30, tNRT );
		else if ( FPS == 59 )
			Init( 60, tNRT );
		else
			return false;
		Error = String.Next( Error );
	} else
		Init( FPS, tNDF );


	Position = Error;

	return true;
}

static void Convert_(
	base__ D,
	base__ H,
	base__ M,
	base__ S,
	const integer_ &F,
	const dXFPS &XFPS,
	frame_count_ &FrameCount )
{
qRH
	integer TotalMinutes;
	integer DFC;		// Drop frame correction.
qRB
#ifdef MTHTMC_DBG
	if ( !XFPS.IsValid() )
		qRFwk();
#endif
	TotalMinutes.Init();
	TotalMinutes = integer( D ) * integer( 24 * 60 ) + integer( H ) * integer( 60 ) + integer( M ) + integer( S ) / integer( 60 ) + integer( F ) / ( XFPS.FPS * integer( 60 ) );

	DFC.Init( 0 );

	if ( XFPS.GetType() == tDF ) {
		if ( XFPS.FPS == integer( 30 ) )
			DFC = 2;
		else if ( XFPS.FPS == integer( 60 ) )
			DFC = 4;
		else
			qRFwk();

		DFC *= TotalMinutes - TotalMinutes / integer( 10 );
	}

	FrameCount.Init( TotalMinutes * integer( 60 ) * XFPS.FPS + ( integer( S ) % integer( 60 ) ) * XFPS.FPS + integer( F ) % ( XFPS.FPS * integer( 60 ) ) - DFC );
qRR
qRT
qRE
}

void mthtmc::Convert(
	base__ D,
	base__ H,
	base__ M,
	base__ S,
	const integer_ &F,
	const dXFPS &XFPS,
	dTimecode &Timecode )
{
qRH
	frame_count FrameCount;
qRB
	FrameCount.Init();
	Convert_( D, H, M, S, F, XFPS, FrameCount );

	Convert( FrameCount, XFPS, Timecode );
qRR
qRT
qRE
}

void mthtmc::Convert(
	const frame_count_ &FrameCount,
	const dXFPS &XFPS,
	dTimecode &Timecode )
{
qRH
	wPeriod Period;
qRB
	Period.Init();
	Convert( XFPS, Period );

	Convert( FrameCount, Period, Timecode );
qRR
qRT
qRE
}

bso::bool__ mthtmc::Convert(
	const str::string_ &String,
	sdr::row__ &Position,
	const dXFPS &DefaultXFPS,
	dTimecode &Timecode )
{
	bso::bool__ Success = false;
qRH
	base__ D = 0, H = 0, M = 0, S = 0, F = 0;
	wXFPS XFPS;
qRB

#ifdef MTHTMC_DBG
	if ( !DefaultXFPS.IsValid() )
		qRFwk();
#endif

	F = ExtractItem_( String, Position );

	if ( TestPosition_( String, Position ) ) {
		S = F;
		F = ExtractItem_( String, Position );

		if ( TestPosition_( String, Position ) ) {
			M = S;
			S = F;
			F = ExtractItem_( String, Position );

			if ( TestPosition_( String, Position ) ) {
				H = M;
				M = S;
				S = F;
				F = ExtractItem_( String, Position );

				if ( TestPosition_( String, Position ) ) {
					D = H;
					H = M;
					M = S;
					S = F;
					F = ExtractItem_( String, Position );
				}
			}
		}
	}

	XFPS.Init();

	if ( Position != qNIL ) {
		if ( ( String( Position ) == '!' ) || ( String( Position ) == '@' ) || ( String( Position ) == '|' ) ) {
			if ( ( Position = String.Next( Position ) ) != qNIL )
				if ( ! XFPS.Init( String, Position ) )
					qRReturn;
		} else {
			XFPS.reset();	// To signal an error.
			qRReturn;
		}
	} else
		XFPS = DefaultXFPS;

	Convert( D, H, M, S, integer( F ), XFPS, Timecode );

	Success = true;
qRR
qRT
qRE
	return Success;
}

static inline base__ Round_(
	base__ Value,
	bso::lfloat__ Remainder,
	bso::sign__ Rounding,	// -1 : floor, 0 : mathematic, 1 : ceil.
	base__ Precision,
	base__ &Fract )
{
	if ( Value >= BSO_U32_MAX )
		qRFwk();

	if ( Precision == 0 ) {
		switch ( Rounding ) {
		case -1:
			break;
		case 0:
			if ( Remainder >= 0.5 )
				Value += 1;
			break;
		case 1:
			if ( Remainder != 0 )
				Value += 1;
			break;
		default:
			qRFwk();
			break;
		}
	} else if ( Precision > 0 ) {

		//  optimiser !
		while ( Precision-- )
			Remainder *= 10;

		if ( Remainder > 999999999 )
			qRFwk();

		switch ( Rounding ) {
		case -1:
			break;
		case 0:
			if ( ( Remainder - (base__)Remainder ) >= 0.5 )
				Remainder++;
			break;
		case 1:
			if ( ( Remainder - (base__)Remainder ) != 0 )
				Remainder++;
			break;
		default:
			qRFwk();
			break;
		}
	} else
		qRFwk();


	Fract = (base__)Remainder;

	return Value;
}

enum round_target__ {	// Spcification de l'unit concern par l'arrondissement.
	rtNone,	// Signale pas d'arrondissemnt.
	rtD,	// Nombre de jours ('Day').
	rtH,	// Nombre d'heures.
	rtM,	// Nombre de minutes.
	rtS,	// Nombre de secondes.
	rtF,		// Nombre de 'Frame's.
	rt_amount,
	rt_Undefined
};

base__ Pow10_( bso::u8__ E )
{
	base__ Result = 1;
#ifdef MTHTMC_DBG
	if ( E > 9 )
		qRFwk();
#endif

	while ( E-- )
		Result *= 10;

	return Result;
}

static inline void HandleLimits_(
	bso::u32__ &D,
	bso::u8__ &H,
	bso::u8__ &M,
	bso::u8__ &S,
	frame_count_ &F,
	const frame_count_ &FPS )
{
	if ( F == FPS ) {
		F.Init( 0 );
		S++;
	}

	if ( S == 60 ) {
		S = 0;
		M++;
	}

	if ( M == 60 ) {
		M = 0;
		H++;
	}

	if ( H == 24 ) {
		H = 0;
		D++;
	}
}

static void Extract_(
	const frame_count_ &FrameCount,
	const frame_count_ &FPS,
	bso::u32__ &D,
	bso::u8__ &H,
	bso::u8__ &M,
	bso::u8__ &S,
	frame_count_ &F )
{
qRH
	integer DD;
	integer HD;
	integer MD;

	integer DI, HI, MI, SI, FI;
qRB
//	FCD.Init( FrameCount * Period.N, Period.D );
	FI.Init( FrameCount );

	DD.Init( integer( 24 * 60 * 60 ) * FPS );
	HD.Init( integer( 60 * 60 ) * FPS );
	MD.Init( integer( 60 ) * FPS );

	DI.Init( FI / DD );
	FI = FI - DI * DD;

	HI.Init( FI / HD );
	FI = FI - HI * HD;

	MI.Init( FI / MD );
	FI = FI - MI * MD;

	SI.Init( FI / FPS );
	FI = FI - SI * FPS;


	F = FI;
	S = (bso::u8__)( SI ).GetU32();
	M = (bso::u8__)( MI ).GetU32();
	H = (bso::u8__)( HI ).GetU32();
	D = ( DI ).GetU32();
qRR
qRT
qRE
}

static void ExtractAndAdjust_(
	const dTimecode &Timecode,
	const dPeriod &Period,
	const frame_count_ &FPS,
	type__ Type,
	bso::u32__ &D,
	bso::u8__ &H,
	bso::u8__ &M,
	bso::u8__ &S,
	frame_count_ &F,
	dRational &Rest )
{
qRH
	mthitg::integer FrameCount;
	integer PI, TI, Correction;
qRB
/*
	if ( Frames < 0 )
		Frames = -Frames;
*/
	FrameCount.Init( Timecode.N * Period.D );

	Rest.N.Init( FrameCount % ( Period.N * Timecode.D ) );
	Rest.D.Init( Period.N * Timecode.D );

	Rest.Simplify();

	FrameCount /= Timecode.D * Period.N;

	if ( Rest.D.GetSign() == -1 )
		qRFwk();

	switch ( Type ) {
	case tDF:
		{
			Correction.Init( 0 );

			if ( FPS == integer( 30 ) )
				Correction = 2;
			else if ( FPS == integer( 60 ) )
				Correction = 4;
			else
				qRFwk();

			PI.Init( Correction * ( FrameCount / ( FPS * integer( 60 ) ) ) - Correction * ( FrameCount / ( FPS * integer( 600 ) ) ) );	// Ponctuel.
			TI.Init( 0 );	// Total.

			while ( PI.GetSign() != 0 ) {
				FrameCount += PI;
				TI += PI;
				PI = Correction * ( FrameCount / ( FPS * integer( 60 ) ) ) - Correction * ( FrameCount / ( FPS * integer( 600 ) ) ) - TI;
			}
		}
		break;
	case tNDF:
	case tNRT:
		break;
	default:
		qRFwk();
		break;
	}

	Extract_( FrameCount, FPS, D, H, M, S, F );
qRR
qRT
qRE
}

static void Extract_(
	const dTimecode &Timecode,
	const dPeriod &Period,
	const frame_count_ &FPS,
	bso::u32__ &D,
	bso::u8__ &H,
	bso::u8__ &M,
	bso::u8__ &S,
	frame_count_ &F,
	dRational &Rest )
{
qRH
	mthitg::integer FrameCount;
	integer PI, TI, Correction;
qRB
/*
	if ( Frames < 0 )
		Frames = -Frames;
*/
	FrameCount.Init( Timecode.N * Period.D );

	Rest.N.Init( FrameCount % ( Period.N * Timecode.D ) );
	Rest.D.Init( Period.N * Timecode.D );

	Rest.Simplify();

	FrameCount /= Timecode.D * Period.N;

	if ( Rest.D.GetSign() == -1 )
		qRFwk();

	Extract_( FrameCount, FPS, D, H, M, S, F );
qRR
qRT
qRE
}

static void Round_(
	const dTimecode &ToConvert,
	const frame_count_ &FPS,
	const dPeriod &Period,
	round_target__ Target,
	bso::sign__ Type,	// -1 : floor, 0 : mathematic, 1 : ceil.
	bso::u8__ Precision,
	dTimecode &Converted )
{
qRH
	bso::u32__ Fract = 0;
	bso::u32__ D = 0;
	bso::u8__ H = 0;
	bso::u8__ M = 0;
	bso::u8__ S = 0;
	frame_count F;
	wRational FractPeriod, FrameRemainder;
	bso::lfloat__ TimeRemainder = 0;
	wPeriod Buffer;
qRB
	if ( Precision > 9 )
		qRFwk();

	F.Init();
	FrameRemainder.Init();

/*
	XFPS.Init( OriginalXFPS.Period );
	XFPS.S_.Type = tNRT;
*/
	Extract_( ToConvert, Period, FPS, D, H, M, S, F, FrameRemainder );

	TimeRemainder = ( F.GetLongFloat() + FrameRemainder.N.GetLongFloat() / FrameRemainder.D.GetLongFloat() ) * ( Period.N.GetLongFloat()/Period.D.GetLongFloat());

	switch ( Target ) {
	case rtNone:
		break;
	case rtD:
		D = Round_( D, ( H * 60 * 60 + M * 60 + S + TimeRemainder ) / ( 60 * 60 * 24 ), Type, Precision, Fract );
		F = H = M = S = 0;
		HandleLimits_( D, H, M, S, F, FPS );
		FractPeriod.Init( 24 * 60 * 60, Pow10_( Precision ) );
		break;
	case rtH:
		H = (bso::u8__)Round_( H, ( M * 60 + S + TimeRemainder) / ( 60 * 60 ), Type, Precision, Fract );
		F = M = S = 0;
		HandleLimits_( D, H, M, S, F, FPS );
		FractPeriod.Init( 60 * 60, Pow10_( Precision ) );
		break;
	case rtM:
		M = (bso::u8__)Round_( M, ( S + TimeRemainder ) / 60, Type, Precision, Fract );
		F = S = 0;
		HandleLimits_( D, H, M, S, F, FPS );
		FractPeriod.Init( 60, Pow10_( Precision ) );
		break;
	case rtS:
		S = (bso::u8__)Round_( S, TimeRemainder, Type, Precision, Fract );
		F = 0;
		HandleLimits_( D, H, M, S, F, FPS );
		FractPeriod.Init( 1, Pow10_( Precision ) );
		break;
	case rtF:
		F = Round_( F.GetU32(), FrameRemainder.N.GetLongFloat() / FrameRemainder.D.GetLongFloat(), Type, Precision, Fract );
		HandleLimits_( D, H, M, S, F, FPS );
		FractPeriod.Init( Period.N, integer( Pow10_( Precision ) ) * Period.D );
		break;
	default:
		qRFwk();
		break;
	}

	Converted = ( ( wRational( ( ( ( ( ( ( integer( D ) * integer( 24 ) ) + integer( H ) ) * integer( 60 ) ) + integer( M ) ) * integer( 60 ) ) + integer( S ) ) * FPS ) + wRational( F  ) ) * Period + wRational( Fract ) * FractPeriod );

	Converted.Simplify();
qRR
qRT
qRE

}

static inline void Display_(
	const integer_ &Value,
	const dRational &RawRemainder,
	bso::sign__ Rounding,	// -1 : floor, 0 : mathematic, 1 : ceiling.
	bso::char__ DecimalSeparator,
	bso::u8__ Width,
	bso::u8__ Precision,
	txf::text_oflow__ &TOFlow )
{
qRH
	char Buffer[100];
	integer I;
	wRational Offset, Remainder;
qRB
	Remainder.Init();

	Remainder =  RawRemainder;

	Remainder.Simplify();

	I.Init( Value );

	if ( Precision == 0 ) {
		switch ( Rounding ) {
		case -1:
			break;
		case 0:
			if ( ( Remainder.N * integer( 2 ) ) >= Remainder.D )
				I.Init( I + integer( 1 ) );
			break;
		case 1:
			if ( Remainder.GetSign() != 0 )
				I.Init( I + integer( 1 ) );
			break;
		default:
			qRFwk();
			break;
		}
	} else if ( Precision > 0 ) {
		Offset.Init( 1, 2 );
		bso::u8__ Counter = Precision;

		// A optimiser.
		while ( Counter-- )
			Remainder.N *= integer( 10 );

		switch ( Rounding ) {
			case -1:
				break;
			case 0:
				if ( ( ( Remainder.N % Remainder.D ) * integer( 2 ) ) >= Remainder.D )
					Remainder.N += Remainder.D;	// Remainder += 1;
				break;
			case 1:
				if ( ( Remainder.N % Remainder.D ).GetSign() != 0 )
					Remainder.N += Remainder.D;	// Remainder += 1;
				break;
			default:
				qRFwk();
				break;
		}
	} else
		qRFwk();

	if ( I > integer( 0xffffffff ) )
		TOFlow << I;	// No 'Width' handling, because 'I' has more then 9 digit.
	else {
		sprintf( Buffer, "%0*.*u", Width, Width, I.GetU32() );
		TOFlow << Buffer;
	}

	if ( Precision ) {
		TOFlow << DecimalSeparator;

		// sprintf( Buffer, "%.*Lf", Precision, Remainder.GetLongFloat() );
		// TOFlow << (Buffer+2);

		sprintf( Buffer, "%u", ( Remainder.N / Remainder.D ).GetU32() );

		char *Pointer = strchr( Buffer, '0' );

		if ( Pointer != NULL )
			*Pointer = 0;

		TOFlow << Buffer;
	}

	Width = 1;
	Precision = 0;
qRR
qRT
qRE
}

#define UNDEFINED_PRECISION	BSO_U8_MAX
#define DEFAULT_PRECISION	0

#define D_( i, f )	Display_( ( i ), ( f ), -1, DecimalSeparator, Width, ( Precision == UNDEFINED_PRECISION ? DEFAULT_PRECISION : Precision ), TOFlow )

bso::bool__ mthtmc::Display(
	const dTimecode &RawTimecode,
	const dXFPS &XFPS,
	const str::string_ &Format,
	const tokens__ &Tokens,
	txf::text_oflow__ &TOFlow )
{
	bso::bool__ Success = false;
qRH
	wRational FrameRemainder;
	integer F, Buffer;
	wRational TimeRemainder;
	bso::char__ C;
	bso::u32__ D;
	bso::u8__ H, M, S, Width = 1, Precision = UNDEFINED_PRECISION;
	bso::bool__ MarkerPending = false;
	bso::bool__ RoundingPending = false;
	bso::char__ DecimalSeparator = Tokens.DS;
	bso::sign__ Rounding = 0;
	wTimecode Timecode;
	wPeriod Period;
qRB
	FrameRemainder.Init();
	F.Init();

	Period.Init();
	Convert( XFPS, Period );

	Timecode.Init();
	Round_( RawTimecode, XFPS.FPS, Period, rtF, Rounding, ( Precision == UNDEFINED_PRECISION ? DEFAULT_PRECISION : Precision ), Timecode );

//	SFC = RawSFC;


	ExtractAndAdjust_( Timecode, Period, XFPS.FPS, XFPS.GetType(), D, H, M, S, F, FrameRemainder );

//	TimeRemainder = ( F.GetLongFloat() + FrameRemainder.N.GetLongFloat() / FrameRemainder.D.GetLongFloat() ) * ( Period.N.GetLongFloat()/Period.D.GetLongFloat());
	TimeRemainder.Init( F );

	TimeRemainder = TimeRemainder + FrameRemainder;
	TimeRemainder = TimeRemainder * Period;

	sdr::row__ Row = Format.First();

	while ( Row != qNIL ) {
		if ( Format( Row ) == '%' ) {
			MarkerPending = true;
			Row = Format.Next( Row );

			if ( Row == qNIL )
				qRReturn;
		}

		if ( RoundingPending ) {
			RoundingPending = false;

			switch ( C = Format( Row ) ) {
			case 'd':
			case 'D':
				Timecode.Init();
				Round_( RawTimecode, XFPS.FPS, Period, rtD, Rounding, ( Precision == UNDEFINED_PRECISION ? DEFAULT_PRECISION : Precision ), Timecode );
				break;
			case 'h':
			case 'H':
				Timecode.Init();
				Round_( RawTimecode, XFPS.FPS, Period, rtH, Rounding, ( Precision == UNDEFINED_PRECISION ? DEFAULT_PRECISION : Precision ), Timecode );
				break;
			case 'm':
			case 'M':
				Timecode.Init();
				Round_( RawTimecode, XFPS.FPS, Period, rtM, Rounding, ( Precision == UNDEFINED_PRECISION ? DEFAULT_PRECISION : Precision ), Timecode );
				break;
			case 's':
			case 'S':
				Timecode.Init();
				Round_( RawTimecode, XFPS.FPS, Period, rtS, Rounding, ( Precision == UNDEFINED_PRECISION ? DEFAULT_PRECISION : Precision ), Timecode );
				break;
			case 'f':
			case 'F':
				Timecode.Init();
				Round_( RawTimecode, XFPS.FPS, Period, rtF, Rounding, ( Precision == UNDEFINED_PRECISION ? DEFAULT_PRECISION : Precision ), Timecode );
				break;
			default:
				if ( !isdigit( C ) )
					qRReturn;

				if ( Precision != UNDEFINED_PRECISION )
					qRReturn;

				Precision = C - '0';

				RoundingPending = true;
				break;
			}

			if ( !RoundingPending ) {
				ExtractAndAdjust_( Timecode, Period, XFPS.FPS, XFPS.GetType(), D, H, M, S, F, FrameRemainder );

//				TimeRemainder = ( F.GetLongFloat() + FrameRemainder.N.GetLongFloat() / FrameRemainder.D.GetLongFloat() ) * ( Period.N.GetLongFloat()/Period.D.GetLongFloat());
				TimeRemainder.Init( F );

				TimeRemainder = TimeRemainder + FrameRemainder;
				TimeRemainder = TimeRemainder * Period;

				Precision = UNDEFINED_PRECISION;
			}
		} else if ( MarkerPending ) {
			MarkerPending = false;

			switch ( C = Format( Row ) ) {
			case '%':
				TOFlow << '%';
				break;
			case 'd':
			case 'D':
				D_(  integer( D ), ( wRational( H * 60 * 60 + M * 60 + S ) + TimeRemainder ) / wRational( integer( 60 * 60 * 24 ) ) );
				break;
			case 'h':
				D_( integer( H ), (wRational( M * 60 + S ) + TimeRemainder ) / wRational( 60 * 60 )  );
				break;
			case 'H':
				D_( integer( H + 24 * D ), (wRational( M * 60 + S ) + TimeRemainder ) / wRational( 60 * 60 )  );
				break;
			case 'm':
				D_( integer( M ), (wRational( S ) + TimeRemainder ) / wRational( 60 ) );
				break;
			case 'M':
				D_( integer( M ) + integer( 60 * ( H + 24 * D ) ), (wRational( S ) + TimeRemainder ) / wRational( 60 ) );
				break;
			case 's':
				D_( integer( S ), TimeRemainder );
				break;
			case 'S':
				D_( integer( S + 60 * ( M + 60 * ( H + 24 * D ) ) ), TimeRemainder );
				break;
			case 'f':
				D_( F, FrameRemainder );
				break;
			case 'F':
				Buffer.Init();
				(Timecode / Period).ToInteger( Buffer );
				D_( Buffer, FrameRemainder );
				break;
			case 'r':	// Remainder as float.
				TOFlow << ( Timecode - RawTimecode ).GetLongFloat();
				break;
			case 'R':	// Remainder as fraction.
				TOFlow << (Timecode - RawTimecode).Simplify();
				break;
			case 't':
				switch ( XFPS.Type() ) {
				case tDF:
					TOFlow << Tokens.DF;
					break;
				case tNRT:
					TOFlow << Tokens.NRT;
					break;
				case tNDF:
					TOFlow << Tokens.NDF;
					break;
				default:
					qRFwk();	// Not 'return false', because a problem with 'XFPS' should be detected before calling this function.
					break;
				}
				break;
			case '<':
				Rounding = -1;
				RoundingPending = true;
				break;
			case '=':
				Rounding = 0;
				RoundingPending = true;
				break;
			case '>':
				Rounding = 1;
				RoundingPending = true;
				break;
			default:
				if ( isdigit( C ) ) {
					MarkerPending = true;

					Width = C - '0';

					Row = Format.Next( Row );

					if ( Row == qNIL )
						qRReturn;

					C = Format( Row );
				}

				if ( isdigit( C ) )
					qRReturn;

				if ( ( C == '.' ) || ( C == ',' ) ) {
					MarkerPending = true;

					DecimalSeparator = C;

					Row = Format.Next( Row );

					if ( Row == qNIL )
						qRReturn;

					C = Format( Row );
				}

				if ( isdigit( C ) ) {
					MarkerPending = true;

					Precision = C - '0';

					Row = Format.Next( Row );

					if ( Row == qNIL )
						qRReturn;

					C = Format( Row );
				}

				if ( isdigit( C ) )
					qRReturn;

				
				Row = Format.Previous( Row );

				if ( !MarkerPending )
					qRReturn;

				break;
			}

			if ( !MarkerPending ) {
				Width = 1;
				Precision = UNDEFINED_PRECISION;
			}
		} else
			TOFlow << Format( Row );

		Row = Format.Next( Row );
	}

	Success = true;
qRR
qRT
qRE
	return Success;
}

