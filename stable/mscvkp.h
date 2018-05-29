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

//	$Id: mscvkp.h,v 1.1 2010/07/16 08:25:54 csimon Exp $

#ifndef MSCVKP__INC
#define MSCVKP__INC

#define MSCVKP_NAME		"MSCVKP"

#define	MSCVKP_VERSION	"$Revision: 1.1 $"

#define MSCVKP_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#if defined( E_DEBUG ) && !defined( MSCVKP_NODBG )
#define MSCVKP_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.1 $
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date: 2010/07/16 08:25:54 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D MuSiC VK (from Roland VK combo organ) Parameters 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bso.h"
#include "bch.h"
#include "ctn.h"
#include "mscmdm.h"

namespace mscvkp {

	//t The type of a datum.
	typedef bso::char__ datum__;

#define MSCVKP_UNDEFINED_DATUM	(bso::char__)0xff	// Dans la norme Midi, '0xff' n'est pas une valeur de donne autorise.

	//t The type of an Address.
	typedef bso::ulong__ address__;

	//d An undefined Address.
	#define MSCVKP_UNDEFINED_ADDRESS	BSO_ULONG_MAX

	#define MSCVKP_LEVEL_MAX	8

	//t The type of data.
/*
	typedef bch::E_BUNCH_( datum__ )	data_;
	typedef bch::E_BUNCH( datum__ )		data;
*/
	typedef str::string_	data_;
	typedef str::string		data;

	//c A data set.
	class data_set_
	: public data_
	{
	public:
		struct s
		: public data_::s
		{
			//o The base address.
			address__ Address;
		} &S_;
		data_set_( s &S )
		: S_( S ),
		  data_( S )
		{
		}
		void reset( bso::bool__ P = true )
		{
			if ( P )
				S_.Address = MSCVKP_UNDEFINED_ADDRESS;

			data_::reset( P );
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			data_::plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY &MM )
		{
			data_::plug( MM );
		}
		data_set_ &operator =( const data_set_ &DS )
		{
			S_.Address = DS.S_.Address;

			data_::operator =( DS );

			return *this;
		}
		//f Initialisation.
		void Init( void )
		{
			S_.Address = MSCVKP_UNDEFINED_ADDRESS;

			data_::Init();
		}
		E_RWDISCLOSE_( address__, Address );
	};

	E_AUTO( data_set )

	inline bso::bool__ IsSystem( address__ Address )
	{
		return 	Address <=0x24;
	}

	inline bso::bool__ IsSystemBasic( address__ Address )
	{
		return IsSystem( Address ) && ( Address <= 0x0d );
	}

	inline bso::bool__ IsSystemMidi( address__ Address )
	{
		return IsSystem( Address ) && ( Address >= 0x0e );
	}

	inline bso::bool__ IsOrchestralVoice( address__ Address )
	{
		return ( Address >> 24 == 0x20 );
	}

	inline bso::bool__ IsTemporary( address__ Address )
	{
		return ( ( Address >> 16 ) == 0x0100 );
	}

	inline bso::bool__ IsRegistration( address__ Address )
	{
		return ( ( Address >> 24 ) == 0x10 ) || IsTemporary( Address );
	}

	inline bso::bool__ IsName( address__ Address )
	{
		return 	IsRegistration( Address ) && ( ( Address & 0xffff ) <= 0x000a );
	}

	inline bso::bool__ IsOrganBasic( address__ Address )
	{
		return ( ( IsTemporary( Address ) || IsRegistration( Address ) ) && ( ( ( Address >> 8 ) & 0xff ) == 0x40 ) );
	}

	inline bso::bool__ IsPedalBar( address__ Address )
	{
		return IsOrganBasic( Address ) && ( ( ( Address &0xff ) == 0x25 ) || ( ( Address & 0xff ) == 0x26 ) );
	}

	inline bso::bool__ IsKeyboardBar( address__ Address )
	{
		return IsOrganBasic( Address ) && ( ( Address &0xff ) >= 0x13 ) && ( ( Address & 0xff ) <= 0x24 );
	}

	inline bso::bool__ IsPedalMixingBar( address__ Address )
	{
		return IsOrganBasic( Address ) && ( ( Address &0xff ) >= 0x27 ) && ( ( Address & 0xff ) <= 0x38 );
	}
	
	inline bso::bool__ IsBar( address__ Address )
	{
		return IsPedalBar( Address ) || IsKeyboardBar( Address ) || IsPedalMixingBar( Address );
	}

	//t Data sets.
	typedef ctn::E_XMCONTAINER_( data_set_ ) data_sets_;
	typedef ctn::E_XMCONTAINER( data_set_ ) data_sets;

	//f Fill 'DataSet' with 'IFlow'.
	void Fill(
		flw::iflow__ &IFlow,
		mscmdm::origin__ Origin,
		data_set_ &DataSet );

	void Fill(
		flw::iflow__ &IFlow,
		mscmdm::origin__ Origin,
		data_sets_ &DataSets );

	//f Return the checksum for 'DataSet'.
	datum__ Checksum( const data_set_ &DataSet );

	//c A description.	
	struct description__ {
	private:
		bso::char__	V_[35];
	public:
		void reset( bso::bool__ = true )
		{
			V_[0] = 0;
		}
		operator const char *( void ) const
		{
			return V_;
		}
		description__( const char *V = "" )
		{
			reset( false );
#ifdef MSCVKP_DBG
			if ( ( strlen( V ) + 1 ) >= sizeof( V_ ) )
				ERRl();
#endif
			strcpy( V_, V );
		}
		bool operator ==( const description__ &D ) const
		{
			return !strcmp( V_, D.V_ );
		}
		bool operator !=( const description__ &D ) const
		{
			return strcmp( V_, D.V_ ) != 0;
		}
	};	// The compiler has problem with 'typedef bso::char__ description__[20]'.

	//t Descriptions.
	typedef bch::E_BUNCH__( description__, MSCVKP_LEVEL_MAX ) descriptions__;

	//f Put the description corresponding to 'Address' in 'Descriptions'.
	void GetDescription(
		address__ Address,
		descriptions__ &Descriptions__,
		bso::bool__ Flat = false );

	//f Return the voice datum from 'Address'.
	inline datum__ GetVoiceDatum( address__ Address )
	{
		return (datum__)( ( Address >> 16 ) & 0xff );
	}

	//f Return the voice label from 'Address'.
	inline const char *GetVoiceLabel( address__ Address )
	{
		static char Buffer[4];

		sprintf( Buffer, "%i", GetVoiceDatum( Address ) + 1 );

		return Buffer;
	}

	//f Return the  registration datum from 'Address'.
	inline datum__ GetRegistrationDatum( address__ Address )
	{
		return (datum__)( ( Address >> 16 ) & 0x7f );
	}

	//f Return the registration label from 'Address'.
	inline const char *GetRegistrationLabel( address__ Address )
	{
		static char Buffer[4];
		datum__ Datum = GetRegistrationDatum( Address );

		sprintf( Buffer, "%c%i%i",( Datum >> 6 ) + 'A', ( ( Datum >> 3 ) & 0x7 ) + 1, ( Datum & 0x7 ) + 1 );

		return Buffer;
	}

	//f Convert 'Address' to a string.
	inline const char *AddressToString(
		address__ Address,
		bso::bool__ WithoutSpaces = false )
	{
		static char Buffer[13];

		if ( WithoutSpaces )
			sprintf( Buffer, "%.2X%.2X%.2X%.2X", Address >> 24, ( Address >> 16 ) & 0xff, ( Address >> 8 ) & 0xff, Address & 0xff );
		else
			sprintf( Buffer, "%.2X %.2X %.2X %.2X", Address >> 24, ( Address >> 16 ) & 0xff, ( Address >> 8 ) & 0xff, Address & 0xff );

		return Buffer;
	}

	//f Convert 'Datum' to a string.
	inline const char *DatumToString(
		datum__ Datum,
		bso::bool__ Hex)
	{
		static char Buffer[4];

		if ( Hex )
			sprintf( Buffer, "%.2X", Datum & 0xff );
		else
			strcpy( Buffer, bso::Convert( (bso::ubyte__)Datum ) );

		return Buffer;
	}

	// Retourne le nombre d'octets envoys.
	bso::size__ Send(
		const data_set_ &DataSet,
		flw::oflow__ &Flow );

	void Send(
		const data_sets_ &DataSets,
		flw::oflow__ &Flow );

	void BuildTrack(
		const data_sets_ &DataSets,
		mscmdm::track_ &Track );



#if 0

	//c A datum.
	struct datum__ {
	private:
		raw_datum__ Value_;
	public:
		datum__( void )
		{
			Value_ = 0;
		}
		//f Store 'Value', which must be < 128.
		void Value( raw_datum__ Value )
		{
#ifdef MSCVKP_DEBUG
			if ( Value & 0x80 )
				ERRu();
#endif
			Value_ = Value;
		}
		//f Return the current value, which much be previously set.
		raw_datum__ Value( void )
		{
#ifdef MSCVKP_DEBUG
			if ( !Is )
				ERRu();
#endif
			return 0x7f & Value_;
		}
		//f Return true if the value significative.
		bso::bool__ IsSignificative( void )
		{
			return ( Value_ & 0x80 ) != 0;
		}
	};

	struct organ_effect__ {
		struct vibrato {
			struct _switch {
				datum__
					Upper,
					Lower;
			} Switch;
			datum__
				Type,
				Vintage,
				Lower;
		} Vibrato;
		datum__ AmpSpeakerType;
		struct overdrive {
			datum__
				Character,
				Level;
		} Overdrive;
		struct equalize_gain {
			datum__
				Bass,
				Middle,
				Treble;
		} EqualizeGain;
		struct rotary {
			datum__
				Sound,
				SpeedSwitch,
				Rotation,
				Type;
			struct level {
				datum__
					Woofer,
					Tweeter;
			} Level;
			struct time {
				struct tweeter {
					datum__
						Rise,
						Fall;
				} Tweeter;
				struct woofer {
					datum__
						Rise,
						Fall;
				} Woofer;
			} Time;
			struct speed {
				struct tweeter {
					datum__
						Slow,
						Fast;
				} Tweeter;
				struct woofer {
					datum__
						Slow,
						Fast;
				} Woofer;
			} Speed;
			datum__ MicDistance;
			struct randomize {
				datum__
					Woofer,
					Tweeter;
			} Spread;
			datum__ Randomize;
		} Rotary;
		struct ring_modulator {
			datum__
				Switch,
				Mode,
				Frequency;
		} RingModulator;
	};

	struct organ_basic__ {
		datum__
			Level,
			WheelType,
			FoldBackSwitch,
			WheelLevelTable;
			struct key_click_level {
				datum__
					On,
					Off;
			} KeyClickLevel;
			struct percussion {
				datum__
					Switch,
					Harmonic,
					Soft,
					Decay;
				struct level {
					datum__
						Soft,
						Norm;
				} Level;
				struct decay_time {
					datum__ 
						Slow,
						Fast;
				} DecayTime;
				datum__
					RechargeTime,
					OneFeetCancel,
					HarmonicBarLevel,
					DirectSwitch;
			} Percussion;
			datum__
				Lower[9], Upper[9];
			struct pedal {
				datum__ HarmonicBar[2];
				datum__ Mix[2][9];
				struct sustain {
					datum__
						Switch,
						Time;
				} Sustain;
				struct attack {
					datum__
						Switch,
						Time,
						Level;
				} Attack;
			} Pedal;
		};

	struct registration_part__ {
		struct keyboard_assign {
			datum__
				Organ,
				Orch,
				Ext;
		} KeyboardAssign;
		datum__ OrganHoldSwitch;
		struct orch {
			datum__ PartLevel;
			struct _switch {
				datum__
					Hold,
					Bend,
					Modulation,
					Expression;
			} Switch;
			datum__ VoiceSelect;
			struct variation {
				datum__
					Stringc,
					Choir,
					Piano,
					Bass,
					Wind,
					Attack,
					Synth,
					Others;
			} variation;

#endif
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &TFlow,
	const mscvkp::descriptions__ &Descriptions );

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &TFlow,
	const mscvkp::data_set_ &DataSet );

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &TFlow,
	const mscvkp::data_sets_ &DataSets );

/*$END$*/
#endif
