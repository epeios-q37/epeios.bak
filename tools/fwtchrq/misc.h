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

#ifndef MISC_INC_
# define MISC_INC_

# include "dwtftr.h"
# include "dwtmov.h"
# include "dwtcpr.h"
// # include "fwtcmd.h"

# include "sclmisc.h"
# include "sclrgstry.h"

# define MISC_NAME_MC			"fwtchrq"
# define MISC_NAME_LC			"fwtchrq"
# define MISC_NAME_UC			"FWTCHRQ"
# define MISC_WEBSITE_URL		"http://q37.info/"
# define MISC_AUTHOR_NAME		"Claude SIMON"
# define MISC_AUTHOR_CONTACT	"http://q37.info/contact/"
# define MISC_OWNER_NAME		"Claude SIMON"
# define MISC_OWNER_CONTACT		"http://q37.info/contact/"
# define MISC_COPYRIGHT			COPYRIGHT_YEARS " " MISC_OWNER_NAME " (" MISC_OWNER_CONTACT ")"	


namespace misc {
	inline void NormalizePath( str::string_ &Path )
	{
		if ( ( Path.Amount() != 0 )
			 && ( ( Path( Path.Last() ) == '/' )
				 || ( Path( Path.Last() ) == '\\' ) ) )
				 Path.Append('.');
	}

	// Test l'existence de 'Path' et signale son �ventuelle absecnce.
	inline void TestPath( const str::string_ &Path )
	{
		if ( !fil::Exists( Path ) )
			sclmisc::ReportAndAbort( "UnableToFind", Path );
	}

	inline void NormalizeAndTestPath( str::string_ &Path )
	{
		NormalizePath( Path );

		TestPath( Path );
	}

	dwtbsc::exclusions_handling__ GetExclusionsHandling_( const sclrgstry::registry_ &Registry );

	void Append(
		const char *Tag,
		const str::string_ &Value,
		str::strings_ &Tags,
		str::strings_ &Values );

	void Append(
		const char *Tag,
		const rgstry::entry___ &Entry,
		const sclrgstry::registry_ &Registry,
		str::strings_ &Tags,
		str::strings_ &Values );

	void Dump(
		dwtftr::drow__ Root,
		const dwtftr::file_tree_ &Tree,
		const fnm::name___ &Filename );

	void Dump(
		const dwtmov::movings_ &Movings,
		dwtcpr::drow__ SceneRoot,
		const dwtcpr::scene_ &Scene,
		const fnm::name___ &Filename );
}

#endif
