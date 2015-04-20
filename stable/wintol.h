/*
	Header for the 'wintol' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: wintol.h,v 1.10 2013/04/11 19:36:54 csimon Exp $

#ifndef WINTOL__INC
#define WINTOL__INC

#define WINTOL_NAME		"WINTOL"

#define	WINTOL_VERSION	"$Revision: 1.10 $"

#define WINTOL_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &WINTOLTutor;

#if defined( E_DEBUG ) && !defined( WINTOL_NODBG )
#define WINTOL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.10 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/11 19:36:54 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

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
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
