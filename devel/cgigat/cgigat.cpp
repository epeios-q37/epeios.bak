/*
	'cgigat.cpp' by Claude SIMON (http://zeusw.org/).

	'cgigat' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#define CGIGAT__COMPILATION

#include "cgigat.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

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



csdscb::action__ cgigat::cgi_gate__::CSDSCBProcess(
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
	return csdscb::aStop;	// Pour fermer la connection.
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class cgigatpersonnalization
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


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static cgigatpersonnalization Tutor;
