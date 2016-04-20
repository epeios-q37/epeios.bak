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

#ifndef SCLBACKND__INC
# define SCLBACKND__INC

# define SCLBACKND_NAME		"SCLBACKND"

# if defined( E_DEBUG ) && !defined( SCLBACKND_NODBG )
#  define SCLBACKND_DBG
# endif

// SoCLe BACKEND

# include "err.h"
# include "flw.h"

# include "csdscb.h"

# include "fblbkd.h"
# include "fblbur.h"

# include "lcl.h"

# include "scldaemon.h"
# include "scllocale.h"
# include "sclerror.h"

/*************************/
/****** New version ******/
/*************************/

namespace sclbacknd {
	// Prédéclaration.
	class fCallback;

	typedef fblbkd::backend___	rBackend_;
	typedef scldaemon::daemon___ rDaemon_;

	struct rBackend 
	: public rBackend_,
	  public rDaemon_
	{
	private:
		fblbkd::text_log_functions__<> _RequestLogFunctions;
		flx::void_oflow_driver___ _VoidFlowDriver;
		rgstry::multi_level_registry _Registry;
		rgstry::level__ _RegistrySetupLevel;
		void *_UP;
	protected:
		virtual bso::bool__ SCLDAEMONProcess( flw::ioflow__ &Flow ) override;
	public:
		void reset( bso::bool__ P = true )
		{
			rBackend_::reset( P );
			rDaemon_::reset( P );
			_RequestLogFunctions.reset( P );
			_VoidFlowDriver.reset( P );
			_Registry.reset( P );
			_RegistrySetupLevel = rgstry::UndefinedLevel;
			_UP = NULL;
		}
		E_CVDTOR( rBackend );
		void Init(
			fblbur::mode__ Mode,
			const char *APIVersion,
			const ntvstr::char__ *ClientOrigin,
			const char *BackendLabel,
			const char *BackendInformations,
			const char *BackendCopyright,
			const char *SoftwareInformations,
			void *UP );
		const rgstry::multi_level_registry_ &Registry( void ) const
		{
			return _Registry;
		}
		rgstry::multi_level_registry_ &Registry( void )
		{
			return _Registry;
		}
		rgstry::level__ GetRegistrySetupLevel( void ) const
		{
			return _RegistrySetupLevel;
		}
		void FillSetupRegistryFollowingId( const str::string_ &SetupId )
		{
			sclrgstry::FillWithSetupOfId( _Registry, _RegistrySetupLevel, SetupId );
		}
		void FillSetupRegistryWithContent( const str::string_ &Content )
		{
			sclrgstry::FillWithContent( _Registry, _RegistrySetupLevel, Content );
		}
		void *UP( void ) const
		{
			return _UP;
		}
	};

	typedef scldaemon::callback___ rCallback_;

	class rCallback
	: public rCallback_
	{
	private:
		fblbur::mode__ _Mode;
	protected:
		virtual bso::sBool SCLDAEMONPluginOverride(
			str::dString &Id,
			str::dString &Arguments ) override
		{
			return SCLBACKNDPluginOverride( Id, Arguments );
		}
		virtual scldaemon::daemon___ *SCLDAEMONNew(	const ntvstr::char__ *Origin ) override
		{
			return SCLBACKNDNew( _Mode, Origin );
		}
		virtual bso::sBool SCLBACKNDPluginOverride(
			str::dString &Id,
			str::dString &Arguments )
		{
			return false;
		}
		virtual rBackend *SCLBACKNDNew(
			fblbur::mode__ Mode,
			const ntvstr::char__ *Origin ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			rCallback_::reset( P );

			_Mode = fblbur::m_Undefined;
		}
		E_CVDTOR( rCallback );
		void Init( fblbur::mode__ Mode )
		{
			scldaemon::mode__ DaemonMode = scldaemon::m_Undefined;

			switch ( Mode ) {
			case fblbur::mNone:
				qRFwk();
				break;
			case fblbur::mEmbedded:
				DaemonMode = scldaemon::mBasic;
				break;
			case fblbur::mRemote:
				DaemonMode = scldaemon::mMuxed;
				break;
			default:
				qRFwk();
				break;
			}

			rCallback_::Init( DaemonMode );

			_Mode = Mode;
		}
	};


}

/*************************/
/****** Old version ******/
/*************************/

namespace sclbacknd {

	typedef rBackend backend___;

	typedef rCallback callback__;

	/* See 'SCLDAEMONGetCallback(...)' for more details.
	The difference is that all 'registry' stuff are already
	initialized. */
	callback__ *SCLBACKNDGetCallback(
		csdleo::context__ Context,
		fblbur::mode__ Mode );	// To overload.
}

#endif
