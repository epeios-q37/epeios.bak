/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

#define MSCRMI__COMPILATION

#include "mscrmi.h"

#include "mscmdm.h"

#include "tht.h"
#include "cio.h"

using namespace mscrmi;

#define MESSAGE_PREFIX	"MSCRMI_"

using xml::parser___;

void mscrmi::Print(
	const adata_ &Data,
	txf::text_oflow__ &Flow )
{
	char Buffer[]= "12345678";
	epeios::row__ Row = Data.First();

	if ( sprintf( Buffer, "%08lX", Data.Address() ) < 0 )
		ERRs();

	Flow << Buffer << " : ";

	while ( Row != NONE ) {
		if ( sprintf( Buffer, "%02lX", (bso::ulong__)Data( Row ) ) < 0 )
			ERRs();

		Flow << Buffer << txf::pad;

		Row = Data.Next( Row );
	}
}

void mscrmi::Print(
	const adata_set_ &DataSet,
	txf::text_oflow__ &Flow )
{
	ctn::E_CMITEM( adata_ ) Data;
	epeios::row__ Row = DataSet.First();

	Data.Init( DataSet );

	while ( Row != NONE ) {
		Print( Data( Row ), Flow );

		Flow << txf::nl;

		Row = DataSet.Next( Row );
	}

}

static inline bso::ulong__ Encode_( bso::ulong__ Value )
// Encodage pour flux MIDI.
{
	return ( Value & 0x7f ) | ( ( Value & 0x3f80) << 1 ) | ( ( Value & 0x1fc000 ) << 2 ) | ( ( Value & 0xfe00000 ) << 3 );
}


static inline bso::ulong__ Decode_( bso::ulong__ Value )
// Decodage d'un flux MIDI.
{
	return ( Value & 0x7f ) + ( ( Value & 0x7f00 ) >> 1 ) + ( ( Value & 0x7f0000 ) >> 2 ) + ( ( Value & 0x7f000000 ) >> 3 );
}

address__ mscrmi::Sum(
	address__ Address,
	size__ Size )
{
	if ( ( Address & 0x80808080 ) != 0 )
		ERRu();

	return Encode_( Address = Decode_( Address ) + Size );
}

static inline xaddress__ Sum_(
	xaddress__ Address,
	size__ Size )
{
	return mscrmi::_SetStencilSize( Sum( mscrmi::_Address( Address ), Size ), mscrmi::_StencilSize( Address ) );
}

#define CASE( name )\
	case t##name:\
		return #name;\
	break


const char *mscrmi::Label( type__ Type )
{
	switch ( Type ) {
	CASE( Nibble2 );
	CASE( Nibble3 );
	CASE( Nibble4 );
	CASE( Octet );
	CASE( Octet2 );
	CASE( Octet3 );
	CASE( Octet4 );
	CASE( Name );
	CASE( HBars );
	default:
		ERRu();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}

#undef CASE

size__ mscrmi::Size( type__ Type )
{
	switch ( Type ) {
	case tOctet:
		return 1;
		break;
	case tNibble2:
	case tOctet2:
		return 2;
		break;
	case tNibble3:
	case tOctet3:
		return 3;
		break;
	case tNibble4:
	case tOctet4:
		return 4;
		break;
	case tName:
		return 16;
		break;
	case tHBars:
		return 9;
		break;
	case t_Group:
		ERRc();
		break;
	default:
		ERRc();
		break;
	}

	return 0;	// Pour viter un 'warning'.
}

void mscrmi::GetBlocs(
	const parameters_ &Parameters,
	blocs_ &Blocs )
{
	bloc__ Bloc;
	row__ Row = Parameters.First();
	ctn::E_CMITEMt( parameter_, row__ ) Parameter;

	Parameter.Init( Parameters );

	if ( Row != NONE ) {
		while ( ( Row != NONE ) && ( Parameter( Row ).Type() == t_Group ) )
			Row = Parameters.Next( Row );

		if ( Row != NONE ) {
			Bloc.Address = Parameter( Row ).Address();
			Bloc.Size = Parameter( Row ).Size();

			Row = Parameters.Next( Row );
		}
	}


	while ( Row != NONE ) {
		while ( ( Row != NONE ) && ( Parameter( Row ).Type() == t_Group ) )
			Row = Parameters.Next( Row );

		if ( Row != NONE ) {
			if ( Sum_( Bloc.Address, Bloc.Size ) == Parameter( Row ).Address() )
				Bloc.Size += Parameter( Row ).Size();
			else {
				Blocs.Append( Bloc );
				Bloc.Address = Parameter( Row ).Address();
				Bloc.Size = Parameter( Row ).Size();
			}
		}

		Row = Parameters.Next( Row );
	}

	Blocs.Append( Bloc );
}


static void PrintPath_(
	row__ Row,
	const parameters_ &Parameters,
	txf::text_oflow__ &Flow )
{
qRH
	str::string Path;
	ctn::E_CMITEMt( parameter_, row__ ) Parameter;
qRB
	Parameter.Init( Parameters );
	Path.Init();

	Row = Parameter( Row ).GroupRow();

	while ( Row != NONE ) {
		Path.Insert( '/', 0 );
		Path.Insert( Parameter( Row ).Label, 0 );

		Row = Parameter( Row ).GroupRow();
	}

	Flow << Path;

qRR
qRT
qRE
}

static inline void Print_(
	const parameter_ &Parameter,
	xml::writer_ &Writer )
{
	char Address[] = "12345678";

	Writer.PutAttribute( "Label", Parameter.Label );

	if ( sprintf( Address, "%0*lX", 2 * ( 4 - _StencilSize( Parameter.S_.Address ) ), Parameter.Offset() ) < 0 )
		ERRs();

	Writer.PutAttribute( "Address", Address );

	if ( Parameter.Type() != t_Group )
		Writer.PutAttribute( "Type", Label( Parameter.Type() ) );
/*
	sprintf( Address, "%0*lX", 2 * ( 4 - _StencilSize( Parameter.S_.Address ) ), Parameter.Offset() );
	Writer.PutAttribute( "Offset", Address );

	if ( Parameter.Size() != 0 )
		Writer.PutAttribute( "Size", bso::Convert( Parameter.Size(), Buffer ) );
*/
}

row__ mscrmi::Search(
	address__ Address,
	row__ Current,
	const parameters_ &Parameters )
{
	ctn::E_CMITEMt( parameter_, row__ ) Parameter;
	row__ &Row = Current;
	
	if ( Row == NONE )
		Row = Parameters.First();

	Parameter.Init( Parameters );

	while ( ( Row != NONE ) &&
			( ( Parameter( Row ).Address() < Address )
				|| ( ( Parameter( Row ).Type() ) == t_Group ) ) )
		Row = Parameters.Next( Row );

	if ( ( Row != NONE ) && ( Parameter( Row ).Address() > Address ) )
		Row = NONE;

	return Row;
}


typedef stk::E_BSTACK_( row__ ) srows_;
E_AUTO( srows );

void mscrmi::Print(
	const parameters_ &Parameters,
	xml::writer_ &Writer )
{
qRH
	srows Rows;
	row__ Row = NONE;
	row__ ParentRow = NONE;
	ctn::E_CMITEMt( parameter_, row__ ) Parameter;
qRB
	Parameter.Init( Parameters );
	Row = Parameters.First();

	Rows.Init();

	while ( Row != NONE ) {

		while ( Parameter( Row ).GroupRow() != ParentRow ) {
			Writer.PopTag();
			ParentRow = Rows.Pop();
		}

		if ( Parameter( Row ).Type() == t_Group ) {
			Rows.Push( ParentRow );
			ParentRow = Row;
			Writer.PushTag( "Parameters" );
		} else
			Writer.PushTag( "Parameter" );

		Print_( Parameter( Row ), Writer );

		if ( Parameter( Row ).Type() != t_Group )
			Writer.PopTag();

		Row = Parameters.Next( Row );
	}

	while ( Rows.Amount() != 0 ) {
		Rows.Pop();
		Writer.PopTag();
	}
qRR
qRT
qRE
}

#define CASE( label )	LCL_CASE( label, ps )

const char *mscrmi::Label( parse_status__ Status )
{
	switch ( Status ) {
		CASE( ParserError )
		CASE( UnexpectedTag )
		CASE( AttributeAlreadyDefined )
		CASE( UnexpectedAttribute )
		CASE( MissingAttribute )
		CASE( BadValue )
	default:
		ERRu();
		break;
	}

	return NULL;	// Pour viter un 'warning'.

}

#undef CASE

const char *mscrmi::Translate(
	parse_status__ Status,
	const lcl::rack__ &LocaleRack,
	STR_BUFFER___ &Buffer )
{
	return LocaleRack.GetTranslation( Label( Status ), MESSAGE_PREFIX, Buffer );
}

const str::string_ &mscrmi::GetTranslation(
	parse_status__ Status,
	const lcl::rack__ &LocaleRack,
	const xtf::coord__ &Coord,
	const str::string_ &FileName,
	str::string_ &Translation )
{
qRH
	str::string Message;
	STR_BUFFER___ SBuffer;
	bso::integer_buffer__ IBuffer;
	lcl::strings Values;
qRB
	Message.Init();

	if ( FileName.Amount() != 0 )
		LocaleRack.GetTranslation( "ErrorAtLineColumnInFile", MESSAGE_PREFIX, Message );
	else
		LocaleRack.GetTranslation( "ErrorAtLineColumn", MESSAGE_PREFIX, Message );

	Values.Init();
	Values.Append( str::string( bso::Convert( Coord.Line, IBuffer ) ) );
	Values.Append( str::string( bso::Convert( Coord.Column, IBuffer ) ) );
	Values.Append( FileName );

	lcl::ReplaceTags( Message, Values );

	Translation.Append( Message );

	Translation.Append( " : " );

	Message.Init( Translate( Status, LocaleRack, SBuffer ) );

	Translation.Append( Message );

qRR
qRT
qRE
	return Translation;
}



#undef CASE


static bso::ubyte__ Convert_(
	const str::string_ &Source,
	bso::char__ *Target )
{
	bso::size__ Amount = Source.Amount();
	bso::ulong__ Value;
	epeios::row__ Error = NONE;

	if ( Amount == 0 )
		return 0;

	if ( Amount & 1 )
		return 0;

	if ( Amount > 8 )
		return 0;

	Value = Source.ToUL( &Error, str::b16 );

	if ( Error != NONE )
		return 0;
#if 0
	switch ( Amount >> 1 ) {
	case 4:
		Target[3] = ( Value >> 24 ) & 0xff;
	case 3:
		Target[2] = ( Value >> 16 ) & 0xff;;
	case 2:
		Target[1] = ( Value >> 8 ) & 0xff;
	case 1:
		Target[0] = Value & 0xff;
		break;
	default:
		ERRc();
		break;
	}
#else
	switch ( Amount >> 1 ) {
	case 4:
		*Target = ( Value >> 24 ) & 0xff;
		Target++;
	case 3:
		*Target = ( Value >> 16 ) & 0xff;;
		Target++;
	case 2:
		*Target = ( Value >> 8 ) & 0xff;
		Target++;
	case 1:
		*Target = Value & 0xff;
		Target++;
		break;
	default:
		ERRc();
		break;
	}
#endif

	return (bso::ubyte__)( Amount >> 1);
}

static xaddress__ HandleAddress_(
	xaddress__ Address,
	const str::string_ &Value )
{
	epeios::row__ Error = NONE;
	address__ Long = Value.ToUL( &Error, str::b16 );
	bso::size__ Amount = Value.Amount();
	bso::ubyte__ StencilSize = 0;

	if ( Error != NONE )
		return MSCRMI_UNDEFINED_ADDRESS;

	if ( Amount == 0 )
		return MSCRMI_UNDEFINED_ADDRESS;

	if ( Amount > 8 )
		return MSCRMI_UNDEFINED_ADDRESS;

	if ( Amount & 1 )	// 'Amount' doit tre pair.
		return MSCRMI_UNDEFINED_ADDRESS;

	StencilSize = (bso::ubyte__)( 4 - ( Amount >> 1 ) );

#ifdef MSCRMI_DBG
	if ( Long > ~_Stencil( StencilSize ) )
		ERRc();
#endif

	return _SetStencilSize( ( _Address( Address ) & _Stencil( StencilSize ) ) | Long, StencilSize );
}


typedef ctn::E_XMCONTAINER_( str::string_ ) labels_;
E_AUTO( labels );

class callback__
{
public:
	virtual parse_status__ Handle(
		const _parameter_core_ &Parameter,
		const str::string_ &Value ) = 0;
};

static parse_status__ ParseParametersSpecifications_(
	parser___ &Parser,
	xaddress__ &Address,
	callback__ &Callback )
{
	parse_status__ Status = ps_Undefined;
qRH
	bso::bool__ NoLabel = true;
	bso::bool__ Continue = true;
	_parameter_core Core;
	label Label;
	str::string Value;
qRB
	while ( Continue ) {
		switch ( Parser.Parse( xml::tfAttribute | xml::tfStartTagClosed ) ) {
		case xml::tAttribute:
			if ( Parser.AttributeName() == "Address" ) {
				if ( ( Address = HandleAddress_( Address, Parser.Value() ) ) == MSCRMI_UNDEFINED_ADDRESS ) {
					Status = psBadValue;
					qRReturn;
				}
			} else if ( Parser.AttributeName() == "Label" ) {
				if ( Parser.Value().Amount() == 0 ) {
					Status = psBadValue;
					qRReturn;
				}

				NoLabel = false;

				Label.Init( Parser.Value() );
			} else {
				Status = psUnexpectedAttribute;
				qRReturn;
			}
			break;
		case xml::tStartTagClosed:
			Continue = false;
			break;
		case xml::tProcessed:
			break;
		case xml::tError:
			Status = psParserError;
			qRReturn;
			break;
		default:
			ERRc();
			break;
		}
	}

	if ( NoLabel ) {
		Status = psMissingAttribute;
		qRReturn;
	}

	Status = psOK;

	Core.Init( Label, Address, t_Group );

	Value.Init();

	Callback.Handle( Core, Value );
qRR
qRT
qRE
	return Status;
}

#define TEST( name )\
	if ( Wording == #name )\
		Type = t##name

static inline type__ Type_( const str::string_ &Wording )
{
	type__ Type = t_Undefined;

	TEST( Octet );
	else TEST( Nibble2 );
	else TEST( Octet2 );
	else TEST( Nibble3 );
	else TEST( Octet3 );
	else TEST( Nibble4 );
	else TEST( Octet4 );
	else TEST( Name );
	else TEST( HBars );

	return Type;
}

#undef TEST

static parse_status__ ParseParameter_(
	parser___ &Parser,
	xaddress__ &Address,
	type__ &Type,
	callback__ &Callback )
{
	parse_status__ Status = ps_Undefined;
qRH
	bso::bool__ Continue = true;
	label Label;
	_parameter_core Core;
	str::string Value;
	bso::bool__ TypeDefined = false;
qRB
	Label.Init();
	Value.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfAttribute | xml::tfEndTag ) ) {
		case xml::tAttribute:
			if ( Parser.AttributeName() == "Label" ) {
				if ( Label.Amount() != 0 ) {
					Status = psAttributeAlreadyDefined;
					qRReturn;
				}

				Label = Parser.Value();

				if ( Label.Amount() == 0 ) {
					Status = psBadValue;
					qRReturn;
				}
			} else if ( Parser.AttributeName() == "Address" ) {
				if ( ( Address = HandleAddress_( Address, Parser.Value() ) ) == MSCRMI_UNDEFINED_ADDRESS )
					return psBadValue;
			} else if ( Parser.AttributeName() == "Type" ) {
				if ( TypeDefined ) {
					Status = psAttributeAlreadyDefined;
					qRReturn;
				}

				TypeDefined = true;

				if ( ( Type = Type_( Parser.Value() ) ) == t_Undefined ) {
					Status = psBadValue;
					qRReturn;
				}
			} else if ( Parser.AttributeName() == "Value" ) {
				if ( Value.Amount() != 0 ) {
					Status = psAttributeAlreadyDefined;
					qRReturn;
				}

				Value = Parser.Value();

				if ( Value.Amount() == 0 ) {
					Status = psBadValue;
					qRReturn;
				}
			} else {
					Status = psUnexpectedAttribute;
					qRReturn;
				}
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tProcessed:
			ERRc();
			break;
		case xml::tError:
			return psParserError;
			break;
		default:
			ERRc();
			break;
		}
	}

	if( Label.Amount() == 0 ) {
		Status = psMissingAttribute;
		qRReturn;
	}

	Status = psOK;

	Core.Init( Label, Address, Type );

	Callback.Handle( Core, Value );

	Address = Sum_( Address, Size( Type ) );

qRR
qRT
qRE
	return Status;

}

static parse_status__ ParseParameters_(
	parser___ &Parser,
	xaddress__ &Address,
	type__ &Type,
	callback__ &Callback )
{
	parse_status__ Status = ps_Undefined;
qRH
	bso::bool__ Continue = true;
	_parameter_core Core;
	str::string Value;
qRB
	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTag | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == "Parameters" ) {
				if ( ( Status = ParseParametersSpecifications_( Parser, Address, Callback ) ) != psOK )
					qRReturn;

				if ( ( Status = ParseParameters_( Parser, Address, Type, Callback ) ) != psOK )
					qRReturn;

				Core.Init();
				Value.Init();

				Callback.Handle( Core, Value );
			} else if ( Parser.TagName() == "Parameter" ) {
				if ( ( Status = ParseParameter_( Parser, Address, Type, Callback ) ) != psOK )
					qRReturn;
			} else {
				Status = psUnexpectedTag;
				qRReturn;
			}

			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tProcessed:
			ERRc();
			break;
		case xml::tError:
			return psParserError;
			break;
		default:
			ERRc();
			break;
		}
	}

	Status = psOK;
qRR
qRT
qRE
	return Status;
}

static parse_status__ ParseParameters_(
	parser___ &Parser,
	callback__ &Callback )
{
	xaddress__ Address = 0;
	type__ Type = tOctet;

	return ParseParameters_( Parser, Address, Type, Callback );
}

static void RevertDeviceFamily_( device_family__ DeviceFamily )
{
	tol::Swap( DeviceFamily[0], DeviceFamily[3] );
	tol::Swap( DeviceFamily[1], DeviceFamily[2] );
}

static parse_status__ ParseImplementationSpecifications_(
	parser___ &Parser,
	midi_implementation_ &Implementation )
{
	bso::bool__ Continue = true;

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfAttribute | xml::tfStartTagClosed ) ) {
		case xml::tAttribute:
			if ( Parser.AttributeName() == "ModelID" ) {
				if ( Implementation.S_.ModelIDLength != 0 )
					return psAttributeAlreadyDefined;
				if ( ( Implementation.S_.ModelIDLength = Convert_( Parser.Value(), Implementation.S_.ModelID ) ) == 0 )
					return psBadValue;
			} else if ( Parser.AttributeName() == "ModelLabel" ) {
				if ( Implementation.ModelLabel.Amount() != 0 )
					return psAttributeAlreadyDefined;
				Implementation.ModelLabel = Parser.Value();
			} else if ( Parser.AttributeName() == "DeviceFamily" ) {
				if ( memcmp( Implementation.S_.DeviceFamily, MSCRMI_UNDEFINED_DEVICE_FAMILY, sizeof( Implementation.S_.DeviceFamily ) ) != 0 )
					return psAttributeAlreadyDefined;
				if ( Convert_( Parser.Value(), Implementation.S_.DeviceFamily ) == 0 )
					return psBadValue;
				// RevertDeviceFamily_( Implementation.S_.DeviceFamily );
			} else
				return psUnexpectedAttribute;
			break;
		case xml::tStartTagClosed:
			Continue = false;
			break;
		case xml::tProcessed:
			ERRc();
			break;
		case xml::tError:
			return psParserError;
			break;
		default:
			ERRc();
			break;
		}
	}

	if ( ( Implementation.S_.ModelIDLength == 0 )
		 || ( memcmp( Implementation.S_.DeviceFamily, MSCRMI_UNDEFINED_DEVICE_FAMILY, sizeof( Implementation.S_.DeviceFamily ) ) == 0 ) )
		return psMissingAttribute;

	return psOK;
}

class implementation_callback__
: public callback__
{
private:
	parameters_ *_Parameters;
	row__ _GroupRow;
protected:
	virtual parse_status__ Handle(
		const _parameter_core_ &Core,
		const str::string_ &Value )
	{
		parse_status__ Status = ps_Undefined;
	qRH
		parameter Parameter;
		row__ GroupRow = NONE;
	qRB
		if ( Core.Address() == MSCRMI_UNDEFINED_ADDRESS ) {
			_GroupRow = _Parameters->Get( _GroupRow ).GroupRow();
			_Parameters->Flush();
		} else {
			Parameter.Init( Core, _GroupRow );

			GroupRow = _Parameters->Add( Parameter );

			if ( Core.Type() == t_Group )
				_GroupRow = GroupRow;
		}

		Status = psOK;

		qRR
		qRT
		qRE
			return Status;
	}
public:
	void reset( bso::bool__ = true )
	{
		_Parameters = NULL;
		_GroupRow = NONE;
	}
	implementation_callback__( void )
	{
		reset( false );
	}
	~implementation_callback__( void )
	{
		reset();
	}
	void Init( parameters_ &Parameters )
	{
		_Parameters = &Parameters;
		_GroupRow = NONE;
	}
};

parse_status__ mscrmi::Parse(
	parser___ &Parser,
	midi_implementation_ &Implementation )
{
	parse_status__ Status = ps_Undefined;
qRH
	implementation_callback__ Callback;
qRB
	Callback.Init( Implementation.Parameters );

	switch ( Parser.Parse( xml::tfStartTag ) ) {
	case xml::tStartTag:
		if ( Parser.TagName() != "MIDIImplementation" ) {
			Status = psUnexpectedTag;
			qRReturn;
		}

		if ( ( Status = ParseImplementationSpecifications_( Parser, Implementation ) ) != psOK )
			qRReturn;

		if ( ( Status = ParseParameters_( Parser, Callback ) ) != psOK )
			qRReturn;

	case xml::tProcessed:
		break;
	case xml::tError:
		Status = psParserError;
		qRReturn;
	default:
		ERRc();
		break;
	}

qRR
qRT
qRE
	return Status;
}

static parse_status__ ParseImplementationsSpecifications_(
	parser___ &Parser,
	midi_implementations_ &Implementations )
{
	switch ( Parser.Parse( xml::tfStartTagClosed ) ) {
	case xml::tStartTagClosed:
		break;
	case xml::tProcessed:
		ERRc();
		break;
	case xml::tError:
		return psParserError;
		qRReturn;
	default:
		ERRc();
		break;
	}

	return psOK;
}

static parse_status__ ParseImplementations_(
	parser___ &Parser,
	midi_implementations_ &Implementations )
{
	parse_status__ Status = ps_Undefined;
qRH
	midi_implementation Implementation;
	bso::bool__ Continue = true;
	implementation_callback__ Callback;
qRB
	while ( Continue ) {
		switch( Parser.Parse( xml::tfStartTag |xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == "MIDIImplementation" ) {
				Implementation.Init();
				Callback.Init( Implementation.Parameters );

				if ( ( Status = ParseImplementationSpecifications_( Parser, Implementation ) ) != psOK )
					qRReturn;

				if ( ( Status = ParseParameters_( Parser, Callback ) ) != psOK )
					qRReturn;

				Implementations.Append( Implementation );
			} else {
				Status = psUnexpectedTag;
				qRReturn;
			}
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tProcessed:
			ERRc();
			break;
		case xml::tError:
			return psParserError;
			qRReturn;
		default:
			ERRc();
			break;
		}
	}

	Status = psOK;
qRR
qRT
qRE
	return Status;
}

parse_status__ mscrmi::Parse(
	parser___ &Parser,
	midi_implementations_ &Implementations )
{
	parse_status__ Status = ps_Undefined;
qRH
qRB
	switch ( Parser.Parse( xml::tfStartTag ) ) {
	case xml::tStartTag:
		if ( Parser.TagName() != "MIDIImplementations" ) {
			Status = psUnexpectedTag;
			qRReturn;
		}

		if ( ( Status = ParseImplementationsSpecifications_( Parser, Implementations ) ) != psOK )
			qRReturn;

		if ( ( Status = ParseImplementations_( Parser, Implementations ) ) != psOK )
			qRReturn;

	case xml::tProcessed:
		break;
	case xml::tError:
		Status = psParserError;
		qRReturn;
	default:
		ERRc();
		break;
	}

qRR
qRT
qRE
	return Status;
}

bso::bool__ mscrmi::Append(
	bso::ulong__ Value,
	size__ Size,
	data_ &Data )
{
	Value = Encode_( Value );

	if ( Size > 4 )
		ERRu();

	if ( ( Value & ~( ~0UL >> ( Size << 3 ) ) ) != 0 )
		return false;	// 'Value' trop grand par rapport au 'Size' donn.

	switch ( Size ) {
	case 0:
		ERRc();
		break;
	case 4:
		Data.Append( Value >> 24 );
	case 3:
		Data.Append( ( Value >> 16 ) & 0xff );
	case 2:
		Data.Append( ( Value >> 8 ) & 0xff );
	case 1:
		Data.Append( Value & 0xff );
		break;
	default:
		ERRc();
		break;
	}

	return true;
}

class settings_callback___
: public callback__
{
private:
	adata_set_ *_DataSet;
	adata _Data;
	const parameters_ *_Parameters;
	row__ _Row;
	row__ _Search( address__ Address ) const
	{
		return Search( Address, _Row, *_Parameters );
	}
	parse_status__ _ConvertOctetNibbleValue( 
		const str::string_ &Value,
		size__ Size,
		data_ &Data )
	{
		bso::ulong__ Long;
		epeios::row__ Error = NONE;

		switch ( Size ) {
		case 0:
			ERRc();
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			Long = Value.ToUL( &Error, ~( 0xffffffff << ( Size * 7 ) ) );
			break;
		default:
			ERRc();
			break;
		}

		if ( Error != NONE )
			return psBadValue;

		if ( !Append( Long, Size, Data ) )
			ERRc();

		return psOK;
	}
	parse_status__ _ConvertTextValue( 
		const str::string_ &Value,
		size__ Size,
		data_ &Data )
	{
		epeios::row__ Row = Value.First();
		bso::char__ Char = 0;

		if ( Value.Amount() > Size )
			return psBadValue;

		while ( Row != NONE ) {
			Char = Value( Row );

			if ( ( Char < 32 ) ||( Char > 127 ) )
				return psBadValue;

			Data.Append( Char );

			Size--;

			Row = Value.Next( Row );
		}

		while ( Size-- )
			Data.Append( ' ' );

		return psOK;
	}
	parse_status__ _ConvertHBarsValue( 
		const str::string_ &Value,
		size__ Size,
		data_ &Data )
	{
		epeios::row__ Row = Value.First();
		bso::char__ Char = 0;

		if ( Value.Amount() != 9 )
			return psBadValue;

		while ( Row != NONE ) {
			Char = Value( Row );

			if ( ( Char < '0' ) ||( Char > '8' ) )
				return psBadValue;

			Data.Append( Char - '0' );

			Row = Value.Next( Row );
		}

		return psOK;
	}
protected:
	virtual parse_status__ Handle(
		const _parameter_core_ &Core,
		const str::string_ &Value )
	{
		parse_status__ Status = ps_Undefined;
	qRH
		data RawData;
		ctn::E_CMITEMt( parameter_, row__ ) Parameter;
	qRB
		Parameter.Init( *_Parameters );

		if ( Core.Type() != t_Group ) {

			if ( ( _Row = _Search( Core.Address() ) ) == NONE ) {
				Status = psBadValue;
				qRReturn;
			}

			if ( Value.Amount() == 0 ) {
				Status =psBadValue;
				qRReturn;
			}

			RawData.Init();

			switch ( Parameter( _Row ).Type() ) {
			case tOctet:
			case tNibble2:
			case tOctet2:
			case tNibble3:
			case tOctet3:
			case tNibble4:
			case tOctet4:
				Status = _ConvertOctetNibbleValue( Value, Parameter( _Row ).Size(), RawData );
				break;
			case tName:
				Status = _ConvertTextValue( Value, Parameter( _Row ).Size(), RawData );
				break;
			case tHBars:
				Status = _ConvertHBarsValue( Value, Parameter( _Row ).Size(), RawData );
				break;
			default:
				ERRc();
				break;
			}

			if ( Status != psOK )
				qRReturn;

			if ( _Data.Address() == MSCRMI_UNDEFINED_ADDRESS )
				_Data.Init( RawData, Core.Address() );
			else if ( ( ( RawData.Amount() + _Data.Amount() ) > 128 )
			          || ( Sum_( _Data.Address(), (size__)_Data.Amount() ) != Core.Address() ) ) {
				_DataSet->Append( _Data );
				_Data.Init( RawData, Core.Address() );
			} else {
				_Data.Append( RawData );
			}
		}

		Status = psOK;

		qRR
		qRT
		qRE
			return Status;
	}
public:
	void reset( bso::bool__ P = true )
	{
		if ( P  ) {
			if ( _Data.Address() != MSCRMI_UNDEFINED_ADDRESS ) {
				_DataSet->Append( _Data );
			}
		}

		_Data.reset( P );
		_DataSet = NULL;
		_Parameters = NULL;
		_Row = NONE;

	}
	settings_callback___( void )
	{
		reset( false );
	}
	~settings_callback___( void )
	{
		reset();
	}
	void Init(
		adata_set_ &DataSet,
		const parameters_ &Parameters )
	{
		reset();

		_DataSet = &DataSet;
		_Parameters = &Parameters;
	}
};


parse_status__ mscrmi::Parse(
	parser___ &Parser,
	const midi_implementation_ &Implementation,
	adata_set_ &DataSet )
{
	parse_status__ Status = ps_Undefined;
qRH
	settings_callback___ Callback;
qRB
	Callback.Init( DataSet, Implementation.Parameters );

	switch ( Parser.Parse( xml::tfStartTag ) ) {
	case xml::tStartTag:
		if ( Parser.TagName() != "Settings" ) {
			Status = psUnexpectedTag;
			qRReturn;
		}

		if ( ( Status = ParseParameters_( Parser, Callback ) ) != psOK )
			qRReturn;

	case xml::tProcessed:
		break;
	case xml::tError:
		Status = psParserError;
		qRReturn;
	default:
		ERRc();
		break;
	}

qRR
qRT
qRE
	return Status;
}


static void Convert_(
	const char *Source,
	bso::ulong__ SourceLength,
	char *Target )
{
	if ( SourceLength > 4 )
		ERRc();

	while ( SourceLength-- ) {
		if ( sprintf( Target, "%02X", *Source ) < 0 )
			ERRs();

		Target += 2;
		Source++;
	}
}

const str::string_  &mscrmi::ToString(
	const bso::char__ (&Value)[4],
	str::string_ &Target )
{
	char Buffer[] ="12345678";

	Convert_( Value, sizeof( Value ), Buffer );

	Target.Append( Buffer );

	return Target;

}


void mscrmi::Print(
	const midi_implementation_ &Implementation,
	xml::writer_ &Writer )
{
	char Buffer[] ="12345678";

	Writer.PushTag( "MIDIImplementation" );
	Writer.PutAttribute( "Model" , Implementation.ModelLabel );

	Convert_( Implementation.S_.ModelID, Implementation.S_.ModelIDLength, Buffer );
	Writer.PutAttribute( "ModelID", Buffer );

	Convert_( Implementation.S_.DeviceFamily, sizeof( Implementation.S_.DeviceFamily ), Buffer );
	Writer.PutAttribute( "DeviceFamily", Buffer );

	Print( Implementation.Parameters, Writer );

	Writer.PopTag();
}

void mscrmi::Print(
	const midi_implementations_ &Implementations,
	xml::writer_ &Writer )
{
	ctn::E_CITEM( midi_implementation_ ) Implementation;
	epeios::row__ Row = Implementations.First();

	Implementation.Init( Implementations );

	Writer.PushTag( "MIDIImplementation" );

	while ( Row != NONE ) {
		Print( Implementation( Row ), Writer );

		Row = Implementations.Next( Row );
	}

	Writer.PopTag();
}

static datum__ Checksum_(
	const data_ &Data,
	epeios::row__ First = 0,
	epeios::row__ Last = NONE )
{
	bso::ubyte__ Checksum = 0;
	epeios::row__ Row = First;

	while( Row != Last ) {
		Checksum += Data( Row );
		Row = Data.Next( Row );
	}

	if ( Last != NONE ) 
		Checksum += Data( Row );

	return ( 128 - ( Checksum & 0x7f ) ) & 0x7f;
}

static void Put_(
	bso::ulong__ Value,
	data_ &Data )
{
	Data.Append(  ( Value >> 24 ) & 0xff );
	Data.Append(  ( Value >> 16 ) & 0xff );
	Data.Append(  ( Value >> 8 ) & 0xff );
	Data.Append(  Value  & 0xff );
}

void mscrmi::RequestData(
	const identity__ &Identity,
	address__ Address,
	size__ Size,
	flw::oflow__ &Flow )
{
qRH
	data Data;
	STR_BUFFER___ Buffer;
qRB
	Data.Init();

	Put_( Address, Data );
	Put_( Encode_( Size ), Data );


	Flow.Write( "\xf0\x41", 2 );
	Flow.Put( Identity.DeviceID );
	Flow.Write( Identity.ModelID, Identity.ModelIDLength );
	Flow.Put( 0x11 );

	Flow.Write( Data.Convert( Buffer ), Data.Amount() );
	Flow.Put( Checksum_( Data ) );

	Flow.Put( 0xF7 );

	Flow.Commit();
qRR

qRT
qRE
}

mscmdm::size__ ExtractSysEx_(
	flw::iflow__ &Flow,
	mscmdm::origin__ Origin,
	mscmdm::data_ &Data )
{
	mscmdm::size__ Size = 0;
qRH
	mscmdm::event_header__ EventHeader;
	mscmdm::data Dummy;
	bso::bool__ Continue = true;
qRB
	while ( Continue ) {
		mscmdm::GetEventHeader( Flow, Origin, EventHeader );

		if ( ( EventHeader.EventType == mscmdm::etSystem ) && ( EventHeader.SystemEvent.Event == mscmdm::sysExclusive ) ) {
			Size = mscmdm::GetEventData( EventHeader, Flow, mscmdm::oDevice, Data );
			Continue = false;
		} else {
			Dummy.Init();
			mscmdm::GetEventData( EventHeader, Flow, mscmdm::oDevice, Dummy );
		}
	}
qRR
qRT
qRE
	return Size;
}

#define CASE( label )	LCL_CASE( label, ts )

const char *mscrmi::Label( transmission_status__ Status )
{
	switch( Status ) {
	CASE( ChecksumError )
	CASE( IncorrectData )
	default:
		ERRu();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}

#undef CASE

static transmission_status__  Extract_(
	const mscmdm::data_ &RawData,
	const identity__ &Identity,
	address__ &Address,
	data_ &Data )
{
	transmission_status__ Status = ts_Undefined;
qRH
	epeios::row__ Row = RawData.First();
	bso::size__ Counter = 0;
	str::string Header, Buffer;
qRB
	Buffer.Init( "\x41" );
	Buffer.Append( Identity.DeviceID );
	Buffer.Append( Identity.ModelID, Identity.ModelIDLength );
	Buffer.Append( '\x12' );

	Header.Init();

	Counter = Buffer.Amount();

	while ( ( Row != NONE ) && ( Counter-- ) ) {
		Header.Append( RawData( Row ) );
		Row = RawData.Next( Row );
	}

	if ( Row == NONE ) {
		Status = tsIncorrectData;
		qRReturn;
	}

	if ( Buffer != Header ) {
		Status = tsIncorrectData;
		qRReturn;
	}

	if ( Checksum_( RawData, Row, RawData.Previous( RawData.Last() ) ) != RawData( RawData.Last() ) ) {
		Status =  tsChecksumError;
		qRReturn;
	}

	Counter = 4;	// Taille d'une addresse.

	while ( Counter-- ) {

		if ( Row == NONE ) {
			Status = tsIncorrectData;
			qRReturn;
		}

		Address = Address << 8 | RawData( Row );

		Row = RawData.Next( Row );
	}

	if ( Row == NONE ) {
		Status = tsIncorrectData;
		qRReturn;
	}
	
	Data.Append( RawData, Row, RawData.Previous( RawData.Last() ) );

	Status = tsOK;
qRR
qRT
qRE
	return Status;
}

static void Fill_(
	const adata_ &Data,
	const parameters_ &Parameters,
	row__ &ParameterRow,
	settings_ &Settings )
{
qRH
	setting Setting;
	epeios::row__ DataRow = NONE;
	address__ Address = NONE;
	ctn::E_CMITEMt( parameter_, row__ ) Parameter;
	type__ Type;
qRB
	Address = Data.Address();
	DataRow = Data.First();
	Parameter.Init( Parameters );

	while ( DataRow != NONE ) {
		while ( ( ParameterRow != NONE ) &&
			    ( ( Parameter( ParameterRow ).Address() < Address )
				   || ( ( Type = Parameter( ParameterRow ).Type() ) == t_Group ) ) )
			ParameterRow = Parameters.Next( ParameterRow );

		if ( ( ParameterRow == NONE ) || ( Parameter( ParameterRow ).Address() > Address ) )
			ERRu();

		if ( !Data.Exists( Data.Next( DataRow, Size( Type ) - 1 ) ) )
			ERRu();

		Setting.Init( ParameterRow );

		Setting.Data.Allocate( Size( Type ) );
		Setting.Data.Store( Data, Size( Type ), 0, DataRow );

		Settings.Append( Setting );

		Address = Sum_( Address, Size( Type ) );

		DataRow = Data.Next( DataRow, Size( Type ) );
	}
qRR
qRT
qRE
}

static bso::ulong__ Convert_( const str::string_ &Value )
{
	bso::ulong__ V = 0;
	epeios::row__ Row = Value.First();

	if ( Value.Amount() > 4 )
		ERRl();

	while ( Row != NONE ) {
		V = ( V << 8 ) + Value( Row );

		Row = Value.Next( Row );
	}

	return V;
}

static const str::string_ &ConvertToOctetValue_(
	const str::string_ &Data,
	str::string_ &Buffer )
{
	bso::integer_buffer__ IBuffer;

	Buffer.Append( bso::Convert( Decode_( Convert_( Data ) ), IBuffer ) );

	return Buffer;
}

static const str::string_ &ConvertToNibbleValue_(
	const str::string_ &Data,
	str::string_ &Buffer )
{
	bso::integer_buffer__ IBuffer;
	bso::ulong__ Value = 0;
	epeios::row__ Row = Data.First();

	while ( Row != NONE ) {
		if ( Data( Row ) > 15 )
			ERRc();

		Value = ( Value << 2 ) + Data( Row );

		Row = Data.Next( Row );
	}

	Buffer.Append( bso::Convert( Value, IBuffer ) );

	return Buffer;
}

static const str::string_ &ConvertToNameValue_(
	const data_ &Data,
	size__ Size,
	str::string_ &Buffer )
{
	epeios::row__ Row = Data.First();
	epeios::size__ Amount = 0;
	bso::char__ Char = 0;

	if ( Data.Amount() > Size )
		ERRc();

	while ( Row != NONE ) {
		Char = Data( Row );

		if ( ( Char < 32 ) ||( Char > 127 ) )
			ERRc();

		Buffer.Append( Char );

		if ( Char != ' ' )
			Amount = Buffer.Amount();

		Row = Data.Next( Row );
	}

	Buffer.Allocate( Amount );

	return Buffer;
}

static const str::string_ &ConvertToHBarsValue_(
	const data_ &Data,
	size__ Size,
	str::string_ &Buffer )
{
	epeios::row__ Row = Data.First();
	bso::char__ Char = 0;

	if ( Data.Amount() > Size )
		ERRc();

	while ( Row != NONE ) {
		Char = Data( Row );

		if ( ( Char < 0 ) ||( Char > 8 ) )
			ERRc();

		Buffer.Append( Char + '0' );

		Row = Data.Next( Row );
	}

	return Buffer;
}

static inline void Print_(
	const setting_ &Setting,
	const parameters_ &Parameters,
	xml::writer_ &Writer )
{
qRH
	char Address[] = "12345678";
	ctn::E_CMITEMt( parameter_, row__ ) Parameter;
	str::string Buffer;
qRB

	Parameter.Init( Parameters );

	Writer.PutAttribute( "Label", Parameter( Setting.Row() ).Label );

	if ( sprintf( Address, "%0*lX", 2 * ( 4 - _StencilSize( Parameter( Setting.Row() ).S_.Address ) ), Parameter( Setting.Row() ).Offset() ) < 0 )
		ERRs();

	Writer.PutAttribute( "Address", Address );

	Buffer.Init();

	switch ( Parameter( Setting.Row() ).Type() ) {
	case tOctet:
	case tOctet2:
	case tOctet3:
	case tOctet4:
		ConvertToOctetValue_( Setting.Data, Buffer );
		/// Attention : si un jour ajout d'un 'tOctetx' ou x > 4, la fonction ci-dessus ne fonctionne pas !
		break;
	case tNibble2:
	case tNibble3:
	case tNibble4:
		ConvertToNibbleValue_( Setting.Data, Buffer );
		/// Attention : si un jour ajout d'un 'tNibblex' ou x > 4, la fonction ci-dessus ne fonctionne pas !
		break;
	case tName:
		ConvertToNameValue_( Setting.Data, Size( Parameter( Setting.Row() ).Type() ), Buffer );
		break;
	case tHBars:
		ConvertToHBarsValue_( Setting.Data, Size( Parameter( Setting.Row() ).Type() ), Buffer );
		break;
	case t_Group:
		ERRc();
		break;
	default:
		ERRc();
		break;
	}

	Writer.PutAttribute( "Value", Buffer);
qRR
qRT
qRE
}

typedef bch::E_BUNCH_( row__ ) rows_;
E_AUTO( rows );

static void BuildPath_(
	row__ Row,
	const parameters_ &Parameters,
	rows_ &Rows )
{
	ctn::E_CMITEMt( parameter_, row__ ) Parameter;

	Parameter.Init( Parameters );

	if ( Row != NONE )
		Row = Parameter( Row ).GroupRow();

	while ( Row != NONE ) {
		Rows.Insert( Row, 0 );
		Row = Parameter( Row ).GroupRow();
	}
}

static epeios::row__ SearchLastCommon_(
	const rows_ &Rows1,
	const rows_ &Rows2 )
{
	epeios::row__ Row1 = Rows1.First(), Row2 = Rows2.First();

	while ( ( Row1 != NONE ) && ( Row2 != NONE ) && ( Rows1( Row1 ) == Rows2( Row2 ) ) ) {
		Row1 = Rows1.Next( Row1 );
		Row2 = Rows2.Next( Row2 );
	}

	if ( Row1 == NONE )
		return Rows1.Last();
	else if ( Row2 == NONE )
		return Rows2.Last();
	else if ( Row1 != Row2 )
		ERRc();

	return Rows1.Previous( Row1 );
}

static void Pop_(
	const rows_ &Rows,
	epeios::row__ TargetRow,
	xml::writer_ &Writer )
{
	epeios::row__ Row = Rows.Last();

	while ( Row != TargetRow ) {
		Writer.PopTag();

		Row = Rows.Previous( Row );
	}
}

static void Push_(
	const rows_ &Rows,
	epeios::row__ Row,
	const parameters_ &Parameters,
	xml::writer_ &Writer )
{
	ctn::E_CMITEMt( parameter_, row__ ) Parameter;
	Parameter.Init( Parameters );

	Row = ( Row == NONE ? Rows.First() : Rows.Next( Row ) );

	while ( Row != NONE ) {
		Writer.PushTag( "Parameters" );
		Print_( Parameter( Rows( Row ) ), Writer );

		Row = Rows.Next( Row );
	}
}

static void PrintTags_(
	row__ Current,
	row__ Previous,
	const parameters_ &Parameters,
	xml::writer_ &Writer )
{
qRH
	rows CurrentRows, PreviousRows;
	epeios::row__ Row = NONE;
qRB
	CurrentRows.Init();
	BuildPath_( Current, Parameters, CurrentRows );

	PreviousRows.Init();
	BuildPath_( Previous, Parameters, PreviousRows );

	Row = SearchLastCommon_( CurrentRows, PreviousRows );

	Pop_( PreviousRows, Row, Writer );

	Push_( CurrentRows, Row, Parameters, Writer );
qRR
qRT
qRE
}

void mscrmi::Print(
	const settings_ &Settings,
	const parameters_ &Parameters,
	xml::writer_ &Writer )
{
qRH
	rows Rows;
	row__ ParameterRow = NONE;
	epeios::row__ Row = NONE;
	row__ PreviousRow = NONE;
	ctn::E_CMITEM( setting_ ) Setting;
	ctn::E_CMITEMt( parameter_, row__ ) Parameter;
	Parameter.Init( Parameters );
qRB
	Setting.Init( Settings );
	Parameter.Init( Parameters );
	Row = Settings.First();

	Rows.Init();

	while ( Row != NONE ) {

		PrintTags_( Setting( Row ).Row(), PreviousRow, Parameters, Writer );

		Writer.PushTag( "Parameter" );
		Print_( Setting( Row ), Parameters, Writer );
		Writer.PopTag();

		PreviousRow = Setting( Row ).Row();

		Row = Settings.Next( Row );
	}

	PrintTags_( NONE, PreviousRow, Parameters, Writer );
qRR
qRT
qRE
}


static void Fill_(
	const adata_set_ &DataSet,
	const parameters_ &Parameters,
	settings_ &Settings )
{
	epeios::row__ DataSetRow = DataSet.First();
	row__ ParameterRow = Parameters.First();
	ctn::E_CMITEM( adata_ ) Data;

	Data.Init( DataSet );

	while ( DataSetRow != NONE ) {
		Fill_( Data( DataSetRow ), Parameters, ParameterRow, Settings );

		DataSetRow = DataSet.Next( DataSetRow );
	}
}


void mscrmi::Fill(
	const adata_set_ &DataSet,
	const midi_implementation_ &Implementation,
	settings_ &Settings )
{
	Fill_( DataSet, Implementation.Parameters, Settings );
}

inline transmission_status__ SendIdentityRequest_(
	device_id__ Id,
	flw::oflow__ &Flow )
{
	Flow.Write( "\xf0\x7e", 2 );
	Flow.Put( Id );
	Flow.Write( "\x06\x01\xF7", 3 );

	return tsOK;
}

transmission_status__ mscrmi::GetDeviceFamilyAndSoftwareRevision(
	device_id__ Id,
	flw::ioflow__ &Flow,
	device_family__ &DeviceFamily,
	software_revision__ &SoftwareRevision )
{
	transmission_status__ Status = ts_Undefined;
qRH
	data Data;
	epeios::row__ Row = NONE;
	data Model;
qRB

	if ( ( Status = SendIdentityRequest_( Id, Flow ) ) != tsOK )
		qRReturn;

	Flow.Commit();

	Data.Init();

	ExtractSysEx_( Flow, mscmdm::oDevice, Data );

	Flow.Dismiss();

	if ( Data.Amount() != 13 ) {
		Status =tsIncorrectData;
		qRReturn;
	}

	Model.Init( "\x7e" );
	Model.Append( Id );
	Model.Append( "\x06\x02\x41" );


	if ( str::Compare( Data, Model, Data.First(), Model.First(), Model.Amount() ) != 0 ) {
		Status = tsIncorrectData;
		qRReturn;
	}

	Data.Recall( Data.First( Model.Amount() ), sizeof( DeviceFamily ), DeviceFamily );
	Data.Recall( Data.First( Model.Amount() + sizeof( DeviceFamily ) ), sizeof( SoftwareRevision ), SoftwareRevision );

	Status = tsOK;
qRR
qRT
qRE
	return Status;
}

transmission_status__ mscrmi::Extract(
	flw::iflow__ &Flow,
	const identity__ &Identity,
	mscmdm::origin__ Origin,
	address__ &Address,
	data_ &Data )
{
	transmission_status__ Status = ts_Undefined;
qRH
	mscmdm::data RawData;
	epeios::row__ Row = NONE;
qRB
	RawData.Init();
	ExtractSysEx_( Flow, Origin, RawData );

	if ( ( Status = Extract_( RawData, Identity, Address, Data ) ) != tsOK )
		qRReturn;

	Status = tsOK;
qRR
qRT
qRE
	return Status;
}

transmission_status__ mscrmi::Retrieve(
	flw::ioflow__ &Flow,
	address__ Address,
	size__ Size,
	const identity__ &Identity,
	adata_ &Data )
{
	transmission_status__ Status = ts_Undefined;
	mscmdm::size__ HandledSize = Size + Data.Amount();

	RequestData( Identity, Address, Size, Flow );

	Data.Address() = MSCRMI_UNDEFINED_ADDRESS;

	if ( Data.Amount() != HandledSize )
		if ( ( Status = Extract( Flow, Identity, mscmdm::oDevice, Data.Address(), Data ) ) != tsOK )
			return Status;

	if ( Data.Address() != Address )
		ERRf();

	while ( Data.Amount() != HandledSize )
		if ( ( Status = Extract( Flow, Identity, mscmdm::oDevice, Address, Data ) ) != tsOK )
			return Status;

	Status = tsOK;

	Flow.Dismiss();

	return Status;
}

transmission_status__ mscrmi::Retrieve(
	flw::ioflow__ &Flow,
	const blocs_ &Blocs,
	const identity__ &Identity,
	adata_set_ &DataSet )
{
	transmission_status__ Status = ts_Undefined;
qRH
	epeios::row__ Row = NONE;
	adata Data;
qRB
	Row = Blocs.First();

	while ( Row != NONE ) {
		Data.Init();

		if( ( Status = Retrieve( Flow, Blocs( Row ).Address, Blocs( Row ).Size, Identity, Data ) ) != tsOK )
			qRReturn;

		DataSet.Append( Data );

		Row = Blocs.Next( Row );

		tht::Suspend( 40 );
	}
qRR
qRT
qRE
	return Status;
}

epeios::row__ mscrmi::GetCorrespondingMIDIImplementation(
	const device_family__ &DeviceFamily,
	const midi_implementations_ &Implementations )
{
	ctn::E_CITEM( midi_implementation_ ) Implementation;
	epeios::row__ Row = Implementations.First();

	Implementation.Init( Implementations );

	while ( Row != NONE ) {
		if ( memcmp( Implementation( Row ).DeviceFamily(), DeviceFamily, sizeof( DeviceFamily ) ) == 0 )
			break;

		Row = Implementations.Next( Row );
	}

	return Row;
}

void mscrmi::Send(
	const adata_ &Data,
	const identity__ &Identity,
	flw::oflow__ &Flow )
{
qRH
	data Message;
	STR_BUFFER___ Buffer;
qRB
	Message.Init();

	Put_( Data.Address(), Message );
	Message.Append( Data );


	Flow.Write( "\xf0\x41", 2 );
	Flow.Put( Identity.DeviceID );
	Flow.Write( Identity.ModelID, Identity.ModelIDLength );
	Flow.Put( 0x12 );

	Flow.Write( Message.Convert( Buffer ), Message.Amount() );
	Flow.Put( Checksum_( Message ) );

	Flow.Put( 0xF7 );

	Flow.Commit();
qRR

qRT
qRE
}

void mscrmi::Send(
	const adata_set_ &DataSet,
	const identity__ &Identity,
	flw::oflow__ &Flow )
{
	ctn::E_CMITEM( adata_ ) Data;
	epeios::row__ Row = DataSet.First();

	Data.Init( DataSet );

	while ( Row != NONE ) {
		Send(Data( Row ),  Identity, Flow );

		Row = DataSet.Next( Row );

		tht::Suspend( 40 );
	}
}
