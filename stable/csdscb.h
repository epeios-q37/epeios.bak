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

#ifndef CSDSCB__INC
# define CSDSCB__INC

# define CSDSCB_NAME		"CSDSCB"

# if defined( E_DEBUG ) && !defined( CSDSCB_NODBG )
#  define CSDSCB_DBG
# endif

// Client-Server Devices Server CallBack

# include "err.h"
# include "flw.h"
# include "tol.h"
# include "ntvstr.h"

/*************/
/**** NEW ****/
/*************/

namespace csdscb {
	qENUM( Action ) {
		aContinue,
		aStop,
		a_amount,
		a_Undefined
	};

	class cProcessing {
	protected:
		virtual void *CSDSCBPreProcess( const ntvstr::char__ *Origin ) = 0;
		virtual eAction CSDSCBProcess(
			flw::ioflow__ &Flow,
			void *UP ) = 0;
		virtual void CSDSCBPostProcess( void *UP ) = 0;
	public:
		void *PreProcess( const ntvstr::char__ *Origin )
		{
			return CSDSCBPreProcess( Origin );
		}
		eAction Process(
			flw::ioflow__ &Flow,
			void *UP )
		{
			return CSDSCBProcess( Flow, UP );
		}
		void PostProcess( void *UP )
		{
			CSDSCBPostProcess( UP );
		}
	};
}

/*************/
/**** OLD ****/
/*************/

namespace csdscb {
	typedef eAction action__;

	typedef cProcessing callback__;
}

#endif
