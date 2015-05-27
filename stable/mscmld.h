/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#ifndef MSCMLD__INC
# define MSCMLD__INC

# define MSCMLD_NAME		"MSCMLD"

# if defined( E_DEBUG ) && !defined( MSCMLD_NODBG )
#  define MSCMLD_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// MuSiC MeLody Description

# include "err.h"
# include "flw.h"
# include "bch.h"
# include "xml.h"
# include "bso.h"

namespace mscmld {

	inline bso::u8__ Log2Abs_( bso::s8__ Value )
	{
		if ( Value < 0 )
			Value = -Value;

		switch ( Value ) {
		case 0:
			ERRFwk();
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
			ERRFwk();
			break;
		}

		return 0;	// To avoid a warning;
	}

	enum pitch_name__ {
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

	const char *GetPitchNameLabel( pitch_name__ Name );

	enum pitch_accidental__ {
		paFlat,
		paNatural,
		paSharp,
		pa_amount,
		pa_Undefined
	};

	const char *GetPitchAccidentalLabel( pitch_accidental__ Accidental );

	typedef bso::u8__ pitch_octave__;
#define MSCMLD_UNDEFINED_PITCH_OCTAVE BSO_U8_MAX

	struct pitch__ {
		pitch_name__ Name;
		pitch_accidental__ Accidental;
		pitch_octave__ Octave;
		void reset( bso::bool__ P = true )
		{
			Name = pn_Undefined;
			Accidental = pa_Undefined;
			Octave = MSCMLD_UNDEFINED_PITCH_OCTAVE;
		}
		pitch__( void )
		{
			reset( false );
		}
		pitch__(
			pitch_octave__ Octave,
			pitch_name__ Name,
			pitch_accidental__ Accidental = paNatural )
		{
			reset( false );

			this->Name = Name;
			this->Accidental = Accidental;
			this->Octave = Octave;
		}
		pitch__(
			pitch_name__ Name,
			pitch_octave__ Octave,
			pitch_accidental__ Accidental = paNatural )
		{
			reset( false );

			this->Name = Name;
			this->Accidental = Accidental;
			this->Octave = Octave;
		}
		pitch__(
			pitch_name__ Name,
			pitch_accidental__ Accidental,
			pitch_octave__ Octave )
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
				   && ( ( ( Accidental != pa_Undefined )
				          && ( Octave != MSCMLD_UNDEFINED_PITCH_OCTAVE ) )
				        || ( Name == pnRest ) );
		}
		bso::u8__ GetChromatic( void ) const;
	};

	inline int operator ==(
		const pitch__ &Op1,
		const pitch__ &Op2 )
	{
		return ( Op1.Name == Op2.Name ) && ( Op1.Accidental == Op2.Accidental ) && ( Op1.Octave == Op2.Octave );
	}

	inline int operator !=(
		const pitch__ &Op1,
		const pitch__ &Op2 )
	{
		return ( Op1.Name != Op2.Name ) || ( Op1.Accidental != Op2.Accidental ) || ( Op1.Octave != Op2.Octave );
	}

	E_ROW( prow__ );

	typedef bch::E_BUNCHt_( pitch__, prow__ ) pitches_;
	E_AUTO( pitches )

# define MSCMLD_UNDEFINED_TUPLET_NUMERATOR		BSO_U8_MAX
# define MSCMLD_UNDEFINED_TUPLET_DENOMINATOR	BSO_U8_MAX

	struct tuplet__ {
		bso::u8__ Numerator;
		bso::u8__ Denominator;
		void reset( bso::bool__ = true )
		{
			Numerator = MSCMLD_UNDEFINED_TUPLET_NUMERATOR;
			Denominator = MSCMLD_UNDEFINED_TUPLET_DENOMINATOR;
		}
		E_CDTOR( tuplet__ );
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
		tuplet__ Op1,
		tuplet__ Op2 )
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
		tuplet__ Op1,
		tuplet__ Op2 )
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

	struct duration__
	{
		bso::s8__ Base;	// 1 : whole, 2 : half, 3 : quarter, 4 : eight, ... -1 : double whole ; -2 : quadruple whole...
		bso::u8__ Modifier;	// Amount of dot.
		tuplet__ Tuplet;
		bso::bool__ TiedToNext;
		void reset( bso::bool__ P = true )
		{
			Base = MSCMLD_UNDEFINED_DURATION_BASE;
			Modifier = MSCMLD_UNDEFINED_DURATION_MODIFIER;
			Tuplet.reset( P );
			TiedToNext = false;
		}
		duration__( void )
		{
			reset( false );
		}
		duration__(
			bso::s8__ Base,
			bso::u8__ Modifier = 0,
			tuplet__ Tuplet = tuplet__(),
			bso::bool__ TiedToNext = false )
		{
			Init( Base, Modifier, TiedToNext, Tuplet );
		}
		duration__(
			bso::s8__ Base,
			bso::u8__ Modifier,
			bso::bool__ TiedToNext,
			tuplet__ Tuplet = tuplet__() )
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
			tuplet__ Tuplet = tuplet__() )
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
			tuplet__ Tuplet = tuplet__(),
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
		duration__ Op1,
		duration__ Op2 )
	{
		return ( ( Op1.Base == Op2.Base ) && ( Op1.Modifier == Op2.Modifier ) );
	}

	inline int operator !=(
		duration__ Op1,
		duration__ Op2 )
	{
		return ( ( Op1.Base != Op2.Base ) || ( Op1.Modifier != Op2.Modifier ) );
	}

	E_ROW( drow__ );

	typedef bch::E_BUNCHt_( duration__, drow__ ) durations_;
	E_AUTO( durations )

	typedef bso::s8__ signature_key__;	// -1: 1 flat, -2, 2 flats, ..., 0 : C key, 1 : 1 sharp, 2 : 2 sharps, ...
#define MSCMLD_UNDEFINED_KEY_SIGNATURE	BSO_S8_MAX

	inline void Initialize( signature_key__ &Key )
	{
		Key = MSCMLD_UNDEFINED_KEY_SIGNATURE;
	}

	inline bso::bool__ IsValid( signature_key__ Key )
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

	struct signature_time__ {
	private:
		numerator__ _Numerator;	// Si < 0, le dnominateur (bien le dnominateur) n'est pas cens tre affich.
		denominator_power__ _DenominatorPower;	// Le dnominateur rel est gal  = 2 ^ abs('DenominatorPower').
		void _Test( void ) const
		{
			if ( !IsValid() )
				ERRFwk();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Numerator = MSCMLD_UNDEFINED_TIME_SIGNATURE_NUMERATOR;
			_DenominatorPower = MSCMLD_UNDEFINED_TIME_SIGNATURE_DENOMINATOR_POWER;
		}
		E_CVDTOR( signature_time__ );
		signature_time__ (
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
				ERRFwk();

			if ( Numerator == 0 )
				ERRFwk();

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
		const signature_time__ &Op1,
		const signature_time__ &Op2 )
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
		const signature_time__ &Op1,
		const signature_time__ &Op2 )
	{
		return !operator==( Op1, Op2 );
	}

	struct signature__ {
		signature_key__ Key;
		signature_time__ Time;
		void reset( bso::bool__ P = true )
		{
			Key = MSCMLD_UNDEFINED_KEY_SIGNATURE;
			Time.reset( P );
		}
		signature__( void )
		{
			reset( false );
		}
		signature__(
			const signature_key__ &Key,
			const signature_time__ &Time )
		{
			reset( false );

			this->Key = Key;
			this->Time = Time;
		}
		signature__(
			const signature_time__ &Time,
			const signature_key__ &Key )
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
		const signature__ &Op1,
		const signature__ &Op2 )
	{
		return ( ( Op1.Key == Op2.Key ) && ( Op1.Time == Op2.Time ) );
	}

	inline int operator !=(
		const signature__ &Op1,
		const signature__ &Op2 )
	{
		return ( ( Op1.Key != Op2.Key ) || ( Op1.Time != Op2.Time ) );
	}

	E_ROW( srow__ );

	typedef bch::E_BUNCHt_( signature__, srow__ ) signatures_;
	E_AUTO( signatures )


	struct note__ {
		pitch__ Pitch;
		duration__ Duration;
		signature__ Signature;
		void reset( bso::bool__ P = true )
		{
			Pitch.reset( P );
			Duration.reset( P );
			Signature.reset( P );
		}
		note__( void )
		{
			reset( false );
		}
		note__(
			const pitch__ &Pitch,
			const duration__ &Duration,
			const signature__ &Signature )
		{
			reset( false );

			this->Pitch = Pitch;
			this->Duration = Duration;
			this->Signature = Signature;
		}
		void Init( void )
		{
			Pitch.Init();
			Duration.Init();
			Signature.Init();
		}
		bso::bool__ IsValid( void ) const
		{
			return ( Pitch.IsValid() && Duration.IsValid() && Signature.IsValid() );
		}
	};

	E_ROW( row__ );

	typedef bch::E_BUNCHt_( note__, row__ ) notes_;
	E_AUTO( notes );

	void Merge(
		const pitches_ &Pitches,
		const durations_ &Durations,
		const signatures_ &Signatures,
		notes_ &Notes );

	E_CDEF( bso::u8__, AnacrousisUndefinedBase, 0 );

	class anacrousis__ {
	public:
		bso::u8__ Base;	// 0: Undefined1: noire, 2: croche, 3: double-croche, ...
		bso::u8__ Amount;
		void reset( bso::bool__ = true )
		{
			Base = AnacrousisUndefinedBase;
			Amount = 0;
		}
		E_CDTOR( anacrousis__);
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

	class melody_ 
	: public notes_
	{
	public:
		struct s
		: public notes_::s
		{
			anacrousis__ Anacrousis;
		} &S_;
		melody_( s &S )
		: S_( S ),
		  notes_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			notes_::reset( P );
			S_.Anacrousis.reset( P );
		}
		void plug( sdr::E_SDRIVER__ &SD )
		{
			notes_::plug( SD );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			notes_::plug( AS );
		}
		melody_ &operator =( const melody_ &M )
		{
			notes_::operator=( M );

			S_.Anacrousis = M.S_.Anacrousis;

			return *this;
		}
		void Init( void )
		{
			notes_::Init();
			S_.Anacrousis.Init();
		}
		bso::bool__ MarkAsAnacrousis( err::handling__ Handling = err::h_Default );	// Marque les notes dj introduite en tant qu'anacrouse.
		E_RODISCLOSE_( anacrousis__, Anacrousis );
	};

	E_AUTO( melody );

	void SplitToMatchBars(
		const melody_ &Source,
		melody_ &Target );

	enum write_status__ {
		wsOK,
		wsBarChekError,
		s_amount,
		s_Undefined
	};

	write_status__ WriteXML(
		const melody_ &Melody,
		xml::writer_ &Writer );

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
		melody_ &Melody,
		bso::bool__ WithRoot );	// Si  'true', la prochaine balise est 'Melody', sinon, on est  l'intrieur de 'Melody'. Dans les deux cas, au rerour on est  l'extrieur de la balise 'Melody'.

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
