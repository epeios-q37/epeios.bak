/*
	'csdscb.h' by Claude SIMON (http://zeusw.org/).

	'csdscb' is part of the Epeios framework.

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

#ifndef CSDSCB__INC
# define CSDSCB__INC

# define CSDSCB_NAME		"CSDSCB"

# if defined( E_DEBUG ) && !defined( CSDSCB_NODBG )
#  define CSDSCB_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// Client-Server Devices Server CallBack

# include "err.h"
# include "flw.h"
# include "tol.h"

namespace csdscb {
	enum action__ {
		aContinue,
		aStop,
		a_amount,
		a_Undefined
	};

	class callback__ {
	protected:
		virtual void *CSDSCBPreProcess( const char *Origin ) = 0;
		virtual action__ CSDSCBProcess(
			flw::ioflow__ &Flow,
			void *UP ) = 0;
		virtual void CSDSCBPostProcess( void *UP ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( callback__ );
		void *PreProcess( const char *Origin )
		{
			return CSDSCBPreProcess( Origin );
		}
		action__ Process(
			flw::ioflow__ &Flow,
			void *UP )
		{
			return CSDSCBProcess( Flow, UP );
		}
		void PostProcess( void *UP )
		{
			CSDSCBPostProcess( UP );
		}
		void Init( void )
		{
			// Standadisation.
		}
	};

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
