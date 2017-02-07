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

#define SCLN__COMPILATION

#include "scln.h"

#include "v8q.h"
#include "bch.h"

using namespace scln;

namespace {
	bch::qBUNCHwl( sFunction_ ) Functions_;
}

void scln::gRegistrar::Register( sFunction_ Function )
{
	Functions_.Append( Function );
}

namespace {
	namespace {
		void Final_( v8::Isolate *isolate )
		{
			err::buffer__ Buffer;
			isolate->ThrowException( v8::Exception::Error( v8::String::NewFromUtf8( isolate, err::Message( Buffer ) ) ) ); 
		}
	}

	void Launch_( const v8::FunctionCallbackInfo<v8::Value>& Info )
	{
	qRFH
		v8q::sArguments Arguments;
	qRFB
		if ( Info.Length() < 1 )
			qRGnr();

		if ( !Info[0]->IsUint32() )
			qRGnr();

		v8::Local<v8::Uint32> Index = v8::Local<v8::Uint32>::Cast(Info[0] );

		if ( !Functions_.Exists( Index->Uint32Value() ) )
			qRGnr();

		Arguments.Init( Info );

		Functions_( Index->Uint32Value())( Arguments );
	qRFR
	qRFT
	qRFE( Final_( Info.GetIsolate() ) )
	}
}

void scln::Register_(
	v8::Local<v8::Object> Exports,
	v8::Local<v8::Value> Module,
	void* priv )
{
	gRegistrar Registrar;

	NODE_SET_METHOD( Exports, "dummy", Launch_ );

	Registrar.Init( Exports );

	scln::SCLNRegister( Registrar );
}

qGCTOR( scln )
{
	Functions_.Init();
}