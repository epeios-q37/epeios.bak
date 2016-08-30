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

	typedef bso::sU16 sTimeout;	// Timeout duration in seconds.
	qCDEF( sTimeout, NoTimeout, 0 );

	class cProcessing {
	protected:
		// If returned value == 'true', the plugin with given 'Id' and 'Arguments' overrides the one defined in the caller's registry (see 'dmnzq').
		virtual bso::sBool CSDSCBPluginOverride(
			str::dString &Id,
			str::dString &Arguments,
			sTimeout &Timeout )
		{
			return false;
		}
		virtual void *CSDSCBPreProcess(
			flw::ioflow__ *Flow,
			const ntvstr::char__ *Origin,
			bso::sBool *OwnerShipTaken ) = 0;	// If set to true, it means that the 'Flow' will be destructed downstream.
		virtual eAction CSDSCBProcess(
			flw::ioflow__ *Flow,
			void *UP ) = 0;
		virtual void CSDSCBPostProcess( void *UP ) = 0;
	public:
		qCALLBACK( Processing );
		bso::sBool PluginOverride(
			str::dString &Id,
			str::dString &Arguments,
			sTimeout &Timeout )
		{
			return CSDSCBPluginOverride( Id, Arguments, Timeout );
		}
		void *PreProcess(
			flw::ioflow__ *Flow,
			const ntvstr::char__ *Origin,
			bso::sBool *OwnerShipTaken )	// If set to true, it means that the 'Flow' will be destructed downstream.
		{
			return CSDSCBPreProcess( Flow, Origin, OwnerShipTaken );
		}
		eAction Process(
			flw::ioflow__ *Flow,
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
