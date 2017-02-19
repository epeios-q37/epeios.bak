/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of xppq.

	xppq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	xppq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with xppq. If not, see <http://www.gnu.org/licenses/>.
*/

#include <node.h>
#include <nan.h>

#include "parser.h"
#include "stream.h"

#include "cio.h"
#include "scln.h"
#include "v8qnjs.h"
#include "xpp.h"
#include "mtk.h"

void scln::SCLNRegister( scln::sRegistrar &Registrar )
{
	cio::Initialize(cio::GetConsoleSet() );
	Registrar.Register( stream::Set );
	Registrar.Register( parser::Parse );
}

SCLN_MODULE( xppq );

