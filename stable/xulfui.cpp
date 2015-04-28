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

#define XULFUI__COMPILATION

#include "xulfui.h"


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
		nsxpcm::Alert( err::Message( Buffer ) );
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
