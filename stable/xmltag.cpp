/*
  'xmltag' library by Claude L. Simon (csimon@webmails.com)
  Requires the 'xmltag' header file ('xmltag.h').
  Copyright (C) 2000,2001 Claude L. SIMON (csimon@webmails.com).

  This file is part of the Epeios (http://epeios.org/) project.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, go to http://www.fsf.org/
  or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#define XMLTAG__COMPILATION

#include "xmltag.h"


#include "stf.h"

using namespace xmltag;

tag_row__ xmltag::tags_::Position(
	const name_ &Name,
	tag_row__ Tag,
	type Type ) const
{
	ctn::E_CMITEMt( tag_, tag_row__ ) Item;
	tag_row__ P;

	Item.Init( *this );

	P = basic_::First( Tag );

	while( ( P != NONE )
	       && ( ( Item( P ).Type() != Type )
		        || ( Item( P ).Name != Name ) ) ) {
		P = basic_::Next( P );
	}

	return P;
}

tag_row__ xmltag::tags_::Init( void )
{
	tag_row__ NRef;
ERRProlog
	tag Tag;
ERRBegin
	Tag.Init( str::string( "" ), xmltag::tRoot );

	NRef = basic_::Init( Tag );
ERRErr
ERREnd
ERREpilog
	return NRef;
}

static bso::bool__ WriteXMLAttributes_(
	const tags_ &Tags,
	tag_row__ TagRow,
	txf::text_oflow___ &Flow )
{
	ctn::E_CMITEMt( tag_, tag_row__ ) Tag;
	bso::bool__ HasChild = false;
	
	Tag.Init( Tags );
	
	TagRow = Tags.First( TagRow );
	
	while( TagRow != NONE ) {
		if ( Tag( TagRow ).IsAttribute() )
			Flow << ' ' << Tag( TagRow ).Name << "=\"\"";
		else
			HasChild = true;
			
		TagRow = Tags.Next( TagRow );
	}
	
	return HasChild;
}

	

static void WriteXMLWithEmptyTag_(
	const tags_ &Tags,
	txf::text_oflow___ &Flow )
{
	dtr::browser__<tag_row__> Browser;
	ctn::E_CMITEMt( tag_, tag_row__ ) Tag;
	bso::bool__ NoRegularChild = false;
	
	Tag.Init( Tags );
	
	Browser.Init( Tags.GetRoot() );
	
	do {
		switch( Browser.Status() ) {
		case dtr::bParent:
		case dtr::bLast:
			if ( !NoRegularChild )
				Flow << "</" << Tag( Browser.Position() ).Name << ">";
			NoRegularChild = false;
			break;
		case dtr::bBrother:
			if ( !Tag( Browser.Position() ).IsAttribute() ) {
				Flow << "<" << Tag( Browser.Position() ).Name;
				if ( !WriteXMLAttributes_( Tags, Browser.Position(), Flow ) )
					Flow << '/';
				Flow << '>';
				NoRegularChild = false;
			}
			break;
		case dtr::bChild:
			NoRegularChild = true;
			if ( !Tag( Browser.Position() ).IsAttribute() ) {
				Flow << "<" << Tag( Browser.Position() ).Name;
				if ( !WriteXMLAttributes_( Tags, Browser.Position(), Flow ) )
					Flow << '/';
				Flow << '>';
				NoRegularChild = false;
			}
			break;
		case dtr::bFirst:
			Flow << "<" << Tag( Browser.Position() ).Name;
			if ( !WriteXMLAttributes_( Tags, Browser.Position(), Flow ) )
				Flow << '/';
			Flow << '>';
			break;
		default:
			ERRc();
			break;
		}
	
	} while( Tags.Browse( Browser ) != NONE );
}

static void WriteXMLNoEmptyTag_(
	const tags_ &Tags,
	txf::text_oflow___ &Flow )
{
	dtr::browser__<tag_row__> Browser;
	tag_row__ Precedent = NONE;
	ctn::E_CMITEMt( tag_, tag_row__ ) Tag;
	bso::bool__ NoRegularChild = true;
	bso::bool__ JustParent = false;
	
	Tag.Init( Tags );
	
	Browser.Init( Tags.GetRoot() );

	do {	
		switch( Browser.Status() ) {
		case dtr::bParent:
		case dtr::bLast:
				if ( !JustParent && !Tag( Precedent ).IsAttribute() )
					Flow << "</" << Tag( Precedent ).Name << '>';
				Flow << "</" << Tag( Browser.Position() ).Name << '>';
				JustParent = true;
			break;
		case dtr::bBrother:
			if ( !Tag( Browser.Position() ).IsAttribute() ) {
				if ( !Tag( Precedent ).IsAttribute() && !JustParent )
					Flow << "</" << Tag( Precedent ).Name << '>';
				Flow << "<" << Tag( Browser.Position() ).Name;
				WriteXMLAttributes_( Tags, Browser.Position(), Flow );
				Flow << '>';
				NoRegularChild = false;
				JustParent = false;
			}
			break;
		case dtr::bChild:
			if ( !Tag( Browser.Position() ).IsAttribute() ) {
				Flow << "<" << Tag( Browser.Position() ).Name;
				WriteXMLAttributes_( Tags, Browser.Position(), Flow );
				Flow << '>';
				JustParent = false;
			}
			break;
		case dtr::bFirst:
			Flow << '<' << Tag( Browser.Position() ).Name;
			if ( WriteXMLAttributes_( Tags, Browser.Position(), Flow ) )
				Flow << '>';
			else
				Flow << "></" << Tag( Browser.Position() ).Name << '>';
			JustParent = false;
			break;
		default:
			ERRc();
			break;
		}
		
		Precedent = Browser.Position();
	} while( Tags.Browse( Browser ) != NONE );
}

void xmltag::WriteXML(
	const tags_ &Tags,
	txf::text_oflow___ &Flow,
	mode Mode )
{
	switch ( Mode ) {
	case mWithEmptyTag:
		WriteXMLWithEmptyTag_( Tags, Flow );
		break;
	case mNoEmptyTag:
		WriteXMLNoEmptyTag_( Tags, Flow );
		break;
	default:
		ERRu();
		break;
	}
}

void xmltag::Merge(
	const tags_ &Source,
	tags_ &Target,
	tag_map_ &TagMap )
{
ERRProlog
	tag_row__ TargetRow, IntermediateRow;
	ctn::E_CMITEMt( tag_, tag_row__ ) SourceItem, TargetItem;
	dtr::browser__<tag_row__> Browser;
	tag Buffer;
ERRBegin
	SourceItem.Init( Source );
	TargetItem.Init( Target );
	
	TagMap.Init();
	
	TagMap.Allocate( Source.Amount() );
	
	TargetRow = Target.GetRoot();
	
	Browser.Init( Source.GetRoot() );
	
	while( Browser.Position() != NONE ) {
		switch( Browser.Status() ) {
		case dtr::bChild:
			if ( ( IntermediateRow = Target.Position( SourceItem( Browser.Position() ).Name, TargetRow, SourceItem( Browser.Position() ).Type() ) ) == NONE ) {
				Buffer.Init( SourceItem().Name, SourceItem().Type() );
				IntermediateRow = Target.BecomeLast( Buffer, TargetRow );
			}
			TagMap.Write( IntermediateRow, Browser.Position() );
			TargetRow = IntermediateRow;			
			break;									
		case dtr::bBrother:
			if ( ( IntermediateRow = Target.Position( SourceItem( Browser.Position() ).Name, Target.Parent( TargetRow ), SourceItem( Browser.Position() ).Type() ) ) == NONE ) {
				Buffer.Init( SourceItem().Name, SourceItem().Type() );
				IntermediateRow = Target.BecomeNext( Buffer, TargetRow );
			}
			TagMap.Write( IntermediateRow, Browser.Position() );
			TargetRow = IntermediateRow;			
			break;									
		case dtr::bParent:
			TargetRow = Target.Parent( TargetRow );
			break;
		case dtr::bLast:
			break;
		case dtr::bFirst:
			if ( SourceItem( Browser.Position() ).Name != TargetItem( Target.GetRoot() ).Name )
				ERRu();
			TagMap.Write( Target.GetRoot(), Browser.Position() );
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
