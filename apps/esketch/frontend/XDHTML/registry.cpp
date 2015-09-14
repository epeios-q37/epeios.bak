/*
	'registry.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "registry.h"

using frdrgstry::entry___;

static entry___ XSLFilesSet_( "XSLFilesSet", sclrgstry::Parameters );
static entry___ FreeXSLFiles_( "XSLFiles", XSLFilesSet_ );
static entry___ TaggedXSLFiles(RGSTRY_TAGGING_ATTRIBUTE("target"), FreeXSLFiles_ );
entry___ registry::XSLLayoutFile("Layout", TaggedXSLFiles );
entry___ registry::XSLCastingFile("Casting", TaggedXSLFiles );