/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

#define NODEQ__COMPILATION

#include "nodeq.h"

// Note to developer : you have to add '<path_to_node>/src' as 'Additional Include Directory' in the 'devel' related project.
#include <node_buffer.h>

using namespace nodeq;

namespace {
	void ConstFreeCallback_(
		char* data,
		void* hint)
	{
		// Does nothing, as called for const buffer on stack.
	}
}

void nodeq::sBuffer::Init(
	const char *Data,
	size_t Length,
	v8::Isolate *Isolate )
{
	Isolate = v8q::GetIsolate( Isolate );
	sValue::Init( v8q::ToLocal( node::Buffer::New( Isolate, (char *)Data, Length, ConstFreeCallback_, NULL ) ) );
}


txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const sBuffer &Buffer)
{
qRH
	sString String;
qRB
	String.Init();
	Buffer.ToString( String );

	Flow << String;
qRR
qRT
qRE
	return Flow;
}


