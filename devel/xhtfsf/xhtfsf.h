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

#ifndef XHTFSF__INC
# define XHTFSF__INC

# define XHTFSF_NAME		"XHTFSF"

# if defined( E_DEBUG ) && !defined( XHTFSF_NODBG )
#  define XHTFSF_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// XHT(ML) Frontend Session Form

# error "Obsolete ! User 'xhtlogin' instead !"

# include "xhtfbs.h"

# include "frdkrn.h"

# include "err.h"
# include "flw.h"

namespace xhtfsf {

	E_CDEF( char *, BackendTypeId, "BackendType" );
	E_CDEF( char *, DaemonBackendId, "DaemonBackend" );
	E_CDEF( char *, EmbeddedBackendId, "EmbeddedBackend" );
	E_CDEF( char *, PredefinedBackendId, "PredefinedBackend" );


	typedef xhtfbs::event_callback__ _event_callback__;

	class event_callbacks__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			// Standardisation.
		}
		E_CVDTOR( event_callbacks__ );
		void Init( void )
		{
			// Standardisation.
		}
	};

	void GetContent(
		xhtagent::agent_core___ &Agent,
		xml::writer_ &Writer );

	void GetContext(
		xhtagent::agent_core___ &Agent,
		xml::writer_ &Writer );

	frdbse::backend_type__ GetBackendFeatures(
		xhtagent::agent_core___ &Agent,
		str::string_ &Feature );

	void DisplaySelectedEmbeddedBackendFileName(
		xhtagent::agent_core___ &Agent,
		const char *Id );
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
