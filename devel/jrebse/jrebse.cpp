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

#define S "/"

#define JAVA "java" S

#define JAVA_IO JAVA "io" S
const char *jrebse::java::io::sPrintStream::Name_ = JAVA_IO  "PrintStream";
const char *jrebse::java::io::sInputStream::Name_ = JAVA_IO  "InputStream";

#define JAVA_LANG JAVA "lang" S
const char *jrebse::java::lang::sInteger::Name_ = JAVA_LANG "Integer";
const char *jrebse::java::lang::sLong::Name_ = JAVA_LANG "Long";
const char *jrebse::java::lang::sSystem::Name_ = JAVA_LANG "System";

