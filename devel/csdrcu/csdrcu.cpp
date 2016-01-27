/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#define CSDRCU__COMPILATION

#include "csdrcu.h"

using namespace csdrcu;

bso::bool__ csdrcu::core___::Init(
	const str::string_ &PluginPath,
	const char *Identifier,
	const str::string_ &Parameters,
	err::handling__ ErrHandling )
{
	bso::bool__ Success = false;
qRH
	TOL_CBUFFER___ Buffer;
qRB
	Retriever_.Init();

	if ( !( Success = Retriever_.Initialize( PluginPath, Identifier, Parameters, err::hUserDefined ) ) ) {
		if ( ErrHandling == err::hThrowException )
			qRFwk();
		else
			qRReturn;
	}

	Driver_ = &Retriever_.Plugin();
	_driver___::Init( fdr::ts_Default );
qRR
qRT
qRE
	return Success;
}
