/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
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

