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
#include "cio.h"
#include "cpe.h"

#include <node_version.h>

using namespace scln;

void scln::ErrFinal( v8::Isolate *Isolate )
{
	err::buffer__ Buffer;
	Isolate = v8q::GetIsolate( Isolate );

	if ( Isolate != NULL )
		Isolate->ThrowException( v8::Exception::Error( v8q::ToString( err::Message( Buffer ) ) ) ); 
	else
		cio::CErr << txf::nl << txf::tab << err::Message( Buffer ) << txf::nl << txf::commit;
}


namespace {
	bch::qBUNCHwl( sFunction_ ) Functions_;
}

void scln::sRegistrar::Register( sFunction_ Function )
{
	Functions_.Append( Function );
}

void scln::sRegistrar::Register(
	const char *Name,
	v8::FunctionCallback Function )
{
	NODE_SET_METHOD( E_(), Name, Function );
}


namespace {
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
	qRFE( ErrFinal( Info.GetIsolate() ) )
	}
}

namespace {
	void GetInfo_(const v8::FunctionCallbackInfo<v8::Value>& Args)
	{
	qRH
		str::wString Info;
	qRB
		Info.Init("Build : " __DATE__ " " __TIME__ " (" );
		Info.Append( cpe::GetDescription() );
		Info.Append( ") - ABI v" NODE_STRINGIFY( NODE_MODULE_VERSION ) " (Node v" NODE_VERSION_STRING ")" );

		Args.GetReturnValue().Set( v8q::sString( Info ).Core() );
	qRR
	qRT
	qRE
	}
}

void scln::Register_(
	v8::Local<v8::Object> Exports,
	v8::Local<v8::Value> Module,
	void* priv )
{
	sRegistrar Registrar;

	NODE_SET_METHOD( Exports, "info", GetInfo_ );
	NODE_SET_METHOD( Exports, "_wrapper", Launch_ );

	Registrar.Init( Exports );

	Functions_.Init();
	scln::SCLNRegister( Registrar );

	v8q::SetGlobalIsolate( v8::Isolate::GetCurrent() );
}

qGCTOR( scln )
{
// Sudenly stops to work ('Segmentation fault', so was deplaced in the 'Register_' function.
//	Functions_.Init();
}