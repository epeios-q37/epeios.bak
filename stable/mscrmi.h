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

//	$Id: mscrmi.h,v 1.15 2011/05/03 19:57:36 csimon Exp $

#ifndef MSCRMI__INC
#define MSCRMI__INC

#define MSCRMI_NAME		"MSCRMI"

#define	MSCRMI_VERSION	"$Revision: 1.15 $"

#define MSCRMI_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( MSCRMI_NODBG )
#define MSCRMI_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.15 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2011/05/03 19:57:36 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D MuSiC Roland MIDI Implementation 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "str.h"
#include "ctn.h"
#include "lstctn.h"
#include "xml.h"
#include "mscmdm.h"
#include "lcl.h"

namespace mscrmi {
	typedef str::string_	label_;
	typedef str::string		label;

	typedef bso::char__ datum__;

	typedef str::string_ data_;
	typedef str::string	data;

	qROW( row__ );

	typedef bso::ulong__ xaddress__;	// 'eXtended address'.
										// 0 - 29 : adresse du paramtre.
										// 30 - 31 : 0, 1, 2 : taille du 'stencil', en mots; 3 la taille du 'stencil' est de 23 bits.
	typedef bso::ulong__ address__;
	typedef bso::ulong__ segment__;
	typedef bso::ulong__ offset__;	
	typedef bso::ulong__ mask__;
	typedef bso::ulong__ stencil__;

	typedef bso::ushort__ size__;
	#define MSCRMI_SIZE_MAX	BSO_USHORT_MAX

	address__ Sum(
		address__ Address,
		size__ Size );

	bso::bool__ Append(
		bso::ulong__ Value,
		size__ Size,
		data_ &Data );

	struct bloc__ {
		address__ Address;
		size__ Size;
	};

	typedef bch::E_BUNCH_( bloc__ ) blocs_;
	E_AUTO( blocs );

#define MSCRMI_UNDEFINED_ADDRESS	BSO_ULONG_MAX

	// 'Assigned data'
	class adata_
	: public data_
	{
	public:
		struct s
		: public data_::s
		{
			//o The base address.
			address__ Address;
		} &S_;
		adata_( s &S )
		: S_( S ),
		  data_( S )
		{
		}
		void reset( bso::bool__ P = true )
		{
			S_.Address = MSCRMI_UNDEFINED_ADDRESS;
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
		adata_ &operator =( const adata_ &AD )
		{
			S_.Address = AD.S_.Address;

			data_::operator =( AD );

			return *this;
		}
		//f Initialisation.
		void Init( void )
		{
			S_.Address = MSCRMI_UNDEFINED_ADDRESS;

			data_::Init();
		}
		void Init(
			const str::string_ &Value,
			address__ Address = MSCRMI_UNDEFINED_ADDRESS )
		{
			data_::Init( Value );
			S_.Address = Address;
		}
		E_RWDISCLOSE_( address__, Address );
	};

	E_AUTO( adata )

	void Print(
		const adata_ &Data,
		txf::text_oflow__ &Flow );

	typedef ctn::E_XMCONTAINER_( adata_ ) adata_set_;
	E_AUTO( adata_set );

	void Print(
		const adata_set_ &DataSet,
		txf::text_oflow__ &Flow );

#define MSCRMI_MASK( size, pos )	( ( ~0UL >> ( 32 - ( size ) ) ) << ( pos ) )


// Un 'stencil' dsigne le masque  appliquer sur un 'xaddress__' pour en rcuprer le segment.
#define MSCRMI_STENCIL_MASK_LENGTH		2
#define MSCRMI_STENCIL_MASK_POSITION	( 32 - MSCRMI_STENCIL_MASK_LENGTH )
#define MSCRMI_STENCIL_MASK				MSCRMI_MASK( MSCRMI_STENCIL_MASK_LENGTH, MSCRMI_STENCIL_MASK_POSITION )

#define MSCRMI_ADDRESS_MASK			( ~MSCRMI_STENCIL_MASK )

#define MSCRMI__EMBED_VALUE( address, mask, position, value )	( ( address & ~mask ) | ( 

	inline xaddress__ _EmbedValue(
		xaddress__ Address,
		mask__ Mask,
		bso::ubyte__ Position,
		bso::ubyte__ Value )
	{
		return ( Address & ~Mask ) | ( Value << Position );
	}

	inline xaddress__ _SetStencilSize(
		xaddress__ Address,
		bso::ubyte__ Size )
	{
#ifdef MSCRMI_DBG
		if ( Size > 3 )
			ERRu();
#endif
		return _EmbedValue( Address, MSCRMI_STENCIL_MASK, MSCRMI_STENCIL_MASK_POSITION, Size );
	}

	inline bso::ubyte__ _GetEmbedValue(
		xaddress__ Address,
		mask__ Mask,
		bso::ubyte__ Position )
	{
		return (bso::ubyte__)( ( Address & Mask ) >> Position );
	}

	inline bso::ubyte__ _StencilSize( xaddress__ Address )
	{
		return _GetEmbedValue( Address, MSCRMI_STENCIL_MASK, MSCRMI_STENCIL_MASK_POSITION );
	}

	inline mask__ _Stencil( bso::ubyte__ Size )
	{
		switch ( Size ) {
		case 0:
		case 1:
		case 2:
			return ~MSCRMI_MASK( 32 - ( Size << 3 ), 0 );
			break;
		case 3:
			return 0Xfffffe00;
			break;
		default:
			ERRu();
			break;
		}

		return 0;	// Pour viter un 'warning'.
	}

	inline mask__ _Stencil( xaddress__ Address )
	{
		return _Stencil( _StencilSize( Address ) );
	}

	inline address__ _Address( xaddress__ Address )
	{
		return Address & ( Address == MSCRMI_UNDEFINED_ADDRESS ? 0xffffffff : MSCRMI_ADDRESS_MASK );
	}

	inline segment__ _Segment( xaddress__ Address )
	{
		return _Address( Address ) & _Stencil( Address );
	}

	inline offset__ _Offset( xaddress__ Address )
	{
		return _Address( Address ) & ~_Stencil( Address );
	}

	enum type__ {
		tNibble2,	// Donne brute, de taille 2 x 4 bits rparties sur 2 octets.
		tNibble3,	// Donne brute, de taille 3 x 4 bits rparties sur 3 octets.
		tNibble4,	// Donne brute, de taille 4 x 4 bits, rpartis sur 4 octets.
		tOctet,		// Donne brute, de 7 bits (8 me bit rserv par le protocole MIDI)
		tOctet2,	// Donne brute, de 14 bits (8 me bit de chaque octet rserv par le protocole MIDI)
		tOctet3,	// Donne brute, de 21 bits (8 me bit de chaque octet rserv par le protocole MIDI)
		tOctet4,	// Donne brute, de 28 bits (8 me bit de chaque octet rserv par le protocole MIDI)
		tName,		// Nom, de taille 16 octets.
		tHBars,		// 'Harmonic Bars' pour des rglages type orgue Hammond.
		t_amount,
		t_Undefined,
		t_Group,	// Signale un groupe de paramtre.
	};

	const char *Label( type__ type );

	size__ Size( type__ Type );

	class _parameter_core_
	{
	public:
		struct s {
			label_::s Label;
			xaddress__ Address;
			type__ Type;
		} &S_;
		label_ Label;
		_parameter_core_( s &S )
		: S_( S ),
		  Label( S.Label )
		{}
		void reset( bso::bool__ P = true )
		{
			Label.reset( P );
			S_.Address = MSCRMI_UNDEFINED_ADDRESS;
			S_.Type = t_Undefined;
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Label.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Label.plug( MM );
		}
		_parameter_core_ &operator =( const _parameter_core_ &PC )
		{
			Label = PC.Label;
			S_.Address = PC.S_.Address;
			S_.Type = PC.S_.Type;

			return *this;
		}
		void Init( void )
		{
			Label.Init();
			S_.Address = MSCRMI_UNDEFINED_ADDRESS;
			S_.Type = t_Undefined;
		}
		void Init(
			const label_ &Label,
			xaddress__ Address,
			type__ Type )
		{
			this->Label.Init( Label );
			S_.Address = Address;
			S_.Type = Type;
		}
		address__ Address( void ) const
		{
			return _Address( S_.Address );
		}
		segment__ Segment( void ) const
		{
			return _Segment( S_.Address );
		}
		offset__ Offset( void ) const
		{
			return _Offset( S_.Address );
		}
		size__ Size( void ) const
		{
			return mscrmi::Size( S_.Type );
		}
		E_RODISCLOSE_( type__, Type );
	};

	E_AUTO( _parameter_core );



	class parameter_
	: public _parameter_core_
	{
	public:
		struct s
		: public _parameter_core_::s
		{
			row__ GroupRow;	// Position du groupe de paramtres (paramtre de taille nulle) auquel il appartient.
		} &S_;
		parameter_( s &S )
		: S_( S ),
		  _parameter_core_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			_parameter_core_::reset( P );
			S_.GroupRow = NONE;
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			_parameter_core_::plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			_parameter_core_::plug( MM );
		}
		parameter_ &operator =( const parameter_ &P )
		{
			_parameter_core_::operator =( P );
			S_.GroupRow = P.S_.GroupRow;

			return *this;
		}
		void Init( void )
		{
			_parameter_core_::Init();
			S_.GroupRow = NONE;
		}
		void Init(
			const label_ &Label,
			xaddress__ Address,
			type__ Type,
			row__ GroupRow )
		{
			_parameter_core_::Init( Label, Address, Type );
			S_.GroupRow = GroupRow;
		}
		void Init(
			const _parameter_core_ &Core,
			row__ GroupRow )
		{
			_parameter_core_::Init();
			_parameter_core_::operator =( Core );

			S_.GroupRow = GroupRow;
		}
		E_RODISCLOSE_( row__, GroupRow )
	};

	E_AUTO( parameter )

	typedef lstctn::E_LXMCONTAINERt_( parameter_, row__ ) parameters_;
	E_AUTO( parameters );

	row__ Search(
		address__ Address,
		row__ Current,	// Si == 'NONE', on part du premier lment de 'Parameters'.
		const parameters_ &Parameters );

	void Print(
		const parameters_ &Parameters,
		xml::writer_ &Writer );

	typedef bso::char__ device_family__[4];
	typedef bso::char__ software_revision__[4];

#define MSCRMI_UNDEFINED_DEVICE_FAMILY	"void"
#define MSCRMI_UNDEFINED_MODEL_ID	"void"

	typedef bso::char__ model_id__[4];

	class midi_implementation_
	{
	public:
		struct s
		{
			model_id__ ModelID;
			bso::ubyte__ ModelIDLength;
			str::string_::s ModelLabel;
			parameters_::s Parameters;
			device_family__ DeviceFamily;
		} &S_;
		str::string_ ModelLabel;
		parameters_ Parameters;
		midi_implementation_( s &S )
		: S_( S ),
		  ModelLabel( S.ModelLabel ),
		  Parameters( S.Parameters )
		{}
		void reset( bso::bool__ P = true )
		{
			ModelLabel.reset( P );
			Parameters.reset( P );

			memcpy( S_.DeviceFamily, MSCRMI_UNDEFINED_DEVICE_FAMILY, sizeof( S_.DeviceFamily ) );
			S_.ModelIDLength = 0;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			ModelLabel.plug( MM );
			Parameters.plug( MM );
		}
		midi_implementation_ &operator=( const midi_implementation_ &MI )
		{
			ModelLabel = MI.ModelLabel;
			Parameters = MI.Parameters;

			memcpy( S_.DeviceFamily, MI.S_.DeviceFamily, sizeof( S_.DeviceFamily ) );

			memcpy( S_.ModelID, MI.S_.ModelID, sizeof( S_.ModelID ) );
			S_.ModelIDLength = MI.S_.ModelIDLength;

			return *this;
		}
		void Init( void )
		{
			ModelLabel.Init();
			Parameters.Init();

			memcpy( S_.DeviceFamily, "void", sizeof( S_.DeviceFamily ) );

			S_.ModelIDLength = 0;
		}
		const device_family__ &DeviceFamily( void ) const
		{
			return S_.DeviceFamily;
		}
	};

	E_AUTO( midi_implementation );

	// Donnet les blocs d'adresses contiges d'une implmentation MIDI.
	void GetBlocs(
		const parameters_ &Parameters,
		blocs_ &Blocs );

	inline void GetBlocs(
		const midi_implementation_ &Implementation,
		blocs_ &Blocs )
	{
		GetBlocs( Implementation.Parameters, Blocs );
	}

	const str::string_ &ToString(
		const bso::char__ (&Value)[4],
		str::string_ &Target );

	void Print(
		const midi_implementation_ &Implementation,
		xml::writer_ &Writer );

	enum parse_status__ {
		psOK,
		psParserError,	// Une erreur s'est produite durant le 'parsage'. Consulter le 'parser' pour connatre l'erreur.
		psUnexpectedTag,
		psAttributeAlreadyDefined,
		psUnexpectedAttribute,
		psMissingAttribute,
		psBadValue,
		ps_amount,
		ps_Undefined
	};

	const char *Label( parse_status__ Status );

	inline const char *Translate(
		parse_status__ Status,
		const lcl::rack__ &LocaleRack,
		STR_BUFFER___ &Buffer );

	const str::string_ &GetTranslation(
		parse_status__ Status,
		const lcl::rack__ &LocaleRack,
		const xtf::coord__ &Coord,
		const str::string_ &FileName,
		str::string_ &Translation );


	parse_status__ Parse(
		xml::parser___ &Parser,
		midi_implementation_ &Implementation );

	parse_status__ Parse(
		xml::parser___ &Parser,
		const midi_implementation_ &Implementation,
		adata_set_ &DataSet );

	typedef ctn::E_XCONTAINER_( midi_implementation_ ) midi_implementations_;
	E_AUTO( midi_implementations );

	void Print(
		const midi_implementations_ &Implementations,
		xml::writer_ &Writer );

	parse_status__ Parse(
		xml::parser___ &Parser,
		midi_implementations_ &Implementations );

	typedef bso::ubyte__ device_id__;

#	define MSCRMI_UNDEFINED_DEVICE_ID	128		// Une donne transmise  travers un canal MIDI ne peut tre > 127.
#	define MSCRMI_DEFAULT_DEVICE_ID		0x10	// Rglage d'usine.

	struct identity__
	{
		device_id__ DeviceID;
		model_id__ ModelID;
		bso::ubyte__ ModelIDLength;
		identity__( void )
		{
			DeviceID = MSCRMI_UNDEFINED_DEVICE_ID;
			ModelIDLength = 0;
		}
	};

	inline void Set(
		device_id__ DeviceId,
		const midi_implementation_ &Implementation,
		identity__ &Identity )
	{
		if ( Implementation.S_.ModelIDLength == 0 )
			ERRu();

		Identity.DeviceID = DeviceId;
		memcpy( Identity.ModelID, Implementation.S_.ModelID, Implementation.S_.ModelIDLength );
		Identity.ModelIDLength = Implementation.S_.ModelIDLength;
	}

	void RequestData(
		const identity__ &Identify,
		address__ Address,
		size__ Size,
		flw::oflow__ &Flow );

	class setting_ {
	public:
		struct s {
			row__ Row;
			data_::s Data;
		} &S_;
		data_ Data;
		setting_( s &S )
		: S_( S ),
		Data( S.Data )
		{}
		void reset( bso::bool__ P = true )
		{
			Data.reset( P );
			S_.Row = NONE;
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Data.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Data.plug( MM );
		}
		setting_ &operator =( const setting_ &S )
		{
			S_.Row = S.S_.Row;
			Data = S.Data;

			return *this;
		}
		void Init( row__ Row = NONE )
		{
			Data.Init();
			S_.Row = Row;
		}
		void Ini(
			row__ Row,
			const data_ &Data )
		{
			S_.Row = Row;
			this->Data.Init( Data );
		}
		E_RODISCLOSE_( row__, Row )
	};

	E_AUTO( setting );

	typedef ctn::E_XMCONTAINER_( setting_ ) settings_;
	E_AUTO( settings );

	void Fill(
		const adata_set_ &DataSet,
		const midi_implementation_ &Implementation,
		settings_ &Settings );

	void Print(
		const settings_ &Settings,
		const parameters_ &Parameters,
		xml::writer_ &Writer );

	inline void Print(
		const settings_ &Settings,
		const midi_implementation_ &Implementation,
		xml::writer_ &Writer )
	{
		tol::buffer__ Buffer;

		Writer.PushTag( "Settings" );
		Writer.PutAttribute( "TimeStamp", tol::DateAndTime( Buffer ) );
		Writer.PutAttribute( "ModelLabel", Implementation.ModelLabel );

		Print( Settings, Implementation.Parameters, Writer );

		Writer.PopTag();
	}

	enum transmission_status__ {
		tsOK,
		tsChecksumError,
		tsIncorrectData,
		tx_amount,
		ts_Undefined
	};

	const char *Label( transmission_status__ Status );

	inline const str::string_ &GetTranslation(
		transmission_status__ Status,
		const lcl::rack__ &LocaleRack,
		str::string_ &Translation  )
	{
		LocaleRack.GetTranslation( Label( Status ), "ERMI_", Translation );

		return Translation;
	}

	transmission_status__ GetDeviceFamilyAndSoftwareRevision(
		device_id__ Id,
		flw::ioflow__ &Flow,
		device_family__ &DeviceFamily,
		software_revision__ &SoftwareRevision );

	transmission_status__ Extract(
		flw::iflow__ &Flow,
		const identity__ &Identity,
		mscmdm::origin__ Origin,
		address__ &Address,
		data_ &Data );


	transmission_status__ Retrieve(
		flw::ioflow__ &Flow,
		address__ Address,
		size__ Size,
		const identity__ &Identity,
		adata_ &Data );

	transmission_status__ Retrieve(
		flw::ioflow__ &Flow,
		const blocs_ &Blocs,
		const identity__ &Identity,
		adata_set_ &Data );

	epeios::row__ GetCorrespondingMIDIImplementation(
		const device_family__ &DeviceFamily,
		const midi_implementations_ &Implementations );

	void Send(
		const adata_ &Data,
		const identity__ &Identity,
		flw::oflow__ &Flow );

	void Send(
		const adata_set_ &DataSet,
		const identity__ &Identity,
		flw::oflow__ &Flow );


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
