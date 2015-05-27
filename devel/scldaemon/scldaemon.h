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

#ifndef SCLDAEMON__INC
# define SCLDAEMON__INC

# define SCLDAEMON_NAME		"SCLDAEMON"

# if defined( E_DEBUG ) && !defined( SCLDAEMON_NODBG )
#  define SCLDAEMON_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// SoCLe DAEMON

# include "err.h"
# include "flw.h"

# include "csdleo.h"

# include "sclrgstry.h"

# ifndef SCLDAEMON_DISABLE_ERROR_DETECTION
#  ifdef E_DEBUG
#   define SCLDAEMON__ERROR_DETECTION_ENABLED
#  endif
# endif

namespace scldaemon {
	class daemon___
	{
	protected:
		virtual bso::bool__ SCLDAEMONProcess( flw::ioflow__ &Flow ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			// Standardization.
		}
		E_CVDTOR( daemon___ );
		void Init( void )
		{
			// Standardization.
		}
		bso::bool__ Process( flw::ioflow__ &Flow )
		{
			return SCLDAEMONProcess( Flow );
		}
	};

	typedef csdscb::callback__ _callback__;

	class callback__
	: public _callback__
	{
	private:
		virtual void *CSDSCBPreProcess( const char *Origin ) override
		{
			return SCLDAEMONNew( Origin );
		}
		virtual csdscb::action__ CSDSCBProcess(
			flw::ioflow__ &Flow,
			void *UP ) override
		{
			daemon___ &Daemon = *(daemon___ *)UP;

			if ( Daemon.Process( Flow ) )
				return csdscb::aContinue;
			else
				return csdscb::aStop;
		}
		virtual void CSDSCBPostProcess( void *UP ) override
		{
		ERRProlog
		ERRBegin
			delete (daemon___ *)UP;
		ERRErr
# ifndef CPE_POSIX
#  ifdef SCLDAEMON__ERROR_DETECTION_ENABLED
			strcpy( NULL, "Une erreur ne devrait pas se produire ; s'il y en a malgrés tout une, cette ligne permet de la détecter facilement avec le debugger." );	// Lire le contenu du paramètre.
#  endif
# endif
			ERRRst();
		ERREnd
		ERREpilog
		}
	protected:
		virtual daemon___ *SCLDAEMONNew( const char *Origin ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			_callback__::reset( P );
		}
		E_CVDTOR( callback__ );
		void Init( void )
		{
			_callback__::Init();
		}
	};

	/* Called once, when the library is loaded. All the 'registry' stuff is already initialized.
	The same returned callback is used to handle each connection (one callback for all connections,
	and NOT a callbackper connection). */
	callback__ *SCLDAEMONGetCallback(
		csdleo::context__ Context,
		csdleo::mode__ Mode );	// To overload !
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
