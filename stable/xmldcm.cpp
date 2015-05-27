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

#define XMLDCM__COMPILATION

#include "xmldcm.h"


using namespace xmldcm;

void xml_document_::GetValue(
	value_row__ Row,
	value_ &Value ) const
{
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;

	TaggedValue.Init( *this );

	Row = First( Row );

	while( Row != NONE ) {
		Value.Add( TaggedValue( Row ).Value );
		Row = Next( Row );
	}
}

static inline const str::string_  &Filter_( const str::string_ &String )
{
	static str::string Result;
	
	Result.Init();
	
	Result = String;
	
	xmlcor::Convert( Result );
	
	return Result;
}
	
static bso::bool__ WriteAttributes_(
	const xml_document_ &XMLD,
	value_row__ ValueRow,
	txf::text_oflow___ &Flow )
{
	bso::bool__ Return = false;
	ctn::E_CMITEMt( tag_, tag_row__ ) Tag;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;

	Tag.Init( XMLD.Tags );
	TaggedValue.Init( XMLD );

	if ( TaggedValue( ValueRow ).Value.Amount() != 0 )
		Return = true;

	ValueRow = XMLD.First( XMLD.Parent( ValueRow ) );

	while( ValueRow != NONE ) {
		if ( Tag( TaggedValue( ValueRow ).TagRow() ).Type() == tAttribute ) {
			Flow << ' ' << Tag( TaggedValue( ValueRow ).TagRow() ).Name << "=\"";
			Flow << Filter_( TaggedValue( ValueRow ).Value ) << '"';
		} else if ( !Return )
			Return = true;

		ValueRow = XMLD.Next( ValueRow );
	}

	return Return;
}

void xmldcm::WriteXML(
	const xml_document_ &XMLD,
	txf::text_oflow___ &Flow,
	value_row__ Root )
{
ERRProlog
	dtr::browser__<value_row__> Browser;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;
	ctn::E_CMITEMt( tag_, tag_row__ ) Tag;
	tag_row__ TagRow;
ERRBegin
	Browser.Init( Root == NONE ? XMLD.GetRoot() : Root );
	TaggedValue.Init( XMLD );
	Tag.Init( XMLD.Tags );
	
	XMLD.Browse( Browser );
	
	while( Browser.Position() != NONE ) {
		switch( Browser.Status() ) {
		case dtr::bBrother: 
			if ( ( ( TagRow = TaggedValue( Browser.Position() ).TagRow() ) == NONE )
			     || !Tag( TagRow ).IsAttribute() ) {
				Flow << "</" << Tag( TaggedValue( XMLD.Previous( Browser.Position() ) ).TagRow() ).Name << '>';
				Flow << Filter_( TaggedValue( Browser.Position() ).Value );
			}
			break;
		case dtr::bChild:
			if ( ( ( TagRow = TaggedValue( Browser.Position() ).TagRow() ) == NONE )
			     || !Tag( TagRow ).IsAttribute() ) {
				Flow << '<' << Tag( TaggedValue( XMLD.Parent( Browser.Position() ) ).TagRow() ).Name;
				WriteAttributes_( XMLD, Browser.Position(), Flow );
				Flow << '>';
				Flow << Filter_( TaggedValue( Browser.Position() ).Value );
			}
			break;			
		case dtr::bParent:
			break;
		case dtr::bLast:
			Flow << "</" << Tag( TaggedValue( Browser.Position() ).TagRow() ).Name << '>';
			break;
		default:
			ERRc();
			break;
		}
				
		XMLD.Browse( Browser );
	}
	
	
	
ERRErr
ERREnd
ERREpilog
}

