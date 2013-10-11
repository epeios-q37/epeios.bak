/*
	'epsmsc' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'epsmsc' header file ('epsmsc.h').
	Copyright (C) 2000, 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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



//	$Id: epsmsc.cpp,v 1.25 2012/12/13 11:36:01 csimon Exp $

#define EPSMSC__COMPILATION

#include "epsmsc.h"

class epsmsctutor
: public ttr_tutor
{
public:
	epsmsctutor( void )
	: ttr_tutor( EPSMSC_NAME )
	{
#ifdef EPSMSC_DBG
		Version = EPSMSC_VERSION "\b\bD $";
#else
		Version = EPSMSC_VERSION;
#endif
		Owner = EPSMSC_OWNER;
		Date = "$Date: 2012/12/13 11:36:01 $";
	}
	virtual ~epsmsctutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace epsmsc;
using namespace txf;

void epsmsc::PrintLicense( txf::text_oflow__ &Flow )
{
	Flow 
	<< "    This program is free software: you can redistribute it and/or modify" << nl
	<< "    it under the terms of the GNU General Public License as published by" << nl
	<< "    the Free Software Foundation, either version 3 of the License, or" << nl
	<< "    (at your option) any later version." << nl
	<< nl
	<< "    This program is distributed in the hope that it will be useful," << nl
	<< "    but WITHOUT ANY WARRANTY; without even the implied warranty of" << nl
	<< "    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << nl
	<< "    GNU General Public License for more details." << nl
	<< nl
	<< "    You should have received a copy of the GNU General Public License" << nl
	<< "    along with this program.  If not, see <http://www.gnu.org/licenses/>." << nl;

}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class epsmscpersonnalization
: public epsmsctutor
{
public:
	epsmscpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~epsmscpersonnalization( void )
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

static epsmscpersonnalization Tutor;

ttr_tutor &EPSMSCTutor = Tutor;
