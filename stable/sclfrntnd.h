/*
	'sclfrntnd.h' by Claude SIMON (http://zeusw.org/).

	'sclfrntnd' is part of the Epeios framework.

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

#ifndef SCLFRNTND__INC
# define SCLFRNTND__INC

# define SCLFRNTND_NAME		"SCLFRNTND"

# if defined( E_DEBUG ) && !defined( SCLFRNTND_NODBG )
#  define SCLFRNTND_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// SoCLe FRoNTeND

# include "frdkrn.h"

# include "sclrgstry.h"

# include "err.h"
# include "flw.h"
# include "xml.h"

namespace sclfrntnd {

	void Report(
		frdkrn::kernel___ &Kernel,
		const char *Message );

	// Action à effectuer sur le projet par défaut.
	enum action__ {
		aNone,		// Aucune action ; l'utilisateur peut sélectionner un autre type de projet, qui sera chargé manuellement.
		aLoad,		// Le project soit être chargé, mais pas lancé.
		aLaunch,	// Le project doit être chargé et lancé.
		a_amount,
		a_Undefined
	};

	const char *GetLabel( action__ Action );

	action__ GetAction( const str::string_ &Label );

	action__ GetProjectsFeatures(
		const char *Language,
		xml::writer_ &Writer );

	void GetBackendsFeatures(
		const char *Language,
		xml::writer_ &Writer );

	bso::uint__ GetBackendPingDelay( void );

	void LoadProject(
		frdbse::project_type__ ProjectType,
		const str::string_ &ProjectFeature );

	void Connect(
		frdkrn::kernel___ &Kernel,
		frdbse::backend_type__ BackendType,
		const str::string_ &BackendFeature,
		const frdkrn::compatibility_informations__ &CompatibilityInformations );

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
