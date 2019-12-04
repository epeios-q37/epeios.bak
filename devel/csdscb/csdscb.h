/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#ifndef CSDSCB_INC_
# define CSDSCB_INC_

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
			fdr::rRWDriver *RWDriver,
			const ntvstr::char__ *Origin ) = 0;
		virtual eAction CSDSCBProcess(
			fdr::rRWDriver *RWDriver,
			void *UP ) = 0;
		// If the returned value is 'true', the underlying socket will be closed.
		// Useful when the socket reading and writing are not handled by the same thread, so the other thread may not be wait indefinitely ( used in 'prxyq').
		virtual bso::sBool CSDSCBPostProcess( void *UP ) = 0;
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
			fdr::rRWDriver *RWDriver,
			const ntvstr::char__ *Origin )
		{
			return CSDSCBPreProcess( RWDriver, Origin );
		}
		eAction Process(
			fdr::rRWDriver *RWDriver,
			void *UP )
		{
			return CSDSCBProcess( RWDriver, UP );
		}
		// If the returned value is 'true', the underlying socket has to be closed upstream.
		bso::sBool PostProcess( void *UP )
		{
			return CSDSCBPostProcess( UP );
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
