/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#define MSCVKX__COMPILATION

#include "mscvkx.h"

#include "mscvkt.h"
#include "xml.h"

using namespace mscvkx;

typedef dtr::browser__<mscvkt::row__>  browser__;
typedef xml::writer_	writer_;

#define PARAMETERS		"Parameters"

#define PARAMETER		"Parameter"
#define ADDRESS			"Address"
#define OFFSET			"Offset"
#define BASE_OFFSET		"BaseOffset"
#define VALUE			"Value"
#define LABEL			"Label"

#define GROUP			"Group"

#define SYSTEM			"System"
#define SYSTEM_BASIC	"Basic"
#define SYSTEM_MIDI		"MIDI"

#define ORCHESTRAL_VOICES	"OrchestralVoices"
#define ORCHESTRAL_VOICE	"OrchestralVoice"

#define REGISTRATIONS	"Registrations"
#define REGISTRATION	"Registration"


static void HandleName_(
	browser__ &Browser,
	const mscvkt::tree_ &Tree,
	writer_ &XML )
{
qRH
	str::string Name;
	bso::bool__ Continue = true;
qRB

	XML.PushTag( PARAMETER );
	XML.PutAttribute( LABEL, Tree( Browser.Position() ).Description );

	Name.Init();

	Tree.Browse( Browser );

	if ( Browser.Kinship() != dtr::kChild )
		ERRc();
	
	XML.PutAttribute( BASE_OFFSET, mscvkp::AddressToString( Tree( Browser.Position() ).Address & 0xffff, true ) );

	while ( Continue ) {
		Name.Append( Tree( Browser.Position() ).Datum );

		Tree.Browse( Browser );

		switch ( Browser.Kinship() ) {
		case dtr::kParent:
			Name.StripTailingCharacter( ' ' );
			XML.PutValue( Name );
			Continue = false;
			break;
		case dtr::kSibling:
			break;
		case dtr::kChild:
			ERRc();
			break;
		default:
			ERRc();
			break;
		}
	}

//	XML.PopTag();
	
qRR
qRT
qRE
}

static void HandleBars_(
	browser__ &Browser,
	const mscvkt::tree_ &Tree,
	writer_ &XML )
{
qRH
	str::string Bars;
	bso::bool__ Continue = true;
qRB

	XML.PushTag( PARAMETER );
	XML.PutAttribute( LABEL, Tree( Browser.Position() ).Description );

	Bars.Init();

	Tree.Browse( Browser );

	if ( Browser.Kinship() != dtr::kChild )
		ERRc();
	
	XML.PutAttribute( BASE_OFFSET, mscvkp::AddressToString( Tree( Browser.Position() ).Address & 0xffff, true ) );

	while ( Continue ) {
		Bars.Append( Tree( Browser.Position() ).Datum + '0' );

		Tree.Browse( Browser );

		switch ( Browser.Kinship() ) {
		case dtr::kParent:
			XML.PutValue( Bars );
			Continue = false;
			break;
		case dtr::kSibling:
			break;
		case dtr::kChild:
			ERRc();
			break;
		default:
			ERRc();
			break;
		}
	}

//	XML.PopTag();
	
qRR
qRT
qRE
}


static void Dump_(
	const mscvkt::tree_ &Tree,
	txf::text_oflow__ &Flow )
{
qRH
	xml::writer XML;
	browser__  Browser;
	mscvkt::row__ Row = NONE;
	bso::bool__ Skip = false;
qRB
	XML.Init( Flow, xml::oIndent );

	XML.PushTag( PARAMETERS );

	Browser.Init( Tree.GetRoot() );

	Row = Tree.Browse( Browser );

	Row = Browser.Position();

	while ( Row != NONE ) {
		switch ( Browser.Kinship() ) {
		case dtr::kParent:
			XML.PopTag();
			break;
		case dtr::kSibling:
			XML.PopTag();
		case dtr::kChild:
			if ( Tree.GetLevel( Row ) == 1 ) {
				mscvkp::address__ Address = Tree( Row ).Address;

				if ( mscvkp::IsSystem( Address ) )
					XML.PushTag( SYSTEM );
				else if ( mscvkp::IsOrchestralVoice( Address ) )
					XML.PushTag( ORCHESTRAL_VOICES );
				else if ( mscvkp::IsRegistration( Address ) )
					XML.PushTag( REGISTRATIONS );
				else
					ERRc();
			} else if ( Tree.GetLevel( Row ) == 2 ) {
				mscvkp::address__ Address = Tree( Row ).Address;

				if ( mscvkp::IsSystemBasic( Address ) )
					XML.PushTag( SYSTEM_BASIC );
				else if ( mscvkp::IsSystemMidi( Address ) )
					XML.PushTag( SYSTEM_MIDI );
				else if ( mscvkp::IsOrchestralVoice( Address ) )
					XML.PushTag( ORCHESTRAL_VOICE );
				else if ( mscvkp::IsRegistration( Address ) )
					XML.PushTag( REGISTRATION );
				else
					ERRc();

				if ( !mscvkp::IsSystem( Address ) ) {
					XML.PutAttribute( ADDRESS, mscvkp::AddressToString( Tree( Row ).Address & 0xffff0000, true ) );
					XML.PutAttribute( LABEL, Tree( Row ).Description );
				}

			} else if ( Tree.HasChild( Row ) ) {
				mscvkp::address__ Address = Tree( Tree.FirstChild( Row ) ).Address;

				if ( mscvkp::IsName( Address ) ) {
					HandleName_( Browser, Tree, XML );
					Skip = true;
				} else if ( mscvkp::IsBar( Address ) ) {
					HandleBars_( Browser, Tree, XML );
					Skip = true;
				} else {
					XML.PushTag( GROUP );
					XML.PutAttribute( LABEL, Tree( Row ).Description );
				}
			} else {
				XML.PushTag( PARAMETER );
				XML.PutAttribute( LABEL, Tree( Row ).Description );
			}
			break;
		default:
			ERRc();
			break;
		}

		if ( !Skip && !Tree.HasChild( Row ) ) {
			XML.PutAttribute( OFFSET, mscvkp::AddressToString( Tree( Row ).Address & 0xffff, true ) );
			XML.PutAttribute( VALUE, mscvkp::DatumToString( Tree( Row ).Datum, false ) );
		}

		Row = Tree.Browse( Browser );

		Skip = false;
	}

	XML.PopTag();
	
qRR
qRT
qRE
}


void mscvkx::Write(
	const mscvkp::data_sets_ &DataSets,
	txf::text_oflow__ &Flow )
{
qRH
	mscvkt::tree Tree;
qRB
	Tree.Init();

	Tree.Add( DataSets );

	Dump_( Tree, Flow );
qRR
qRT
qRE
}

struct callback__
: public xml::callback__
{
private:
	bso::bool__ _Ignore;	// A vrai si les attributs/valeur de la balise doivent tre ignors (on n'est pas dans une balise 'Parameter').
	bso::bool__ _Special;	// La balise doit avoir un traitement spcial (nom d'un registre, par exemple).
	bso::bool__ _Append;		// La valuer doit tre ajoute dans le 'DataSet' courant, sinon un nouveau doit tre cre.
	bso::bool__ _IgnoreLabel;	// Address of offset was given, so 'Label' attribute can be ignored.
	mscvkp::address__ _Address;
	mscvkp::address__ _Offset;
	mscvkp::datum__ _Datum;
	mscvkp::data_set _DataSet;
	mscvkp::data_sets_ *_DataSets;
	bso::bool__ _HandleAddress(
		const str::string_ &Pattern,
		bso::bool__ SetAddress )
	{
		epeios::row__ Error = NONE;
		mscvkp::address__ Address = Pattern.ToUL( &Error, str::b16 );

		if ( Error != NONE )
			return false;

		if ( SetAddress ) {
			_Address = Address;
			_Offset = 0;
			_Append = false;
		} else {
#ifdef MSCVKX_DBG
			if ( Address > 0xffff )
				ERRc();
#endif
			_Append = ( Address == ( _Offset + 1 ) );
			_Offset = Address;
		}

		return true;
	}
	bso::bool__ _HandleLabel( const str::string_ &Pattern )
	{
		// Only registration label are yet handled.
		mscvkp::address__ Address = 0;

		bso::ubyte__ Number;
		bso::char__ C;

		if ( Pattern.Amount() != 3 )
			return false;

		switch ( Pattern( Pattern.First() ) ) {
		case 'A' :
		case 'a':
			Address = 0x10000000;
			break;
		case 'B':
		case 'b':
			Address = 0x10400000;
			break;
		default:
			return false;
			break;
		}

		C = Pattern( Pattern.First( 1 ) );

		if ( ( C < '1' ) ||( C > '8' ) )
			return false;

		Number = ( C - '1' ) * 8;

		C = Pattern( Pattern.First( 2 ) );

		if ( ( C < '1' ) ||( C > '8' ) )
			return false;

		Number += ( C -'1' );

		Address += Number * 0x10000;

		_Address = Address;
		_Offset = 0;
		_Append = false;

		return true;
	}
	bso::bool__ _HandleValue( const str::string_ &Pattern )
	{
		epeios::row__ Error = NONE;

#ifdef MSCVKX_DBG
		if ( _Datum != MSCVKP_UNDEFINED_DATUM )
			ERRc();
#endif

		_Datum = Pattern.ToUB( &Error, str::bAuto );

		if ( Error != NONE )
			return false;

		return true;
	}
	bso::bool__ _HandleName( const str::string_ &Value )
	{
		bso::bool__ Success = false;
	qRH
		str::string Name;
	qRB
		if ( Value.Amount() > 16 )
			qRReturn;

		Name.Init( Value );

		while ( Name.Amount() < 16 )
			Name.Append( ' ' );

		if ( _Append )
			_DataSet.Append( Name );
		else {
			if ( _DataSet.Amount() != 0 )
				_DataSets->Append( _DataSet );

			_DataSet.Init();

			_DataSet.SetAddress( _Address + _Offset);

			_DataSet.Append( Name );
		}

		Success = true;

	qRR
	qRT
	qRE
		return Success;
	}
	bso::bool__ _HandleBars( const str::string_ &Value )
	{
		bso::bool__ Success = false;
	qRH
		str::string Bars;
		epeios::row__ Row = NONE;
	qRB
		if ( mscvkp::IsPedalBar( _Address + _Offset ) ) {
			if ( Value.Amount() != 2 )
				qRReturn;
		} else if ( Value.Amount() != 9 )
			qRReturn;

		Bars.Init();

		Row = Value.First();

		while ( Row != NONE ) {
			if ( ( Value( Row ) < '0' ) || ( Value( Row ) > '8' ) )
				qRReturn;

			Bars.Append( Value( Row ) - '0' );

			Row = Value.Next ( Row );
		}

		if ( _Append )
			_DataSet.Append( Bars );
		else {
			if ( _DataSet.Amount() != 0 )
				_DataSets->Append( _DataSet );

			_DataSet.Init();

			_DataSet.SetAddress( _Address + _Offset );

			_DataSet.Append( Bars );
		}

		Success = true;

	qRR
	qRT
	qRE
		return Success;
	}
	bso::bool__ _HandleSpecial( const str::string_ &Value )
	{
		if ( mscvkp::IsName( _Address + _Offset ) )
			return _HandleName( Value );
		else if ( mscvkp::IsBar( _Address + _Offset) )
			return _HandleBars( Value );
		else
			return false;

		return true;
	}
protected:
	virtual bso::bool__ XMLProcessingInstruction( const xml::dump_ &Dump )
	{
		return true;
	}
	virtual bso::bool__ XMLStartTag(
		const str::string_ &Name,
		const xml::dump_ &)
	{
		_IgnoreLabel = true;

		if ( Name == PARAMETERS )
			_Ignore = true;
		else if ( Name == GROUP )
			_Ignore = true;
		else if ( Name == SYSTEM )
			_Ignore = true;
		else if ( Name == SYSTEM_BASIC )
			_Ignore = true;
		else if ( Name == SYSTEM_MIDI )
			_Ignore = true;
		else if ( Name == ORCHESTRAL_VOICES )
			_Ignore = true;
		else if ( Name == ORCHESTRAL_VOICE )
			_Ignore = false;
		else if ( Name == REGISTRATIONS )
			_Ignore = true;
		else if ( Name == REGISTRATION ) {
			_Ignore = false;
			_IgnoreLabel = false;
		} else if ( Name == PARAMETER )
			_Ignore = false;
		else
			return false;

		return true;
	}
	virtual bso::bool__ XMLStartTagClosed(
		const str::string_ &Name,
		const xml::dump_ & )
	{
		return true;
	}
	virtual bso::bool__ XMLAttribute(
		const str::string_ &TagName,
		const str::string_ &Name,
		const str::string_ &Value,
		const xml::dump_ & )
	{
		if ( !_Ignore ) {
			if ( Name == ADDRESS ) {
				if ( !_HandleAddress( Value, true ) )
					return false;
				_IgnoreLabel = true;
			} else if ( Name == OFFSET ) {
				if ( !_HandleAddress( Value, false ) )
					return false;
				_IgnoreLabel = true;
			} else if ( Name == BASE_OFFSET ) {
				if ( !_HandleAddress( Value, false ) )
					return false;
				_Special = true;
				_IgnoreLabel = true;
			} else if ( Name == LABEL ) {
				if ( !_IgnoreLabel )
					if ( !_HandleLabel( Value ) )
						return false;
			} else if ( Name == VALUE ) {
				if ( _Special )
					return false;
				else if ( !_HandleValue( Value ) )
					return false;
			} else
				return false;
		}

		return true;
	}
	virtual bso::bool__ XMLValue(
		const str::string_ &TagName,
		const str::string_ &Value,
		const xml::dump_ & )
	{
		if ( !_Ignore )
			if ( _Special ) {
				if ( !_HandleSpecial( Value ) )
					return false;
			} else
				return false;

		return true;
	}
	virtual bso::bool__ XMLEndTag(
		const str::string_ &Name,
		const xml::dump_ & )
	{
		if ( !_Ignore && !_Special && ( _Datum != MSCVKP_UNDEFINED_DATUM ) ) {
			if ( _Append )
				_DataSet.Append( _Datum );
			else {
#ifdef MSCVKP_DBG
				if ( _DataSets == NULL )
					ERRu();
#endif

				if ( _DataSet.Amount() != 0 )
					_DataSets->Append( _DataSet );

				_DataSet.Init();

				_DataSet.SetAddress( _Address + _Offset );
				_DataSet.Append( _Datum );
			}

			_Datum = MSCVKP_UNDEFINED_DATUM;

			_Special = false;
		}

		_Ignore = true;
		_Special = false;

		return true;
	}
public:
	callback__( void )
	{
		_DataSets = NULL;
	}
	void Init( mscvkp::data_sets_ &DataSets )
	{
		_DataSets = &DataSets;
		_DataSet.Init();

		_Ignore = false;
		_Append = false;
		_Special = false;

		_Address = 0;
		_Offset = 0;
		_Datum = MSCVKP_UNDEFINED_DATUM;
	}
	void Dismiss( void )
	{
#ifdef MSCVKP_DBG
		if ( _DataSets == NULL )
			ERRu();
#endif
		if ( _DataSet.Amount() != 0 )
			_DataSets->Append( _DataSet );
	}
};

xpp::status__ mscvkx::Parse( 
	flw::iflow__ &IFlow,
	const str::string_ &BaseDirectory,
	mscvkp::data_sets_ &DataSets,
	xtf::coord__ &ErrorCoords,
	str::string_ &GuiltyFileName )
{
	xpp::status__ Status = xpp::s_Undefined;
qRH
	xpp::preprocessing_iflow___ PFlow;
	callback__ Callback;
	xtf::extended_text_iflow__ XFlow;
qRB
	Callback.Init( DataSets );

	PFlow.Init( IFlow, BaseDirectory );
	XFlow.Init( PFlow );

	if ( xml::Parse( XFlow, true, Callback ) == xml::sOK ) {
		Callback.Dismiss();
		Status = xpp::sOK;
	} else {
		GuiltyFileName = PFlow.LocalizedFileName();
		Status = PFlow.Status();
	}

qRR
qRT
qRE
	return Status;
}
