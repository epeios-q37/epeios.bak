/*
	'sclplugin.h' by Claude SIMON (http://zeusw.org/).

	'sclplugin' is part of the Epeios framework.

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

#ifndef SCLPLUGIN__INC
# define SCLPLUGIN__INC

# define SCLPLUGIN_NAME		"SCLPLUGIN"

# if defined( E_DEBUG ) && !defined( SCLPLUGIN_NODBG )
#  define SCLPLUGIN_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// SoCLe PLUGIN

# include "plgncore.h"

# include "err.h"
# include "flw.h"

namespace sclplugin {
	typedef plgncore::callback__ _callback__;

	class callback__
	: public _callback__
	{
	protected:
		virtual void PLGNCOREInitialize(
			const plgncore::data__ *Data,
			... /* Autres paramtres. Le dernier doit tre = 'NULL' */ ) override;
		virtual void *PLGNCORERetrievePlugin( void ) override;
		virtual void PLGNCOREReleasePlugin( void *Plugin ) override;
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

	// Fonctions  surcharger, (la macro ci-dessous le fait).
	const char *SCLPLUGINPluginIdentification( void );
	void *SCLPLUGINRetrievePlugin( void );
	void SCLPLUGINReleasePlugin( void * );
}

# define SCLPLUGIN_DEF( plugin )\
	const char *sclplugin::SCLPLUGINPluginIdentification( void )\
	{\
		return plugin::Identification();\
	}\
	void *sclplugin::SCLPLUGINRetrievePlugin( void )\
	{\
		plugin *Plugin = new plugin;\
		\
		if ( Plugin == NULL )\
			ERRAlc();\
		\
		return Plugin;\
	}\
\
	void sclplugin::SCLPLUGINReleasePlugin( void *Plugin )\
	{\
		if ( Plugin == NULL )\
			ERRFwk();\
		\
		delete (plugin *)Plugin;\
	}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
