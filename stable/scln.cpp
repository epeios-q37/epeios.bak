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

#include <uv.h>
#include <node_buffer.h>
#include "scln.h"

#include "v8qnjs.h"

#include "v8q.h"
#include "bch.h"
#include "cio.h"

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

		 Info.GetReturnValue().Set(v8::Undefined(Info.GetIsolate()));
	qRFR
	qRFT
	qRFE( ErrFinal( Info.GetIsolate() ) )
	}
}


static void Test( const char *Text )
{
  v8::HandleScope handleScope(v8q::GetIsolate());
	v8qnjs::sBuffer Buffer;
	v8::MaybeLocal<v8::Object> MBuffer = node::Buffer::Copy( v8q::GetIsolate(), Text, strlen( Text ) );
	v8::Local<v8::Object> LBuffer = v8qnjs::ToLocal( MBuffer );
	Buffer.Init( LBuffer );
	cio::COut << Buffer << txf::nl << txf::commit;

}

struct work{
	uv_work_t Request;
	v8::Isolate *Isolate;
};

void WorkAsync_(uv_work_t *req)
{
	work *Work = static_cast<work *>(req->data);
	v8::Isolate *Isolate = Work->Isolate;

	v8::Locker Locker( Isolate );

  v8::HandleScope handleScope( Isolate);

	v8qnjs::sBuffer Buffer;
	v8::MaybeLocal<v8::Object> MBuffer = node::Buffer::Copy( Isolate, "Hello!", 6 );
	v8::Local<v8::Object> LBuffer = v8qnjs::ToLocal( MBuffer );
	Buffer.Init( LBuffer, Isolate );
	cio::COut << Buffer << txf::nl << txf::commit;
}

void WorkAsyncComplete_(
	uv_work_t *req,
	int Status )
{
	work *Work = static_cast<work *>(req->data);

	Test("End");

	delete Work;
}

void Test( const v8::FunctionCallbackInfo<v8::Value>&args )
{
	cio::Initialize( cio::tConsole );
	v8::Isolate *Isolate = args.GetIsolate();

	Test( "Start" );
	
	work *Work = new work;

	Work->Request.data = Work;
	Work->Isolate = args.GetIsolate();

	Isolate->Exit();
	v8::Unlocker Unlocker( Isolate );

	uv_queue_work( uv_default_loop(), &Work->Request, WorkAsync_, WorkAsyncComplete_ );
	

	args.GetReturnValue().Set(v8::Undefined(Isolate));
	/*
	Isolate->Exit();
	v8::Unlocker Unlocker( Isolate );
	*/
}


void scln::Register_(
	v8::Local<v8::Object> Exports,
	v8::Local<v8::Value> Module,
	void* priv )
{

//	NODE_SET_METHOD( Exports, "test", Test );

	NODE_SET_METHOD( Exports, "_wrapper", Launch_ );
	sRegistrar Registrar;
	Registrar.Init( Exports );
	Functions_.Init();
	scln::SCLNRegister( Registrar );

	/*
	v8q::SetGlobalIsolate( v8::Isolate::GetCurrent() );
	v8::HandleScope handleScope(v8q::GetIsolate());
	*/

}

qGCTOR( scln )
{
// Sudenly stops to work ('Segmentation fault', so was deplaced in the 'Register_' function.
//	Functions_.Init();
}