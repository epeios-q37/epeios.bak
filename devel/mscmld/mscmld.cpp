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

#define SIGNATURE_TAG	"Signature"
#define NOTE_TAG		"Note"
#define REST_TAG		"Rest"
#define BAR_TAG			"Bar"
#define MELODY_TAG		"Melody"
#define KEY_TAG			"Key"
#define TIME_TAG		"Time"
#define RAW_ATTRIBUTE	"Raw"

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

	if ( Tuplet.IsInUse() ) {
		mthfrc::Div( Result, mthfrc::fraction( Tuplet.Den ), Buffer );
		mthfrc::Mul( Buffer, mthfrc::fraction( Tuplet.Num ), Result );
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

	Bar.Init( Time.Numerator, Time.ComputeDenominator() );

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
			Bar.Init( Time.Numerator, Time.ComputeDenominator() );
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
			Bar.Init( Time.Numerator, Time.ComputeDenominator() );
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
		Absolute += 10;
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

	Total.Init( Get( Row ).Signature.Time.Numerator, Get( Row ).Signature.Time.ComputeDenominator() );

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

		
		S_.Anacrousis = FindDuration_( Note, false );
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

	Writer.PushTag( "Pitch" );

	Writer.PutAttribute( "Name", GetPitchNameLabel( Pitch.Name ) );

	Writer.PutAttribute( "Accidental", GetPitchAccidentalLabel( Pitch.Accidental ) );

	Writer.PutAttribute( "Octave", bso::Convert( Pitch.Octave, Buffer ) );

	Writer.PutAttribute( "Diatonic", bso::Convert( GetDiatonicAbsolute_( Pitch ), Buffer ) );

	Writer.PutAttribute( "Chromatic", bso::Convert( GetChromaticAbsolute_( Pitch ), Buffer ) );

	if ( PreviousPitch.Name != pn_Undefined ) {
		bso::s16__ DiatonicDelta = (bso::s16__)GetDiatonicAbsolute_( Pitch ) - GetDiatonicAbsolute_( PreviousPitch );
		bso::s16__ ChromaticDelta = (bso::s16__)GetChromaticAbsolute_( Pitch ) - GetChromaticAbsolute_( PreviousPitch );

		Writer.PushTag( "Diff" );

		Writer.PutAttribute( "Diatonic", bso::Convert( DiatonicDelta, Buffer ) );
		Writer.PutAttribute( "Chromatic", bso::Convert( ChromaticDelta, Buffer ) );

		Writer.PopTag();
	}

	Writer.PopTag();
}

static void WriteXMLCoreAttributes_(
	const duration__ &Duration,
	xml::writer_ &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PutAttribute( "Base", bso::Convert( Duration.Base, Buffer ) );

	Writer.PutAttribute( "Modifier", bso::Convert( Duration.Modifier, Buffer ) );
}

static void WriteXML_(
	const duration__ &Duration,
	const duration__ &PreviousDuration,
	xml::writer_ &Writer )
{
	Writer.PushTag( "Duration" );

	WriteXMLCoreAttributes_( Duration, Writer );

	if ( PreviousDuration.Base != 0 )
		Writer.PutAttribute( "TiedToPrevious", ( PreviousDuration.TiedToNext ? "yes" : "no" ) );

	Writer.PutAttribute( "TiedToNext", ( Duration.TiedToNext ? "yes" : "no" ) );

	Writer.PutAttribute( "Diff", ( ( Duration != PreviousDuration ) ? "yes" : "no" ) );

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

	Writer.PutAttribute( "Name", GetPitchNameLabel( Name ) );

	Writer.PutAttribute( "Accidental", GetPitchAccidentalLabel( Accidental ) );

	Writer.PutAttribute( RAW_ATTRIBUTE, bso::Convert( Key, Buffer ) );

	Writer.PutAttribute( "Diff", ( Key != PreviousKey ) ? "yes" : "no" );

	Writer.PopTag();
}

static void WriteXML_(
	const signature_time__ &Time,
	const signature_time__ &PreviousTime,
	xml::writer_ &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PushTag( TIME_TAG );

	Writer.PutAttribute( "Numerator", bso::Convert( Time.Numerator, Buffer ) );

	Writer.PutAttribute( "Denominator", bso::Convert( Time.ComputeDenominator(), Buffer ) );

	if ( Time.IsDenHidden() )
		Writer.PutAttribute( "DenominatorIsHidden", "true" );

	Writer.PutAttribute( "Diff", ( Time != PreviousTime ) ? "yes" : "no" );

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
ERRBegin
	Mark = Writer.GetMark();

	Row = Melody.First();

	if ( Melody.Anacrousis().IsValid() ) {
		Writer.PushTag( "Anacrousis" );
		WriteXMLCoreAttributes_( Melody.Anacrousis(), Writer );
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
			WriteXML_( Note.Signature, PreviousNote.Signature, Writer );
		}
		
		if ( BarClosed ) {
			if ( PreviousNote.IsValid() ) { // Si ce n'est pas le cas, c'est que la première note n'a pas encore été traité, et on évite ainsi une 'bar' avant elle.
				Writer.PushTag( BAR_TAG );
				Writer.PopTag();
			}

			BarFraction.Init( Note.Signature.Time.Numerator, Note.Signature.Time.ComputeDenominator() );

			if ( HandleAnacrousis ) {
				Buffer.Init( BarFraction );
				NoteFraction.Init();
				GetFraction_( Melody.Anacrousis(), NoteFraction );
				Adjust_( Buffer, NoteFraction, &BarHandlingError );
				Adjust_( BarFraction, Buffer, &BarHandlingError );
				HandleAnacrousis = false;
			}
		}

		if ( Note.Duration.Tuplet.IsInUse() ) {
			if ( Note.Duration.Tuplet != PreviousNote.Duration.Tuplet ) {
				if ( PreviousNote.Duration.Tuplet.IsInUse() )
					Writer.PopTag( TupletControl );
				TupletControl = Writer.PushTag( "Tuplet" );
				xml::PutAttribute( "Numerator", Note.Duration.Tuplet.Num, Writer );
				xml::PutAttribute( "Denominator", Note.Duration.Tuplet.Den, Writer );
			}
		} else if ( PreviousNote.Duration.Tuplet.IsInUse() )
			Writer.PopTag( TupletControl );

		WriteXML_( Note, PreviousNote, Writer );

		NoteFraction.Init();

		GetFraction_( Note.Duration, NoteFraction );

		BarClosed = Adjust_( BarFraction, NoteFraction, &BarHandlingError );

		Row = Melody.Next( Row );
	}

	if ( Note.Duration.Tuplet.IsInUse() )
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

	Key = Raw.ToU8( &Error );

	if ( Error != E_NIL )
		return psBadValue;
	else if ( Key > 7 )
		return psBadValue;
	else if ( Key < 7 )
		return psBadValue;
	else
		return psOK;
}


// ...<Key>...</Key>...
//         ^
// ...<Key>...</Key>...
//                  ^
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
					Status = psKeySignatureAlreadyDefined;
				else
					Status = GetKeyRaw_( Parser.Value(), Key );
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
}



// ...<Signature>...<Signature>...
//               ^
// ...<Signature>...<Signature>...
//                             ^
static parse_status__ ParseSignature_(
	xml::parser___ &Parser,
	signature__ &Signature )
{
	parse_status__ Status = psOK;
ERRProlog
	signature_key__ Key = MSCMLD_UNDEFINED_KEY_SIGNATURE;
	signature_time__ Time;
ERRBegin
	Initialize( Key );
	Time.Init();

	while ( Status == psOK ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == KEY_TAG )
				if ( IsValid( Key ) )
					Status = psKeySignatureAlreadyDefined;
				else
					Status = ParseKey_( Parser, Key );
			else if ( Parser.TagName() == TIME_TAG )
				if ( Time.IsValid() )
					Status = psTimeSignatureAlreadyDefined;
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
			if ( Key != 
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

// ...<Melody>...</Melody>...
//            ^
// ...<Melody>...</Melody>...
//                        ^
static parse_status__ Parse_(
	xml::parser___ &Parser,
	melody_ &Melody ) 
{
	parse_status__ Status = psOK;
ERRProlog
	signature__ Signature;
	note__ Note;
	bso::bool__ Continue = true;
ERRBegin
	while ( Status == psOK ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == SIGNATURE_TAG )
				Status = ParseSignature_( Parser, Signature );
			else if ( Parser.TagName() == NOTE_TAG ) {
				Status = ParserNote_( Parser, Signature, Note );
			else if ( Parser.TagName() == REST_TAG )
				Status = ParseRest_( Parser, Signature, Note );
			else if ( Parser.TagName() == BAR_TAG )
				Status = ParseBar_( Parser );
			else
				Status = psUnexpectedTag;

			if ( Status == sOK )
				if ( Note.IsValid() )
					Melody.Append( Note );
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
			break;
		default:
			ERRFwk();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
}

void mscmld::ParseXML(
	xml::parser___ &Parser,
	melody_ &Melody,
	bso::bool__ WithRoot );	// Si à 'true', la prochaine balise est 'Melody', sinon, on est à l'intérieur de 'Melody'. Dans les deux cas, au rerour on est à l'extérieur de la balise 'Melody'.




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
