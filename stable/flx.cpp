/*
	'flx' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'flx' header file ('flx.h').
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: flx.cpp,v 1.62 2013/04/05 16:33:35 csimon Exp $

#define FLX__COMPILATION

#include "flx.h"

class flxtutor
: public ttr_tutor
{
public:
	flxtutor( void )
	: ttr_tutor( FLX_NAME )
	{
#ifdef FLX_DBG
		Version = FLX_VERSION "\b\bD $";
#else
		Version = FLX_VERSION;
#endif
		Owner = FLX_OWNER;
		Date = "$Date: 2013/04/05 16:33:35 $";
	}
	virtual ~flxtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

flx::void_oflow_driver___ flx::VoidOFlowDriver;
flx::void_oflow__ flx::VoidOFlow;

flx::void_iflow_driver___ flx::VoidIFlowDriver;
flx::void_iflow__ flx::VoidIFlow;


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class flxpersonnalization
: public flxtutor
{
public:
	flxpersonnalization( void )
	{
		flx::VoidOFlowDriver.Init( fdr::ts_Default );
		flx::VoidOFlow.Init();

		flx::VoidIFlowDriver.Init( fdr::ts_Default );
		flx::VoidIFlow.Init();

		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~flxpersonnalization( void )
	{
		// Les 4 lignes ci-dessous ne devraient pas être nécessaire, mais si absentes, problême de 'pure virtual function call' (du moins sous VC++10).
		flx::VoidIFlow.reset();
		flx::VoidIFlowDriver.reset();

		flx::VoidOFlow.reset();
		flx::VoidOFlowDriver.reset();


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

static flxpersonnalization Tutor;

ttr_tutor &FLXTutor = Tutor;
