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

// SoCLe Backend

#ifndef SCLB_INC_
# define SCLB_INC_

# define SCLB_NAME		"SCLB"

# if defined( E_DEBUG ) && !defined( SCLB_NODBG )
#  define SCLB_DBG
# endif

# include "scld.h"
# include "sclm.h"
# include "sclr.h"

# include "err.h"
# include "fblbkd.h"

namespace sclb {
	// Pr�d�claration.
	class fCallback;

	typedef fblbkd::backend___	rBackend_;
	typedef scld::cDaemon cDaemon_;

	struct rBackend
	: public rBackend_,
	  public cDaemon_
	{
	private:
		bso::sBool CompatibilityTested_;
		fblbkd::text_log_functions__<> _RequestLogFunctions;
		flx::void_odriver___ _VoidFlowDriver;
		rgstry::multi_layer_registry _Registry;
		rgstry::layer__ _RegistrySetupLayer;
	protected:
		void *FBLBKDUserPointer( void ) override
		{
			return this;
		}
		bso::bool__ SCLDProcess( fdr::rRWDriver *IODriver ) override;
		virtual void *SCLBStuff( void ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			rBackend_::reset( P );
			CompatibilityTested_ = false;
			_RequestLogFunctions.reset( P );
			_VoidFlowDriver.reset( P );
			_Registry.reset( P );
			_RegistrySetupLayer = rgstry::UndefinedLayer;
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
			return SCLBStuff();
		}
		const rgstry::multi_layer_registry_ &Registry( void ) const
		{
			return _Registry;
		}
		rgstry::multi_layer_registry_ &Registry( void )
		{
			return _Registry;
		}
		rgstry::layer__ GetRegistrySetupLayer( void ) const
		{
			return _RegistrySetupLayer;
		}
		void FillSetupRegistryFollowingId( const str::string_ &SetupId )
		{
			sclr::FillWithSetupOfId( _Registry, _RegistrySetupLayer, SetupId );
		}
		void FillSetupRegistryWithContent( const str::string_ &Content )
		{
			sclr::FillWithContent( _Registry, _RegistrySetupLayer, sclm::GetBinPath(), Content );
		}
	};

	typedef scld::rCallback rCallback_;

	class rCallback
	: public rCallback_
	{
	private:
		fblovl::eMode _Mode;
	protected:
		virtual bso::sBool SCLDPluginOverride(
			str::dString &Id,
			str::dString &Arguments,
			csdscb::sTimeout &Timeout ) override
		{
			return SCLBPluginOverride( Id, Arguments, Timeout  );
		}
		virtual scld::cDaemon *SCLDNew( const ntvstr::char__ *Origin ) override
		{
			return SCLBNew( _Mode, Origin );
		}
		virtual bso::sBool SCLBPluginOverride(
			str::dString &Id,
			str::dString &Arguments,
			csdscb::sTimeout &Timeout )
		{
			return false;
		}
		virtual rBackend *SCLBNew(
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

namespace sclb {

	typedef rBackend backend___;

	typedef rCallback callback__;

	/* See 'SCLDAEMONGetCallback(...)' for more details.
	The difference is that all 'registry' stuff are already
	initialized. */
	callback__ *SCLBGetCallback(
		csdleo::context__ Context,
		fblovl::eMode Mode );	// To overload.

}

#endif
