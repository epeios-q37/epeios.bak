/*
	'xmldoc' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xmldoc' header file ('xmldoc.h').
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



//	$Id: xmldoc.cpp,v 1.3 2013/07/25 15:59:15 csimon Exp $

#define XMLDOC__COMPILATION

#include "xmldoc.h"

class xmldoctutor
: public ttr_tutor
{
public:
	xmldoctutor( void )
	: ttr_tutor( XMLDOC_NAME )
	{
#ifdef XMLDOC_DBG
		Version = XMLDOC_VERSION "\b\bD $";
#else
		Version = XMLDOC_VERSION;
#endif
		Owner = XMLDOC_OWNER;
		Date = "$Date: 2013/07/25 15:59:15 $";
	}
	virtual ~xmldoctutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace xmldoc;

using namespace xmlval;
using namespace xmlelm;

static bso::bool__ WriteAttributes_(
	const document_ &Document,
	vrow__ Row,
	txf::text_oflow__ &Flow )
{
	bso::bool__ HasChild = false;
ERRProlog
	ctn::E_CMITEMt( value_, vrow__ ) Value;
	ctn::E_CMITEMt( name_, erow__ ) Name;
	str::string Result;
ERRBegin
	Value.Init( Document.Data );
	Name.Init( Document.Structure );

	Row = Document.Data.FirstChild( Row );

	HasChild = ( Row != E_NIL );

	while( Row != E_NIL ) {
		if ( Document.Structure.GetOddity( Document.Data.GetOddity( Row ) ) == tAttribute ) {
			Flow << ' ' << Name( Document.Data.GetOddity( Row ) ) << "=\"";

			Result.Init();
			xmlbsc::TransformUsingEntities( Value( Row ), false, Result );
			Flow << Result << '"';
		}

		Row = Document.Data.NextSibling( Row );
	}
ERRErr
ERREnd
ERREpilog
	return HasChild;
}

void xmldoc::WriteXML(
	const document_ &Document,
	vrow__ Root,
	txf::text_oflow__ &Flow )
{
ERRProlog
	dtr::browser__<vrow__> Browser;
	ctn::E_CMITEMt( value_, vrow__ ) Value;
	ctn::E_CMITEMt( name_, erow__ ) Name;
	bso::bool__ HasChild;
	str::string Result;
ERRBegin
	Value.Init( Document.Data );
	Name.Init( Document.Structure );

	Flow << "<" << Name( Document.Data.GetOddity( Root ) );
	HasChild = WriteAttributes_( Document, Root, Flow );
	Flow << '>';

	Result.Init();
	xmlbsc::TransformUsingEntities( Value( Root ), false, Result );
	Flow << Result;

#ifdef XMLDOC_DBG
	Flow << txf::commit;
#endif

	Browser.Init( Root );
	Document.Data.Browse( Browser );

	while( Browser.Position() != E_NIL ) {
		switch( Browser.Kinship() ) {
		case dtr::kChild:
		case dtr::kSibling:
			if ( Document.Structure.GetOddity( Document.Data.GetOddity( Browser.Position() ) ) != tAttribute ) {
				Flow << "<" << Name( Document.Data.GetOddity( Browser.Position() ) );
				HasChild = WriteAttributes_( Document, Browser.Position(), Flow );
				Flow << '>';
#ifdef XMLDOC_DBG
				Flow << txf::commit;
#endif

				Result.Init();
				xmlbsc::TransformUsingEntities( Value( Browser.Position() ), false, Result );
				Flow << Result;
#ifdef XMLDOC_DBG
				Flow << txf::commit;
#endif
				if ( HasChild )
					break;
			} else
				break;
		case dtr::kParent:
			Flow << "</" << Name( Document.Data.GetOddity( Browser.Position() ) ) << '>';
#ifdef XMLDOC_DBG
			Flow << txf::commit;
#endif
			break;
		default:
			ERRFwk();
			break;
		}

		Document.Data.Browse( Browser );
	}

	Flow << "</" << Name( Document.Data.GetOddity( Root ) ) << '>';

#ifdef XMLDOC_DBG
	Flow << txf::commit;
#endif

ERRErr
ERREnd
ERREpilog
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xmldocpersonnalization
: public xmldoctutor
{
public:
	xmldocpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xmldocpersonnalization( void )
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

static xmldocpersonnalization Tutor;

ttr_tutor &XMLDOCTutor = Tutor;
