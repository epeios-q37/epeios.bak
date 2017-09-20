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

extern "C" FUNCTION_SPEC n4all::fRegister N4ALL_REGISTER_FUNCTION_NAME;

n4all::cLauncher *N4ALLRegister(
	n4all::cRegistrar *Registrar,
	n4all::sData *Data )
{
	n4all::cLauncher *Launcher = NULL;
qRFH
qRFB
	sclmisc::Initialize( *Data->SCLRack, *Data->Location );

	Launcher = scln4a::SCLN4ARegister( *Registrar, Data->UP );
qRFR
	if ( Launcher != NULL )
		delete Launcher;
qRFT
qRFE( sclmisc::ErrFinal() )
	return Launcher;
}
