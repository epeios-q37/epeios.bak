/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#ifndef SCLLOCALE__INC
# define SCLLOCALE__INC

# define SCLLOCALE_NAME		"SCLLOCALE"

# if defined( E_DEBUG ) && !defined( SCLLOCALE_NODBG )
#  define SCLLOCALE_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// SoCLe LOCALE

# include "err.h"
# include "flw.h"
# include "str.h"
# include "lcl.h"

namespace scllocale {

	E_CDEF(char, DefaultMarker, '%' );

	const lcl::locale_ &GetLocale( void );

	enum target__ {
		tMain,
		tConfiguration,
		tProject,
		t_amount,
		t_Undefined
	};

	void EraseLocale( target__ Target );

	void SetLocale(
		target__ Target,
		const rgstry::entry__ &Entry );

	void LoadLocale(
		target__ Target,
		xtf::extended_text_iflow__ &Flow,
		const char *Directory,
		const char *RootPath );

	const str::string_ &GetTranslation(
		const char *Text,
		const char *Language,
		str::string_ &Translation );

	const str::string_ &GetTranslation(
		const lcl::meaning_ &Meaning,
		const char *Language,
		str::string_ &Translation );

	void TranslateTags(
		str::string_ &String,
		const char *Language,
		char Marker = DefaultMarker );

	void TranslateTags(
		const str::string_ &In,
		const char *Language,
		str::string_ &Out,
		char Marker = DefaultMarker );
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
