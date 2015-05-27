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

#ifndef CSDSUF__INC
#define CSDSUF__INC

#define CSDSUF_NAME		"CSDSUF"

#if defined( E_DEBUG ) && !defined( CSDSUF_NODBG )
#define CSDSUF_DBG
#endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// Client-Server Devices Server User Functions

# error "Obsolete ! Use 'CSDSCB' instead !"

# include "err.h"
# include "flw.h"
# include "tol.h"

namespace csdsuf {
	enum action__ {
		aContinue,
		aStop,
		a_amount,
		a_Undefined
	};

	class user_functions__ {
	protected:
		virtual void *CSDSUFPreProcess( const char *Origin ) = 0;
		virtual action__ CSDSUFProcess(
			flw::ioflow__ &Flow,
			void *UP ) = 0;
		virtual void CSDSUFPostProcess( void *UP ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( user_functions__ );
		void *PreProcess( const char *Origin )
		{
			return CSDSUFPreProcess( Origin );
		}
		action__ Process(
			flw::ioflow__ &Flow,
			void *UP )
		{
			return CSDSUFProcess( Flow, UP );
		}
		void PostProcess( void *UP )
		{
			CSDSUFPostProcess( UP );
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
