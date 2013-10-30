/*
	'sclrgstry.h' by Claude SIMON (http://zeusw.org/).

	'sclrgstry' is part of the Epeios framework.

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

#ifndef SCLRGSTRY__INC
#define SCLRGSTRY__INC

#define SCLRGSTRY_NAME		"SCLRGSTRY"

#if defined( E_DEBUG ) && !defined( SCLRGSTRY_NODBG )
#define SCLRGSTRY_DBG
#endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// SoCLe ReGiSTRY

# include "err.h"
# include "flw.h"

# include "rgstry.h"

namespace sclrgstry {

	using rgstry::tags_;
	using rgstry::tags;

//	extern const lcl::rack__ *LocaleRack;	// A définir.

	const rgstry::registry_ &GetRegistry( void );

	rgstry::row__ GetRoot( void );

	extern rgstry::entry___ Parameters;

	extern rgstry::entry___ Language;

	extern rgstry::entry___ Locale;	// Du fichier de configuration uniquement.

	using rgstry::value;
	using rgstry::value_;

	void Load(
		flw::iflow__ &Flow,
		const char *Directory,
		const char *RootPath );

	bso::bool__ IsReady( void );

	void ReportBadOrNoValueForEntryErrorAndAbort( const rgstry::tentry__ &Entry );
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
