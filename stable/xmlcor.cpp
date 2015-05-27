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

#define XMLCOR__COMPILATION

#include "xmlcor.h"

using namespace xmlcor;

void xmlcor::Add(
	const tagged_values_ &Source,
	const tag_map_ &TagMap,
	tagged_values_ &Target )
{
ERRProlog
	value_row__ TargetRow;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) SourceItem, TargetItem;
	dtr::browser__<value_row__> Browser;
	tagged_value Buffer;
ERRBegin
	SourceItem.Init( Source );
	TargetItem.Init( Target );
	
	TargetRow = Target.GetRoot();
	
	Browser.Init( Source.GetRoot() );
	
	Buffer.Init();
	
	while( Browser.Position() != NONE ) {
		switch( Browser.Status() ) {
		case dtr::bChild:
			if ( ( Buffer = SourceItem( Browser.Position() ) ).TagRow() != NONE )
				Buffer.TagRow( TagMap( Buffer.TagRow() ) );
				
			TargetRow = Target.BecomeLast( Buffer, TargetRow );
			break;									
		case dtr::bBrother:
			if ( ( Buffer = SourceItem( Browser.Position() ) ).TagRow() != NONE )
				Buffer.TagRow( TagMap( Buffer.TagRow() ) );
				
			TargetRow = Target.BecomeNext( Buffer, TargetRow );
			break;									
		case dtr::bParent:
		case dtr::bLast:
			TargetRow = Target.Parent( TargetRow );
			break;
		case dtr::bFirst:
			break;
		default:
			ERRc();
			break;
		}
		
		Source.Browse( Browser );
	}
ERRErr
ERREnd
ERREpilog
}

value_row__ xml_core_::Next(
	value_row__ ValueRow,
	tag_row__ TagRow ) const
{
	value_row__ &P = ValueRow;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;
	
	TaggedValue.Init( *this );
		
	do	
		P = Next( P );
	while( ( P != NONE ) && ( TaggedValue( P ).TagRow() != TagRow ) );
		
	return P;
}

value_row__ xml_core_::First(
	value_row__ ValueRow,
	tag_row__ TagRow ) const
{
	value_row__ &P = ValueRow;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;
	
	TaggedValue.Init( *this );
	
	P = First( P );

	if ( ( P != NONE ) && ( TaggedValue( P ).TagRow() != TagRow ) )
		P = Next( P, TagRow );
		
	return P;
}

void xmlcor::Convert( str::string_ &Target )
{
ERRProlog
	epeios::row__ Position = Target.First();
	bso::char__ C;
	str::string Buffer;
ERRBegin
	while( Position != NONE ) {
		switch ( C = Target( Position ) ) {
		case '"':
			Buffer.Init( "&#34;" );
			Target.Delete( Position );
			Target.Insert( Buffer, Position );
			break;
		case '<':
			Buffer.Init( "&lt;" );
			Target.Delete( Position );
			Target.Insert( Buffer, Position );
			break;
		case '>':
			Buffer.Init( "&gt;" );
			Target.Delete( Position );
			Target.Insert( Buffer, Position );
			break;
		case '&':
			Buffer.Init( "&amp;" );
			Target.Delete( Position );
			Target.Insert( Buffer, Position );
			break;
		default:
			break;
		}
		
		Position = Target.Next( Position );	// Could be dangerous, but actually works.
	}
ERRErr
ERREnd
ERREpilog
}
