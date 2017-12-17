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

#define SCLNJS_COMPILATION_

#include "sclnjs.h"

using namespace sclnjs;

namespace {
	template <typename callback> callback *Get_(
		int Index,
		n4all::cCaller &Caller,
		n4njs::eType Type )
	{
		callback *Callback = NULL;
	qRH;
	qRB;
		Caller.GetArgument( Index, Type, &Callback );
	qRR;
		if ( Callback != NULL )
			delete Callback;
	qRT;
	qRE;
		return Callback;
	}

	template <typename callback, typename host> void Get_(
		int Index,
		n4all::cCaller &Caller,
		n4njs::eType Type,
		host &Host )
	{
	qRH
		callback *Callback = NULL;
	qRB
		Callback = Get_<callback>( Index, Caller, Type );
		Host.Assign( Callback );
	qRR
		if ( Callback != NULL )
			delete Callback;
	qRT
	qRE
	}
}

template <> void scln4::Get(
	int Index,
	cCaller_ &Caller,
	sclnjs::rString &String )
{
	Get_<n4njs::cUString>( Index, Caller, n4njs::tString, String );
}

void scln4::Get(
	int Index,
	cCaller_ &Caller,
	str::dString *Target )
{
qRH;
	sclnjs::rString String;
qRB;
	String.Init();
	Get( Index, Caller, String );

	*Target = String.Callback().Get();
qRR;
qRT;
qRE;
}

template <> void scln4::Get(
	int Index,
	cCaller_ &Caller,
	sclnjs::rStrings &Strings )
{
	Get_<n4njs::cUStrings>( Index, Caller, n4njs::tStrings, Strings );
}

void scln4::Get(
	int Index,
	cCaller_ &Caller,
	str::dStrings *Target )
{
qRH;
	sclnjs::rStrings Strings;
qRB;
	Strings.Init();
	Get( Index, Caller, Strings );

	*Target = Strings.Callback().Get();
qRR;
qRT;
qRE;
}

template <> void scln4::Get(
	int Index,
	cCaller_ &Caller,
	sclnjs::rCallbacks &Callbacks )
{
	Get_<n4njs::cUCallbacks>( Index, Caller, n4njs::tCallbacks, Callbacks );
}

template <> void scln4::Get(
	int Index,
	cCaller_ &Caller,
	sclnjs::rObject &Object )
{
	Get_<n4njs::cUObject>( Index, Caller, n4njs::tObject, Object );
}

template <> void scln4::Get(
	int Index,
	cCaller_ &Caller,
	sclnjs::rRStream &Stream )
{
	Get_<n4njs::cURStream>( Index, Caller, n4njs::tRStream, Stream );
}

template <> void scln4::Get(
	int Index,
	cCaller_ &Caller,
	sclnjs::rBuffer &Buffer )
{
	Get_<n4njs::cUBuffer>( Index, Caller, n4njs::tBuffer, Buffer );
}

template <> void scln4::Get(
	int Index,
	cCaller_ &Caller,
	n4njs::cUCallback *&Callback )
{
	Callback = Get_<n4njs::cUCallback>( Index, Caller, n4njs::tCallback );
}

template <> void scln4::Get(
	int Index,
	cCaller_ &Caller,
	sclnjs::rCallback &Callback )
{
	Get_<n4njs::cUCallback>( Index, Caller, n4njs::tCallback, Callback );
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	rBuffer &Buffer )
{
qRH
	str::wString String;
qRB
	String.Init();
	Buffer.ToString( String );

	Flow << String;
qRR
qRT
qRE
	return Flow;
}

namespace {
	n4njs::fAsync Async_ = NULL;
}

namespace {
	typedef n4all::cLauncher cLauncher_;

	class sLauncher_
	: public cLauncher_ {
	protected:
		virtual void N4ALLLaunch(
			void *Function,
			n4all::cCaller &RawCaller ) override
		{
			sCaller Caller;

			Caller.Init( RawCaller );

			( (fFunction *)Function )( Caller );
		}
		virtual void N4ALLInfo( str::dString &Info ) override
		{
		qRH
			flx::rStringOFlow BaseFlow;
			txf::sWFlow Flow;
		qRB
			BaseFlow.Init( Info );
			Flow.Init( BaseFlow );

			SCLNJSInfo( Flow );
		qRR
		qRT
		qRE
		}
	public:
		void Init( void )
		{}
	};
}

n4all::cLauncher *scln4a::SCLN4ARegister(
	n4all::cRegistrar &RegistrarCallback,
	void *UP )
{
	n4all::cLauncher *Launcher = NULL;
qRH
	sclnjs::sRegistrar Registrar;
qRB
	const n4njs::gShared &Shared = *( const n4njs::gShared* )UP;

	if ( Shared.Async == NULL )
		qRFwk();

	Async_ = Shared.Async;

	Registrar.Init( RegistrarCallback );

	SCLNJSRegister( Registrar );

	Launcher = new sLauncher_;

	if ( Launcher == NULL )
		qRAlc();
qRR
	if ( Launcher != NULL )
		delete Launcher;
qRT
qRE
	return Launcher;
}


bso::sBool sclnjs::rRStream::Push( const str::dString &SBuffer )
{
	bso::sBool Result = false;
qRH
	qCBUFFERr Buffer;
qRB
	Result = Push( (void *)SBuffer.Convert( Buffer ), SBuffer.Amount() );
qRR
qRT
qRE
	return Result;
}

void sclnjs::Launch( cAsync &Async )
{
	return Async_( Async );
}
