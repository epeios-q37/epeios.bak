/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#ifndef CSDRCC__INC
# define CSDRCC__INC

# define CSDRCC_NAME		"CSDRCC"

# if defined( E_DEBUG ) && !defined( CSDRCC_NODBG )
#  define CSDRCC_DBG
# endif

// Client-Server Devices Remote Client Core

# include "err.h"
# include "fdr.h"

#define CSDRCC_PLUGIN_NAME			"Remote"
#define CSDRCC_PLUGIN_API_VERSION	"1"

# pragma message( __LOC__ "To ameliorate !" )
/*
	TODO:;
	This library should be a 'frontend' specific one, as it uses the muxed connections.
	Or it should be configurable.

	Also, the same frontend (a CGI for example) which asks for multiple connections to backend
	would receive a PHYSICAL connection to backend, rather then a LOGICAL one. Hence, the plugin
	should have a method which gives a flow, rather than be a flow.
*/

namespace csdrcc {
	typedef fdr::ioflow_driver_base___ _driver___;

	class driver___
	{
	protected:
		virtual _driver___ *CSDRCCNew( void ) = 0;
		virtual void CSDRCCDelete( _driver___ * ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
		}
		E_CVDTOR( driver___ );
		void Init( void )
		{
		}
		_driver___ *New( void )
		{
			return CSDRCCNew();
		}
		void Delete( _driver___ *D )
		{
			return CSDRCCDelete( D );
		}
		static const char *Label( void );
	};
}

#endif
