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

namespace {
  rXMelody XMelody_;
  mtx::rMutex Mutex_ = mtx::Undefined;
}

namespace {
  bso::sS8 GetRegistryRawSignatureKey_( void )
  {
    bso::sS8 Key = sclm::MGetS8(registry::parameter::signature::Key, -8, 8);

    if ( Key == 0 ) // 0 usually stands for C Key, but 8 or -8 have to be used instead,
                    // to indicate the accidental to use for alterate note in C key.
      qRFwk();

    return Key;
  }

  sSignatureKey GetRegistrySignatureKey_( void )
  {
    bso::sS8 RawKey = GetRegistryRawSignatureKey_();

    if ( abs(RawKey) == 8 )
      return 0;
    else
      return RawKey;
  }

  sSignatureTime GetRegistrySignatureTime_( void )
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

  sSignature GetRegistrySignature_( void )
  {
    return sSignature(GetRegistrySignatureKey_(), GetRegistrySignatureTime_());
  }

  eAccidental GetRegistryAccidental_(void)
  {
    return GetRegistryRawSignatureKey_() > 0 ? aSharp : aFlat;
  }
}

void melody::Initialize(void)
{
  XMelody_.BaseOctave = sclm::MGetU8(registry::parameter::BaseOctave, 9);
  XMelody_.Signature = GetRegistrySignature_();
  XMelody_.Accidental = GetRegistryAccidental_();
}

rXMelody &melody::Get(hGuard &Guard)
{
  Guard.InitAndLock(Mutex_);

  return XMelody_;
}

bso::sS8 melody::Handle(
  sNote Note,
  rXMelody &XMelody)
{
  bso::sS8 RelativeOctave = GetOctave(Note, XMelody.Accidental) - XMelody.BaseOctave;

  if ( RelativeOctave < 0 )
    return RelativeOctave;

  if ( RelativeOctave > 3)
    return RelativeOctave - 3;

  if ( XMelody.Row == qNIL )
    XMelody.Append(Note);
  else if ( XMelody.Overwrite ) {
    XMelody.Store(Note, XMelody.Row);
    XMelody.Row = XMelody.Next(XMelody.Row);
  } else
    XMelody.InsertAt(Note, XMelody.Row);

  return 0;
}

namespace {
  void UpdateSignatureKey_(
    sSignatureKey Key,
    dMelody &Melody)
  {
    sNote Note;
    sRow Row = Melody.First();

    while ( Row != qNIL ) {
      Note.Init();

      Melody.Recall(Row, Note);
      Note.Signature.Key = Key;
      Melody.Store(Note, Row);

      Row = Melody.Next(Row);
    }
  }
}

void melody::HandleKeyAndAccidental(
  bso::sU8 Key,
  eAccidental Accidental,
  rXMelody &XMelody)
{
  XMelody.Accidental = Accidental;

  UpdateSignatureKey_(Accidental == aSharp ? Key : -Key, XMelody);
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
static sOctave GetOctave_(bso::sU8 Absolute)
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

eAccidental GetAccidental_(
	bso::sU8 Absolute,
	sSignatureKey Key )
{
	switch ( Absolute %12 ) {
	case 0:
		if ( Key >= 7 )
			return aSharp;
		else
			return aNatural;
		break;
	case 1:
		if ( Key <= -4 )
			return aFlat;
		else
			return aSharp;
		break;
	case 2:
		return aNatural;
		break;
	case 3:
		if ( Key <= -2 )
			return aFlat;
		else
			return aSharp;
		break;
	case 4:
		if ( Key <= -7 )
			return aFlat;
		else
			return aNatural;
		break;
	case 5:
		if ( Key >= 6 )
			return aSharp;
		else
			return aNatural;
		break;
	case 6:
		if ( Key <= -5 )
			return aFlat;
		else
			return aSharp;
		break;
	case 7:
		return aNatural;
		break;
	case 8:
		if ( Key <= -3 )
			return aFlat;
		else
			return aSharp;
		break;
	case 9:
		return aNatural;
		break;
	case 10:
		if ( Key <= -1 )
			return aFlat;
		else
			return aSharp;
		break;
	case 11:
		if ( Key <= -6 )
			return aFlat;
		else
			return aNatural;
		break;
	default:
		qRGnr();
		break;
	}

	qRGnr();

	return a_Undefined;	// To avoid a wanrning.
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

qGCTOR( melody )
{
  XMelody_.Init();
  Mutex_ = mtx::Create();
}
