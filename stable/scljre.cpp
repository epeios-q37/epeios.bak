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

#define SCLJRE_COMPILATION_

#include "scljre.h"

#include "sclerror.h"
#include "scllocale.h"
#include "sclmisc.h"

#include "str.h"

using namespace scljre;

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
			sJObject Return;

			Caller.Init( RawCaller );

			Return = ( (fFunction *)Function )( Caller );

			RawCaller.SetReturnValue( n4jre::t_Undefined, Return );	// 'sJObject' is a generic type, so no type is specified.
		}
		virtual void N4ALLInfo( str::dString &Info ) override
		{
		qRH
			flx::rStringOFlow BaseFlow;
			txf::sOFlow Flow;
		qRB
			BaseFlow.Init( Info );
			Flow.Init( BaseFlow );

			SCLJREInfo( Flow );
		qRR
		qRT
		qRE
		}
	public:
		void Init( void )
		{}
	};
}

namespace {
	n4jre::fNew New_ = NULL;
}

cObject *scljre::New(
	const char *ClassName,
	const char *Signature,
	int ArgC,
	sValue *ArgV )
{
	if ( New_ == NULL )
		qRFwk();

	return New_( ClassName, Signature, ArgC, ArgV );
}



n4all::cLauncher *scln4a::SCLN4ARegister(
	n4all::cRegistrar &RegistrarCallback,
	void *UP )
{
	n4all::cLauncher *Launcher = NULL;
qRH
	scljre::sRegistrar Registrar;
qRB
	const n4jre::gShared &Shared = *( const n4jre::gShared* )UP;

	if ( Shared.New == NULL )
		qRFwk();

	New_ = Shared.New;

	Registrar.Init( RegistrarCallback );
	SCLJRERegister( Registrar );

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

sJObject scljre::String( const str::dString &UTF )
{
	scljre::java::lang::sString JString;
qRH
	rJString Charset;
	rJByteArray Bytes;
qRB
	Charset.Init( sizeof( "UTF-8" ), "UTF-8", n4jre::hOriginal );

	if ( UTF.Amount() > LONG_MAX )
		qRLmt();

	Bytes.Init( (long)UTF.Amount() );

	UTF.Recall( UTF.First(), UTF.Amount(), (char *)Bytes.Core() );

	JString.Init( Bytes, Charset );
qRR
qRT
qRE
	return JString.Object().Object();
}

