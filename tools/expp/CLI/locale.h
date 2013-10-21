/*
	Header for the 'locale' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of the 'xxx' tool.
	Copyright (C) 2011 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'xxx'.

    'xxx' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xxx' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'xxx'.  If not, see <http://www.gnu.org/licenses/>.
*/	

#ifndef LOCALE__INC
# define LOCALE__INC

// # include "xxx.h"
# include "scllocale.h"
# include "scltool.h"

# include "lcl.h"
# include "cio.h"

// 'Simple Translation Function'. To get the translation of messages which do not need parameters.
# define STF( name )\
	inline const str::string_ &Get##name##Translation( str::string_ &Translation )\
	{\
		return scllocale::GetTranslation( Label( t##name ), scltool::GetLanguage(), Translation );\
	}

// 'Simple Meaning Function'. To get the translation of messages which do not need parameters.
# define SMF( name )\
	inline const lcl::meaning_ &Get##name##Translation( lcl::meaning_ &Meaning )\
	{\
		return GetMeaning_( t##name, &Meaning );\
	}

namespace locale {

	enum text__ {
		tProcessCommandDescription,
		tEncryptCommandDescription,
		tNamespaceOptionDescription,
		tNoIndentOptionDescription,
		tSourceFileArgumentDescription,
		tDestFileArgumentDescription,
		tProcessingError,
		tEncryptionError,
		t_amount,
		t_Undefined
	};

// To adjust if above 'enum' is modified.
# define LOCALE__TEXT_AMOUNT	8

	using scllocale::GetLocale;

	const char *Label( text__ Text );

	const lcl::meaning_ &GetMeaning_(
		text__ Text,
		lcl::meaning_ *Meaning,	// '&Meaning' ne fonctionne pas avec 'va_arg'.
		... );

	STF( ProcessCommandDescription );
	STF( EncryptCommandDescription );
	STF( NoIndentOptionDescription );
	STF( SourceFileArgumentDescription );
	STF( DestFileArgumentDescription );

	inline const lcl::meaning_ &GetNamespaceOptionDescriptionMeaning(
		const char *DefaultNamespace,
		lcl::meaning_ &Meaning )
	{
		return GetMeaning_( tNamespaceOptionDescription, &Meaning, DefaultNamespace );
	}

	inline const lcl::meaning_ &GetProcessingErrorMeaning(
		const xpp::context___ &Context,
		lcl::meaning_ &Meaning )
	{
		return GetMeaning_( tProcessingError, &Meaning, &Context );
	}

	inline const lcl::meaning_ &GetEncryptionErrorMeaning(
		const xpp::context___ &Context,
		lcl::meaning_ &Meaning )
	{
		return GetMeaning_( tEncryptionError, &Meaning, &Context );
	}

}

#endif
