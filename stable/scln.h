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

// SoCLe Node.js

#ifndef SCLN__INC
# define SCLN__INC

# define SCLN_NAME		"SCLN"

# if defined( E_DEBUG ) && !defined( SCLN_NODBG )
#  define SCLN_DBG
# endif

# include <node.h>

# include "err.h"
# include "tol.h"
# include "v8q.h"

namespace scln {

	void SetPendingError( const str::dString &Error );

	void ErrFinal( v8::Isolate *Isolate = NULL );
	
	void Register_(
		v8::Local<v8::Object> Exports,
		v8::Local<v8::Value> Module,
		void* priv );

	typedef void (* sFunction_)( const v8q::sArguments &);

	class sRegistrar
	{
	private:
		qRMV( v8::Local<v8::Object>, E_, Exports_ );
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Exports_ );
		}
		qCDTOR( sRegistrar )
		void Init( v8::Local<v8::Object> &Exports )
		{
			Exports_ = &Exports;
		}
		void Register( sFunction_ Function );
		void Register(
			const char *Name,
			v8::FunctionCallback Function );
	};

	void SCLNRegister( sRegistrar &Registrar );	// To overload by user.
	extern const char *SCLNProductVersion;	// To define by user.
}

#define SCLN_MODULE( name ) NODE_MODULE( name, scln::Register_ );

#endif
