/*
	'scllocale.h' by Claude SIMON (http://zeusw.org/).

	'scllocale' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
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

	const lcl::locale_ &GetLocale( void );

	inline const str::string_ &GetTranslation(
		const char *Text,
		const char *Language,
		str::string_ &Translation )
	{
		return GetLocale().GetTranslation( Text, Language, Translation );
	}

	inline const str::string_ &GetTranslation(
		const lcl::meaning_ &Meaning,
		const char *Language,
		str::string_ &Translation )
	{
		return GetLocale().GetTranslation( Meaning, Language, Translation );
	}

	void Load(
		flw::iflow__ &Flow,
		const char *Directory,
		const char *RootPath,
		utf::format__ Format );	// Chargement de la locale de base, propre au logiciel.

	lcl::level__ Push(
		flw::iflow__ &Flow,
		const char *Directory,
		const char *RootPath,
		utf::format__ Format,
		rgstry::context___ &Context );
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
