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

#include "n4aups.h"
#include "sclerror.h"
#include "v8q.h"

using namespace wrapper;

typedef n4a::cCaller cCaller_;

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
		virtual void N4AGetArgument(
			int Index,
			n4a::eType Type,
			void *Value ) override
		{
			Index++;	// The first one was the function id.

			if ( Index >= I_().Length() )
				qRGnr();

			switch ( Type ) {
			case n4a::tString:
				GetString_( Index, I_(), *( str::dString * )Value );
				break;
			default:
				qRGnr();
				break;
			}
		}
		virtual void N4ASetReturnValue(
			n4a::eType Type,
			const void *Value ) override
		{
			switch ( Type ) {
			case n4a::tString:
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
qRH
	sCaller_ Caller;
qRB
	if ( Info.Length() < 1 )
		qRGnr();

	if ( !Info[0]->IsUint32() )
		qRGnr();

	v8::Local<v8::Uint32> Index = v8::Local<v8::Uint32>::Cast(Info[0] );

	Caller.Init( Info );

	n4aups::GetLauncher().Launch( n4aups::GetFunction( Index->Uint32Value()), Caller );

	if ( sclerror::IsErrorPending() )
		qRAbort();	// To force the handling of a pending error.
qRR
qRT
qRE
}

