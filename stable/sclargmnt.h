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

#ifndef SCLARGMNT__INC
# define SCLARGMNT__INC

# define SCLARGMNT_NAME		"SCLARGMNT"

# if defined( E_DEBUG ) && !defined( SCLARGMNT_NODBG )
#  define SCLARGMNT_DBG
# endif

// SoCLe ARGuMeNTs

# include "scli.h"

# include "err.h"
# include "txf.h"
# include "str.h"

namespace sclargmnt {
	E_ENUM( first_argument ) {
		faIsCommand,	// The first argument is the command (when arguments come from the CLI).
		faIsArgument,	// The first argument is really a argument (when arguments come from another source).
		fa_amount,
		fa_Undefined
	};

	E_ENUM( unknown_arguments ) {
		uaIgnore,	// Ignore unknown arguments; useful when using CEF, for example.
		uaReport,	// report uknown arguments.
		ua_amount,
		ua_Undefined,
	};

	const str::string_ &GetCommand( str::string_ &Command );

	void PrintUsage(
		const scli::sInfo &Info,
		txf::text_oflow__ &Flow );

	void FillRegistry(
		const str::strings_ &Arguments,
		first_argument__ FirstArgument,
		unknown_arguments__ UnknownArguments );

	void FillRegistry(
		const str::dString &MergedArguments,
		first_argument__ FirstArgument,
		unknown_arguments__ UnknownArguments );

	void Normalize(
		const str::dString &Raw,
		str::dString &Normalized );
}

#endif
