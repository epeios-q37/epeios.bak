/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'NJSq'.

    'NJSq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'NJSq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'NJSq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "wrapper.h"

#include "v8q.h"

#include "sclerror.h"

using namespace wrapper;

namespace {
	using njs::cLauncher;

	 cLauncher *Launcher_ = NULL;

	cLauncher &GetLauncher_( void )
	{
		if ( Launcher_ == NULL )
			qRGnr();

		return *Launcher_;
	}
}

void wrapper::SetLauncher( cLauncher *Launcher )
{
	if ( Launcher_ != NULL )
		qRGnr();

	if ( Launcher == NULL )
		qRGnr();

	Launcher_ = Launcher;
}

bso::sBool wrapper::GetLauncherInfo( str::dString &Info )
{
	if ( Launcher_ == NULL ) {
		sclmisc::GetBaseTranslation( common::message::NoRegisteredComponent, Info );
		return false;
	}

	Launcher_->Info( Info );

	return true;
}


typedef njs::cCaller cCaller_;

inline void GetString_(
	int Index,
	const v8::FunctionCallbackInfo<v8::Value> &Info,
	str::dString &Value )
{
	qRH
		v8q::sString String;
	qRB
		String.Init( Info[Index] );

		String.Get( Value );
	qRR
		qRT
		qRE
}

void SetReturnValue_(
	const v8::FunctionCallbackInfo<v8::Value> &Info,
	const str::dString &Value )
{
	Info.GetReturnValue().Set( v8q::sString( Value ).Core() );
}

namespace {
	class sCaller_
	: public cCaller_ {
	private:
		qRMV( const v8::FunctionCallbackInfo<v8::Value>, I_, Info_ );
	protected:
		virtual void NJSGetArgument(
			int Index,
			njs::eType Type,
			void *Value ) override
		{
			if ( Index >= I_().Length() )
				qRGnr();

			switch ( Type ) {
			case njs::tString:
				GetString_( Index, I_(), *( str::dString * )Value );
				break;
			default:
				qRGnr();
				break;
			}
		}
		virtual void NJSSetReturnValue(
			njs::eType Type,
			const void *Value ) override
		{
			switch ( Type ) {
			case njs::tString:
				SetReturnValue_( I_(), *( const str::dString * )Value );
				break;
			default:
				qRGnr();
				break;
			}
		}
	public:
		void reset( bso::sBool = true )
		{
			Info_ = NULL;
		}
		qCDTOR( sCaller_ );
		void Init( const v8::FunctionCallbackInfo<v8::Value> &Info )
		{
			Info_ = &Info;
		}
	};
}

void wrapper::Launch( const v8::FunctionCallbackInfo<v8::Value>& Info )
{
qRFH
	sCaller_ Caller;
qRFB
	if ( Info.Length() < 1 )
		qRGnr();

	if ( !Info[0]->IsUint32() )
		qRGnr();

	v8::Local<v8::Uint32> Index = v8::Local<v8::Uint32>::Cast(Info[0] );

	if ( !common::Functions.Exists( Index->Uint32Value() ) )
		qRGnr();

	Caller.Init( Info );

	GetLauncher_().Launch( common::Functions( Index->Uint32Value()), Caller );

	if ( sclerror::IsErrorPending() )
		qRAbort();	// To force the handling of a pending error.
qRFR
qRFT
qRFE( common::ErrFinal( Info.GetIsolate() ) )
}

void wrapper::DeleteLauncher( void )
{
	if ( Launcher_ != NULL )
		delete Launcher_;
}
