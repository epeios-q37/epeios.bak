/*
  'xmltol' library by Claude L. Simon (csimon@webmails.com)
  Requires the 'xmltol' header file ('xmltol.h').
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

#define XMLTOL__COMPILATION

#include "xmltol.h"

#include "stf.h"

using namespace xmltol;

void xmltol::Convert(
	const xml_document_ &XMLDCM,
	xml_database_ &XMLDBS )
{
ERRProlog
	dtr::browser__<value_row__> Browser;
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;
	ctn::E_CMITEMt( tag_, tag_row__ ) Tag;
	tag_row__ TagRow;
	xml_database_filler__ XMLDF;
	bso::bool__ MakePop = false;
	bso::bool__ WasPush = false;
ERRBegin
	Browser.Init( XMLDCM.GetRoot() );

	XMLDBS.Init();

	XMLDBS.tagged_values_::Init( XMLDCM.Tags.GetRoot() );

	XMLDBS.Tags = XMLDCM.Tags;

	TaggedValue.Init( XMLDCM );

	XMLDF.Init( XMLDBS );
	
	Tag.Init( XMLDCM.Tags );

	while( Browser.Position() != NONE ) {
		switch( Browser.Status() ) {
		case dtr::bFirst:
			XMLDF.PushTag( XMLDCM.Tags.GetRoot() );
			MakePop = true;
			break;
		case dtr::bLast:
		case dtr::bParent:
			if ( MakePop ) {
				XMLDF.PopTag();
			}
			MakePop = true;
			break;
		case dtr::bChild:
			if ( WasPush ) {
				MakePop = true;
				WasPush = false;
			} else {
				MakePop = false;
				WasPush = false;
			}
		case dtr::bBrother:
			TagRow = TaggedValue( Browser.Position() ).TagRow();
			
			if ( ( TagRow != NONE ) && Tag( TagRow ).IsAttribute() ) {
				XMLDF.PutAttribute( TagRow, TaggedValue( Browser.Position() ).Value );
			} else {
				XMLDF.PutValue( TaggedValue( Browser.Position() ).Value );
				if ( TagRow != NONE ) {
					XMLDF.PushTag( TagRow );
					WasPush = true;
				}
			}
			break;
		default:
			ERRc();
		}

		XMLDCM.Browse( Browser );
	}
ERRErr
ERREnd
ERREpilog
}

void xmltol::Replace(
	tags_ &Tags,
	const xml_database_ &NewTags )
{
ERRProlog
	ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;
	dtr::browser__< value_row__ > Browser;
	tag_map TagMap;
ERRBegin
	TagMap.Init();

	Merge( NewTags.Tags, Tags, TagMap );

	Browser.Init( NewTags.GetRoot() );
	
	TaggedValue.Init( NewTags );
	
	while( Browser.Position() != NONE ) {
		switch( Browser.Status() ) {
		case dtr::bFirst:
		case dtr::bChild:
		case dtr::bBrother:
		if ( TaggedValue( Browser.Position() ).Value.Amount() != 0 )
			Tags( TagMap( TaggedValue( Browser.Position() ).TagRow() ) ).Name = TaggedValue( Browser.Position() ).Value;
			break;
		case dtr::bLast:
		case dtr::bParent:
			break;
		default:
			ERRc();
		}
		
		NewTags.Browse( Browser );
	}
	
	Tags.Sync();
	
ERRErr
ERREnd
ERREpilog
}

