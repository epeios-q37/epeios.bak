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

# include "csdleo.h"

# include "fblbkd.h"

namespace sclbacknd {

	struct _data__ 
	{
		fblbkd::backend___ &Backend;
		fblbkd::text_log_functions__<> RequestLogFunctions;
		flx::void_oflow_driver___ VoidFlowDriver;
		_data__( fblbkd::backend___ &Backend )
		: Backend( Backend )
		{}
		void Init( void )
		{
			VoidFlowDriver.Init( fdr::tsDisabled );
			RequestLogFunctions.Init( VoidFlowDriver );
		}
	};

	template <typename backend>	struct data___
	: public _data__
	{
	public:
		backend Backend;
		data___( void )
		: _data__( Backend )
		{}
		void Init( 
			fblbur::mode__ Mode,
			const lcl::locale_ &Locale,
			const char *Origin )
		{
			_data__::Init();
			Backend.Init( Mode, Locale, Origin );
		}
	};

	// A définir par l'utilisateur.
	extern const char *TargetName;

	void *New(
		fblbur::mode__ Mode,
		const lcl::locale_ &Locale,
		const char *Origin );	// A Surcharger.
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
