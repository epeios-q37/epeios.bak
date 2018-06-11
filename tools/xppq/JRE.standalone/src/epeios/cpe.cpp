/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define CPE__COMPILATION

#include "cpe.h"

#include <string.h>

using namespace cpe;

#define C( label )\
	if ( Digest[0] )\
		strcat(Digest, ";");	\
\
	strcat( Digest, label )

const char *cpe::GetOSDigest( void )
{
	static char Digest[100] = "";

	if ( !Digest[0] ) {

#ifdef CPE_S_ANDROID
		C( CPE_SL_ANDROID );
#endif

#ifdef CPE_S_LINUX
		C( CPE_SL_LINUX );
#endif

#ifdef CPE_S_GNULINUX
		C( CPE_SL_GNULINUX );
#endif

#ifdef CPE_S_DARWIN
		C( CPE_SL_DARWIN );
#endif

#ifdef CPE_S_IOS
		C( CPE_SL_IOS );
#endif

#ifdef CPE_S_OSX
		C( CPE_SL_OSX );
#endif

#ifdef CPE_S_CYGWIN
		C( CPE_SL_CYGWIN );
#endif

#ifdef CPE_S_WIN
		C( CPE_SL_WIN );
#endif

#ifdef CPE_S_WIN32
		C( CPE_SL_WIN32);
#endif

#ifdef CPE_S_WIN64
		C( CPE_SL_WIN64 );
#endif

#ifdef CPE_S_UNIX
		C( CPE_SL_UNIX );
#endif

#ifdef CPE_S_POSIX
		C( CPE_SL_POSIX);
#endif
	}

	return Digest;
}

