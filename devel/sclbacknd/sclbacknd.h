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
// # include "fblbur.h"

# include "lcl.h"

# include "scldaemon.h"
# include "scllocale.h"
# include "sclerror.h"
# include "sclmisc.h"

/*************************/
/****** New version ******/
/*************************/

namespace sclbacknd {
	// Prédéclaration.
	class fCallback;

	typedef fblbkd::backend___	rBackend_;
	typedef scldaemon::cDaemon cDaemon_;

	struct rBackend 
	: public rBackend_,
	  public cDaemon_
	{
	private:
		bso::sBool CompatibilityTested_;
		fblbkd::text_log_functions__<> _RequestLogFunctions;
		flx::void_odriver___ _VoidFlowDriver;
		rgstry::multi_level_registry _Registry;
		rgstry::level__ _RegistrySetupLevel;
	protected:
		void *FBLBKDUserPointer( void ) override
		{
			return this;
		}
		bso::bool__ SCLDAEMONProcess( fdr::rRWDriver *IODriver ) override;
		virtual void *SCLBACKNDStuff( void ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			rBackend_::reset( P );
			CompatibilityTested_ = false;
			_RequestLogFunctions.reset( P );
			_VoidFlowDriver.reset( P );
			_Registry.reset( P );
			_RegistrySetupLevel = rgstry::UndefinedLevel;
		}
		E_CVDTOR( rBackend );
		void Init(
			fblovl::eMode Mode,
			const char *APIVersion,
			const ntvstr::char__ *ClientOrigin,
			const char *BackendLabel,
			const char *BackendInformations,
			const char *BackendCopyright,
			const char *SoftwareInformations );
		void *Stuff( void )
		{
			return SCLBACKNDStuff();
		}
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
			sclrgstry::FillWithContent( _Registry, _RegistrySetupLevel, sclmisc::GetBinPath(), Content );
		}
	};

	typedef scldaemon::rCallback rCallback_;

	class rCallback
	: public rCallback_
	{
	private:
		fblovl::eMode _Mode;
	protected:
		virtual bso::sBool SCLDAEMONPluginOverride(
			str::dString &Id,
			str::dString &Arguments,
			csdscb::sTimeout &Timeout ) override
		{
			return SCLBACKNDPluginOverride( Id, Arguments, Timeout  );
		}
		virtual scldaemon::cDaemon *SCLDAEMONNew( const ntvstr::char__ *Origin ) override
		{
			return SCLBACKNDNew( _Mode, Origin );
		}
		virtual bso::sBool SCLBACKNDPluginOverride(
			str::dString &Id,
			str::dString &Arguments,
			csdscb::sTimeout &Timeout )
		{
			return false;
		}
		virtual rBackend *SCLBACKNDNew(
			fblovl::eMode Mode,
			const ntvstr::char__ *Origin ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			rCallback_::reset( P );

			_Mode = fblovl::m_Undefined;
		}
		E_CVDTOR( rCallback );
		void Init( fblovl::eMode Mode )
		{
			switch ( Mode ) {
			case fblovl::mNone:
				qRFwk();
				break;
			case fblovl::mReferenced:
				break;
			case fblovl::mSerialized:
				break;
			default:
				qRFwk();
				break;
			}

			rCallback_::Init();

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
		fblovl::eMode Mode );	// To overload.
}

#endif
