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

#ifndef XHTLOGIN__INC
# define XHTLOGIN__INC

# define XHTLOGIN_NAME		"XHTLOGIN"

# if defined( E_DEBUG ) && !defined( XHTLOGIN_NODBG )
#  define XHTLOGIN_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// XHT(ML) LOGIN

# include "xhtbase.h"
# include "xhtagent.h"

# include "frdbse.h"

# include "err.h"
# include "flw.h"
# include "tol.h"

namespace xhtlogin {
	E_CDEF( char *, BackendTypeId, "BackendType" );
	E_CDEF( char *, DaemonBackendId, "DaemonBackend" );
	E_CDEF( char *, EmbeddedBackendId, "EmbeddedBackend" );
	E_CDEF( char *, PredefinedBackendId, "PredefinedBackend" );


	typedef xhtbase::event_callback__ _event_callback__;

	void GetContent(
		xhtagent::agent___ &Agent,
		xml::writer_ &Writer );

	void GetContext(
		xhtagent::agent___ &Agent,
		xml::writer_ &Writer );

	frdbse::backend_type__ GetBackendFeatures(
		xhtagent::agent___ &Agent,
		str::string_ &Feature );

	void DisplaySelectedEmbeddedBackendFileName(
		xhtagent::agent___ &Agent,
		const char *Id );

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
