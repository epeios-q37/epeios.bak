/*
	'xulfui' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfui' header file ('xulfui.h').
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



//	$Id: xulfui.cpp,v 1.10 2013/01/16 16:37:36 csimon Exp $

#define XULFUI__COMPILATION

#include "xulfui.h"

class xulfuitutor
: public ttr_tutor
{
public:
	xulfuitutor( void )
	: ttr_tutor( XULFUI_NAME )
	{
#ifdef XULFUI_DBG
		Version = XULFUI_VERSION "\b\bD $";
#else
		Version = XULFUI_VERSION;
#endif
		Owner = XULFUI_OWNER;
		Date = "$Date: 2013/01/16 16:37:36 $";
	}
	virtual ~xulfuitutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "xulftk.h"

#include "lstbch.h"
#include "lstctn.h"

using namespace xulfui;

bso::bool__ xulfui::steering_callback__::GECKOORegister(
	nsIDOMWindow *Window,
	const str::string_ &Id )
{
	bso::bool__ Success = false;
ERRProlog
	err::buffer__ Buffer;
ERRBegin
	Success = XULFUIRegister( Window, Id );
ERRErr
	if ( ERRType != err::t_Abort )
		_Trunk->UI().Alert( err::Message( Buffer ) );
	else
		Success = true;
	ERRRst();
ERREnd
ERREpilog
	return Success;
}

void xulfui::steering_callback__::GECKOOPreRegistration( void )
{
ERRProlog
	err::buffer__ Buffer;
ERRBegin
	XULFUIPreRegistration();
ERRErr
	if ( ERRType != err::t_Abort )
		_Trunk->UI().Alert( err::Message( Buffer ) );

	ERRRst();
ERREnd
ERREpilog
}

void xulfui::steering_callback__::GECKOOPostRegistration( void )
{
ERRProlog
	err::buffer__ Buffer;
ERRBegin
	XULFUIPostRegistration();
ERRErr
	if ( ERRType != err::t_Abort )
		_Trunk->UI().Alert( err::Message( Buffer ) );

	ERRRst();
ERREnd
ERREpilog
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulfuipersonnalization
: public xulfuitutor
{
public:
	xulfuipersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulfuipersonnalization( void )
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

static xulfuipersonnalization Tutor;

ttr_tutor &XULFUITutor = Tutor;
