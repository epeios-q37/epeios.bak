/*
	'cgigat' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'cgigat' header file ('cgigat.h').
	Copyright (C) 2001-2004 Claude SIMON.

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



//	$Id: cgigat.cpp,v 1.2 2013/01/24 08:25:24 csimon Exp $

#define CGIGAT__COMPILATION

#include "cgigat.h"

class cgigattutor
: public ttr_tutor
{
public:
	cgigattutor( void )
	: ttr_tutor( CGIGAT_NAME )
	{
#ifdef CGIGAT_DBG
		Version = CGIGAT_VERSION "\b\bD $";
#else
		Version = CGIGAT_VERSION;
#endif
		Owner = CGIGAT_OWNER;
		Date = "$Date: 2013/01/24 08:25:24 $";
	}
	virtual ~cgigattutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "flx.h"

using namespace cgigat;

void cgigat::cgi_gate__::_Process(
	const cgiarg::arguments_ &Arguments,
	str::string_ &XML,
	str::string_ &XSLFileName )
{
ERRProlog
	flx::E_STRING_OFLOW___ SFlow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
ERRBegin
	SFlow.Init( XML );
	TFlow.Init( SFlow );
	Writer.Init( TFlow, xml::oCompact, xml::e_Default );

	_Process( Arguments, Writer, XSLFileName );
ERRErr
ERREnd
ERREpilog
}



csdsuf::action__ cgigat::cgi_gate__::CSDSUFProcess(
	flw::ioflow__ &Flow,
	void *UP )
{
ERRProlog
	cgiarg::arguments Arguments;
	str::string XML;
	str::string XSLFileName;
	txf::text_oflow__ TFlow;
ERRBegin
	Arguments.Init();
	Arguments.Parse( Flow );

	XML.Init();
	XSLFileName.Init();

	_Process( Arguments, XML, XSLFileName );

	TFlow.Init( Flow );

	TFlow << XSLFileName << txf::nl << XML << txf::commit;
ERRErr
ERREnd
ERREpilog
	return csdsuf::aStop;	// Pour fermer la connection.
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class cgigatpersonnalization
: public cgigattutor
{
public:
	cgigatpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~cgigatpersonnalization( void )
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

static cgigatpersonnalization Tutor;

ttr_tutor &CGIGATTutor = Tutor;
