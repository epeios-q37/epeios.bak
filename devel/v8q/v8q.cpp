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

#define V8Q__COMPILATION

#include "v8q.h"

#include "ntvstr.h"
#include "flx.h"

using namespace v8q;

namespace {
	// Global isolate.
	v8::Isolate *Isolate_ = NULL;
}

void v8q::SetGlobalIsolate( v8::Isolate *Isolate )
{
	if ( Isolate_ != NULL )
		qRFwk();

	Isolate_ = Isolate;
}

v8::Isolate *v8q::GetGlobalIsolate( void )
{
	if ( Isolate_ == NULL )
		return v8::Isolate::GetCurrent();
	else
		return Isolate_;
}
