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

#define SCLZND_COMPILATION_

#include "sclznd.h"

#include "sclerror.h"
#include "scllocale.h"
#include "sclmisc.h"

#include "bch.h"
#include "str.h"
#include "cio.h"

using namespace sclznd;

namespace {
	typedef n4all::cLauncher cLauncher_;

	class sLauncher_
	: public cLauncher_ {
	protected:
		virtual void N4ALLCall(
			n4all::sEnv *,	// Not used.
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

			SCLZNDInfo( Flow );
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
	void *UP,
	const sclmisc::sInfo *&Info )
{
	n4all::cLauncher *Launcher = NULL;
qRH
	sclznd::sRegistrar Registrar;
qRB
	const n4znd::gShared &Shared = *( const n4znd::gShared* )UP;

	Registrar.Init( RegistrarCallback );
	Info = &SCLZNDRegister( Registrar );

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

