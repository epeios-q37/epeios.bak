/*
	'frdssn' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'frdssn' header file ('frdssn.h').
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

#define FRDSSN__COMPILATION

#include "frdssn.h"

using namespace frdssn;

bso::bool__ frdssn::session___::FillSessionRegistry(
	xtf::extended_text_iflow__ &SettingsXFlow,
	const xpp::criterions___ &Criterions,
	error_set___ &ErrorSet )
{
	bso::bool__ Success = false;
ERRProlog
	TOL_CBUFFER___ FileNameBuffer, PathBuffer;
ERRBegin
	_Test();

	switch ( _Registry.FillSession( SettingsXFlow, Criterions, FRDKRN_SESSION_LABEL, ErrorSet.Context ) ) {
	case rgstry::sOK:
		break;
	case rgstry::sUnableToFindRootPath:
		ERRFwk();
		break;
	default:
		ERRReturn;
		break;
	}

	Success = true;
	ERRErr
		ERREnd
		ERREpilog
		return Success;
}

bso::bool__ frdssn::session___::FillSessionRegistry(
	xtf::extended_text_iflow__ &SettingsXFlow,
	const xpp::criterions___ &Criterions )
{
	bso::bool__ Success = false;
ERRProlog
	error_set___ ErrorSet;
ERRBegin
	ErrorSet.Init();

Success = FillSessionRegistry( SettingsXFlow, Criterions, ErrorSet );
ERRErr
ERREnd
ERREpilog
	return Success;
}

