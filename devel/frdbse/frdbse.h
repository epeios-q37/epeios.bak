/*
	'frdbse.h' by Claude SIMON (http://zeusw.org/).

	'frdbse' is part of the Epeios framework.

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

#ifndef FRDBSE__INC
# define FRDBSE__INC

# define FRDBSE_NAME		"FRDBSE"

# if defined( E_DEBUG ) && !defined( FRDBSE_NODBG )
#  define FRDBSE_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// FRonteNd BaSE

# include "err.h"
# include "flw.h"
# include "str.h"
# include "lcl.h"

namespace frdbse {

	enum project_type__ {
		ptNew,
		ptPredefined,
		ptUser,
		pt_amount,
		pt_Undefined
	};

	const char *GetLabel( project_type__ ProjectType );

	project_type__ GetProjectType( const str::string_ &Pattern );

	enum backend_type__ {
		btDaemon,
		btEmbedded,
		btPredefined,
		bt_amount,
		bt_Undefined
	};

	const char *GetLabel( backend_type__ BackendType );

	backend_type__ GetBackendType( const str::string_ &Pattern );
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
