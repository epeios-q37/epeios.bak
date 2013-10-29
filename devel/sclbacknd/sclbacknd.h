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

namespace sclbacknd {

	// Prédéclaration.
	class callback__;

	typedef fblbkd::backend___	_backend___;

	struct backend___ 
	: public _backend___
	{
	private:
		fblbkd::text_log_functions__<> _RequestLogFunctions;
		flx::void_oflow_driver___ _VoidFlowDriver;
	public:
		void reset( bso::bool__ P = true )
		{
			_backend___::reset( P );
			_RequestLogFunctions.reset( P );
			_VoidFlowDriver.reset( P );
		}
		E_CVDTOR( backend___ );
		void Init(
			fblbur::mode__ Mode,
			const char *APIVersion,
			const char *ClientOrigin,
			const char *BackendLabel,
			const lcl::locale_ &Locale,
			const char *BackendInformations,
			const char *BackendCopyright,
			const char *SoftwareInformations )
		{
			_backend___::Init( Mode, APIVersion, ClientOrigin, BackendLabel, Locale, BackendInformations, BackendCopyright, SoftwareInformations );
			_VoidFlowDriver.Init( fdr::tsDisabled );
			_RequestLogFunctions.Init( _VoidFlowDriver );
		}
		friend class sclbacknd::callback__;
	};

	typedef csdscb::callback__ _callback__;

	class callback__
	: public _callback__
	{
	private:
		fblbur::mode__ _Mode;
		const lcl::locale_ *_Locale;
		virtual void *CSDSCBPreProcess( const char *Origin )
		{
			return SCLBACKNDNew( _Mode, *_Locale, Origin );
		}
		virtual csdscb::action__ CSDSCBProcess(
			flw::ioflow__ &Flow,
			void *UP )
		{
			backend___ &Backend = *(backend___ *)UP;

			if ( Backend.Handle( Flow, NULL, Backend._RequestLogFunctions ) )
				return csdscb::aContinue;
			else
				return csdscb::aStop;
		}
		virtual void CSDSCBPostProcess( void *UP )
		{
			delete (backend___ *)UP;
		}
	protected:
		virtual backend___ *SCLBACKNDNew(
			fblbur::mode__ Mode,
			const lcl::locale_ &Locale,
			const char *Origin ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			_callback__::reset( P );

			_Mode = fblbur::m_Undefined;
			_Locale = NULL;
		}
		E_CVDTOR( callback__ );
		void Init(
			fblbur::mode__ Mode,
			const lcl::locale_ &Locale )
		{
			_callback__::Init();

			_Mode = Mode;
			_Locale = &Locale;
		}
	};

	callback__ *SCLBACKNDNew( 
		fblbur::mode__ Mode,
		const lcl::locale_ &Locale );	// A surcharger.


	// A définir par l'utilisateur.
	extern const char *TargetName;
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
