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

// SoCLe Daemon

#ifndef SCLD_INC_
# define SCLD_INC_

# define SCLD_NAME		"SCLD"

# if defined( E_DEBUG ) && !defined( SCLD_NODBG )
#  define SCLD_DBG
# endif

# include "scli.h"

# include "bso.h"
# include "csdleo.h"
# include "csdscb.h"
# include "err.h"
# include "fdr.h"

# ifndef SCLD_DISABLE_ERROR_DETECTION
#  ifdef E_DEBUG
#   define SCLD_ERROR_DETECTION_ENABLED_
#  endif
# endif

namespace scld {
	class cDaemon
	{
	protected:
		virtual bso::bool__ SCLDProcess( fdr::rRWDriver *IODriver ) = 0;
		// Post processing is made by the destructor.
	public:
		qCALLBACK( Daemon)
		bso::bool__ Process( fdr::rRWDriver *IODriver )
		{
			return SCLDProcess( IODriver );
		}
	};

	typedef csdscb::cProcessing cProcessing_;

	class rCallback
	: public cProcessing_
	{
	protected:
		virtual bso::sBool CSDSCBPluginOverride(
			str::dString &Id,
			str::dString &Arguments,
			csdscb::sTimeout &Timeout ) override
		{
			return SCLDPluginOverride( Id, Arguments, Timeout );
		}
		virtual void *CSDSCBPreProcess(
			fdr::rRWDriver *IODriver,
            const ntvstr::char__ *Origin ) override
		{
			return SCLDNew( Origin );
		}
		virtual csdscb::action__ CSDSCBProcess(
			fdr::rRWDriver *IODriver,
			void *UP ) override
		{
			cDaemon &Daemon = *(cDaemon*)UP;

			if ( Daemon.Process( IODriver ) )
				return csdscb::aContinue;
			else
				return csdscb::aStop;
		}
		virtual bso::sBool CSDSCBPostProcess( void *UP ) override
		{
		qRH
		qRB
			delete (cDaemon*)UP;
		qRR
# ifndef CPE_S_POSIX
#  ifdef SCLD_ERROR_DETECTION_ENABLED_
			strcpy( NULL, "There should no be errors, but, if one occurs, this line should facilitate the debugging." );
#  endif
# endif
			ERRRst();
		qRT
		qRE
			return true;
		}
	protected:
		virtual bso::sBool SCLDPluginOverride(
			str::dString &Id,
			str::dString &Arguments,
			csdscb::sTimeout &Timeout )
		{
			return false;
		}
		virtual cDaemon *SCLDNew( const ntvstr::char__ *Origin ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
		}
		E_CVDTOR( rCallback );
		void Init( void )
		{
		}
	};

	// To define by user.
	const scli::sInfo &SCLDInfo( void );

	/* Called once, when the library is loaded. All the 'registry' stuff is already initialized.
	The same returned callback is used to handle each connection (one callback for all connections,
	and NOT a callback per connection). */
	rCallback *SCLDGetCallback(
		csdleo::context__ Context,
		csdleo::mode__ Mode );	// To overload !
}

#endif
