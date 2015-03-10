/*
	'plgn.h' by Claude SIMON (http://zeusw.org/).

	'plgn' is part of the Epeios framework.

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

#ifndef PLGN__INC
# define PLGN__INC

# define PLGN_NAME		"PLGN"

# if defined( E_DEBUG ) && !defined( PLGN_NODBG )
#  define PLGN_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// PLuGiN

# include "plgncore.h"

# include "err.h"
# include "flw.h"

# include "dlbrry.h"
# include "fnm.h"

# include "sclerror.h"

namespace plgn {

	template <typename plugin> class retriever___
	{
	private:
		dlbrry::dynamic_library___ _Library;
		plugin *_Plugin;
		void _ReleasePlugin( void )
		{
			plgncore::release_plugin *Function = dlbrry::GetFunction<plgncore::release_plugin *>( E_STRING( PLGNCORE_RELEASE_PLUGIN_FUNCTION_NAME ), _Library );

			if ( Function == NULL )
				ERRFwk();

			if ( _Plugin == NULL )
				ERRFwk();

			Function( _Plugin );
		}
		bso::bool__ _IsCompatible( void )
		{
			plgncore::plugin_identification *Function = dlbrry::GetFunction<plgncore::plugin_identification *>( E_STRING( PLGNCORE_PLUGIN_IDENTIFICATION_FUNCTION_NAME ), _Library );

			if ( Function == NULL )
				ERRFwk();

			return !strcmp(plugin::Identification(), Function() );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _Plugin != NULL )
					_ReleasePlugin();
			}

			_Library.reset( P );
			_Plugin = NULL;
		}
		E_CDTOR( retriever___ );
		bso::bool__ Init(
			const ntvstr::string___ &PluginNameAndLocation,
			err::handling__ ErrHandling = err::h_Default )
		{
		ERRProlog
			plgncore::data__ Data;
			fnm::name___ Location;
			TOL_CBUFFER___ Buffer;
		ERRBegin
			plgncore::retrieve_plugin *Function = NULL;

			if ( !_Library.Init( PluginNameAndLocation, ErrHandling ) )
				return NULL;

			if ( !_IsCompatible() )
				if ( ErrHandling == err::hThrowException )
					ERRFwk();
				else
					return false;

			Function = dlbrry::GetFunction<plgncore::retrieve_plugin *>( E_STRING( PLGNCORE_RETRIEVE_PLUGIN_FUNCTION_NAME ), _Library );

			if ( Function == NULL )
				ERRFwk();

			Location.Init();
			fnm::GetLocation( PluginNameAndLocation, Location );

			Data.Init( err::ERRError, sclerror::SCLERRORError, Location.UTF8( Buffer ) );

			_Plugin = (plugin *)Function( &Data );

			if ( ( _Plugin == NULL) && ( ErrHandling == err::hThrowException ) )
				ERRFwk();
		ERRErr
		ERREnd
		ERREpilog
			return _Plugin != NULL;
		}
		plugin &Plugin( void )
		{
			if ( _Plugin == NULL )
				ERRFwk();

			return *_Plugin;
		}
	};
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
