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

#ifndef XHTPROLOG__INC
# define XHTPROLOG__INC

# define XHTPROLOG_NAME		"XHTPROLOG"

# if defined( E_DEBUG ) && !defined( XHTPROLOG_NODBG )
#  define XHTPROLOG_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// XHT(ML) PROLOG

# include "xhtbase.h"
# include "xhtagent.h"

# include "frdbse.h"

# include "err.h"
# include "flw.h"

namespace xhtprolog {
	typedef xhtbase::event_callback__ _event_callback__;

	E_CDEF( char *, ProjectTypeId, "ProjectType" );
	E_CDEF( char *, PredefinedProjectId, "PredefinedProject" );
	E_CDEF( char *, UserProjectId, "UserProject" );

	void GetContent(
		xhtagent::agent___ &Agent,
		xml::writer_ &Writer );

	void GetContext(
		xhtagent::agent___ &Agent,
		xml::writer_ &Writer );

	frdbse::project_type__ GetProjectFeatures(
		xhtagent::agent___ &Agent,
		str::string_ &ProjectFeature );

	void DisplaySelectedProjectFileName(
		xhtagent::agent___ &Agent,
		const char *Id );
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
