/*
	'scldaemon.h' by Claude SIMON (http://zeusw.org/).

	'scldaemon' is part of the Epeios framework.

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

namespace scldaemon {
	class daemon___
	{
	private:
		rgstry::multi_level_registry _Registry;
	protected:
		virtual bso::bool__ SCLDAEMONProcess( flw::ioflow__ &Flow ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			_Registry.reset( P );
		}
		E_CVDTOR( daemon___ );
		void Init( void )
		{
			_Registry.Init();
			_Registry.Push( sclrgstry::GetRegistry() );
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
		virtual void *CSDSCBPreProcess( const char *Origin )
		{
			return SCLDAEMONNew( Origin );
		}
		virtual csdscb::action__ CSDSCBProcess(
			flw::ioflow__ &Flow,
			void *UP )
		{
			daemon___ &Daemon = *(daemon___ *)UP;

			if ( Daemon.Process( Flow ) )
				return csdscb::aContinue;
			else
				return csdscb::aStop;
		}
		virtual void CSDSCBPostProcess( void *UP )
		{
			delete (daemon___ *)UP;
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


	// A définir par l'utilisateur.
	callback__ *SCLDAEMONNewCallback( csdleo::mode__ Mode );	// To overload !

	void DisplayModuleClosingMessage( void );

	void DisplayModuleClosedMessage( void );
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
