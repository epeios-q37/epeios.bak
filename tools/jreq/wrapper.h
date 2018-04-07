/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'JREq.

    'JREq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'JREq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'JREq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef WRAPPER_INC_
# define WRAPPER_INC_

# include "n4allw.h"
# include "n4jre.h"

# include "jniq.h"

namespace wrapper {
	using n4allw::rLauncher;

	n4jre::cObject *NewObject(
		n4jre::sEnv *Env,
		const char * ClassName,
		const char * Signature,
		int ArgC,
		n4jre::sValue * ArgV );

	n4jre::cObject *NewObjectArray(
		n4jre::sEnv *Env,
		n4jre::sJSize Length,
		const char * ClassName );

	jobject Call(
		JNIEnv *Env,
		rLauncher &Launcher,
		jint Index,
		jobjectArray &Args );
}

#endif
