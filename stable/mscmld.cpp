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

#define MSCMLD_COMPILATION_

#include "mscmld.h"

using namespace mscmld;

#include "mthrtn.h"

#define dMelodyTAG							"Melody"


#define SIGNATURE_TAG						"Signature"

#define KEY_TAG								"Key"

#define TIME_TAG							"Time"

#define ANACROUSIS_TAG						"Anacrousis"

namespace {
  qCDEFC( AnacrousisBaseAttributeLabel_, "Base" );
  qCDEFC( AnacrousisAmountAttributeLabel_, "Amount" );
}


#define NOTE_TAG							"Note"
#define PITCH_TAG							"Pitch"
#define DURATION_TAG						"Duration"
#define BASE_TAG							"Base"
#define MODIFIER_TAG						"Modifier"

#define DIFF_TAG							"Diff"

#define REST_TAG							"Rest"

#define BAR_TAG								"Bar"

#define RAW_ATTRIBUTE						"Raw"
#define NUMERATOR_ATTRIBUTE					"Numerator"
#define RAW_NUMERATOR_ATTRIBUTE				"RawNumerator"
#define DENOMINATOR_ATTRIBUTE				"Denominator"
#define DENOMINATOR_POWER_ATTRIBUTE			"DenominatorPower"
#define HIDDEN_DENOMINATOR_FLAG_ATTRIBUTE	"DenominatorIsHidden"

#define NAME_ATTRIBUTE						"Name"
#define ACCIDENTAL_ATTRIBUTE				"Accidental"
#define OCTAVE_ATTRIBUTE					"Octave"

#define TIED_FLAG_ATTRIBUTE					"TiedToNext"
#define TUPLET_TAG							"Tuplet"

#define DIFF_ATTRIBUTE						"Diff"

#define YES_VALUE							"yes"
#define NO_VALUE							"no"

namespace {
  qCDEFC( ChromaticAttributeLabel_, "Chromatic" );
  qCDEFC( DiatonicAttributeLabel_, "Diatonic" );
}

const char *mscmld::GetLabel(ePitchName Name)
{
  static const bso::sChar *Names[pn_amount] = {"a", "b", "c", "d", "e", "f", "g", "r"};

  if ( Name >= pn_amount )
    qRFwk();

  return Names[Name];
}

#define C( name )	case a##name : return #name; break

const char *mscmld::GetLabel(eAccidental Accidental)
{
	switch( Accidental ) {
	C( Flat );
	C( Natural );
	C( Sharp );
	default:
		qRFwk();
		break;
	}

	return NULL;	// To avoir a 'warning'.
}

#undef C

namespace {
	stsfsm::wAutomat AccidentalAutomat_;

	void FillAccidentalAutomat_( void )
	{
		AccidentalAutomat_.Init();
		stsfsm::Fill<eAccidental>( AccidentalAutomat_, a_amount, GetLabel );
	}
}

eAccidental mscmld::GetAccidental(const str::dString &Pattern)
{
	return stsfsm::GetId( Pattern, AccidentalAutomat_, a_Undefined, a_amount );
}

static mthrtn::dRational &GetFraction_(
	bso::u8__ Base,
	bso::u8__ DotAmount,
	sTuplet Tuplet,
	mthrtn::dRational &Result )
{
qRH
	mthrtn::wRational Dot, Buffer;
qRB
	if ( Base >= 1 )
		Result.Init( 1, 1 << ( Base - 1 ) );
	else if ( Base < 1 )
		Result.Init( 1 << Base );
	else
		qRFwk();

	Dot.Init();
	Buffer.Init();

	Dot = Result;

	while ( DotAmount-- ) {
		Buffer = Dot;
		mthrtn::Div( Buffer, mthrtn::wRational( 2 ), Dot );

		Buffer = Result;
		mthrtn::Add( Buffer, Dot, Result );
	}

	if ( Tuplet.IsValid() ) {
		mthrtn::Div( Result, mthrtn::wRational( Tuplet.Denominator ), Buffer );
		mthrtn::Mul( Buffer, mthrtn::wRational( Tuplet.Numerator ), Result );
	}
qRR
qRT
qRE
	return Result;
}

static inline mthrtn::dRational &GetFraction_(
	const sDuration &Duration,
	mthrtn::dRational &Result )
{
	if ( Duration.Base == 0 )
		qRFwk();

	return GetFraction_( Duration.Base, Duration.Modifier, Duration.Tuplet, Result );
}

sDuration FindDuration_(
	mthrtn::dRational &Fraction,
	bso::bool__ AlwaysTied )
{
	sDuration Duration;
qRH
	bso::u8__ Base = 0;
	bso::u8__ Dot = 0;
	bso::bool__ Continue = true;
	mthrtn::wRational Candidate, Buffer;
qRB
	Base = 1;
	Dot = 2;

	while ( Continue ) {
		Candidate.Init();

		GetFraction_( Base, Dot, sTuplet(), Candidate );

		switch ( ( Fraction - Candidate ).GetSign() ) {
		case -1:
			switch ( Dot ) {
			case 2:
			case 1:
				Dot--;
				break;
			case 0:
				if ( Base == 7 ) {
					Duration = sDuration();
					Continue = false;
				} else {
					Base++;
					Dot = 2;
				}
				break;
			default:
				qRFwk();
				break;
			}
			break;
		case 0:
			Duration = sDuration( Base, Dot, AlwaysTied );
			Buffer.Init();
			Buffer = Fraction;
			mthrtn::Sub( Buffer, Candidate, Fraction );
			Continue = false;
			break;
		case 1:
			Duration = sDuration( Base, Dot, true );
			Buffer.Init();
			Buffer = Fraction;
			mthrtn::Sub( Buffer, Candidate, Fraction );
			Continue = false;
			break;
		default:
			qRFwk();
			break;
		}
	}
qRR
qRT
	if ( Fraction.GetSign() < 0 )
		qRFwk();
qRE
	return Duration;
}


static void HandleOverflow_(
	mthrtn::dRational &Note,
	const sSignatureTime &Time,
	const sPitch &Pitch,
	const sSignature &Signature,
	mthrtn::dRational &Bar,
	dMelody &Melody )
{
qRH
	mthrtn::wRational Buffer;
	sDuration Duration;
qRB
	Buffer.Init();

	Buffer = Note;

	mthrtn::Sub( Buffer, Bar, Note );

	if ( Note.GetSign() != 1 )
		qRFwk();

	while ( Bar.GetSign() != 0 ) {
		Duration = FindDuration_( Bar, true );

		if ( !Duration.IsValid() )
			qRLmt();

		Melody.Append( sNote( Pitch, Duration, Signature ) );
	}

	Bar.Init( Time.Numerator(), Time.Denominator() );

	Buffer.Init();

	Buffer = Bar;

	mthrtn::Sub( Buffer, Note, Bar );

	if ( Bar.GetSign() != 1 )
		qRLmt();

	while ( Note.GetSign() != 0 ) {
		Duration = FindDuration_( Note, false );

		if ( !Duration.IsValid() )
			qRLmt();

		Melody.Append( sNote( Pitch, Duration, Signature ) );
	}

qRR
qRT
qRE
}

void mscmld::SplitToMatchBars(
	const dMelody &Source,
	dMelody &Target )
{
qRH
	sRow Row = qNIL;
	mthrtn::wRational Bar, Note, Buffer;
	sSignatureTime Time;
qRB
	Row = Source.First();

	while ( Row != qNIL ) {
		if ( !Source( Row ).IsValid() )
			qRFwk();

		if ( Time != Source( Row ).Signature.Time ) {
			Time = Source( Row ).Signature.Time;
			Bar.Init( Time.Numerator(), Time.Denominator() );
		}

		Note.Init();

		GetFraction_( Source( Row ).Duration, Note );

		switch ( ( Bar - Note ).GetSign() ) {
		case 1:
			Buffer.Init();
			Buffer = Bar;
			mthrtn::Sub( Buffer, Note, Bar );

			Target.Append( Source( Row ) );
			break;
		case 0:
			Bar.Init( Time.Numerator(), Time.Denominator() );
			Target.Append( Source( Row ) );
			break;
		case -1:
			HandleOverflow_( Note, Time, Source( Row ).Pitch, Source( Row ).Signature, Bar, Target );
			break;
		}

		Row = Source.Next( Row );
	}
qRR
qRT
qRE
}

void mscmld::UpdateTimeSignature(
  const sSignatureTime &Signature,
  dMelody &Melody,
  sRow First,
  sRow Last)
{
    sRow &Row = First;
    sNote Note;

    if ( Row == qNIL )
      Row = Melody.First();

    if ( Last != qNIL ) {
      if ( !Melody.Exists(Last) )
        qRFwk();
      Last = Melody.Next(Last);
    }

    while ( Row != Last ) {
      Note = Melody(Row);

      Note.Signature.Time = Signature;

      Melody.Store(Note, Row);

      Row = Melody.Next(Row);
    }
}


static bso::u8__ GetChromaticAbsolute_( const sAltPitch &Pitch )
{
	if ( ( BSO_U8_MAX / 12 ) < Pitch.Octave )
		qRFwk();

	bso::u8__ Absolute = Pitch.Octave * 12;

	switch ( Pitch.Name ) {
	case pnA:
		Absolute += 9;
		break;
	case pnB:
		Absolute += 11;
		break;
	case pnC:
		Absolute += 0;
		break;
	case pnD:
		Absolute += 2;
		break;
	case pnE:
		Absolute += 4;
		break;
	case pnF:
		Absolute += 5;
		break;
	case pnG:
		Absolute += 7;
		break;
	case pnRest:
		break;
	default:
		qRFwk();
		break;
	}

	switch ( Pitch.Accidental ) {
		case aFlat:
			if ( Absolute == 0 )
				qRFwk();
			Absolute--;
			break;
		case aNatural:
			break;
		case aSharp:
			if ( Absolute == BSO_U8_MAX )
				qRFwk();
			Absolute++;
			break;
		default:
			qRFwk();
			break;
	}

	return Absolute;
}

bso::u8__ mscmld::sAltPitch::GetChromatic( void ) const
{
	return GetChromaticAbsolute_( *this );
}

static bso::u8__ GetDiatonicAbsolute_( const sAltPitch &Pitch )
{
	if ( ( BSO_U8_MAX / 7 ) < Pitch.Octave )
		qRFwk();

	bso::u8__ Absolute = Pitch.Octave * 7;

	switch ( Pitch.Name ) {
	case pnA:
		Absolute += 5;
		break;
	case pnB:
		Absolute += 6;
		break;
	case pnC:
		Absolute += 0;
		break;
	case pnD:
		Absolute += 1;
		break;
	case pnE:
		Absolute += 2;
		break;
	case pnF:
		Absolute += 3;
		break;
	case pnG:
		Absolute += 4;
		break;
	case pnRest:
		break;
	default:
		qRFwk();
		break;
	}

	return Absolute;
}

const sAltPitch &mscmld::Convert(
  sPitch Pitch,
  sSignatureKey Key,
  eAccidental Accidental,  // Accidental to use if key = 0 (C key).
  sAltPitch &AltPitch)
{
  if ( Pitch == pRest ) {
    AltPitch.Name = pnRest;
    AltPitch.Accidental = a_Undefined;
    AltPitch.Octave = 0;
  } else if ( Pitch != p_Undefined ) {
    bso::sU8 RelChromatic = Pitch % 12;

    if ( Key != 0 )
      if ( Key > 0 )
        Accidental = aSharp;
      else
        Accidental = aFlat;
    else if ( ( Accidental != aSharp ) && ( Accidental != aFlat ) )
      qRFwk();

    // In this context 'key' can not be 0 (C key).
    // It should be instead -8 or 8 to indicate which of flat or sharp should be
    // used for alterated note.

    switch ( RelChromatic ) {
    case 0:
      AltPitch.Name = pnC;
      Accidental = aNatural;
      break;
    case 1:
      AltPitch.Name = Accidental == aSharp ? pnC : pnD;
      break;
    case 2:
      AltPitch.Name = pnD;
      Accidental = aNatural;
      break;
    case 3:
      AltPitch.Name = Accidental == aSharp ? pnD : pnE;
      break;
    case 4:
      AltPitch.Name = pnE;
      Accidental = aNatural;
      break;
    case 5:
      AltPitch.Name = pnF;
      Accidental = aNatural;
      break;
    case 6:
      AltPitch.Name = Accidental == aSharp ? pnF : pnG;
      break;
    case 7:
      AltPitch.Name = pnG;
      Accidental = aNatural;
      break;
    case 8:
      AltPitch.Name = Accidental == aSharp ? pnG : pnA;
      break;
    case 9:
      AltPitch.Name = pnA;
      Accidental = aNatural;
      break;
    case 10:
      AltPitch.Name = Accidental == aSharp ? pnA : pnB;
      break;
    case 11:
      AltPitch.Name = pnB;
      Accidental = aNatural;
      break;
    default:
      qRFwk();
      break;
    }

    AltPitch.Octave = Pitch / 12;
    AltPitch.Accidental = Accidental;

    if ( GetChromaticAbsolute_(AltPitch) != Pitch)
      qRFwk();
  }

  return AltPitch;
}

void mscmld::Merge(
	const dPitches &Pitches,
	const dDurations &Durations,
	const dSignatures &Signatures,
	dNotes &Notes )
{
	sPRow PRow = Pitches.First();
	sDRow DRow = Durations.First();
	sSRow SRow = Signatures.First();

	while ( ( PRow != qNIL ) && ( DRow != qNIL ) && ( SRow != qNIL ) ) {
		Notes.Append( sNote( Pitches( PRow ), Durations( DRow ), Signatures( SRow ) ) );

		PRow = Pitches.Next( PRow );
		DRow = Durations.Next( DRow );
		SRow = Signatures.Next( SRow );
	}
}

static bso::bool__ Adjust_(
	mthrtn::dRational &BarFraction,
	const mthrtn::dRational &NoteFraction,
	bso::bool__ *Error )
{
	bso::bool__ BarComplete = false;
qRH
	mthrtn::wRational Buffer;
qRB
#ifdef DEBUG
	cio::cout << BarFraction << txf::tab << NoteFraction << txf::tab << txf::sync;
#endif
	if ( !*Error ) {
        switch ( ( BarFraction - NoteFraction ).GetSign() ) {
        case -1 :
            *Error = true;
            break;
        case 0:
            BarComplete = true;
            break;
        case 1:
            Buffer.Init();
            Buffer = BarFraction;
            mthrtn::Sub( Buffer, NoteFraction, BarFraction );
            break;
        default:
            qRFwk();
            break;
        }
#ifdef DEBUG
        cio::cout << BarFraction << txf::nl;
#endif
	}

qRR
qRT
qRE
	return BarComplete;
}

#define C( n )\
		case 1 << n:\
			Anacrousis.Base = n+1;\
			break

static void Convert_(
	const mthrtn::dRational &RawDuration,
	sAnacrousis &Anacrousis )
{
qRH
	mthrtn::wRational Duration;
qRB
	Duration.Init( RawDuration );

	Duration.Simplify();

	switch ( Duration.D.GetU32() ) {
  C( 0 );
  C( 1 );
  C( 2 );
  C( 3 );
  C( 4 );
  C( 5 );
  C( 6 );
  C( 7 );
	default:
		qRFwk();
		break;
	}

	if ( Duration.N.GetU32() >= bso::U8Max )
		qRFwk();

	Anacrousis.Amount = (bso::u8__)Duration.N.GetU32();
qRR
qRT
qRE
}

bso::bool__ mscmld::dMelody::MarkAsAnacrousis( qRPN )
{
	bso::bool__ BarIsComplete = false;
qRH
	mthrtn::wRational Total, Remaining, Note;
	bso::bool__ BarHandlingError = false;
	sRow Row = qNIL;
qRB
	if ( IsEmpty() )
		qRFwk();

	Row = First();

	Total.Init( Get( Row ).Signature.Time.Numerator(), Get( Row ).Signature.Time.Denominator() );

	Remaining.Init( Total );

	while ( ( Row != qNIL ) && !BarIsComplete ) {
        Note.Init();
        GetFraction_( Get( Row ).Duration, Note );

        BarIsComplete = ::Adjust_( Remaining, Note, &BarHandlingError );

        Row = Next( Row );
	}

	if ( BarIsComplete ) {
        if ( qRP == err::hThrowException )
            qRFwk();
	} else {
		Note.Init();
		Note = Total - Remaining;

		Note.Simplify();

		Convert_( Note, S_.Anacrousis );
	}
qRR
qRT
qRE
	return !BarIsComplete;
}

static void WriteXMLAltPitch_(
	const sAltPitch &Pitch,
	const sAltPitch &PreviousPitch,
	xml::rWriter &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PushTag( PITCH_TAG );

	Writer.PutAttribute( NAME_ATTRIBUTE, GetLabel( Pitch.Name ) );

	Writer.PutAttribute( ACCIDENTAL_ATTRIBUTE, GetLabel( Pitch.Accidental ) );

	Writer.PutAttribute( OCTAVE_ATTRIBUTE, bso::Convert( Pitch.Octave, Buffer ) );

	Writer.PutAttribute( DiatonicAttributeLabel_, bso::Convert( GetDiatonicAbsolute_( Pitch ), Buffer ) );

	Writer.PutAttribute( ChromaticAttributeLabel_, bso::Convert( GetChromaticAbsolute_( Pitch ), Buffer ) );

	if ( PreviousPitch.Name != pn_Undefined ) {
		bso::s16__ DiatonicDelta = (bso::s16__)GetDiatonicAbsolute_( Pitch ) - GetDiatonicAbsolute_( PreviousPitch );
		bso::s16__ ChromaticDelta = (bso::s16__)GetChromaticAbsolute_( Pitch ) - GetChromaticAbsolute_( PreviousPitch );

		Writer.PushTag( DIFF_TAG );

		Writer.PutAttribute( DiatonicAttributeLabel_, bso::Convert( DiatonicDelta, Buffer ) );
		Writer.PutAttribute( ChromaticAttributeLabel_, bso::Convert( ChromaticDelta, Buffer ) );

		Writer.PopTag();
	}

	Writer.PopTag();
}

const char *mscmld::GetLabel(
    sPitch Pitch,
    sSignatureKey Key,
    eAccidental Accidental)
{
  sAltPitch AltPitch;

  AltPitch.Init();

  return GetLabel(Convert(Pitch, Key, Accidental, AltPitch).Name);
}

sOctave mscmld::GetOctave(
    sPitch Pitch,
    sSignatureKey Key,
    eAccidental Accidental)
{
  sAltPitch AltPitch;

  AltPitch.Init();

  return Convert(Pitch, Key, Accidental, AltPitch).Octave;
}

static void WriteXMLPitch_(
  sPitch Pitch,
  sPitch PreviousPitch,
  sSignatureKey Key,
  eAccidental Accidental,
  xml::rWriter &Writer)
{
  sAltPitch AltPitch, PreviousAltPitch;

  tol::Init(AltPitch, PreviousAltPitch);

  Convert(Pitch, Key, Accidental, AltPitch);
  Convert(PreviousPitch, Key, Accidental, PreviousAltPitch);

  WriteXMLAltPitch_(AltPitch, PreviousAltPitch, Writer);
}

static void WriteXMLDurationCoreAttributes_(
	const sDuration &Duration,
	xml::rWriter &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PutAttribute( BASE_TAG, bso::Convert( Duration.Base, Buffer ) );

	if ( Duration.Modifier != 0 )
		Writer.PutAttribute( "Modifier", bso::Convert( Duration.Modifier, Buffer ) );
}

static void WriteXMLDuration_(
	const sDuration &Duration,
	const sDuration &PreviousDuration,
	xml::rWriter &Writer )
{
	Writer.PushTag( DURATION_TAG );

	WriteXMLDurationCoreAttributes_( Duration, Writer );

	if ( PreviousDuration.Base != 0 )
		Writer.PutAttribute( "TiedToPrevious", ( PreviousDuration.TiedToNext ? "yes" : "no" ) );

	Writer.PutAttribute( TIED_FLAG_ATTRIBUTE, ( Duration.TiedToNext ? "yes" : "no" ) );

	Writer.PutAttribute( DIFF_ATTRIBUTE, ( ( Duration != PreviousDuration ) ? "yes" : "no" ) );

	Writer.PopTag();

}

static void WriteXMLSignatureKey_(
	const sSignatureKey &Key,
	const sSignatureKey &PreviousKey,
	xml::rWriter &Writer )
{
	ePitchName Name = pn_Undefined;
	eAccidental Accidental = a_Undefined;
	bso::integer_buffer__ Buffer;

	Writer.PushTag( KEY_TAG );

	switch ( Key ) {
	case -7:
		Name = pnC;
		Accidental = aFlat;
		break;
	case -6:
		Name = pnG;
		Accidental = aFlat;
		break;
	case -5:
		Name = pnD;
		Accidental = aFlat;
		break;
	case -4:
		Name = pnA;
		Accidental = aFlat;
		break;
	case -3:
		Name = pnE;
		Accidental = aFlat;
		break;
	case -2:
		Name = pnB;
		Accidental = aFlat;
		break;
	case -1:
		Name = pnF;
		Accidental = aNatural;
		break;
	case 0:
		Name = pnC;
		Accidental = aNatural;
		break;
	case 1:
		Name = pnG;
		Accidental = aNatural;
		break;
	case 2:
		Name = pnD;
		Accidental = aNatural;
		break;
	case 3:
		Name = pnA;
		Accidental = aNatural;
		break;
	case 4:
		Name = pnE;
		Accidental = aNatural;
		break;
	case 5:
		Name = pnB;
		Accidental = aNatural;
		break;
	case 6:
		Name = pnF;
		Accidental = aSharp;
		break;
	case 7:
		Name = pnC;
		Accidental = aSharp;
		break;
	default:
		qRFwk();
		break;
	}

	Writer.PutAttribute( NAME_ATTRIBUTE, GetLabel( Name ) );

	Writer.PutAttribute( ACCIDENTAL_ATTRIBUTE, GetLabel( Accidental ) );

	Writer.PutAttribute( RAW_ATTRIBUTE, bso::Convert( Key, Buffer ) );

	Writer.PutAttribute( DIFF_ATTRIBUTE, ( Key != PreviousKey ) ? "yes" : "no" );

	Writer.PopTag();
}

static void WriteXMLSignatureTime_(
	const sSignatureTime &Time,
	const sSignatureTime &PreviousTime,
	xml::rWriter &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PushTag( TIME_TAG );

	Writer.PutAttribute( NUMERATOR_ATTRIBUTE, bso::Convert( Time.Numerator(), Buffer ) );

	Writer.PutAttribute( RAW_NUMERATOR_ATTRIBUTE, bso::Convert( Time.RawNumerator(), Buffer ) );

	Writer.PutAttribute( DENOMINATOR_ATTRIBUTE, bso::Convert( Time.Denominator(), Buffer ) );

	Writer.PutAttribute( DENOMINATOR_POWER_ATTRIBUTE, bso::Convert( Time.DenominatorPower(), Buffer ) );

	if ( Time.IsDenHidden() )
		Writer.PutAttribute( HIDDEN_DENOMINATOR_FLAG_ATTRIBUTE, YES_VALUE );

	Writer.PutAttribute( DIFF_ATTRIBUTE, ( Time != PreviousTime ) ? YES_VALUE : NO_VALUE );

	Writer.PopTag();
}

static void WriteXMLSignature_(
	const sSignature &Signature,
	const sSignature &PreviousSignature,
	xml::rWriter &Writer )
{
	Writer.PushTag( SIGNATURE_TAG );

//	Writer.PutAttribute( "Diff", ( Signature != PreviousSignature ) ? "yes" : "no" );

	WriteXMLSignatureKey_( Signature.Key, PreviousSignature.Key, Writer );

	WriteXMLSignatureTime_( Signature.Time, PreviousSignature.Time, Writer );

	Writer.PopTag();
}

static void WriteXMLNote_(
	const sNote &Note,
	const sNote &PreviousNote,
	eAccidental Accidental,
	xml::rWriter &Writer )
{
	if ( Note.Pitch == pRest )
		Writer.PushTag( REST_TAG );
	else {
		Writer.PushTag( NOTE_TAG );

		WriteXMLPitch_( Note.Pitch, PreviousNote.Pitch, Note.Signature.Key, Accidental, Writer );
	}

	WriteXMLDuration_( Note.Duration, PreviousNote.Duration, Writer );

	Writer.PopTag();
}


write_status__ mscmld::WriteXML(
	const dMelody &Melody,
	eAccidental Accidental,
	xml::rWriter &Writer )
{
	write_status__ Status = wsOK;
qRH
	sRow Row = qNIL;
	mthrtn::wRational BarFraction, NoteFraction, Buffer;
	sNote PreviousNote, Note;
	bso::bool__ BarClosed = true;
	bso::bool__ BarHandlingError = false;
	bso::bool__ HandleAnacrousis = false;
	xml::sMark TupletControl;
	xml::sMark Mark =xml::Undefined;
	bso::bool__ WriteBar = false;
	bso::bool__ WriteSignature = false;
qRB
	Mark = Writer.GetMark();

	Row = Melody.First();

	if ( ( Melody.Anacrousis().IsValid() ) && (Melody.Anacrousis().Amount != 0 ) ) {
		Writer.PushTag( ANACROUSIS_TAG );
		xml::PutAttribute( AnacrousisBaseAttributeLabel_, Melody.Anacrousis().Base, Writer );
		xml::PutAttribute( AnacrousisAmountAttributeLabel_, Melody.Anacrousis().Amount, Writer );
		Writer.PopTag();
		HandleAnacrousis = true;
	}

	while ( Row != qNIL ) {
		if ( !Melody( Row ).IsValid() )
			qRFwk();

		if ( Note.IsValid() ) {
			if ( Note.Pitch != pRest ) {
				PreviousNote = Note;
			} else {
				PreviousNote.Duration = Note.Duration;
				PreviousNote.Signature = Note.Signature;
			}
		}

		Note = Melody( Row );

		if ( Note.Signature != PreviousNote.Signature ) {
			if ( !BarClosed && ( PreviousNote.IsValid() ) )
				Status = wsBarChekError;
			else
				WriteSignature = true;
		}

		if ( Status == wsOK ) {
            if ( BarClosed ) {
                if ( PreviousNote.IsValid() ) // Si ce n'est pas le cas, c'est que la premire note n'a pas encore t trait, et on vite ainsi une 'bar' avant elle.
                    WriteBar = true;

                BarFraction.Init( Note.Signature.Time.Numerator(), Note.Signature.Time.Denominator() );

                if ( HandleAnacrousis ) {
                    Buffer.Init( BarFraction );

                    if ( !Melody.Anacrousis().IsValid() )
                        qRFwk();

                    NoteFraction.Init( Melody.Anacrousis().Amount, 1 << ( Melody.Anacrousis().Base - 1 ) );
                    Adjust_( Buffer, NoteFraction, &BarHandlingError );
                    Adjust_( BarFraction, Buffer, &BarHandlingError );
                    HandleAnacrousis = false;
                }
            }

            if ( Note.Duration.Tuplet.IsValid() ) {
                if ( Note.Duration.Tuplet != PreviousNote.Duration.Tuplet ) {
                    if ( PreviousNote.Duration.Tuplet.IsValid() )
                        Writer.PopTag( TupletControl );

                    if ( WriteBar ){
                        Writer.PushTag( BAR_TAG );
                        Writer.PopTag();
                    }

                    if ( WriteSignature )
                        WriteXMLSignature_( Note.Signature, PreviousNote.Signature, Writer );

                    TupletControl = Writer.PushTag( TUPLET_TAG );
                    xml::PutAttribute( NUMERATOR_ATTRIBUTE, Note.Duration.Tuplet.Numerator, Writer );
                    xml::PutAttribute( DENOMINATOR_ATTRIBUTE, Note.Duration.Tuplet.Denominator, Writer );
                }
            } else if ( PreviousNote.Duration.Tuplet.IsValid() ) {
                Writer.PopTag( TupletControl );

                if ( WriteBar ) {
                    Writer.PushTag( BAR_TAG );
                    Writer.PopTag();
                }

                if ( WriteSignature )
                    WriteXMLSignature_( Note.Signature, PreviousNote.Signature, Writer );

            } else {
                if ( WriteBar ) {
                    Writer.PushTag( BAR_TAG );
                    Writer.PopTag();
                }

                if ( WriteSignature )
                    WriteXMLSignature_( Note.Signature, PreviousNote.Signature, Writer );
            }

            WriteBar = false;
            WriteSignature = false;

            WriteXMLNote_( Note, PreviousNote, Accidental, Writer );

            NoteFraction.Init();

            GetFraction_( Note.Duration, NoteFraction );

            BarClosed = Adjust_( BarFraction, NoteFraction, &BarHandlingError );

            Row = Melody.Next( Row );
        }

        if ( Note.Duration.Tuplet.IsValid() )
            Writer.PopTag( TupletControl );
	}
qRR
qRT
	if ( Mark != xml::Undefined )
		if ( Mark != Writer.GetMark() )
			Writer.Rewind( Mark );
qRE
	return Status;
}

static parse_status__ GetKeyRaw_(
	const str::string_ &Raw,
	sSignatureKey &Key )
{
	sdr::row__ Error = qNIL;

	Key = Raw.ToS8( &Error );

	if ( Error != qNIL )
		return psBadValue;
	else if ( Key > 7 )
		return psBadValue;
	else if ( Key < -7 )
		return psBadValue;
	else
		return psOK;
}

static parse_status__ ParseKey_(
	xml::parser___ &Parser,
	sSignatureKey &Key )
{
	parse_status__ Status = psOK;
	bso::bool__ Continue = true;

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious )  ){
		case xml::tStartTag:
			Status = psUnexpectedTag;
			break;
		case xml::tAttribute:
			if ( Parser.AttributeName() == RAW_ATTRIBUTE ) {
				if ( IsValid( Key ) ) {
					Status = psAlreadyDefined;
				} else {
					Status = GetKeyRaw_( Parser.Value(), Key );
				}
			}
			break;
		case xml::tValue:
			Status = psUnexpectedValue;
			break;
		case xml::tEndTag:
			if ( Parser.TagName() != KEY_TAG )
				qRFwk();
			Continue = false;
			break;
		default:
			qRFwk();
			break;
		}

		if ( Status != psOK )
			Continue = false;
	}

	return Status;
}

#if 0    // Obsolete ?
static parse_status__ GetU8(
	const str::string_ &Value,
	bso::u8__ &U8 )
{
	sdr::row__ Error qNIL;

	U8 = Value.ToU8( &Error );

	if ( Error != qNIL )
		return psBadValue;

	return psOK;
}
#endif

static parse_status__ ParseTime_(
	xml::parser___ &Parser,
	sSignatureTime &Time )
{
	parse_status__ Status = psOK;
	bso::bool__ Continue = true;
	bso::s8__ RawNumerator = MSCMLD_UNDEFINED_TIME_SIGNATURE_NUMERATOR;
	bso::u8__ DenominatorPower = MSCMLD_UNDEFINED_TIME_SIGNATURE_DENOMINATOR_POWER;
	sdr::row__ Error = qNIL;

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			Status = psUnexpectedTag;
			break;
		case xml::tAttribute:
			if ( Parser.AttributeName() == RAW_NUMERATOR_ATTRIBUTE ) {
				if ( RawNumerator != MSCMLD_UNDEFINED_TIME_SIGNATURE_NUMERATOR )
					Status = psAlreadyDefined;

				RawNumerator = Parser.Value().ToS8( &Error );

				if ( Error != qNIL )
					Status = psBadValue;
			} else if ( Parser.AttributeName() == DENOMINATOR_POWER_ATTRIBUTE ) {
				if ( DenominatorPower != MSCMLD_UNDEFINED_TIME_SIGNATURE_DENOMINATOR_POWER )
					Status = psAlreadyDefined;

				DenominatorPower = Parser.Value().ToU8();

				if ( Error != qNIL )
					Status = psBadValue;
			}
			break;
		case xml::tValue:
			Status = psUnexpectedValue;
			break;
		case xml::tEndTag:
			if ( Parser.TagName() != TIME_TAG )
				qRFwk();

			if ( RawNumerator == MSCMLD_UNDEFINED_TIME_SIGNATURE_NUMERATOR )
				Status = psMissingSignatureTimeRawNumerator;
			else if ( DenominatorPower == MSCMLD_UNDEFINED_TIME_SIGNATURE_DENOMINATOR_POWER )
				Status = psMissingSignatureTimeDenominatorPower;
			else
				Time.SetRaw( RawNumerator, DenominatorPower );

			Continue = false;
			break;
		default:
			qRFwk();
		}

		if ( Status != psOK )
			Continue = false;
	}

	return Status;
}

static parse_status__ ParseSignature_(
	xml::parser___ &Parser,
	sSignature &Signature )
{
	parse_status__ Status = psOK;
qRH
	sSignatureKey Key = MSCMLD_UNDEFINED_KEY_SIGNATURE;
	sSignatureTime Time;
	bso::bool__ Continue = true;
qRB
	Initialize( Key );
	Time.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == KEY_TAG )
				if ( IsValid( Key ) )
					Status = psAlreadyDefined;
				else
					Status = ParseKey_( Parser, Key );
			else if ( Parser.TagName() == TIME_TAG )
				if ( Time.IsValid() )
					Status = psAlreadyDefined;
				else
					Status = ParseTime_( Parser, Time );
			else
				Status = psUnexpectedTag;
			break;
		case xml::tAttribute:
			Status = psUnexpectedAttribute;
			break;
		case xml::tValue:
			Status = psUnexpectedValue;
			break;
		case xml::tEndTag:
			if ( Parser.TagName() != SIGNATURE_TAG )
				qRFwk();

			if ( !IsValid( Key ) )
				Status = psMissingSignatureKey;

			if ( !Time.IsValid() )
				Status = psMissingSignatureTime;

			Signature.Time = Time;
			Signature.Key = Key;

			Continue = false;

			break;
		default:
			qRFwk();
			break;
		}
	}
qRR
qRT
qRE
	return Status;
}

static ePitchName GetPitchName_( const str::string_ &Name )
{
	int i = 0;

	while ( ( i < pn_amount ) && ( Name != GetLabel( (ePitchName)i ) ) )
		i++;

	if ( i > pn_amount )
		i = pn_Undefined;

	return (ePitchName)i;
}

static eAccidental GetPitchAccidental_(const str::string_ &Accidental)
{
	int i = 0;

	while ( ( i < a_amount ) && ( Accidental != GetLabel( (eAccidental)i ) ) )
		i++;

	if ( i > a_amount )
		i = a_Undefined;

	return (eAccidental)i;
}

static sOctave GetPitchOctave_( const str::string_ &Octave )
{
	sdr::row__ Error = qNIL;
	sOctave O = Octave.ToU8( &Error );

	if ( Error != qNIL )
		O = MSCMLD_UNDEFINED_PITCH_OCTAVE;

	return O;
}

static parse_status__ ParsePitch_(
	xml::parser___ &Parser,
	sAltPitch &Pitch )
{
	parse_status__ Status = psOK;
	bso::bool__ Continue = true;
	ePitchName Name = pn_Undefined;
	eAccidental Accidental = a_Undefined;
	sOctave Octave = MSCMLD_UNDEFINED_PITCH_OCTAVE;

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == DIFF_TAG )
				Parser.Skip();
			else
				Status = psUnexpectedTag;
			break;
		case xml::tAttribute:
			if ( Parser.AttributeName() == NAME_ATTRIBUTE ) {
				if ( Name != pn_Undefined )
					Status = psAlreadyDefined;
				else {
					Name = GetPitchName_( Parser.Value() );

					if ( Name == pn_Undefined )
						Status = psBadValue;
				}
			} else if ( Parser.AttributeName() == ACCIDENTAL_ATTRIBUTE ) {
				if ( Accidental != a_Undefined )
					Status = psAlreadyDefined;
				else {
					Accidental = GetPitchAccidental_( Parser.Value() );

					if ( Accidental == a_Undefined )
						Status = psBadValue;
				}
			} else if ( Parser.AttributeName() == OCTAVE_ATTRIBUTE ) {
				if ( Octave != MSCMLD_UNDEFINED_PITCH_OCTAVE )
					Status = psAlreadyDefined;
				else {
					Octave = GetPitchOctave_( Parser.Value() );

					if ( Octave == MSCMLD_UNDEFINED_PITCH_OCTAVE )
						Status = psBadValue;
				}
			}
			break;
		case xml::tValue:
			Status = psUnexpectedValue;
			break;
		case xml::tEndTag:
			if ( Parser.TagName() != PITCH_TAG )
				qRFwk();

			if ( Name == pn_Undefined )
				Status = psMissingPitchName;
			else if ( Accidental == a_Undefined )
				Status = psMissingPitchAccidental;
			else if ( Octave == MSCMLD_UNDEFINED_PITCH_OCTAVE )
				Status = psMissingPitchOctave;
			else {
				Pitch.Name = Name;
				Pitch.Accidental = Accidental;
				Pitch.Octave = Octave;
			}

			Continue = false;
			break;
		default:
            qRFwk();
            break;
		}

		if ( Status != psOK )
			Continue = false;
	}

	return Status;
}

static bso::s8__ GetDurationBase_( const str::string_ &Value )
{
	bso::s8__ Base = MSCMLD_UNDEFINED_DURATION_BASE;
	sdr::row__ Error = qNIL;

	Base = Value.ToS8( &Error );

	if ( Error != qNIL )
		Base = MSCMLD_UNDEFINED_DURATION_BASE;

	return Base;
}

static bso::u8__ GetDurationModifier_( const str::string_ &Value )
{
	bso::u8__ Modifier = MSCMLD_UNDEFINED_DURATION_MODIFIER;
	sdr::row__ Error = qNIL;

	Modifier = Value.ToU8( &Error );

	if ( Error != qNIL )
		Modifier = MSCMLD_UNDEFINED_DURATION_BASE;

	return Modifier;
}

tol::xbool__ GetTiedFlag_( const str::string_ &Value )
{
	if ( Value == NO_VALUE )
		return tol::xbFalse;
	else if ( Value == YES_VALUE )
		return tol::xbYes;
	else
		return tol::xb_Undefined;
}



static parse_status__ ParseDuration_(
	const char *Tag,
	xml::parser___ &Parser,
	const sTuplet &Tuplet,
	sDuration &Duration )
{
	parse_status__ Status = psOK;
	bso::bool__ Continue = true;
	bso::s8__ Base = MSCMLD_UNDEFINED_DURATION_BASE;
	bso::u8__ Modifier = MSCMLD_UNDEFINED_DURATION_MODIFIER;
	tol::xbool__ TiedToNext = tol::xb_Undefined;

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			Status = psUnexpectedTag;
			break;
		case xml::tAttribute:
			if ( Parser.AttributeName() == BASE_TAG ) {
				if ( Base != MSCMLD_UNDEFINED_DURATION_BASE )
					Status = psAlreadyDefined;
				else {
					Base = GetDurationBase_( Parser.Value() );

					if ( Base == MSCMLD_UNDEFINED_DURATION_BASE )
						Status = psBadValue;
				}
			} else if ( Parser.AttributeName() == MODIFIER_TAG ) {
				if ( Modifier != MSCMLD_UNDEFINED_DURATION_MODIFIER )
					Status = psAlreadyDefined;
				else {
					Modifier = GetDurationModifier_( Parser.Value() );

					if ( Modifier == MSCMLD_UNDEFINED_DURATION_MODIFIER )
						Status = psBadValue;
				}
			} else if ( Parser.AttributeName() == TIED_FLAG_ATTRIBUTE ) {
				if( TiedToNext != tol::xb_Undefined )
					Status = psAlreadyDefined;
				else {
					TiedToNext = GetTiedFlag_( Parser.Value() );

					if ( TiedToNext == tol::xb_Undefined )
						Status = psBadValue;
				}
			}
			break;
		case xml::tValue:
			Status = psUnexpectedValue;
			break;
		case xml::tEndTag:
			if ( Parser.TagName() != Tag )
				qRFwk();

			if ( Base == MSCMLD_UNDEFINED_DURATION_BASE )
				Status = psMissingDurationBase;
			else {
				if ( Modifier == MSCMLD_UNDEFINED_DURATION_MODIFIER )
					Modifier = 0;

				if ( TiedToNext == tol::xb_Undefined )
					TiedToNext = tol::xbFalse;

				if ( Tuplet.IsValid() )
					Duration.Init( Base, Modifier, Tuplet, TiedToNext );
				else
					Duration.Init( Base, Modifier, TiedToNext );
			}

			Continue = false;
			break;
		default:
			qRFwk();
			break;
		}

		if ( Status != psOK )
			Continue = false;
	}

	return Status;
}

static parse_status__ ParseNote_(
	xml::parser___ &Parser,
	const sSignature &Signature,
	const sTuplet &Tuplet,
	sNote &Note )
{
	parse_status__ Status = psOK;
	bso::bool__ Continue = true;
	sAltPitch Pitch;
	sDuration Duration;

	Pitch.Init();
	Duration.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == PITCH_TAG ) {
				if ( Pitch.IsValid() )
					Status = psAlreadyDefined;
				Status = ParsePitch_( Parser, Pitch );
			} else if ( Parser.TagName() == DURATION_TAG ) {
				if ( Duration.IsValid() )
					Status = psAlreadyDefined;
				Status = ParseDuration_( DURATION_TAG, Parser, Tuplet, Duration );
			} else
				Status = psUnexpectedTag;
		break;
		case xml::tAttribute:
			Status = psUnexpectedAttribute;
			break;
		case xml::tValue:
			Status = psUnexpectedValue;
			break;
		case xml::tEndTag:
			if ( Parser.TagName() != NOTE_TAG )
				qRFwk();

			if ( !Pitch.IsValid() )
				Status = psMissingPitch;
			else if ( !Duration.IsValid() )
				Status = psMissingDuration;
			else if ( !Signature.IsValid() )
				Status = psMissingSignature;
			else {
				Note.Pitch = GetChromaticAbsolute_(Pitch);
				Note.Duration = Duration;
				Note.Signature = Signature;
			}

			Continue = false;
			break;
		default:
			qRFwk();
			break;
		}

		if  ( Status != psOK )
			Continue = false;
	}

	return Status;
}

static parse_status__ ParseBar_( xml::parser___ &Parser )
{
	Parser.Skip();

	return psOK;
}

static parse_status__ ParseRest_(
	xml::parser___ &Parser,
	const sSignature &Signature,
	const sTuplet &Tuplet,
	sNote &Note  )
{
	parse_status__ Status = psOK;
	bso::bool__ Continue = true;
	sAltPitch Pitch;
	sDuration Duration;

	Pitch.Init();
	Pitch.Name = pnRest;

	Duration.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == DURATION_TAG ) {
				if ( Duration.IsValid() )
					Status = psAlreadyDefined;
				Status = ParseDuration_( DURATION_TAG, Parser, Tuplet, Duration );
			} else
				Status = psUnexpectedTag;
		break;
		case xml::tAttribute:
			Status = psUnexpectedAttribute;
			break;
		case xml::tValue:
			Status = psUnexpectedValue;
			break;
		case xml::tEndTag:
			if ( Parser.TagName() != REST_TAG )
				qRFwk();

			if ( !Pitch.IsValid() )
				qRFwk();
			else if ( !Duration.IsValid() )
				Status = psMissingDuration;
			else if ( !Signature.IsValid() )
				Status = psMissingSignature;
			else {
				Note.Pitch = pRest;
				Note.Duration = Duration;
			}

			Continue = false;
			break;
		default:
			qRFwk();
			break;
		}

		if  ( Status != psOK )
			Continue = false;
	}

	return Status;
}

static bso::u8__ GetTupletNumerator_( const str::string_ &Value )
{
	bso::u8__ Numerator = MSCMLD_UNDEFINED_TUPLET_NUMERATOR;
	sdr::row__ Error = qNIL;

	Numerator = Value.ToU8( &Error );

	if ( Error != qNIL )
		Numerator = MSCMLD_UNDEFINED_TUPLET_NUMERATOR;

	return Numerator;
}

static bso::u8__ GetTupletDenominator_( const str::string_ &Value )
{
	bso::u8__ Denominator = MSCMLD_UNDEFINED_TUPLET_DENOMINATOR;
	sdr::row__ Error = qNIL;

	Denominator = Value.ToU8( &Error );

	if ( Error != qNIL )
		Denominator = MSCMLD_UNDEFINED_TUPLET_DENOMINATOR;

	return Denominator;
}

static parse_status__ ParseTuplet_(
	xml::parser___ &Parser,
	const sSignature &Signature,
	dMelody &Melody )
{
	parse_status__ Status = psOK;
	bso::bool__ Continue = true;
	bso::u8__
		Numerator = MSCMLD_UNDEFINED_TUPLET_NUMERATOR,
		Denominator = MSCMLD_UNDEFINED_TUPLET_DENOMINATOR;
	sTuplet Tuplet;
	sNote Note;

	Note.Init();

	Tuplet.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			if ( !Tuplet.IsValid() ) {
				if ( Numerator == MSCMLD_UNDEFINED_TUPLET_NUMERATOR )
					Status = psMissingTupletNumerator;
				else if ( Denominator == MSCMLD_UNDEFINED_TUPLET_DENOMINATOR )
					Status = psMissingTupletDenominator;
				else {
					Tuplet.Numerator = Numerator;
					Tuplet.Denominator = Denominator;
				}
			}

			if ( Status == psOK ) {

				if ( !Tuplet.IsValid() )
					qRFwk();

				if ( Parser.TagName() == NOTE_TAG )
					Status = ParseNote_( Parser, Signature, Tuplet, Note );
				else if ( Parser.TagName() == REST_TAG )
					Status = ParseRest_( Parser, Signature, Tuplet, Note );
				else
					Status = psUnexpectedTag;

				if ( Status == psOK )
					if ( Note.IsValid() ) {
						Melody.Append( Note );
						Note.Init();
					}
			}
			break;
		case xml::tAttribute:
			if ( Parser.AttributeName() == NUMERATOR_ATTRIBUTE ) {
				if ( Numerator != MSCMLD_UNDEFINED_TUPLET_NUMERATOR )
					Status = psAlreadyDefined;
				else
					Numerator = GetTupletNumerator_( Parser.Value() );

				if ( Numerator == MSCMLD_UNDEFINED_TUPLET_NUMERATOR )
					Status = psBadValue;
			} else if ( Parser.AttributeName() == DENOMINATOR_ATTRIBUTE ) {
				if ( Denominator != MSCMLD_UNDEFINED_TUPLET_DENOMINATOR )
					Status = psAlreadyDefined;
				else
					Denominator = GetTupletDenominator_( Parser.Value() );

				if ( Denominator == MSCMLD_UNDEFINED_TUPLET_DENOMINATOR )
						Status = psBadValue;
			}
			break;
		case xml::tValue:
			Status = psUnexpectedValue;
			break;
		case xml::tEndTag:
			if ( Parser.TagName() != TUPLET_TAG )
				qRFwk();

			Continue = false;
			break;
		default:
			qRFwk();
			break;
		}

		if ( Status != psOK )
			Continue = false;
	}

	return Status;
}

static parse_status__ ParseAnacrousis_(
	xml::parser___ &Parser,
	const sSignature &Signature,
	sAnacrousis &Anacrousis )
{
	parse_status__ Status = psOK;
	bso::bool__ Continue = true;
	bso::u8__
		Base = AnacrousisUndefinedBase,
		Amount = 0;
	sdr::row__ Error = qNIL;

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			Status = psUnexpectedTag;
			break;
		case xml::tAttribute:
			if ( Parser.AttributeName() == AnacrousisBaseAttributeLabel_ ) {
				if ( Base != AnacrousisUndefinedBase )
					Status = psAlreadyDefined;
				else
					Base = Parser.Value().ToU8( &Error );

				if ( (Error != qNIL) || ( Base == AnacrousisUndefinedBase ) )
					Status = psBadValue;
			} else if ( Parser.AttributeName() == AnacrousisAmountAttributeLabel_ ) {
				if ( Amount != 0 )
					Status = psAlreadyDefined;
				else
					Amount = Parser.Value().ToU8( &Error );

				if ( ( Error != qNIL ) || ( Amount == 0 ) )
					Status = psBadValue;
			} else
				Status = psUnexpectedAttribute;
		case xml::tValue:
			Status = psUnexpectedValue;
			break;
		case xml::tEndTag:
			if ( Parser.TagName() != ANACROUSIS_TAG )
				qRFwk();

			if ( Base == AnacrousisUndefinedBase )
				Status = psMissingAnacrousisBase;
			else if ( Amount == 0 )
				Status = psMissingAnacrousisAmount;
			else {
				Anacrousis.Base = Base;
				Anacrousis.Amount = Amount;
			}


			Continue = false;
			break;
		default:
			qRFwk();
			break;
		}

		if ( Status != psOK )
			Continue = false;
	}

	return Status;
}


static parse_status__ Parse_(
	xml::parser___ &Parser,
	dMelody &Melody )
{
	parse_status__ Status = psOK;
	bso::bool__ Continue = true;
	sSignature Signature;
	sNote Note;

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == SIGNATURE_TAG )
				Status = ParseSignature_( Parser, Signature );
			else if ( Parser.TagName() == NOTE_TAG )
				Status = ParseNote_( Parser, Signature, sTuplet(), Note );
			else if ( Parser.TagName() == REST_TAG )
				Status = ParseRest_( Parser, Signature, sTuplet(), Note );
			else if ( Parser.TagName() == BAR_TAG )
				Status = ParseBar_( Parser );
			else if ( Parser.TagName() == TUPLET_TAG )
				Status = ParseTuplet_( Parser, Signature, Melody );
			else if ( Parser.TagName() == ANACROUSIS_TAG )
				Status = ParseAnacrousis_( Parser, Signature, Melody.S_.Anacrousis );
			else
				Status = psUnexpectedTag;

			if ( Status == psOK )
				if ( Note.IsValid() ) {
					Melody.Append( Note );
					Note.Init();
				}
			break;
		case xml::tAttribute:
			Status = psUnexpectedAttribute;
			break;
		case xml::tValue:
			Status = psUnexpectedValue;
			break;
		case xml::tEndTag:
			if ( Parser.TagName() != dMelodyTAG )
				qRFwk();

			Continue = false;
			break;
		default:
			qRFwk();
			break;
		}

		if ( Status != psOK )
			Continue = false;
	}

	return Status;
}

parse_status__ mscmld::ParseXML(
	xml::parser___ &Parser,
	dMelody &Melody,
	bso::bool__ WithRoot )	// Si  'true', la prochaine balise est 'Melody', sinon, on est  l'intrieur de 'Melody'. Dans les deux cas, au retour on est  l'extrieur de la balise 'Melody'.
{
	if ( WithRoot)
		qRVct();

	return Parse_( Parser, Melody );
}

qGCTOR( mscmld ) {
	FillAccidentalAutomat_();
}
