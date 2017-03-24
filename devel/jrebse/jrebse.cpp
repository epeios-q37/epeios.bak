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

#define JREBSE_COMPILATION_

#include "jrebse.h"

using namespace jrebse;

#define N( ns, hierarchy, name )\
	const char *jrebse::ns::name::Name = hierarchy #name;\
	const char *jrebse::ns::name::Signature = "L" hierarchy #name ";";

#define S "/"

#define JAVA "java" S

#define JAVA_IO JAVA "io" S
N( java::io, JAVA_IO, PrintStream );
N( java::io, JAVA_IO, InputStream );

#define JAVA_LANG JAVA "lang" S
N( java::lang, JAVA_LANG, Integer );
N( java::lang, JAVA_LANG, Long );
N( java::lang, JAVA_LANG, Object );
N( java::lang, JAVA_LANG, String );
N( java::lang, JAVA_LANG, System );

