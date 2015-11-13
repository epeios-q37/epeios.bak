/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of fwtchrq.

    fwtchrq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    fwtchrq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with fwtchrq.  If not, see <http://www.gnu.org/licenses/>
*/

#include "misc.h"

#include "registry.h"

#include "cio.h"
#include "flf.h"

using namespace misc;

void misc::Append(
	const char *Tag,
	const str::string_ &Value,
	str::strings_ &Tags,
	str::strings_ &Values )
{
	if ( Tags.Append( str::string( Tag ) ) != Values.Append( Value ) )
		qRGnr();
}

void misc::Append(
	const char *Tag,
	const rgstry::entry___ &Entry,
	const sclrgstry::registry_ &Registry,
	str::strings_ &Tags,
	str::strings_ &Values )
{
qRH
	str::string Value;
qRB
	Value.Init();
	sclrgstry::MGetValue( Registry, Entry, Value );

	Append( Tag, Value, Tags, Values );
qRR
qRT
qRE
}
