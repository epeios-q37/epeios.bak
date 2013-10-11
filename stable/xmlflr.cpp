/*
	'xmlflr' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xmlflr' header file ('xmlflr.h').
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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



//	$Id: xmlflr.cpp,v 1.2 2013/07/25 15:59:16 csimon Exp $

#define XMLFLR__COMPILATION

#include "xmlflr.h"

class xmlflrtutor
: public ttr_tutor
{
public:
	xmlflrtutor( void )
	: ttr_tutor( XMLFLR_NAME )
	{
#ifdef XMLFLR_DBG
		Version = XMLFLR_VERSION "\b\bD $";
#else
		Version = XMLFLR_VERSION;
#endif
		Owner = XMLFLR_OWNER;
		Date = "$Date: 2013/07/25 15:59:16 $";
	}
	virtual ~xmlflrtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace xmlflr;

static bso::bool__ WriteAttributes_(
	const xmldoc::document_ &Document,
	vrow__ Row,
	filler__ &Filler )
{
	bso::bool__ HasChild = false;
ERRProlog
	ctn::E_CMITEMt( value_, vrow__ ) Value;
	ctn::E_CMITEMt( name_, erow__ ) Name;
ERRBegin
	Value.Init( Document.Data );
	Name.Init( Document.Structure );

	Row = Document.Data.FirstChild( Row );

	HasChild = ( Row != E_NIL );

	while( Row != E_NIL ) {
		if ( Document.Structure.GetOddity( Document.Data.GetOddity( Row ) ) == xmlelm::tAttribute )
			Filler.PutAttribute( Name( Document.Data.GetOddity( Row ) ), Value( Row ) );
		Row = Document.Data.NextSibling( Row );
	}
ERRErr
ERREnd
ERREpilog
	return HasChild;
}

void xmlflr::filler__::Put(
	const xmldoc::document_ &Document,
	vrow__ Root )
{
ERRProlog
	dtr::browser__<vrow__> Browser;
	ctn::E_CMITEMt( value_, vrow__ ) Value;
	ctn::E_CMITEMt( name_, erow__ ) Name;
	str::string Result;
ERRBegin
	Value.Init( Document.Data );
	Name.Init( Document.Structure );

	PushTag( Name( Document.Data.GetOddity( Root ) ) );
	PutValue( Value( Root ) );

	Browser.Init( Root );
	Document.Data.Browse( Browser );

	while( Browser.Position() != E_NIL ) {
		switch( Browser.Kinship() ) {
		case dtr::kChild:
		case dtr::kSibling:
			if ( Document.Structure.GetOddity( Document.Data.GetOddity( Browser.Position() ) ) != xmlelm::tAttribute ) {
				PushTag( Name( Document.Data.GetOddity( Browser.Position() ) ) );
				PutValue( Value( Browser.Position() ) );
				if ( WriteAttributes_( Document, Browser.Position(), *this ) )
					break;
			} else
				break;
		case dtr::kParent:
			PopTag();
			break;
		default:
			ERRFwk();
			break;
		}

		Document.Data.Browse( Browser );
	}

	PopTag();
ERRErr
ERREnd
ERREpilog
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xmlflrpersonnalization
: public xmlflrtutor
{
public:
	xmlflrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xmlflrpersonnalization( void )
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

static xmlflrpersonnalization Tutor;

ttr_tutor &XMLFLRTutor = Tutor;
