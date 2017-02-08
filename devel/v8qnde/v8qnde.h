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

// NoDE.js related extension to the 'v8q' module

#ifndef V8QNDE__INC
# define V8QNDE__INC

# define V8QNDE_NAME		"V8QNDE"

# if defined( E_DEBUG ) && !defined( V8QNDE_NODBG )
#  define V8QNDE_DBG
# endif

# include "v8q.h"
# include "err.h"

namespace v8qnde {
	using namespace v8q;

	class sBuffer
	: public sObject
	{
	public:
		qCDTOR( sBuffer );
		using sObject::Init;
		void Init( v8::Isolate *Isolate )
		{
			sObject::Init( v8q::Execute( Isolate, "require( 'buffer' );"), Isolate );
		}
		void Init(
			const char *String,
			v8::Isolate *Isolate )
		{
			Init( Isolate );
			Launch( "from", String );
		}
		void ToString(
			sString &String,
			v8::Isolate *Isolate )
		{
			String.Init( Launch( "toString" ), Isolate );
		}
	};

	class sStream
	: public sObject
	{
	public:
		qCDTOR( sStream );
		void OnData( const sFunction &Callback )
		{
			On( "data", Callback );
		}
		void Push( const sBuffer &Buffer )
		{
			Launch( "Push", Buffer );
		}
	};
}

#endif
