/*
	'mthtmc' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'mthtmc' header file ('mthtmc.h').
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id: mthtmc.cpp,v 1.1 2010/07/15 18:26:44 csimon Exp $

#define MTHTMC__COMPILATION

#include "mthtmc.h"

class mthtmctutor
: public ttr_tutor
{
public:
	mthtmctutor( void )
	: ttr_tutor( MTHTMC_NAME )
	{
#ifdef MTHTMC_DBG
		Version = MTHTMC_VERSION "\b\bD $";
#else
		Version = MTHTMC_VERSION;
#endif
		Owner = MTHTMC_OWNER;
		Date = "$Date: 2010/07/15 18:26:44 $";
	}
	virtual ~mthtmctutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "mthitg.h"

using namespace mthtmc;

using mthitg::integer_;
using mthitg::integer;

static inline bso::ulong__ ExtractItem_(
	const str::string_ String,
	epeios::row__ &Position )
{
	if ( Position == NONE )
		return 0;

	return String.ToUL( Position, &Position, str::bAuto );
}

static inline bso::bool__ TestPosition_(
	const str::string_ &String,
	epeios::row__ &Position )
{
	if ( Position == NONE )
		return false;

	bso::char__ C = String( Position );

	if ( ( C != '.' ) && ( C != ':' )  && ( C != ';' ) )
		return false;
	else {
		Position = String.Next( Position );
		return true;
	}
}

bso::bool__ xfps_::Init(
	const str::string_ &String,
	epeios::row__ &Position )
{
	epeios::row__ Error = NONE;
	bso::ulong__ FPS = String.ToUL( Position, &Error, str::bAuto );

	this->FPS.reset();
	S_.Type = t_Undefined;

	if ( FPS < 1 )
		return false;

	if ( Error == NONE )
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
	bso::ulong__ D,
	bso::ulong__ H,
	bso::ulong__ M,
	bso::ulong__ S,
	const integer_ &F,
	const xfps_ &XFPS,
	frame_count_ &FrameCount )
{
ERRProlog
	integer TotalMinutes;
	integer DFC;		// Drop frame correction.
ERRBegin
#ifdef MTHTMC_DBG
	if ( !XFPS.IsValid() )
		ERRu();
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
			ERRc();

		DFC *= TotalMinutes - TotalMinutes / integer( 10 );
	}

	FrameCount.Init( TotalMinutes * integer( 60 ) * XFPS.FPS + ( integer( S ) % integer( 60 ) ) * XFPS.FPS + integer( F ) % ( XFPS.FPS * integer( 60 ) ) - DFC );
ERRErr
ERREnd
ERREpilog
}

void mthtmc::Convert(
	bso::ulong__ D,
	bso::ulong__ H,
	bso::ulong__ M,
	bso::ulong__ S,
	const integer_ &F,
	const xfps_ &XFPS,
	timecode_ &Timecode )
{
ERRProlog
	frame_count FrameCount;
ERRBegin
	FrameCount.Init();
	Convert_( D, H, M, S, F, XFPS, FrameCount );

	Convert( FrameCount, XFPS, Timecode );
ERRErr
ERREnd
ERREpilog
}

void mthtmc::Convert(
	const frame_count_ &FrameCount,
	const xfps_ &XFPS,
	timecode_ &Timecode )
{
ERRProlog
	period Period;
ERRBegin
	Period.Init();
	Convert( XFPS, Period );

	Convert( FrameCount, Period, Timecode );
ERRErr
ERREnd
ERREpilog
}

bso::bool__ mthtmc::Convert(
	const str::string_ &String,
	epeios::row__ &Position,
	const xfps_ &DefaultXFPS,
	timecode_ &Timecode )
{
	bso::bool__ Success = false;
ERRProlog
	bso::ulong__ D = 0, H = 0, M = 0, S = 0, F = 0;
	xfps XFPS;
ERRBegin

#ifdef MTHTMC_DBG
	if ( !DefaultXFPS.IsValid() )
		ERRu();
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

	if ( Position != NONE ) {
		if ( ( String( Position ) == '!' ) || ( String( Position ) == '@' ) || ( String( Position ) == '|' ) ) {
			if ( ( Position = String.Next( Position ) ) != NONE )
				if ( ! XFPS.Init( String, Position ) )
					ERRReturn;
		} else {
			XFPS.reset();	// To signal an error.
			ERRReturn;
		}
	} else
		XFPS = DefaultXFPS;

	Convert( D, H, M, S, integer( F ), XFPS, Timecode );

	Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}

static inline bso::ulong__ Round_(
	bso::ulong__ Value,
	bso::lfloat__ Remainder,
	bso::sign__ Rounding,	// -1 : floor, 0 : mathematic, 1 : ceil.
	bso::ubyte__ Precision,
	bso::ulong__ &Fract )
{
	if ( Value >= BSO_ULONG_MAX )
		ERRc();

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
			ERRc();
			break;
		}
	} else if ( Precision > 0 ) {

		//  optimiser !
		while ( Precision-- )
			Remainder *= 10;

		if ( Remainder > 999999999 )
			ERRc();

		switch ( Rounding ) {
		case -1:
			break;
		case 0:
			if ( ( Remainder - (bso::ulong__)Remainder ) >= 0.5 )
				Remainder++;
			break;
		case 1:
			if ( ( Remainder - (bso::ulong__)Remainder ) != 0 )
				Remainder++;
			break;
		default:
			ERRc();
			break;
		}
	} else
		ERRc();


	Fract = (bso::ulong__)Remainder;

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

bso::ulong__ Pow10_( bso::ubyte__ E )
{
	bso::ulong__ Result = 1;
#ifdef MTHTMC_DBG
	if ( E > 9 )
		ERRc();
#endif

	while ( E-- )
		Result *= 10;

	return Result;
}

static inline void HandleLimits_(
	bso::ulong__ &D,
	bso::ubyte__ &H,
	bso::ubyte__ &M,
	bso::ubyte__ &S,
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
	bso::ulong__ &D,
	bso::ubyte__ &H,
	bso::ubyte__ &M,
	bso::ubyte__ &S,
	frame_count_ &F )
{
ERRProlog
	integer DD;
	integer HD;
	integer MD;

	integer DI, HI, MI, SI, FI;
ERRBegin
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
	S = (bso::ubyte__)( SI ).GetULong();
	M = (bso::ubyte__)( MI ).GetULong();
	H = (bso::ubyte__)( HI ).GetULong();
	D = ( DI ).GetULong();
ERRErr
ERREnd
ERREpilog
}

static void ExtractAndAdjust_(
	const timecode_ &Timecode,
	const period_ &Period,
	const frame_count_ &FPS,
	type__ Type,
	bso::ulong__ &D,
	bso::ubyte__ &H,
	bso::ubyte__ &M,
	bso::ubyte__ &S,
	frame_count_ &F,
	fraction_ &Rest )
{
ERRProlog
	mthitg::integer FrameCount;
	integer PI, TI, Correction;
ERRBegin
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
		ERRc();

	switch ( Type ) {
	case tDF:
		{
			Correction.Init( 0 );

			if ( FPS == integer( 30 ) )
				Correction = 2;
			else if ( FPS == integer( 60 ) )
				Correction = 4;
			else
				ERRc();

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
		ERRc();
		break;
	}

	Extract_( FrameCount, FPS, D, H, M, S, F );
ERRErr
ERREnd
ERREpilog
}

static void Extract_(
	const timecode_ &Timecode,
	const period_ &Period,
	const frame_count_ &FPS,
	bso::ulong__ &D,
	bso::ubyte__ &H,
	bso::ubyte__ &M,
	bso::ubyte__ &S,
	frame_count_ &F,
	fraction_ &Rest )
{
ERRProlog
	mthitg::integer FrameCount;
	integer PI, TI, Correction;
ERRBegin
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
		ERRc();

	Extract_( FrameCount, FPS, D, H, M, S, F );
ERRErr
ERREnd
ERREpilog
}

static void Round_(
	const timecode_ &ToConvert,
	const frame_count_ &FPS,
	const period_ &Period,
	round_target__ Target,
	bso::sign__ Type,	// -1 : floor, 0 : mathematic, 1 : ceil.
	bso::ubyte__ Precision,
	timecode_ &Converted )
{
ERRProlog
	bso::ulong__ Fract = 0;
	bso::ulong__ D = 0;
	bso::ubyte__ H = 0;
	bso::ubyte__ M = 0;
	bso::ubyte__ S = 0;
	frame_count F;
	fraction FractPeriod, FrameRemainder;
	bso::lfloat__ TimeRemainder = 0;
	period Buffer;
ERRBegin
	if ( Precision > 9 )
		ERRc();

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
		H = (bso::ubyte__)Round_( H, ( M * 60 + S + TimeRemainder) / ( 60 * 60 ), Type, Precision, Fract );
		F = M = S = 0;
		HandleLimits_( D, H, M, S, F, FPS );
		FractPeriod.Init( 60 * 60, Pow10_( Precision ) );
		break;
	case rtM:
		M = (bso::ubyte__)Round_( M, ( S + TimeRemainder ) / 60, Type, Precision, Fract );
		F = S = 0;
		HandleLimits_( D, H, M, S, F, FPS );
		FractPeriod.Init( 60, Pow10_( Precision ) );
		break;
	case rtS:
		S = (bso::ubyte__)Round_( S, TimeRemainder, Type, Precision, Fract );
		F = 0;
		HandleLimits_( D, H, M, S, F, FPS );
		FractPeriod.Init( 1, Pow10_( Precision ) );
		break;
	case rtF:
		F = Round_( F.GetULong(), FrameRemainder.N.GetLongFloat() / FrameRemainder.D.GetLongFloat(), Type, Precision, Fract );
		HandleLimits_( D, H, M, S, F, FPS );
		FractPeriod.Init( Period.N, integer( Pow10_( Precision ) ) * Period.D );
		break;
	default:
		ERRc();
		break;
	}

	Converted = ( ( fraction( ( ( ( ( ( ( integer( D ) * integer( 24 ) ) + integer( H ) ) * integer( 60 ) ) + integer( M ) ) * integer( 60 ) ) + integer( S ) ) * FPS ) + fraction( F  ) ) * Period + fraction( Fract ) * FractPeriod );

	Converted.Simplify();
ERRErr
ERREnd
ERREpilog

}

static inline void Display_(
	const integer_ &Value,
	const fraction_ &RawRemainder,
	bso::sign__ Rounding,	// -1 : floor, 0 : mathematic, 1 : ceil.
	bso::char__ DecimalSeparator,
	bso::ubyte__ Width,
	bso::ubyte__ Precision,
	txf::text_oflow__ &TOFlow )
{
ERRProlog
	char Buffer[100];
	integer I;
	fraction Offset, Remainder;
ERRBegin
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
			ERRc();
			break;
		}
	} else if ( Precision > 0 ) {
		Offset.Init( 1, 2 );
		bso::ubyte__ Counter = Precision;

		// A optimiser.
		while ( Counter-- )
			Remainder.N *= integer( 10 );

		switch ( Rounding ) {
			case -1:
				break;
			case 0:
				if ( ( Remainder.N.Mod( Remainder.D ) * integer( 2 ) ) >= Remainder.D )
					Remainder.N += Remainder.D;	// Remainder += 1;
				break;
			case 1:
				if ( Remainder.N.Mod( Remainder.D ).GetSign() != 0 )
					Remainder.N += Remainder.D;	// Remainder += 1;
				break;
			default:
				ERRc();
				break;
		}
	} else
		ERRc();

	if ( I > integer( 0xffffffff ) )
		TOFlow << I;	// No 'Width' handling, because 'I' has more then 9 digit.
	else {
		sprintf( Buffer, "%0*.*u", Width, Width, I.GetULong() );
		TOFlow << Buffer;
	}

	if ( Precision ) {
		TOFlow << DecimalSeparator;

		// sprintf( Buffer, "%.*Lf", Precision, Remainder.GetLongFloat() );
		// TOFlow << (Buffer+2);

		sprintf( Buffer, "%u", ( Remainder.N / Remainder.D ).GetULong() );

		char *Pointer = strchr( Buffer, '0' );

		if ( Pointer != NULL )
			*Pointer = 0;

		TOFlow << Buffer;
	}

	Width = 1;
	Precision = 0;
ERRErr
ERREnd
ERREpilog
}

#define UNDEFINED_PRECISION	BSO_UBYTE_MAX
#define DEFAULT_PRECISION	0

#define D_( i, f )	Display_( ( i ), ( f ), -1, DecimalSeparator, Width, ( Precision == UNDEFINED_PRECISION ? DEFAULT_PRECISION : Precision ), TOFlow )

bso::bool__ mthtmc::Display(
	const timecode_ &RawTimecode,
	const xfps_ &XFPS,
	const str::string_ &Format,
	bso::char__ NonDropFramePunctuation,
	bso::char__ DropFramePunctuation,
	bso::char__ NoRealtimePunctuation,
	bso::char__ DefaultDecimalSeparator,
	txf::text_oflow__ &TOFlow )
{
	bso::bool__ Success = false;
ERRProlog
	fraction FrameRemainder;
	integer F;
	fraction TimeRemainder;
	bso::char__ C;
	bso::ulong__ D;
	bso::ubyte__ H, M, S, Width = 1, Precision = UNDEFINED_PRECISION;
	bso::bool__ MarkerPending = false;
	bso::bool__ RoundingPending = false;
	bso::char__ &DecimalSeparator = DefaultDecimalSeparator;
	bso::sign__ Rounding = 0;
	timecode Timecode;
	period Period;
ERRBegin
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

	epeios::row__ Row = Format.First();

	while ( Row != NONE ) {
		if ( Format( Row ) == '%' ) {
			MarkerPending = true;
			Row = Format.Next( Row );

			if ( Row == NONE )
				ERRReturn;
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
					ERRReturn;

				if ( Precision != UNDEFINED_PRECISION )
					ERRReturn;

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
				D_(  integer( D ), ( fraction( H * 60 * 60 + M * 60 + S ) + TimeRemainder ) / fraction( integer( 60 * 60 * 24 ) ) );
				break;
			case 'h':
				D_( integer( H ), ( fraction( M * 60 + S ) + TimeRemainder ) / fraction( 60 * 60 )  );
				break;
			case 'H':
				D_( integer( H + 24 * D ), ( fraction( M * 60 + S ) + TimeRemainder ) / fraction( 60 * 60 )  );
				break;
			case 'm':
				D_( integer( M ), ( fraction( S ) + TimeRemainder ) / fraction( 60 ) );
				break;
			case 'M':
				D_( integer( M ) + integer( 60 * ( H + 24 * D ) ), ( fraction( S ) + TimeRemainder ) / fraction( 60 ) );
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
				D_( ( Timecode / Period ).GetInteger(), FrameRemainder );
				break;
			case 'c':	// Complment.
				TOFlow << ( Timecode - RawTimecode ).GetLongFloat();
				break;
			case 'C':
				TOFlow << (Timecode - RawTimecode).Simplify();
				break;
			case 'p':
				switch ( XFPS.Type() ) {
				case tDF:
					TOFlow << DropFramePunctuation;
					break;
				case tNRT:
					TOFlow << NoRealtimePunctuation;
					break;
				case tNDF:
					TOFlow << NonDropFramePunctuation;
					break;
				default:
					ERRu();	// Not 'return false', because a problem with 'XFPS' should be detected before calling this function.
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

					if ( Row == NONE )
						ERRReturn;

					C = Format( Row );
				}

				if ( isdigit( C ) )
					ERRReturn;

				if ( ( C == '.' ) || ( C == ',' ) ) {
					MarkerPending = true;

					DecimalSeparator = C;

					Row = Format.Next( Row );

					if ( Row == NONE )
						ERRReturn;

					C = Format( Row );
				}

				if ( isdigit( C ) ) {
					MarkerPending = true;

					Precision = C - '0';

					Row = Format.Next( Row );

					if ( Row == NONE )
						ERRReturn;

					C = Format( Row );
				}

				if ( isdigit( C ) )
					ERRReturn;

				
				Row = Format.Previous( Row );

				if ( !MarkerPending )
					ERRReturn;

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
ERRErr
ERREnd
ERREpilog
	return Success;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class mthtmcpersonnalization
: public mthtmctutor
{
public:
	mthtmcpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mthtmcpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static mthtmcpersonnalization Tutor;

ttr_tutor &MTHTMCTutor = Tutor;
