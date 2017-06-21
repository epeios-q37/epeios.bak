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

#include "common.h"

#include "v8q.h"

#include "sclerror.h"
#include "sclmisc.h"

#include "str.h"
#include "tol.h"

using namespace common;

qCDEF( char *, common::message::NoRegisteredAddon, "NoRegisteredAddon" );

rFunctions common::Functions;

void common::ErrFinal( v8::Isolate *Isolate )
{
qRH
	str::wString Message;
	err::buffer__ Buffer;
qRB
	Isolate = v8q::GetIsolate( Isolate );

	Message.Init();

	if ( ERRType != err::t_Abort ) {
		Message.Append( err::Message( Buffer ) );

		ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.
	} else if ( sclerror::IsErrorPending() )
		sclmisc::GetSCLBasePendingErrorTranslation( Message );

	if ( Isolate != NULL )
		Isolate->ThrowException( v8::Exception::Error( v8q::ToString( Message ) ) );
	else
		cio::CErr << txf::nl << Message << txf::nl;
qRR
	ERRRst();
qRT
qRE
}



