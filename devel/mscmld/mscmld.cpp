/*
	'mscmld.cpp' by Claude SIMON (http://zeusw.org/).

	'mscmld' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#define MSCMLD__COMPILATION

#include "mscmld.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace mscmld;

#include "mthfrc.h"

#define MELODY_TAG							"Melody"


#define SIGNATURE_TAG						"Signature"

#define KEY_TAG								"Key"

#define TIME_TAG							"Time"

#define ANACROUSIS_TAG						"Anacrousis"
E_CDEF( char *, AnacrousisBaseAttribute_, "Base" );
E_CDEF( char *, AnacrousisAmountAttribute_, "Amount" );


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



const char *mscmld::GetPitchNameLabel( pitch_name__ Name )
{
	switch( Name ) {
	case pnA:
		return "a";
		break;
	case pnB:
		return "b";
		break;
	case pnC:
		return "c";
		break;
	case pnD:
		return "d";
		break;
	case pnE:
		return "e";
		break;
	case pnF:
		return "f";
		break;
	case pnG:
		return "g";
		break;
	case pnRest:
		return "r";
		break;
	default:
		ERRFwk();
		break;
	}

	return NULL;	// To avoir a 'warning'.
}

const char *mscmld::GetPitchAccidentalLabel( pitch_accidental__ Accidental )
{
	switch( Accidental ) {
	case paFlat:
		return "Flat";
		break;
	case paNatural:
		return "Natural";
		break;
	case paSharp:
		return "Sharp";
		break;
	default:
		ERRFwk();
		break;
	}

	return NULL;	// To avoir a 'warning'.
}

static mthfrc::fraction_ &GetFraction_(
	bso::u8__ Base,
	bso::u8__ DotAmount,
	tuplet__ Tuplet,
	mthfrc::fraction_ &Result )
{
ERRProlog
	mthfrc::fraction Dot, Buffer;
ERRBegin
	if ( Base >= 1 )
		Result.Init( 1, 1 << ( Base - 1 ) );
	else if ( Base < 1 )
		Result.Init( 1 << Base );
	else
		ERRFwk();

	Dot.Init();
	Buffer.Init();

	Dot = Result;

	while ( DotAmount-- ) {
		Buffer = Dot;
		mthfrc::Div( Buffer, mthfrc::fraction( 2 ), Dot );

		Buffer = Result;
		mthfrc::Add( Buffer, Dot, Result );
	}

	if ( Tuplet.IsValid() ) {
		mthfrc::Div( Result, mthfrc::fraction( Tuplet.Denominator ), Buffer );
		mthfrc::Mul( Buffer, mthfrc::fraction( Tuplet.Numerator ), Result );
	}
ERRErr
ERREnd
ERREpilog
	return Result;
}

static inline mthfrc::fraction_ &GetFraction_(
	const duration__ &Duration,
	mthfrc::fraction_ &Result )
{
	if ( Duration.Base == 0 )
		ERRFwk();

	return GetFraction_( Duration.Base, Duration.Modifier, Duration.Tuplet, Result );
}

duration__ FindDuration_(
	mthfrc::fraction_ &Fraction,
	bso::bool__ AlwaysTied )
{
	duration__ Duration;
ERRProlog
	bso::u8__ Base = 0;
	bso::u8__ Dot = 0;
	bso::bool__ Continue = true;
	mthfrc::fraction Candidate, Buffer;
ERRBegin
	Base = 1;
	Dot = 2;

	while ( 1 ) {
		Candidate.Init();

		GetFraction_( Base, Dot, tuplet__(), Candidate );

		switch ( ( Fraction - Candidate ).GetSign() ) {
		case -1:
			switch ( Dot ) {
			case 2:
			case 1:
				Dot--;
				break;
			case 0:
				if ( Base == 7 ) {
					Duration = duration__();
					ERRReturn;
				} else {
					Base++;
					Dot = 2;
				}
				break;
			default:
				ERRFwk();
				break;
			}
			break;
		case 0:
			Duration = duration__( Base, Dot, AlwaysTied );
			Buffer.Init();
			Buffer = Fraction;
			mthfrc::Sub( Buffer, Candidate, Fraction );
			ERRReturn;
			break;
		case 1:
			Duration = duration__( Base, Dot, true );
			Buffer.Init();
			Buffer = Fraction;
			mthfrc::Sub( Buffer, Candidate, Fraction );
			ERRReturn;
			break;
		default:
			ERRFwk();
			break;
		}
	}
ERRErr
ERREnd
	if ( Fraction.GetSign() < 0 )
		ERRFwk();
ERREpilog
	return Duration;
}


static void HandleOverflow_(
	mthfrc::fraction_ &Note,
	const signature_time__ &Time,
	const pitch__ &Pitch,
	const signature__ &Signature,
	mthfrc::fraction_ &Bar,
	melody_ &Melody )
{
ERRProlog
	mthfrc::fraction Buffer;
	duration__ Duration;
ERRBegin
	Buffer.Init();

	Buffer = Note;

	mthfrc::Sub( Buffer, Bar, Note );

	if ( Note.GetSign() != 1 )
		ERRFwk();

	while ( Bar.GetSign() != 0 ) {
		Duration = FindDuration_( Bar, true );

		if ( !Duration.IsValid() )
			ERRLmt();

		Melody.Append( note__( Pitch, Duration, Signature ) );
	}

	Bar.Init( Time.Numerator(), Time.Denominator() );

	Buffer.Init();

	Buffer = Bar;

	mthfrc::Sub( Buffer, Note, Bar );

	if ( Bar.GetSign() != 1 )
		ERRLmt();

	while ( Note.GetSign() != 0 ) {
		Duration = FindDuration_( Note, false );

		if ( !Duration.IsValid() )
			ERRLmt();

		Melody.Append( note__( Pitch, Duration, Signature ) );
	}

ERRErr
ERREnd
ERREpilog
}

void mscmld::SplitToMatchBars(
	const melody_ &Source,
	melody_ &Target )
{
ERRProlog
	row__ Row = E_NIL;
	mthfrc::fraction Bar, Note, Buffer;
	signature_time__ Time;
ERRBegin
	Row = Source.First();

	while ( Row != E_NIL ) {
		if ( !Source( Row ).IsValid() )
			ERRFwk();

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
			mthfrc::Sub( Buffer, Note, Bar );

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
ERRErr
ERREnd
ERREpilog
}

static bso::u8__ GetChromaticAbsolute_( const pitch__ &Pitch )
{
	if ( ( BSO_U8_MAX / 12 ) < Pitch.Octave )
		ERRFwk();

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
		ERRFwk();
		break;
	}

	switch ( Pitch.Accidental ) {
		case paFlat:
			if ( Absolute == 0 )
				ERRFwk();
			Absolute--;
			break;
		case paNatural:
			break;
		case paSharp:
			if ( Absolute == BSO_U8_MAX )
				ERRFwk();
			Absolute++;
			break;
		default:
			ERRFwk();
			break;
	}

	return Absolute;
}

bso::u8__ mscmld::pitch__::GetChromatic( void ) const
{
	return GetChromaticAbsolute_( *this );
}


static bso::u8__ GetDiatonicAbsolute_( const pitch__ &Pitch )
{
	if ( ( BSO_U8_MAX / 7 ) < Pitch.Octave )
		ERRFwk();

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
		ERRFwk();
		break;
	}

	return Absolute;
}

void mscmld::Merge(
	const pitches_ &Pitches,
	const durations_ &Durations,
	const signatures_ &Signatures,
	notes_ &Notes )
{
	prow__ PRow = Pitches.First();
	drow__ DRow = Durations.First();
	srow__ SRow = Signatures.First();

	while ( ( PRow != E_NIL ) && ( DRow != E_NIL ) && ( SRow != E_NIL ) ) {
		Notes.Append( note__( Pitches( PRow ), Durations( DRow ), Signatures( SRow ) ) );

		PRow = Pitches.Next( PRow );
		DRow = Durations.Next( DRow );
		SRow = Signatures.Next( SRow );
	}
}

static bso::bool__ Adjust_(
	mthfrc::fraction_ &BarFraction,
	const mthfrc::fraction_ &NoteFraction,
	bso::bool__ *Error )
{
	bso::bool__ BarComplete = false;
ERRProlog
	mthfrc::fraction Buffer;
ERRBegin
#ifdef DEBUG
	cio::cout << BarFraction << txf::tab << NoteFraction << txf::tab << txf::sync;
#endif
	if ( *Error )
		ERRReturn;

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
		mthfrc::Sub( Buffer, NoteFraction, BarFraction );
		break;
	default:
		ERRFwk();
		break;
	}
#ifdef DEBUG
	cio::cout << BarFraction << txf::nl;
#endif

ERRErr
ERREnd
ERREpilog
	return BarComplete;
}

#define C( n )\
		case 1 << n:\
			Anacrousis.Base = n+1;\
			break

static void Convert_(
	const mthfrc::fraction_ &RawDuration,
	anacrousis__ &Anacrousis )
{
ERRProlog
	mthfrc::fraction Duration;
ERRBegin
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
		ERRFwk();
		break;
	}

	if ( Duration.N.GetU32() >= bso::U8Max )
		ERRFwk();

	Anacrousis.Amount = (bso::u8__)Duration.N.GetU32();
ERRErr
ERREnd
ERREpilog
}

bso::bool__ mscmld::melody_::MarkAsAnacrousis( err::handling__ Handling )
{
	bso::bool__ Success = false;
ERRProlog
	mthfrc::fraction Total, Remaining, Note;
	bso::bool__ BarHandlingError = false;
	row__ Row = E_NIL;
	bso::bool__ BarIsComplete = false;
ERRBegin
	if ( IsEmpty() )
		ERRFwk();

	Row = First();

	Total.Init( Get( Row ).Signature.Time.Numerator(), Get( Row ).Signature.Time.Denominator() );

	Remaining.Init( Total );

	while ( Row != E_NIL ) {
		if ( BarIsComplete )
			if ( Handling == err::hThrowException )
				ERRFwk();
			else
				ERRReturn;

		Note.Init();
		GetFraction_( Get( Row ).Duration, Note );

		BarIsComplete = ::Adjust_( Remaining, Note, &BarHandlingError );

		Row = Next( Row );
	}

	if ( !BarIsComplete ) {
		Note.Init();
		Note = Total - Remaining;

		Note.Simplify();
	
		Convert_( Note, S_.Anacrousis );
	}

	Success = true;

ERRErr
ERREnd
ERREpilog
	return Success;
}

static void WriteXML_(
	const pitch__ &Pitch,
	const pitch__ &PreviousPitch,
	xml::writer_ &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PushTag( PITCH_TAG );

	Writer.PutAttribute( NAME_ATTRIBUTE, GetPitchNameLabel( Pitch.Name ) );

	Writer.PutAttribute( ACCIDENTAL_ATTRIBUTE, GetPitchAccidentalLabel( Pitch.Accidental ) );

	Writer.PutAttribute( OCTAVE_ATTRIBUTE, bso::Convert( Pitch.Octave, Buffer ) );

	Writer.PutAttribute( "Diatonic", bso::Convert( GetDiatonicAbsolute_( Pitch ), Buffer ) );

	Writer.PutAttribute( "Chromatic", bso::Convert( GetChromaticAbsolute_( Pitch ), Buffer ) );

	if ( PreviousPitch.Name != pn_Undefined ) {
		bso::s16__ DiatonicDelta = (bso::s16__)GetDiatonicAbsolute_( Pitch ) - GetDiatonicAbsolute_( PreviousPitch );
		bso::s16__ ChromaticDelta = (bso::s16__)GetChromaticAbsolute_( Pitch ) - GetChromaticAbsolute_( PreviousPitch );

		Writer.PushTag( DIFF_TAG );

		Writer.PutAttribute( "Diatonic", bso::Convert( DiatonicDelta, Buffer ) );
		Writer.PutAttribute( "Chromatic", bso::Convert( ChromaticDelta, Buffer ) );

		Writer.PopTag();
	}

	Writer.PopTag();
}

static void WriteXMLDurationCoreAttributes_(
	const duration__ &Duration,
	xml::writer_ &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PutAttribute( BASE_TAG, bso::Convert( Duration.Base, Buffer ) );

	if ( Duration.Modifier != 0 )
		Writer.PutAttribute( "Modifier", bso::Convert( Duration.Modifier, Buffer ) );
}

static void WriteXML_(
	const duration__ &Duration,
	const duration__ &PreviousDuration,
	xml::writer_ &Writer )
{
	Writer.PushTag( DURATION_TAG );

	WriteXMLDurationCoreAttributes_( Duration, Writer );

	if ( PreviousDuration.Base != 0 )
		Writer.PutAttribute( "TiedToPrevious", ( PreviousDuration.TiedToNext ? "yes" : "no" ) );

	Writer.PutAttribute( TIED_FLAG_ATTRIBUTE, ( Duration.TiedToNext ? "yes" : "no" ) );

	Writer.PutAttribute( DIFF_ATTRIBUTE, ( ( Duration != PreviousDuration ) ? "yes" : "no" ) );

	Writer.PopTag();

}

static void WriteXML_(
	const signature_key__ &Key,
	const signature_key__ &PreviousKey,
	xml::writer_ &Writer )
{
	pitch_name__ Name = pn_Undefined;
	pitch_accidental__ Accidental = pa_Undefined;
	bso::integer_buffer__ Buffer;

	Writer.PushTag( KEY_TAG );

	switch ( Key ) {
	case -7:
		Name = pnC;
		Accidental = paFlat;
		break;
	case -6:
		Name = pnG;
		Accidental = paFlat;
		break;
	case -5:
		Name = pnD;
		Accidental = paFlat;
		break;
	case -4:
		Name = pnA;
		Accidental = paFlat;
		break;
	case -3:
		Name = pnE;
		Accidental = paFlat;
		break;
	case -2:
		Name = pnB;
		Accidental = paFlat;
		break;
	case -1:
		Name = pnF;
		Accidental = paNatural;
		break;
	case 0:
		Name = pnC;
		Accidental = paNatural;
		break;
	case 1:
		Name = pnG;
		Accidental = paNatural;
		break;
	case 2:
		Name = pnD;
		Accidental = paNatural;
		break;
	case 3:
		Name = pnA;
		Accidental = paNatural;
		break;
	case 4:
		Name = pnE;
		Accidental = paNatural;
		break;
	case 5:
		Name = pnB;
		Accidental = paNatural;
		break;
	case 6:
		Name = pnF;
		Accidental = paSharp;
		break;
	case 7:
		Name = pnC;
		Accidental = paSharp;
		break;
	default:
		ERRFwk();
		break;
	}

	Writer.PutAttribute( NAME_ATTRIBUTE, GetPitchNameLabel( Name ) );

	Writer.PutAttribute( ACCIDENTAL_ATTRIBUTE, GetPitchAccidentalLabel( Accidental ) );

	Writer.PutAttribute( RAW_ATTRIBUTE, bso::Convert( Key, Buffer ) );

	Writer.PutAttribute( DIFF_ATTRIBUTE, ( Key != PreviousKey ) ? "yes" : "no" );

	Writer.PopTag();
}

static void WriteXML_(
	const signature_time__ &Time,
	const signature_time__ &PreviousTime,
	xml::writer_ &Writer )
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

static void WriteXML_(
	const signature__ &Signature,
	const signature__ &PreviousSignature,
	xml::writer_ &Writer )
{
	Writer.PushTag( SIGNATURE_TAG );

//	Writer.PutAttribute( "Diff", ( Signature != PreviousSignature ) ? "yes" : "no" );

	WriteXML_( Signature.Key, PreviousSignature.Key, Writer );

	WriteXML_( Signature.Time, PreviousSignature.Time, Writer );

	Writer.PopTag();
}



static void WriteXML_(
	const note__ &Note,
	const note__ &PreviousNote,
	xml::writer_ &Writer )
{
	if ( Note.Pitch.Name == pnRest )
		Writer.PushTag( REST_TAG );
	else {
		Writer.PushTag( NOTE_TAG );

		WriteXML_( Note.Pitch, PreviousNote.Pitch, Writer );
	}

	WriteXML_( Note.Duration, PreviousNote.Duration, Writer );

	Writer.PopTag();
}


write_status__ mscmld::WriteXML(
	const melody_ &Melody,
	xml::writer_ &Writer )
{
	write_status__ Status = wsOK;
ERRProlog
	row__ Row = E_NIL;
	mthfrc::fraction BarFraction, NoteFraction, Buffer;
	note__ PreviousNote, Note;
	bso::bool__ BarClosed = true;
	bso::bool__ BarHandlingError = false;
	bso::bool__ HandleAnacrousis = false;
	xml::mark__ TupletControl;
	xml::mark__ Mark = XML_UNDEFINED_MARK;
	bso::bool__ WriteBar = false;
	bso::bool__ WriteSignature = false;
ERRBegin
	Mark = Writer.GetMark();

	Row = Melody.First();

	if ( ( Melody.Anacrousis().IsValid() ) && (Melody.Anacrousis().Amount != 0 ) ) {
		Writer.PushTag( ANACROUSIS_TAG );
		xml::PutAttribute( AnacrousisBaseAttribute_, Melody.Anacrousis().Base, Writer );
		xml::PutAttribute( AnacrousisAmountAttribute_, Melody.Anacrousis().Amount, Writer );
		Writer.PopTag();
		HandleAnacrousis = true;
	}

	while ( Row != E_NIL ) {
		if ( !Melody( Row ).IsValid() )
			ERRFwk();

		if ( Note.IsValid() )
			if ( Note.Pitch.Name != pnRest )
				PreviousNote = Note;
			else {
				PreviousNote.Duration = Note.Duration;
				PreviousNote.Signature = Note.Signature;
			}


		Note = Melody( Row );

		if ( Note.Signature != PreviousNote.Signature ) {
			if ( !BarClosed && ( PreviousNote.IsValid() ) ) {
				Status = wsBarChekError;
				ERRReturn;
			}
			WriteSignature = true;
		}
		
		if ( BarClosed ) {
			if ( PreviousNote.IsValid() ) // Si ce n'est pas le cas, c'est que la premire note n'a pas encore t trait, et on vite ainsi une 'bar' avant elle.
				WriteBar = true;

			BarFraction.Init( Note.Signature.Time.Numerator(), Note.Signature.Time.Denominator() );

			if ( HandleAnacrousis ) {
				Buffer.Init( BarFraction );

				if ( !Melody.Anacrousis().IsValid() )
					ERRFwk();

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
					WriteXML_( Note.Signature, PreviousNote.Signature, Writer );

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
				WriteXML_( Note.Signature, PreviousNote.Signature, Writer );

		} else { 
			if ( WriteBar ) {
				Writer.PushTag( BAR_TAG );
				Writer.PopTag();
			}

			if ( WriteSignature )
				WriteXML_( Note.Signature, PreviousNote.Signature, Writer );
		}

		WriteBar = false;
		WriteSignature = false;

		WriteXML_( Note, PreviousNote, Writer );

		NoteFraction.Init();

		GetFraction_( Note.Duration, NoteFraction );

		BarClosed = Adjust_( BarFraction, NoteFraction, &BarHandlingError );

		Row = Melody.Next( Row );
	}

	if ( Note.Duration.Tuplet.IsValid() )
		Writer.PopTag( TupletControl );
ERRErr
ERREnd
	if ( Mark != XML_UNDEFINED_MARK )
		if ( Mark != Writer.GetMark() )
			Writer.Rewind( Mark );
ERREpilog
	return Status;
}

static parse_status__ GetKeyRaw_(
	const str::string_ &Raw,
	signature_key__ &Key )
{
	sdr::row__ Error = E_NIL;

	Key = Raw.ToS8( &Error );

	if ( Error != E_NIL )
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
	signature_key__ &Key )
{
	parse_status__ Status = psOK;
	bso::bool__ Continue = true;
	
	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious )  ){
		case xml::tStartTag:
			Status = psUnexpectedTag;
			break;
		case xml::tAttribute:
			if ( Parser.AttributeName() == RAW_ATTRIBUTE )
				if ( IsValid( Key ) )
					Status = psAlreadyDefined;
				else
					Status = GetKeyRaw_( Parser.Value(), Key );
			break;
		case xml::tValue:
			Status = psUnexpectedValue;
			break;
		case xml::tEndTag:
			if ( Parser.TagName() != KEY_TAG )
				ERRFwk();
			Continue = false;
			break;
		default:
			ERRFwk();
			break;
		}

		if ( Status != psOK )
			Continue = false;
	}

	return Status;
}

static parse_status__ GetU8(
	const str::string_ &Value,
	bso::u8__ &U8 )
{
	sdr::row__ Error E_NIL;

	U8 = Value.ToU8( &Error );

	if ( Error != E_NIL )
		return psBadValue;

	return psOK;
}

static parse_status__ ParseTime_(
	xml::parser___ &Parser,
	signature_time__ &Time )
{
	parse_status__ Status = psOK;
	bso::bool__ Continue = true;
	bso::s8__ RawNumerator = MSCMLD_UNDEFINED_TIME_SIGNATURE_NUMERATOR;
	bso::u8__ DenominatorPower = MSCMLD_UNDEFINED_TIME_SIGNATURE_DENOMINATOR_POWER;
	sdr::row__ Error = E_NIL;

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

				if ( Error != E_NIL )
					Status = psBadValue;
			} else if ( Parser.AttributeName() == DENOMINATOR_POWER_ATTRIBUTE ) {
				if ( DenominatorPower != MSCMLD_UNDEFINED_TIME_SIGNATURE_DENOMINATOR_POWER )
					Status = psAlreadyDefined;

				DenominatorPower = Parser.Value().ToU8();

				if ( Error != E_NIL )
					Status = psBadValue;
			}
			break;
		case xml::tValue:
			Status = psUnexpectedValue;
			break;
		case xml::tEndTag:
			if ( Parser.TagName() != TIME_TAG )
				ERRFwk();

			if ( RawNumerator == MSCMLD_UNDEFINED_TIME_SIGNATURE_NUMERATOR )
				Status = psMissingSignatureTimeRawNumerator;
			else if ( DenominatorPower == MSCMLD_UNDEFINED_TIME_SIGNATURE_DENOMINATOR_POWER )
				Status = psMissingSignatureTimeDenominatorPower;
			else
				Time.SetRaw( RawNumerator, DenominatorPower );

			Continue = false;
			break;
		default:
			ERRFwk();
		}

		if ( Status != psOK )
			Continue = false;
	}

	return Status;
}

static parse_status__ ParseSignature_(
	xml::parser___ &Parser,
	signature__ &Signature )
{
	parse_status__ Status = psOK;
ERRProlog
	signature_key__ Key = MSCMLD_UNDEFINED_KEY_SIGNATURE;
	signature_time__ Time;
	bso::bool__ Continue = true;
ERRBegin
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
				ERRFwk();

			if ( !IsValid( Key ) )
				Status = psMissingSignatureKey;

			if ( !Time.IsValid() )
				Status = psMissingSignatureTime;

			Signature.Time = Time;
			Signature.Key = Key;

			Continue = false;

			break;
		default:
			ERRFwk();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
	return Status;
}

static pitch_name__ GetPitchName_( const str::string_ &Name )
{
	int i = 0;

	while ( ( i < pn_amount ) && ( Name != GetPitchNameLabel( (pitch_name__)i ) ) )
		i++;

	if ( i > pn_amount )
		i = pn_Undefined;

	return (pitch_name__)i;
}

static pitch_accidental__ GetPitchAccidental_( const str::string_ &Accidental )
{
	int i = 0;

	while ( ( i < pa_amount ) && ( Accidental != GetPitchAccidentalLabel( (pitch_accidental__)i ) ) )
		i++;

	if ( i > pa_amount )
		i = pa_Undefined;

	return (pitch_accidental__)i;
}

static pitch_octave__ GetPitchOctave_( const str::string_ &Octave )
{
	sdr::row__ Error = E_NIL;
	pitch_octave__ O = Octave.ToU8( &Error );

	if ( Error != E_NIL )
		O = MSCMLD_UNDEFINED_PITCH_OCTAVE;

	return O;


}

static parse_status__ ParsePitch_(
	xml::parser___ &Parser,
	pitch__ &Pitch )
{
	parse_status__ Status = psOK;
	bso::bool__ Continue = true;
	pitch_name__ Name = pn_Undefined;
	pitch_accidental__ Accidental = pa_Undefined;
	pitch_octave__ Octave = MSCMLD_UNDEFINED_PITCH_OCTAVE;

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
				if ( Accidental != pa_Undefined )
					Status = psAlreadyDefined;
				else {
					Accidental = GetPitchAccidental_( Parser.Value() );

					if ( Accidental == pa_Undefined )
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
				ERRFwk();

			if ( Name == pn_Undefined )
				Status = psMissingPitchName;
			else if ( Accidental == pa_Undefined )
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
		}

		if ( Status != psOK )
			Continue = false;
	}

	return Status;
}

static bso::s8__ GetDurationBase_( const str::string_ &Value )
{
	bso::s8__ Base = MSCMLD_UNDEFINED_DURATION_BASE;
	sdr::row__ Error = E_NIL;

	Base = Value.ToS8( &Error );

	if ( Error != E_NIL )
		Base = MSCMLD_UNDEFINED_DURATION_BASE;

	return Base;
}

static bso::u8__ GetDurationModifier_( const str::string_ &Value )
{
	bso::u8__ Modifier = MSCMLD_UNDEFINED_DURATION_MODIFIER;
	sdr::row__ Error = E_NIL;

	Modifier = Value.ToU8( &Error );

	if ( Error != E_NIL )
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
	const tuplet__ &Tuplet,
	duration__ &Duration )
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
				ERRFwk();

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
			ERRFwk();
			break;
		}

		if ( Status != psOK )
			Continue = false;
	}

	return Status;
}

static parse_status__ ParseNote_( 
	xml::parser___ &Parser,
	const signature__ &Signature,
	const tuplet__ &Tuplet,
	note__ &Note )
{
	parse_status__ Status = psOK;
	bso::bool__ Continue = true;
	pitch__ Pitch;
	duration__ Duration;

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
				ERRFwk();

			if ( !Pitch.IsValid() )
				Status = psMissingPitch;
			else if ( !Duration.IsValid() )
				Status = psMissingDuration;
			else if ( !Signature.IsValid() )
				Status = psMissingSignature;
			else {
				Note.Pitch = Pitch;
				Note.Duration = Duration;
				Note.Signature = Signature;
			}

			Continue = false;
			break;
		default:
			ERRFwk();
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
	const signature__ &Signature,
	const tuplet__ &Tuplet,
	note__ &Note  )
{
	parse_status__ Status = psOK;
	bso::bool__ Continue = true;
	pitch__ Pitch;
	duration__ Duration;

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
				ERRFwk();

			if ( !Pitch.IsValid() )
				ERRFwk();
			else if ( !Duration.IsValid() )
				Status = psMissingDuration;
			else if ( !Signature.IsValid() )
				Status = psMissingSignature;
			else {
				Note.Pitch = Pitch;
				Note.Duration = Duration;
			}

			Continue = false;
			break;
		default:
			ERRFwk();
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
	sdr::row__ Error = E_NIL;

	Numerator = Value.ToU8( &Error );

	if ( Error != E_NIL )
		Numerator = MSCMLD_UNDEFINED_TUPLET_NUMERATOR;

	return Numerator;
}

static bso::u8__ GetTupletDenominator_( const str::string_ &Value )
{
	bso::u8__ Denominator = MSCMLD_UNDEFINED_TUPLET_DENOMINATOR;
	sdr::row__ Error = E_NIL;

	Denominator = Value.ToU8( &Error );

	if ( Error != E_NIL )
		Denominator = MSCMLD_UNDEFINED_TUPLET_DENOMINATOR;

	return Denominator;
}

static parse_status__ ParseTuplet_( 
	xml::parser___ &Parser,
	const signature__ &Signature,
	melody_ &Melody )
{
	parse_status__ Status = psOK;
	bso::bool__ Continue = true;
	bso::u8__
		Numerator = MSCMLD_UNDEFINED_TUPLET_NUMERATOR,
		Denominator = MSCMLD_UNDEFINED_TUPLET_DENOMINATOR;
	tuplet__ Tuplet;
	note__ Note;

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
					ERRFwk();

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
				ERRFwk();

			Continue = false;
			break;
		default:
			ERRFwk();
			break;
		}

		if ( Status != psOK )
			Continue = false;
	}

	return Status;
}

static parse_status__ ParseAnacrousis_(
	xml::parser___ &Parser,
	const signature__ &Signature,
	anacrousis__ &Anacrousis ) 
{
	parse_status__ Status = psOK;
	bso::bool__ Continue = true;
	bso::u8__
		Base = AnacrousisUndefinedBase,
		Amount = 0;
	sdr::row__ Error = E_NIL;

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			Status = psUnexpectedTag;
			break;
		case xml::tAttribute:
			if ( Parser.AttributeName() == AnacrousisBaseAttribute_ ) {
				if ( Base != AnacrousisUndefinedBase )
					Status = psAlreadyDefined;
				else
					Base = Parser.Value().ToU8( &Error );

				if ( (Error != E_NIL) || ( Base == AnacrousisUndefinedBase ) )
					Status = psBadValue;
			} else if ( Parser.AttributeName() == AnacrousisAmountAttribute_ ) {
				if ( Amount != 0 )
					Status = psAlreadyDefined;
				else
					Amount = Parser.Value().ToU8( &Error );

				if ( ( Error != E_NIL ) || ( Amount == 0 ) )
					Status = psBadValue;
			} else 
				Status = psUnexpectedAttribute;
		case xml::tValue:
			Status = psUnexpectedValue;
			break;
		case xml::tEndTag:
			if ( Parser.TagName() != ANACROUSIS_TAG )
				ERRFwk();

			if ( Base == AnacrousisUndefinedBase )
				Status = psMissingAnacrousisBase;
			else if ( Amount = 0 )
				Status = psMissingAnacrousisAmount;
			else {
				Anacrousis.Base = Base;
				Anacrousis.Amount = Amount;

			}


			Continue = false;
			break;
		default:
			ERRFwk();
			break;
		}

		if ( Status != psOK )
			Continue = false;
	}

	return Status;
}


static parse_status__ Parse_(
	xml::parser___ &Parser,
	melody_ &Melody ) 
{
	parse_status__ Status = psOK;
	bso::bool__ Continue = true;
	signature__ Signature;
	note__ Note;

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == SIGNATURE_TAG )
				Status = ParseSignature_( Parser, Signature );
			else if ( Parser.TagName() == NOTE_TAG )
				Status = ParseNote_( Parser, Signature, tuplet__(), Note );
			else if ( Parser.TagName() == REST_TAG )
				Status = ParseRest_( Parser, Signature, tuplet__(), Note );
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
			if ( Parser.TagName() != MELODY_TAG )
				ERRFwk();

			Continue = false;
			break;
		default:
			ERRFwk();
			break;
		}

		if ( Status != psOK )
			Continue = false;
	}

	return Status;
}

parse_status__ mscmld::ParseXML(
	xml::parser___ &Parser,
	melody_ &Melody,
	bso::bool__ WithRoot )	// Si  'true', la prochaine balise est 'Melody', sinon, on est  l'intrieur de 'Melody'. Dans les deux cas, au retour on est  l'extrieur de la balise 'Melody'.
{
	if ( WithRoot)
		ERRVct();

	return Parse_( Parser, Melody );
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class mscmldpersonnalization
{
public:
	mscmldpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mscmldpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static mscmldpersonnalization Tutor;
