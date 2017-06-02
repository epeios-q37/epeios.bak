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

// Note to developer : include 'h:\hg\NJSIncludeDirectories.props' in the '.vcxproj'.
# include <node.h>

# include "err.h"
# include "v8q.h"

namespace sclnjs {
	void ErrFinal( v8::Isolate *Isolate = NULL );
	
	void Register_(
		v8::Local<v8::Object> Exports,
		v8::Local<v8::Value> Module,
		void* priv );

	template <typename item> inline void Get_(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> Infos,
		item &Item )
	{
		if ( Index >= Infos.Length()  )
			qRFwk();

		Item.Init( Infos[Index] );
	}

	template <typename item, typename ...items> inline void Get_(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> Infos,
		item &Item,
		items &...Items )
	{
		Get_( Index, Infos, Item );

		Get_( Index+1, Infos, Items... );
	}


	class sArguments {
	private:
		qRMV( const v8::FunctionCallbackInfo<v8::Value>, A_, Arguments_ );
	public:
		void reset( bso::sBool P = true )
		{
			Arguments_ = NULL;
		}
		qCDTOR( sArguments );
		void Init( const v8::FunctionCallbackInfo<v8::Value> &Arguments )
		{
			Arguments_ = &Arguments;
		}
		template <typename item> void Get(
			bso::sUInt Index,
			item &Item ) const
		{
			if ( Index == 0 )
				qRFwk();

			Get_( Index, A_(), Item );
		}
		template <typename ...items> inline void Get( items &...Items ) const
		{
			Get_( 1, A_(), Items... );
		}
		template <typename item> void SetReturnValue( const item &Item ) const
		{
			A_().GetReturnValue().Set( Item.Core() );
		}
		void This( v8q::sObject &This )
		{
			This.Init( A_().This() );
		}
		v8::Isolate *Isolate( void ) const
		{
			return A_().GetIsolate();
		}
	};

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
