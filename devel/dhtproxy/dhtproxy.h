/*
	'dhtproxy.h' by Claude SIMON (http://zeusw.org/).

	'dhtproxy' is part of the Epeios framework.

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

#ifndef DHTPROXY__INC
# define DHTPROXY__INC

# define DHTPROXY_NAME		"DHTPROXY"

# if defined( E_DEBUG ) && !defined( DHTPROXY_NODBG )
#  define DHTPROXY_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// DHT(ML) PROXY

# include "xhtcllbk.h"

# include "err.h"
# include "flw.h"
# include "dlbrry.h"

namespace dhtproxy {
    class proxy___
    {
    private:
		dlbrry::dynamic_library___ _Library;
        xhtcllbk::callback__ *_Callback;
		xhtcllbk::proxy_callback__ *_ProxyCallback;
		xhtcllbk::callback__ &_C( void ) const
		{
			if ( _Callback == NULL )
				ERRFwk();

			return *_Callback;
		}
		xhtcllbk::proxy_callback__ &_PC( void ) const
		{
			if ( _ProxyCallback == NULL )
				ERRFwk();

			return *_ProxyCallback;
		}
	protected:
		virtual xhtcllbk::proxy_callback__ &DHTPROXYGetProxyCallback( void ) = 0;
    public:
        void reset( bso::bool__ P = true )
        {
			_Library.reset( P );
			_Callback = NULL;
			_ProxyCallback = NULL;
        }
        E_CDTOR( proxy___ );
		bso::bool__ Init(
			const str::string_ &ModuleFileName,
			const char *Identification );	// 'ProxyCallback' is returned.
		xhtcllbk::session_callback__ *GetNewSession( void ) const
		{
			return _C().New( _PC() );
		}
		const char *GetBaseLanguage( TOL_CBUFFER___ &Buffer ) const
		{
			return _C().BaseLanguage( Buffer );
		}
	};

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
