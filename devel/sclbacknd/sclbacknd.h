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

#ifndef SCLBACKEND__INC
# define SCLBACKEND__INC

# define SCLBACKEND_NAME		"SCLBACKEND"

# if defined( E_DEBUG ) && !defined( SCLBACKEND_NODBG )
#  define SCLBACKEND_DBG
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

namespace sclbacknd {

	// Prédéclaration.
	class callback__;

	typedef fblbkd::backend___	_backend___;
	typedef scldaemon::daemon___ _daemon___;

	struct backend___ 
	: public _backend___,
	  public _daemon___
	{
	private:
		fblbkd::text_log_functions__<> _RequestLogFunctions;
		flx::void_oflow_driver___ _VoidFlowDriver;
		rgstry::multi_level_registry _Registry;
		rgstry::level__ _RegistrySetupLevel;
		void *_UP;
	protected:
		virtual bso::bool__ SCLDAEMONProcess( flw::ioflow__ &Flow ) override
		{
			bso::bool__ Continue = true;
		qRH
		qRB
			Continue = Handle( Flow, _UP, _RequestLogFunctions );
		qRR
			if ( ERRType == err::t_Abort )
			{
				ERRRst();
			qRH
				str::string Translation;
				TOL_CBUFFER___ Buffer;
			qRB
				Translation.Init();
				sclerror::GetPendingErrorTranslation( Language(), Translation );

				sclerror::ResetPendingError();

				fblbrq::Report( fblovl::rRequestError, Translation.Convert( Buffer ), Flow );
			qRR
			qRT
			qRE
			}
		qRT
		qRE
			return Continue;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_backend___::reset( P );
			_daemon___::reset( P );
			_RequestLogFunctions.reset( P );
			_VoidFlowDriver.reset( P );
			_Registry.reset( P );
			_RegistrySetupLevel = rgstry::UndefinedLevel;
			_UP = NULL;
		}
		E_CVDTOR( backend___ );
		void Init(
			fblbur::mode__ Mode,
			const char *APIVersion,
			const ntvstr::char__ *ClientOrigin,
			const char *BackendLabel,
			const char *BackendInformations,
			const char *BackendCopyright,
			const char *SoftwareInformations,
			void *UP )
		{
			_backend___::Init( Mode, APIVersion, ClientOrigin, BackendLabel, scllocale::GetLocale(), BackendInformations, BackendCopyright, SoftwareInformations );
			_daemon___::Init();
			_VoidFlowDriver.Init( fdr::tsDisabled, flx::aAllowed );
			_RequestLogFunctions.Init( _VoidFlowDriver );
			_Registry.Init();
			_Registry.Push( sclrgstry::GetCommonRegistry() );
			_RegistrySetupLevel = _Registry.CreateEmbedded( str::string( "Setup" ) );
			_UP = UP;
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
		void FillSetupRegistry( const str::string_ &SetupId )
		{
			sclrgstry::FillSetupRegistry( _Registry, _RegistrySetupLevel, SetupId );
		}
		void *UP( void ) const
		{
			return _UP;
		}
	};

	typedef scldaemon::callback__ _callback__;

	class callback__
	: public _callback__
	{
	private:
		fblbur::mode__ _Mode;
	protected:
		virtual scldaemon::daemon___ *SCLDAEMONNew(	const ntvstr::char__ *Origin ) override
		{
			return SCLBACKNDNew( _Mode, Origin );
		}
		virtual backend___ *SCLBACKNDNew(
			fblbur::mode__ Mode,
			const ntvstr::char__ *Origin ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			_callback__::reset( P );

			_Mode = fblbur::m_Undefined;
		}
		E_CVDTOR( callback__ );
		void Init( fblbur::mode__ Mode )
		{
			_callback__::Init();

			_Mode = Mode;
		}
	};


	/* See 'SCLDAEMONGetCallback(...)' for more details.
	The difference is that all 'registry' stuff are already
	initialized. */
	callback__ *SCLBACKNDGetCallback(
		csdleo::context__ Context,
		fblbur::mode__ Mode );	// To overload.
}

#endif
