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

//	$Id: wintol.h,v 1.10 2013/04/11 19:36:54 csimon Exp $

#ifndef WINTOL__INC
#define WINTOL__INC

#define WINTOL_NAME		"WINTOL"

#define	WINTOL_VERSION	"$Revision: 1.10 $"

#define WINTOL_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( WINTOL_NODBG )
#define WINTOL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.10 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/11 19:36:54 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D WINdows TOols 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include <signal.h>

#include "err.h"
#include "flw.h"
#include "cpe.h"

namespace wintol {

	bso::bool__ InstallService(
		const char *ServiceName,
		const char *DisplayName,
		const char *Description );

	bso::bool__ Remove( const char *ServiceName );


	inline void PatchSignalHandlingForWindowsService( void )
	{
#	ifndef CPE__CYGWIN
		signal( SIGBREAK, SIG_DFL );
#	endif
	}
	// Used by below class.

	class service__
	{
	protected:
		virtual void WINTOLProcess( void ) = 0;
		virtual void WINTOLShutdown( void ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
		}
		bso::bool__ Init( void )
		{
			return true;
		}
		bso::bool__ Launch( const char *ServiceName );	// Mme valeur que celle passe  'InstallService(...)'.
		// Return 'true' if termination was required.
		bso::bool__ TestTermination( void );
		void Process( void )
		{
			WINTOLProcess();
		}
		void Shutdown( void )
		{
			WINTOLShutdown();
		}
	};
}

/*$END$*/
#endif
