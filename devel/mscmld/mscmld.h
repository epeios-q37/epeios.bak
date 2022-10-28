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

#ifndef MSCMLD_INC_
# define MSCMLD_INC_

# define MSCMLD_NAME		"MSCMLD"

# if defined( E_DEBUG ) && !defined( MSCMLD_NODBG )
#  define MSCMLD_DBG
# endif

// MuSiC MeLody Description

# include "err.h"
# include "flw.h"
# include "bch.h"
# include "xml.h"
# include "bso.h"

namespace mscmld {

	inline bso::u8__ Log2Abs_( bso::sU8 Value )
	{
		if ( Value < 0 )
			Value = -Value;

		switch ( Value ) {
		case 0:
			qRFwk();
			break;
		case 1:
			return 0;
			break;
		case 2:
			return 1;
			break;
		case 4:
			return 2;
			break;
		case 8:
			return 3;
			break;
		case 16:
			return 4;
			break;
		case 32:
			return 5;
			break;
		case 64:
			return 6;
			break;
		case 128:
			return 7;
			break;
		default:
			qRFwk();
			break;
		}

		return 0;	// To avoid a warning;
	}

	// If modified, modify 'const char *mscmld::GetLabel(ePitchName Name)' !!!
	qENUM( PitchName ) {
		pnA,
		pnB,
		pnC,
		pnD,
		pnE,
		pnF,
		pnG,
		pnRest,
		pn_amount,
		pn_Undefined
	};

	const char *GetLabel( ePitchName Name );

	qENUM( Accidental ) {
		aFlat,
		aNatural,
		aSharp,
		a_amount,
		a_Undefined
	};

	const char *GetLabel( eAccidental Accidental );

  eAccidental GetAccidental(const str::dString &Pattern);

	typedef bso::u8__ sOctave;
#define MSCMLD_UNDEFINED_PITCH_OCTAVE BSO_U8_MAX

	struct sAltPitch {
		ePitchName Name;
		eAccidental Accidental;
		sOctave Octave;
		void reset( bso::bool__ P = true )
		{
			Name = pn_Undefined;
			Accidental = a_Undefined;
			Octave = MSCMLD_UNDEFINED_PITCH_OCTAVE;
		}
		qCTOR( sAltPitch );
		sAltPitch(
			sOctave Octave,
			ePitchName Name,
			eAccidental Accidental = aNatural )
		{
			reset( false );

			this->Name = Name;
			this->Accidental = Accidental;
			this->Octave = Octave;
		}
		sAltPitch(
			ePitchName Name,
			sOctave Octave,
			eAccidental Accidental = aNatural )
		{
			reset( false );

			this->Name = Name;
			this->Accidental = Accidental;
			this->Octave = Octave;
		}
		sAltPitch(
			ePitchName Name,
			eAccidental Accidental,
			sOctave Octave )
		{
			this->Name = Name;
			this->Accidental = Accidental;
			this->Octave = Octave;
		}
		void Init( void )
		{
			reset();
		}
		bso::bool__ IsValid( void ) const
		{
			return ( Name != pn_Undefined )
				   && ( ( ( Accidental != a_Undefined )
				          && ( Octave != MSCMLD_UNDEFINED_PITCH_OCTAVE ) )
				        || ( Name == pnRest ) );
		}
		bso::u8__ GetChromatic( void ) const;
	};

	inline const char *GetLabel(const sAltPitch &Pitch)
	{
	  return GetLabel(Pitch.Name);
	}

	inline int operator ==(
		const sAltPitch &Op1,
		const sAltPitch &Op2 )
	{
		return ( Op1.Name == Op2.Name ) && ( Op1.Accidental == Op2.Accidental ) && ( Op1.Octave == Op2.Octave );
	}

	inline int operator !=(
		const sAltPitch &Op1,
		const sAltPitch &Op2 )
	{
		return ( Op1.Name != Op2.Name ) || ( Op1.Accidental != Op2.Accidental ) || ( Op1.Octave != Op2.Octave );
	}

	qROW( PRow );

	typedef bch::qBUNCHd( sAltPitch, sPRow ) dAltPitches;
	qW( AltPitches )

# define MSCMLD_UNDEFINED_TUPLET_NUMERATOR		BSO_U8_MAX
# define MSCMLD_UNDEFINED_TUPLET_DENOMINATOR	BSO_U8_MAX

	struct sTuplet {
		bso::u8__ Numerator;
		bso::u8__ Denominator;
		void reset( bso::bool__ = true )
		{
			Numerator = MSCMLD_UNDEFINED_TUPLET_NUMERATOR;
			Denominator = MSCMLD_UNDEFINED_TUPLET_DENOMINATOR;
		}
		qCDTOR( sTuplet );
		void Init( void )
		{
			reset();
		}
		bso::bool__ IsValid( void ) const
		{
			return ( Numerator != MSCMLD_UNDEFINED_TUPLET_NUMERATOR ) && ( Denominator != MSCMLD_UNDEFINED_TUPLET_DENOMINATOR );
		}
	};

	inline int operator ==(
		sTuplet Op1,
		sTuplet Op2 )
	{
		if ( Op1.IsValid() )
			if ( Op1.IsValid() )
				return ( ( Op1.Denominator == Op2.Denominator ) && ( Op1.Numerator == Op2.Numerator ) );
			else
				return false;
		else if ( Op2.IsValid() )
			return false;
		else
			return true;
	}

	inline int operator !=(
		sTuplet Op1,
		sTuplet Op2 )
	{
		if ( Op1.IsValid() )
			if ( Op1.IsValid() )
				return ( ( Op1.Denominator != Op2.Denominator ) || ( Op1.Numerator != Op2.Numerator ) );
			else
				return true;
		else if ( Op2.IsValid() )
			return true;
		else
			return false;
	}

# define MSCMLD_UNDEFINED_DURATION_BASE	0
# define MSCMLD_UNDEFINED_DURATION_MODIFIER	BSO_U8_MAX

	struct sDuration {
		bso::s8__ Base;	// 1 : whole, 2 : half, 3 : quarter, 4 : eight, ... -1 : double whole ; -2 : quadruple whole...
		bso::u8__ Modifier;	// Amount of dot.
		sTuplet Tuplet;
		bso::bool__ TiedToNext;
		void reset( bso::bool__ P = true )
		{
			Base = MSCMLD_UNDEFINED_DURATION_BASE;
			Modifier = MSCMLD_UNDEFINED_DURATION_MODIFIER;
			Tuplet.reset( P );
			TiedToNext = false;
		}
		qCDTOR( sDuration );
		sDuration(
			bso::s8__ Base,
			bso::u8__ Modifier = 0,
			sTuplet Tuplet = sTuplet(),
			bso::bool__ TiedToNext = false )
		{
			Init( Base, Modifier, TiedToNext, Tuplet );
		}
		sDuration(
			bso::s8__ Base,
			bso::u8__ Modifier,
			bso::bool__ TiedToNext,
			sTuplet Tuplet = sTuplet() )
		{
			Init( Base, Modifier, TiedToNext, Tuplet );
		}
		void Init( void )
		{
			reset();
		}
		void Init(
			bso::s8__ Base,
			bso::u8__ Modifier,
			bso::bool__ TiedToNext,
			sTuplet Tuplet = sTuplet() )
		{
			Init();

			this->Base = Base;
			this->Modifier = Modifier;
			this->Tuplet = Tuplet;
			this->TiedToNext = TiedToNext;
		}
		void Init(
			bso::s8__ Base,
			bso::u8__ Modifier = 0,
			sTuplet Tuplet = sTuplet(),
			bso::bool__ TiedToNext = false )
		{
			Init( Base, Modifier, TiedToNext, Tuplet );
		}
		bso::bool__ IsValid( void ) const
		{
			return ( Base != MSCMLD_UNDEFINED_DURATION_BASE ) && ( Modifier != MSCMLD_UNDEFINED_DURATION_MODIFIER );
		}
	};

	inline int operator ==(
		sDuration Op1,
		sDuration Op2 )
	{
		return ( ( Op1.Base == Op2.Base ) && ( Op1.Modifier == Op2.Modifier ) );
	}

	inline int operator !=(
		sDuration Op1,
		sDuration Op2 )
	{
		return ( ( Op1.Base != Op2.Base ) || ( Op1.Modifier != Op2.Modifier ) );
	}

	qROW( DRow );

	typedef bch::qBUNCHd( sDuration, sDRow ) dDurations;
	qW( Durations )

	typedef bso::s8__ sSignatureKey;	// -1: 1 flat, -2, 2 flats, ..., 0 : C key, 1 : 1 sharp, 2 : 2 sharps, ...
#define MSCMLD_UNDEFINED_KEY_SIGNATURE	BSO_S8_MAX

	inline void Initialize( sSignatureKey &Key )
	{
		Key = MSCMLD_UNDEFINED_KEY_SIGNATURE;
	}

	inline bso::bool__ IsValid( sSignatureKey Key )
	{
		return Key != MSCMLD_UNDEFINED_KEY_SIGNATURE;
	}

	E_TMIMIC__( bso::s8__, numerator__ );
	E_TMIMIC__( bso::u8__, denominator_power__ );

# define MSCMLD_UNDEFINED_TIME_SIGNATURE_NUMERATOR			0
# define MSCMLD_TIME_SIGNATURE_NUMERATOR_MAX				BSO_S8_MAX
# define MSCMLD_UNDEFINED_TIME_SIGNATURE_DENOMINATOR_POWER	BSO_U8_MAX

	inline bso::bool__ IsValid( numerator__ Numerator )
	{
		return Numerator != MSCMLD_UNDEFINED_TIME_SIGNATURE_NUMERATOR;
	}

	inline bso::bool__ IsValid( denominator_power__ DenominatorPower )
	{
		return DenominatorPower != MSCMLD_UNDEFINED_TIME_SIGNATURE_DENOMINATOR_POWER;
	}

	struct sSignatureTime {
	private:
		numerator__ _Numerator;	// Si < 0, le d�nominateur (bien le d�nominateur) n'est pas cens� �tre affich�.
		denominator_power__ _DenominatorPower;	// Le d�nominateur r�el est �gal � = 2 ^ abs('DenominatorPower').
		void _Test( void ) const
		{
			if ( !IsValid() )
				qRFwk();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Numerator = MSCMLD_UNDEFINED_TIME_SIGNATURE_NUMERATOR;
			_DenominatorPower = MSCMLD_UNDEFINED_TIME_SIGNATURE_DENOMINATOR_POWER;
		}
		qCVDTOR( sSignatureTime );
		sSignatureTime (
			bso::u8__ Numerator,
			bso::s8__ Denominator,
			bso::bool__ DenominatorIsHidden = false )
		{
			reset( false );

			Init( Numerator, Denominator, DenominatorIsHidden );
		}
		void Init( void )
		{
			reset();
		}
		void Init(
			bso::u8__ Numerator,
			bso::u8__ Denominator,
			bso::bool__ DenominatorIsHidden = false  )
		{
			if ( Numerator > MSCMLD_TIME_SIGNATURE_NUMERATOR_MAX )
				qRFwk();

			if ( Numerator == 0 )
				qRFwk();

			_Numerator = ( DenominatorIsHidden ? -Numerator : Numerator );

			_DenominatorPower = Log2Abs_( Denominator );
		}
		void SetRaw(
			bso::s8__ RawNumerator,
			bso::u8__ DenominatorPower )
		{
			_Numerator = RawNumerator;
			_DenominatorPower = DenominatorPower;
		}
		bso::bool__ IsValid( void ) const
		{
			return ( mscmld::IsValid( _Numerator ) && mscmld::IsValid( _DenominatorPower ) );
		}
		bso::u8__ DenominatorPower( void ) const
		{
			_Test();

			return *_DenominatorPower;
		}
		bso::u8__ Denominator( void ) const
		{
			return 1 << DenominatorPower();
		}
		bso::s8__ RawNumerator( void ) const
		{
			_Test();

			return *_Numerator;
		}
		bso::u8__ Numerator( void ) const
		{
			bso::s8__ N = RawNumerator();

			return ( N < 0 ? -N : N );
		}
		bso::bool__ IsDenHidden( void ) const
		{
			return RawNumerator() < 0;
		}
	};

	inline int operator ==(
		const sSignatureTime &Op1,
		const sSignatureTime &Op2 )
	{
		bso::bool__
			V1 = Op1.IsValid(),
			V2 = Op2.IsValid();

		if ( !V1 )
			if ( !V2 )
				return true;
			else
				return false;
		else if ( !V2 )
			return false;

		return ( ( Op1.Numerator() == Op2.Numerator() ) && ( Op1.Denominator() == Op2.Denominator() ) );
	}

	inline int operator !=(
		const sSignatureTime &Op1,
		const sSignatureTime &Op2 )
	{
		return !operator==( Op1, Op2 );
	}

	struct sSignature {
		sSignatureKey Key;
		sSignatureTime Time;
		void reset( bso::bool__ P = true )
		{
			Key = MSCMLD_UNDEFINED_KEY_SIGNATURE;
			Time.reset( P );
		}
		qCDTOR( sSignature );
		sSignature(
			const sSignatureKey &Key,
			const sSignatureTime &Time )
		{
			reset( false );

			this->Key = Key;
			this->Time = Time;
		}
		sSignature(
			const sSignatureTime &Time,
			const sSignatureKey &Key )
		{
			reset( false );

			this->Key = Key;
			this->Time = Time;
		}
		void Init( void )
		{
			Key = MSCMLD_UNDEFINED_KEY_SIGNATURE;
			Time.Init();
		}
		bso::bool__ IsValid( void ) const
		{
			return Time.IsValid() && ( mscmld::IsValid( Key  ) );
		}
	};

	inline int operator ==(
		const sSignature &Op1,
		const sSignature &Op2 )
	{
		return ( ( Op1.Key == Op2.Key ) && ( Op1.Time == Op2.Time ) );
	}

	inline int operator !=(
		const sSignature &Op1,
		const sSignature &Op2 )
	{
		return ( ( Op1.Key != Op2.Key ) || ( Op1.Time != Op2.Time ) );
	}

	qROW( SRow );

	typedef bch::qBUNCHd( sSignature, sSRow ) dSignatures;
	qW( Signatures )

	typedef bso::sU8 sPitch;

	typedef bch::qBUNCHd( sPitch, sPRow ) dPitches;
	qW( Pitches );

	qENUM( Pitch )	{
	  pRest = 128,
	  p_amount,
	  p_Undefined
	};

  const sAltPitch &Convert(
    sPitch Pitch,
    sSignatureKey Key,
    eAccidental Accidental,  // Accidental to use if key = 0 (C key).
    sAltPitch &AltPitch);

	const char *GetLabel(
    sPitch Pitch,
    sSignatureKey Key,
    eAccidental Accidental);

	sOctave GetOctave(
    sPitch Pitch,
    sSignatureKey Key,
    eAccidental Accidental);

	inline bso::sBool IsPitchValid(sPitch Pitch)
	{
	  return Pitch < p_amount;
	}

	struct sNote {
		sPitch Pitch;
		sDuration Duration;
		sSignature Signature;
		void reset( bso::bool__ P = true )
		{
			Pitch = p_Undefined;
			Duration.reset( P );
			Signature.reset( P );
		}
		qCDTOR( sNote );
		sNote(
			const sPitch &Pitch,
			const sDuration &Duration,
			const sSignature &Signature )
		{
			reset( false );

			this->Pitch = Pitch;
			this->Duration = Duration;
			this->Signature = Signature;
		}
		void Init( void )
		{
			Pitch = p_Undefined;
			Duration.Init();
			Signature.Init();
		}
		bso::bool__ IsValid( void ) const
		{
			return ( IsPitchValid(Pitch) && Duration.IsValid() && Signature.IsValid() );
		}
	};

	qROW( Row );

	typedef bch::qBUNCHd( sNote, sRow ) dNotes;
	qW( Notes );

  inline const sAltPitch &Convert(
    const sNote &Note,
    eAccidental Accidental,  // Accidental to use if key = 0 (C key).
    sAltPitch &AltPitch)
  {
    return Convert(Note.Pitch, Note.Signature.Key, Accidental, AltPitch);
  }

	inline const char *GetLabel(
    const sNote Note,
    eAccidental Accidental)
	{
	  return GetLabel(Note.Pitch, Note.Signature.Key, Accidental);
	}

	inline sOctave GetOctave(
    const sNote Note,
    eAccidental Accidental)
	{
	  return GetOctave(Note.Pitch, Note.Signature.Key, Accidental);
	}

	void Merge(
		const dPitches &Pitches,
		const dDurations &Durations,
		const dSignatures &Signatures,
		dNotes &Notes );

	E_CDEF( bso::u8__, AnacrousisUndefinedBase, 0 );

	class sAnacrousis {
	public:
		bso::u8__ Base;	// 0: Undefined1: noire, 2: croche, 3: double-croche, ...
		bso::u8__ Amount;
		void reset( bso::bool__ = true )
		{
			Base = AnacrousisUndefinedBase;
			Amount = 0;
		}
		qCDTOR( sAnacrousis );
		void Init( void )
		{
			Base = 1;
			Amount = 0;
		}
		bso::bool__ IsValid( void ) const
		{
			return Base != AnacrousisUndefinedBase;
		}
	};

	class dMelody
	: public dNotes
	{
	public:
		struct s
		: public dNotes::s
		{
			sAnacrousis Anacrousis;
		} &S_;
		dMelody( s &S )
		: dNotes( S ),
		  S_( S ) {}
		void reset( bso::bool__ P = true )
		{
			dNotes::reset( P );
			S_.Anacrousis.reset( P );
		}
		void plug( bch::sHook &Hook )
		{
			dNotes::plug( Hook );
		}
		void plug( qASd *AS )
		{
			dNotes::plug( AS );
		}
		dMelody &operator =( const dMelody &M )
		{
			dNotes::operator=( M );

			S_.Anacrousis = M.S_.Anacrousis;

			return *this;
		}
		void Init( void )
		{
			dNotes::Init();
			S_.Anacrousis.Init();
		}
		bso::bool__ MarkAsAnacrousis( err::handling__ Handling = err::h_Default );	// Marque les notes d�j� introduite en tant qu'anacrouse.
		qRODISCLOSEd( sAnacrousis, Anacrousis );
	};

	qW( Melody );

	void SplitToMatchBars(
		const dMelody &Source,
		dMelody &Target );

  void UpdateTimeSignature(
    const sSignatureTime &Signature,
    dMelody &Melody,
    sRow First = qNIL,
    sRow Last = qNIL);

	enum write_status__ {
		wsOK,
		wsBarChekError,
		s_amount,
		s_Undefined
	};

	write_status__ WriteXML(
		const dMelody &Melody,
		eAccidental Accidental,  // Indicates which accidental to use as alteration in C key.
		xml::rWriter &Writer );

	enum parse_status__ {
		psOK,
		psXML,	// Erreur dans le flux XML.
		psUnexpectedTag,
		psUnexpectedAttribute,
		psUnexpectedValue,
		psBadValue,
		psAlreadyDefined,
		psMissingSignature,
		psMissingSignatureKey,
		psMissingSignatureTime,
		psMissingSignatureTimeRawNumerator,
		psMissingSignatureTimeDenominatorPower,
		psMissingTupletNumerator,
		psMissingTupletDenominator,
		psMissingPitch,
		psMissingPitchName,
		psMissingPitchAccidental,
		psMissingPitchOctave,
		psMissingDuration,
		psMissingDurationBase,
		psMissingDurationModifier,
		psMissingAnacrousisBase,
		psMissingAnacrousisAmount,
		ps_amount,
		ps_Undefined
	};

	parse_status__ ParseXML(
		xml::parser___ &Parser,
		dMelody &Melody,
		bso::bool__ WithRoot );	// Si � 'true', la prochaine balise est 'Melody', sinon, on est � l'int�rieur de 'Melody'. Dans les deux cas, au rerour on est � l'ext�rieur de la balise 'Melody'.

}

#endif
