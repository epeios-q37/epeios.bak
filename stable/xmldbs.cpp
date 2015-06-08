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

#define XMLDBS__COMPILATION

#include "xmldbs.h"


using namespace xmldbs;

value_row__ xml_database_::Next(
	value_row__ ValueRow,
	const value_ &Value ) const
{
	value_row__ &P = ValueRow;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;
		
	TaggedValue.Init( *this );

	do
		P = Next( P );
	while( ( P != NONE ) && ( TaggedValue( P ).Value != Value ) );
		
	return P;
}

value_row__ xml_database_::Next(
	value_row__ ValueRow,
	tag_row__ TagRow,
	const value_ &Value ) const
{
	value_row__ &P = ValueRow;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;
		
	TaggedValue.Init( *this );

	do	
		P = Next( P, TagRow );
	while( ( P != NONE ) && ( TaggedValue( P ).Value != Value ) );
		
	return P;
}



value_row__ xml_database_::First(
	value_row__ ValueRow,
	const value_ &Value ) const
{
	value_row__ &P = ValueRow;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;
		
	TaggedValue.Init( *this );
	
	P = First( P );

	if ( ( P != NONE ) && ( TaggedValue( P ).Value != Value ) )
		P = Next( ValueRow, Value );
		
	return P;
}

value_row__ xml_database_::First(
	value_row__ ValueRow,
	tag_row__ TagRow,
	const value_ &Value ) const
{
	value_row__ &P = ValueRow;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;
		
	TaggedValue.Init( *this );
	
	P = First( P, TagRow );

	if ( ( P != NONE ) && ( TaggedValue( P ).Value != Value ) )
		P = Next( P, TagRow, Value );
		
	return P;
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
	const xml_database_ &XMLD,
	value_row__ ValueRow,
	txf::text_oflow___ &Flow )
{
	bso::bool__ Return = false;
	ctn::E_CMITEMt( tag_, tag_row__ ) Tag;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;

	Tag.Init( XMLD.Tags );
	TaggedValue.Init( XMLD );

	if ( ( ValueRow = XMLD.First( ValueRow ) ) != NONE )
		Return = true;

	while( ValueRow != NONE ) {
		if ( Tag( TaggedValue( ValueRow ).TagRow() ).Type() == tAttribute ) {
			Flow << ' ' << Tag( TaggedValue( ValueRow ).TagRow() ).Name << "=\"";
			Flow << Filter_( TaggedValue( ValueRow ).Value ) << '"';
		}

		ValueRow = XMLD.Next( ValueRow );
	}

	return Return;
}

void xmldbs::WriteXML(
	const xml_database_ &XMLD,
	txf::text_oflow___ &Flow )
{
qRH
	dtr::browser__<value_row__> Browser;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;
	ctn::E_CMITEMt( tag_, tag_row__ ) Tag;
	tag_row__ TagRow;
	bso::bool__ HasChild;
qRB
	Browser.Init( XMLD.GetRoot() );
	TaggedValue.Init( XMLD );
	Tag.Init( XMLD.Tags );
	
	while( Browser.Position() != NONE ) {
		switch( Browser.Status() ) {
		case dtr::bFirst:
		case dtr::bChild:
		case dtr::bBrother: 
			if ( !Tag( TagRow = TaggedValue( Browser.Position() ).TagRow() ).IsAttribute() ) {
				Flow << "<" << Tag( TagRow ).Name;
				HasChild = WriteAttributes_( XMLD, Browser.Position(), Flow );
				Flow << '>';
				Flow << Filter_( TaggedValue( Browser.Position() ).Value );
			}
			
			if ( HasChild )
				break;			
		case dtr::bParent:
		case dtr::bLast:
			if ( !Tag( TaggedValue( Browser.Position() ).TagRow() ).IsAttribute() )
				Flow << "</" << Tag( TaggedValue( Browser.Position() ).TagRow() ).Name << '>';
			break;
		default:
			ERRc();
			break;
		}
				
		XMLD.Browse( Browser );
	}
qRR
qRT
qRE
}

