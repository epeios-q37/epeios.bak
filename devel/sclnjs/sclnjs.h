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

// SoCLe Node.JS

#ifndef SCLNJS_INC_
# define SCLNJS_INC_

# define SCLNJS_NAME		"SCLNJS"

# if defined( E_DEBUG ) && !defined( SCLNJS_NODBG )
#  define SCLNJS_DBG
# endif

# include "err.h"

namespace sclnjs {
	void ErrFinal( v8::Isolate *Isolate = NULL );
	
	typedef void (* sFunction_)( const sArguments &);

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

	void SCLNJSRegister( sRegistrar &Registrar );	// To overload by user.
	extern const char *SCLNJSProductVersion;	// To define by user.
}

# define SCLNJS_MODULE( name ) NODE_MODULE( name, sclnjs::Register_ );

#endif
