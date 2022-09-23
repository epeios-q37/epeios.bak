/*
	Copyright (C) 2021 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'mscfdraftq' tool.

    'mscfdraftq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'mscfdraftq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'mscfdraftq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "melody.h"

#include "messages.h"
#include "midiq.h"
#include "registry.h"

#include "sclm.h"

#include "flf.h"

using namespace melody;

using namespace mscmld;

sSignatureKey melody::GetSignatureKey( void )
{
	return sclm::MGetS8( registry::parameter::signature::Key, -7, 7 );
}

sSignatureTime melody::GetSignatureTime( void )
{
  sSignatureTime Signature;
qRH;
  str::wString RawTime;
  flx::sStringRFlow Flow;
  bso::sU8 Num = 0, Den = 4;
qRB;
  RawTime.Init();
  sclm::MGetValue(registry::parameter::signature::Time, RawTime);

  Flow.Init(RawTime);

  if ( !Flow.GetNumber(Num, qRPU) )
    if ( ( Flow.Get() != '/' ) || !Flow.GetNumber(Den, qRPU ) )
      sclm::ReportAndAbort(ML(BadTimeSignature));

  Signature = sSignatureTime(Num, Den);
qRR;
qRT;
qRE;
  return Signature;
}

sSignature melody::GetSignature( void )
{
	return sSignature(GetSignatureKey(), GetSignatureTime());
}
/*
static const sDuration &GetTempoUnit_( sDuration &Unit )
{
qRH;
	str::wString RawUnit;
	bso::bool__ Missing = false;
	sdr::sRow Error = qNIL;
	bso::sU8 Base = 0;
	sdr::sRow Row = qNIL;
	bso::sU8 Modifier = 0;
qRB;
	RawUnit.Init();

	Missing = sclm::OGetValue( registry::parameter::tempo::Unit, RawUnit );

	if ( !Missing ) {
    Base = RawUnit.ToU8(&Error, str::sBase(10), str::sULimit<bso::sU8>(9));

    if ( Error != qNIL ) {
        if ( RawUnit( Error ) != '.' )
            sclr::ReportBadOrNoValueForEntryErrorAndAbort( registry::parameter::tempo::Unit );

        Row = Error;

        while ( (Row != qNIL) && (RawUnit( Row ) == '.') ) {
            Modifier++;
            Row = RawUnit.Next( Row );

            if ( Modifier >= 4 )
                sclr::ReportBadOrNoValueForEntryErrorAndAbort( registry::parameter::tempo::Unit );

            if ( Row != qNIL )
                sclr::ReportBadOrNoValueForEntryErrorAndAbort( registry::parameter::tempo::Unit );
        }
    }

    Unit = sDuration ( Base, Modifier );
	}
qRR;
qRT;
qRE;
	return Unit;
}

sTempo &melody::GetTempo(sTempo &Tempo)
{
	Tempo.Value = sclm::MGetU8( registry::parameter::tempo::Value );
	GetTempoUnit_( Tempo.Unit );

	return Tempo;
}
*/
static sPitchOctave GetOctave_(bso::sU8 Absolute)
{
	return Absolute / 12;
}

static ePitchName GetName_(
	bso::sU8 Absolute,
	sSignatureKey Key)
{
	switch ( Absolute % 12 ) {
	case 0:
		if ( Key >= 7 )
			return pnB;
		else
			return pnC;
		break;
	case 1:
		if ( Key <= -4 )
			return pnD;
		else
			return pnC;
		break;
	case 2:
		return pnD;
		break;
	case 3:
		if ( Key <= -2 )
			return pnE;
		else
			return pnD;
		break;
	case 4:
		if ( Key <= -7 )
			return pnF;
		else
			return pnE;
		break;
	case 5:
		if ( Key >= 6 )
			return pnE;
		else
			return pnF;
		break;
	case 6:
		if ( Key <= -5 )
			return pnG;
		else
			return pnF;
		break;
	case 7:
		return pnG;
		break;
	case 8:
		if ( Key <= -3 )
			return pnA;
		else
			return pnG;
		break;
	case 9:
		return pnA;
		break;
	case 10:
		if ( Key <= -1 )
			return pnB;
		else
			return pnA;
		break;
	case 11:
		if ( Key <= -6 )
			return pnC;
		else
			return pnB;
		break;
	default:
		qRGnr();
		break;
	}

	qRGnr();

	return pn_Undefined;	// To avoid a warning.
}

ePitchAccidental GetAccidental_(
	bso::sU8 Absolute,
	sSignatureKey Key )
{
	switch ( Absolute %12 ) {
	case 0:
		if ( Key >= 7 )
			return paSharp;
		else
			return paNatural;
		break;
	case 1:
		if ( Key <= -4 )
			return paFlat;
		else
			return paSharp;
		break;
	case 2:
		return paNatural;
		break;
	case 3:
		if ( Key <= -2 )
			return paFlat;
		else
			return paSharp;
		break;
	case 4:
		if ( Key <= -7 )
			return paFlat;
		else
			return paNatural;
		break;
	case 5:
		if ( Key >= 6 )
			return paSharp;
		else
			return paNatural;
		break;
	case 6:
		if ( Key <= -5 )
			return paFlat;
		else
			return paSharp;
		break;
	case 7:
		return paNatural;
		break;
	case 8:
		if ( Key <= -3 )
			return paFlat;
		else
			return paSharp;
		break;
	case 9:
		return paNatural;
		break;
	case 10:
		if ( Key <= -1 )
			return paFlat;
		else
			return paSharp;
		break;
	case 11:
		if ( Key <= -6 )
			return paFlat;
		else
			return paNatural;
		break;
	default:
		qRGnr();
		break;
	}

	qRGnr();

	return pa_Undefined;	// To avoid a wanrning.
}

namespace {
  bso::sBool PrintSignaturekey_(
    const sSignatureKey &Signature,
    txf::sWFlow &Flow)
  {
    if ( Signature < 0 )
      Flow << (bso::sUInt)-Signature << "♭";
    else if ( Signature > 0 )
      Flow << (bso::sUInt)Signature << "♯";
    else
      return false;

    return true;
  }

  void PrintSignatureTime_(
    const sSignatureTime &Signature,
    txf::sWFlow &Flow)
  {
    Flow << (bso::sUInt)Signature.Numerator() << '/' << (bso::sUInt)Signature.Denominator();
  }
}

void melody::PrintSignature(txf::sWFlow &Flow)
{
  const melody::sSignature Signature = GetSignature();

  if ( PrintSignaturekey_(Signature.Key, Flow) )
    Flow << ' ';

  PrintSignatureTime_(Signature.Time, Flow);
}

static void Print_(
	const sAltPitch &Pitch,
	txf::text_oflow__ &Flow )
{
	Flow << GetPitchNameLabel( Pitch);

	switch ( Pitch.Accidental ) {
	case paNatural:
		break;
	case paSharp:
		Flow << "♯";
		break;
	case paFlat:
		Flow << "♭";
		break;
	default:
		qRGnr();
		break;
	}
}

static void Print_(
	const sDuration &Duration,
	txf::text_oflow__ &Flow )
{
	bso::sU8 Modifier = Duration.Modifier;

	Flow << (char)((bso::sU8)Duration.Base + '0');

	while ( Modifier-- )
		Flow << '.';

	if ( Duration.TiedToNext )
		Flow << '-';
}

void melody::Print(
	const sNote &Note,
	txf::sWFlow &Flow,
	bso::bool__ Selected)
{
	if ( Selected )
		Flow << '<';

	Print_( Note.Pitch, Flow );

	Print_( Note.Duration, Flow );

	if ( Selected )
		Flow << '>';

	Flow << ' ' << txf::commit;
}

void melody::Print(
	const dMelody &Melody,
	sRow Current,
	txf::sWFlow &Flow)
{
qRH;
	sRow Row = qNIL;
qRB;
	Row = Melody.First();

	while ( Row != qNIL ) {
		Print(Melody( Row ), Flow, Current == Row);

		Row = Melody.Next( Row );
	}

	Flow << txf::commit << txf::nl;
qRR;
qRT;
qRE;
}

bso::sU32 ComputeTime_(
	sDuration Duration,
	bso::sU32 Base )	// Base : duration (in ms) of a whole.
{
	bso::sU32 Time = Base;

	switch ( Duration.Base ) {
	case 1 :
		break;
	case 2 :
		Time >>= 1;
		break;
	case 3 :
		Time >>= 2;
		break;
	case 4 :
		Time >>= 3;
		break;
	case 5 :
		Time >>= 4;
		break;
	case 6 :
		Time >>= 5;
		break;
	case 7 :
		Time >>= 6;
		break;
	case 8 :
		Time >>= 7;
		break;
	case 9 :
		Time >>= 8;
		break;
	default:
		qRGnr();
		break;
	}

	switch ( Duration.Modifier ) {
	case 0:
		break;
	case 1:
		Time += ( Time >> 1 );
		break;
	case 2:
		Time += ( Time >> 1 ) + ( Time >> 2 );
		break;
	case 3:
		Time += ( Time >> 1 ) + ( Time >> 2 ) + ( Time >> 3 );
		break;
	default:
		qRGnr();
		break;
	}

	return Time;
}

bso::sU32 ComputeBase_(const sTempo &Tempo)
{
	return 4 * ComputeTime_(Tempo.Unit, 4 * 60 * 1000 / Tempo.Value);
}

void melody::Play(
	const dMelody &Melody,
	const sTempo &Tempo,
	flw::oflow__ &Flow )
{
qRH;
	sRow Row = qNIL;
	bso::sU32 Base = 0;
	bso::sBool Tied = false;
qRB;
	Base = ComputeBase_( Tempo );

	Row = Melody.First();

	while ( Row != qNIL ) {
		midiq::Play( Melody( Row ), Base, Tied, Flow );

		Tied = false;

		Tied = Melody( Row ).Duration.TiedToNext;

		Row = Melody.Next( Row );
	}
qRR;
qRT;
qRE;
}
