/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of XPPq.

	XPPq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XPPq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XPPq. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef I18N_INC_
# define I18N_INC_

// # include "xxx.h"
# include "scllocale.h"
# include "scltool.h"

# include "lcl.h"
# include "cio.h"

// 'Simple Translation Function'. To get the translation of messages which do not need parameters.
# define STF( name )\
	inline const str::string_ &Get##name##Translation( str::string_ &Translation )\
	{\
		return sclmisc::GetBaseTranslation( Label( t##name ), Translation );\
	}

// 'Simple Meaning Function'. To get the translation of messages which do not need parameters.
# define SMF( name )\
	inline const lcl::meaning_ &Get##name##Translation( lcl::meaning_ &Meaning )\
	{\
		return GetMeaning_( t##name, &Meaning );\
	}

namespace i18n {
	enum text__ {
		tProcessingError,
		tEncryptionError,
		t_amount,
		t_Undefined
	};

// To adjust if above 'enum' is modified.
# define LOCALE__TEXT_AMOUNT	2

	using scllocale::GetLocale;

	const char *Label( text__ Text );

	const lcl::meaning_ &GetMeaning_(
		text__ Text,
		lcl::meaning_ *Meaning,	// '&Meaning' ne fonctionne pas avec 'va_arg'.
		... );

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
