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

#define SCLN4A_COMPILATION_

#include "scln4a.h"

using namespace scln4a;

#ifdef CPE_S_WIN
# define FUNCTION_SPEC __declspec(dllexport)
#else
# define FUNCTION_SPEC
#endif

typedef n4all::cLauncher cLauncher_;

class sLauncher_
: public cLauncher_
{
protected:
	virtual void N4ALLLaunch(
		void *Function,
		cCaller_ &RawCaller ) override
	{
		sCaller Caller;

		Caller.Init( RawCaller );

		((fFunction *)Function )( Caller );
	}
	virtual void N4ALLInfo( str::dString &Info ) override
	{
	qRH
		flx::rStringOFlow BaseFlow;
		txf::sOFlow Flow;
	qRB
		BaseFlow.Init( Info );
		Flow.Init( BaseFlow );

		SCLN4AInfo( Flow );
	qRR
	qRT
	qRE
	}
public:
	void Init( void )
	{}
};

extern "C" FUNCTION_SPEC n4all::fRegister N4ALL_REGISTER_FUNCTION_NAME;

n4all::cLauncher *N4ALLRegister(
	n4all::cRegistrar *RawRegistrar,
	n4all::sData *Data )
{
	sLauncher_ *Launcher = NULL;
qRFH
	sRegistrar Registrar;
qRFB
	sclmisc::Initialize( *Data->SCLRack, *Data->Location );

	Launcher = new sLauncher_;

	if ( Launcher == NULL )
		qRAlc();

	Launcher->Init();

	Registrar.Init( *RawRegistrar );

	scln4a::SCLN4ARegister( Registrar, Data->UP );
qRFR
	if ( Launcher != NULL )
		delete Launcher;
qRFT
qRFE( sclmisc::ErrFinal() )
	return Launcher;
}
