/*
	'wkcllbck.h' by Claude SIMON (http://zeusw.org/).

	'wkcllbck' is part of the Epeios framework.

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

#ifndef WKCLLBCK__INC
# define WKCLLBCK__INC

# define WKCLLBCK_NAME		"WKCLLBCK"

# if defined( E_DEBUG ) && !defined( WKCLLBCK_NODBG )
#  define WKCLLBCK_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// WebKit CaLLBaCKs

# include "err.h"
# include "flw.h"
# include "str.h"

# define WKCLLBCK_SHARED_DATA_VERSION_NUMBER	"1"

# define WKCLLBCK_SHARED_DATA_VERSION	WKCLLBCK_SHARED_DATA_VERSION_NUMBER "-" CPE_ARCHITECTURE_LABEL


# define WKCLLBCK_LAUNCH_FUNCTION_NAME		WKCLLBCKLaunch

namespace wkcllbck {
	class callback__ {
	protected:
		virtual void WKCLLBCKSetXML( const str::string_ &XML ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standadisation.
		}
		E_CVDTOR( callback__ );
		void Init( void )
		{
			// Standadisation.
		}
		void SetXML( const str::string_ &XML )
		{
			WKCLLBCKSetXML( XML );
		}
	};

#pragma pack( push, 1)
		// NOTA : Si modifié, modifier 'CSDLEO_SHARED_DATA_VERSION' !
	class shared_data__
	{
	private:
		const char *_Version;	// Toujours en première position.
		bso::uint__ _Control;	// Une valeur relative au contenu de la structure, à des fins de test primaire de compatibilité.
		callback__ *_Callback;
		void *_UP;				// A la discrétion de l'utilisateur.
	public:
		void reset( bso::bool__ = true )
		{
			_Version = NULL;
			_Control = 0;
		}
		E_CDTOR( shared_data__ );
		void Init(
			callback__ &Callback,
			void *UP =  NULL )
		{
			_Version = WKCLLBCK_SHARED_DATA_VERSION;
			_Control = ControlComputing();
			_UP = UP;
			_Callback = &Callback;
		}
		size_t ControlComputing( void )
		{
			return sizeof( fdr::oflow_driver___<> );
		}
		callback__ &Callback( void ) const
		{
			if ( _Callback == NULL )
				ERRFwk();

			return *_Callback;
		}
	};

#pragma pack( pop )

	typedef void (launch)( const shared_data__ &Data );
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
