/*
  'xmldcm' library by Claude L. Simon (csimon@webmails.com)
  Requires the 'xmldcm' header file ('xmldcm.h').
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

//	$Id: xmldcm.cpp,v 1.7 2012/11/14 16:06:40 csimon Exp $

#define XMLDCM__COMPILATION

#include "xmldcm.h"

class xmldcmtutor
: public ttr_tutor
{
public:
	xmldcmtutor( void )
	: ttr_tutor( XMLDCM_NAME )
	{
#ifdef XMLDCM_DBG
		Version = XMLDCM_VERSION "\b\bD $";
#else
		Version = XMLDCM_VERSION;
#endif
		Owner = XMLDCM_OWNER;
		Date = "$Date: 2012/11/14 16:06:40 $";
	}
	virtual ~xmldcmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xmldcmpersonnalization
: public xmldcmtutor
{
public:
	xmldcmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xmldcmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static xmldcmpersonnalization Tutor;

ttr_tutor &XMLDCMTutor = Tutor;
