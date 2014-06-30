/*
	'sclbackend.h' by Claude SIMON (http://zeusw.org/).

	'sclbackend' is part of the Epeios framework.

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

#ifndef SCLBACKEND__INC
# define SCLBACKEND__INC

# define SCLBACKEND_NAME		"SCLBACKEND"

# if defined( E_DEBUG ) && !defined( SCLBACKEND_NODBG )
#  define SCLBACKEND_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

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
		void *_UP;
	protected:
		virtual bso::bool__ SCLDAEMONProcess( flw::ioflow__ &Flow )
		{
			bso::bool__ Continue = true;
		ERRProlog
		ERRBegin
			Continue = Handle( Flow, _UP, _RequestLogFunctions );
		ERRErr
			if ( ERRType == err::t_Abort )
			{
				ERRRst();
			ERRProlog
				str::string Translation;
				TOL_CBUFFER___ Buffer;
			ERRBegin
				Translation.Init();
				sclerror::GetPendingError( Language(), Translation );

				fblbrq::Report( fblovl::rRequestError, Translation.Convert( Buffer ), Flow );
			ERRErr
			ERREnd
			ERREpilog
			}
		ERREnd
		ERREpilog
			return Continue;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_backend___::reset( P );
			_daemon___::reset( P );
			_RequestLogFunctions.reset( P );
			_VoidFlowDriver.reset( P );
			_UP = NULL;
		}
		E_CVDTOR( backend___ );
		void Init(
			fblbur::mode__ Mode,
			const char *APIVersion,
			const char *ClientOrigin,
			const char *BackendLabel,
			const char *BackendInformations,
			const char *BackendCopyright,
			const char *SoftwareInformations,
			void *UP )
		{
			_backend___::Init( Mode, APIVersion, ClientOrigin, BackendLabel, scllocale::GetLocale(), BackendInformations, BackendCopyright, SoftwareInformations );
			_daemon___::Init();
			_VoidFlowDriver.Init( fdr::tsDisabled );
			_RequestLogFunctions.Init( _VoidFlowDriver );
			_UP = UP;
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
		virtual scldaemon::daemon___ *SCLDAEMONNew(	const char *Origin )
		{
			return SCLBACKNDNew( _Mode, Origin );
		}
		virtual backend___ *SCLBACKNDNew(
			fblbur::mode__ Mode,
			const char *Origin ) = 0;
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

	callback__ *SCLBACKNDNewCallback( fblbur::mode__ Mode );	// A surcharger.
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
