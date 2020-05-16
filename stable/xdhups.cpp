/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define XDHUPS_COMPILATION_

#include "xdhups.h"

using namespace xdhups;

extern "C" typedef xdhcdc::retrieve retrieve;

bso::bool__ xdhups::rAgent::Init(
	xdhcuc::cGlobal &Upstream,
	xdhcdc::eMode Mode,
	const str::string_ &ModuleFileName,
	dlbrry::eNormalization Normalization,
	const char *Identification )
{
	bso::bool__ Success = false;
qRH
	xdhcdc::sData Data;
	fnm::name___ Location;
	TOL_CBUFFER___ Buffer;
qRB
	Location.Init();
	Data.Init( Mode, Identification, fnm::GetLocation( ModuleFileName, Location ).UTF8( Buffer ) );

	Library_.Init( ModuleFileName, Normalization );

	retrieve *Retrieve = dlbrry::GetFunction<retrieve *>( E_STRING( XDHCDC_RETRIEVE_FUNCTION_NAME ), Library_ );

	if ( Retrieve != NULL ) {
		Callback_ = Retrieve();

		if ( Callback_ == NULL )
			qRFwk();

		Callback_->Initialize(Data, Upstream);

		Success = true;
	}
qRR
qRT
qRE
	return Success;
}

bso::sBool xdhups::rAgent::_IsValid(const str::dString &Token)
{
	bso::sBool ReturnValue = false;
qRH
	str::wString DummmyHead;
qRB
	DummmyHead.Init();

	ReturnValue = GetHead(Token, DummmyHead);
qRR
qRT
qRE
	return ReturnValue;
}

